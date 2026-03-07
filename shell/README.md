## 数据库
### 安装
步骤省略，按正常流程安装即可。几乎可以支持所有数据库，建议使用mysql系列

root 用户：
配置源：
wget https://repo.mysql.com/mysql80-community-release-el7-5.noarch.rpm
yum localinstall mysql80-community-release-el7-5.noarch.rpm
rpm -ivh mysql80-community-release-el7-5.noarch.rpm

配置密钥：
rpm --import https://repo.mysql.com/RPM-GPG-KEY-mysql-2023
or
wget https://repo.mysql.com/RPM-GPG-KEY-mysql-2023
rpm --import RPM-GPG-KEY-mysql-2023
安装：
yum install mysql-community-server -y

systemctl enable mysqld
systemctl start mysqld
systemctl restart mysqld
systemctl status mysqld
systemctl stop mysqld

grep 'temporary password' /var/log/mysqld.log
mysql -uroot -p
ALTER USER 'root'@'localhost' IDENTIFIED WITH 'mysql_native_password' BY '!QAZ2wsx';
FLUSH PRIVILEGES;
CREATE DATABASE perpetual;

yum install epel-release
yum install jq
yum install tmux
yum install mysql

root用户
trade用户可以打开80/443端口
setcap 'cap_net_bind_service=+ep' /home/trade/run/client/nginx/sbin/nginx

yum install snapd
systemctl enable --now snapd.socket
ln -s /var/lib/snapd/snap /snap
snap install --classic certbot
ln -s /snap/bin/certbot /usr/bin/certbot

/root/build/cert_up.sh

#可以查看和删除已经生成的，错误的生成可以删除
certbot certificates
certbot delete

certbot certonly --manual
	#手动创建文件验证
	/etc/letsencrypt/live/yourdomain/
	Certbot 会为你自动生成 SSL 配置文件，通常位于：
	/etc/letsencrypt/live/yourdomain/fullchain.pem（证书）
	/etc/letsencrypt/live/yourdomain/privkey.pem（私钥）
	cp /etc/letsencrypt/live/yourdomain/* /home/centos/deploy/
		
		
certbot renew --dry-run
 
crontab
0 */12 * * * certbot renew --quiet /root/build/cert_up.sh
 
trade 用户:
### 部署
#### 拉取脚本
```bash
# 到部署机器的指定用户目录
cd ~ 

# 拉取部署脚本
cd ~ && ./repair_run.sh `${build工程的仓库地址}`
svn checkout svn://www.gitee.com/cwjjcom/batch/run ./

### 创建数据库
	mysql.sh
	CREATE DATABASE perpetual;
	
### 创建表
```bash
# 执行创建脚本
# 脚本位置 在发布的版本中的test目录下 Mysql.Create.Snap.sql Mysql.Create.Down.sql
	mysql.sh Mysql.Create.Snap.sql
	mysql.sh Mysql.Create.Down.sql
