# trade

#### 介绍
交易和清算功能整合一体的版本

#### 组件安装
linux安装gcc
	yum install gcc-c++
	yum install gdb
windows 安装 visual studio

#### 使用说明

1. 设置环境变量
	windows:把setenv.bat.example重命名setenv.bat，通知把setenv.bat里面的路径配置安安装vs路径
	
2. 编译
	cd build
	build.bat/build.sh
	
3. 打包
	cd build
	pack.bat/pack.sh
	
4. 运行
	windows:start.bat/stop.bat
	linux:start.sh/stop.sh/show.sh
	
5. 研发
	windows:
	(1)使用vs打开所欲需要的模块进行代码编辑和调试
	(2)vs打开 \*.dsw 文件，默认是会转化的，同时默认是x86平台，需要手动新建一个x64的编译环境
	(3)需要需要调试，需要手动替换调试的目录到工程文件的项目属性->调试路径
	
6. 单独编译
	cd 工程根目录
	windows: setenv.bat
	cd build/trade
	nmake -f makefile.你所属的平台