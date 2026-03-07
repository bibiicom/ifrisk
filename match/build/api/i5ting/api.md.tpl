!!function!!
#include <string>
using namespace std;

string getPK(string para, string field)
{
	if (para.find(field) != string::npos)
		return "yes";			
	return "no";
}

string getIntValue(string para)
{
	if (para=="FrontNo")
		return "1";
	if (para=="SortNumber")
		return "-1";		
	if (para.find("No") != string::npos)
		return "1033201232";		
	if (para.find("Is") != string::npos)
		return "1";
	if (para.find("Group") != string::npos)
		return "999";	
	if (para.find("Grade") != string::npos)
		return "666";			
	return "0";
}

string getDoubleValue(string para)
{
	if (para=="Fee")
		return "0.22364";
	if (para=="CloseProfit")
		return "10";
	if (para=="BusinessNo")
		return "11176422300333";
	if (para.find("Time") != string::npos)
		return "1657543832";		
	if (para.find("Leverage") != string::npos)
		return "125";		
	return "182582.18";
}

string getStringValue(string para)
{
	if (para=="ExchangeID")
		return "CEX";
	if (para=="InstrumentID")
		return "BTCUSD";
	if (para=="ProductGroup")
		return "Swap";
	if (para=="SettleGroup")
		return "Swap";
	if (para=="Remark")
		return "my_little_tips";
	if (para=="APPID")
		return "ios";
	if (para=="OrderID")
		return "12376422300333";
	if (para=="TradeID")
		return "16576422300333";
	if (para=="OffsetFlag")
		return "0";
		
	if (para.find("Currency") != string::npos)
		return "BTC";
	if (para.find("Group") != string::npos)
		return "999";		
	if (para.find("ID") != string::npos)
		return "1983723";
	return "";
}

string getFilterValue(string para)
{
	if (para=="PushMarketTrade")
		return "DEX_BTCUSD";
	if (para=="MarketOrder")
		return "DEX_BTCUSD";
	if (para=="PushKLine")
		return "DEX_BTCUSD_15m";
	if (para=="PushMarketDataOverView")
		return "DEX_BTCUSD_2";
	if (para=="PushFiveDepthMarketData")
		return "DEX_BTCUSD_3";
	if (para=="PushTenDepthMarketData")
		return "DEX_BTCUSD_2";
	if (para=="PushOptionsMarketData")
		return "DEX_BTCUSD";
	return "";
}


string getTopicID(string para)
{
	if (para=="PushMarketTrade")
		return "2";
	if (para=="MarketOrder")
		return "25";
	if (para=="PushKLine")
		return "4";
	if (para=="PushMarketDataOverView")
		return "7";
	if (para=="PushFiveDepthMarketData")
		return "5";
	if (para=="PushTenDepthMarketData")
		return "10";
	if (para=="PushOptionsMarketData")
		return "7";
	if (para=="PushTimeSync")
		return "6";
	if (para=="PushInstrument")
		return "6";	
	return "3";
}

!!endfunction!!
**API Document**
______

# **接入说明**
## **接入URL**
### Restful Feed

url format | description
 -------- | --------
`https://127.0.0.1/action/v1.0/*` | 业务操作接口
`https://127.0.0.1/query/v1.0/*`  | 查询接口
`https://127.0.0.1/market/v1.0/*` | 行情获取接口
`https://127.0.0.1/SendInsert/Table` | 内存数据库插入
`https://127.0.0.1/SendUpdate/Table` | 内存数据库更新
`https://127.0.0.1/SendInsUpd/Table` | 内存数据库插入或者更新
`https://127.0.0.1/SendClear/Table` | 内存数据库清理字段
`https://127.0.0.1/SendDelete/Table` | 内存数据库删除字段
`https://127.0.0.1/SendQryOne/Table` | 内存数据库查询唯一记录

### WebSocket Feed
`wss://127.0.0.1/ws`

## **接口例子**

### **Web/App**

https://127.0.0.1/SendQryOne/Table

### **管理后台**