```

```
#### 设置全局命令
```bash
# 编辑环境变量文件
vi ~/.bashrc

# 添加内容
alias ls="ls --color"
alias ll="ls -l"
alias del="rm -rf"
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:.
export LANG=en_US.UTF-8
export PATH=$PATH:$HOME/run
export PATH=$PATH:$HOME/control

# 更新环境变量
source ~/.bashrc

# vim 配置文件
vi .vimrc

" 设置内部编码为 UTF-8
set encoding=utf-8
" 设置文件编码自动检测顺序
set fileencodings=utf-8,gbk,gb2312,gb18030,cp936,latin1
" 设置终端显示编码为 UTF-8
set termencoding=utf-8
" 确保文件编码正确写入
set fileencoding=utf-8

```
#### 部署运行
```bash
# 修改客户端配置
# 只有需要部署客户端才需要此操作
cp ~test/client/nginx.conf.template ~test/client/nginx.conf && vi ~test/client/nginx.conf

# 到指定目录
cd ~/deploy

# 修改模块配置文件，按需添加模块
# 模块说明：
# trade 交易核心
# sinkredis 同步数据到redis
# sinkdb 同步数据库到数据库
# query 查询
# redis
# markatmaker 行情
# triggerorder 条件单
# client 客户端
vi list.run

# 复制网络配置文件模板
cp network.ini.example network.ini

# 编辑网络配置文件，按需更改相关配置
vi network.ini

# 初始化
chmod u+x init.sh && ./init.sh 16g

# 选取发布版本
./get.sh
or 直接获取最新版本
./last.sh 

# 发布
./publish.sh

# 数据库
cd ./test/sql
./iconv.sh
mysql.sh Mysql.Create.Snap.sql
mysql.sh Mysql.Create.Down.sql.utf-8


# 启停
start.sh
show.sh
stop.sh

# 多机操作
cd control
deploy_get or deploy_last
deploy_publish
stopall
showall
stopall
```
## 相关文件说明
```json
├── control  // 该文件夹下的脚本用于控制多台服务
│		├── chkverall
│		├── get_release_all       	// 获取所有机器上编译好的版本
│		├── publishall							// 发布所有机器上服务
│		├── startall								// 启动所有机器上服务
│		├── cleanall								// 清空所有机器上服务的缓存，日志文件，流水文件等(必须所有服务停止后才能执行)
│		├── goto										// 登录到指定的机器
│		├── showall									// 显示所有机器上服务
│		├── stopall									// 停止所有机器上服务
│		├── get_lastest_all					// 获取所有服务上编译好的最新的版本
│		├── initall									// 初始化所有机器
│		├── sshall									// 建立所有机器链接
│		├── updateall								// 更新所有机器上服务
│		├── list.hosts							// 所有机器配置文件(需要手动创建)
│		├── list.hosts.example			// 所有机器配置文件模板
│		└── onestep									// 所有机器上的服务初始化，发布，启动一步完成
├── deploy  // 该文件夹下的脚本用于初始化和发布
│		├── get_release.sh					// 获取当前机器上编译好的版本
│		├── get_lastest.sh					// 获取当前机器上最新的编译好的文件
│		├── publish.sh							// 发布当前机器上的服务
│		├── init.sh									// 初始化当前机器上的服务
│		├── onestep.sh							// 当前机器的服务初始化，发布，启动一步完成
│		├── list.run								// 当前机器上的服务模块配置(只有初始化时才有用，需要手动创建)
│		├── list.run.all						// 所有服务模块配置
│		└── network.ini							// 配置文件
├── test    // 存放各个服务模块的模板和脚本文件
├── release // 该文件夹下存放编译好的文件
│		├── client									// 存放客户端编译文件的文件夹
│		└── trade										// 存放交易核心编译文件的文件夹
├── run     // 运行目录
│		├── trade
│		│		├── flow								// 存放交易服务流水文件夹(包含所有消息的顺序排列)
│		│		├── snap								// 存放交易服务快照文件夹
│		│		├── dump								// 存放交易服务错误消息文件夹
│		│		├── backup_remote.sh		// 备份必要文件到其它机器
│		│		├── get1.sh							// 获取最新必备的调试文件(用于本地调试)
│		│		├── stop.sh							// 停止当前服务
│		│		├── backup.sh						// 备份必要文件到本地backup目录
│		│		├── clean.sh						// 清空交易服务的缓存，日志文件，流水文件等(必须服务停止后才能执行)
│		│		├── diffsnap.sh					// 比较两个snap文件的差别
│		│		├── repair.sh						// 清除导致服务无法继续运行的消息
│		│		├── rollback						// 恢复指定用户到指定快照数据(该操作可能导致帐不平，需要有后续操作)
│		│		├── rsync.sh						// 实时同步必要文件到远程机器
│		│		├── trade								// 可执行主文件
│		│		├── *.so								// 可执行文件
│		│		├── network.ini					// 网络配置文件(所有模块的网络配置)
│		│		├── nohup.out						// 交易服务的stt输出文件
│		│		├── omq.*.log						// omq的日志文件
│		│		├── omq.ini							// omq的配置文件
│		│		├── trade.*.log					// 交易服务日志
│		│		├── trade.ini						// 交易服务基本配置(如：内存大小，日志存放路径)
│		│		└── dumpPhase.log				// 交易服务快照完成之后的最后一个序号文件
│		├── sinkdb
│		│		├── flow								// 存放同步数据库服务流水文件夹(包含所有消息的顺序排列)
│		│		├── clean.sh						// 清空同步数据库服务的缓存，日志文件，流水文件等(必须服务停止后才能执行)
│		│		├── sinkdb							// 主可执行文件
│		│		├── *.so								// 可执行文件
│		│		├── network.ini					// 网络配置文件(所有模块的网络配置)
│		│		├── nohup.out						// 同步数据库服务的stt输出文件
│		│		├── omq.*.log						// omq的日志文件
│		│		├── omq.ini							// omq的配置文件
│		│		├── sinkdb.*.log				// 同步数据库服务日志
│		│		└── sinkdb.ini					// 同步数据库服务基本配置(如：内存大小，日志存放路径)
│		├── sinkredis
│		│		├── flow								// 存放同步redis服务流水文件夹(包含所有消息的顺序排列)
│		│		├── clean.sh						// 清空同步redis服务的缓存，日志文件，流水文件等(必须服务停止后才能执行)
│		│		├── sinkredis						// 主可执行文件
│		│		├── *.so								// 可执行文件
│		│		├── network.ini					// 网络配置文件(所有模块的网络配置)
│		│		├── nohup.out						// 同步redis服务的stt输出文件
│		│		├── omq.*.log						// omq的日志文件
│		│		├── omq.ini							// omq的配置文件
│		│		├── sinkredis.*.log			// 同步redis服务日志
│		│		└── sinkredis.ini				// 同步redis服务基本配置(如：内存大小，日志存放路径)
│		├── triggerorder
│   │		├── flow								// 存放条件单服务流水文件夹(包含所有消息的顺序排列)
│		│		├── clean.sh						// 清空条件单服务的缓存，日志文件，流水文件等(必须服务停止后才能执行)
│		│		├── triggerorder				// 主可执行文件
│		│		├── *.so								// 可执行文件
│		│		├── network.ini					// 网络配置文件(所有模块的网络配置)
│		│		├── nohup.out						// 条件单服务的stt输出文件
│		│		├── omq.*.log						// omq的日志文件
│		│		├── omq.ini							// omq的配置文件
│		│		├── triggerorder.*.log	// 条件单服务日志
│		│		└── triggerorder.ini		// 条件单服务基本配置(如：内存大小，日志存放路径)
│		├── query
│		│		├── clean.sh						// 清空查询服务的缓存，日志文件，流水文件等(必须服务停止后才能执行)
│		│		├── query								// 主可执行文件
│		│		├── *.so								// 可执行文件
│		│		├── network.ini					// 网络配置文件(所有模块的网络配置)
│		│		├── nohup.out						// 查询服务的stt输出文件
│		│		├── omq.*.log						// omq的日志文件
│		│		├── omq.ini							// omq的配置文件
│		│		├── query.*.log					// 查询服务日志
│		│		└── query.ini						// 查询服务基本配置(如：内存大小，日志存放路径)
│		├── redis
│		│		├── bigkeys.sh					// 查看redis里面最大数据对应的key
│		│		├── start.sh						// 启动redis
│		│		├── clean.sh						// 清空redis的缓存(必须服务停止后才能执行)
│		│		├── redis								// 主可执行文件
│		│		├── redis-cli						// redis命令行执行文件
│		│		├── redis.linux.conf		// redis配置文件
│		│		├── network.ini					// 网络配置文件(所有模块的网络配置)
│		│		└── nohup.out						// redis的stt输出文件
│		├── marketmaker
│		│		├── flow								// 存放行情服务流水文件夹(包含所有消息的顺序排列)
│		│		├── clean.sh						// 清空行情服务的缓存，日志文件，流水文件等(必须服务停止后才能执行)
│		│		├── marketmaker					// 主可执行文件
│		│		├── *.so								// 可执行文件
│		│		├── network.ini					// 网络配置文件(所有模块的网络配置)
│		│		├── nohup.out						// 行情服务的stt输出文件
│		│		├── omq.*.log						// omq的日志文件
│		│		└── omq.ini							// omq的配置文件
│		├── client
│		│		├── nginx								// 客户端容器
│		│		├── show.sh							// 显示客户端信息
│		│		├── clean.sh						// 清空客户端缓存和日志文件
│		│		├── start.sh						// 启动客户端
│		│		└── stop.sh							// 停止客户端
│		├── backup_remote.sh				// 备份当前机器上所有服务必要文件到其它机器
│		├── cleanlog.sh							// 清除当前机器上的所有服务日志
│		├── mysql.sh								// 数据库执行命令
│		├── backup.sh								// 备份当前机器上所有服务必要文件到本地backup目录
│		├── clean.sh								// 清空当前机器上服务的缓存，日志文件，流水文件等(必须服务停止后才能执行)
│		├── pack.sh									// 打包，不需要手动执行
│		├── before_stop.sh					// 停止服务前执行，不需要手动执行
│		├── start.sh								// 启动当前机器上的所有服务
│		├── stop.sh									// 停止当前机器上的所有服务
│		├── chkver.sh								// 检查版本
│		├── rsync.sh								// 实时同步当前机器上所有服务的必要文件到远程机器
│		├── svn_revert.sh						// svn revert(svn命令)
│		├── mdbsnap.sh							// 打快照
│		├── show.sh									// 显示当前机器上的所有服务
│		└── svn_stop.sh							// 停止svn
└── repair_run.sh								// 初始化
```
