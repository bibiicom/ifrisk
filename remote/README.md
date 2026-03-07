安装方法：
	如果svn代码服务器的地址有变动，请到相应的配置文件里面修改地址
linux centos7.9:
	mv /etc/yum.repos.d/CentOS-Base.repo /etc/yum.repos.d/CentOS-Base.repo.bak
	curl -o /etc/yum.repos.d/CentOS-Base.repo https://mirrors.aliyun.com/repo/Centos-7.repo
	#yum clean all
	#yum makecache

	1.安装git
		yum install git
	2.安装gcc
		yum install gcc gcc-c++ gdb
	3.安装unzip
		yum install unzip
		yum install rsync
	4.获取脚本
	（2）对于无法互联网获取脚本的地方，使用中转机器
		cp env.ini.example env.ini
		vi env.ini
		mkdir build
		cd build
		scp remote@ip:./build/sync.sh ./
		./sync.sh
		
	5.密钥给run的用户
		ssh-keygen
		要用trade用户，手动复制黏贴不能直接拷过去，会改变trade的目录属性，导致无法ssh登录
		cat .ssh/id_rsa.pub >> /home/centos/.ssh/authorized_keys

windows:
	1.需要安装TortoiseSVN，并且安装命令行
	2.安装visual studio 2017(如果安装其他版本，需要在example里面修改配置)
	3.env/*.ini.example -> *.ini
	4.build_*.bat -> abuild_*.bat
	3.setenv.bat.example -> setenv.bat
	4.需要安装openssh的客户端功能，windows需要启用这个模块
	5.如果需要使用远程编译linux功能：
		（1）找一台远程的linux服务器，建立好信任关系
		（2）并且填写env.ini里面的地址和用户名
		（3）远程linux需要安装gcc
		
		
## Nodejs
### 安装
非必要，只有需要部署客户端才需要安装
```bash
# 下载
# 官网地址：https://nodejs.org
# 中文网地址：http://nodejs.cn

# 安装 nvm（Node 版本管理器）
curl -o- https://raw.githubusercontent.com/nvm-sh/nvm/v0.40.0/install.sh | bash
# 下载并安装 Node.js（可能需要重新启动终端）
nvm install 16

windows安装，直接下载安装

添加环境变量
.bashrc 
export NPM_CONFIG_PREFIX="/root/npm_modules"

```
## 编译部署
### 编译
#### 组件安装

# 安装unzip
yum install unzip

```
#### 拉取脚本
```bash
# repair_build.sh 需要手动放入
cd ~ && ./repair_build.sh `${build工程的仓库地址}`
```
#### 设置环境变量
```bash
cd ~/batch && chmod u+x set_global_env.linux.sh && ./set_global_env.linux.sh
```
#### 修改编译配置
```bash
# 到指定目录
cd ~/autobuild/env

# 创建配置文件
cp remote_release.ini.example remote_release.ini

# 修改配置文件
# remote_release_ipaddr 需要将编译文件推送到指定部署机器的ip
# remote_release_userid 需要将编译文件推送到指定部署机器的用户名
# remote_release_path 需要将编译文件推送到指定部署机器的目录
vi remote_release.ini

# 将本机的公钥复制到远程机器的authorized_keys文件中，此步骤根据需要操作
ssh-copy-id trade@127.0.0.1
```
#### 编译
```bash
# 到编译目录
cd ~/autobuild

# 编译交易核心
./build_trade.sh

# 编译tools
./buildtools.sh
```		