https://127.0.0.1/SendQryOne/Table

### **风控接口**

https://127.0.0.1/SendQryOne/Table

### **做市接口**

https://127.0.0.1/SendQryOne/Table

## **接口格式**

### **请求格式**
request type | url format | description
-------- | -------- | --------
Restful GET | /action/v1.0/*?A=1&B=2 | GET请求，所有参数包含在url路径中
Restful POST | /action/v1.0/* | POST请求，所有参数以JSON格式放在请求主体(Body)中

### **返回格式**
所有的接口都是JSON格式。

### **数据类型**
type | description
-------- | --------
string | 字符串类型
int | 整数类型
double| 浮点数类型

## **接口类型**

### **公共接口**
公共接口可用于获取基础信息和行情数据。公共接口无需认证即可调用。

### **私有接口**
私有接口可用于订单管理和账户管理。每个私有请求必须使用规范的验证形式进行签名。
私有接口需要使用您的APIKey进行验证。您可以在这里[申请APIKey](http://127.0.0.1)。

## **通信类型**

### **同步方式**
同步的调用方式使用http的方式调用，一个请求必须等到响应回来之后才能终止

### **异步方式**
异步的调用方式是使用ws的方式调用，一个请求不需要等待响应回来之后就可以发送下一个请求，这种情况下，需要标记每个不同的请求，以便于响应回来之后能对上

## **签名认证**

### **概述**
API请求在通过internet传输的过程中极有可能被篡改，为了确保请求未被更改，除公共接口(基础信息，行情数据)外的私有接口均必须使用您的APIKey 做签名认证，以校验参数或参数值在传输途中是否发生了更改。

### **申请APIKey**
在对任何请求进行签名之前，您必须通过网站[申请APIKey](http://127.0.0.1)。申请成功后，请务必记住以下信息：

* AccessKey：API访问密钥
* SecretKey：签名认证加密所使用的密钥(仅申请时可见)

AccessKey和SecretKey由网站随机生成和提供。如果您忘记AccessKey或SecretKey，则无法恢复，请您通过网站重新申请新的APIKey。

**风险提示：这两个密钥与账号安全紧密相关，无论何时都请勿将二者同时透露给其他人。如果发现APIKey泄露，请尽快删除该APIKey。**

### **签名计算**
签名是使用APIKey的SecretKey密钥，采用HMAC SHA256方法，对Timestamp + Method + RequestPath + Body字符串(+表示字符串连接)进行加密，然后通过Base64编码输出而得到的。

其中，
* SecretKey：APIKey的SecretKey密钥。例如：bdf199dd7f92b761968fdf7793f91509。
* Timestamp：发起请求的时间戳，精确到秒。例如：1601164749。
* Method：请求方法：GET或POST(字母全部大写)。
* RequestPath：请求接口路径。例如：/query/v1.0/Instrument。
* Body：请求体构成的字符串，如果没有请求体(通常为GET请求)则Body可省略。例如：{"UserID":"1001"}。

### **签名请求**
所有私有接口的请求头(Header)都必须包含以下内容：

* ACCESS-KEY：APIKey的AccessKey密钥。
* ACCESS-SIGN：签名计算得出的值(请参阅签名计算)。
* ACCESS-TIMESTAMP：发起请求的时间戳(与参与签名的Timestamp一致)。

## **Token验证**
### **restful验证**
 RestFul接口需要在http接口的header头中增加otoken的标签，其值为获取到的操作token值即可
### **WS验证**
 ws接口不需要签名认证，使用token调用UserLogin接口就可以使用

# **Restful请求响应接口**

!!enter FTD!!
!!travel packages!!
!!if !strcmp(@usage, "request")!!
!!let packName=@name!!

## **!!@comment!!**

+ **Request URL**

`POST /action/v1.0/!!@name!!`

!!if !strcmp(@packName, "SendQryMarketOrder")!!
`POST /market/v1.0/!!@name!!`
!!endif!!

+ **Request Post Body**

```
{
	!!let curlpara=""!!
	!!let httppara=""!!
	!!travel self!!
		!!let fieldName=@name!!
		!!enter FTD!!
			!!travel fields!!
				!!if !strcmp(@fieldName,@name)!!
					!!travel self!!
						!!let thisName=@name!!
						!!let thisType=@type!!
						!!enter FTD!!
							!!travel types!!
								!!let cmpTypeName=multiaddstring(3,"C",@typename,"Type")!!
								!!if !strcmp(@cmpTypeName,@thisType)!!
									!!if !strcmp(@name, "EnumChar")!!
										!!let thisValue="\"1\""!!
										!!let curlpara=multiaddstring(4, @curlpara, "\"", @thisName, "\":\"1\",")!!
										!!let httppara=multiaddstring(4, @httppara, @thisName, "=", "1&")!!
									!!elseif !strcmp(@type, "int")!!
										!!let thisValue=getIntValue(@thisName).c_str()!!
										!!let curlpara=multiaddstring(6, @curlpara, "\"", @thisName, "\":\"", @thisValue, ",")!!
										!!let httppara=multiaddstring(5, @httppara, @thisName, "=", @thisValue, "&")!!
									!!elseif !strcmp(@type, "double")!!
										!!let thisValue=getDoubleValue(@thisName).c_str()!!
										!!let curlpara=multiaddstring(6, @curlpara, "\"", @thisName, "\":\"", @thisValue, ",")!!
										!!let httppara=multiaddstring(5, @httppara, @thisName, "=", @thisValue, "&")!!
									!!else!!
										!!let thisValue=getStringValue(@thisName).c_str()!!
										!!let curlpara=multiaddstring(6, @curlpara, "\"", @thisName, "\":\"",@thisValue,"\",")!!
										!!let httppara=multiaddstring(5, @httppara, @thisName, "=", @thisValue, "&")!!
										!!let thisValue=multiaddstring(3, "\"", @thisValue, "\"")!!										
									!!endif!!
	!!if @pumpid!=0!!,!!endif!!"!!@thisName!!":!!@thisValue!!
								!!endif!!
							!!next!!
						!!leave!!
					!!next!!
				!!endif!!	
			!!next!!
		!!leave!!
	!!next!!
}
```
+ **curl Example**

````
!!let curlpara_1=headstr(@curlpara,-1)!!
curl -d '{!!@curlpara_1!!}'  -X POST  http://127.0.0.1:10000/action/v1.0/!!@packName!!
````
+ **ws Example**

````
{"RequestNo":2,"!!@packName!!":{!!@curlpara_1!!}}
````
+ **Web rowser Example**

````
!!let httppara_1=headstr(@httppara,-1)!!
http://127.0.0.1:10000/action/v1.0/!!@packName!!?!!@httppara_1!!
````
+ **Request Parameters**

parameter | shortName | required | type | description | remark
-------- | -------- | -------- | -------- | -------- | --------
	!!travel self!!
		!!let fieldName=@name!!
		!!enter FTD!!
			!!travel fields!!
				!!if !strcmp(@fieldName,@name)!!
					!!travel self!!
						!!let shortName=@short!!
						!!let isRequire=@required!!
						!!let remark=""!!
						!!let thisType=@type!!
						!!enter FTD!!
							!!travel types!!
								!!let cmpTypeName=multiaddstring(3,"C",@typename,"Type")!!
								!!if !strcmp(@cmpTypeName,@thisType)!!
									!!let lastType=@type!!
									!!if !strcmp(@name, "EnumChar")!!
										!!travel self!!
											!!let remark=multiaddstring(5, @remark, @value, ":", @label, ";")!!
										!!next!!
									!!endif!!
								!!endif!!
							!!next!!
						!!leave!!
!!@name!! | !!@shortName!! | !!@isRequire!! | !!@lastType!! | !!@label!! | !!@remark!!
					!!next!!
				!!endif!!
			!!next!!
		!!leave!!
	!!next!!

+ **Response**

```
{
	!!enter FTD!!
		!!travel packages!!
			!!if !strcmp(@usage,"response") && !strcmp(@packName+4,@name+4)!!
	"action": "!!@name!!",
	"errorCode": 0,
	"errorMsg": "Success",
	"result": [			
				!!travel self!!
					!!let fieldName=@name!!
				{"table":"!!@fieldName!!",
				 "data":{
					!!enter FTD!!
						!!travel fields!!
							!!if !strcmp(@fieldName,@name)!!
								!!travel self!!
									!!let thisName=@name!!
									!!let thisType=@type!!
									!!enter FTD!!
										!!travel types!!
											!!let cmpTypeName=multiaddstring(3,"C",@typename,"Type")!!
											!!if !strcmp(@cmpTypeName,@thisType)!!
												!!if !strcmp(@name, "EnumChar")!!
													!!let thisValue="\"1\""!!
												!!elseif !strcmp(@type, "int")!!
													!!let thisValue=getIntValue(@thisName).c_str()!!
												!!elseif !strcmp(@type, "double")!!
													!!let thisValue=getDoubleValue(@thisName).c_str()!!
												!!else!!
													!!let thisValue=getStringValue(@thisName).c_str()!!
													!!let thisValue=multiaddstring(3, "\"", @thisValue, "\"")!!
												!!endif!!
					!!if @pumpid!=0!!,!!endif!!!!@thisName!!:!!@thisValue!!
											!!endif!!
										!!next!!
									!!leave!!
								!!next!!
							!!endif!!
						!!next!!
					!!leave!!
					}
				}
				!!next!!
	]
			!!endif!!
		!!next!!
	!!leave!!
}
```

+ **Response Parameters**
	!!enter FTD!!
		!!travel packages!!
			!!if !strcmp(@usage,"response") && !strcmp(@packName+4,@name+4)!!			
				!!travel self!!
					!!let fieldName=@name!!

!!@name!!:

parameter | shortName | type | description | remark
-------- | -------- | -------- | -------- | -------- 					
					!!enter FTD!!
						!!travel fields!!
							!!if !strcmp(@fieldName,@name)!!
								!!travel self!!
									!!let shortName=@short!!
									!!let remark=""!!
									!!let thisType=@type!!
									!!enter FTD!!
										!!travel types!!
											!!let cmpTypeName=multiaddstring(3,"C",@typename,"Type")!!
											!!if !strcmp(@cmpTypeName,@thisType)!!
												!!let lastType=@type!!
												!!if !strcmp(@name, "EnumChar")!!
													!!travel self!!
														!!let remark=multiaddstring(5, @remark, @value, ":", @label, ";")!!
													!!next!!
												!!endif!!
											!!endif!!
										!!next!!
									!!leave!!
!!@name!! | !!@shortName!! | !!@lastType!! | !!@label!! | !!@remark!!
								!!next!!
							!!endif!!
						!!next!!
					!!leave!!
				!!next!!
			!!endif!!
		!!next!!
	!!leave!!

!!elseif !strcmp(@usage, "query")!!
!!let packName=@name!!
## **!!@comment!!**

+ **Request Parameters**

parameter | required | type | description | remark
-------- | -------- | -------- | -------- | --------
	!!let curlpara=""!!
	!!travel self!!
		!!let isRequire=@required!!
		!!let remark=""!!
		!!let thisType=@type!!
		!!let thisName=@name!!
		!!let thisValue=""!!
		!!enter FTD!!
			!!travel types!!
				!!let cmpTypeName=multiaddstring(3,"C",@typename,"Type")!!
				!!if !strcmp(@cmpTypeName,@thisType)!!
					!!let lastType=@type!!
					!!if !strcmp(@name, "EnumChar")!!
						!!travel self!!
							!!let remark=multiaddstring(5, @remark, @value, ":", @label, ";")!!
						!!next!!
					!!endif!!
					!!if !strcmp(@name, "EnumChar")!!
						!!let thisValue="1"!!
					!!elseif !strcmp(@type, "int")!!
						!!let thisValue=getIntValue(@thisName).c_str()!!
					!!elseif !strcmp(@type, "double")!!
						!!let thisValue=getDoubleValue(@thisName).c_str()!!
					!!else!!
						!!let thisValue=getStringValue(@thisName).c_str()!!
					!!endif!!					
				!!endif!!
			!!next!!
		!!leave!!
		!!if @pumpid==0!!
			!!let curlpara=multiaddstring(4, @curlpara, @thisName, "=", @thisValue)!!
		!!else!!
			!!let curlpara=multiaddstring(5, @curlpara, "&", @thisName, "=", @thisValue)!!
		!!endif!!
!!@name!! | !!@isRequire!! | !!@lastType!! | !!@label!! | !!@remark!!
	!!next!!

+ **Request URL**  

`GET http://127.0.0.1:10000/query/v1.0/!!@name!!?!!@curlpara!!`

+ **Response**

```
{
	"total_page": 1,
	"current_page": 1,
	"total_size": 2,
	"data": [{
!!let responseType=@response!!
!!enter FTD!!
	!!travel fields!!
		!!if !strcmp(@responseType, @name)!!
			!!travel self!!
				!!let thisType=@type!!
				!!let thisName=@name!!
				!!enter FTD!!
					!!travel types!!
						!!let cmpTypeName=multiaddstring(3,"C",@typename,"Type")!!
						!!if !strcmp(@cmpTypeName,@thisType)!!
							!!if !strcmp(@name, "EnumChar")!!
								!!let thisValue="\"1\""!!
							!!elseif !strcmp(@type, "int")!!
								!!let thisValue=getIntValue(@thisName).c_str()!!
							!!elseif !strcmp(@type, "double")!!
								!!let thisValue=getDoubleValue(@thisName).c_str()!!
							!!else!!
								!!let thisValue=getStringValue(@thisName).c_str()!!
								!!let thisValue=multiaddstring(3, "\"", @thisValue, "\"")!!
							!!endif!!
						!!endif!!
					!!next!!
				!!leave!!
			!!if @pumpid!=0!!,!!endif!!!!@thisName!!:!!@thisValue!!	
			!!next!!
		!!endif!!
	!!next!!
!!leave!!
	}]
}
```

+ **Response Parameters**

parameter | shortName | type | description | remark
-------- | -------- | -------- | -------- | -------- 
!!enter FTD!!
	!!travel fields!!
		!!if !strcmp(@responseType, @name)!!
			!!travel self!!
				!!let shortName=@short!!
				!!let remark=""!!
				!!let thisType=@type!!
				!!enter FTD!!
					!!travel types!!
						!!let cmpTypeName=multiaddstring(3,"C",@typename,"Type")!!
						!!if !strcmp(@cmpTypeName,@thisType)!!
							!!let lastType=@type!!
							!!if !strcmp(@name, "EnumChar")!!
								!!travel self!!
									!!let remark=multiaddstring(5, @remark, @value, ":", @label, ";")!!
								!!next!!
							!!endif!!
						!!endif!!
					!!next!!
				!!leave!!
!!@name!! | !!@shortName!! | !!@lastType!! | !!@label!! | !!@remark!!
			!!next!!
		!!endif!!
	!!next!!
!!leave!!

!!endif!!
!!next!!
!!leave!!

# **WebSocket数据推送接口**

## **订阅接口汇总**
!!enter FTD!!
name | TopicID | Comments | usage
-------- | -------- | -------- | --------
!!travel topics!!
!!@name!! | !!@value!! | !!@comment!! | !!@usage!!
!!next!!
!!leave!!


!!enter FTD!!
!!travel packages!!
!!if !strcmp(@usage, "Push")!!
!!let packName=@name!!
## **!!@comment!!**

+ **Request URL**  `/ws`

+ **Request**

```
{
	"SendTopicAction": {
		"Action": "1",
!!let filterValue=getFilterValue(@packName).c_str()!!
		"FilterValue": "!!@filterValue!!",
		"LocalNo": 5,
		"ResumeNo": -1,
!!let topicID=getTopicID(@packName).c_str()!!
		"TopicID": "!!@topicID!!",
		"BusinessNo": 19283726532
	}
}
```

+ **Request Parameters**

parameter | required | type | description | remark
-------- | -------- | -------- | -------- | --------
Action | yes | string | 订阅操作 | 0:全部退订;1:订阅;2:退订;
FilterValue | yes | string | 过滤值 | 格式:$ExchangeID_$InstrumentID_$Period,Period取值范围[1m:1分钟;5m:5分钟;15m:15分钟;30m:30分钟;1h:1小时;4h:4小时;12h:12小时;1d:一天;1w:一个星期;1o:一个月;1y:一年;]
LocalNo | yes | int | 本地唯一标识号 | 大于0，字符串比较，自定义,一次会话中不能重复
ResumeNo | yes | int | 续传号 | 0:从头开始;-1:从服务端最新位置续传;
TopicID | yes | string | 订阅主题标识 | 2:成交明细;3:私有交易主题;4:最新K线主题;5:5档行情快照主题;6:广播主题,合约变化,时钟心跳等;7:行情概要快照主题;8:全档行情增量主题;9:风控数据(持仓、资金、行情、时间),分控用户才能订阅，无法Filter;10:10档行情快照主题;
BusinessNo | no | int | 全局业务号 | 有BusinessNo业务的数据会从BusinessNo开始传输

+ **Response**

```
{
	"action": "!!@name!!",
	"localNo": 232,
	"seq": 123,
	"changeType": "3",
	"result": [			
				!!travel self!!
					!!let fieldName=@name!!
				{"table":"!!@fieldName!!",
				 "data":{
					!!enter FTD!!
						!!travel fields!!
							!!if !strcmp(@fieldName,@name)!!
								!!travel self!!
									!!let thisName=@name!!
									!!let thisType=@type!!
									!!enter FTD!!
										!!travel types!!
											!!let cmpTypeName=multiaddstring(3,"C",@typename,"Type")!!
											!!if !strcmp(@cmpTypeName,@thisType)!!
												!!if !strcmp(@name, "EnumChar")!!
													!!let thisValue="\"1\""!!
												!!elseif !strcmp(@type, "int")!!
													!!let thisValue=getIntValue(@thisName).c_str()!!
												!!elseif !strcmp(@type, "double")!!
													!!let thisValue=getDoubleValue(@thisName).c_str()!!
												!!else!!
													!!let thisValue=getStringValue(@thisName).c_str()!!
													!!let thisValue=multiaddstring(3, "\"", @thisValue, "\"")!!
												!!endif!!
					!!if @pumpid!=0!!,!!endif!!!!@thisName!!:!!@thisValue!!
											!!endif!!
										!!next!!
									!!leave!!
								!!next!!
							!!endif!!
						!!next!!
					!!leave!!
					}
				}
				!!next!!
	]
}
```

+ **Response Parameters**
				!!travel self!!
					!!let fieldName=@name!!

!!@name!!:

parameter | shortName | type | description | remark
-------- | -------- | -------- | -------- | -------- 					
					!!enter FTD!!
						!!travel fields!!
							!!if !strcmp(@fieldName,@name)!!
								!!travel self!!
									!!let shortName=@short!!
									!!let remark=""!!
									!!let thisType=@type!!
									!!enter FTD!!
										!!travel types!!
											!!let cmpTypeName=multiaddstring(3,"C",@typename,"Type")!!
											!!if !strcmp(@cmpTypeName,@thisType)!!
												!!let lastType=@type!!
												!!if !strcmp(@name, "EnumChar")!!
													!!travel self!!
														!!let remark=multiaddstring(5, @remark, @value, ":", @label, ";")!!
													!!next!!
												!!endif!!
											!!endif!!
										!!next!!
									!!leave!!
!!@name!! | !!@shortName!! | !!@lastType!! | !!@label!! | !!@remark!!
								!!next!!
							!!endif!!
						!!next!!
					!!leave!!
				!!next!!
!!endif!!
!!next!!
!!leave!!

# **内存数据库操作接口**

!!enter FTD!!
!!travel fields!!
!!if valid_name("usage") && !strcmp(@usage, "InitTable")!!
## **!!@comment!!**
!!let tableName=@name!!
+ **Request URL**  

```
!!enter FTD!!
!!travel packages!!
!!if !strcmp(@usage, "mdbrequest")!!
 !!@comment!!:  POST /action/v1.0/!!@name!!/!!@tableName!!
!!endif!!
!!next!!
!!leave!!
```

+ **Request**

```
{
	!!travel self!!
		!!let thisType=@type!!
		!!let thisName=@name!!
		!!enter FTD!!
			!!travel types!!
				!!let cmpTypeName=multiaddstring(3,"C",@typename,"Type")!!
				!!if !strcmp(@cmpTypeName,@thisType)!!
					!!if !strcmp(@name, "EnumChar")!!
						!!let thisValue="\"1\""!!
					!!elseif !strcmp(@type, "int")!!
						!!let thisValue=getIntValue(@thisName).c_str()!!
					!!elseif !strcmp(@type, "double")!!
						!!let thisValue=getDoubleValue(@thisName).c_str()!!
					!!else!!
						!!let thisValue=getStringValue(@thisName).c_str()!!
						!!let thisValue=multiaddstring(3, "\"", @thisValue, "\"")!!
					!!endif!!
				!!endif!!
			!!next!!
		!!leave!!
	!!if @pumpid!=0!!,!!endif!!!!@thisName!!:!!@thisValue!!	
	!!next!!
}
```

+ **Response**

```
{
	"action": "Recv*",
	"errorCode": 0,
	"errorMsg": "Success",
	"result": [
		{"table":"!!@tableName!!",
		"data":{	
		!!travel self!!
			!!let thisName=@name!!
			!!let thisType=@type!!
			!!enter FTD!!
				!!travel types!!
					!!let cmpTypeName=multiaddstring(3,"C",@typename,"Type")!!
					!!if !strcmp(@cmpTypeName,@thisType)!!
						!!if !strcmp(@name, "EnumChar")!!
							!!let thisValue="\"1\""!!
						!!elseif !strcmp(@type, "int")!!
							!!let thisValue=getIntValue(@thisName).c_str()!!
						!!elseif !strcmp(@type, "double")!!
							!!let thisValue=getDoubleValue(@thisName).c_str()!!
						!!else!!
							!!let thisValue=getStringValue(@thisName).c_str()!!
							!!let thisValue=multiaddstring(3, "\"", @thisValue, "\"")!!
						!!endif!!
					!!endif!!
				!!next!!
			!!leave!!
			!!if @pumpid!=0!!,!!endif!!!!@thisName!!:!!@thisValue!!
		!!next!!
		}
	}
}
```	

+ **Request & Response Parameters**

parameter | shortName | required | type | description | remark
-------- | -------- | -------- | -------- | -------- | --------
		!!travel self!!
			!!let shortName=@short!!
			!!let isRequire=getPK(@DBPK,@name).c_str()!!
			!!let remark=""!!
			!!let thisType=@type!!
			!!enter FTD!!
				!!travel types!!
					!!let cmpTypeName=multiaddstring(3,"C",@typename,"Type")!!
					!!if !strcmp(@cmpTypeName,@thisType)!!
						!!let lastType=@type!!
						!!if !strcmp(@name, "EnumChar")!!
							!!travel self!!
								!!let remark=multiaddstring(5, @remark, @value, ":", @label, ";")!!
							!!next!!
						!!endif!!
					!!endif!!
				!!next!!
			!!leave!!
!!@name!! | !!@shortName!! | !!@isRequire!! | !!@lastType!! | !!@label!! | !!@remark!!
		!!next!!
!!endif!!
!!next!!
!!leave!!

# **错误码**

## **错误码说明**
error code | description
-------- | --------
!!enter errors!!
	!!travel self!!
!!@value!! | !!@prompt!!
	!!next!!
!!leave!!