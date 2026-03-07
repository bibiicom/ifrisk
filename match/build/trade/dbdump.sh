#!/bin/bash

dumpdir="./dump"

# 数据库连接信息
dbname=$(grep 'dbname=' network.ini | grep -v '#' | awk -F= '{print $2}')
dbuser=$(grep 'dbuser=' network.ini | grep -v '#' | awk -F= '{print $2}')
dbpasswd=$(grep 'dbpasswd=' network.ini | awk -F= '{print $2}')
ipall=${dbname##MYSQL://}
ipport=${ipall%@*}
database=${ipall#*@}
ip=${ipport%:*}
port=${ipport#*:}

# 获取所有表名
TABLES=()
while IFS= read -r table; do
    TABLES+=("$table")
done < <(mysql -h"$ip" -P"$port" -u"$dbuser" -p"$dbpasswd" -D"$database" -N -e "SHOW TABLES;")

rm -rf "$dumpdir"
mkdir -p "$dumpdir"

# 遍历每个表
for table in "${TABLES[@]}"; do
    # 过滤以t_开头且不包含t_D_的表
    if [[ ! "$table" =~ ^t_ ]]; then
        continue
    fi
    if [[ "$table" =~ ^t_D_ ]]; then
        continue
    fi
	#移除t_前缀
	csv_name="${table#t_}"
    # 获取列名并按顺序保存
    columns=()
    while IFS= read -r col; do
        columns+=("$col")
    done < <(mysql -h"$ip" -P"$port" -u"$dbuser" -p"$dbpasswd" -D"$database" -N -e "
        SET SESSION group_concat_max_len = 1000000;
        SELECT COLUMN_NAME 
        FROM INFORMATION_SCHEMA.COLUMNS 
        WHERE TABLE_NAME = '$table'
          AND TABLE_SCHEMA = '$database'
        ORDER BY ORDINAL_POSITION")

    # 构造带反引号的列名列表，避免关键字冲突
    select_cols=$(printf '`%s`,' "${columns[@]}" | sed 's/,$//')

    # 生成CSV文件头
    header=$(printf '"%s",' "${columns[@]}" | sed 's/,$//')
    echo "$header" > "$dumpdir/$csv_name.csv"

    # 导出数据，显式指定列顺序（表名也添加反引号）
    mysql -h"$ip" -P"$port" -u"$dbuser" -p"$dbpasswd" -D"$database" --quick --batch --skip-column-names -e "SELECT $select_cols FROM \`$table\`" | sed 's/\t/","/g;s/^/"/;s/$/"/;s/\r//g' >> "$dumpdir/$csv_name.csv"

    echo "Exported table $table to $dumpdir/$csv_name.csv"
done