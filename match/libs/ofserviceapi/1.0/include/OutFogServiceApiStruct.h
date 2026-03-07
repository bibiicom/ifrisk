/////////////////////////////////////////////////////////////////////////
///OutFogServiceApiStruct.h
///定义了类 OutFogServiceApiStruct
///@history 
///徐忠华 创建该文件
/////////////////////////////////////////////////////////////////////////

#ifndef _OUTFOG_SERVICE_API_STRUCT_H_
#define _OUTFOG_SERVICE_API_STRUCT_H_

//Stream的最大大小
#define OUTFOG_MAX_PACKAGE_SIZE 4096 

//Type 字典定义
#define OUTFOG_TYPE_NULL		'A'		//无操作
#define OUTFOG_TYPE_INIT		'B'		//业务初始化标志
#define OUTFOG_TYPE_START		'C'		//开始工作,启动线程
#define OUTFOG_TYPE_ADDSESSION	'D'		//连接
#define OUTFOG_TYPE_DELSESSION	'E'		//登出
#define OUTFOG_TYPE_INITLINK	'F'		//初始化Link
#define OUTFOG_TYPE_TIMER		'G'		//定时器
#define OUTFOG_TYPE_REDOOVER	'R'		//Redo完成标志

	//	SubIndexName	代表本地字符日期
	//	UserID			代表本地字符时间
	//	RequestNo		代表毫秒数
	//	SessionNo		代表标准时间秒数

#define OUTFOG_TYPE_SUBTOPIC	'I'		//订阅主题
#define OUTFOG_TYPE_UNSUBTOPIC	'J'		//退订主题

#define OUTFOG_TYPE_MANAGER		'K'		//系统管理命令
#define OUTFOG_TYPE_STOP		'L'		//OMQ忽略报文,输入流消失，系统停止工作
#define OUTFOG_TYPE_ADDAPIKEY	'M'		//增加ApiKey
#define OUTFOG_TYPE_LOOP		'N'		//作为未切换主系统工作前的循环回调
#define OUTFOG_TYPE_DELAPIKEY	'O'		//删除ApiKey
#define OUTFOG_TYPE_ADDAPIKEYALLOWIP	'P'		//增加ApiKey IP
#define OUTFOG_TYPE_DELAPIKEYALLOWIP    'Q'		//删除ApiKey IP
#define OUTFOG_TYPE_DELINDEXFLOW    'S'		//删除索引流

#define OUTFOG_TYPE_REQUEST		'1'		//请求
#define OUTFOG_TYPE_RESPONSE	'2'		//应答
#define OUTFOG_TYPE_PUBLISH		'3'		//发布
#define OUTFOG_TYPE_LOGIN		'4'		//登录
#define OUTFOG_TYPE_RSPPUB		'5'		//应答和发布

#define OUTFOG_TYPE_INSERT		'a'		//插入
#define OUTFOG_TYPE_DELETE		'b'		//删除
#define OUTFOG_TYPE_UPDATE		'c'		//更新
#define OUTFOG_TYPE_INSUPD		'd'		//插入或者更新

	//	FrontNo			对redis：负数代表此条不保存，0表示正常，正数表示最多保存的条数
	//					对于DB，大于0表示最多保存n条，意味着数据无需保存数据库，
	//  SessionNo		代表实际使用的FieldNo

//Direction 字典定义
#define OUTFOG_DIRECTION_TO_NULL	'N'		//没有规定
#define OUTFOG_DIRECTION_TO_FRONT	'F'		//向前置发布
#define OUTFOG_DIRECTION_TO_OFFER	'O'		//向报盘机发布
#define OUTFOG_DIRECTION_TO_KERNEL	'K'		//向核心发布

//Chain字典定义
//--使用多Field组成一个包的场景
#define OUTFOG_CHAIN_FIRST			'F'		//第一个
#define OUTFOG_CHAIN_CONTINUE		'C'		//还有后续Field
#define OUTFOG_CHAIN_LAST			'L'		//最后一个
//--使用Field一个包的场景
#define OUTFOG_CHAIN_SINGLE			'S'		//只有一个报文

//DataFormat字典定义
#define OUTFOG_DATAFORMAT_STRUCT	'C'		//类或者结构体
#define OUTFOG_DATAFORMAT_STREAM	'S'		//字节流()
#define OUTFOG_DATAFORMAT_JSON		'J'		//Json格式
#define OUTFOG_DATAFORMAT_OUTPUT	'O'		//打印格式
#define OUTFOG_DATAFORMAT_FIX		'F'		//FIX格式
#define OUTFOG_DATAFORMAT_CSV		'V'		//CSV格式
#define OUTFOG_DATAFORMAT_FORMDATA	'D'		//GetFormData格式

//SubjectID 字典定义
#define OUTFOG_SUBJECTID_NULL		INT_MAX	//不需要向主题发送
#define OUTFOG_SUBJECTID_MULTI		0		//除了会话(1)之外的多个主题，使用USERID(2,3,4)标识

//Tid的定义
#define OUTFOG_TNO_NULL		 0xFFFFFFFF //表示没有TID

#define OUTFOG_RESERVE_SYNC_YES		'1'
#define OUTFOG_RESERVE_SYNC_NO		'\0'

//定义Token是否为管理Token
#define OUTFOG_REMARK_TOKEN_MANAGER		'm'

// #ifdef  WINDOWS
// typedef __int64		OF_INT8;
// #else
// typedef long long	OF_INT8;
// #endif

//typedef double	OF_INT8;
//#pragma pack()

//系统默认是8字节对齐，如果内部代码改为4字节对齐则出现问题
//这里把按照字节从大到小排列，不管是4字节还是8字节，都能处理
//double:8,int:4,bool:4,char:1 数组的大小为2的n次方

struct TOutFogMetaRef
{
	//订阅字符串的顺序意味着后面的字符串可以被使用的，比如Remark可以使用的就是128个字符
	char Remark[24];		//自定义字段,极限可以使用128个字符为Token，ErrorMsg
	char SubIndexName[48];	//订阅索引,可以使用96个字符,Reserve在使用SubIndexName的时候有含义不能使用
	char UserID[48];		//用户名,可以使用53个字符
	char Reserve[5];		//预留字段(5个字节double可用);同步Push报文([0]:是否同步数据库;[1]:是否同步Redis)
	char Direction;			//数据发送方向(见上字典定义)
	char Type;				//操作类型(见上字典定义)
	char Chain;				//连续标志(见上字典定义);数据转换功能定义为数据格式：类、字节流、json、CSV等
	double SequenceNo;		//各主题内部的序号
	double GlobalNo;		//全局统一的序号
	double BusinessNo;		//业务号;Req报文是Link标识号(正数)
	int FrontNo;			//前置号(负数);同步Push报文(表示最多保存Redis的条数)
	int RequestNo;			//客户定义请求号(负数表示不需要回复);同步Push报文传的是线程序号
	int SessionNo;			//会话号;同步Push报文代表实际使用的FieldNo
	int MessageNo;			//发送包头的数据识别id
	int Length;				//数据长度
	int SubjectNo;			//属于的主题号,错误信息ErrorNo
	int FieldNo;			//数据类型号	
	int IsReplay;			//是否是反演的数据 >0:redo;=0最后一个报文;<0正常
};
#endif

