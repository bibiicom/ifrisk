#!/bin/bash

# 从 network.ini 提取数据库信息（保持不变）
dbname=$(grep 'dbname=' network.ini | grep -v '#' | awk -F = '{print $2}')
dbuser=$(grep 'dbuser=' network.ini | grep -v '#' | awk -F = '{print $2}')
dbpasswd=$(grep 'dbpasswd=' network.ini | awk -F = '{print $2}')

# 解析连接信息（保持不变）
ipall=${dbname##MYSQL://}
ipport=${ipall%@*}
database=${ipall#*@}
ip=${ipport%:*}
port=${ipport#*:}

# 获取最新快照编号
last=$(cat ./dumpPhase.log)
lastsnap="./snap/snap.$last"
# 创建工作目录
rm -rf snap.$last snap.db
mkdir snap.$last snap.db

cp $lastsnap/CurrentTime.csv ./snap.$last/
# 从CurrentTime.csv获取CurrTime赋值为UpdateTime
UpdateTime=$(awk -F, 'NR==1{for(i=1;i<=NF;i++){gsub(/"/,"",$i);if($i=="UpdateTime")col=i}} NR==2{gsub(/"/,"",$col);print $col}' snap.$last/CurrentTime.csv)
echo "UpdateTime:$UpdateTime"

# 主处理循环（结构不变）
for file in Order Position Account PositionOrder TriggerOrder Account Fee MarginRate ServiceConfig; do
    TABLE_NAME="t_$file"
	cp $lastsnap/$file.csv ./snap.$last/
    columns=()
    while IFS= read -r col; do
        columns+=("$col")
    done < <(mysql -h"$ip" -P"$port" -u"$dbuser" -p"$dbpasswd" -D"$database" -N -e "
        SET SESSION group_concat_max_len = 1000000;
        SELECT COLUMN_NAME 
        FROM INFORMATION_SCHEMA.COLUMNS 
        WHERE TABLE_NAME = '$TABLE_NAME'
          AND TABLE_SCHEMA = '$database'
        ORDER BY ORDINAL_POSITION")

    # 构造带反引号的列名列表，避免关键字冲突
    select_cols=$(printf '`%s`,' "${columns[@]}" | sed 's/,$//')

    # 生成CSV文件头
    header=$(printf '"%s",' "${columns[@]}" | sed 's/,$//')
    echo "$header" > ./snap.db/$file.csv

    # 导出数据，显式指定列顺序（表名也添加反引号）
    mysql -h"$ip" -P"$port" -u"$dbuser" -p"$dbpasswd" -D"$database" --quick --batch --skip-column-names -e "SELECT $select_cols FROM $TABLE_NAME" | sed 's/\t/","/g;s/^/"/;s/$/"/;s/\r//g' >> ./snap.db/$file.csv
    
    # 获取所有主键列（关键修改点）
    PRIMARY_KEYS=($(mysql -h"$ip" -P"$port" -u"$dbuser" -p"$dbpasswd" -D"$database" -N -e "SHOW KEYS FROM $TABLE_NAME WHERE Key_name = 'PRIMARY'" | awk '{print $5}'))
    	
    # 定位CSV列位置（支持多列）
    declare -A key_col_map
    header=$(head -1 ./snap.db/$file.csv | sed 's/"//g' | tr ',' ' ')
    col_index=1
    for col in $header; do
        for key in "${PRIMARY_KEYS[@]}"; do
            if [ "$col" == "$key" ]; then
                key_col_map["$key"]=$col_index
            fi
        done
        ((col_index++))
    done

    # 生成主键列表（多列合并）
    key_cols=$(printf "%s," "${PRIMARY_KEYS[@]}" | sed 's/,$//')
    awk_script='BEGIN{FS=OFS=","} {
        gsub(/"/, "", $0)
        key=""
        for (i=0; i<'"${#PRIMARY_KEYS[@]}"'; i++) {
            col='"${key_col_map[${PRIMARY_KEYS[0]}]}"'+i
            key=(key==""?"":key",") $col
        }
        print key
    }'

    # 生成旧数据主键列表
    tail -n +2 ./snap.db/$file.csv | awk "$awk_script" | sort > snap.db/${file}_keys.txt

    # 生成新数据主键列表
    tail -n +2 ./snap.$last/$file.csv | awk "$awk_script" | sort > ./snap.$last/${file}_keys.txt

    # 找出需删除的主键（保持不变）
    comm -23 ./snap.db/${file}_keys.txt ./snap.$last/${file}_keys.txt > snap.db/${file}_keys_to_delete.txt

    if [ -s snap.db/${file}_keys_to_delete.txt ]; then
        BATCH_SIZE=1000
        TOTAL_KEYS=$(wc -l < snap.db/${file}_keys_to_delete.txt)
        PROCESSED=0

        # 生成安全删除语句（关键修改点）
        delete_template="DELETE FROM $TABLE_NAME WHERE CONCAT_WS(',', ${key_cols}) IN (%s)"
        
        split -l $BATCH_SIZE snap.db/${file}_keys_to_delete.txt snap.db/${file}_batch_
        
        for batch_file in snap.db/${file}_batch_*; do
            # 生成带引号的列表
            keys=$(sed "s/^/'/;s/$/'/;s/,/','/g" "$batch_file" | paste -sd,)
            
            # 执行删除
            if ! mysql -h"$ip" -P"$port" -u"$dbuser" -p"$dbpasswd" -D"$database" -e "$(printf "$delete_template" "$keys")"; then
                echo "[ERROR] $TABLE_NAME 批次删除失败" >&2
                exit 1
            else
                CURRENT_BATCH=$(wc -l < "$batch_file")
                PROCESSED=$((PROCESSED + CURRENT_BATCH))
                echo "[OK] $TABLE_NAME 已处理 $PROCESSED/$TOTAL_KEYS 条"
            fi
        done
    else
        echo "[INFO] $TABLE_NAME 无需删除记录"
    fi
done