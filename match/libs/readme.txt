应用lib库的使用说明：

简单示例：
	//使用动态库omqsapi，动态库的目录是../../../run/libs/ofserviceapi/1.0/omqsapi
	<package dir="../../../run/libs/ofserviceapi" version="1.0" libpath="omqsapi"/>
	//使用静态库openssl，静态库的目录是../../../build/libs/openssl/1.1.0h/static
	<package dir="../../../build/libs/openssl" version="1.1.0h" libpath="static" platform="windows"/>
	//使用动态库openssl，动态库的目录是../../../build/libs/openssl/1.1.0h/dynamic
	<package dir="../../../build/libs/openssl" version="1.1.0h" libpath="dynamic" platform="linux"/>
	//只有include文件，没有库，用于实现动态库的dll工程
	<package dir="../../../build/libs/ofserviceapi" version="1.0" libpath=""/>

使用概述：
	需要使用库的时候，在prj文件里面增加此行。
	libtype这一列必须出现。
	可以使用动态库、静态库,也可以仅包括头文件，实现库

字段含义：
	dir:库的主目录，保护子目录，下面包括include目录、库目录
	version：版本号，可以理解为dir目录下面的子目录，不填表示就在主目录下面
	libpath：库的存放位置，可以理解为dir+version目录下面的子目录，不填表示dir+version目录
	platform：不同平台使用不同的库才需要
			