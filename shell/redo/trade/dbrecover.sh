#!/bin/bash

# 从 network.ini 文件中提取数据库信息
dbname=$(grep 'dbname=' network.ini | grep -v '#' | awk -F = '{print $2}')
dbuser=$(grep 'dbuser=' network.ini | grep -v '#' | awk -F = '{print $2}')
dbpasswd=$(grep 'dbpasswd=' network.ini | awk -F = '{print $2}')

# 解析数据库连接字符串，获取 IP、端口和数据库名
ipall=${dbname##MYSQL://}
ipport=${ipall%@*}
database=${ipall#*@}
ip=${ipport%:*}
port=${ipport#*:}

for snap in snap/*; do
	# 遍历 Trade 和 AccountDetail 文件
	for file in Trade AccountDetail; do
		TABLE_NAME="perpetual.t_D_$file"
		CSV_FILE="$snap/${file}.csv"
		# 从 $snap/$file.csv 恢复数据到$TABLE_NAME
		# 检查 CSV 文件是否存在
		if [ -f "$CSV_FILE" ]; then
			echo "开始从 $CSV_FILE 恢复数据到 $TABLE_NAME"
			# 使用 mysql 命令执行 LOAD DATA INFILE 语句导入数据
			mysql -h "$ip" -P "$port" -u "$dbuser" -p"$dbpasswd" "$database" <<EOF
	LOAD DATA INFILE '$CSV_FILE'
	INTO TABLE $TABLE_NAME
	FIELDS TERMINATED BY ',' 
	OPTIONALLY ENCLOSED BY '"'
	LINES TERMINATED BY '\n'
	IGNORE 1 ROWS;
	EOF
			if [ $? -eq 0 ]; then
				echo "成功从 $CSV_FILE 恢复数据到 $TABLE_NAME"
			else
				echo "从 $CSV_FILE 恢复数据到 $TABLE_NAME 失败"
			fi
		else
			echo "CSV 文件 $CSV_FILE 未找到"
		fi	
	done
done
    