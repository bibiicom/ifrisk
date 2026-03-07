#!/bin/bash

# 如何恢复由于sinkdb2异常停止导致的快照数据库缺失
#### 重启sinkdb2
#start.sh sinkdb 2
#### 等待trade在sinkdb2启动之后的第一个快照完成之后
#这个目的是将次新的快照更新到数据库，但不会替换最新的结果
#./dbrecover.sh
#### 等待trade在sinkdb2启动之后的第二个快照完成之后
#这个目的是为了将次新快照和之前遗留的需要删除的数据删除
#./dbclear.sh

# 数据库连接信息
dbname=`grep 'dbname=' network.ini | grep -v '#' | awk -F = '{print $2}'`
dbuser=`grep 'dbuser=' network.ini | grep -v '#' | awk -F = '{print $2}'`
dbpasswd=`grep 'dbpasswd=' network.ini | awk -F = '{print $2}'`
ipall=${dbname##MYSQL://}
ipport=${ipall%@*}
database=${ipall#*@}
ip=${ipport%:*}
port=${ipport#*:}

last=$(cat ./dumpPhase.log)
lastsnap="./snap/snap.$last"
rm -rf snap.*
mkdir snap.$last
echo "from $lastsnap"
for INPUT_FILE in "$lastsnap"/*; do
    if [[ ! $(basename "$INPUT_FILE") =~ (LastKLine|UserSession|MemberPosition|CurrentTime) ]]; then
        TABLE_NAME="perpetual.t_$(basename "$INPUT_FILE" .csv)"
        OUTPUT_FILE="./snap.$last/$(basename "$INPUT_FILE" .csv).sql"
        > "$OUTPUT_FILE"

        # 读取 CSV 列名并提取主键
        read -r COLUMNS_RAW < "$INPUT_FILE"
        COLUMNS_RAW="${COLUMNS_RAW//\"}"
        IFS=',' read -ra RAW_COL_ARRAY <<< "$COLUMNS_RAW"

        PRIMARY_KEYS=()
        CLEANED_COLUMNS=()
        for raw_col in "${RAW_COL_ARRAY[@]}"; do
            if [[ "$raw_col" == *"*"* ]]; then
                clean_col="${raw_col//\*}"
                PRIMARY_KEYS+=("$clean_col")
                CLEANED_COLUMNS+=("$clean_col")
            else
                CLEANED_COLUMNS+=("$raw_col")
            fi
        done

        COLUMNS=$(IFS=','; echo "${CLEANED_COLUMNS[*]}")

        i=0
		insert=0
		update=0
        while IFS= read -r LINE; do
            if [ -z "$LINE" ] || [ $i -eq 0 ]; then
                ((i++))
                continue
            fi

            CLEANED_LINE="${LINE//\"}"
            IFS=',' read -ra COL_ARRAY <<< "$COLUMNS"
            IFS=',' read -ra VAL_ARRAY <<< "$CLEANED_LINE"

            # 提取主键值和 UpdateTime 值
            PRIMARY_VALUES=()
            NEW_UPDATE_TIME=""
            for ((j = 0; j < ${#COL_ARRAY[@]}; j++)); do
                col="${COL_ARRAY[$j]}"
                val="${VAL_ARRAY[$j]}"
                if [[ " ${PRIMARY_KEYS[@]} " =~ " $col " ]]; then
                    PRIMARY_VALUES+=("$col='$val'")  # 主键条件格式：col='val'
                fi
                if [[ "$col" == "UpdateTime" ]]; then
                    NEW_UPDATE_TIME="$val"
                fi
            done

            WHERE_CLAUSE=""
            for ((k = 0; k < ${#PRIMARY_VALUES[@]}; k++)); do
                if [ $k -eq 0 ]; then
                    WHERE_CLAUSE="${PRIMARY_VALUES[$k]}"
                else
                    WHERE_CLAUSE="$WHERE_CLAUSE AND ${PRIMARY_VALUES[$k]}"
                fi
            done

            QUERY_SQL="SELECT UpdateTime FROM $TABLE_NAME WHERE $WHERE_CLAUSE;" 
            OLD_UPDATE_TIME=$(mysql -h$ip -P$port -u$dbuser -p$dbpasswd -D$database -NBe "$QUERY_SQL" 2>/dev/null)

            # 判断是否需要插入
            if [[ -z "$OLD_UPDATE_TIME" ]]; then
                # 生成 INSERT 语句
                NEW_COLUMNS=()
                NEW_VALUES=()
                for ((j = 0; j < ${#COL_ARRAY[@]}; j++)); do
                    if [ -n "${VAL_ARRAY[$j]}" ]; then
                        NEW_COLUMNS+=("${COL_ARRAY[$j]}")
                        NEW_VALUES+=("'${VAL_ARRAY[$j]}'")
                    fi
                done

                NEW_COLUMNS_SQL=$(IFS=", "; echo "${NEW_COLUMNS[*]}")
                NEW_VALUES_SQL=$(IFS=", "; echo "${NEW_VALUES[*]}")

                SQL_STATEMENT="INSERT INTO $TABLE_NAME ($NEW_COLUMNS_SQL) VALUES ($NEW_VALUES_SQL);"
                ((insert++))
				echo "$SQL_STATEMENT" >> "$OUTPUT_FILE"
            else
                if [[ "$NEW_UPDATE_TIME" -gt "$OLD_UPDATE_TIME" ]]; then
                    # 生成 UPDATE 语句
                    SET_CLAUSE_PARTS=()
                    for ((j = 0; j < ${#COL_ARRAY[@]}; j++)); do
                        if [ -n "${VAL_ARRAY[$j]}" ]; then
                            col="${COL_ARRAY[$j]}"
                            val="'${VAL_ARRAY[$j]}'"
                            SET_CLAUSE_PARTS+=("$col=$val")
                        fi
                    done
                    SET_CLAUSE=$(IFS=", "; echo "${SET_CLAUSE_PARTS[*]}")

                    SQL_STATEMENT="UPDATE $TABLE_NAME SET $SET_CLAUSE WHERE $WHERE_CLAUSE AND UpdateTime=$OLD_UPDATE_TIME;"
                    ((update++))
					echo "$SQL_STATEMENT" >> "$OUTPUT_FILE"
                fi
            fi
        done < "$INPUT_FILE"
        echo "SQL 文件已生成: $OUTPUT_FILE insert:$insert,update:$update" 
    fi
done

echo "gen sql ok, next to run sql"

for file in ./snap.$last/*; do
    if [[ ! $(basename "$file") =~ (CurrentTime) ]]; then
        echo "run $file"
        mysql -h$ip -P$port -u$dbuser -p$dbpasswd -D$database < $file
    fi
done