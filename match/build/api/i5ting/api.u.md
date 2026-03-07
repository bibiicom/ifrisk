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


## **一般消息请求**

+ **Request URL**

`POST /action/v1.0/SendCommand`


+ **Request Post Body**

```
{
	,"UserID":"1983723"
	,"LocalID":"1983723"
	,"Command":""
	,"Content":""
}
```
+ **curl Example**

````
curl -d '{"UserID":"1983723","LocalID":"1983723","Command":"","Content":""}'  -X POST  http://127.0.0.1:10000/action/v1.0/SendCommand
````
+ **ws Example**

````
{"RequestNo":2,"SendCommand":{"UserID":"1983723","LocalID":"1983723","Command":"","Content":""}}
````
+ **Web rowser Example**

````
http://127.0.0.1:10000/action/v1.0/SendCommand?UserID=1983723&LocalID=1983723&Command=&Content=
````
+ **Request Parameters**

parameter | shortName | required | type | description | remark
-------- | -------- | -------- | -------- | -------- | --------
UserID | UserID | no | string | 交易用户代码 | 
LocalID | LocalID | no | string | 命令本地号 | 
Command | Command | no | string | 命令 | 
Content | Content | no | string | 字段内容 | 

+ **Response**

```
{
	"action": "RecvCommand",
	"errorCode": 0,
	"errorMsg": "Success",
	"result": [			
				{"table":"Command",
				 "data":{
					,UserID:"1983723"
					,LocalID:"1983723"
					,Command:""
					,Content:""
					}
				}
	]
}
```

+ **Response Parameters**

Command:

parameter | shortName | type | description | remark
-------- | -------- | -------- | -------- | -------- 					
UserID | UserID | string | 交易用户代码 | 
LocalID | LocalID | string | 命令本地号 | 
Command | Command | string | 命令 | 
Content | Content | string | 字段内容 | 


## **创建上层连接请求**

+ **Request URL**

`POST /action/v1.0/SendInitLink`


+ **Request Post Body**

```
{
	,"LinkNo":1033201232
	,"LinkUserID":"1983723"
	,"LinkPassword":""
	,"EncryptType":"1"
	,"LinkDllName":""
	,"LinkDllLoad":""
	,"LinkAddress":""
	,"LinkStatus":"1"
	,"IsActive":1
	,"Config":""
	,"Remark":"my_little_tips"
}
```
+ **curl Example**

````
curl -d '{"LinkNo":"1033201232,"LinkUserID":"1983723","LinkPassword":"","EncryptType":"1","LinkDllName":"","LinkDllLoad":"","LinkAddress":"","LinkStatus":"1","IsActive":"1,"Config":"","Remark":"my_little_tips"}'  -X POST  http://127.0.0.1:10000/action/v1.0/SendInitLink
````
+ **ws Example**

````
{"RequestNo":2,"SendInitLink":{"LinkNo":"1033201232,"LinkUserID":"1983723","LinkPassword":"","EncryptType":"1","LinkDllName":"","LinkDllLoad":"","LinkAddress":"","LinkStatus":"1","IsActive":"1,"Config":"","Remark":"my_little_tips"}}
````
+ **Web rowser Example**

````
http://127.0.0.1:10000/action/v1.0/SendInitLink?LinkNo=1033201232&LinkUserID=1983723&LinkPassword=&EncryptType=1&LinkDllName=&LinkDllLoad=&LinkAddress=&LinkStatus=1&IsActive=1&Config=&Remark=my_little_tips
````
+ **Request Parameters**

parameter | shortName | required | type | description | remark
-------- | -------- | -------- | -------- | -------- | --------
LinkNo | LinkNo | no | int | 通道号 | 
LinkUserID | LinkUserID | no | string | 通道用户 | 
LinkPassword | LinkPassword | no | string | 通道用户密码 | 
EncryptType | EncryptType | no | string | 密码加密算法 | 0:不加密;1:MD5加密;2:Base64加密;
LinkDllName | LinkDllName | no | string | LinkDll名 | 
LinkDllLoad | LinkDllLoad | no | string | LinkDll加载库 | 
LinkAddress | LinkAddress | no | string | Link连接地址 | 
LinkStatus | LinkStatus | no | string | Link状态 | 0:不活跃;1:已登录;2:已登出;
IsActive | IsActive | no | int | 是否活跃 | 
Config | Config | no | string | 报盘自定义的配置 | 
Remark | Remark | no | string | 备注 | 

+ **Response**

```
{
	"action": "RecvInitLink",
	"errorCode": 0,
	"errorMsg": "Success",
	"result": [			
				{"table":"Link",
				 "data":{
					,LinkNo:1033201232
					,LinkUserID:"1983723"
					,LinkPassword:""
					,EncryptType:"1"
					,LinkDllName:""
					,LinkDllLoad:""
					,LinkAddress:""
					,LinkStatus:"1"
					,IsActive:1
					,Config:""
					,Remark:"my_little_tips"
					}
				}
	]
}
```

+ **Response Parameters**

Link:

parameter | shortName | type | description | remark
-------- | -------- | -------- | -------- | -------- 					
LinkNo | LinkNo | int | 通道号 | 
LinkUserID | LinkUserID | string | 通道用户 | 
LinkPassword | LinkPassword | string | 通道用户密码 | 
EncryptType | EncryptType | string | 密码加密算法 | 0:不加密;1:MD5加密;2:Base64加密;
LinkDllName | LinkDllName | string | LinkDll名 | 
LinkDllLoad | LinkDllLoad | string | LinkDll加载库 | 
LinkAddress | LinkAddress | string | Link连接地址 | 
LinkStatus | LinkStatus | string | Link状态 | 0:不活跃;1:已登录;2:已登出;
IsActive | IsActive | int | 是否活跃 | 
Config | Config | string | 报盘自定义的配置 | 
Remark | Remark | string | 备注 | 


## **用户登录请求**

+ **Request URL**

`POST /action/v1.0/SendUserLogin`


+ **Request Post Body**

```
{
	,"UserID":"1983723"
	,"Password":""
	,"UserProductID":"1983723"
	,"MacAddress":""
	,"ClientIPAddress":""
	,"HDSerialID":"1983723"
	,"AuthCode":""
	,"APPID":"ios"
	,"Token":""
	,"Remark":"my_little_tips"
}
```
+ **curl Example**

````
curl -d '{"UserID":"1983723","Password":"","UserProductID":"1983723","MacAddress":"","ClientIPAddress":"","HDSerialID":"1983723","AuthCode":"","APPID":"ios","Token":"","Remark":"my_little_tips"}'  -X POST  http://127.0.0.1:10000/action/v1.0/SendUserLogin
````
+ **ws Example**

````
{"RequestNo":2,"SendUserLogin":{"UserID":"1983723","Password":"","UserProductID":"1983723","MacAddress":"","ClientIPAddress":"","HDSerialID":"1983723","AuthCode":"","APPID":"ios","Token":"","Remark":"my_little_tips"}}
````
+ **Web rowser Example**

````
http://127.0.0.1:10000/action/v1.0/SendUserLogin?UserID=1983723&Password=&UserProductID=1983723&MacAddress=&ClientIPAddress=&HDSerialID=1983723&AuthCode=&APPID=ios&Token=&Remark=my_little_tips
````
+ **Request Parameters**

parameter | shortName | required | type | description | remark
-------- | -------- | -------- | -------- | -------- | --------
UserID | UserID | yes | string | 交易用户代码 | 
Password | Password | yes | string | 密码 | 
UserProductID | UserProductID | no | string | 用户端产品信息 | 
MacAddress | MacAddress | no | string | Mac地址 | 
ClientIPAddress | ClientIPAddress | no | string | 终端IP地址 | 
HDSerialID | HDSerialID | no | string | 硬盘序列号 | 
AuthCode | AuthCode | no | string | 授权编码 | 
APPID | APPID | no | string | 应用编号 | 
Token | Token | no | string | 令牌 | 
Remark | Remark | no | string | 备注 | 

+ **Response**

```
{
	"action": "RecvUserLogin",
	"errorCode": 0,
	"errorMsg": "Success",
	"result": [			
				{"table":"UserLogin",
				 "data":{
					,UserID:"1983723"
					,Password:""
					,UserProductID:"1983723"
					,MacAddress:""
					,ClientIPAddress:""
					,HDSerialID:"1983723"
					,AuthCode:""
					,APPID:"ios"
					,Token:""
					,Remark:"my_little_tips"
					}
				}
				{"table":"UserSession",
				 "data":{
					,UserID:"1983723"
					,Password:""
					,UserProductID:"1983723"
					,MacAddress:""
					,ClientIPAddress:""
					,HDSerialID:"1983723"
					,AuthCode:""
					,APPID:"ios"
					,Token:""
					,Remark:"my_little_tips"
					,APIID:"1983723"
					,LoginTime:1657543832
					,ExpireTime:1657543832
					,IPAddress:""
					,MaxLocalID:"1983723"
					,SessionNo:1033201232
					,FrontNo:1
					,AccessLimit:0
					,UserType:"1"
					,MemberID:"1983723"
					,LimitAccesses:0
					,SinkType:"1"
					,InsertTime:1657543832
					,UpdateTime:1657543832
					}
				}
	]
}
```

+ **Response Parameters**

UserLogin:

parameter | shortName | type | description | remark
-------- | -------- | -------- | -------- | -------- 					
UserID | UserID | string | 交易用户代码 | 
Password | Password | string | 密码 | 
UserProductID | UserProductID | string | 用户端产品信息 | 
MacAddress | MacAddress | string | Mac地址 | 
ClientIPAddress | ClientIPAddress | string | 终端IP地址 | 
HDSerialID | HDSerialID | string | 硬盘序列号 | 
AuthCode | AuthCode | string | 授权编码 | 
APPID | APPID | string | 应用编号 | 
Token | Token | string | 令牌 | 
Remark | Remark | string | 备注 | 

UserSession:

parameter | shortName | type | description | remark
-------- | -------- | -------- | -------- | -------- 					
UserID | UserID | string | 交易用户代码 | 
Password | Password | string | 密码 | 
UserProductID | UserProductID | string | 用户端产品信息 | 
MacAddress | MacAddress | string | Mac地址 | 
ClientIPAddress | ClientIPAddress | string | 终端IP地址 | 
HDSerialID | HDSerialID | string | 硬盘序列号 | 
AuthCode | AuthCode | string | 授权编码 | 
APPID | APPID | string | 应用编号 | 
Token | Token | string | 令牌 | 
Remark | Remark | string | 备注 | 
APIID | APIID | string | 接口端产品信息 | 
LoginTime | LoginTime | double | 登录时间 | 
ExpireTime | ExpireTime | double | 失效时间 | 
IPAddress | IPAddress | string | 允许登录的IP地址 | 
MaxLocalID | MaxLocalID | string | 最大本地标识 | 
SessionNo | SessionNo | int | 会话编号 | 
FrontNo | FrontNo | int | 前置编号 | 
AccessLimit | AccessLimit | int | 每秒访问的次数限制 | 
UserType | UserType | string | 用户类型 | 1:普通用户(订阅自己/可下自己订单);2:观察用户OB(订阅其他用户/不可下单);5:超级管理员(订阅所有/可给所有用户下单/可爆仓);
MemberID | MemberID | string | 成员代码 | 
LimitAccesses | LimitAccesses | int | 该秒已经访问的次数 | 
SinkType | SinkType | string | 下游系统同步类型 | 0:同步到数据库;1:同步到Redis不到数据库;2:不同步;
InsertTime | InsertTime | double | 插入时间 | 
UpdateTime | UpdateTime | double | 更新时间 | 


## **用户退出请求**

+ **Request URL**

`POST /action/v1.0/SendUserLogout`


+ **Request Post Body**

```
{
	,"UserID":"1983723"
	,"APPID":"ios"
	,"Token":""
}
```
+ **curl Example**

````
curl -d '{"UserID":"1983723","APPID":"ios","Token":""}'  -X POST  http://127.0.0.1:10000/action/v1.0/SendUserLogout
````
+ **ws Example**

````
{"RequestNo":2,"SendUserLogout":{"UserID":"1983723","APPID":"ios","Token":""}}
````
+ **Web rowser Example**

````
http://127.0.0.1:10000/action/v1.0/SendUserLogout?UserID=1983723&APPID=ios&Token=
````
+ **Request Parameters**

parameter | shortName | required | type | description | remark
-------- | -------- | -------- | -------- | -------- | --------
UserID | UserID | no | string | 交易用户代码 | 
APPID | APPID | no | string | 应用编号 | 
Token | Token | no | string | 令牌 | 

+ **Response**

```
{
	"action": "RecvUserLogout",
	"errorCode": 0,
	"errorMsg": "Success",
	"result": [			
				{"table":"UserLogout",
				 "data":{
					,UserID:"1983723"
					,APPID:"ios"
					,Token:""
					}
				}
	]
}
```

+ **Response Parameters**

UserLogout:

parameter | shortName | type | description | remark
-------- | -------- | -------- | -------- | -------- 					
UserID | UserID | string | 交易用户代码 | 
APPID | APPID | string | 应用编号 | 
Token | Token | string | 令牌 | 


## **报单录入请求**

+ **Request URL**

`POST /action/v1.0/SendOrderInsert`


+ **Request Post Body**

```
{
	,"MemberID":"1983723"
	,"TradeUnitID":"1983723"
	,"AccountID":"1983723"
	,"LocalID":"1983723"
	,"ExchangeID":"CEX"
	,"InstrumentID":"BTCUSD"
	,"OrderPriceType":"1"
	,"Direction":"1"
	,"OffsetFlag":"1"
	,"Price":182582.18
	,"Volume":182582.18
	,"VolumeDisplay":182582.18
	,"VolumeMode":"1"
	,"Cost":182582.18
	,"OrderType":"1"
	,"GTDTime":1657543832
	,"MinVolume":182582.18
	,"BusinessType":"1"
	,"BusinessValue":""
	,"CloseOrderID":"1983723"
	,"IsCrossMargin":1
	,"Remark":"my_little_tips"
	,"OrderID":"12376422300333"
	,"CopyMemberID":"1983723"
	,"CopyOrderID":"1983723"
	,"Leverage":125
	,"CopyProfitRate":182582.18
	,"APPID":"ios"
	,"PositionID":"1983723"
	,"TriggerPrice":182582.18
	,"Reserve":182582.18
}
```
+ **curl Example**

````
curl -d '{"MemberID":"1983723","TradeUnitID":"1983723","AccountID":"1983723","LocalID":"1983723","ExchangeID":"CEX","InstrumentID":"BTCUSD","OrderPriceType":"1","Direction":"1","OffsetFlag":"1","Price":"182582.18,"Volume":"182582.18,"VolumeDisplay":"182582.18,"VolumeMode":"1","Cost":"182582.18,"OrderType":"1","GTDTime":"1657543832,"MinVolume":"182582.18,"BusinessType":"1","BusinessValue":"","CloseOrderID":"1983723","IsCrossMargin":"1,"Remark":"my_little_tips","OrderID":"12376422300333","CopyMemberID":"1983723","CopyOrderID":"1983723","Leverage":"125,"CopyProfitRate":"182582.18,"APPID":"ios","PositionID":"1983723","TriggerPrice":"182582.18,"Reserve":"182582.18}'  -X POST  http://127.0.0.1:10000/action/v1.0/SendOrderInsert
````
+ **ws Example**

````
{"RequestNo":2,"SendOrderInsert":{"MemberID":"1983723","TradeUnitID":"1983723","AccountID":"1983723","LocalID":"1983723","ExchangeID":"CEX","InstrumentID":"BTCUSD","OrderPriceType":"1","Direction":"1","OffsetFlag":"1","Price":"182582.18,"Volume":"182582.18,"VolumeDisplay":"182582.18,"VolumeMode":"1","Cost":"182582.18,"OrderType":"1","GTDTime":"1657543832,"MinVolume":"182582.18,"BusinessType":"1","BusinessValue":"","CloseOrderID":"1983723","IsCrossMargin":"1,"Remark":"my_little_tips","OrderID":"12376422300333","CopyMemberID":"1983723","CopyOrderID":"1983723","Leverage":"125,"CopyProfitRate":"182582.18,"APPID":"ios","PositionID":"1983723","TriggerPrice":"182582.18,"Reserve":"182582.18}}
````
+ **Web rowser Example**

````
http://127.0.0.1:10000/action/v1.0/SendOrderInsert?MemberID=1983723&TradeUnitID=1983723&AccountID=1983723&LocalID=1983723&ExchangeID=CEX&InstrumentID=BTCUSD&OrderPriceType=1&Direction=1&OffsetFlag=1&Price=182582.18&Volume=182582.18&VolumeDisplay=182582.18&VolumeMode=1&Cost=182582.18&OrderType=1&GTDTime=1657543832&MinVolume=182582.18&BusinessType=1&BusinessValue=&CloseOrderID=1983723&IsCrossMargin=1&Remark=my_little_tips&OrderID=12376422300333&CopyMemberID=1983723&CopyOrderID=1983723&Leverage=125&CopyProfitRate=182582.18&APPID=ios&PositionID=1983723&TriggerPrice=182582.18&Reserve=182582.18
````
+ **Request Parameters**

parameter | shortName | required | type | description | remark
-------- | -------- | -------- | -------- | -------- | --------
MemberID | M | no | string | 成员代码 | 
TradeUnitID | T | no | string | 交易单元代码 | 
AccountID | A | no | string | 资金账号 | 
LocalID | L | no | string | 报单本地标识 | 
ExchangeID | E | yes | string | 交易所代码 | 
InstrumentID | I | yes | string | 标的代码 | 
OrderPriceType | PT | yes | string | 报单价格类型 | 0:限价(手动指定Price值);1:任意价(Price按最高最低限价取值);2:最优价/对手价(Price按对方1档价取值);3:五档价(Price按对方5档价取值);4:十档价(类同上);7:最优Marker价(Price按本方1档价或其+1Tick取值);8:Price按委托额Cost和订单簿情况确定值;9:保底点差价:成交价格Price不会劣于系统设置差价成交;
Direction | D | yes | string | 买卖方向 | 0:买;1:卖;
OffsetFlag | O | no | string | 开平标志 | 0:开仓;1:平仓;2:强平;3:平今;4:平昨;5:全平;6:指定订单平仓;7:指定成交平仓;8:最大平仓;
Price | P | no | double | 报单价格 | 
Volume | V | no | double | 数量 | 
VolumeDisplay | D | no | double | 显示数量 | 
VolumeMode | VM | no | string | 数量取值方式 | 0:百分比;1:绝对值;
Cost | C | no | double | 委托额 | 
OrderType | OT | no | string | 订单类型 | 0:普通订单;1:(FillAndKill/IOC)立即完成并且剩余撤销;2:(FillOrKill)立即完成全部或者最小订单要求否则全部撤销;3:需要进入订单簿被动成交,否则全部撤销(只做Maker/PostOnly);4:不需要撮合直接成交;
GTDTime | GT | no | double | GTD时间 | 
MinVolume | MV | no | double | 最小成交量 | 
BusinessType | BT | no | string | 业务类别 | 0:普通;1:申赎;2:行权;3:中立仓申报;4:递延交割申报;5:互换定单;6:质押;7:合并分拆;8:转股;9:回售回购;a:投票;b:融资融券;c:期权执行;d:传递被平仓订单BusinessValue;P:自定义11;Q:自定义10;R:自定义9;S:连续交易;T:自定义7;U:画线委托;V:限价市价;W:条件;X:止盈止损;Y:追踪出场;Z:指标类型;
BusinessValue | BV | no | string | 业务值 | 
CloseOrderID | CI | no | string | 平仓指定开仓的订单号 | 
IsCrossMargin | IsCrossMargin | no | int | 是否全仓 | 
Remark | R | no | string | 备注 | 
OrderID | OrderID | no | string | 报单系统唯一代码 | 
CopyMemberID | CopyMemberID | no | string | 带单员代码 | 
CopyOrderID | CopyOrderID | no | string | 带单员报单号 | 
Leverage | Leverage | no | double | 委托单杠杆倍数 | 
CopyProfitRate | CopyProfitRate | no | double | 带单分配比例 | 
APPID | APPID | no | string | 应用编号 | 
PositionID | B | no | string | 持仓代码 | 
TriggerPrice | TriggerPrice | no | double | 触发价 | 
Reserve | Reserve | no | double | 保留资金 | 

+ **Response**

```
{
	"action": "RecvOrderInsert",
	"errorCode": 0,
	"errorMsg": "Success",
	"result": [			
				{"table":"OrderInsert",
				 "data":{
					,MemberID:"1983723"
					,TradeUnitID:"1983723"
					,AccountID:"1983723"
					,LocalID:"1983723"
					,ExchangeID:"CEX"
					,InstrumentID:"BTCUSD"
					,OrderPriceType:"1"
					,Direction:"1"
					,OffsetFlag:"1"
					,Price:182582.18
					,Volume:182582.18
					,VolumeDisplay:182582.18
					,VolumeMode:"1"
					,Cost:182582.18
					,OrderType:"1"
					,GTDTime:1657543832
					,MinVolume:182582.18
					,BusinessType:"1"
					,BusinessValue:""
					,CloseOrderID:"1983723"
					,IsCrossMargin:1
					,Remark:"my_little_tips"
					,OrderID:"12376422300333"
					,CopyMemberID:"1983723"
					,CopyOrderID:"1983723"
					,Leverage:125
					,CopyProfitRate:182582.18
					,APPID:"ios"
					,PositionID:"1983723"
					,TriggerPrice:182582.18
					,Reserve:182582.18
					}
				}
				{"table":"Order",
				 "data":{
					,MemberID:"1983723"
					,TradeUnitID:"1983723"
					,AccountID:"1983723"
					,LocalID:"1983723"
					,ExchangeID:"CEX"
					,InstrumentID:"BTCUSD"
					,OrderPriceType:"1"
					,Direction:"1"
					,OffsetFlag:"1"
					,Price:182582.18
					,Volume:182582.18
					,VolumeDisplay:182582.18
					,VolumeMode:"1"
					,Cost:182582.18
					,OrderType:"1"
					,GTDTime:1657543832
					,MinVolume:182582.18
					,BusinessType:"1"
					,BusinessValue:""
					,CloseOrderID:"1983723"
					,IsCrossMargin:1
					,Remark:"my_little_tips"
					,OrderID:"12376422300333"
					,CopyMemberID:"1983723"
					,CopyOrderID:"1983723"
					,Leverage:125
					,CopyProfitRate:182582.18
					,APPID:"ios"
					,PositionID:"1983723"
					,TriggerPrice:182582.18
					,Reserve:182582.18
					,OrderStatus:"1"
					,DeriveSource:"1"
					,DeriveDetail:""
					,VolumeTraded:182582.18
					,VolumeRemain:182582.18
					,VolumeCancled:182582.18
					,InsertTime:1657543832
					,UpdateTime:1657543832
					,Priority:0
					,TimeSortNo:1657543832
					,FrontNo:1
					,PriceCurrency:"BTC"
					,FeeCurrency:"BTC"
					,ClearCurrency:"BTC"
					,FrozenMoney:182582.18
					,FrozenFee:182582.18
					,FrozenMargin:182582.18
					,Fee:0.22364
					,CloseProfit:10
					,Turnover:182582.18
					,RelatedOrderID:"1983723"
					,BusinessResult:""
					,BusinessNo:11176422300333
					,Tradable:0
					,SettlementGroup:"999"
					,PosiDirection:"1"
					,TradePrice:182582.18
					,OpenPrice:182582.18
					,TriggerOrderID:"1983723"
					,SLTriggerPrice:182582.18
					,TPTriggerPrice:182582.18
					,CopyProfit:182582.18
					,Position:182582.18
					,UserID:"1983723"
					,LastPriceByInsert:182582.18
					,BidPrice1ByInsert:182582.18
					,AskPrice1ByInsert:182582.18
					,Available:182582.18
					,CreateTime:""
					}
				}
	]
}
```

+ **Response Parameters**

OrderInsert:

parameter | shortName | type | description | remark
-------- | -------- | -------- | -------- | -------- 					
MemberID | M | string | 成员代码 | 
TradeUnitID | T | string | 交易单元代码 | 
AccountID | A | string | 资金账号 | 
LocalID | L | string | 报单本地标识 | 
ExchangeID | E | string | 交易所代码 | 
InstrumentID | I | string | 标的代码 | 
OrderPriceType | PT | string | 报单价格类型 | 0:限价(手动指定Price值);1:任意价(Price按最高最低限价取值);2:最优价/对手价(Price按对方1档价取值);3:五档价(Price按对方5档价取值);4:十档价(类同上);7:最优Marker价(Price按本方1档价或其+1Tick取值);8:Price按委托额Cost和订单簿情况确定值;9:保底点差价:成交价格Price不会劣于系统设置差价成交;
Direction | D | string | 买卖方向 | 0:买;1:卖;
OffsetFlag | O | string | 开平标志 | 0:开仓;1:平仓;2:强平;3:平今;4:平昨;5:全平;6:指定订单平仓;7:指定成交平仓;8:最大平仓;
Price | P | double | 报单价格 | 
Volume | V | double | 数量 | 
VolumeDisplay | D | double | 显示数量 | 
VolumeMode | VM | string | 数量取值方式 | 0:百分比;1:绝对值;
Cost | C | double | 委托额 | 
OrderType | OT | string | 订单类型 | 0:普通订单;1:(FillAndKill/IOC)立即完成并且剩余撤销;2:(FillOrKill)立即完成全部或者最小订单要求否则全部撤销;3:需要进入订单簿被动成交,否则全部撤销(只做Maker/PostOnly);4:不需要撮合直接成交;
GTDTime | GT | double | GTD时间 | 
MinVolume | MV | double | 最小成交量 | 
BusinessType | BT | string | 业务类别 | 0:普通;1:申赎;2:行权;3:中立仓申报;4:递延交割申报;5:互换定单;6:质押;7:合并分拆;8:转股;9:回售回购;a:投票;b:融资融券;c:期权执行;d:传递被平仓订单BusinessValue;P:自定义11;Q:自定义10;R:自定义9;S:连续交易;T:自定义7;U:画线委托;V:限价市价;W:条件;X:止盈止损;Y:追踪出场;Z:指标类型;
BusinessValue | BV | string | 业务值 | 
CloseOrderID | CI | string | 平仓指定开仓的订单号 | 
IsCrossMargin | IsCrossMargin | int | 是否全仓 | 
Remark | R | string | 备注 | 
OrderID | OrderID | string | 报单系统唯一代码 | 
CopyMemberID | CopyMemberID | string | 带单员代码 | 
CopyOrderID | CopyOrderID | string | 带单员报单号 | 
Leverage | Leverage | double | 委托单杠杆倍数 | 
CopyProfitRate | CopyProfitRate | double | 带单分配比例 | 
APPID | APPID | string | 应用编号 | 
PositionID | B | string | 持仓代码 | 
TriggerPrice | TriggerPrice | double | 触发价 | 
Reserve | Reserve | double | 保留资金 | 

Order:

parameter | shortName | type | description | remark
-------- | -------- | -------- | -------- | -------- 					
MemberID | M | string | 成员代码 | 
TradeUnitID | T | string | 交易单元代码 | 
AccountID | A | string | 资金账号 | 
LocalID | L | string | 报单本地标识 | 
ExchangeID | E | string | 交易所代码 | 
InstrumentID | I | string | 标的代码 | 
OrderPriceType | PT | string | 报单价格类型 | 0:限价(手动指定Price值);1:任意价(Price按最高最低限价取值);2:最优价/对手价(Price按对方1档价取值);3:五档价(Price按对方5档价取值);4:十档价(类同上);7:最优Marker价(Price按本方1档价或其+1Tick取值);8:Price按委托额Cost和订单簿情况确定值;9:保底点差价:成交价格Price不会劣于系统设置差价成交;
Direction | D | string | 买卖方向 | 0:买;1:卖;
OffsetFlag | O | string | 开平标志 | 0:开仓;1:平仓;2:强平;3:平今;4:平昨;5:全平;6:指定订单平仓;7:指定成交平仓;8:最大平仓;
Price | P | double | 报单价格 | 
Volume | V | double | 数量 | 
VolumeDisplay | D | double | 显示数量 | 
VolumeMode | VM | string | 数量取值方式 | 0:百分比;1:绝对值;
Cost | C | double | 委托额 | 
OrderType | OT | string | 订单类型 | 0:普通订单;1:(FillAndKill/IOC)立即完成并且剩余撤销;2:(FillOrKill)立即完成全部或者最小订单要求否则全部撤销;3:需要进入订单簿被动成交,否则全部撤销(只做Maker/PostOnly);4:不需要撮合直接成交;
GTDTime | GT | double | GTD时间 | 
MinVolume | MV | double | 最小成交量 | 
BusinessType | BT | string | 业务类别 | 0:普通;1:申赎;2:行权;3:中立仓申报;4:递延交割申报;5:互换定单;6:质押;7:合并分拆;8:转股;9:回售回购;a:投票;b:融资融券;c:期权执行;d:传递被平仓订单BusinessValue;P:自定义11;Q:自定义10;R:自定义9;S:连续交易;T:自定义7;U:画线委托;V:限价市价;W:条件;X:止盈止损;Y:追踪出场;Z:指标类型;
BusinessValue | BV | string | 业务值 | 
CloseOrderID | CI | string | 平仓指定开仓的订单号 | 
IsCrossMargin | IsCrossMargin | int | 是否全仓 | 
Remark | R | string | 备注 | 
OrderID | OrderID | string | 报单系统唯一代码 | 
CopyMemberID | CopyMemberID | string | 带单员代码 | 
CopyOrderID | CopyOrderID | string | 带单员报单号 | 
Leverage | Leverage | double | 委托单杠杆倍数 | 
CopyProfitRate | CopyProfitRate | double | 带单分配比例 | 
APPID | APPID | string | 应用编号 | 
PositionID | B | string | 持仓代码 | 
TriggerPrice | TriggerPrice | double | 触发价 | 
Reserve | Reserve | double | 保留资金 | 
OrderStatus | OrderStatus | string | 报单状态 | 0:未设置状态;1:全部成交;2:部分成交未撤单;3:部分成交已撤单;4:未成交未撤单;6:无成交已撤单;
DeriveSource | DeriveSource | string | 衍生来源 | 0:正常;1:报价衍生;2:期权执行;3:组合衍生;4:场外成交强平衍生;5:场外成交大宗交易衍生;6:场外成交期转现衍生;7:场外成交只刷单衍生;8:触发订单衍生;9:止损触发订单衍生;a:止盈触发订单衍生;b:资金操作衍生;c:仓位合并衍生;d:CFD订单衍生;e:场外成交ADL衍生;f:回购手续费不能是币;
DeriveDetail | DeriveDetail | string | 衍生明细 | 
VolumeTraded | VolumeTraded | double | 成交数量 | 
VolumeRemain | VolumeRemain | double | 剩余数量 | 
VolumeCancled | VolumeCancled | double | 已经撤单数量 | 
InsertTime | InsertTime | double | 插入时间 | 
UpdateTime | UpdateTime | double | 更新时间 | 
Priority |  | int | 优先权 | 
TimeSortNo |  | double | 按时间排队的序号 | 
FrontNo | FrontNo | int | 前置编号 | 
PriceCurrency | PriceCurrency | string | 计价币种 | 
FeeCurrency | FeeCurrency | string | 手续费币种 | 
ClearCurrency | ClearCurrency | string | 清算币种 | 
FrozenMoney | FrozenMoney | double | 冻结资金 | 
FrozenFee | FrozenFee | double | 冻结手续费 | 
FrozenMargin | FrozenMargin | double | 冻结保证金 | 
Fee | Fee | double | 手续费 | 
CloseProfit | CloseProfit | double | 平仓盈亏 | 
Turnover | Turnover | double | 成交金额 | 
RelatedOrderID | RelatedOrderID | string | 关联报单号 | 
BusinessResult | BusinessResult | string | 业务执行结果 | 
BusinessNo | BusinessNo | double | 业务序列号 | 
Tradable |  | int | 是否可以参加交易 | 
SettlementGroup |  | string | 结算组编号 | 
PosiDirection | PosiDirection | string | 持仓多空方向 | 0:多头;1:空头;2:净;
TradePrice | TradePrice | double | 成交均价 | 
OpenPrice | OpenPrice | double | 平仓成交时的开仓均价 | 
TriggerOrderID | TriggerOrderID | string | 触发报单号 | 
SLTriggerPrice | SLTriggerPrice | double | 开仓报单成交之后的止损触发价 | 
TPTriggerPrice | TPTriggerPrice | double | 开仓报单成交之后的止盈触发价 | 
CopyProfit | CopyProfit | double | 带单盈利分配 | 
Position | Position | double | 成交时的持仓量 | 
UserID | U | string | 交易用户代码 | 
LastPriceByInsert |  | double | 插入时的最新价 | 
BidPrice1ByInsert |  | double | 插入时的买一价 | 
AskPrice1ByInsert |  | double | 插入时的卖一价 | 
Available | Available | double | 可用资金 | 
CreateTime |  | string | 创建时间 | 


## **报单操作请求**

+ **Request URL**

`POST /action/v1.0/SendOrderAction`


+ **Request Post Body**

```
{
	,"OrderID":"12376422300333"
	,"OrderLocalID":"1983723"
	,"ActionFlag":"1"
	,"LocalID":"1983723"
	,"MemberID":"1983723"
	,"AccountID":"1983723"
	,"Price":182582.18
	,"Volume":182582.18
	,"Direction":"1"
	,"VolumeDisplay":182582.18
	,"ProductGroup":"Swap"
	,"ExchangeID":"CEX"
	,"InstrumentID":"BTCUSD"
	,"Cost":182582.18
	,"Remark":"my_little_tips"
}
```
+ **curl Example**

````
curl -d '{"OrderID":"12376422300333","OrderLocalID":"1983723","ActionFlag":"1","LocalID":"1983723","MemberID":"1983723","AccountID":"1983723","Price":"182582.18,"Volume":"182582.18,"Direction":"1","VolumeDisplay":"182582.18,"ProductGroup":"Swap","ExchangeID":"CEX","InstrumentID":"BTCUSD","Cost":"182582.18,"Remark":"my_little_tips"}'  -X POST  http://127.0.0.1:10000/action/v1.0/SendOrderAction
````
+ **ws Example**

````
{"RequestNo":2,"SendOrderAction":{"OrderID":"12376422300333","OrderLocalID":"1983723","ActionFlag":"1","LocalID":"1983723","MemberID":"1983723","AccountID":"1983723","Price":"182582.18,"Volume":"182582.18,"Direction":"1","VolumeDisplay":"182582.18,"ProductGroup":"Swap","ExchangeID":"CEX","InstrumentID":"BTCUSD","Cost":"182582.18,"Remark":"my_little_tips"}}
````
+ **Web rowser Example**

````
http://127.0.0.1:10000/action/v1.0/SendOrderAction?OrderID=12376422300333&OrderLocalID=1983723&ActionFlag=1&LocalID=1983723&MemberID=1983723&AccountID=1983723&Price=182582.18&Volume=182582.18&Direction=1&VolumeDisplay=182582.18&ProductGroup=Swap&ExchangeID=CEX&InstrumentID=BTCUSD&Cost=182582.18&Remark=my_little_tips
````
+ **Request Parameters**

parameter | shortName | required | type | description | remark
-------- | -------- | -------- | -------- | -------- | --------
OrderID | OrderID | no | string | 报单系统唯一代码 | 
OrderLocalID | OrderLocalID | no | string | 报单本地标识 | 
ActionFlag | ActionFlag | yes | string | 报单操作标志 | 1:删除;2:挂起;3:激活;4:修改;5:触发;6:增减;
LocalID | LocalID | no | string | 操作本地标识 | 
MemberID | MemberID | no | string | 成员代码 | 
AccountID | AccountID | no | string | 资金账号 | 
Price | Price | no | double | 价格 | 
Volume | Volume | no | double | 数量变化 | 
Direction | Direction | no | string | 买卖方向 | 0:买;1:卖;
VolumeDisplay | D | no | double | 显示数量 | 
ProductGroup | ProductGroup | no | string | 产品组 | 
ExchangeID | ExchangeID | no | string | 交易所代码 | 
InstrumentID | InstrumentID | no | string | 标的代码 | 
Cost | C | no | double | 委托额 | 
Remark | Remark | no | string | 备注 | 

+ **Response**

```
{
	"action": "RecvOrderAction",
	"errorCode": 0,
	"errorMsg": "Success",
	"result": [			
				{"table":"OrderAction",
				 "data":{
					,OrderID:"12376422300333"
					,OrderLocalID:"1983723"
					,ActionFlag:"1"
					,LocalID:"1983723"
					,MemberID:"1983723"
					,AccountID:"1983723"
					,Price:182582.18
					,Volume:182582.18
					,Direction:"1"
					,VolumeDisplay:182582.18
					,ProductGroup:"Swap"
					,ExchangeID:"CEX"
					,InstrumentID:"BTCUSD"
					,Cost:182582.18
					,Remark:"my_little_tips"
					}
				}
				{"table":"Order",
				 "data":{
					,MemberID:"1983723"
					,TradeUnitID:"1983723"
					,AccountID:"1983723"
					,LocalID:"1983723"
					,ExchangeID:"CEX"
					,InstrumentID:"BTCUSD"
					,OrderPriceType:"1"
					,Direction:"1"
					,OffsetFlag:"1"
					,Price:182582.18
					,Volume:182582.18
					,VolumeDisplay:182582.18
					,VolumeMode:"1"
					,Cost:182582.18
					,OrderType:"1"
					,GTDTime:1657543832
					,MinVolume:182582.18
					,BusinessType:"1"
					,BusinessValue:""
					,CloseOrderID:"1983723"
					,IsCrossMargin:1
					,Remark:"my_little_tips"
					,OrderID:"12376422300333"
					,CopyMemberID:"1983723"
					,CopyOrderID:"1983723"
					,Leverage:125
					,CopyProfitRate:182582.18
					,APPID:"ios"
					,PositionID:"1983723"
					,TriggerPrice:182582.18
					,Reserve:182582.18
					,OrderStatus:"1"
					,DeriveSource:"1"
					,DeriveDetail:""
					,VolumeTraded:182582.18
					,VolumeRemain:182582.18
					,VolumeCancled:182582.18
					,InsertTime:1657543832
					,UpdateTime:1657543832
					,Priority:0
					,TimeSortNo:1657543832
					,FrontNo:1
					,PriceCurrency:"BTC"
					,FeeCurrency:"BTC"
					,ClearCurrency:"BTC"
					,FrozenMoney:182582.18
					,FrozenFee:182582.18
					,FrozenMargin:182582.18
					,Fee:0.22364
					,CloseProfit:10
					,Turnover:182582.18
					,RelatedOrderID:"1983723"
					,BusinessResult:""
					,BusinessNo:11176422300333
					,Tradable:0
					,SettlementGroup:"999"
					,PosiDirection:"1"
					,TradePrice:182582.18
					,OpenPrice:182582.18
					,TriggerOrderID:"1983723"
					,SLTriggerPrice:182582.18
					,TPTriggerPrice:182582.18
					,CopyProfit:182582.18
					,Position:182582.18
					,UserID:"1983723"
					,LastPriceByInsert:182582.18
					,BidPrice1ByInsert:182582.18
					,AskPrice1ByInsert:182582.18
					,Available:182582.18
					,CreateTime:""
					}
				}
	]
}
```

+ **Response Parameters**

OrderAction:

parameter | shortName | type | description | remark
-------- | -------- | -------- | -------- | -------- 					
OrderID | OrderID | string | 报单系统唯一代码 | 
OrderLocalID | OrderLocalID | string | 报单本地标识 | 
ActionFlag | ActionFlag | string | 报单操作标志 | 1:删除;2:挂起;3:激活;4:修改;5:触发;6:增减;
LocalID | LocalID | string | 操作本地标识 | 
MemberID | MemberID | string | 成员代码 | 
AccountID | AccountID | string | 资金账号 | 
Price | Price | double | 价格 | 
Volume | Volume | double | 数量变化 | 
Direction | Direction | string | 买卖方向 | 0:买;1:卖;
VolumeDisplay | D | double | 显示数量 | 
ProductGroup | ProductGroup | string | 产品组 | 
ExchangeID | ExchangeID | string | 交易所代码 | 
InstrumentID | InstrumentID | string | 标的代码 | 
Cost | C | double | 委托额 | 
Remark | Remark | string | 备注 | 

Order:

parameter | shortName | type | description | remark
-------- | -------- | -------- | -------- | -------- 					
MemberID | M | string | 成员代码 | 
TradeUnitID | T | string | 交易单元代码 | 
AccountID | A | string | 资金账号 | 
LocalID | L | string | 报单本地标识 | 
ExchangeID | E | string | 交易所代码 | 
InstrumentID | I | string | 标的代码 | 
OrderPriceType | PT | string | 报单价格类型 | 0:限价(手动指定Price值);1:任意价(Price按最高最低限价取值);2:最优价/对手价(Price按对方1档价取值);3:五档价(Price按对方5档价取值);4:十档价(类同上);7:最优Marker价(Price按本方1档价或其+1Tick取值);8:Price按委托额Cost和订单簿情况确定值;9:保底点差价:成交价格Price不会劣于系统设置差价成交;
Direction | D | string | 买卖方向 | 0:买;1:卖;
OffsetFlag | O | string | 开平标志 | 0:开仓;1:平仓;2:强平;3:平今;4:平昨;5:全平;6:指定订单平仓;7:指定成交平仓;8:最大平仓;
Price | P | double | 报单价格 | 
Volume | V | double | 数量 | 
VolumeDisplay | D | double | 显示数量 | 
VolumeMode | VM | string | 数量取值方式 | 0:百分比;1:绝对值;
Cost | C | double | 委托额 | 
OrderType | OT | string | 订单类型 | 0:普通订单;1:(FillAndKill/IOC)立即完成并且剩余撤销;2:(FillOrKill)立即完成全部或者最小订单要求否则全部撤销;3:需要进入订单簿被动成交,否则全部撤销(只做Maker/PostOnly);4:不需要撮合直接成交;
GTDTime | GT | double | GTD时间 | 
MinVolume | MV | double | 最小成交量 | 
BusinessType | BT | string | 业务类别 | 0:普通;1:申赎;2:行权;3:中立仓申报;4:递延交割申报;5:互换定单;6:质押;7:合并分拆;8:转股;9:回售回购;a:投票;b:融资融券;c:期权执行;d:传递被平仓订单BusinessValue;P:自定义11;Q:自定义10;R:自定义9;S:连续交易;T:自定义7;U:画线委托;V:限价市价;W:条件;X:止盈止损;Y:追踪出场;Z:指标类型;
BusinessValue | BV | string | 业务值 | 
CloseOrderID | CI | string | 平仓指定开仓的订单号 | 
IsCrossMargin | IsCrossMargin | int | 是否全仓 | 
Remark | R | string | 备注 | 
OrderID | OrderID | string | 报单系统唯一代码 | 
CopyMemberID | CopyMemberID | string | 带单员代码 | 
CopyOrderID | CopyOrderID | string | 带单员报单号 | 
Leverage | Leverage | double | 委托单杠杆倍数 | 
CopyProfitRate | CopyProfitRate | double | 带单分配比例 | 
APPID | APPID | string | 应用编号 | 
PositionID | B | string | 持仓代码 | 
TriggerPrice | TriggerPrice | double | 触发价 | 
Reserve | Reserve | double | 保留资金 | 
OrderStatus | OrderStatus | string | 报单状态 | 0:未设置状态;1:全部成交;2:部分成交未撤单;3:部分成交已撤单;4:未成交未撤单;6:无成交已撤单;
DeriveSource | DeriveSource | string | 衍生来源 | 0:正常;1:报价衍生;2:期权执行;3:组合衍生;4:场外成交强平衍生;5:场外成交大宗交易衍生;6:场外成交期转现衍生;7:场外成交只刷单衍生;8:触发订单衍生;9:止损触发订单衍生;a:止盈触发订单衍生;b:资金操作衍生;c:仓位合并衍生;d:CFD订单衍生;e:场外成交ADL衍生;f:回购手续费不能是币;
DeriveDetail | DeriveDetail | string | 衍生明细 | 
VolumeTraded | VolumeTraded | double | 成交数量 | 
VolumeRemain | VolumeRemain | double | 剩余数量 | 
VolumeCancled | VolumeCancled | double | 已经撤单数量 | 
InsertTime | InsertTime | double | 插入时间 | 
UpdateTime | UpdateTime | double | 更新时间 | 
Priority |  | int | 优先权 | 
TimeSortNo |  | double | 按时间排队的序号 | 
FrontNo | FrontNo | int | 前置编号 | 
PriceCurrency | PriceCurrency | string | 计价币种 | 
FeeCurrency | FeeCurrency | string | 手续费币种 | 
ClearCurrency | ClearCurrency | string | 清算币种 | 
FrozenMoney | FrozenMoney | double | 冻结资金 | 
FrozenFee | FrozenFee | double | 冻结手续费 | 
FrozenMargin | FrozenMargin | double | 冻结保证金 | 
Fee | Fee | double | 手续费 | 
CloseProfit | CloseProfit | double | 平仓盈亏 | 
Turnover | Turnover | double | 成交金额 | 
RelatedOrderID | RelatedOrderID | string | 关联报单号 | 
BusinessResult | BusinessResult | string | 业务执行结果 | 
BusinessNo | BusinessNo | double | 业务序列号 | 
Tradable |  | int | 是否可以参加交易 | 
SettlementGroup |  | string | 结算组编号 | 
PosiDirection | PosiDirection | string | 持仓多空方向 | 0:多头;1:空头;2:净;
TradePrice | TradePrice | double | 成交均价 | 
OpenPrice | OpenPrice | double | 平仓成交时的开仓均价 | 
TriggerOrderID | TriggerOrderID | string | 触发报单号 | 
SLTriggerPrice | SLTriggerPrice | double | 开仓报单成交之后的止损触发价 | 
TPTriggerPrice | TPTriggerPrice | double | 开仓报单成交之后的止盈触发价 | 
CopyProfit | CopyProfit | double | 带单盈利分配 | 
Position | Position | double | 成交时的持仓量 | 
UserID | U | string | 交易用户代码 | 
LastPriceByInsert |  | double | 插入时的最新价 | 
BidPrice1ByInsert |  | double | 插入时的买一价 | 
AskPrice1ByInsert |  | double | 插入时的卖一价 | 
Available | Available | double | 可用资金 | 
CreateTime |  | string | 创建时间 | 


## **触发单录入请求**

+ **Request URL**

`POST /action/v1.0/SendTriggerOrderInsert`


+ **Request Post Body**

```
{
	,"MemberID":"1983723"
	,"TradeUnitID":"1983723"
	,"AccountID":"1983723"
	,"LocalID":"1983723"
	,"ExchangeID":"CEX"
	,"InstrumentID":"BTCUSD"
	,"OrderPriceType":"1"
	,"Direction":"1"
	,"OffsetFlag":"1"
	,"Price":182582.18
	,"Volume":182582.18
	,"VolumeDisplay":182582.18
	,"VolumeMode":"1"
	,"Cost":182582.18
	,"OrderType":"1"
	,"GTDTime":1657543832
	,"MinVolume":182582.18
	,"BusinessType":"1"
	,"BusinessValue":""
	,"CloseOrderID":"1983723"
	,"IsCrossMargin":1
	,"Remark":"my_little_tips"
	,"OrderID":"12376422300333"
	,"CopyMemberID":"1983723"
	,"CopyOrderID":"1983723"
	,"Leverage":125
	,"CopyProfitRate":182582.18
	,"APPID":"ios"
	,"PositionID":"1983723"
	,"TriggerPrice":182582.18
	,"Reserve":182582.18
	,"SLPrice":182582.18
	,"SLTriggerPrice":182582.18
	,"TPPrice":182582.18
	,"TPTriggerPrice":182582.18
	,"RiskBefore":0
	,"TriggerOrderType":"1"
	,"TriggerDetail":""
	,"TriggerPriceType":"1"
	,"TriggerValue":""
	,"CloseSLPrice":182582.18
	,"CloseSLTriggerPrice":182582.18
	,"CloseTPPrice":182582.18
	,"CloseTPTriggerPrice":182582.18
	,"CloseOrderPriceType":"1"
	,"ClosePrice":182582.18
	,"CloseTriggerPrice":182582.18
	,"RelatedOrderID":"1983723"
	,"ActiveTime":1657543832
	,"TriggerTime":1657543832
}
```
+ **curl Example**

````
curl -d '{"MemberID":"1983723","TradeUnitID":"1983723","AccountID":"1983723","LocalID":"1983723","ExchangeID":"CEX","InstrumentID":"BTCUSD","OrderPriceType":"1","Direction":"1","OffsetFlag":"1","Price":"182582.18,"Volume":"182582.18,"VolumeDisplay":"182582.18,"VolumeMode":"1","Cost":"182582.18,"OrderType":"1","GTDTime":"1657543832,"MinVolume":"182582.18,"BusinessType":"1","BusinessValue":"","CloseOrderID":"1983723","IsCrossMargin":"1,"Remark":"my_little_tips","OrderID":"12376422300333","CopyMemberID":"1983723","CopyOrderID":"1983723","Leverage":"125,"CopyProfitRate":"182582.18,"APPID":"ios","PositionID":"1983723","TriggerPrice":"182582.18,"Reserve":"182582.18,"SLPrice":"182582.18,"SLTriggerPrice":"182582.18,"TPPrice":"182582.18,"TPTriggerPrice":"182582.18,"RiskBefore":"0,"TriggerOrderType":"1","TriggerDetail":"","TriggerPriceType":"1","TriggerValue":"","CloseSLPrice":"182582.18,"CloseSLTriggerPrice":"182582.18,"CloseTPPrice":"182582.18,"CloseTPTriggerPrice":"182582.18,"CloseOrderPriceType":"1","ClosePrice":"182582.18,"CloseTriggerPrice":"182582.18,"RelatedOrderID":"1983723","ActiveTime":"1657543832,"TriggerTime":"1657543832}'  -X POST  http://127.0.0.1:10000/action/v1.0/SendTriggerOrderInsert
````
+ **ws Example**

````
{"RequestNo":2,"SendTriggerOrderInsert":{"MemberID":"1983723","TradeUnitID":"1983723","AccountID":"1983723","LocalID":"1983723","ExchangeID":"CEX","InstrumentID":"BTCUSD","OrderPriceType":"1","Direction":"1","OffsetFlag":"1","Price":"182582.18,"Volume":"182582.18,"VolumeDisplay":"182582.18,"VolumeMode":"1","Cost":"182582.18,"OrderType":"1","GTDTime":"1657543832,"MinVolume":"182582.18,"BusinessType":"1","BusinessValue":"","CloseOrderID":"1983723","IsCrossMargin":"1,"Remark":"my_little_tips","OrderID":"12376422300333","CopyMemberID":"1983723","CopyOrderID":"1983723","Leverage":"125,"CopyProfitRate":"182582.18,"APPID":"ios","PositionID":"1983723","TriggerPrice":"182582.18,"Reserve":"182582.18,"SLPrice":"182582.18,"SLTriggerPrice":"182582.18,"TPPrice":"182582.18,"TPTriggerPrice":"182582.18,"RiskBefore":"0,"TriggerOrderType":"1","TriggerDetail":"","TriggerPriceType":"1","TriggerValue":"","CloseSLPrice":"182582.18,"CloseSLTriggerPrice":"182582.18,"CloseTPPrice":"182582.18,"CloseTPTriggerPrice":"182582.18,"CloseOrderPriceType":"1","ClosePrice":"182582.18,"CloseTriggerPrice":"182582.18,"RelatedOrderID":"1983723","ActiveTime":"1657543832,"TriggerTime":"1657543832}}
````
+ **Web rowser Example**

````
http://127.0.0.1:10000/action/v1.0/SendTriggerOrderInsert?MemberID=1983723&TradeUnitID=1983723&AccountID=1983723&LocalID=1983723&ExchangeID=CEX&InstrumentID=BTCUSD&OrderPriceType=1&Direction=1&OffsetFlag=1&Price=182582.18&Volume=182582.18&VolumeDisplay=182582.18&VolumeMode=1&Cost=182582.18&OrderType=1&GTDTime=1657543832&MinVolume=182582.18&BusinessType=1&BusinessValue=&CloseOrderID=1983723&IsCrossMargin=1&Remark=my_little_tips&OrderID=12376422300333&CopyMemberID=1983723&CopyOrderID=1983723&Leverage=125&CopyProfitRate=182582.18&APPID=ios&PositionID=1983723&TriggerPrice=182582.18&Reserve=182582.18&SLPrice=182582.18&SLTriggerPrice=182582.18&TPPrice=182582.18&TPTriggerPrice=182582.18&RiskBefore=0&TriggerOrderType=1&TriggerDetail=&TriggerPriceType=1&TriggerValue=&CloseSLPrice=182582.18&CloseSLTriggerPrice=182582.18&CloseTPPrice=182582.18&CloseTPTriggerPrice=182582.18&CloseOrderPriceType=1&ClosePrice=182582.18&CloseTriggerPrice=182582.18&RelatedOrderID=1983723&ActiveTime=1657543832&TriggerTime=1657543832
````
+ **Request Parameters**

parameter | shortName | required | type | description | remark
-------- | -------- | -------- | -------- | -------- | --------
MemberID | M | no | string | 成员代码 | 
TradeUnitID | T | no | string | 交易单元代码 | 
AccountID | A | no | string | 资金账号 | 
LocalID | L | no | string | 报单本地标识 | 
ExchangeID | E | yes | string | 交易所代码 | 
InstrumentID | I | yes | string | 标的代码 | 
OrderPriceType | PT | yes | string | 报单价格类型 | 0:限价(手动指定Price值);1:任意价(Price按最高最低限价取值);2:最优价/对手价(Price按对方1档价取值);3:五档价(Price按对方5档价取值);4:十档价(类同上);7:最优Marker价(Price按本方1档价或其+1Tick取值);8:Price按委托额Cost和订单簿情况确定值;9:保底点差价:成交价格Price不会劣于系统设置差价成交;
Direction | D | yes | string | 买卖方向 | 0:买;1:卖;
OffsetFlag | O | no | string | 开平标志 | 0:开仓;1:平仓;2:强平;3:平今;4:平昨;5:全平;6:指定订单平仓;7:指定成交平仓;8:最大平仓;
Price | P | no | double | 报单价格 | 
Volume | V | no | double | 数量 | 
VolumeDisplay | D | no | double | 显示数量 | 
VolumeMode | VM | no | string | 数量取值方式 | 0:百分比;1:绝对值;
Cost | C | no | double | 委托额 | 
OrderType | OT | no | string | 订单类型 | 0:普通订单;1:(FillAndKill/IOC)立即完成并且剩余撤销;2:(FillOrKill)立即完成全部或者最小订单要求否则全部撤销;3:需要进入订单簿被动成交,否则全部撤销(只做Maker/PostOnly);4:不需要撮合直接成交;
GTDTime | GT | no | double | GTD时间 | 
MinVolume | MV | no | double | 最小成交量 | 
BusinessType | BT | no | string | 业务类别 | 0:普通;1:申赎;2:行权;3:中立仓申报;4:递延交割申报;5:互换定单;6:质押;7:合并分拆;8:转股;9:回售回购;a:投票;b:融资融券;c:期权执行;d:传递被平仓订单BusinessValue;P:自定义11;Q:自定义10;R:自定义9;S:连续交易;T:自定义7;U:画线委托;V:限价市价;W:条件;X:止盈止损;Y:追踪出场;Z:指标类型;
BusinessValue | BV | no | string | 业务值 | 
CloseOrderID | CI | no | string | 平仓指定开仓的订单号 | 
IsCrossMargin | IsCrossMargin | no | int | 是否全仓 | 
Remark | R | no | string | 备注 | 
OrderID | OrderID | no | string | 报单系统唯一代码 | 
CopyMemberID | CopyMemberID | no | string | 带单员代码 | 
CopyOrderID | CopyOrderID | no | string | 带单员报单号 | 
Leverage | Leverage | no | double | 委托单杠杆倍数 | 
CopyProfitRate | CopyProfitRate | no | double | 带单分配比例 | 
APPID | APPID | no | string | 应用编号 | 
PositionID | B | no | string | 持仓代码 | 
TriggerPrice | TriggerPrice | no | double | 触发价 | 
Reserve | Reserve | no | double | 保留资金 | 
SLPrice | SLPrice | no | double | 止损价 | 
SLTriggerPrice | SLTriggerPrice | no | double | 止损触发价 | 
TPPrice | TPPrice | no | double | 止盈价 | 
TPTriggerPrice | TPTriggerPrice | no | double | 止盈触发价 | 
RiskBefore | RiskBefore | no | int | 是否提前检查风控 | 
TriggerOrderType | TriggerOrderType | no | string | 触发的订单类型 | 0:未定义;1:持仓止盈止损;2:订单止盈止损;3:条件单;4:追踪(跟踪)订单;5:其他算法交易;6:指标类型;7:指标类型止盈止损;8:跟单类型;9:划线下单类型;a:其他类型1;b:其他类型2;c:其他类型3;d:其他类型4;e:其他类型5;
TriggerDetail | TriggerDetail | no | string | 触发类型明细 | 
TriggerPriceType | TriggerPriceType | no | string | 触发价类型 | 0:最新价;1:标记价;2:标的指数价;
TriggerValue | TriggerValue | no | string | 触发单具体设置信息 | 
CloseSLPrice | CloseSLPrice | no | double | 平仓止损价 | 
CloseSLTriggerPrice | CloseSLTriggerPrice | no | double | 平仓止损触发价 | 
CloseTPPrice | CloseTPPrice | no | double | 平仓止盈价 | 
CloseTPTriggerPrice | CloseTPTriggerPrice | no | double | 平仓止盈触发价 | 
CloseOrderPriceType | CloseOrderPriceType | no | string | 报单价格类型 | 0:限价(手动指定Price值);1:任意价(Price按最高最低限价取值);2:最优价/对手价(Price按对方1档价取值);3:五档价(Price按对方5档价取值);4:十档价(类同上);7:最优Marker价(Price按本方1档价或其+1Tick取值);8:Price按委托额Cost和订单簿情况确定值;9:保底点差价:成交价格Price不会劣于系统设置差价成交;
ClosePrice | ClosePrice | no | double | 平仓价 | 
CloseTriggerPrice | CloseTriggerPrice | no | double | 平仓触发价 | 
RelatedOrderID | RelatedOrderID | no | string | 关联报单号 | 
ActiveTime | ActiveTime | no | double | 激活时间 | 
TriggerTime | TriggerTime | no | double | 触发时间 | 

+ **Response**

```
{
	"action": "RecvTriggerOrderInsert",
	"errorCode": 0,
	"errorMsg": "Success",
	"result": [			
				{"table":"TriggerOrderInsert",
				 "data":{
					,MemberID:"1983723"
					,TradeUnitID:"1983723"
					,AccountID:"1983723"
					,LocalID:"1983723"
					,ExchangeID:"CEX"
					,InstrumentID:"BTCUSD"
					,OrderPriceType:"1"
					,Direction:"1"
					,OffsetFlag:"1"
					,Price:182582.18
					,Volume:182582.18
					,VolumeDisplay:182582.18
					,VolumeMode:"1"
					,Cost:182582.18
					,OrderType:"1"
					,GTDTime:1657543832
					,MinVolume:182582.18
					,BusinessType:"1"
					,BusinessValue:""
					,CloseOrderID:"1983723"
					,IsCrossMargin:1
					,Remark:"my_little_tips"
					,OrderID:"12376422300333"
					,CopyMemberID:"1983723"
					,CopyOrderID:"1983723"
					,Leverage:125
					,CopyProfitRate:182582.18
					,APPID:"ios"
					,PositionID:"1983723"
					,TriggerPrice:182582.18
					,Reserve:182582.18
					,SLPrice:182582.18
					,SLTriggerPrice:182582.18
					,TPPrice:182582.18
					,TPTriggerPrice:182582.18
					,RiskBefore:0
					,TriggerOrderType:"1"
					,TriggerDetail:""
					,TriggerPriceType:"1"
					,TriggerValue:""
					,CloseSLPrice:182582.18
					,CloseSLTriggerPrice:182582.18
					,CloseTPPrice:182582.18
					,CloseTPTriggerPrice:182582.18
					,CloseOrderPriceType:"1"
					,ClosePrice:182582.18
					,CloseTriggerPrice:182582.18
					,RelatedOrderID:"1983723"
					,ActiveTime:1657543832
					,TriggerTime:1657543832
					}
				}
				{"table":"TriggerOrder",
				 "data":{
					,MemberID:"1983723"
					,TradeUnitID:"1983723"
					,AccountID:"1983723"
					,LocalID:"1983723"
					,ExchangeID:"CEX"
					,InstrumentID:"BTCUSD"
					,OrderPriceType:"1"
					,Direction:"1"
					,OffsetFlag:"1"
					,Price:182582.18
					,Volume:182582.18
					,VolumeDisplay:182582.18
					,VolumeMode:"1"
					,Cost:182582.18
					,OrderType:"1"
					,GTDTime:1657543832
					,MinVolume:182582.18
					,BusinessType:"1"
					,BusinessValue:""
					,CloseOrderID:"1983723"
					,IsCrossMargin:1
					,Remark:"my_little_tips"
					,OrderID:"12376422300333"
					,CopyMemberID:"1983723"
					,CopyOrderID:"1983723"
					,Leverage:125
					,CopyProfitRate:182582.18
					,APPID:"ios"
					,PositionID:"1983723"
					,TriggerPrice:182582.18
					,Reserve:182582.18
					,SLPrice:182582.18
					,SLTriggerPrice:182582.18
					,TPPrice:182582.18
					,TPTriggerPrice:182582.18
					,RiskBefore:0
					,TriggerOrderType:"1"
					,TriggerDetail:""
					,TriggerPriceType:"1"
					,TriggerValue:""
					,CloseSLPrice:182582.18
					,CloseSLTriggerPrice:182582.18
					,CloseTPPrice:182582.18
					,CloseTPTriggerPrice:182582.18
					,CloseOrderPriceType:"1"
					,ClosePrice:182582.18
					,CloseTriggerPrice:182582.18
					,RelatedOrderID:"1983723"
					,ActiveTime:1657543832
					,TriggerTime:1657543832
					,TimeSortNo:1657543832
					,TriggerStatus:"1"
					,PosiDirection:"1"
					,FrontNo:1
					,ErrorNo:1033201232
					,ErrorMsg:""
					,InsertTime:1657543832
					,UpdateTime:1657543832
					,BusinessNo:11176422300333
					,CreateTime:""
					}
				}
	]
}
```

+ **Response Parameters**

TriggerOrderInsert:

parameter | shortName | type | description | remark
-------- | -------- | -------- | -------- | -------- 					
MemberID | M | string | 成员代码 | 
TradeUnitID | T | string | 交易单元代码 | 
AccountID | A | string | 资金账号 | 
LocalID | L | string | 报单本地标识 | 
ExchangeID | E | string | 交易所代码 | 
InstrumentID | I | string | 标的代码 | 
OrderPriceType | PT | string | 报单价格类型 | 0:限价(手动指定Price值);1:任意价(Price按最高最低限价取值);2:最优价/对手价(Price按对方1档价取值);3:五档价(Price按对方5档价取值);4:十档价(类同上);7:最优Marker价(Price按本方1档价或其+1Tick取值);8:Price按委托额Cost和订单簿情况确定值;9:保底点差价:成交价格Price不会劣于系统设置差价成交;
Direction | D | string | 买卖方向 | 0:买;1:卖;
OffsetFlag | O | string | 开平标志 | 0:开仓;1:平仓;2:强平;3:平今;4:平昨;5:全平;6:指定订单平仓;7:指定成交平仓;8:最大平仓;
Price | P | double | 报单价格 | 
Volume | V | double | 数量 | 
VolumeDisplay | D | double | 显示数量 | 
VolumeMode | VM | string | 数量取值方式 | 0:百分比;1:绝对值;
Cost | C | double | 委托额 | 
OrderType | OT | string | 订单类型 | 0:普通订单;1:(FillAndKill/IOC)立即完成并且剩余撤销;2:(FillOrKill)立即完成全部或者最小订单要求否则全部撤销;3:需要进入订单簿被动成交,否则全部撤销(只做Maker/PostOnly);4:不需要撮合直接成交;
GTDTime | GT | double | GTD时间 | 
MinVolume | MV | double | 最小成交量 | 
BusinessType | BT | string | 业务类别 | 0:普通;1:申赎;2:行权;3:中立仓申报;4:递延交割申报;5:互换定单;6:质押;7:合并分拆;8:转股;9:回售回购;a:投票;b:融资融券;c:期权执行;d:传递被平仓订单BusinessValue;P:自定义11;Q:自定义10;R:自定义9;S:连续交易;T:自定义7;U:画线委托;V:限价市价;W:条件;X:止盈止损;Y:追踪出场;Z:指标类型;
BusinessValue | BV | string | 业务值 | 
CloseOrderID | CI | string | 平仓指定开仓的订单号 | 
IsCrossMargin | IsCrossMargin | int | 是否全仓 | 
Remark | R | string | 备注 | 
OrderID | OrderID | string | 报单系统唯一代码 | 
CopyMemberID | CopyMemberID | string | 带单员代码 | 
CopyOrderID | CopyOrderID | string | 带单员报单号 | 
Leverage | Leverage | double | 委托单杠杆倍数 | 
CopyProfitRate | CopyProfitRate | double | 带单分配比例 | 
APPID | APPID | string | 应用编号 | 
PositionID | B | string | 持仓代码 | 
TriggerPrice | TriggerPrice | double | 触发价 | 
Reserve | Reserve | double | 保留资金 | 
SLPrice | SLPrice | double | 止损价 | 
SLTriggerPrice | SLTriggerPrice | double | 止损触发价 | 
TPPrice | TPPrice | double | 止盈价 | 
TPTriggerPrice | TPTriggerPrice | double | 止盈触发价 | 
RiskBefore | RiskBefore | int | 是否提前检查风控 | 
TriggerOrderType | TriggerOrderType | string | 触发的订单类型 | 0:未定义;1:持仓止盈止损;2:订单止盈止损;3:条件单;4:追踪(跟踪)订单;5:其他算法交易;6:指标类型;7:指标类型止盈止损;8:跟单类型;9:划线下单类型;a:其他类型1;b:其他类型2;c:其他类型3;d:其他类型4;e:其他类型5;
TriggerDetail | TriggerDetail | string | 触发类型明细 | 
TriggerPriceType | TriggerPriceType | string | 触发价类型 | 0:最新价;1:标记价;2:标的指数价;
TriggerValue | TriggerValue | string | 触发单具体设置信息 | 
CloseSLPrice | CloseSLPrice | double | 平仓止损价 | 
CloseSLTriggerPrice | CloseSLTriggerPrice | double | 平仓止损触发价 | 
CloseTPPrice | CloseTPPrice | double | 平仓止盈价 | 
CloseTPTriggerPrice | CloseTPTriggerPrice | double | 平仓止盈触发价 | 
CloseOrderPriceType | CloseOrderPriceType | string | 报单价格类型 | 0:限价(手动指定Price值);1:任意价(Price按最高最低限价取值);2:最优价/对手价(Price按对方1档价取值);3:五档价(Price按对方5档价取值);4:十档价(类同上);7:最优Marker价(Price按本方1档价或其+1Tick取值);8:Price按委托额Cost和订单簿情况确定值;9:保底点差价:成交价格Price不会劣于系统设置差价成交;
ClosePrice | ClosePrice | double | 平仓价 | 
CloseTriggerPrice | CloseTriggerPrice | double | 平仓触发价 | 
RelatedOrderID | RelatedOrderID | string | 关联报单号 | 
ActiveTime | ActiveTime | double | 激活时间 | 
TriggerTime | TriggerTime | double | 触发时间 | 

TriggerOrder:

parameter | shortName | type | description | remark
-------- | -------- | -------- | -------- | -------- 					
MemberID | M | string | 成员代码 | 
TradeUnitID | T | string | 交易单元代码 | 
AccountID | A | string | 资金账号 | 
LocalID | L | string | 报单本地标识 | 
ExchangeID | E | string | 交易所代码 | 
InstrumentID | I | string | 标的代码 | 
OrderPriceType | PT | string | 报单价格类型 | 0:限价(手动指定Price值);1:任意价(Price按最高最低限价取值);2:最优价/对手价(Price按对方1档价取值);3:五档价(Price按对方5档价取值);4:十档价(类同上);7:最优Marker价(Price按本方1档价或其+1Tick取值);8:Price按委托额Cost和订单簿情况确定值;9:保底点差价:成交价格Price不会劣于系统设置差价成交;
Direction | D | string | 买卖方向 | 0:买;1:卖;
OffsetFlag | O | string | 开平标志 | 0:开仓;1:平仓;2:强平;3:平今;4:平昨;5:全平;6:指定订单平仓;7:指定成交平仓;8:最大平仓;
Price | P | double | 报单价格 | 
Volume | V | double | 数量 | 
VolumeDisplay | D | double | 显示数量 | 
VolumeMode | VM | string | 数量取值方式 | 0:百分比;1:绝对值;
Cost | C | double | 委托额 | 
OrderType | OT | string | 订单类型 | 0:普通订单;1:(FillAndKill/IOC)立即完成并且剩余撤销;2:(FillOrKill)立即完成全部或者最小订单要求否则全部撤销;3:需要进入订单簿被动成交,否则全部撤销(只做Maker/PostOnly);4:不需要撮合直接成交;
GTDTime | GT | double | GTD时间 | 
MinVolume | MV | double | 最小成交量 | 
BusinessType | BT | string | 业务类别 | 0:普通;1:申赎;2:行权;3:中立仓申报;4:递延交割申报;5:互换定单;6:质押;7:合并分拆;8:转股;9:回售回购;a:投票;b:融资融券;c:期权执行;d:传递被平仓订单BusinessValue;P:自定义11;Q:自定义10;R:自定义9;S:连续交易;T:自定义7;U:画线委托;V:限价市价;W:条件;X:止盈止损;Y:追踪出场;Z:指标类型;
BusinessValue | BV | string | 业务值 | 
CloseOrderID | CI | string | 平仓指定开仓的订单号 | 
IsCrossMargin | IsCrossMargin | int | 是否全仓 | 
Remark | R | string | 备注 | 
OrderID | OrderID | string | 报单系统唯一代码 | 
CopyMemberID | CopyMemberID | string | 带单员代码 | 
CopyOrderID | CopyOrderID | string | 带单员报单号 | 
Leverage | Leverage | double | 委托单杠杆倍数 | 
CopyProfitRate | CopyProfitRate | double | 带单分配比例 | 
APPID | APPID | string | 应用编号 | 
PositionID | B | string | 持仓代码 | 
TriggerPrice | TriggerPrice | double | 触发价 | 
Reserve | Reserve | double | 保留资金 | 
SLPrice | SLPrice | double | 止损价 | 
SLTriggerPrice | SLTriggerPrice | double | 止损触发价 | 
TPPrice | TPPrice | double | 止盈价 | 
TPTriggerPrice | TPTriggerPrice | double | 止盈触发价 | 
RiskBefore | RiskBefore | int | 是否提前检查风控 | 
TriggerOrderType | TriggerOrderType | string | 触发的订单类型 | 0:未定义;1:持仓止盈止损;2:订单止盈止损;3:条件单;4:追踪(跟踪)订单;5:其他算法交易;6:指标类型;7:指标类型止盈止损;8:跟单类型;9:划线下单类型;a:其他类型1;b:其他类型2;c:其他类型3;d:其他类型4;e:其他类型5;
TriggerDetail | TriggerDetail | string | 触发类型明细 | 
TriggerPriceType | TriggerPriceType | string | 触发价类型 | 0:最新价;1:标记价;2:标的指数价;
TriggerValue | TriggerValue | string | 触发单具体设置信息 | 
CloseSLPrice | CloseSLPrice | double | 平仓止损价 | 
CloseSLTriggerPrice | CloseSLTriggerPrice | double | 平仓止损触发价 | 
CloseTPPrice | CloseTPPrice | double | 平仓止盈价 | 
CloseTPTriggerPrice | CloseTPTriggerPrice | double | 平仓止盈触发价 | 
CloseOrderPriceType | CloseOrderPriceType | string | 报单价格类型 | 0:限价(手动指定Price值);1:任意价(Price按最高最低限价取值);2:最优价/对手价(Price按对方1档价取值);3:五档价(Price按对方5档价取值);4:十档价(类同上);7:最优Marker价(Price按本方1档价或其+1Tick取值);8:Price按委托额Cost和订单簿情况确定值;9:保底点差价:成交价格Price不会劣于系统设置差价成交;
ClosePrice | ClosePrice | double | 平仓价 | 
CloseTriggerPrice | CloseTriggerPrice | double | 平仓触发价 | 
RelatedOrderID | RelatedOrderID | string | 关联报单号 | 
ActiveTime | ActiveTime | double | 激活时间 | 
TriggerTime | TriggerTime | double | 触发时间 | 
TimeSortNo | TimeSortNo | double | 按时间排队的序号 | 
TriggerStatus | TriggerStatus | string | 触发报单状态 | 0:未设置状态;1:活跃(运行)状态;2:已经触发;3:触发失败;4:撤单;
PosiDirection | PosiDirection | string | 持仓多空方向 | 0:多头;1:空头;2:净;
FrontNo | FrontNo | int | 前置编号 | 
ErrorNo | ErrorNo | int | 错误代码 | 
ErrorMsg | ErrorMsg | string | 错误信息 | 
InsertTime | InsertTime | double | 插入时间 | 
UpdateTime | UpdateTime | double | 更新时间 | 
BusinessNo | BusinessNo | double | 业务序列号 | 
CreateTime | CreateTime | string | 创建时间 | 


## **触发单操作请求**

+ **Request URL**

`POST /action/v1.0/SendTriggerOrderAction`


+ **Request Post Body**

```
{
	,"OrderID":"12376422300333"
	,"OrderLocalID":"1983723"
	,"ActionFlag":"1"
	,"LocalID":"1983723"
	,"MemberID":"1983723"
	,"AccountID":"1983723"
	,"Price":182582.18
	,"Volume":182582.18
	,"Direction":"1"
	,"VolumeDisplay":182582.18
	,"ProductGroup":"Swap"
	,"ExchangeID":"CEX"
	,"InstrumentID":"BTCUSD"
	,"Cost":182582.18
	,"Remark":"my_little_tips"
	,"SLPrice":182582.18
	,"SLTriggerPrice":182582.18
	,"TPPrice":182582.18
	,"TPTriggerPrice":182582.18
	,"TriggerPrice":182582.18
	,"TriggerOrderType":""
	,"TriggerValue":""
	,"TriggerDetail":""
	,"TriggerPriceType":"1"
	,"ErrorNo":1033201232
	,"ErrorMsg":""
}
```
+ **curl Example**

````
curl -d '{"OrderID":"12376422300333","OrderLocalID":"1983723","ActionFlag":"1","LocalID":"1983723","MemberID":"1983723","AccountID":"1983723","Price":"182582.18,"Volume":"182582.18,"Direction":"1","VolumeDisplay":"182582.18,"ProductGroup":"Swap","ExchangeID":"CEX","InstrumentID":"BTCUSD","Cost":"182582.18,"Remark":"my_little_tips","SLPrice":"182582.18,"SLTriggerPrice":"182582.18,"TPPrice":"182582.18,"TPTriggerPrice":"182582.18,"TriggerPrice":"182582.18,"TriggerOrderType":"","TriggerValue":"","TriggerDetail":"","TriggerPriceType":"1","ErrorNo":"1033201232,"ErrorMsg":""}'  -X POST  http://127.0.0.1:10000/action/v1.0/SendTriggerOrderAction
````
+ **ws Example**

````
{"RequestNo":2,"SendTriggerOrderAction":{"OrderID":"12376422300333","OrderLocalID":"1983723","ActionFlag":"1","LocalID":"1983723","MemberID":"1983723","AccountID":"1983723","Price":"182582.18,"Volume":"182582.18,"Direction":"1","VolumeDisplay":"182582.18,"ProductGroup":"Swap","ExchangeID":"CEX","InstrumentID":"BTCUSD","Cost":"182582.18,"Remark":"my_little_tips","SLPrice":"182582.18,"SLTriggerPrice":"182582.18,"TPPrice":"182582.18,"TPTriggerPrice":"182582.18,"TriggerPrice":"182582.18,"TriggerOrderType":"","TriggerValue":"","TriggerDetail":"","TriggerPriceType":"1","ErrorNo":"1033201232,"ErrorMsg":""}}
````
+ **Web rowser Example**

````
http://127.0.0.1:10000/action/v1.0/SendTriggerOrderAction?OrderID=12376422300333&OrderLocalID=1983723&ActionFlag=1&LocalID=1983723&MemberID=1983723&AccountID=1983723&Price=182582.18&Volume=182582.18&Direction=1&VolumeDisplay=182582.18&ProductGroup=Swap&ExchangeID=CEX&InstrumentID=BTCUSD&Cost=182582.18&Remark=my_little_tips&SLPrice=182582.18&SLTriggerPrice=182582.18&TPPrice=182582.18&TPTriggerPrice=182582.18&TriggerPrice=182582.18&TriggerOrderType=&TriggerValue=&TriggerDetail=&TriggerPriceType=1&ErrorNo=1033201232&ErrorMsg=
````
+ **Request Parameters**

parameter | shortName | required | type | description | remark
-------- | -------- | -------- | -------- | -------- | --------
OrderID | OrderID | no | string | 报单系统唯一代码 | 
OrderLocalID | OrderLocalID | no | string | 报单本地标识 | 
ActionFlag | ActionFlag | yes | string | 报单操作标志 | 1:删除;2:挂起;3:激活;4:修改;5:触发;6:增减;
LocalID | LocalID | no | string | 操作本地标识 | 
MemberID | MemberID | no | string | 成员代码 | 
AccountID | AccountID | no | string | 资金账号 | 
Price | Price | no | double | 价格 | 
Volume | Volume | no | double | 数量变化 | 
Direction | Direction | no | string | 买卖方向 | 0:买;1:卖;
VolumeDisplay | D | no | double | 显示数量 | 
ProductGroup | ProductGroup | no | string | 产品组 | 
ExchangeID | ExchangeID | no | string | 交易所代码 | 
InstrumentID | InstrumentID | no | string | 标的代码 | 
Cost | C | no | double | 委托额 | 
Remark | Remark | no | string | 备注 | 
SLPrice | SLPrice | no | double | 止损价 | 
SLTriggerPrice | SLTriggerPrice | no | double | 止损触发价 | 
TPPrice | TPPrice | no | double | 止盈价 | 
TPTriggerPrice | TPTriggerPrice | no | double | 止盈触发价 | 
TriggerPrice | TriggerPrice | no | double | 触发价 | 
TriggerOrderType | TriggerOrderType | no | string | 触发的订单类型 | 
TriggerValue | TriggerValue | no | string | 触发单具体设置信息 | 
TriggerDetail | TriggerDetail | no | string | 触发类型明细 | 
TriggerPriceType | TriggerPriceType | no | string | 触发价类型 | 0:最新价;1:标记价;2:标的指数价;
ErrorNo | ErrorNo | no | int | 错误代码 | 
ErrorMsg | ErrorMsg | no | string | 错误信息 | 

+ **Response**

```
{
	"action": "RecvTriggerOrderAction",
	"errorCode": 0,
	"errorMsg": "Success",
	"result": [			
				{"table":"TriggerOrderAction",
				 "data":{
					,OrderID:"12376422300333"
					,OrderLocalID:"1983723"
					,ActionFlag:"1"
					,LocalID:"1983723"
					,MemberID:"1983723"
					,AccountID:"1983723"
					,Price:182582.18
					,Volume:182582.18
					,Direction:"1"
					,VolumeDisplay:182582.18
					,ProductGroup:"Swap"
					,ExchangeID:"CEX"
					,InstrumentID:"BTCUSD"
					,Cost:182582.18
					,Remark:"my_little_tips"
					,SLPrice:182582.18
					,SLTriggerPrice:182582.18
					,TPPrice:182582.18
					,TPTriggerPrice:182582.18
					,TriggerPrice:182582.18
					,TriggerOrderType:""
					,TriggerValue:""
					,TriggerDetail:""
					,TriggerPriceType:"1"
					,ErrorNo:1033201232
					,ErrorMsg:""
					}
				}
				{"table":"TriggerOrder",
				 "data":{
					,MemberID:"1983723"
					,TradeUnitID:"1983723"
					,AccountID:"1983723"
					,LocalID:"1983723"
					,ExchangeID:"CEX"
					,InstrumentID:"BTCUSD"
					,OrderPriceType:"1"
					,Direction:"1"
					,OffsetFlag:"1"
					,Price:182582.18
					,Volume:182582.18
					,VolumeDisplay:182582.18
					,VolumeMode:"1"
					,Cost:182582.18
					,OrderType:"1"
					,GTDTime:1657543832
					,MinVolume:182582.18
					,BusinessType:"1"
					,BusinessValue:""
					,CloseOrderID:"1983723"
					,IsCrossMargin:1
					,Remark:"my_little_tips"
					,OrderID:"12376422300333"
					,CopyMemberID:"1983723"
					,CopyOrderID:"1983723"
					,Leverage:125
					,CopyProfitRate:182582.18
					,APPID:"ios"
					,PositionID:"1983723"
					,TriggerPrice:182582.18
					,Reserve:182582.18
					,SLPrice:182582.18
					,SLTriggerPrice:182582.18
					,TPPrice:182582.18
					,TPTriggerPrice:182582.18
					,RiskBefore:0
					,TriggerOrderType:"1"
					,TriggerDetail:""
					,TriggerPriceType:"1"
					,TriggerValue:""
					,CloseSLPrice:182582.18
					,CloseSLTriggerPrice:182582.18
					,CloseTPPrice:182582.18
					,CloseTPTriggerPrice:182582.18
					,CloseOrderPriceType:"1"
					,ClosePrice:182582.18
					,CloseTriggerPrice:182582.18
					,RelatedOrderID:"1983723"
					,ActiveTime:1657543832
					,TriggerTime:1657543832
					,TimeSortNo:1657543832
					,TriggerStatus:"1"
					,PosiDirection:"1"
					,FrontNo:1
					,ErrorNo:1033201232
					,ErrorMsg:""
					,InsertTime:1657543832
					,UpdateTime:1657543832
					,BusinessNo:11176422300333
					,CreateTime:""
					}
				}
	]
}
```

+ **Response Parameters**

TriggerOrderAction:

parameter | shortName | type | description | remark
-------- | -------- | -------- | -------- | -------- 					
OrderID | OrderID | string | 报单系统唯一代码 | 
OrderLocalID | OrderLocalID | string | 报单本地标识 | 
ActionFlag | ActionFlag | string | 报单操作标志 | 1:删除;2:挂起;3:激活;4:修改;5:触发;6:增减;
LocalID | LocalID | string | 操作本地标识 | 
MemberID | MemberID | string | 成员代码 | 
AccountID | AccountID | string | 资金账号 | 
Price | Price | double | 价格 | 
Volume | Volume | double | 数量变化 | 
Direction | Direction | string | 买卖方向 | 0:买;1:卖;
VolumeDisplay | D | double | 显示数量 | 
ProductGroup | ProductGroup | string | 产品组 | 
ExchangeID | ExchangeID | string | 交易所代码 | 
InstrumentID | InstrumentID | string | 标的代码 | 
Cost | C | double | 委托额 | 
Remark | Remark | string | 备注 | 
SLPrice | SLPrice | double | 止损价 | 
SLTriggerPrice | SLTriggerPrice | double | 止损触发价 | 
TPPrice | TPPrice | double | 止盈价 | 
TPTriggerPrice | TPTriggerPrice | double | 止盈触发价 | 
TriggerPrice | TriggerPrice | double | 触发价 | 
TriggerOrderType | TriggerOrderType | string | 触发的订单类型 | 
TriggerValue | TriggerValue | string | 触发单具体设置信息 | 
TriggerDetail | TriggerDetail | string | 触发类型明细 | 
TriggerPriceType | TriggerPriceType | string | 触发价类型 | 0:最新价;1:标记价;2:标的指数价;
ErrorNo | ErrorNo | int | 错误代码 | 
ErrorMsg | ErrorMsg | string | 错误信息 | 

TriggerOrder:

parameter | shortName | type | description | remark
-------- | -------- | -------- | -------- | -------- 					
MemberID | M | string | 成员代码 | 
TradeUnitID | T | string | 交易单元代码 | 
AccountID | A | string | 资金账号 | 
LocalID | L | string | 报单本地标识 | 
ExchangeID | E | string | 交易所代码 | 
InstrumentID | I | string | 标的代码 | 
OrderPriceType | PT | string | 报单价格类型 | 0:限价(手动指定Price值);1:任意价(Price按最高最低限价取值);2:最优价/对手价(Price按对方1档价取值);3:五档价(Price按对方5档价取值);4:十档价(类同上);7:最优Marker价(Price按本方1档价或其+1Tick取值);8:Price按委托额Cost和订单簿情况确定值;9:保底点差价:成交价格Price不会劣于系统设置差价成交;
Direction | D | string | 买卖方向 | 0:买;1:卖;
OffsetFlag | O | string | 开平标志 | 0:开仓;1:平仓;2:强平;3:平今;4:平昨;5:全平;6:指定订单平仓;7:指定成交平仓;8:最大平仓;
Price | P | double | 报单价格 | 
Volume | V | double | 数量 | 
VolumeDisplay | D | double | 显示数量 | 
VolumeMode | VM | string | 数量取值方式 | 0:百分比;1:绝对值;
Cost | C | double | 委托额 | 
OrderType | OT | string | 订单类型 | 0:普通订单;1:(FillAndKill/IOC)立即完成并且剩余撤销;2:(FillOrKill)立即完成全部或者最小订单要求否则全部撤销;3:需要进入订单簿被动成交,否则全部撤销(只做Maker/PostOnly);4:不需要撮合直接成交;
GTDTime | GT | double | GTD时间 | 
MinVolume | MV | double | 最小成交量 | 
BusinessType | BT | string | 业务类别 | 0:普通;1:申赎;2:行权;3:中立仓申报;4:递延交割申报;5:互换定单;6:质押;7:合并分拆;8:转股;9:回售回购;a:投票;b:融资融券;c:期权执行;d:传递被平仓订单BusinessValue;P:自定义11;Q:自定义10;R:自定义9;S:连续交易;T:自定义7;U:画线委托;V:限价市价;W:条件;X:止盈止损;Y:追踪出场;Z:指标类型;
BusinessValue | BV | string | 业务值 | 
CloseOrderID | CI | string | 平仓指定开仓的订单号 | 
IsCrossMargin | IsCrossMargin | int | 是否全仓 | 
Remark | R | string | 备注 | 
OrderID | OrderID | string | 报单系统唯一代码 | 
CopyMemberID | CopyMemberID | string | 带单员代码 | 
CopyOrderID | CopyOrderID | string | 带单员报单号 | 
Leverage | Leverage | double | 委托单杠杆倍数 | 
CopyProfitRate | CopyProfitRate | double | 带单分配比例 | 
APPID | APPID | string | 应用编号 | 
PositionID | B | string | 持仓代码 | 
TriggerPrice | TriggerPrice | double | 触发价 | 
Reserve | Reserve | double | 保留资金 | 
SLPrice | SLPrice | double | 止损价 | 
SLTriggerPrice | SLTriggerPrice | double | 止损触发价 | 
TPPrice | TPPrice | double | 止盈价 | 
TPTriggerPrice | TPTriggerPrice | double | 止盈触发价 | 
RiskBefore | RiskBefore | int | 是否提前检查风控 | 
TriggerOrderType | TriggerOrderType | string | 触发的订单类型 | 0:未定义;1:持仓止盈止损;2:订单止盈止损;3:条件单;4:追踪(跟踪)订单;5:其他算法交易;6:指标类型;7:指标类型止盈止损;8:跟单类型;9:划线下单类型;a:其他类型1;b:其他类型2;c:其他类型3;d:其他类型4;e:其他类型5;
TriggerDetail | TriggerDetail | string | 触发类型明细 | 
TriggerPriceType | TriggerPriceType | string | 触发价类型 | 0:最新价;1:标记价;2:标的指数价;
TriggerValue | TriggerValue | string | 触发单具体设置信息 | 
CloseSLPrice | CloseSLPrice | double | 平仓止损价 | 
CloseSLTriggerPrice | CloseSLTriggerPrice | double | 平仓止损触发价 | 
CloseTPPrice | CloseTPPrice | double | 平仓止盈价 | 
CloseTPTriggerPrice | CloseTPTriggerPrice | double | 平仓止盈触发价 | 
CloseOrderPriceType | CloseOrderPriceType | string | 报单价格类型 | 0:限价(手动指定Price值);1:任意价(Price按最高最低限价取值);2:最优价/对手价(Price按对方1档价取值);3:五档价(Price按对方5档价取值);4:十档价(类同上);7:最优Marker价(Price按本方1档价或其+1Tick取值);8:Price按委托额Cost和订单簿情况确定值;9:保底点差价:成交价格Price不会劣于系统设置差价成交;
ClosePrice | ClosePrice | double | 平仓价 | 
CloseTriggerPrice | CloseTriggerPrice | double | 平仓触发价 | 
RelatedOrderID | RelatedOrderID | string | 关联报单号 | 
ActiveTime | ActiveTime | double | 激活时间 | 
TriggerTime | TriggerTime | double | 触发时间 | 
TimeSortNo | TimeSortNo | double | 按时间排队的序号 | 
TriggerStatus | TriggerStatus | string | 触发报单状态 | 0:未设置状态;1:活跃(运行)状态;2:已经触发;3:触发失败;4:撤单;
PosiDirection | PosiDirection | string | 持仓多空方向 | 0:多头;1:空头;2:净;
FrontNo | FrontNo | int | 前置编号 | 
ErrorNo | ErrorNo | int | 错误代码 | 
ErrorMsg | ErrorMsg | string | 错误信息 | 
InsertTime | InsertTime | double | 插入时间 | 
UpdateTime | UpdateTime | double | 更新时间 | 
BusinessNo | BusinessNo | double | 业务序列号 | 
CreateTime | CreateTime | string | 创建时间 | 


## **止盈止损报单录入请求**

+ **Request URL**

`POST /action/v1.0/SendCloseOrderInsert`


+ **Request Post Body**

```
{
	,"MemberID":"1983723"
	,"TradeUnitID":"1983723"
	,"AccountID":"1983723"
	,"LocalID":"1983723"
	,"ExchangeID":"CEX"
	,"InstrumentID":"BTCUSD"
	,"OrderPriceType":"1"
	,"Direction":"1"
	,"OffsetFlag":"1"
	,"Price":182582.18
	,"Volume":182582.18
	,"VolumeDisplay":182582.18
	,"VolumeMode":"1"
	,"Cost":182582.18
	,"OrderType":"1"
	,"GTDTime":1657543832
	,"MinVolume":182582.18
	,"BusinessType":"1"
	,"BusinessValue":""
	,"CloseOrderID":"1983723"
	,"IsCrossMargin":1
	,"Remark":"my_little_tips"
	,"OrderID":"12376422300333"
	,"CopyMemberID":"1983723"
	,"CopyOrderID":"1983723"
	,"Leverage":125
	,"CopyProfitRate":182582.18
	,"APPID":"ios"
	,"PositionID":"1983723"
	,"TriggerPrice":182582.18
	,"Reserve":182582.18
	,"CloseSLPrice":182582.18
	,"SLTriggerPrice":182582.18
	,"CloseSLTriggerPrice":182582.18
	,"CloseTPPrice":182582.18
	,"TPTriggerPrice":182582.18
	,"CloseTPTriggerPrice":182582.18
	,"CloseOrderPriceType":"1"
	,"ClosePrice":182582.18
	,"CloseTriggerPrice":182582.18
	,"CloseTriggerPriceType":"1"
}
```
+ **curl Example**

````
curl -d '{"MemberID":"1983723","TradeUnitID":"1983723","AccountID":"1983723","LocalID":"1983723","ExchangeID":"CEX","InstrumentID":"BTCUSD","OrderPriceType":"1","Direction":"1","OffsetFlag":"1","Price":"182582.18,"Volume":"182582.18,"VolumeDisplay":"182582.18,"VolumeMode":"1","Cost":"182582.18,"OrderType":"1","GTDTime":"1657543832,"MinVolume":"182582.18,"BusinessType":"1","BusinessValue":"","CloseOrderID":"1983723","IsCrossMargin":"1,"Remark":"my_little_tips","OrderID":"12376422300333","CopyMemberID":"1983723","CopyOrderID":"1983723","Leverage":"125,"CopyProfitRate":"182582.18,"APPID":"ios","PositionID":"1983723","TriggerPrice":"182582.18,"Reserve":"182582.18,"CloseSLPrice":"182582.18,"SLTriggerPrice":"182582.18,"CloseSLTriggerPrice":"182582.18,"CloseTPPrice":"182582.18,"TPTriggerPrice":"182582.18,"CloseTPTriggerPrice":"182582.18,"CloseOrderPriceType":"1","ClosePrice":"182582.18,"CloseTriggerPrice":"182582.18,"CloseTriggerPriceType":"1"}'  -X POST  http://127.0.0.1:10000/action/v1.0/SendCloseOrderInsert
````
+ **ws Example**

````
{"RequestNo":2,"SendCloseOrderInsert":{"MemberID":"1983723","TradeUnitID":"1983723","AccountID":"1983723","LocalID":"1983723","ExchangeID":"CEX","InstrumentID":"BTCUSD","OrderPriceType":"1","Direction":"1","OffsetFlag":"1","Price":"182582.18,"Volume":"182582.18,"VolumeDisplay":"182582.18,"VolumeMode":"1","Cost":"182582.18,"OrderType":"1","GTDTime":"1657543832,"MinVolume":"182582.18,"BusinessType":"1","BusinessValue":"","CloseOrderID":"1983723","IsCrossMargin":"1,"Remark":"my_little_tips","OrderID":"12376422300333","CopyMemberID":"1983723","CopyOrderID":"1983723","Leverage":"125,"CopyProfitRate":"182582.18,"APPID":"ios","PositionID":"1983723","TriggerPrice":"182582.18,"Reserve":"182582.18,"CloseSLPrice":"182582.18,"SLTriggerPrice":"182582.18,"CloseSLTriggerPrice":"182582.18,"CloseTPPrice":"182582.18,"TPTriggerPrice":"182582.18,"CloseTPTriggerPrice":"182582.18,"CloseOrderPriceType":"1","ClosePrice":"182582.18,"CloseTriggerPrice":"182582.18,"CloseTriggerPriceType":"1"}}
````
+ **Web rowser Example**

````
http://127.0.0.1:10000/action/v1.0/SendCloseOrderInsert?MemberID=1983723&TradeUnitID=1983723&AccountID=1983723&LocalID=1983723&ExchangeID=CEX&InstrumentID=BTCUSD&OrderPriceType=1&Direction=1&OffsetFlag=1&Price=182582.18&Volume=182582.18&VolumeDisplay=182582.18&VolumeMode=1&Cost=182582.18&OrderType=1&GTDTime=1657543832&MinVolume=182582.18&BusinessType=1&BusinessValue=&CloseOrderID=1983723&IsCrossMargin=1&Remark=my_little_tips&OrderID=12376422300333&CopyMemberID=1983723&CopyOrderID=1983723&Leverage=125&CopyProfitRate=182582.18&APPID=ios&PositionID=1983723&TriggerPrice=182582.18&Reserve=182582.18&CloseSLPrice=182582.18&SLTriggerPrice=182582.18&CloseSLTriggerPrice=182582.18&CloseTPPrice=182582.18&TPTriggerPrice=182582.18&CloseTPTriggerPrice=182582.18&CloseOrderPriceType=1&ClosePrice=182582.18&CloseTriggerPrice=182582.18&CloseTriggerPriceType=1
````
+ **Request Parameters**

parameter | shortName | required | type | description | remark
-------- | -------- | -------- | -------- | -------- | --------
MemberID | M | no | string | 成员代码 | 
TradeUnitID | T | no | string | 交易单元代码 | 
AccountID | A | no | string | 资金账号 | 
LocalID | L | no | string | 报单本地标识 | 
ExchangeID | E | yes | string | 交易所代码 | 
InstrumentID | I | yes | string | 标的代码 | 
OrderPriceType | PT | yes | string | 报单价格类型 | 0:限价(手动指定Price值);1:任意价(Price按最高最低限价取值);2:最优价/对手价(Price按对方1档价取值);3:五档价(Price按对方5档价取值);4:十档价(类同上);7:最优Marker价(Price按本方1档价或其+1Tick取值);8:Price按委托额Cost和订单簿情况确定值;9:保底点差价:成交价格Price不会劣于系统设置差价成交;
Direction | D | yes | string | 买卖方向 | 0:买;1:卖;
OffsetFlag | O | no | string | 开平标志 | 0:开仓;1:平仓;2:强平;3:平今;4:平昨;5:全平;6:指定订单平仓;7:指定成交平仓;8:最大平仓;
Price | P | no | double | 报单价格 | 
Volume | V | no | double | 数量 | 
VolumeDisplay | D | no | double | 显示数量 | 
VolumeMode | VM | no | string | 数量取值方式 | 0:百分比;1:绝对值;
Cost | C | no | double | 委托额 | 
OrderType | OT | no | string | 订单类型 | 0:普通订单;1:(FillAndKill/IOC)立即完成并且剩余撤销;2:(FillOrKill)立即完成全部或者最小订单要求否则全部撤销;3:需要进入订单簿被动成交,否则全部撤销(只做Maker/PostOnly);4:不需要撮合直接成交;
GTDTime | GT | no | double | GTD时间 | 
MinVolume | MV | no | double | 最小成交量 | 
BusinessType | BT | no | string | 业务类别 | 0:普通;1:申赎;2:行权;3:中立仓申报;4:递延交割申报;5:互换定单;6:质押;7:合并分拆;8:转股;9:回售回购;a:投票;b:融资融券;c:期权执行;d:传递被平仓订单BusinessValue;P:自定义11;Q:自定义10;R:自定义9;S:连续交易;T:自定义7;U:画线委托;V:限价市价;W:条件;X:止盈止损;Y:追踪出场;Z:指标类型;
BusinessValue | BV | no | string | 业务值 | 
CloseOrderID | CI | no | string | 平仓指定开仓的订单号 | 
IsCrossMargin | IsCrossMargin | no | int | 是否全仓 | 
Remark | R | no | string | 备注 | 
OrderID | OrderID | no | string | 报单系统唯一代码 | 
CopyMemberID | CopyMemberID | no | string | 带单员代码 | 
CopyOrderID | CopyOrderID | no | string | 带单员报单号 | 
Leverage | Leverage | no | double | 委托单杠杆倍数 | 
CopyProfitRate | CopyProfitRate | no | double | 带单分配比例 | 
APPID | APPID | no | string | 应用编号 | 
PositionID | B | no | string | 持仓代码 | 
TriggerPrice | TriggerPrice | no | double | 触发价 | 
Reserve | Reserve | no | double | 保留资金 | 
CloseSLPrice | CloseSLPrice | no | double | 平仓止损价 | 
SLTriggerPrice | SLTriggerPrice | no | double | 平仓止损触发价 | 
CloseSLTriggerPrice | CloseSLTriggerPrice | no | double | 平仓止损触发价 | 
CloseTPPrice | CloseTPPrice | no | double | 平仓止盈价 | 
TPTriggerPrice | TPTriggerPrice | no | double | 平仓止盈触发价 | 
CloseTPTriggerPrice | CloseTPTriggerPrice | no | double | 平仓止盈触发价 | 
CloseOrderPriceType | CloseOrderPriceType | no | string | 报单价格类型 | 0:限价(手动指定Price值);1:任意价(Price按最高最低限价取值);2:最优价/对手价(Price按对方1档价取值);3:五档价(Price按对方5档价取值);4:十档价(类同上);7:最优Marker价(Price按本方1档价或其+1Tick取值);8:Price按委托额Cost和订单簿情况确定值;9:保底点差价:成交价格Price不会劣于系统设置差价成交;
ClosePrice | ClosePrice | no | double | 平仓价 | 
CloseTriggerPrice | CloseTriggerPrice | no | double | 平仓触发价 | 
CloseTriggerPriceType | CloseTriggerPriceType | no | string | 触发价类型 | 0:最新价;1:标记价;2:标的指数价;

+ **Response**

```
{
	"action": "RecvCloseOrderInsert",
	"errorCode": 0,
	"errorMsg": "Success",
	"result": [			
				{"table":"CloseOrderInsert",
				 "data":{
					,MemberID:"1983723"
					,TradeUnitID:"1983723"
					,AccountID:"1983723"
					,LocalID:"1983723"
					,ExchangeID:"CEX"
					,InstrumentID:"BTCUSD"
					,OrderPriceType:"1"
					,Direction:"1"
					,OffsetFlag:"1"
					,Price:182582.18
					,Volume:182582.18
					,VolumeDisplay:182582.18
					,VolumeMode:"1"
					,Cost:182582.18
					,OrderType:"1"
					,GTDTime:1657543832
					,MinVolume:182582.18
					,BusinessType:"1"
					,BusinessValue:""
					,CloseOrderID:"1983723"
					,IsCrossMargin:1
					,Remark:"my_little_tips"
					,OrderID:"12376422300333"
					,CopyMemberID:"1983723"
					,CopyOrderID:"1983723"
					,Leverage:125
					,CopyProfitRate:182582.18
					,APPID:"ios"
					,PositionID:"1983723"
					,TriggerPrice:182582.18
					,Reserve:182582.18
					,CloseSLPrice:182582.18
					,SLTriggerPrice:182582.18
					,CloseSLTriggerPrice:182582.18
					,CloseTPPrice:182582.18
					,TPTriggerPrice:182582.18
					,CloseTPTriggerPrice:182582.18
					,CloseOrderPriceType:"1"
					,ClosePrice:182582.18
					,CloseTriggerPrice:182582.18
					,CloseTriggerPriceType:"1"
					}
				}
				{"table":"Order",
				 "data":{
					,MemberID:"1983723"
					,TradeUnitID:"1983723"
					,AccountID:"1983723"
					,LocalID:"1983723"
					,ExchangeID:"CEX"
					,InstrumentID:"BTCUSD"
					,OrderPriceType:"1"
					,Direction:"1"
					,OffsetFlag:"1"
					,Price:182582.18
					,Volume:182582.18
					,VolumeDisplay:182582.18
					,VolumeMode:"1"
					,Cost:182582.18
					,OrderType:"1"
					,GTDTime:1657543832
					,MinVolume:182582.18
					,BusinessType:"1"
					,BusinessValue:""
					,CloseOrderID:"1983723"
					,IsCrossMargin:1
					,Remark:"my_little_tips"
					,OrderID:"12376422300333"
					,CopyMemberID:"1983723"
					,CopyOrderID:"1983723"
					,Leverage:125
					,CopyProfitRate:182582.18
					,APPID:"ios"
					,PositionID:"1983723"
					,TriggerPrice:182582.18
					,Reserve:182582.18
					,OrderStatus:"1"
					,DeriveSource:"1"
					,DeriveDetail:""
					,VolumeTraded:182582.18
					,VolumeRemain:182582.18
					,VolumeCancled:182582.18
					,InsertTime:1657543832
					,UpdateTime:1657543832
					,Priority:0
					,TimeSortNo:1657543832
					,FrontNo:1
					,PriceCurrency:"BTC"
					,FeeCurrency:"BTC"
					,ClearCurrency:"BTC"
					,FrozenMoney:182582.18
					,FrozenFee:182582.18
					,FrozenMargin:182582.18
					,Fee:0.22364
					,CloseProfit:10
					,Turnover:182582.18
					,RelatedOrderID:"1983723"
					,BusinessResult:""
					,BusinessNo:11176422300333
					,Tradable:0
					,SettlementGroup:"999"
					,PosiDirection:"1"
					,TradePrice:182582.18
					,OpenPrice:182582.18
					,TriggerOrderID:"1983723"
					,SLTriggerPrice:182582.18
					,TPTriggerPrice:182582.18
					,CopyProfit:182582.18
					,Position:182582.18
					,UserID:"1983723"
					,LastPriceByInsert:182582.18
					,BidPrice1ByInsert:182582.18
					,AskPrice1ByInsert:182582.18
					,Available:182582.18
					,CreateTime:""
					}
				}
				{"table":"TriggerOrder",
				 "data":{
					,MemberID:"1983723"
					,TradeUnitID:"1983723"
					,AccountID:"1983723"
					,LocalID:"1983723"
					,ExchangeID:"CEX"
					,InstrumentID:"BTCUSD"
					,OrderPriceType:"1"
					,Direction:"1"
					,OffsetFlag:"1"
					,Price:182582.18
					,Volume:182582.18
					,VolumeDisplay:182582.18
					,VolumeMode:"1"
					,Cost:182582.18
					,OrderType:"1"
					,GTDTime:1657543832
					,MinVolume:182582.18
					,BusinessType:"1"
					,BusinessValue:""
					,CloseOrderID:"1983723"
					,IsCrossMargin:1
					,Remark:"my_little_tips"
					,OrderID:"12376422300333"
					,CopyMemberID:"1983723"
					,CopyOrderID:"1983723"
					,Leverage:125
					,CopyProfitRate:182582.18
					,APPID:"ios"
					,PositionID:"1983723"
					,TriggerPrice:182582.18
					,Reserve:182582.18
					,SLPrice:182582.18
					,SLTriggerPrice:182582.18
					,TPPrice:182582.18
					,TPTriggerPrice:182582.18
					,RiskBefore:0
					,TriggerOrderType:"1"
					,TriggerDetail:""
					,TriggerPriceType:"1"
					,TriggerValue:""
					,CloseSLPrice:182582.18
					,CloseSLTriggerPrice:182582.18
					,CloseTPPrice:182582.18
					,CloseTPTriggerPrice:182582.18
					,CloseOrderPriceType:"1"
					,ClosePrice:182582.18
					,CloseTriggerPrice:182582.18
					,RelatedOrderID:"1983723"
					,ActiveTime:1657543832
					,TriggerTime:1657543832
					,TimeSortNo:1657543832
					,TriggerStatus:"1"
					,PosiDirection:"1"
					,FrontNo:1
					,ErrorNo:1033201232
					,ErrorMsg:""
					,InsertTime:1657543832
					,UpdateTime:1657543832
					,BusinessNo:11176422300333
					,CreateTime:""
					}
				}
	]
}
```

+ **Response Parameters**

CloseOrderInsert:

parameter | shortName | type | description | remark
-------- | -------- | -------- | -------- | -------- 					
MemberID | M | string | 成员代码 | 
TradeUnitID | T | string | 交易单元代码 | 
AccountID | A | string | 资金账号 | 
LocalID | L | string | 报单本地标识 | 
ExchangeID | E | string | 交易所代码 | 
InstrumentID | I | string | 标的代码 | 
OrderPriceType | PT | string | 报单价格类型 | 0:限价(手动指定Price值);1:任意价(Price按最高最低限价取值);2:最优价/对手价(Price按对方1档价取值);3:五档价(Price按对方5档价取值);4:十档价(类同上);7:最优Marker价(Price按本方1档价或其+1Tick取值);8:Price按委托额Cost和订单簿情况确定值;9:保底点差价:成交价格Price不会劣于系统设置差价成交;
Direction | D | string | 买卖方向 | 0:买;1:卖;
OffsetFlag | O | string | 开平标志 | 0:开仓;1:平仓;2:强平;3:平今;4:平昨;5:全平;6:指定订单平仓;7:指定成交平仓;8:最大平仓;
Price | P | double | 报单价格 | 
Volume | V | double | 数量 | 
VolumeDisplay | D | double | 显示数量 | 
VolumeMode | VM | string | 数量取值方式 | 0:百分比;1:绝对值;
Cost | C | double | 委托额 | 
OrderType | OT | string | 订单类型 | 0:普通订单;1:(FillAndKill/IOC)立即完成并且剩余撤销;2:(FillOrKill)立即完成全部或者最小订单要求否则全部撤销;3:需要进入订单簿被动成交,否则全部撤销(只做Maker/PostOnly);4:不需要撮合直接成交;
GTDTime | GT | double | GTD时间 | 
MinVolume | MV | double | 最小成交量 | 
BusinessType | BT | string | 业务类别 | 0:普通;1:申赎;2:行权;3:中立仓申报;4:递延交割申报;5:互换定单;6:质押;7:合并分拆;8:转股;9:回售回购;a:投票;b:融资融券;c:期权执行;d:传递被平仓订单BusinessValue;P:自定义11;Q:自定义10;R:自定义9;S:连续交易;T:自定义7;U:画线委托;V:限价市价;W:条件;X:止盈止损;Y:追踪出场;Z:指标类型;
BusinessValue | BV | string | 业务值 | 
CloseOrderID | CI | string | 平仓指定开仓的订单号 | 
IsCrossMargin | IsCrossMargin | int | 是否全仓 | 
Remark | R | string | 备注 | 
OrderID | OrderID | string | 报单系统唯一代码 | 
CopyMemberID | CopyMemberID | string | 带单员代码 | 
CopyOrderID | CopyOrderID | string | 带单员报单号 | 
Leverage | Leverage | double | 委托单杠杆倍数 | 
CopyProfitRate | CopyProfitRate | double | 带单分配比例 | 
APPID | APPID | string | 应用编号 | 
PositionID | B | string | 持仓代码 | 
TriggerPrice | TriggerPrice | double | 触发价 | 
Reserve | Reserve | double | 保留资金 | 
CloseSLPrice | CloseSLPrice | double | 平仓止损价 | 
SLTriggerPrice | SLTriggerPrice | double | 平仓止损触发价 | 
CloseSLTriggerPrice | CloseSLTriggerPrice | double | 平仓止损触发价 | 
CloseTPPrice | CloseTPPrice | double | 平仓止盈价 | 
TPTriggerPrice | TPTriggerPrice | double | 平仓止盈触发价 | 
CloseTPTriggerPrice | CloseTPTriggerPrice | double | 平仓止盈触发价 | 
CloseOrderPriceType | CloseOrderPriceType | string | 报单价格类型 | 0:限价(手动指定Price值);1:任意价(Price按最高最低限价取值);2:最优价/对手价(Price按对方1档价取值);3:五档价(Price按对方5档价取值);4:十档价(类同上);7:最优Marker价(Price按本方1档价或其+1Tick取值);8:Price按委托额Cost和订单簿情况确定值;9:保底点差价:成交价格Price不会劣于系统设置差价成交;
ClosePrice | ClosePrice | double | 平仓价 | 
CloseTriggerPrice | CloseTriggerPrice | double | 平仓触发价 | 
CloseTriggerPriceType | CloseTriggerPriceType | string | 触发价类型 | 0:最新价;1:标记价;2:标的指数价;

Order:

parameter | shortName | type | description | remark
-------- | -------- | -------- | -------- | -------- 					
MemberID | M | string | 成员代码 | 
TradeUnitID | T | string | 交易单元代码 | 
AccountID | A | string | 资金账号 | 
LocalID | L | string | 报单本地标识 | 
ExchangeID | E | string | 交易所代码 | 
InstrumentID | I | string | 标的代码 | 
OrderPriceType | PT | string | 报单价格类型 | 0:限价(手动指定Price值);1:任意价(Price按最高最低限价取值);2:最优价/对手价(Price按对方1档价取值);3:五档价(Price按对方5档价取值);4:十档价(类同上);7:最优Marker价(Price按本方1档价或其+1Tick取值);8:Price按委托额Cost和订单簿情况确定值;9:保底点差价:成交价格Price不会劣于系统设置差价成交;
Direction | D | string | 买卖方向 | 0:买;1:卖;
OffsetFlag | O | string | 开平标志 | 0:开仓;1:平仓;2:强平;3:平今;4:平昨;5:全平;6:指定订单平仓;7:指定成交平仓;8:最大平仓;
Price | P | double | 报单价格 | 
Volume | V | double | 数量 | 
VolumeDisplay | D | double | 显示数量 | 
VolumeMode | VM | string | 数量取值方式 | 0:百分比;1:绝对值;
Cost | C | double | 委托额 | 
OrderType | OT | string | 订单类型 | 0:普通订单;1:(FillAndKill/IOC)立即完成并且剩余撤销;2:(FillOrKill)立即完成全部或者最小订单要求否则全部撤销;3:需要进入订单簿被动成交,否则全部撤销(只做Maker/PostOnly);4:不需要撮合直接成交;
GTDTime | GT | double | GTD时间 | 
MinVolume | MV | double | 最小成交量 | 
BusinessType | BT | string | 业务类别 | 0:普通;1:申赎;2:行权;3:中立仓申报;4:递延交割申报;5:互换定单;6:质押;7:合并分拆;8:转股;9:回售回购;a:投票;b:融资融券;c:期权执行;d:传递被平仓订单BusinessValue;P:自定义11;Q:自定义10;R:自定义9;S:连续交易;T:自定义7;U:画线委托;V:限价市价;W:条件;X:止盈止损;Y:追踪出场;Z:指标类型;
BusinessValue | BV | string | 业务值 | 
CloseOrderID | CI | string | 平仓指定开仓的订单号 | 
IsCrossMargin | IsCrossMargin | int | 是否全仓 | 
Remark | R | string | 备注 | 
OrderID | OrderID | string | 报单系统唯一代码 | 
CopyMemberID | CopyMemberID | string | 带单员代码 | 
CopyOrderID | CopyOrderID | string | 带单员报单号 | 
Leverage | Leverage | double | 委托单杠杆倍数 | 
CopyProfitRate | CopyProfitRate | double | 带单分配比例 | 
APPID | APPID | string | 应用编号 | 
PositionID | B | string | 持仓代码 | 
TriggerPrice | TriggerPrice | double | 触发价 | 
Reserve | Reserve | double | 保留资金 | 
OrderStatus | OrderStatus | string | 报单状态 | 0:未设置状态;1:全部成交;2:部分成交未撤单;3:部分成交已撤单;4:未成交未撤单;6:无成交已撤单;
DeriveSource | DeriveSource | string | 衍生来源 | 0:正常;1:报价衍生;2:期权执行;3:组合衍生;4:场外成交强平衍生;5:场外成交大宗交易衍生;6:场外成交期转现衍生;7:场外成交只刷单衍生;8:触发订单衍生;9:止损触发订单衍生;a:止盈触发订单衍生;b:资金操作衍生;c:仓位合并衍生;d:CFD订单衍生;e:场外成交ADL衍生;f:回购手续费不能是币;
DeriveDetail | DeriveDetail | string | 衍生明细 | 
VolumeTraded | VolumeTraded | double | 成交数量 | 
VolumeRemain | VolumeRemain | double | 剩余数量 | 
VolumeCancled | VolumeCancled | double | 已经撤单数量 | 
InsertTime | InsertTime | double | 插入时间 | 
UpdateTime | UpdateTime | double | 更新时间 | 
Priority |  | int | 优先权 | 
TimeSortNo |  | double | 按时间排队的序号 | 
FrontNo | FrontNo | int | 前置编号 | 
PriceCurrency | PriceCurrency | string | 计价币种 | 
FeeCurrency | FeeCurrency | string | 手续费币种 | 
ClearCurrency | ClearCurrency | string | 清算币种 | 
FrozenMoney | FrozenMoney | double | 冻结资金 | 
FrozenFee | FrozenFee | double | 冻结手续费 | 
FrozenMargin | FrozenMargin | double | 冻结保证金 | 
Fee | Fee | double | 手续费 | 
CloseProfit | CloseProfit | double | 平仓盈亏 | 
Turnover | Turnover | double | 成交金额 | 
RelatedOrderID | RelatedOrderID | string | 关联报单号 | 
BusinessResult | BusinessResult | string | 业务执行结果 | 
BusinessNo | BusinessNo | double | 业务序列号 | 
Tradable |  | int | 是否可以参加交易 | 
SettlementGroup |  | string | 结算组编号 | 
PosiDirection | PosiDirection | string | 持仓多空方向 | 0:多头;1:空头;2:净;
TradePrice | TradePrice | double | 成交均价 | 
OpenPrice | OpenPrice | double | 平仓成交时的开仓均价 | 
TriggerOrderID | TriggerOrderID | string | 触发报单号 | 
SLTriggerPrice | SLTriggerPrice | double | 开仓报单成交之后的止损触发价 | 
TPTriggerPrice | TPTriggerPrice | double | 开仓报单成交之后的止盈触发价 | 
CopyProfit | CopyProfit | double | 带单盈利分配 | 
Position | Position | double | 成交时的持仓量 | 
UserID | U | string | 交易用户代码 | 
LastPriceByInsert |  | double | 插入时的最新价 | 
BidPrice1ByInsert |  | double | 插入时的买一价 | 
AskPrice1ByInsert |  | double | 插入时的卖一价 | 
Available | Available | double | 可用资金 | 
CreateTime |  | string | 创建时间 | 

TriggerOrder:

parameter | shortName | type | description | remark
-------- | -------- | -------- | -------- | -------- 					
MemberID | M | string | 成员代码 | 
TradeUnitID | T | string | 交易单元代码 | 
AccountID | A | string | 资金账号 | 
LocalID | L | string | 报单本地标识 | 
ExchangeID | E | string | 交易所代码 | 
InstrumentID | I | string | 标的代码 | 
OrderPriceType | PT | string | 报单价格类型 | 0:限价(手动指定Price值);1:任意价(Price按最高最低限价取值);2:最优价/对手价(Price按对方1档价取值);3:五档价(Price按对方5档价取值);4:十档价(类同上);7:最优Marker价(Price按本方1档价或其+1Tick取值);8:Price按委托额Cost和订单簿情况确定值;9:保底点差价:成交价格Price不会劣于系统设置差价成交;
Direction | D | string | 买卖方向 | 0:买;1:卖;
OffsetFlag | O | string | 开平标志 | 0:开仓;1:平仓;2:强平;3:平今;4:平昨;5:全平;6:指定订单平仓;7:指定成交平仓;8:最大平仓;
Price | P | double | 报单价格 | 
Volume | V | double | 数量 | 
VolumeDisplay | D | double | 显示数量 | 
VolumeMode | VM | string | 数量取值方式 | 0:百分比;1:绝对值;
Cost | C | double | 委托额 | 
OrderType | OT | string | 订单类型 | 0:普通订单;1:(FillAndKill/IOC)立即完成并且剩余撤销;2:(FillOrKill)立即完成全部或者最小订单要求否则全部撤销;3:需要进入订单簿被动成交,否则全部撤销(只做Maker/PostOnly);4:不需要撮合直接成交;
GTDTime | GT | double | GTD时间 | 
MinVolume | MV | double | 最小成交量 | 
BusinessType | BT | string | 业务类别 | 0:普通;1:申赎;2:行权;3:中立仓申报;4:递延交割申报;5:互换定单;6:质押;7:合并分拆;8:转股;9:回售回购;a:投票;b:融资融券;c:期权执行;d:传递被平仓订单BusinessValue;P:自定义11;Q:自定义10;R:自定义9;S:连续交易;T:自定义7;U:画线委托;V:限价市价;W:条件;X:止盈止损;Y:追踪出场;Z:指标类型;
BusinessValue | BV | string | 业务值 | 
CloseOrderID | CI | string | 平仓指定开仓的订单号 | 
IsCrossMargin | IsCrossMargin | int | 是否全仓 | 
Remark | R | string | 备注 | 
OrderID | OrderID | string | 报单系统唯一代码 | 
CopyMemberID | CopyMemberID | string | 带单员代码 | 
CopyOrderID | CopyOrderID | string | 带单员报单号 | 
Leverage | Leverage | double | 委托单杠杆倍数 | 
CopyProfitRate | CopyProfitRate | double | 带单分配比例 | 
APPID | APPID | string | 应用编号 | 
PositionID | B | string | 持仓代码 | 
TriggerPrice | TriggerPrice | double | 触发价 | 
Reserve | Reserve | double | 保留资金 | 
SLPrice | SLPrice | double | 止损价 | 
SLTriggerPrice | SLTriggerPrice | double | 止损触发价 | 
TPPrice | TPPrice | double | 止盈价 | 
TPTriggerPrice | TPTriggerPrice | double | 止盈触发价 | 
RiskBefore | RiskBefore | int | 是否提前检查风控 | 
TriggerOrderType | TriggerOrderType | string | 触发的订单类型 | 0:未定义;1:持仓止盈止损;2:订单止盈止损;3:条件单;4:追踪(跟踪)订单;5:其他算法交易;6:指标类型;7:指标类型止盈止损;8:跟单类型;9:划线下单类型;a:其他类型1;b:其他类型2;c:其他类型3;d:其他类型4;e:其他类型5;
TriggerDetail | TriggerDetail | string | 触发类型明细 | 
TriggerPriceType | TriggerPriceType | string | 触发价类型 | 0:最新价;1:标记价;2:标的指数价;
TriggerValue | TriggerValue | string | 触发单具体设置信息 | 
CloseSLPrice | CloseSLPrice | double | 平仓止损价 | 
CloseSLTriggerPrice | CloseSLTriggerPrice | double | 平仓止损触发价 | 
CloseTPPrice | CloseTPPrice | double | 平仓止盈价 | 
CloseTPTriggerPrice | CloseTPTriggerPrice | double | 平仓止盈触发价 | 
CloseOrderPriceType | CloseOrderPriceType | string | 报单价格类型 | 0:限价(手动指定Price值);1:任意价(Price按最高最低限价取值);2:最优价/对手价(Price按对方1档价取值);3:五档价(Price按对方5档价取值);4:十档价(类同上);7:最优Marker价(Price按本方1档价或其+1Tick取值);8:Price按委托额Cost和订单簿情况确定值;9:保底点差价:成交价格Price不会劣于系统设置差价成交;
ClosePrice | ClosePrice | double | 平仓价 | 
CloseTriggerPrice | CloseTriggerPrice | double | 平仓触发价 | 
RelatedOrderID | RelatedOrderID | string | 关联报单号 | 
ActiveTime | ActiveTime | double | 激活时间 | 
TriggerTime | TriggerTime | double | 触发时间 | 
TimeSortNo | TimeSortNo | double | 按时间排队的序号 | 
TriggerStatus | TriggerStatus | string | 触发报单状态 | 0:未设置状态;1:活跃(运行)状态;2:已经触发;3:触发失败;4:撤单;
PosiDirection | PosiDirection | string | 持仓多空方向 | 0:多头;1:空头;2:净;
FrontNo | FrontNo | int | 前置编号 | 
ErrorNo | ErrorNo | int | 错误代码 | 
ErrorMsg | ErrorMsg | string | 错误信息 | 
InsertTime | InsertTime | double | 插入时间 | 
UpdateTime | UpdateTime | double | 更新时间 | 
BusinessNo | BusinessNo | double | 业务序列号 | 
CreateTime | CreateTime | string | 创建时间 | 


## **止盈止损报单操作请求**

+ **Request URL**

`POST /action/v1.0/SendCloseOrderAction`


+ **Request Post Body**

```
{
	,"OrderID":"12376422300333"
	,"OrderLocalID":"1983723"
	,"ActionFlag":"1"
	,"LocalID":"1983723"
	,"MemberID":"1983723"
	,"AccountID":"1983723"
	,"Price":182582.18
	,"Volume":182582.18
	,"Direction":"1"
	,"VolumeDisplay":182582.18
	,"ProductGroup":"Swap"
	,"ExchangeID":"CEX"
	,"InstrumentID":"BTCUSD"
	,"Cost":182582.18
	,"Remark":"my_little_tips"
	,"SLPrice":182582.18
	,"SLTriggerPrice":182582.18
	,"TPPrice":182582.18
	,"TPTriggerPrice":182582.18
	,"TriggerPrice":182582.18
	,"TriggerOrderType":""
	,"TriggerValue":""
	,"TriggerDetail":""
	,"TriggerPriceType":"1"
	,"ErrorNo":1033201232
	,"ErrorMsg":""
}
```
+ **curl Example**

````
curl -d '{"OrderID":"12376422300333","OrderLocalID":"1983723","ActionFlag":"1","LocalID":"1983723","MemberID":"1983723","AccountID":"1983723","Price":"182582.18,"Volume":"182582.18,"Direction":"1","VolumeDisplay":"182582.18,"ProductGroup":"Swap","ExchangeID":"CEX","InstrumentID":"BTCUSD","Cost":"182582.18,"Remark":"my_little_tips","SLPrice":"182582.18,"SLTriggerPrice":"182582.18,"TPPrice":"182582.18,"TPTriggerPrice":"182582.18,"TriggerPrice":"182582.18,"TriggerOrderType":"","TriggerValue":"","TriggerDetail":"","TriggerPriceType":"1","ErrorNo":"1033201232,"ErrorMsg":""}'  -X POST  http://127.0.0.1:10000/action/v1.0/SendCloseOrderAction
````
+ **ws Example**

````
{"RequestNo":2,"SendCloseOrderAction":{"OrderID":"12376422300333","OrderLocalID":"1983723","ActionFlag":"1","LocalID":"1983723","MemberID":"1983723","AccountID":"1983723","Price":"182582.18,"Volume":"182582.18,"Direction":"1","VolumeDisplay":"182582.18,"ProductGroup":"Swap","ExchangeID":"CEX","InstrumentID":"BTCUSD","Cost":"182582.18,"Remark":"my_little_tips","SLPrice":"182582.18,"SLTriggerPrice":"182582.18,"TPPrice":"182582.18,"TPTriggerPrice":"182582.18,"TriggerPrice":"182582.18,"TriggerOrderType":"","TriggerValue":"","TriggerDetail":"","TriggerPriceType":"1","ErrorNo":"1033201232,"ErrorMsg":""}}
````
+ **Web rowser Example**

````
http://127.0.0.1:10000/action/v1.0/SendCloseOrderAction?OrderID=12376422300333&OrderLocalID=1983723&ActionFlag=1&LocalID=1983723&MemberID=1983723&AccountID=1983723&Price=182582.18&Volume=182582.18&Direction=1&VolumeDisplay=182582.18&ProductGroup=Swap&ExchangeID=CEX&InstrumentID=BTCUSD&Cost=182582.18&Remark=my_little_tips&SLPrice=182582.18&SLTriggerPrice=182582.18&TPPrice=182582.18&TPTriggerPrice=182582.18&TriggerPrice=182582.18&TriggerOrderType=&TriggerValue=&TriggerDetail=&TriggerPriceType=1&ErrorNo=1033201232&ErrorMsg=
````
+ **Request Parameters**

parameter | shortName | required | type | description | remark
-------- | -------- | -------- | -------- | -------- | --------
OrderID | OrderID | no | string | 报单系统唯一代码 | 
OrderLocalID | OrderLocalID | no | string | 报单本地标识 | 
ActionFlag | ActionFlag | yes | string | 报单操作标志 | 1:删除;2:挂起;3:激活;4:修改;5:触发;6:增减;
LocalID | LocalID | no | string | 操作本地标识 | 
MemberID | MemberID | no | string | 成员代码 | 
AccountID | AccountID | no | string | 资金账号 | 
Price | Price | no | double | 价格 | 
Volume | Volume | no | double | 数量变化 | 
Direction | Direction | no | string | 买卖方向 | 0:买;1:卖;
VolumeDisplay | D | no | double | 显示数量 | 
ProductGroup | ProductGroup | no | string | 产品组 | 
ExchangeID | ExchangeID | no | string | 交易所代码 | 
InstrumentID | InstrumentID | no | string | 标的代码 | 
Cost | C | no | double | 委托额 | 
Remark | Remark | no | string | 备注 | 
SLPrice | SLPrice | no | double | 止损价 | 
SLTriggerPrice | SLTriggerPrice | no | double | 止损触发价 | 
TPPrice | TPPrice | no | double | 止盈价 | 
TPTriggerPrice | TPTriggerPrice | no | double | 止盈触发价 | 
TriggerPrice | TriggerPrice | no | double | 触发价 | 
TriggerOrderType | TriggerOrderType | no | string | 触发的订单类型 | 
TriggerValue | TriggerValue | no | string | 触发单具体设置信息 | 
TriggerDetail | TriggerDetail | no | string | 触发类型明细 | 
TriggerPriceType | TriggerPriceType | no | string | 触发价类型 | 0:最新价;1:标记价;2:标的指数价;
ErrorNo | ErrorNo | no | int | 错误代码 | 
ErrorMsg | ErrorMsg | no | string | 错误信息 | 

+ **Response**

```
{
	"action": "RecvCloseOrderAction",
	"errorCode": 0,
	"errorMsg": "Success",
	"result": [			
				{"table":"CloseOrderAction",
				 "data":{
					,OrderID:"12376422300333"
					,OrderLocalID:"1983723"
					,ActionFlag:"1"
					,LocalID:"1983723"
					,MemberID:"1983723"
					,AccountID:"1983723"
					,Price:182582.18
					,Volume:182582.18
					,Direction:"1"
					,VolumeDisplay:182582.18
					,ProductGroup:"Swap"
					,ExchangeID:"CEX"
					,InstrumentID:"BTCUSD"
					,Cost:182582.18
					,Remark:"my_little_tips"
					,SLPrice:182582.18
					,SLTriggerPrice:182582.18
					,TPPrice:182582.18
					,TPTriggerPrice:182582.18
					,TriggerPrice:182582.18
					,TriggerOrderType:""
					,TriggerValue:""
					,TriggerDetail:""
					,TriggerPriceType:"1"
					,ErrorNo:1033201232
					,ErrorMsg:""
					}
				}
				{"table":"TriggerOrder",
				 "data":{
					,MemberID:"1983723"
					,TradeUnitID:"1983723"
					,AccountID:"1983723"
					,LocalID:"1983723"
					,ExchangeID:"CEX"
					,InstrumentID:"BTCUSD"
					,OrderPriceType:"1"
					,Direction:"1"
					,OffsetFlag:"1"
					,Price:182582.18
					,Volume:182582.18
					,VolumeDisplay:182582.18
					,VolumeMode:"1"
					,Cost:182582.18
					,OrderType:"1"
					,GTDTime:1657543832
					,MinVolume:182582.18
					,BusinessType:"1"
					,BusinessValue:""
					,CloseOrderID:"1983723"
					,IsCrossMargin:1
					,Remark:"my_little_tips"
					,OrderID:"12376422300333"
					,CopyMemberID:"1983723"
					,CopyOrderID:"1983723"
					,Leverage:125
					,CopyProfitRate:182582.18
					,APPID:"ios"
					,PositionID:"1983723"
					,TriggerPrice:182582.18
					,Reserve:182582.18
					,SLPrice:182582.18
					,SLTriggerPrice:182582.18
					,TPPrice:182582.18
					,TPTriggerPrice:182582.18
					,RiskBefore:0
					,TriggerOrderType:"1"
					,TriggerDetail:""
					,TriggerPriceType:"1"
					,TriggerValue:""
					,CloseSLPrice:182582.18
					,CloseSLTriggerPrice:182582.18
					,CloseTPPrice:182582.18
					,CloseTPTriggerPrice:182582.18
					,CloseOrderPriceType:"1"
					,ClosePrice:182582.18
					,CloseTriggerPrice:182582.18
					,RelatedOrderID:"1983723"
					,ActiveTime:1657543832
					,TriggerTime:1657543832
					,TimeSortNo:1657543832
					,TriggerStatus:"1"
					,PosiDirection:"1"
					,FrontNo:1
					,ErrorNo:1033201232
					,ErrorMsg:""
					,InsertTime:1657543832
					,UpdateTime:1657543832
					,BusinessNo:11176422300333
					,CreateTime:""
					}
				}
	]
}
```

+ **Response Parameters**

CloseOrderAction:

parameter | shortName | type | description | remark
-------- | -------- | -------- | -------- | -------- 					
OrderID | OrderID | string | 报单系统唯一代码 | 
OrderLocalID | OrderLocalID | string | 报单本地标识 | 
ActionFlag | ActionFlag | string | 报单操作标志 | 1:删除;2:挂起;3:激活;4:修改;5:触发;6:增减;
LocalID | LocalID | string | 操作本地标识 | 
MemberID | MemberID | string | 成员代码 | 
AccountID | AccountID | string | 资金账号 | 
Price | Price | double | 价格 | 
Volume | Volume | double | 数量变化 | 
Direction | Direction | string | 买卖方向 | 0:买;1:卖;
VolumeDisplay | D | double | 显示数量 | 
ProductGroup | ProductGroup | string | 产品组 | 
ExchangeID | ExchangeID | string | 交易所代码 | 
InstrumentID | InstrumentID | string | 标的代码 | 
Cost | C | double | 委托额 | 
Remark | Remark | string | 备注 | 
SLPrice | SLPrice | double | 止损价 | 
SLTriggerPrice | SLTriggerPrice | double | 止损触发价 | 
TPPrice | TPPrice | double | 止盈价 | 
TPTriggerPrice | TPTriggerPrice | double | 止盈触发价 | 
TriggerPrice | TriggerPrice | double | 触发价 | 
TriggerOrderType | TriggerOrderType | string | 触发的订单类型 | 
TriggerValue | TriggerValue | string | 触发单具体设置信息 | 
TriggerDetail | TriggerDetail | string | 触发类型明细 | 
TriggerPriceType | TriggerPriceType | string | 触发价类型 | 0:最新价;1:标记价;2:标的指数价;
ErrorNo | ErrorNo | int | 错误代码 | 
ErrorMsg | ErrorMsg | string | 错误信息 | 

TriggerOrder:

parameter | shortName | type | description | remark
-------- | -------- | -------- | -------- | -------- 					
MemberID | M | string | 成员代码 | 
TradeUnitID | T | string | 交易单元代码 | 
AccountID | A | string | 资金账号 | 
LocalID | L | string | 报单本地标识 | 
ExchangeID | E | string | 交易所代码 | 
InstrumentID | I | string | 标的代码 | 
OrderPriceType | PT | string | 报单价格类型 | 0:限价(手动指定Price值);1:任意价(Price按最高最低限价取值);2:最优价/对手价(Price按对方1档价取值);3:五档价(Price按对方5档价取值);4:十档价(类同上);7:最优Marker价(Price按本方1档价或其+1Tick取值);8:Price按委托额Cost和订单簿情况确定值;9:保底点差价:成交价格Price不会劣于系统设置差价成交;
Direction | D | string | 买卖方向 | 0:买;1:卖;
OffsetFlag | O | string | 开平标志 | 0:开仓;1:平仓;2:强平;3:平今;4:平昨;5:全平;6:指定订单平仓;7:指定成交平仓;8:最大平仓;
Price | P | double | 报单价格 | 
Volume | V | double | 数量 | 
VolumeDisplay | D | double | 显示数量 | 
VolumeMode | VM | string | 数量取值方式 | 0:百分比;1:绝对值;
Cost | C | double | 委托额 | 
OrderType | OT | string | 订单类型 | 0:普通订单;1:(FillAndKill/IOC)立即完成并且剩余撤销;2:(FillOrKill)立即完成全部或者最小订单要求否则全部撤销;3:需要进入订单簿被动成交,否则全部撤销(只做Maker/PostOnly);4:不需要撮合直接成交;
GTDTime | GT | double | GTD时间 | 
MinVolume | MV | double | 最小成交量 | 
BusinessType | BT | string | 业务类别 | 0:普通;1:申赎;2:行权;3:中立仓申报;4:递延交割申报;5:互换定单;6:质押;7:合并分拆;8:转股;9:回售回购;a:投票;b:融资融券;c:期权执行;d:传递被平仓订单BusinessValue;P:自定义11;Q:自定义10;R:自定义9;S:连续交易;T:自定义7;U:画线委托;V:限价市价;W:条件;X:止盈止损;Y:追踪出场;Z:指标类型;
BusinessValue | BV | string | 业务值 | 
CloseOrderID | CI | string | 平仓指定开仓的订单号 | 
IsCrossMargin | IsCrossMargin | int | 是否全仓 | 
Remark | R | string | 备注 | 
OrderID | OrderID | string | 报单系统唯一代码 | 
CopyMemberID | CopyMemberID | string | 带单员代码 | 
CopyOrderID | CopyOrderID | string | 带单员报单号 | 
Leverage | Leverage | double | 委托单杠杆倍数 | 
CopyProfitRate | CopyProfitRate | double | 带单分配比例 | 
APPID | APPID | string | 应用编号 | 
PositionID | B | string | 持仓代码 | 
TriggerPrice | TriggerPrice | double | 触发价 | 
Reserve | Reserve | double | 保留资金 | 
SLPrice | SLPrice | double | 止损价 | 
SLTriggerPrice | SLTriggerPrice | double | 止损触发价 | 
TPPrice | TPPrice | double | 止盈价 | 
TPTriggerPrice | TPTriggerPrice | double | 止盈触发价 | 
RiskBefore | RiskBefore | int | 是否提前检查风控 | 
TriggerOrderType | TriggerOrderType | string | 触发的订单类型 | 0:未定义;1:持仓止盈止损;2:订单止盈止损;3:条件单;4:追踪(跟踪)订单;5:其他算法交易;6:指标类型;7:指标类型止盈止损;8:跟单类型;9:划线下单类型;a:其他类型1;b:其他类型2;c:其他类型3;d:其他类型4;e:其他类型5;
TriggerDetail | TriggerDetail | string | 触发类型明细 | 
TriggerPriceType | TriggerPriceType | string | 触发价类型 | 0:最新价;1:标记价;2:标的指数价;
TriggerValue | TriggerValue | string | 触发单具体设置信息 | 
CloseSLPrice | CloseSLPrice | double | 平仓止损价 | 
CloseSLTriggerPrice | CloseSLTriggerPrice | double | 平仓止损触发价 | 
CloseTPPrice | CloseTPPrice | double | 平仓止盈价 | 
CloseTPTriggerPrice | CloseTPTriggerPrice | double | 平仓止盈触发价 | 
CloseOrderPriceType | CloseOrderPriceType | string | 报单价格类型 | 0:限价(手动指定Price值);1:任意价(Price按最高最低限价取值);2:最优价/对手价(Price按对方1档价取值);3:五档价(Price按对方5档价取值);4:十档价(类同上);7:最优Marker价(Price按本方1档价或其+1Tick取值);8:Price按委托额Cost和订单簿情况确定值;9:保底点差价:成交价格Price不会劣于系统设置差价成交;
ClosePrice | ClosePrice | double | 平仓价 | 
CloseTriggerPrice | CloseTriggerPrice | double | 平仓触发价 | 
RelatedOrderID | RelatedOrderID | string | 关联报单号 | 
ActiveTime | ActiveTime | double | 激活时间 | 
TriggerTime | TriggerTime | double | 触发时间 | 
TimeSortNo | TimeSortNo | double | 按时间排队的序号 | 
TriggerStatus | TriggerStatus | string | 触发报单状态 | 0:未设置状态;1:活跃(运行)状态;2:已经触发;3:触发失败;4:撤单;
PosiDirection | PosiDirection | string | 持仓多空方向 | 0:多头;1:空头;2:净;
FrontNo | FrontNo | int | 前置编号 | 
ErrorNo | ErrorNo | int | 错误代码 | 
ErrorMsg | ErrorMsg | string | 错误信息 | 
InsertTime | InsertTime | double | 插入时间 | 
UpdateTime | UpdateTime | double | 更新时间 | 
BusinessNo | BusinessNo | double | 业务序列号 | 
CreateTime | CreateTime | string | 创建时间 | 


## **报价录入请求**

+ **Request URL**

`POST /action/v1.0/SendQuoteInsert`


+ **Request Post Body**

```
{
	,"ExchangeID":"CEX"
	,"InstrumentID":"BTCUSD"
	,"MemberID":"1983723"
	,"TradeUnitID":"1983723"
	,"LocalID":"1983723"
	,"Volume":182582.18
	,"BuyOffsetFlag":"1"
	,"BuyPrice":182582.18
	,"SellOffsetFlag":"1"
	,"SellPrice":182582.18
	,"Remark":"my_little_tips"
}
```
+ **curl Example**

````
curl -d '{"ExchangeID":"CEX","InstrumentID":"BTCUSD","MemberID":"1983723","TradeUnitID":"1983723","LocalID":"1983723","Volume":"182582.18,"BuyOffsetFlag":"1","BuyPrice":"182582.18,"SellOffsetFlag":"1","SellPrice":"182582.18,"Remark":"my_little_tips"}'  -X POST  http://127.0.0.1:10000/action/v1.0/SendQuoteInsert
````
+ **ws Example**

````
{"RequestNo":2,"SendQuoteInsert":{"ExchangeID":"CEX","InstrumentID":"BTCUSD","MemberID":"1983723","TradeUnitID":"1983723","LocalID":"1983723","Volume":"182582.18,"BuyOffsetFlag":"1","BuyPrice":"182582.18,"SellOffsetFlag":"1","SellPrice":"182582.18,"Remark":"my_little_tips"}}
````
+ **Web rowser Example**

````
http://127.0.0.1:10000/action/v1.0/SendQuoteInsert?ExchangeID=CEX&InstrumentID=BTCUSD&MemberID=1983723&TradeUnitID=1983723&LocalID=1983723&Volume=182582.18&BuyOffsetFlag=1&BuyPrice=182582.18&SellOffsetFlag=1&SellPrice=182582.18&Remark=my_little_tips
````
+ **Request Parameters**

parameter | shortName | required | type | description | remark
-------- | -------- | -------- | -------- | -------- | --------
ExchangeID | ExchangeID | yes | string | 交易所代码 | 
InstrumentID | InstrumentID | yes | string | 标的代码 | 
MemberID | MemberID | no | string | 成员代码 | 
TradeUnitID | TradeUnitID | no | string | 交易单元代码 | 
LocalID | LocalID | no | string | 报价本地标识 | 
Volume | Volume | yes | double | 数量 | 
BuyOffsetFlag | BuyOffsetFlag | no | string | 买方组合开平标志 | 0:开仓;1:平仓;2:强平;3:平今;4:平昨;5:全平;6:指定订单平仓;7:指定成交平仓;8:最大平仓;
BuyPrice | BuyPrice | yes | double | 买方价格 | 
SellOffsetFlag | SellOffsetFlag | no | string | 卖方组合开平标志 | 0:开仓;1:平仓;2:强平;3:平今;4:平昨;5:全平;6:指定订单平仓;7:指定成交平仓;8:最大平仓;
SellPrice | SellPrice | yes | double | 卖方价格 | 
Remark | Remark | no | string | 备注 | 

+ **Response**

```
{
	"action": "RecvQuoteInsert",
	"errorCode": 0,
	"errorMsg": "Success",
	"result": [			
				{"table":"QuoteInsert",
				 "data":{
					,ExchangeID:"CEX"
					,InstrumentID:"BTCUSD"
					,MemberID:"1983723"
					,TradeUnitID:"1983723"
					,LocalID:"1983723"
					,Volume:182582.18
					,BuyOffsetFlag:"1"
					,BuyPrice:182582.18
					,SellOffsetFlag:"1"
					,SellPrice:182582.18
					,Remark:"my_little_tips"
					}
				}
				{"table":"Order",
				 "data":{
					,MemberID:"1983723"
					,TradeUnitID:"1983723"
					,AccountID:"1983723"
					,LocalID:"1983723"
					,ExchangeID:"CEX"
					,InstrumentID:"BTCUSD"
					,OrderPriceType:"1"
					,Direction:"1"
					,OffsetFlag:"1"
					,Price:182582.18
					,Volume:182582.18
					,VolumeDisplay:182582.18
					,VolumeMode:"1"
					,Cost:182582.18
					,OrderType:"1"
					,GTDTime:1657543832
					,MinVolume:182582.18
					,BusinessType:"1"
					,BusinessValue:""
					,CloseOrderID:"1983723"
					,IsCrossMargin:1
					,Remark:"my_little_tips"
					,OrderID:"12376422300333"
					,CopyMemberID:"1983723"
					,CopyOrderID:"1983723"
					,Leverage:125
					,CopyProfitRate:182582.18
					,APPID:"ios"
					,PositionID:"1983723"
					,TriggerPrice:182582.18
					,Reserve:182582.18
					,OrderStatus:"1"
					,DeriveSource:"1"
					,DeriveDetail:""
					,VolumeTraded:182582.18
					,VolumeRemain:182582.18
					,VolumeCancled:182582.18
					,InsertTime:1657543832
					,UpdateTime:1657543832
					,Priority:0
					,TimeSortNo:1657543832
					,FrontNo:1
					,PriceCurrency:"BTC"
					,FeeCurrency:"BTC"
					,ClearCurrency:"BTC"
					,FrozenMoney:182582.18
					,FrozenFee:182582.18
					,FrozenMargin:182582.18
					,Fee:0.22364
					,CloseProfit:10
					,Turnover:182582.18
					,RelatedOrderID:"1983723"
					,BusinessResult:""
					,BusinessNo:11176422300333
					,Tradable:0
					,SettlementGroup:"999"
					,PosiDirection:"1"
					,TradePrice:182582.18
					,OpenPrice:182582.18
					,TriggerOrderID:"1983723"
					,SLTriggerPrice:182582.18
					,TPTriggerPrice:182582.18
					,CopyProfit:182582.18
					,Position:182582.18
					,UserID:"1983723"
					,LastPriceByInsert:182582.18
					,BidPrice1ByInsert:182582.18
					,AskPrice1ByInsert:182582.18
					,Available:182582.18
					,CreateTime:""
					}
				}
	]
}
```

+ **Response Parameters**

QuoteInsert:

parameter | shortName | type | description | remark
-------- | -------- | -------- | -------- | -------- 					
ExchangeID | ExchangeID | string | 交易所代码 | 
InstrumentID | InstrumentID | string | 标的代码 | 
MemberID | MemberID | string | 成员代码 | 
TradeUnitID | TradeUnitID | string | 交易单元代码 | 
LocalID | LocalID | string | 报价本地标识 | 
Volume | Volume | double | 数量 | 
BuyOffsetFlag | BuyOffsetFlag | string | 买方组合开平标志 | 0:开仓;1:平仓;2:强平;3:平今;4:平昨;5:全平;6:指定订单平仓;7:指定成交平仓;8:最大平仓;
BuyPrice | BuyPrice | double | 买方价格 | 
SellOffsetFlag | SellOffsetFlag | string | 卖方组合开平标志 | 0:开仓;1:平仓;2:强平;3:平今;4:平昨;5:全平;6:指定订单平仓;7:指定成交平仓;8:最大平仓;
SellPrice | SellPrice | double | 卖方价格 | 
Remark | Remark | string | 备注 | 

Order:

parameter | shortName | type | description | remark
-------- | -------- | -------- | -------- | -------- 					
MemberID | M | string | 成员代码 | 
TradeUnitID | T | string | 交易单元代码 | 
AccountID | A | string | 资金账号 | 
LocalID | L | string | 报单本地标识 | 
ExchangeID | E | string | 交易所代码 | 
InstrumentID | I | string | 标的代码 | 
OrderPriceType | PT | string | 报单价格类型 | 0:限价(手动指定Price值);1:任意价(Price按最高最低限价取值);2:最优价/对手价(Price按对方1档价取值);3:五档价(Price按对方5档价取值);4:十档价(类同上);7:最优Marker价(Price按本方1档价或其+1Tick取值);8:Price按委托额Cost和订单簿情况确定值;9:保底点差价:成交价格Price不会劣于系统设置差价成交;
Direction | D | string | 买卖方向 | 0:买;1:卖;
OffsetFlag | O | string | 开平标志 | 0:开仓;1:平仓;2:强平;3:平今;4:平昨;5:全平;6:指定订单平仓;7:指定成交平仓;8:最大平仓;
Price | P | double | 报单价格 | 
Volume | V | double | 数量 | 
VolumeDisplay | D | double | 显示数量 | 
VolumeMode | VM | string | 数量取值方式 | 0:百分比;1:绝对值;
Cost | C | double | 委托额 | 
OrderType | OT | string | 订单类型 | 0:普通订单;1:(FillAndKill/IOC)立即完成并且剩余撤销;2:(FillOrKill)立即完成全部或者最小订单要求否则全部撤销;3:需要进入订单簿被动成交,否则全部撤销(只做Maker/PostOnly);4:不需要撮合直接成交;
GTDTime | GT | double | GTD时间 | 
MinVolume | MV | double | 最小成交量 | 
BusinessType | BT | string | 业务类别 | 0:普通;1:申赎;2:行权;3:中立仓申报;4:递延交割申报;5:互换定单;6:质押;7:合并分拆;8:转股;9:回售回购;a:投票;b:融资融券;c:期权执行;d:传递被平仓订单BusinessValue;P:自定义11;Q:自定义10;R:自定义9;S:连续交易;T:自定义7;U:画线委托;V:限价市价;W:条件;X:止盈止损;Y:追踪出场;Z:指标类型;
BusinessValue | BV | string | 业务值 | 
CloseOrderID | CI | string | 平仓指定开仓的订单号 | 
IsCrossMargin | IsCrossMargin | int | 是否全仓 | 
Remark | R | string | 备注 | 
OrderID | OrderID | string | 报单系统唯一代码 | 
CopyMemberID | CopyMemberID | string | 带单员代码 | 
CopyOrderID | CopyOrderID | string | 带单员报单号 | 
Leverage | Leverage | double | 委托单杠杆倍数 | 
CopyProfitRate | CopyProfitRate | double | 带单分配比例 | 
APPID | APPID | string | 应用编号 | 
PositionID | B | string | 持仓代码 | 
TriggerPrice | TriggerPrice | double | 触发价 | 
Reserve | Reserve | double | 保留资金 | 
OrderStatus | OrderStatus | string | 报单状态 | 0:未设置状态;1:全部成交;2:部分成交未撤单;3:部分成交已撤单;4:未成交未撤单;6:无成交已撤单;
DeriveSource | DeriveSource | string | 衍生来源 | 0:正常;1:报价衍生;2:期权执行;3:组合衍生;4:场外成交强平衍生;5:场外成交大宗交易衍生;6:场外成交期转现衍生;7:场外成交只刷单衍生;8:触发订单衍生;9:止损触发订单衍生;a:止盈触发订单衍生;b:资金操作衍生;c:仓位合并衍生;d:CFD订单衍生;e:场外成交ADL衍生;f:回购手续费不能是币;
DeriveDetail | DeriveDetail | string | 衍生明细 | 
VolumeTraded | VolumeTraded | double | 成交数量 | 
VolumeRemain | VolumeRemain | double | 剩余数量 | 
VolumeCancled | VolumeCancled | double | 已经撤单数量 | 
InsertTime | InsertTime | double | 插入时间 | 
UpdateTime | UpdateTime | double | 更新时间 | 
Priority |  | int | 优先权 | 
TimeSortNo |  | double | 按时间排队的序号 | 
FrontNo | FrontNo | int | 前置编号 | 
PriceCurrency | PriceCurrency | string | 计价币种 | 
FeeCurrency | FeeCurrency | string | 手续费币种 | 
ClearCurrency | ClearCurrency | string | 清算币种 | 
FrozenMoney | FrozenMoney | double | 冻结资金 | 
FrozenFee | FrozenFee | double | 冻结手续费 | 
FrozenMargin | FrozenMargin | double | 冻结保证金 | 
Fee | Fee | double | 手续费 | 
CloseProfit | CloseProfit | double | 平仓盈亏 | 
Turnover | Turnover | double | 成交金额 | 
RelatedOrderID | RelatedOrderID | string | 关联报单号 | 
BusinessResult | BusinessResult | string | 业务执行结果 | 
BusinessNo | BusinessNo | double | 业务序列号 | 
Tradable |  | int | 是否可以参加交易 | 
SettlementGroup |  | string | 结算组编号 | 
PosiDirection | PosiDirection | string | 持仓多空方向 | 0:多头;1:空头;2:净;
TradePrice | TradePrice | double | 成交均价 | 
OpenPrice | OpenPrice | double | 平仓成交时的开仓均价 | 
TriggerOrderID | TriggerOrderID | string | 触发报单号 | 
SLTriggerPrice | SLTriggerPrice | double | 开仓报单成交之后的止损触发价 | 
TPTriggerPrice | TPTriggerPrice | double | 开仓报单成交之后的止盈触发价 | 
CopyProfit | CopyProfit | double | 带单盈利分配 | 
Position | Position | double | 成交时的持仓量 | 
UserID | U | string | 交易用户代码 | 
LastPriceByInsert |  | double | 插入时的最新价 | 
BidPrice1ByInsert |  | double | 插入时的买一价 | 
AskPrice1ByInsert |  | double | 插入时的卖一价 | 
Available | Available | double | 可用资金 | 
CreateTime |  | string | 创建时间 | 


## **报价操作请求**

+ **Request URL**

`POST /action/v1.0/SendQuoteAction`


+ **Request Post Body**

```
{
	,"OrderID":"12376422300333"
	,"OrderLocalID":"1983723"
	,"ActionFlag":"1"
	,"LocalID":"1983723"
	,"MemberID":"1983723"
	,"AccountID":"1983723"
	,"Price":182582.18
	,"Volume":182582.18
	,"Direction":"1"
	,"VolumeDisplay":182582.18
	,"ProductGroup":"Swap"
	,"ExchangeID":"CEX"
	,"InstrumentID":"BTCUSD"
	,"Cost":182582.18
	,"Remark":"my_little_tips"
	,"TradeUnitID":"1983723"
}
```
+ **curl Example**

````
curl -d '{"OrderID":"12376422300333","OrderLocalID":"1983723","ActionFlag":"1","LocalID":"1983723","MemberID":"1983723","AccountID":"1983723","Price":"182582.18,"Volume":"182582.18,"Direction":"1","VolumeDisplay":"182582.18,"ProductGroup":"Swap","ExchangeID":"CEX","InstrumentID":"BTCUSD","Cost":"182582.18,"Remark":"my_little_tips","TradeUnitID":"1983723"}'  -X POST  http://127.0.0.1:10000/action/v1.0/SendQuoteAction
````
+ **ws Example**

````
{"RequestNo":2,"SendQuoteAction":{"OrderID":"12376422300333","OrderLocalID":"1983723","ActionFlag":"1","LocalID":"1983723","MemberID":"1983723","AccountID":"1983723","Price":"182582.18,"Volume":"182582.18,"Direction":"1","VolumeDisplay":"182582.18,"ProductGroup":"Swap","ExchangeID":"CEX","InstrumentID":"BTCUSD","Cost":"182582.18,"Remark":"my_little_tips","TradeUnitID":"1983723"}}
````
+ **Web rowser Example**

````
http://127.0.0.1:10000/action/v1.0/SendQuoteAction?OrderID=12376422300333&OrderLocalID=1983723&ActionFlag=1&LocalID=1983723&MemberID=1983723&AccountID=1983723&Price=182582.18&Volume=182582.18&Direction=1&VolumeDisplay=182582.18&ProductGroup=Swap&ExchangeID=CEX&InstrumentID=BTCUSD&Cost=182582.18&Remark=my_little_tips&TradeUnitID=1983723
````
+ **Request Parameters**

parameter | shortName | required | type | description | remark
-------- | -------- | -------- | -------- | -------- | --------
OrderID | OrderID | no | string | 报单系统唯一代码 | 
OrderLocalID | OrderLocalID | no | string | 报单本地标识 | 
ActionFlag | ActionFlag | yes | string | 报单操作标志 | 1:删除;2:挂起;3:激活;4:修改;5:触发;6:增减;
LocalID | LocalID | no | string | 操作本地标识 | 
MemberID | MemberID | no | string | 成员代码 | 
AccountID | AccountID | no | string | 资金账号 | 
Price | Price | no | double | 价格 | 
Volume | Volume | no | double | 数量变化 | 
Direction | Direction | no | string | 买卖方向 | 0:买;1:卖;
VolumeDisplay | D | no | double | 显示数量 | 
ProductGroup | ProductGroup | no | string | 产品组 | 
ExchangeID | ExchangeID | no | string | 交易所代码 | 
InstrumentID | InstrumentID | no | string | 标的代码 | 
Cost | C | no | double | 委托额 | 
Remark | Remark | no | string | 备注 | 
TradeUnitID | TradeUnitID | no | string | 交易单元代码 | 

+ **Response**

```
{
	"action": "RecvQuoteAction",
	"errorCode": 0,
	"errorMsg": "Success",
	"result": [			
				{"table":"QuoteAction",
				 "data":{
					,OrderID:"12376422300333"
					,OrderLocalID:"1983723"
					,ActionFlag:"1"
					,LocalID:"1983723"
					,MemberID:"1983723"
					,AccountID:"1983723"
					,Price:182582.18
					,Volume:182582.18
					,Direction:"1"
					,VolumeDisplay:182582.18
					,ProductGroup:"Swap"
					,ExchangeID:"CEX"
					,InstrumentID:"BTCUSD"
					,Cost:182582.18
					,Remark:"my_little_tips"
					,TradeUnitID:"1983723"
					}
				}
				{"table":"Order",
				 "data":{
					,MemberID:"1983723"
					,TradeUnitID:"1983723"
					,AccountID:"1983723"
					,LocalID:"1983723"
					,ExchangeID:"CEX"
					,InstrumentID:"BTCUSD"
					,OrderPriceType:"1"
					,Direction:"1"
					,OffsetFlag:"1"
					,Price:182582.18
					,Volume:182582.18
					,VolumeDisplay:182582.18
					,VolumeMode:"1"
					,Cost:182582.18
					,OrderType:"1"
					,GTDTime:1657543832
					,MinVolume:182582.18
					,BusinessType:"1"
					,BusinessValue:""
					,CloseOrderID:"1983723"
					,IsCrossMargin:1
					,Remark:"my_little_tips"
					,OrderID:"12376422300333"
					,CopyMemberID:"1983723"
					,CopyOrderID:"1983723"
					,Leverage:125
					,CopyProfitRate:182582.18
					,APPID:"ios"
					,PositionID:"1983723"
					,TriggerPrice:182582.18
					,Reserve:182582.18
					,OrderStatus:"1"
					,DeriveSource:"1"
					,DeriveDetail:""
					,VolumeTraded:182582.18
					,VolumeRemain:182582.18
					,VolumeCancled:182582.18
					,InsertTime:1657543832
					,UpdateTime:1657543832
					,Priority:0
					,TimeSortNo:1657543832
					,FrontNo:1
					,PriceCurrency:"BTC"
					,FeeCurrency:"BTC"
					,ClearCurrency:"BTC"
					,FrozenMoney:182582.18
					,FrozenFee:182582.18
					,FrozenMargin:182582.18
					,Fee:0.22364
					,CloseProfit:10
					,Turnover:182582.18
					,RelatedOrderID:"1983723"
					,BusinessResult:""
					,BusinessNo:11176422300333
					,Tradable:0
					,SettlementGroup:"999"
					,PosiDirection:"1"
					,TradePrice:182582.18
					,OpenPrice:182582.18
					,TriggerOrderID:"1983723"
					,SLTriggerPrice:182582.18
					,TPTriggerPrice:182582.18
					,CopyProfit:182582.18
					,Position:182582.18
					,UserID:"1983723"
					,LastPriceByInsert:182582.18
					,BidPrice1ByInsert:182582.18
					,AskPrice1ByInsert:182582.18
					,Available:182582.18
					,CreateTime:""
					}
				}
	]
}
```

+ **Response Parameters**

QuoteAction:

parameter | shortName | type | description | remark
-------- | -------- | -------- | -------- | -------- 					
OrderID | OrderID | string | 报单系统唯一代码 | 
OrderLocalID | OrderLocalID | string | 报单本地标识 | 
ActionFlag | ActionFlag | string | 报单操作标志 | 1:删除;2:挂起;3:激活;4:修改;5:触发;6:增减;
LocalID | LocalID | string | 操作本地标识 | 
MemberID | MemberID | string | 成员代码 | 
AccountID | AccountID | string | 资金账号 | 
Price | Price | double | 价格 | 
Volume | Volume | double | 数量变化 | 
Direction | Direction | string | 买卖方向 | 0:买;1:卖;
VolumeDisplay | D | double | 显示数量 | 
ProductGroup | ProductGroup | string | 产品组 | 
ExchangeID | ExchangeID | string | 交易所代码 | 
InstrumentID | InstrumentID | string | 标的代码 | 
Cost | C | double | 委托额 | 
Remark | Remark | string | 备注 | 
TradeUnitID | TradeUnitID | string | 交易单元代码 | 

Order:

parameter | shortName | type | description | remark
-------- | -------- | -------- | -------- | -------- 					
MemberID | M | string | 成员代码 | 
TradeUnitID | T | string | 交易单元代码 | 
AccountID | A | string | 资金账号 | 
LocalID | L | string | 报单本地标识 | 
ExchangeID | E | string | 交易所代码 | 
InstrumentID | I | string | 标的代码 | 
OrderPriceType | PT | string | 报单价格类型 | 0:限价(手动指定Price值);1:任意价(Price按最高最低限价取值);2:最优价/对手价(Price按对方1档价取值);3:五档价(Price按对方5档价取值);4:十档价(类同上);7:最优Marker价(Price按本方1档价或其+1Tick取值);8:Price按委托额Cost和订单簿情况确定值;9:保底点差价:成交价格Price不会劣于系统设置差价成交;
Direction | D | string | 买卖方向 | 0:买;1:卖;
OffsetFlag | O | string | 开平标志 | 0:开仓;1:平仓;2:强平;3:平今;4:平昨;5:全平;6:指定订单平仓;7:指定成交平仓;8:最大平仓;
Price | P | double | 报单价格 | 
Volume | V | double | 数量 | 
VolumeDisplay | D | double | 显示数量 | 
VolumeMode | VM | string | 数量取值方式 | 0:百分比;1:绝对值;
Cost | C | double | 委托额 | 
OrderType | OT | string | 订单类型 | 0:普通订单;1:(FillAndKill/IOC)立即完成并且剩余撤销;2:(FillOrKill)立即完成全部或者最小订单要求否则全部撤销;3:需要进入订单簿被动成交,否则全部撤销(只做Maker/PostOnly);4:不需要撮合直接成交;
GTDTime | GT | double | GTD时间 | 
MinVolume | MV | double | 最小成交量 | 
BusinessType | BT | string | 业务类别 | 0:普通;1:申赎;2:行权;3:中立仓申报;4:递延交割申报;5:互换定单;6:质押;7:合并分拆;8:转股;9:回售回购;a:投票;b:融资融券;c:期权执行;d:传递被平仓订单BusinessValue;P:自定义11;Q:自定义10;R:自定义9;S:连续交易;T:自定义7;U:画线委托;V:限价市价;W:条件;X:止盈止损;Y:追踪出场;Z:指标类型;
BusinessValue | BV | string | 业务值 | 
CloseOrderID | CI | string | 平仓指定开仓的订单号 | 
IsCrossMargin | IsCrossMargin | int | 是否全仓 | 
Remark | R | string | 备注 | 
OrderID | OrderID | string | 报单系统唯一代码 | 
CopyMemberID | CopyMemberID | string | 带单员代码 | 
CopyOrderID | CopyOrderID | string | 带单员报单号 | 
Leverage | Leverage | double | 委托单杠杆倍数 | 
CopyProfitRate | CopyProfitRate | double | 带单分配比例 | 
APPID | APPID | string | 应用编号 | 
PositionID | B | string | 持仓代码 | 
TriggerPrice | TriggerPrice | double | 触发价 | 
Reserve | Reserve | double | 保留资金 | 
OrderStatus | OrderStatus | string | 报单状态 | 0:未设置状态;1:全部成交;2:部分成交未撤单;3:部分成交已撤单;4:未成交未撤单;6:无成交已撤单;
DeriveSource | DeriveSource | string | 衍生来源 | 0:正常;1:报价衍生;2:期权执行;3:组合衍生;4:场外成交强平衍生;5:场外成交大宗交易衍生;6:场外成交期转现衍生;7:场外成交只刷单衍生;8:触发订单衍生;9:止损触发订单衍生;a:止盈触发订单衍生;b:资金操作衍生;c:仓位合并衍生;d:CFD订单衍生;e:场外成交ADL衍生;f:回购手续费不能是币;
DeriveDetail | DeriveDetail | string | 衍生明细 | 
VolumeTraded | VolumeTraded | double | 成交数量 | 
VolumeRemain | VolumeRemain | double | 剩余数量 | 
VolumeCancled | VolumeCancled | double | 已经撤单数量 | 
InsertTime | InsertTime | double | 插入时间 | 
UpdateTime | UpdateTime | double | 更新时间 | 
Priority |  | int | 优先权 | 
TimeSortNo |  | double | 按时间排队的序号 | 
FrontNo | FrontNo | int | 前置编号 | 
PriceCurrency | PriceCurrency | string | 计价币种 | 
FeeCurrency | FeeCurrency | string | 手续费币种 | 
ClearCurrency | ClearCurrency | string | 清算币种 | 
FrozenMoney | FrozenMoney | double | 冻结资金 | 
FrozenFee | FrozenFee | double | 冻结手续费 | 
FrozenMargin | FrozenMargin | double | 冻结保证金 | 
Fee | Fee | double | 手续费 | 
CloseProfit | CloseProfit | double | 平仓盈亏 | 
Turnover | Turnover | double | 成交金额 | 
RelatedOrderID | RelatedOrderID | string | 关联报单号 | 
BusinessResult | BusinessResult | string | 业务执行结果 | 
BusinessNo | BusinessNo | double | 业务序列号 | 
Tradable |  | int | 是否可以参加交易 | 
SettlementGroup |  | string | 结算组编号 | 
PosiDirection | PosiDirection | string | 持仓多空方向 | 0:多头;1:空头;2:净;
TradePrice | TradePrice | double | 成交均价 | 
OpenPrice | OpenPrice | double | 平仓成交时的开仓均价 | 
TriggerOrderID | TriggerOrderID | string | 触发报单号 | 
SLTriggerPrice | SLTriggerPrice | double | 开仓报单成交之后的止损触发价 | 
TPTriggerPrice | TPTriggerPrice | double | 开仓报单成交之后的止盈触发价 | 
CopyProfit | CopyProfit | double | 带单盈利分配 | 
Position | Position | double | 成交时的持仓量 | 
UserID | U | string | 交易用户代码 | 
LastPriceByInsert |  | double | 插入时的最新价 | 
BidPrice1ByInsert |  | double | 插入时的买一价 | 
AskPrice1ByInsert |  | double | 插入时的卖一价 | 
Available | Available | double | 可用资金 | 
CreateTime |  | string | 创建时间 | 


## **订阅主题请求**

+ **Request URL**

`POST /action/v1.0/SendTopicAction`


+ **Request Post Body**

```
{
	,"Action":"1"
	,"TopicID":"1983723"
	,"Index":""
	,"ResumeNo":182582.18
	,"BusinessNo":11176422300333
	,"Token":""
	,"Zip":0
	,"Frequency":0
}
```
+ **curl Example**

````
curl -d '{"Action":"1","TopicID":"1983723","Index":"","ResumeNo":"182582.18,"BusinessNo":"11176422300333,"Token":"","Zip":"0,"Frequency":"0}'  -X POST  http://127.0.0.1:10000/action/v1.0/SendTopicAction
````
+ **ws Example**

````
{"RequestNo":2,"SendTopicAction":{"Action":"1","TopicID":"1983723","Index":"","ResumeNo":"182582.18,"BusinessNo":"11176422300333,"Token":"","Zip":"0,"Frequency":"0}}
````
+ **Web rowser Example**

````
http://127.0.0.1:10000/action/v1.0/SendTopicAction?Action=1&TopicID=1983723&Index=&ResumeNo=182582.18&BusinessNo=11176422300333&Token=&Zip=0&Frequency=0
````
+ **Request Parameters**

parameter | shortName | required | type | description | remark
-------- | -------- | -------- | -------- | -------- | --------
Action | A | yes | string | 订阅操作 | 0:全部退订;1:订阅;2:退订;
TopicID | T | no | string | 请参考TOPICID类型定义 | 
Index | I | no | string | 过滤值 | 
ResumeNo | R | no | double | 续传号:0是从头开始,-1是从服务端最新位置续传 | 
BusinessNo | B | no | double | 续传号:0是从头开始,不能为负数 | 
Token | Token | no | string | 令牌 | 
Zip | Zip | no | int | 压缩方式 | 
Frequency | Frequency | no | int | 每间隔多少频率读取一次 | 

+ **Response**

```
{
	"action": "RecvTopicAction",
	"errorCode": 0,
	"errorMsg": "Success",
	"result": [			
				{"table":"TopicAction",
				 "data":{
					,Action:"1"
					,TopicID:"1983723"
					,Index:""
					,ResumeNo:182582.18
					,BusinessNo:11176422300333
					,Token:""
					,Zip:0
					,Frequency:0
					}
				}
	]
}
```

+ **Response Parameters**

TopicAction:

parameter | shortName | type | description | remark
-------- | -------- | -------- | -------- | -------- 					
Action | A | string | 订阅操作 | 0:全部退订;1:订阅;2:退订;
TopicID | T | string | 请参考TOPICID类型定义 | 
Index | I | string | 过滤值 | 
ResumeNo | R | double | 续传号:0是从头开始,-1是从服务端最新位置续传 | 
BusinessNo | B | double | 续传号:0是从头开始,不能为负数 | 
Token | Token | string | 令牌 | 
Zip | Zip | int | 压缩方式 | 
Frequency | Frequency | int | 每间隔多少频率读取一次 | 


## **设置行情请求**

+ **Request URL**

`POST /action/v1.0/SendMarketDataUpdate`


+ **Request Post Body**

```
{
	,"ExchangeID":"CEX"
	,"InstrumentID":"BTCUSD"
	,"MarkedPrice":182582.18
	,"TheoryPrice":182582.18
	,"PreSettlementPrice":182582.18
	,"SettlementPrice":182582.18
	,"UnderlyingPrice":182582.18
	,"PrePositionFeeRate":182582.18
	,"PositionFeeRate":182582.18
	,"InstrumentStatus":"1"
	,"Volume":182582.18
	,"Turnover":182582.18
	,"Remark":"my_little_tips"
}
```
+ **curl Example**

````
curl -d '{"ExchangeID":"CEX","InstrumentID":"BTCUSD","MarkedPrice":"182582.18,"TheoryPrice":"182582.18,"PreSettlementPrice":"182582.18,"SettlementPrice":"182582.18,"UnderlyingPrice":"182582.18,"PrePositionFeeRate":"182582.18,"PositionFeeRate":"182582.18,"InstrumentStatus":"1","Volume":"182582.18,"Turnover":"182582.18,"Remark":"my_little_tips"}'  -X POST  http://127.0.0.1:10000/action/v1.0/SendMarketDataUpdate
````
+ **ws Example**

````
{"RequestNo":2,"SendMarketDataUpdate":{"ExchangeID":"CEX","InstrumentID":"BTCUSD","MarkedPrice":"182582.18,"TheoryPrice":"182582.18,"PreSettlementPrice":"182582.18,"SettlementPrice":"182582.18,"UnderlyingPrice":"182582.18,"PrePositionFeeRate":"182582.18,"PositionFeeRate":"182582.18,"InstrumentStatus":"1","Volume":"182582.18,"Turnover":"182582.18,"Remark":"my_little_tips"}}
````
+ **Web rowser Example**

````
http://127.0.0.1:10000/action/v1.0/SendMarketDataUpdate?ExchangeID=CEX&InstrumentID=BTCUSD&MarkedPrice=182582.18&TheoryPrice=182582.18&PreSettlementPrice=182582.18&SettlementPrice=182582.18&UnderlyingPrice=182582.18&PrePositionFeeRate=182582.18&PositionFeeRate=182582.18&InstrumentStatus=1&Volume=182582.18&Turnover=182582.18&Remark=my_little_tips
````
+ **Request Parameters**

parameter | shortName | required | type | description | remark
-------- | -------- | -------- | -------- | -------- | --------
ExchangeID | ExchangeID | yes | string | 交易所代码 | 
InstrumentID | InstrumentID | yes | string | 标的代码 | 
MarkedPrice | MarkedPrice | no | double | 合理价格 | 
TheoryPrice | TheoryPrice | no | double | 理论价 | 
PreSettlementPrice | PreSettlementPrice | no | double | 上次结算价 | 
SettlementPrice | SettlementPrice | no | double | 结算价 | 
UnderlyingPrice | UnderlyingPrice | no | double | 基础标的价格 | 
PrePositionFeeRate | PrePositionFeeRate | no | double | 持仓费按比例 | 
PositionFeeRate | PositionFeeRate | no | double | 预计持仓费按比例 | 
InstrumentStatus | InstrumentStatus | no | string | 标的交易状态 | 0:开盘前;1:非交易;2:连续交易;3:集合竞价报单;4:集合竞价价格平衡;5:集合竞价撮合;6:收盘;7:不活跃;
Volume | Volume | no | double | 数量 | 
Turnover | Turnover | no | double | 成交金额 | 
Remark | Remark | no | string | 备注 | 

+ **Response**

```
{
	"action": "RecvMarketDataUpdate",
	"errorCode": 0,
	"errorMsg": "Success",
	"result": [			
				{"table":"MarketDataUpdate",
				 "data":{
					,ExchangeID:"CEX"
					,InstrumentID:"BTCUSD"
					,MarkedPrice:182582.18
					,TheoryPrice:182582.18
					,PreSettlementPrice:182582.18
					,SettlementPrice:182582.18
					,UnderlyingPrice:182582.18
					,PrePositionFeeRate:182582.18
					,PositionFeeRate:182582.18
					,InstrumentStatus:"1"
					,Volume:182582.18
					,Turnover:182582.18
					,Remark:"my_little_tips"
					}
				}
				{"table":"MarketData",
				 "data":{
					,ExchangeID:"CEX"
					,InstrumentID:"BTCUSD"
					,ProductGroup:"Swap"
					,UpdateTime:1657543832
					,PreClosePrice:182582.18
					,PreSettlementPrice:182582.18
					,PreOpenInterest:182582.18
					,PrePositionFeeRate:182582.18
					,ClosePrice:182582.18
					,PositionFeeTime:1657543832
					,SettlementTime:1657543832
					,UpperLimitPrice:182582.18
					,LowerLimitPrice:182582.18
					,UnderlyingPrice:182582.18
					,MarkedPrice:182582.18
					,PositionFeeRate:182582.18
					,HighestPrice:182582.18
					,LowestPrice:182582.18
					,LastPrice:182582.18
					,Volume:182582.18
					,Turnover:182582.18
					,OpenInterest:182582.18
					,SettlementPrice:182582.18
					,OpenPrice:182582.18
					,IsPubMarketData:1
					,InstrumentStatus:"1"
					,Remark:"my_little_tips"
					,BusinessNo:11176422300333
					,MarkedPriceTotal:182582.18
					,MarkedPriceWeight:182582.18
					,L25UpperPrice:182582.18
					,L25LowerPrice:182582.18
					,L25UpperPrice1:182582.18
					,L25LowerPrice1:182582.18
					,L25UpperPrice2:182582.18
					,L25LowerPrice2:182582.18
					,BidPrice1:182582.18
					,BidVolume1:182582.18
					,AskPrice1:182582.18
					,AskVolume1:182582.18
					,TheoryPrice:182582.18
					,HighestPrice24:182582.18
					,LowestPrice24:182582.18
					,Volume24:182582.18
					,Turnover24:182582.18
					,OpenPrice24:182582.18
					,Frequency:0
					,InsertTime:1657543832
					}
				}
				{"table":"Instrument",
				 "data":{
					,ExchangeID:"CEX"
					,InstrumentID:"BTCUSD"
					,InstrumentName:""
					,SettlementGroup:"999"
					,ProductGroup:"Swap"
					,UnderlyingID:"1983723"
					,ProductClass:"1"
					,ProductType:"1"
					,StrikePrice:182582.18
					,OptionsType:"1"
					,VolumeMultiple:182582.18
					,StartTime:1657543832
					,ExpireTime:1657543832
					,PriceCurrency:"BTC"
					,ClearCurrency:"BTC"
					,IsInverse:1
					,BaseCurrency:"BTC"
					,MarginPriceType:"1"
					,BasisPrice:182582.18
					,MinOrderVolume:182582.18
					,MinOrderCost:182582.18
					,MaxOrderVolume:182582.18
					,PriceTick:182582.18
					,VolumeTick:182582.18
					,ShowVolumeTick:182582.18
					,TradingModel:"1"
					,DayStartTime:""
					,SortNumber:-1
					,Remark:"my_little_tips"
					,DefaultLeverage:125
					,PriceLimitValueMode:"1"
					,PriceLimitPrice:"1"
					,PriceLimitUpperValue:182582.18
					,PriceLimitLowerValue:182582.18
					,MarginRateGroup:"999"
					,CFDGroup:"999"
					,FeeGroup:"999"
					,TradingRightGroup:"999"
					,MaxOpenInterest:182582.18
					,FundingRateGroup:"999"
					,InsertTime:1657543832
					,UpdateTime:1657543832
					,MarkedPriceTimes:0
					}
				}
	]
}
```

+ **Response Parameters**

MarketDataUpdate:

parameter | shortName | type | description | remark
-------- | -------- | -------- | -------- | -------- 					
ExchangeID | ExchangeID | string | 交易所代码 | 
InstrumentID | InstrumentID | string | 标的代码 | 
MarkedPrice | MarkedPrice | double | 合理价格 | 
TheoryPrice | TheoryPrice | double | 理论价 | 
PreSettlementPrice | PreSettlementPrice | double | 上次结算价 | 
SettlementPrice | SettlementPrice | double | 结算价 | 
UnderlyingPrice | UnderlyingPrice | double | 基础标的价格 | 
PrePositionFeeRate | PrePositionFeeRate | double | 持仓费按比例 | 
PositionFeeRate | PositionFeeRate | double | 预计持仓费按比例 | 
InstrumentStatus | InstrumentStatus | string | 标的交易状态 | 0:开盘前;1:非交易;2:连续交易;3:集合竞价报单;4:集合竞价价格平衡;5:集合竞价撮合;6:收盘;7:不活跃;
Volume | Volume | double | 数量 | 
Turnover | Turnover | double | 成交金额 | 
Remark | Remark | string | 备注 | 

MarketData:

parameter | shortName | type | description | remark
-------- | -------- | -------- | -------- | -------- 					
ExchangeID | X | string | 交易所代码 | 
InstrumentID | I | string | 标的代码 | 
ProductGroup | G | string | 产品组 | 
UpdateTime | U | double | 更新时间 | 
PreClosePrice |  | double | 上次收盘 | 
PreSettlementPrice |  | double | 上次结算价 | 
PreOpenInterest |  | double | 上次持仓量 | 
PrePositionFeeRate |  | double | 上次持仓费按比例 | 
ClosePrice | ClosePrice | double | 收盘价 | 
PositionFeeTime | PF | double | 收取持仓费用时间 | 
SettlementTime | SettlementTime | double | 结算时间 | 
UpperLimitPrice | C | double | 最高限价 | 
LowerLimitPrice | F | double | 最低限价 | 
UnderlyingPrice | D | double | 基础标的价格 | 
MarkedPrice | M | double | 标记价格 | 
PositionFeeRate | R | double | 预计持仓费按比例 | 
HighestPrice | H | double | 当日最高价 | 
LowestPrice | L | double | 当日最低价 | 
LastPrice | N | double | 最新价 | 
Volume | V | double | 当日数量 | 
Turnover | T | double | 当日成交金额 | 
OpenInterest | P | double | 持仓量 | 
SettlementPrice | S | double | 预估结算价 | 
OpenPrice | O | double | 当日开盘价 | 
IsPubMarketData |  | int | 行情发布状态 | 
InstrumentStatus | B | string | 标的交易状态 | 0:开盘前;1:非交易;2:连续交易;3:集合竞价报单;4:集合竞价价格平衡;5:集合竞价撮合;6:收盘;7:不活跃;
Remark | RE | string | 备注 | 
BusinessNo |  | double | 业务序列号 | 
MarkedPriceTotal |  | double | 标记价格汇总 | 
MarkedPriceWeight |  | double | 标记价格权重 | 
L25UpperPrice |  | double | 25档行情最高价 | 
L25LowerPrice |  | double | 25档行情最低价 | 
L25UpperPrice1 |  | double | 25档行情最高价1 | 
L25LowerPrice1 |  | double | 25档行情最低价1 | 
L25UpperPrice2 |  | double | 25档行情最高价2 | 
L25LowerPrice2 |  | double | 25档行情最低价2 | 
BidPrice1 | BP1 | double | 申买价一 | 
BidVolume1 | BV1 | double | 申买量一 | 
AskPrice1 | AP1 | double | 申卖价一 | 
AskVolume1 | AV1 | double | 申卖量一 | 
TheoryPrice | TP | double | 理论价 | 
HighestPrice24 | H2 | double | 24小时最高价 | 
LowestPrice24 | L2 | double | 24小时最低价 | 
Volume24 | V2 | double | 24小时数量 | 
Turnover24 | T2 | double | 24小时成交金额 | 
OpenPrice24 | O2 | double | 24小时开盘价 | 
Frequency |  | int | 每秒更新的次数 | 
InsertTime |  | double | 插入时间 | 

Instrument:

parameter | shortName | type | description | remark
-------- | -------- | -------- | -------- | -------- 					
ExchangeID | X | string | 交易所代码 | 
InstrumentID | I | string | 标的代码 | 
InstrumentName | N | string | 标的名称 | 
SettlementGroup |  | string | 结算组编号 | 
ProductGroup | G | string | 产品组 | 
UnderlyingID |  | string | 基础标的代码 | 
ProductClass |  | string | 产品大类 | 1:保证金交易;2:权利金交易;3:资产转让（股票/基金/国债等）;4:兑换交易（资金之间的互换）;5:组合;6:行情;
ProductType |  | string | 产品类型 | 1:交割期货;2:永续、利率互换期货;3:即期/日交割期货;4:递延交割期货;5:权证;6:美式期权;7:欧式期权;8:币币交易;9:外汇兑换;a:股票;b:债券;c:基金;
StrikePrice |  | double | 执行价 | 
OptionsType |  | string | 期权类型 | 0:非期权;1:看涨;2:看跌;
VolumeMultiple | VM | double | 数量乘数 | 
StartTime |  | double | 启动时间 | 
ExpireTime |  | double | 到期时间 | 
PriceCurrency | PC | string | 计价币种 | 
ClearCurrency | CC | string | 清算币种 | 
IsInverse | II | int | 是否反向标的 | 
BaseCurrency | BB | string | 标的基础货币 | 
MarginPriceType |  | string | 保证金价格类型 | 0:不按比例按固定值,无需价格;1:昨结算价;2:开仓价;3:标记价;4:最新价;
BasisPrice |  | double | 挂牌基准价 | 
MinOrderVolume | MOV | double | 最小下单量 | 
MinOrderCost | MOC | double | 最小下单额 | 
MaxOrderVolume |  | double | 最大下单量 | 
PriceTick | PT | double | 最小变动价位 | 
VolumeTick | VT | double | 最小变动量 | 
ShowVolumeTick |  | double | 前端行情显示最小变动量(服务端不用) | 
TradingModel |  | string | 交易模式 | 0:价格优先时间优先;1:价格优先按比例分配;2:只能和做市商报价成交;3:没有撮合;
DayStartTime |  | string | 每日开始交易时间 | 
SortNumber |  | int | 排序字段 | 
Remark | RE | string | 备注 | 
DefaultLeverage | DL | double | 默认杠杆倍数 | 
PriceLimitValueMode |  | string | 取值方式 | 0:百分比;1:绝对值;
PriceLimitPrice |  | string | 限价价格类型 | 1:昨结算价;2:最新价;3:基础标的价;4:标记价;
PriceLimitUpperValue |  | double | 上限 | 
PriceLimitLowerValue |  | double | 下限 | 
MarginRateGroup |  | string | 保证金组 | 
CFDGroup |  | string | 差价组 | 
FeeGroup |  | string | 手续费组 | 
TradingRightGroup |  | string | 权限组 | 
MaxOpenInterest |  | double | 最大持仓量 | 
FundingRateGroup |  | string | 资金费用组 | 
InsertTime |  | double | 插入时间 | 
UpdateTime |  | double | 更新时间 | 
MarkedPriceTimes |  | int | 标记价格计算次数(0:最新价;1-n:次数;空:不生成标记价) | 


## **调整持仓请求**

+ **Request URL**

`POST /action/v1.0/SendPositionAction`


+ **Request Post Body**

```
{
	,"LocalID":"1983723"
	,"PositionID":"1983723"
	,"ExchangeID":"CEX"
	,"InstrumentID":"BTCUSD"
	,"ProductGroup":"Swap"
	,"MemberID":"1983723"
	,"TradeUnitID":"1983723"
	,"AccountID":"1983723"
	,"PosiDirection":"1"
	,"Amount":182582.18
	,"LongLeverage":125
	,"ShortLeverage":125
	,"NetLeverage":125
	,"ActionType":"1"
	,"IsCrossMargin":1
	,"IsAutoAddMargin":1
	,"Remark":"my_little_tips"
}
```
+ **curl Example**

````
curl -d '{"LocalID":"1983723","PositionID":"1983723","ExchangeID":"CEX","InstrumentID":"BTCUSD","ProductGroup":"Swap","MemberID":"1983723","TradeUnitID":"1983723","AccountID":"1983723","PosiDirection":"1","Amount":"182582.18,"LongLeverage":"125,"ShortLeverage":"125,"NetLeverage":"125,"ActionType":"1","IsCrossMargin":"1,"IsAutoAddMargin":"1,"Remark":"my_little_tips"}'  -X POST  http://127.0.0.1:10000/action/v1.0/SendPositionAction
````
+ **ws Example**

````
{"RequestNo":2,"SendPositionAction":{"LocalID":"1983723","PositionID":"1983723","ExchangeID":"CEX","InstrumentID":"BTCUSD","ProductGroup":"Swap","MemberID":"1983723","TradeUnitID":"1983723","AccountID":"1983723","PosiDirection":"1","Amount":"182582.18,"LongLeverage":"125,"ShortLeverage":"125,"NetLeverage":"125,"ActionType":"1","IsCrossMargin":"1,"IsAutoAddMargin":"1,"Remark":"my_little_tips"}}
````
+ **Web rowser Example**

````
http://127.0.0.1:10000/action/v1.0/SendPositionAction?LocalID=1983723&PositionID=1983723&ExchangeID=CEX&InstrumentID=BTCUSD&ProductGroup=Swap&MemberID=1983723&TradeUnitID=1983723&AccountID=1983723&PosiDirection=1&Amount=182582.18&LongLeverage=125&ShortLeverage=125&NetLeverage=125&ActionType=1&IsCrossMargin=1&IsAutoAddMargin=1&Remark=my_little_tips
````
+ **Request Parameters**

parameter | shortName | required | type | description | remark
-------- | -------- | -------- | -------- | -------- | --------
LocalID | LocalID | no | string | 出入金本地号 | 
PositionID | PositionID | no | string | 持仓代码 | 
ExchangeID | ExchangeID | yes | string | 交易所代码 | 
InstrumentID | InstrumentID | yes | string | 标的代码 | 
ProductGroup | ProductGroup | no | string | 产品组 | 
MemberID | MemberID | yes | string | 成员代码 | 
TradeUnitID | TradeUnitID | yes | string | 交易单元代码 | 
AccountID | AccountID | no | string | 资金账号 | 
PosiDirection | PosiDirection | no | string | 持仓多空方向，0:多;1:空;2:净;空值:多空方向同时操作 | 0:多头;1:空头;2:净;
Amount | Amount | yes | double | 发生额 | 
LongLeverage | LongLeverage | no | double | 杠杆倍数 | 
ShortLeverage | ShortLeverage | no | double | 杠杆倍数 | 
NetLeverage | NetLeverage | no | double | 杠杆倍数 | 
ActionType | ActionType | yes | string | 操作类型 | 1:调整杠杆倍数;2:增加保证金;3:减少保证金;4:全仓逐仓模式调换;5:调整不超过最大杠杆倍数;6:调整默认杠杆倍数;7:调整默认全逐仓;8:调整成本价;a:全平;b:最大平仓;c:清理之后才能切换的全逐仓切换;
IsCrossMargin | IsCrossMargin | no | int | 是否全仓 | 
IsAutoAddMargin | IsAutoAddMargin | no | int | 是否自动追加保证金 | 
Remark | R | no | string | 备注 | 

+ **Response**

```
{
	"action": "RecvPositionAction",
	"errorCode": 0,
	"errorMsg": "Success",
	"result": [			
				{"table":"PositionAction",
				 "data":{
					,LocalID:"1983723"
					,PositionID:"1983723"
					,ExchangeID:"CEX"
					,InstrumentID:"BTCUSD"
					,ProductGroup:"Swap"
					,MemberID:"1983723"
					,TradeUnitID:"1983723"
					,AccountID:"1983723"
					,PosiDirection:"1"
					,Amount:182582.18
					,LongLeverage:125
					,ShortLeverage:125
					,NetLeverage:125
					,ActionType:"1"
					,IsCrossMargin:1
					,IsAutoAddMargin:1
					,Remark:"my_little_tips"
					}
				}
				{"table":"Position",
				 "data":{
					,PositionID:"1983723"
					,MemberID:"1983723"
					,TradeUnitID:"1983723"
					,ExchangeID:"CEX"
					,InstrumentID:"BTCUSD"
					,PositionClass:"1"
					,PosiDirection:"1"
					,PrePosition:182582.18
					,Position:182582.18
					,LongFrozen:182582.18
					,ShortFrozen:182582.18
					,PreLongFrozen:182582.18
					,PreShortFrozen:182582.18
					,HighestPosition:182582.18
					,ClosePosition:182582.18
					,PositionCost:182582.18
					,CostPrice:182582.18
					,UseMargin:182582.18
					,FrozenMargin:182582.18
					,LongFrozenMargin:182582.18
					,ShortFrozenMargin:182582.18
					,CloseProfit:10
					,TotalPositionCost:182582.18
					,TotalCloseProfit:182582.18
					,OpenPrice:182582.18
					,ClosePrice:182582.18
					,TradeFee:182582.18
					,PositionFee:182582.18
					,Leverage:125
					,AccountID:"1983723"
					,PriceCurrency:"BTC"
					,ClearCurrency:"BTC"
					,SettlementGroup:"999"
					,IsCrossMargin:1
					,CloseOrderID:"1983723"
					,SLTriggerPrice:182582.18
					,TPTriggerPrice:182582.18
					,BeginTime:1657543832
					,InsertTime:1657543832
					,LastOpenTime:1657543832
					,UpdateTime:1657543832
					,BusinessNo:11176422300333
					,IsAutoAddMargin:1
					,Frequency:0
					,MaintMargin:182582.18
					,UnRealProfit:182582.18
					,LiquidPrice:182582.18
					,CreateTime:""
					,CopyMemberID:"1983723"
					,CopyProfitRate:182582.18
					,CopyProfit:182582.18
					,FirstTradeID:"1983723"
					,LastTradeID:"1983723"
					,BusinessType:"1"
					,BusinessValue:""
					,Reserve:182582.18
					,ReserveProfit:182582.18
					,Remark:"my_little_tips"
					}
				}
	]
}
```

+ **Response Parameters**

PositionAction:

parameter | shortName | type | description | remark
-------- | -------- | -------- | -------- | -------- 					
LocalID | LocalID | string | 出入金本地号 | 
PositionID | PositionID | string | 持仓代码 | 
ExchangeID | ExchangeID | string | 交易所代码 | 
InstrumentID | InstrumentID | string | 标的代码 | 
ProductGroup | ProductGroup | string | 产品组 | 
MemberID | MemberID | string | 成员代码 | 
TradeUnitID | TradeUnitID | string | 交易单元代码 | 
AccountID | AccountID | string | 资金账号 | 
PosiDirection | PosiDirection | string | 持仓多空方向，0:多;1:空;2:净;空值:多空方向同时操作 | 0:多头;1:空头;2:净;
Amount | Amount | double | 发生额 | 
LongLeverage | LongLeverage | double | 杠杆倍数 | 
ShortLeverage | ShortLeverage | double | 杠杆倍数 | 
NetLeverage | NetLeverage | double | 杠杆倍数 | 
ActionType | ActionType | string | 操作类型 | 1:调整杠杆倍数;2:增加保证金;3:减少保证金;4:全仓逐仓模式调换;5:调整不超过最大杠杆倍数;6:调整默认杠杆倍数;7:调整默认全逐仓;8:调整成本价;a:全平;b:最大平仓;c:清理之后才能切换的全逐仓切换;
IsCrossMargin | IsCrossMargin | int | 是否全仓 | 
IsAutoAddMargin | IsAutoAddMargin | int | 是否自动追加保证金 | 
Remark | R | string | 备注 | 

Position:

parameter | shortName | type | description | remark
-------- | -------- | -------- | -------- | -------- 					
PositionID | PositionID | string | 持仓代码 | 
MemberID | MemberID | string | 成员代码 | 
TradeUnitID | TradeUnitID | string | 交易单元代码 | 
ExchangeID | ExchangeID | string | 交易所代码 | 
InstrumentID | InstrumentID | string | 标的代码 | 
PositionClass | PositionClass | string | 持仓类型 | 0:不设置;1:保证金交易;3:资产;4:持币成本;
PosiDirection | PosiDirection | string | 持仓多空方向 | 0:多头;1:空头;2:净;
PrePosition | PrePosition | double | 上次持仓 | 
Position | Position | double | 总持仓 | 
LongFrozen | LongFrozen | double | 多头冻结 | 
ShortFrozen | ShortFrozen | double | 空头冻结 | 
PreLongFrozen | PreLongFrozen | double | 昨日多头冻结 | 
PreShortFrozen | PreShortFrozen | double | 昨日空头冻结 | 
HighestPosition | HighestPosition | double | 最大持仓 | 
ClosePosition | ClosePosition | double | 可平持仓 | 
PositionCost | PositionCost | double | 持仓成本 | 
CostPrice | CostPrice | double | 成本价 | 
UseMargin | UseMargin | double | 占用保证金 | 
FrozenMargin | FrozenMargin | double | 冻结保证金 | 
LongFrozenMargin | LongFrozenMargin | double | 多头冻结保证金 | 
ShortFrozenMargin | ShortFrozenMargin | double | 空头冻结保证金 | 
CloseProfit | CloseProfit | double | 平仓盈亏 | 
TotalPositionCost | TotalPositionCost | double | 开仓成本 | 
TotalCloseProfit | TotalCloseProfit | double | 总平仓盈亏 | 
OpenPrice | OpenPrice | double | 开仓均价 | 
ClosePrice | ClosePrice | double | 平仓均价 | 
TradeFee | TradeFee | double | 交易费用 | 
PositionFee | PositionFee | double | 持仓费用或者股票分红等 | 
Leverage | Leverage | double | 杠杆倍数 | 
AccountID | AccountID | string | 资金账号 | 
PriceCurrency | PriceCurrency | string | 计价币种 | 
ClearCurrency | ClearCurrency | string | 清算币种 | 
SettlementGroup | SettlementGroup | string | 结算组编号 | 
IsCrossMargin | IsCrossMargin | int | 是否全仓 | 
CloseOrderID | CloseOrderID | string | 平仓单代码 | 
SLTriggerPrice | SLTriggerPrice | double | 止损触发价 | 
TPTriggerPrice | TPTriggerPrice | double | 止盈触发价 | 
BeginTime | BeginTime | double | 持仓不为0的开始时间 | 
InsertTime | InsertTime | double | 插入时间(新持仓时间) | 
LastOpenTime | LastOpenTime | double | 最后开仓时间 | 
UpdateTime | UpdateTime | double | 更新时间 | 
BusinessNo | BusinessNo | double | 业务序列号 | 
IsAutoAddMargin | IsAutoAddMargin | int | 是否自动追加保证金 | 
Frequency | Frequency | int | 每秒更新的次数 | 
MaintMargin | MaintMargin | double | 维持保证金 | 
UnRealProfit | UnRealProfit | double | 未实现盈亏 | 
LiquidPrice | LiquidPrice | double | 清算价格 | 
CreateTime | CreateTime | string | 创建时间 | 
CopyMemberID | CopyMemberID | string | 带单员代码 | 
CopyProfitRate | CopyProfitRate | double | 带单分配比例 | 
CopyProfit | CopyProfit | double | 带单盈利分配 | 
FirstTradeID | FID | string | 初始成交订单号 | 
LastTradeID | LID | string | 结束成交订单号 | 
BusinessType | BT | string | 业务类别 | 0:普通;1:申赎;2:行权;3:中立仓申报;4:递延交割申报;5:互换定单;6:质押;7:合并分拆;8:转股;9:回售回购;a:投票;b:融资融券;c:期权执行;d:传递被平仓订单BusinessValue;P:自定义11;Q:自定义10;R:自定义9;S:连续交易;T:自定义7;U:画线委托;V:限价市价;W:条件;X:止盈止损;Y:追踪出场;Z:指标类型;
BusinessValue | BV | string | 业务值 | 
Reserve | Reserve | double | 保留资金 | 
ReserveProfit | ReserveProfit | double | 保留资金盈亏 | 
Remark | R | string | 备注 | 


## **用户出入金请求**

+ **Request URL**

`POST /action/v1.0/SendAccountAction`


+ **Request Post Body**

```
{
	,"ActionLocalID":"1983723"
	,"LocalID":"1983723"
	,"OrderID":"12376422300333"
	,"MemberID":"1983723"
	,"AccountID":"1983723"
	,"SettlementGroup":"999"
	,"OtherAccountID":"1983723"
	,"OtherSettlementGroup":"999"
	,"OtherMemberID":"1983723"
	,"Amount":182582.18
	,"FrozenMoney":182582.18
	,"Currency":"BTC"
	,"ActionType":"1"
	,"IsReserve":1
	,"AccountType":""
	,"MarginRateGrade":""
	,"TradingRightGrade":""
	,"Price":182582.18
	,"Volume":182582.18
	,"ExchangeID":"CEX"
	,"InstrumentID":"BTCUSD"
	"Source":""
	,"RelatedID":"1983723"
	,"FeeDeduceRate":182582.18
	,"Remark":"my_little_tips"
}
```
+ **curl Example**

````
curl -d '{"ActionLocalID":"1983723","LocalID":"1983723","OrderID":"12376422300333","MemberID":"1983723","AccountID":"1983723","SettlementGroup":"999","OtherAccountID":"1983723","OtherSettlementGroup":"999","OtherMemberID":"1983723","Amount":"182582.18,"FrozenMoney":"182582.18,"Currency":"BTC","ActionType":"1","IsReserve":"1,"AccountType":"","MarginRateGrade":"","TradingRightGrade":"","Price":"182582.18,"Volume":"182582.18,"ExchangeID":"CEX","InstrumentID":"BTCUSD","Source":"","RelatedID":"1983723","FeeDeduceRate":"182582.18,"Remark":"my_little_tips"}'  -X POST  http://127.0.0.1:10000/action/v1.0/SendAccountAction
````
+ **ws Example**

````
{"RequestNo":2,"SendAccountAction":{"ActionLocalID":"1983723","LocalID":"1983723","OrderID":"12376422300333","MemberID":"1983723","AccountID":"1983723","SettlementGroup":"999","OtherAccountID":"1983723","OtherSettlementGroup":"999","OtherMemberID":"1983723","Amount":"182582.18,"FrozenMoney":"182582.18,"Currency":"BTC","ActionType":"1","IsReserve":"1,"AccountType":"","MarginRateGrade":"","TradingRightGrade":"","Price":"182582.18,"Volume":"182582.18,"ExchangeID":"CEX","InstrumentID":"BTCUSD","Source":"","RelatedID":"1983723","FeeDeduceRate":"182582.18,"Remark":"my_little_tips"}}
````
+ **Web rowser Example**

````
http://127.0.0.1:10000/action/v1.0/SendAccountAction?ActionLocalID=1983723&LocalID=1983723&OrderID=12376422300333&MemberID=1983723&AccountID=1983723&SettlementGroup=999&OtherAccountID=1983723&OtherSettlementGroup=999&OtherMemberID=1983723&Amount=182582.18&FrozenMoney=182582.18&Currency=BTC&ActionType=1&IsReserve=1&AccountType=&MarginRateGrade=&TradingRightGrade=&Price=182582.18&Volume=182582.18&ExchangeID=CEX&InstrumentID=BTCUSD&Source=&RelatedID=1983723&FeeDeduceRate=182582.18&Remark=my_little_tips
````
+ **Request Parameters**

parameter | shortName | required | type | description | remark
-------- | -------- | -------- | -------- | -------- | --------
ActionLocalID | ActionLocalID | no | string | 关联的之前的资金操作本地号 | 
LocalID | LocalID | no | string | 出入金本地号 | 
OrderID | OrderID | no | string | 关联订单号 | 
MemberID | MemberID | yes | string | 成员代码 | 
AccountID | AccountID | yes | string | 资金账号 | 
SettlementGroup | SettlementGroup | yes | string | 结算组编号 | 
OtherAccountID | OtherAccountID | no | string | 内部转账资金账号 | 
OtherSettlementGroup | OtherSettlementGroup | no | string | 内部转账结算组编号 | 
OtherMemberID | OtherMemberID | no | string | 内部转账成员代码 | 
Amount | Amount | yes | double | 发生额 | 
FrozenMoney | FrozenMoney | no | double | 冻结资金 | 
Currency | Currency | yes | string | 币种 | 
ActionType | ActionType | yes | string | 资金操作类型 | 1:入金;2:出金;3:冻结资金;4:释放冻结资金;5:释放冻结资金,同时出金;6:内部转账;7:设置保留资金限制;8:切换保留资金类型;9:保留资金激活失效;a:使用订单冻结资金;b:切换是否可以出金;
IsReserve | IsReserve | no | int | 是否保留资金 | 
AccountType | AccountType | no | string | 账户类型 | 
MarginRateGrade | MarginRateGrade | no | string | 保证金等级 | 
TradingRightGrade | TradingRightGrade | no | string | 权限等级 | 
Price | Price | no | double | 价格 | 
Volume | Volume | no | double | 数量 | 
ExchangeID | ExchangeID | no | string | 交易所代码 | 
InstrumentID | InstrumentID | no | string | 标的代码 | 
Source | Source | no | string | 财务流水类型 | 
RelatedID | RelatedID | no | string | 内外对账ID | 
FeeDeduceRate | FeeDeduceRate | no | double | 作为手续费的抵扣比例 | 
Remark | Remark | no | string | 备注 | 

+ **Response**

```
{
	"action": "RecvAccountAction",
	"errorCode": 0,
	"errorMsg": "Success",
	"result": [			
				{"table":"AccountAction",
				 "data":{
					,ActionLocalID:"1983723"
					,LocalID:"1983723"
					,OrderID:"12376422300333"
					,MemberID:"1983723"
					,AccountID:"1983723"
					,SettlementGroup:"999"
					,OtherAccountID:"1983723"
					,OtherSettlementGroup:"999"
					,OtherMemberID:"1983723"
					,Amount:182582.18
					,FrozenMoney:182582.18
					,Currency:"BTC"
					,ActionType:"1"
					,IsReserve:1
					,AccountType:""
					,MarginRateGrade:""
					,TradingRightGrade:""
					,Price:182582.18
					,Volume:182582.18
					,ExchangeID:"CEX"
					,InstrumentID:"BTCUSD"
					Source:""
					,RelatedID:"1983723"
					,FeeDeduceRate:182582.18
					,Remark:"my_little_tips"
					}
				}
				{"table":"Account",
				 "data":{
					,SettlementGroup:"999"
					,AccountID:"1983723"
					,Currency:"BTC"
					,MemberID:"1983723"
					,Available:182582.18
					,Withdrawable:182582.18
					,PreBalance:182582.18
					,Balance:182582.18
					,PositionCost:182582.18
					,UseMargin:182582.18
					,CrossMargin:182582.18
					,CloseProfit:10
					,Deposit:182582.18
					,Withdraw:182582.18
					,FrozenMargin:182582.18
					,FrozenMoney:182582.18
					,FrozenFee:182582.18
					,Fee:0.22364
					,MoneyChange:182582.18
					,Reserve:182582.18
					,ReserveAvailable:182582.18
					,ReserveLimit:182582.18
					,ReserveProfit:182582.18
					,ReserveType:"1"
					,ReserveActive:0
					,TotalCloseProfit:182582.18
					,TotalDeposit:182582.18
					,TotalWithdraw:182582.18
					,TotalFee:182582.18
					,TotalMoneyChange:182582.18
					,InsertTime:1657543832
					,UpdateTime:1657543832
					,AccountType:""
					,BusinessNo:11176422300333
					,MarginRateGrade:""
					,TradingRightGrade:""
					,Frequency:0
					,MaintMargin:182582.18
					,UnRealProfit:182582.18
					,CopyProfit:182582.18
					,CFDType:"1"
					,FeeGrade:""
					,CFDGrade:""
					,Borrow:182582.18
					,Remark:"my_little_tips"
					,MaxLocalID:"1983723"
					,CurrValue:182582.18
					,CanReduce:0
					}
				}
	]
}
```

+ **Response Parameters**

AccountAction:

parameter | shortName | type | description | remark
-------- | -------- | -------- | -------- | -------- 					
ActionLocalID | ActionLocalID | string | 关联的之前的资金操作本地号 | 
LocalID | LocalID | string | 出入金本地号 | 
OrderID | OrderID | string | 关联订单号 | 
MemberID | MemberID | string | 成员代码 | 
AccountID | AccountID | string | 资金账号 | 
SettlementGroup | SettlementGroup | string | 结算组编号 | 
OtherAccountID | OtherAccountID | string | 内部转账资金账号 | 
OtherSettlementGroup | OtherSettlementGroup | string | 内部转账结算组编号 | 
OtherMemberID | OtherMemberID | string | 内部转账成员代码 | 
Amount | Amount | double | 发生额 | 
FrozenMoney | FrozenMoney | double | 冻结资金 | 
Currency | Currency | string | 币种 | 
ActionType | ActionType | string | 资金操作类型 | 1:入金;2:出金;3:冻结资金;4:释放冻结资金;5:释放冻结资金,同时出金;6:内部转账;7:设置保留资金限制;8:切换保留资金类型;9:保留资金激活失效;a:使用订单冻结资金;b:切换是否可以出金;
IsReserve | IsReserve | int | 是否保留资金 | 
AccountType | AccountType | string | 账户类型 | 
MarginRateGrade | MarginRateGrade | string | 保证金等级 | 
TradingRightGrade | TradingRightGrade | string | 权限等级 | 
Price | Price | double | 价格 | 
Volume | Volume | double | 数量 | 
ExchangeID | ExchangeID | string | 交易所代码 | 
InstrumentID | InstrumentID | string | 标的代码 | 
Source | Source | string | 财务流水类型 | 
RelatedID | RelatedID | string | 内外对账ID | 
FeeDeduceRate | FeeDeduceRate | double | 作为手续费的抵扣比例 | 
Remark | Remark | string | 备注 | 

Account:

parameter | shortName | type | description | remark
-------- | -------- | -------- | -------- | -------- 					
SettlementGroup | SettlementGroup | string | 结算组编号 | 
AccountID | AccountID | string | 资金账号 | 
Currency | Currency | string | 币种 | 
MemberID | MemberID | string | 成员代码 | 
Available | Available | double | 可用资金 | 
Withdrawable | Withdrawable | double | 可取资金 | 
PreBalance | PreBalance | double | 上次静态权益 | 
Balance | Balance | double | 静态权益 | 
PositionCost | PositionCost | double | 持仓成本 | 
UseMargin | UseMargin | double | 所有占用保证金 | 
CrossMargin | CrossMargin | double | 全仓保证金 | 
CloseProfit | CloseProfit | double | 平仓盈亏 | 
Deposit | Deposit | double | 入金金额 | 
Withdraw | Withdraw | double | 出金金额 | 
FrozenMargin | FrozenMargin | double | 冻结保证金 | 
FrozenMoney | FrozenMoney | double | 冻结资金 | 
FrozenFee | FrozenFee | double | 冻结手续费 | 
Fee | Fee | double | 手续费 | 
MoneyChange | MoneyChange | double | 资金变化 | 
Reserve | Reserve | double | 保留资金 | 
ReserveAvailable | ReserveAvailable | double | 保留资金可用 | 
ReserveLimit | ReserveLimit | double | 保留资金限制 | 
ReserveProfit | ReserveProfit | double | 保留资金盈亏 | 
ReserveType | ReserveType | string | 保留资金类型 | 0:先亏损保留资金再亏损真钱(劣后);1:先亏损真钱再亏损保留资金(优先);2:保留资金不做亏损(抵押金);
ReserveActive | ReserveActive | int | 保留资金是否活跃 | 
TotalCloseProfit | TotalCloseProfit | double | 总平仓盈亏 | 
TotalDeposit | TotalDeposit | double | 总入金金额 | 
TotalWithdraw | TotalWithdraw | double | 总出金金额 | 
TotalFee | TotalFee | double | 总手续费 | 
TotalMoneyChange | TotalMoneyChange | double | 总资金变化 | 
InsertTime | InsertTime | double | 插入时间 | 
UpdateTime | UpdateTime | double | 更新时间 | 
AccountType | AccountType | string | 账户类型 | 
BusinessNo | BusinessNo | double | 业务序列号 | 
MarginRateGrade | MarginRateGrade | string | 保证金等级 | 
TradingRightGrade | TradingRightGrade | string | 权限等级 | 
Frequency | Frequency | int | 每秒更新的次数 | 
MaintMargin | MaintMargin | double | 维持保证金 | 
UnRealProfit | UnRealProfit | double | 未实现盈亏 | 
CopyProfit | CopyProfit | double | 带单盈利分配 | 
CFDType | CFDType | string | 点差接管类型 | 0:先撮合后点差接管;1:只撮合不点差接管;2:不撮合只点差接管;
FeeGrade | FeeGrade | string | 手续费等级 | 
CFDGrade | CFDGrade | string | 差价等级 | 
Borrow | Borrow | double | 借款周期结算量 | 
Remark | Remark | string | 备注 | 
MaxLocalID | MaxLocalID | string | 最大本地标识 | 
CurrValue | CurrValue | double | 当前价值 | 
CanReduce | CanReduce | int | 是否可以出金 | 


## **成员设置请求**

+ **Request URL**

`POST /action/v1.0/SendMemberAction`


+ **Request Post Body**

```
{
	,"LocalID":"1983723"
	,"MemberID":"1983723"
	,"RiskLevel":"1"
	,"PositionType":"1"
}
```
+ **curl Example**

````
curl -d '{"LocalID":"1983723","MemberID":"1983723","RiskLevel":"1","PositionType":"1"}'  -X POST  http://127.0.0.1:10000/action/v1.0/SendMemberAction
````
+ **ws Example**

````
{"RequestNo":2,"SendMemberAction":{"LocalID":"1983723","MemberID":"1983723","RiskLevel":"1","PositionType":"1"}}
````
+ **Web rowser Example**

````
http://127.0.0.1:10000/action/v1.0/SendMemberAction?LocalID=1983723&MemberID=1983723&RiskLevel=1&PositionType=1
````
+ **Request Parameters**

parameter | shortName | required | type | description | remark
-------- | -------- | -------- | -------- | -------- | --------
LocalID | LocalID | no | string | 出入金本地号 | 
MemberID | MemberID | yes | string | 成员代码 | 
RiskLevel | RiskLevel | no | string | 风控类型 | 0:本币种资金权益;1:结算组所有资金权益;2:资金账号所有资金权益;3:会员账号所有资金权益;
PositionType | PositionType | no | string | 持仓类型 | 0:不设置;1:综合持仓(双边持仓);2:净持仓(单边持仓);

+ **Response**

```
{
	"action": "RecvMemberAction",
	"errorCode": 0,
	"errorMsg": "Success",
	"result": [			
				{"table":"MemberAction",
				 "data":{
					,LocalID:"1983723"
					,MemberID:"1983723"
					,RiskLevel:"1"
					,PositionType:"1"
					}
				}
				{"table":"Member",
				 "data":{
					,MemberID:"1983723"
					,MemberType:"1"
					,Grade:""
					,Priority:0
					,FeeGrade:""
					,MarginRateGrade:""
					,TradingRightGrade:""
					,CFDGrade:""
					,LiquidityGrade:""
					,RiskLevel:"1"
					,PositionType:"1"
					,TriggerOrders:0
					,OpenOrders:0
					,OpenPositions:0
					,MaxLocalID:"1983723"
					,SinkType:"1"
					,CopyMemberID:"1983723"
					,Region:""
					,FeeDeduceDisCount:182582.18
					,FeeDeduceCurrency:"BTC"
					,CanBorrow:0
					,Remark:"my_little_tips"
					,InsertTime:1657543832
					,UpdateTime:1657543832
					}
				}
	]
}
```

+ **Response Parameters**

MemberAction:

parameter | shortName | type | description | remark
-------- | -------- | -------- | -------- | -------- 					
LocalID | LocalID | string | 出入金本地号 | 
MemberID | MemberID | string | 成员代码 | 
RiskLevel | RiskLevel | string | 风控类型 | 0:本币种资金权益;1:结算组所有资金权益;2:资金账号所有资金权益;3:会员账号所有资金权益;
PositionType | PositionType | string | 持仓类型 | 0:不设置;1:综合持仓(双边持仓);2:净持仓(单边持仓);

Member:

parameter | shortName | type | description | remark
-------- | -------- | -------- | -------- | -------- 					
MemberID | MemberID | string | 成员代码 | 
MemberType | MemberType | string | 成员类型 | a:普通;b:内部做市商;c:系统手续费使用;d:风控使用;e:运营使用;f:运营保留资金使用;g:外部做市商;
Grade | Grade | string | 等级 | 
Priority | Priority | int | 优先权 | 
FeeGrade | FeeGrade | string | 手续费等级 | 
MarginRateGrade | MarginRateGrade | string | 保证金等级 | 
TradingRightGrade | TradingRightGrade | string | 权限等级 | 
CFDGrade | CFDGrade | string | 差价等级 | 
LiquidityGrade | LiquidityGrade | string | 流动性等级 | 
RiskLevel | RiskLevel | string | 风控类型 | 0:本币种资金权益;1:结算组所有资金权益;2:资金账号所有资金权益;3:会员账号所有资金权益;
PositionType | PositionType | string | 持仓类型 | 0:不设置;1:综合持仓(双边持仓);2:净持仓(单边持仓);
TriggerOrders | TriggerOrders | int | 触发订单个数 | 
OpenOrders | OpenOrders | int | 未成交订单数 | 
OpenPositions | OpenPositions | int | 用户持仓个数 | 
MaxLocalID | MaxLocalID | string | 最大本地标识 | 
SinkType | SinkType | string | 下游系统同步类型 | 0:同步到数据库;1:同步到Redis不到数据库;2:不同步;
CopyMemberID | CopyMemberID | string | 带单员代码 | 
Region | Region | string |  | 
FeeDeduceDisCount | FeeDeduceDisCount | double | 作为手续费的抵扣优惠比例 | 
FeeDeduceCurrency | FeeDeduceCurrency | string | 作为手续费的抵扣比例 | 
CanBorrow | CanBorrow | int | 是否需要借钱交易 | 
Remark | Remark | string | 备注 | 
InsertTime | InsertTime | double | 插入时间 | 
UpdateTime | UpdateTime | double | 更新时间 | 


## **内部做市商申报请求**

+ **Request URL**

`POST /action/v1.0/SendRobotOrder`


+ **Request Post Body**

```
{
	,"MemberID":"1983723"
	,"TradeUnitID":"1983723"
	,"AccountID":"1983723"
	,"UserID":"1983723"
	,"LocalID":"1983723"
	,"ExchangeID":"CEX"
	,"InstrumentID":"BTCUSD"
	,"OrderPriceType":"1"
	,"Direction":"1"
	,"OffsetFlag":"1"
	,"Price":182582.18
	,"Volume":182582.18
	,"VolumeDisplay":182582.18
}
```
+ **curl Example**

````
curl -d '{"MemberID":"1983723","TradeUnitID":"1983723","AccountID":"1983723","UserID":"1983723","LocalID":"1983723","ExchangeID":"CEX","InstrumentID":"BTCUSD","OrderPriceType":"1","Direction":"1","OffsetFlag":"1","Price":"182582.18,"Volume":"182582.18,"VolumeDisplay":"182582.18}'  -X POST  http://127.0.0.1:10000/action/v1.0/SendRobotOrder
````
+ **ws Example**

````
{"RequestNo":2,"SendRobotOrder":{"MemberID":"1983723","TradeUnitID":"1983723","AccountID":"1983723","UserID":"1983723","LocalID":"1983723","ExchangeID":"CEX","InstrumentID":"BTCUSD","OrderPriceType":"1","Direction":"1","OffsetFlag":"1","Price":"182582.18,"Volume":"182582.18,"VolumeDisplay":"182582.18}}
````
+ **Web rowser Example**

````
http://127.0.0.1:10000/action/v1.0/SendRobotOrder?MemberID=1983723&TradeUnitID=1983723&AccountID=1983723&UserID=1983723&LocalID=1983723&ExchangeID=CEX&InstrumentID=BTCUSD&OrderPriceType=1&Direction=1&OffsetFlag=1&Price=182582.18&Volume=182582.18&VolumeDisplay=182582.18
````
+ **Request Parameters**

parameter | shortName | required | type | description | remark
-------- | -------- | -------- | -------- | -------- | --------
MemberID | M | no | string | 成员代码 | 
TradeUnitID | T | no | string | 交易单元代码 | 
AccountID | A | no | string | 资金账号 | 
UserID | U | yes | string | 交易用户代码 | 
LocalID | L | no | string | 报单本地标识 | 
ExchangeID | E | yes | string | 交易所代码 | 
InstrumentID | I | yes | string | 标的代码 | 
OrderPriceType | PT | yes | string | 报单价格类型 | 0:限价(手动指定Price值);1:任意价(Price按最高最低限价取值);2:最优价/对手价(Price按对方1档价取值);3:五档价(Price按对方5档价取值);4:十档价(类同上);7:最优Marker价(Price按本方1档价或其+1Tick取值);8:Price按委托额Cost和订单簿情况确定值;9:保底点差价:成交价格Price不会劣于系统设置差价成交;
Direction | D | yes | string | 买卖方向 | 0:买;1:卖;
OffsetFlag | O | no | string | 开平标志 | 0:开仓;1:平仓;2:强平;3:平今;4:平昨;5:全平;6:指定订单平仓;7:指定成交平仓;8:最大平仓;
Price | P | no | double | 报单价格 | 
Volume | V | no | double | 数量 | 
VolumeDisplay | D | no | double | 显示数量 | 

+ **Response**

```
{
	"action": "RecvRobotOrder",
	"errorCode": 0,
	"errorMsg": "Success",
	"result": [			
				{"table":"RobotOrder",
				 "data":{
					,MemberID:"1983723"
					,TradeUnitID:"1983723"
					,AccountID:"1983723"
					,UserID:"1983723"
					,LocalID:"1983723"
					,ExchangeID:"CEX"
					,InstrumentID:"BTCUSD"
					,OrderPriceType:"1"
					,Direction:"1"
					,OffsetFlag:"1"
					,Price:182582.18
					,Volume:182582.18
					,VolumeDisplay:182582.18
					}
				}
	]
}
```

+ **Response Parameters**

RobotOrder:

parameter | shortName | type | description | remark
-------- | -------- | -------- | -------- | -------- 					
MemberID | M | string | 成员代码 | 
TradeUnitID | T | string | 交易单元代码 | 
AccountID | A | string | 资金账号 | 
UserID | U | string | 交易用户代码 | 
LocalID | L | string | 报单本地标识 | 
ExchangeID | E | string | 交易所代码 | 
InstrumentID | I | string | 标的代码 | 
OrderPriceType | PT | string | 报单价格类型 | 0:限价(手动指定Price值);1:任意价(Price按最高最低限价取值);2:最优价/对手价(Price按对方1档价取值);3:五档价(Price按对方5档价取值);4:十档价(类同上);7:最优Marker价(Price按本方1档价或其+1Tick取值);8:Price按委托额Cost和订单簿情况确定值;9:保底点差价:成交价格Price不会劣于系统设置差价成交;
Direction | D | string | 买卖方向 | 0:买;1:卖;
OffsetFlag | O | string | 开平标志 | 0:开仓;1:平仓;2:强平;3:平今;4:平昨;5:全平;6:指定订单平仓;7:指定成交平仓;8:最大平仓;
Price | P | double | 报单价格 | 
Volume | V | double | 数量 | 
VolumeDisplay | D | double | 显示数量 | 


## **场外成交申报请求**

+ **Request URL**

`POST /action/v1.0/SendOTCTradeInsert`


+ **Request Post Body**

```
{
	,"LocalID":"1983723"
	,"ExchangeID":"CEX"
	,"InstrumentID":"BTCUSD"
	,"Price":182582.18
	,"Volume":182582.18
	,"BuyMemberID":"1983723"
	,"BuyAccountID":"1983723"
	,"BuyTradeUnitID":"1983723"
	,"BuyOffsetFlag":"1"
	,"SellMemberID":"1983723"
	,"SellAccountID":"1983723"
	,"SellTradeUnitID":"1983723"
	,"SellOffsetFlag":"1"
	,"OTCType":"1"
	,"TakerDirection":"1"
	,"TriggerPrice":182582.18
	,"Remark":"my_little_tips"
}
```
+ **curl Example**

````
curl -d '{"LocalID":"1983723","ExchangeID":"CEX","InstrumentID":"BTCUSD","Price":"182582.18,"Volume":"182582.18,"BuyMemberID":"1983723","BuyAccountID":"1983723","BuyTradeUnitID":"1983723","BuyOffsetFlag":"1","SellMemberID":"1983723","SellAccountID":"1983723","SellTradeUnitID":"1983723","SellOffsetFlag":"1","OTCType":"1","TakerDirection":"1","TriggerPrice":"182582.18,"Remark":"my_little_tips"}'  -X POST  http://127.0.0.1:10000/action/v1.0/SendOTCTradeInsert
````
+ **ws Example**

````
{"RequestNo":2,"SendOTCTradeInsert":{"LocalID":"1983723","ExchangeID":"CEX","InstrumentID":"BTCUSD","Price":"182582.18,"Volume":"182582.18,"BuyMemberID":"1983723","BuyAccountID":"1983723","BuyTradeUnitID":"1983723","BuyOffsetFlag":"1","SellMemberID":"1983723","SellAccountID":"1983723","SellTradeUnitID":"1983723","SellOffsetFlag":"1","OTCType":"1","TakerDirection":"1","TriggerPrice":"182582.18,"Remark":"my_little_tips"}}
````
+ **Web rowser Example**

````
http://127.0.0.1:10000/action/v1.0/SendOTCTradeInsert?LocalID=1983723&ExchangeID=CEX&InstrumentID=BTCUSD&Price=182582.18&Volume=182582.18&BuyMemberID=1983723&BuyAccountID=1983723&BuyTradeUnitID=1983723&BuyOffsetFlag=1&SellMemberID=1983723&SellAccountID=1983723&SellTradeUnitID=1983723&SellOffsetFlag=1&OTCType=1&TakerDirection=1&TriggerPrice=182582.18&Remark=my_little_tips
````
+ **Request Parameters**

parameter | shortName | required | type | description | remark
-------- | -------- | -------- | -------- | -------- | --------
LocalID | LocalID | no | string | 成交对本地标识 | 
ExchangeID | ExchangeID | yes | string | 交易所代码 | 
InstrumentID | InstrumentID | yes | string | 标的代码 | 
Price | Price | yes | double | 价格 | 
Volume | Volume | yes | double | 数量 | 
BuyMemberID | BuyMemberID | yes | string | 买方成员代码 | 
BuyAccountID | BuyAccountID | no | string | 买方交易单元代码 | 
BuyTradeUnitID | BuyTradeUnitID | yes | string | 买方交易单元代码 | 
BuyOffsetFlag | BuyOffsetFlag | yes | string | 买方组合开平标志 | 0:开仓;1:平仓;2:强平;3:平今;4:平昨;5:全平;6:指定订单平仓;7:指定成交平仓;8:最大平仓;
SellMemberID | SellMemberID | yes | string | 卖方方成员代码 | 
SellAccountID | SellAccountID | no | string | 卖方交易单元代码 | 
SellTradeUnitID | SellTradeUnitID | yes | string | 卖方交易单元代码 | 
SellOffsetFlag | SellOffsetFlag | yes | string | 卖方组合开平标志 | 0:开仓;1:平仓;2:强平;3:平今;4:平昨;5:全平;6:指定订单平仓;7:指定成交平仓;8:最大平仓;
OTCType | OTCType | yes | string | OTC类型 | 0:强平;1:大宗交易;2:期转现;3:只刷单;4:仓位合并;5:差价交易;6:ADL减仓;7:回购手续费不一样;
TakerDirection | TakerDirection | no | string | Taker的方向 | 0:买;1:卖;
TriggerPrice | TriggerPrice | no | double | 触发价 | 
Remark | Remark | no | string | 备注 | 

+ **Response**

```
{
	"action": "RecvOTCTradeInsert",
	"errorCode": 0,
	"errorMsg": "Success",
	"result": [			
				{"table":"OTCTradeInsert",
				 "data":{
					,LocalID:"1983723"
					,ExchangeID:"CEX"
					,InstrumentID:"BTCUSD"
					,Price:182582.18
					,Volume:182582.18
					,BuyMemberID:"1983723"
					,BuyAccountID:"1983723"
					,BuyTradeUnitID:"1983723"
					,BuyOffsetFlag:"1"
					,SellMemberID:"1983723"
					,SellAccountID:"1983723"
					,SellTradeUnitID:"1983723"
					,SellOffsetFlag:"1"
					,OTCType:"1"
					,TakerDirection:"1"
					,TriggerPrice:182582.18
					,Remark:"my_little_tips"
					}
				}
				{"table":"OTCTrade",
				 "data":{
					,LocalID:"1983723"
					,ExchangeID:"CEX"
					,InstrumentID:"BTCUSD"
					,Price:182582.18
					,Volume:182582.18
					,BuyMemberID:"1983723"
					,BuyAccountID:"1983723"
					,BuyTradeUnitID:"1983723"
					,BuyOffsetFlag:"1"
					,SellMemberID:"1983723"
					,SellAccountID:"1983723"
					,SellTradeUnitID:"1983723"
					,SellOffsetFlag:"1"
					,OTCType:"1"
					,TakerDirection:"1"
					,TriggerPrice:182582.18
					,Remark:"my_little_tips"
					,TradeID:"16576422300333"
					,TradeTime:1657543832
					,InsertTime:1657543832
					,DeriveSource:"1"
					,BuyOrderID:"1983723"
					,SellOrderID:"1983723"
					}
				}
	]
}
```

+ **Response Parameters**

OTCTradeInsert:

parameter | shortName | type | description | remark
-------- | -------- | -------- | -------- | -------- 					
LocalID | LocalID | string | 成交对本地标识 | 
ExchangeID | ExchangeID | string | 交易所代码 | 
InstrumentID | InstrumentID | string | 标的代码 | 
Price | Price | double | 价格 | 
Volume | Volume | double | 数量 | 
BuyMemberID | BuyMemberID | string | 买方成员代码 | 
BuyAccountID | BuyAccountID | string | 买方交易单元代码 | 
BuyTradeUnitID | BuyTradeUnitID | string | 买方交易单元代码 | 
BuyOffsetFlag | BuyOffsetFlag | string | 买方组合开平标志 | 0:开仓;1:平仓;2:强平;3:平今;4:平昨;5:全平;6:指定订单平仓;7:指定成交平仓;8:最大平仓;
SellMemberID | SellMemberID | string | 卖方方成员代码 | 
SellAccountID | SellAccountID | string | 卖方交易单元代码 | 
SellTradeUnitID | SellTradeUnitID | string | 卖方交易单元代码 | 
SellOffsetFlag | SellOffsetFlag | string | 卖方组合开平标志 | 0:开仓;1:平仓;2:强平;3:平今;4:平昨;5:全平;6:指定订单平仓;7:指定成交平仓;8:最大平仓;
OTCType | OTCType | string | OTC类型 | 0:强平;1:大宗交易;2:期转现;3:只刷单;4:仓位合并;5:差价交易;6:ADL减仓;7:回购手续费不一样;
TakerDirection | TakerDirection | string | Taker的方向 | 0:买;1:卖;
TriggerPrice | TriggerPrice | double | 触发价 | 
Remark | Remark | string | 备注 | 

OTCTrade:

parameter | shortName | type | description | remark
-------- | -------- | -------- | -------- | -------- 					
LocalID | LocalID | string | 成交对本地标识 | 
ExchangeID | ExchangeID | string | 交易所代码 | 
InstrumentID | InstrumentID | string | 标的代码 | 
Price | Price | double | 价格 | 
Volume | Volume | double | 数量 | 
BuyMemberID | BuyMemberID | string | 买方成员代码 | 
BuyAccountID | BuyAccountID | string | 买方交易单元代码 | 
BuyTradeUnitID | BuyTradeUnitID | string | 买方交易单元代码 | 
BuyOffsetFlag | BuyOffsetFlag | string | 买方组合开平标志 | 0:开仓;1:平仓;2:强平;3:平今;4:平昨;5:全平;6:指定订单平仓;7:指定成交平仓;8:最大平仓;
SellMemberID | SellMemberID | string | 卖方方成员代码 | 
SellAccountID | SellAccountID | string | 卖方交易单元代码 | 
SellTradeUnitID | SellTradeUnitID | string | 卖方交易单元代码 | 
SellOffsetFlag | SellOffsetFlag | string | 卖方组合开平标志 | 0:开仓;1:平仓;2:强平;3:平今;4:平昨;5:全平;6:指定订单平仓;7:指定成交平仓;8:最大平仓;
OTCType | OTCType | string | OTC类型 | 0:强平;1:大宗交易;2:期转现;3:只刷单;4:仓位合并;5:差价交易;6:ADL减仓;7:回购手续费不一样;
TakerDirection | TakerDirection | string | Taker的方向 | 0:买;1:卖;
TriggerPrice | TriggerPrice | double | 触发价 | 
Remark | Remark | string | 备注 | 
TradeID | TradeID | string | 成交代码 | 
TradeTime | TradeTime | double | 成交时间 | 
InsertTime | InsertTime | double | 插入时间 | 
DeriveSource | DeriveSource | string | 成交类型 | 0:正常;1:报价衍生;2:期权执行;3:组合衍生;4:场外成交强平衍生;5:场外成交大宗交易衍生;6:场外成交期转现衍生;7:场外成交只刷单衍生;8:触发订单衍生;9:止损触发订单衍生;a:止盈触发订单衍生;b:资金操作衍生;c:仓位合并衍生;d:CFD订单衍生;e:场外成交ADL衍生;f:回购手续费不能是币;
BuyOrderID | BuyOrderID | string | 买方报单系统唯一代码 | 
SellOrderID | SellOrderID | string | 卖方报单系统唯一代码 | 


## **资金结算请求**

+ **Request URL**

`POST /action/v1.0/SendSettlement`


+ **Request Post Body**

```
{
	,"APPID":"ios"
	,"LocalID":"1983723"
	,"ExchangeID":"CEX"
	,"InstrumentID":"BTCUSD"
	,"SettlementGroup":"999"
	,"SettleAction":"1"
	,"Value":182582.18
	,"Value1":182582.18
	,"Value2":182582.18
	,"Value3":182582.18
	,"Remark":"my_little_tips"
}
```
+ **curl Example**

````
curl -d '{"APPID":"ios","LocalID":"1983723","ExchangeID":"CEX","InstrumentID":"BTCUSD","SettlementGroup":"999","SettleAction":"1","Value":"182582.18,"Value1":"182582.18,"Value2":"182582.18,"Value3":"182582.18,"Remark":"my_little_tips"}'  -X POST  http://127.0.0.1:10000/action/v1.0/SendSettlement
````
+ **ws Example**

````
{"RequestNo":2,"SendSettlement":{"APPID":"ios","LocalID":"1983723","ExchangeID":"CEX","InstrumentID":"BTCUSD","SettlementGroup":"999","SettleAction":"1","Value":"182582.18,"Value1":"182582.18,"Value2":"182582.18,"Value3":"182582.18,"Remark":"my_little_tips"}}
````
+ **Web rowser Example**

````
http://127.0.0.1:10000/action/v1.0/SendSettlement?APPID=ios&LocalID=1983723&ExchangeID=CEX&InstrumentID=BTCUSD&SettlementGroup=999&SettleAction=1&Value=182582.18&Value1=182582.18&Value2=182582.18&Value3=182582.18&Remark=my_little_tips
````
+ **Request Parameters**

parameter | shortName | required | type | description | remark
-------- | -------- | -------- | -------- | -------- | --------
APPID | APPID | no | string | 应用编号 | 
LocalID | LocalID | no | string | 成交对本地标识 | 
ExchangeID | ExchangeID | yes | string | 交易所代码 | 
InstrumentID | InstrumentID | yes | string | 标的代码 | 
SettlementGroup | SettlementGroup | no | string | 结算组编号 | 
SettleAction | SettleAction | yes | string | 结算操作 | 0:结算操作开始;1:定期无负债结算;2:到期交割(行权);3:资金费率交换;4:隔夜费划转;5:分摊;6:资金初始化;7:结算操作结束;8:数据检查;9:数据修复;a:平账;b:清理;c:清空没有持仓的产品;d:借贷利息;
Value | Value | no | double | 操作使用值 | 
Value1 | Value1 | no | double | 发生值1 | 
Value2 | Value2 | no | double | 发生值2 | 
Value3 | Value3 | no | double | 发生值3 | 
Remark | Remark | no | string | 备注 | 

+ **Response**

```
{
	"action": "RecvSettlement",
	"errorCode": 0,
	"errorMsg": "Success",
	"result": [			
				{"table":"Settlement",
				 "data":{
					,APPID:"ios"
					,LocalID:"1983723"
					,ExchangeID:"CEX"
					,InstrumentID:"BTCUSD"
					,SettlementGroup:"999"
					,SettleAction:"1"
					,Value:182582.18
					,Value1:182582.18
					,Value2:182582.18
					,Value3:182582.18
					,Remark:"my_little_tips"
					}
				}
				{"table":"SettleDetail",
				 "data":{
					,SettleDetailID:"1983723"
					,APPID:"ios"
					,LocalID:"1983723"
					,ExchangeID:"CEX"
					,InstrumentID:"BTCUSD"
					,SettlementGroup:"999"
					,SettleAction:"1"
					,Value:182582.18
					,Value1:182582.18
					,Value2:182582.18
					,Value3:182582.18
					,Remark:"my_little_tips"
					,FundingRateGroup:"999"
					,SettleSegment:""
					,InsertTime:""
					,BusinessNo:11176422300333
					}
				}
	]
}
```

+ **Response Parameters**

Settlement:

parameter | shortName | type | description | remark
-------- | -------- | -------- | -------- | -------- 					
APPID | APPID | string | 应用编号 | 
LocalID | LocalID | string | 成交对本地标识 | 
ExchangeID | ExchangeID | string | 交易所代码 | 
InstrumentID | InstrumentID | string | 标的代码 | 
SettlementGroup | SettlementGroup | string | 结算组编号 | 
SettleAction | SettleAction | string | 结算操作 | 0:结算操作开始;1:定期无负债结算;2:到期交割(行权);3:资金费率交换;4:隔夜费划转;5:分摊;6:资金初始化;7:结算操作结束;8:数据检查;9:数据修复;a:平账;b:清理;c:清空没有持仓的产品;d:借贷利息;
Value | Value | double | 操作使用值 | 
Value1 | Value1 | double | 发生值1 | 
Value2 | Value2 | double | 发生值2 | 
Value3 | Value3 | double | 发生值3 | 
Remark | Remark | string | 备注 | 

SettleDetail:

parameter | shortName | type | description | remark
-------- | -------- | -------- | -------- | -------- 					
SettleDetailID | SettleDetailID | string | 操作明细号 | 
APPID | APPID | string | 应用编号 | 
LocalID | LocalID | string | 成交对本地标识 | 
ExchangeID | ExchangeID | string | 交易所代码 | 
InstrumentID | InstrumentID | string | 标的代码 | 
SettlementGroup | SettlementGroup | string | 结算组编号 | 
SettleAction | SettleAction | string | 结算操作 | 0:结算操作开始;1:定期无负债结算;2:到期交割(行权);3:资金费率交换;4:隔夜费划转;5:分摊;6:资金初始化;7:结算操作结束;8:数据检查;9:数据修复;a:平账;b:清理;c:清空没有持仓的产品;d:借贷利息;
Value | Value | double | 操作使用值 | 
Value1 | Value1 | double | 发生值1 | 
Value2 | Value2 | double | 发生值2 | 
Value3 | Value3 | double | 发生值3 | 
Remark | Remark | string | 备注 | 
FundingRateGroup | FundingRateGroup | string | 资金费用组 | 
SettleSegment | SettleSegment | string | 结算段 | 
InsertTime | InsertTime | string | 创建时间 | 
BusinessNo | BusinessNo | double | 业务序列号 | 


## **清理行情数据请求**

+ **Request URL**

`POST /action/v1.0/SendClearMarket`


+ **Request Post Body**

```
{
	,"ExchangeID":"CEX"
	,"InstrumentID":"BTCUSD"
}
```
+ **curl Example**

````
curl -d '{"ExchangeID":"CEX","InstrumentID":"BTCUSD"}'  -X POST  http://127.0.0.1:10000/action/v1.0/SendClearMarket
````
+ **ws Example**

````
{"RequestNo":2,"SendClearMarket":{"ExchangeID":"CEX","InstrumentID":"BTCUSD"}}
````
+ **Web rowser Example**

````
http://127.0.0.1:10000/action/v1.0/SendClearMarket?ExchangeID=CEX&InstrumentID=BTCUSD
````
+ **Request Parameters**

parameter | shortName | required | type | description | remark
-------- | -------- | -------- | -------- | -------- | --------
ExchangeID | ExchangeID | yes | string | 交易所代码 | 
InstrumentID | InstrumentID | yes | string | 标的代码 | 

+ **Response**

```
{
	"action": "RecvClearMarket",
	"errorCode": 0,
	"errorMsg": "Success",
	"result": [			
				{"table":"ClearMarket",
				 "data":{
					,ExchangeID:"CEX"
					,InstrumentID:"BTCUSD"
					}
				}
	]
}
```

+ **Response Parameters**

ClearMarket:

parameter | shortName | type | description | remark
-------- | -------- | -------- | -------- | -------- 					
ExchangeID | ExchangeID | string | 交易所代码 | 
InstrumentID | InstrumentID | string | 标的代码 | 


## **清理最新K线请求**

+ **Request URL**

`POST /action/v1.0/SendClearLastKLine`


+ **Request Post Body**

```
{
	,"ExchangeID":"CEX"
	,"InstrumentID":"BTCUSD"
	,"Bar":""
}
```
+ **curl Example**

````
curl -d '{"ExchangeID":"CEX","InstrumentID":"BTCUSD","Bar":""}'  -X POST  http://127.0.0.1:10000/action/v1.0/SendClearLastKLine
````
+ **ws Example**

````
{"RequestNo":2,"SendClearLastKLine":{"ExchangeID":"CEX","InstrumentID":"BTCUSD","Bar":""}}
````
+ **Web rowser Example**

````
http://127.0.0.1:10000/action/v1.0/SendClearLastKLine?ExchangeID=CEX&InstrumentID=BTCUSD&Bar=
````
+ **Request Parameters**

parameter | shortName | required | type | description | remark
-------- | -------- | -------- | -------- | -------- | --------
ExchangeID | ExchangeID | yes | string | 交易所代码 | 
InstrumentID | InstrumentID | yes | string | 标的代码 | 
Bar | Bar | no | string | K线周期代码(n[m/h/d/o]) | 

+ **Response**

```
{
	"action": "RecvClearLastKLine",
	"errorCode": 0,
	"errorMsg": "Success",
	"result": [			
				{"table":"ClearLastKLine",
				 "data":{
					,ExchangeID:"CEX"
					,InstrumentID:"BTCUSD"
					,Bar:""
					}
				}
	]
}
```

+ **Response Parameters**

ClearLastKLine:

parameter | shortName | type | description | remark
-------- | -------- | -------- | -------- | -------- 					
ExchangeID | ExchangeID | string | 交易所代码 | 
InstrumentID | InstrumentID | string | 标的代码 | 
Bar | Bar | string | K线周期代码(n[m/h/d/o]) | 


## **重置LocalID请求**

+ **Request URL**

`POST /action/v1.0/SendResetLocalID`


+ **Request Post Body**

```
{
	,"Token":""
	,"UserID":"1983723"
	,"APPID":"ios"
	,"MemberID":"1983723"
	,"SettlementGroup":"999"
	,"AccountID":"1983723"
	,"Currency":"BTC"
}
```
+ **curl Example**

````
curl -d '{"Token":"","UserID":"1983723","APPID":"ios","MemberID":"1983723","SettlementGroup":"999","AccountID":"1983723","Currency":"BTC"}'  -X POST  http://127.0.0.1:10000/action/v1.0/SendResetLocalID
````
+ **ws Example**

````
{"RequestNo":2,"SendResetLocalID":{"Token":"","UserID":"1983723","APPID":"ios","MemberID":"1983723","SettlementGroup":"999","AccountID":"1983723","Currency":"BTC"}}
````
+ **Web rowser Example**

````
http://127.0.0.1:10000/action/v1.0/SendResetLocalID?Token=&UserID=1983723&APPID=ios&MemberID=1983723&SettlementGroup=999&AccountID=1983723&Currency=BTC
````
+ **Request Parameters**

parameter | shortName | required | type | description | remark
-------- | -------- | -------- | -------- | -------- | --------
Token | Token | no | string | 令牌 | 
UserID | UserID | no | string | 交易用户代码 | 
APPID | APPID | no | string | 应用编号 | 
MemberID | MemberID | no | string | 成员代码 | 
SettlementGroup | SettlementGroup | no | string | 结算组编号 | 
AccountID | AccountID | no | string | 资金账号 | 
Currency | Currency | no | string | 币种 | 

+ **Response**

```
{
	"action": "RecvResetLocalID",
	"errorCode": 0,
	"errorMsg": "Success",
	"result": [			
				{"table":"ResetLocalID",
				 "data":{
					,Token:""
					,UserID:"1983723"
					,APPID:"ios"
					,MemberID:"1983723"
					,SettlementGroup:"999"
					,AccountID:"1983723"
					,Currency:"BTC"
					}
				}
	]
}
```

+ **Response Parameters**

ResetLocalID:

parameter | shortName | type | description | remark
-------- | -------- | -------- | -------- | -------- 					
Token | Token | string | 令牌 | 
UserID | UserID | string | 交易用户代码 | 
APPID | APPID | string | 应用编号 | 
MemberID | MemberID | string | 成员代码 | 
SettlementGroup | SettlementGroup | string | 结算组编号 | 
AccountID | AccountID | string | 资金账号 | 
Currency | Currency | string | 币种 | 


## **OToken录入请求**

+ **Request URL**

`POST /action/v1.0/SendOTokenInsert`


+ **Request Post Body**

```
{
	,"UserID":"1983723"
	,"APPID":"ios"
	,"Token":""
	,"MemberID":"1983723"
	,"LoginTime":1657543832
	,"ExpireTime":1657543832
	,"AccessLimit":0
}
```
+ **curl Example**

````
curl -d '{"UserID":"1983723","APPID":"ios","Token":"","MemberID":"1983723","LoginTime":"1657543832,"ExpireTime":"1657543832,"AccessLimit":"0}'  -X POST  http://127.0.0.1:10000/action/v1.0/SendOTokenInsert
````
+ **ws Example**

````
{"RequestNo":2,"SendOTokenInsert":{"UserID":"1983723","APPID":"ios","Token":"","MemberID":"1983723","LoginTime":"1657543832,"ExpireTime":"1657543832,"AccessLimit":"0}}
````
+ **Web rowser Example**

````
http://127.0.0.1:10000/action/v1.0/SendOTokenInsert?UserID=1983723&APPID=ios&Token=&MemberID=1983723&LoginTime=1657543832&ExpireTime=1657543832&AccessLimit=0
````
+ **Request Parameters**

parameter | shortName | required | type | description | remark
-------- | -------- | -------- | -------- | -------- | --------
UserID | UserID | yes | string | 交易用户代码 | 
APPID | APPID | yes | string | 应用编号 | 
Token | Token | yes | string | 令牌 | 
MemberID | MemberID | yes | string | 成员代码 | 
LoginTime | LoginTime | yes | double | 登录时间 | 
ExpireTime | ExpireTime | yes | double | 失效时间 | 
AccessLimit | AccessLimit | no | int | 每秒访问的次数限制 | 

+ **Response**

```
{
	"action": "RecvOTokenInsert",
	"errorCode": 0,
	"errorMsg": "Success",
	"result": [			
				{"table":"OTokenInsert",
				 "data":{
					,UserID:"1983723"
					,APPID:"ios"
					,Token:""
					,MemberID:"1983723"
					,LoginTime:1657543832
					,ExpireTime:1657543832
					,AccessLimit:0
					}
				}
				{"table":"UserSession",
				 "data":{
					,UserID:"1983723"
					,Password:""
					,UserProductID:"1983723"
					,MacAddress:""
					,ClientIPAddress:""
					,HDSerialID:"1983723"
					,AuthCode:""
					,APPID:"ios"
					,Token:""
					,Remark:"my_little_tips"
					,APIID:"1983723"
					,LoginTime:1657543832
					,ExpireTime:1657543832
					,IPAddress:""
					,MaxLocalID:"1983723"
					,SessionNo:1033201232
					,FrontNo:1
					,AccessLimit:0
					,UserType:"1"
					,MemberID:"1983723"
					,LimitAccesses:0
					,SinkType:"1"
					,InsertTime:1657543832
					,UpdateTime:1657543832
					}
				}
	]
}
```

+ **Response Parameters**

OTokenInsert:

parameter | shortName | type | description | remark
-------- | -------- | -------- | -------- | -------- 					
UserID | UserID | string | 交易用户代码 | 
APPID | APPID | string | 应用编号 | 
Token | Token | string | 令牌 | 
MemberID | MemberID | string | 成员代码 | 
LoginTime | LoginTime | double | 登录时间 | 
ExpireTime | ExpireTime | double | 失效时间 | 
AccessLimit | AccessLimit | int | 每秒访问的次数限制 | 

UserSession:

parameter | shortName | type | description | remark
-------- | -------- | -------- | -------- | -------- 					
UserID | UserID | string | 交易用户代码 | 
Password | Password | string | 密码 | 
UserProductID | UserProductID | string | 用户端产品信息 | 
MacAddress | MacAddress | string | Mac地址 | 
ClientIPAddress | ClientIPAddress | string | 终端IP地址 | 
HDSerialID | HDSerialID | string | 硬盘序列号 | 
AuthCode | AuthCode | string | 授权编码 | 
APPID | APPID | string | 应用编号 | 
Token | Token | string | 令牌 | 
Remark | Remark | string | 备注 | 
APIID | APIID | string | 接口端产品信息 | 
LoginTime | LoginTime | double | 登录时间 | 
ExpireTime | ExpireTime | double | 失效时间 | 
IPAddress | IPAddress | string | 允许登录的IP地址 | 
MaxLocalID | MaxLocalID | string | 最大本地标识 | 
SessionNo | SessionNo | int | 会话编号 | 
FrontNo | FrontNo | int | 前置编号 | 
AccessLimit | AccessLimit | int | 每秒访问的次数限制 | 
UserType | UserType | string | 用户类型 | 1:普通用户(订阅自己/可下自己订单);2:观察用户OB(订阅其他用户/不可下单);5:超级管理员(订阅所有/可给所有用户下单/可爆仓);
MemberID | MemberID | string | 成员代码 | 
LimitAccesses | LimitAccesses | int | 该秒已经访问的次数 | 
SinkType | SinkType | string | 下游系统同步类型 | 0:同步到数据库;1:同步到Redis不到数据库;2:不同步;
InsertTime | InsertTime | double | 插入时间 | 
UpdateTime | UpdateTime | double | 更新时间 | 


## **OToken操作请求**

+ **Request URL**

`POST /action/v1.0/SendOTokenAction`


+ **Request Post Body**

```
{
	,"UserID":"1983723"
	,"APPID":"ios"
	,"ActionFlag":"1"
}
```
+ **curl Example**

````
curl -d '{"UserID":"1983723","APPID":"ios","ActionFlag":"1"}'  -X POST  http://127.0.0.1:10000/action/v1.0/SendOTokenAction
````
+ **ws Example**

````
{"RequestNo":2,"SendOTokenAction":{"UserID":"1983723","APPID":"ios","ActionFlag":"1"}}
````
+ **Web rowser Example**

````
http://127.0.0.1:10000/action/v1.0/SendOTokenAction?UserID=1983723&APPID=ios&ActionFlag=1
````
+ **Request Parameters**

parameter | shortName | required | type | description | remark
-------- | -------- | -------- | -------- | -------- | --------
UserID | UserID | yes | string | 交易用户代码 | 
APPID | APPID | yes | string | 应用编号 | 
ActionFlag | ActionFlag | yes | string | 操作标志 | 1:删除;2:挂起;3:激活;4:修改;5:触发;6:增减;

+ **Response**

```
{
	"action": "RecvOTokenAction",
	"errorCode": 0,
	"errorMsg": "Success",
	"result": [			
				{"table":"OTokenAction",
				 "data":{
					,UserID:"1983723"
					,APPID:"ios"
					,ActionFlag:"1"
					}
				}
				{"table":"UserSession",
				 "data":{
					,UserID:"1983723"
					,Password:""
					,UserProductID:"1983723"
					,MacAddress:""
					,ClientIPAddress:""
					,HDSerialID:"1983723"
					,AuthCode:""
					,APPID:"ios"
					,Token:""
					,Remark:"my_little_tips"
					,APIID:"1983723"
					,LoginTime:1657543832
					,ExpireTime:1657543832
					,IPAddress:""
					,MaxLocalID:"1983723"
					,SessionNo:1033201232
					,FrontNo:1
					,AccessLimit:0
					,UserType:"1"
					,MemberID:"1983723"
					,LimitAccesses:0
					,SinkType:"1"
					,InsertTime:1657543832
					,UpdateTime:1657543832
					}
				}
	]
}
```

+ **Response Parameters**

OTokenAction:

parameter | shortName | type | description | remark
-------- | -------- | -------- | -------- | -------- 					
UserID | UserID | string | 交易用户代码 | 
APPID | APPID | string | 应用编号 | 
ActionFlag | ActionFlag | string | 操作标志 | 1:删除;2:挂起;3:激活;4:修改;5:触发;6:增减;

UserSession:

parameter | shortName | type | description | remark
-------- | -------- | -------- | -------- | -------- 					
UserID | UserID | string | 交易用户代码 | 
Password | Password | string | 密码 | 
UserProductID | UserProductID | string | 用户端产品信息 | 
MacAddress | MacAddress | string | Mac地址 | 
ClientIPAddress | ClientIPAddress | string | 终端IP地址 | 
HDSerialID | HDSerialID | string | 硬盘序列号 | 
AuthCode | AuthCode | string | 授权编码 | 
APPID | APPID | string | 应用编号 | 
Token | Token | string | 令牌 | 
Remark | Remark | string | 备注 | 
APIID | APIID | string | 接口端产品信息 | 
LoginTime | LoginTime | double | 登录时间 | 
ExpireTime | ExpireTime | double | 失效时间 | 
IPAddress | IPAddress | string | 允许登录的IP地址 | 
MaxLocalID | MaxLocalID | string | 最大本地标识 | 
SessionNo | SessionNo | int | 会话编号 | 
FrontNo | FrontNo | int | 前置编号 | 
AccessLimit | AccessLimit | int | 每秒访问的次数限制 | 
UserType | UserType | string | 用户类型 | 1:普通用户(订阅自己/可下自己订单);2:观察用户OB(订阅其他用户/不可下单);5:超级管理员(订阅所有/可给所有用户下单/可爆仓);
MemberID | MemberID | string | 成员代码 | 
LimitAccesses | LimitAccesses | int | 该秒已经访问的次数 | 
SinkType | SinkType | string | 下游系统同步类型 | 0:同步到数据库;1:同步到Redis不到数据库;2:不同步;
InsertTime | InsertTime | double | 插入时间 | 
UpdateTime | UpdateTime | double | 更新时间 | 


## **APIKEY录入请求**

+ **Request URL**

`POST /action/v1.0/SendAPIKeyInsert`


+ **Request Post Body**

```
{
	,"Name":""
	,"UserID":"1983723"
	,"APPID":"ios"
	,"MemberID":"1983723"
	,"AccessKey":""
	,"SecretKey":""
	,"Auth":""
	,"AccessLimit":0
	,"CreateTime":1657543832
	,"ExpireTime":1657543832
	,"IPAddress":""
}
```
+ **curl Example**

````
curl -d '{"Name":"","UserID":"1983723","APPID":"ios","MemberID":"1983723","AccessKey":"","SecretKey":"","Auth":"","AccessLimit":"0,"CreateTime":"1657543832,"ExpireTime":"1657543832,"IPAddress":""}'  -X POST  http://127.0.0.1:10000/action/v1.0/SendAPIKeyInsert
````
+ **ws Example**

````
{"RequestNo":2,"SendAPIKeyInsert":{"Name":"","UserID":"1983723","APPID":"ios","MemberID":"1983723","AccessKey":"","SecretKey":"","Auth":"","AccessLimit":"0,"CreateTime":"1657543832,"ExpireTime":"1657543832,"IPAddress":""}}
````
+ **Web rowser Example**

````
http://127.0.0.1:10000/action/v1.0/SendAPIKeyInsert?Name=&UserID=1983723&APPID=ios&MemberID=1983723&AccessKey=&SecretKey=&Auth=&AccessLimit=0&CreateTime=1657543832&ExpireTime=1657543832&IPAddress=
````
+ **Request Parameters**

parameter | shortName | required | type | description | remark
-------- | -------- | -------- | -------- | -------- | --------
Name | Name | yes | string | 备注 | 
UserID | UserID | yes | string | 交易用户代码 | 
APPID | APPID | no | string | 应用编号 | 
MemberID | MemberID | yes | string | 成员代码 | 
AccessKey | AccessKey | yes | string | 访问密钥 | 
SecretKey | SecretKey | yes | string | 签名加密密钥 | 
Auth | Auth | yes | string | 权限，由8位二进制数组成，0：关闭，1：开通。由高位到低位依次代表：只读;提币;交易。如：10100000表示只读、交易权限开通，提币权限关闭 | 
AccessLimit | AccessLimit | no | int | 每秒访问的次数限制 | 
CreateTime | CreateTime | yes | double | 创建时间 | 
ExpireTime | ExpireTime | no | double | 失效时间 | 
IPAddress | IPAddress | no | string | 绑定IP地址，多个IP地址用半角分号分隔，如：192.168.1.1;192.168.1.2 | 

+ **Response**

```
{
	"action": "RecvAPIKeyInsert",
	"errorCode": 0,
	"errorMsg": "Success",
	"result": [			
				{"table":"APIKeyInsert",
				 "data":{
					,Name:""
					,UserID:"1983723"
					,APPID:"ios"
					,MemberID:"1983723"
					,AccessKey:""
					,SecretKey:""
					,Auth:""
					,AccessLimit:0
					,CreateTime:1657543832
					,ExpireTime:1657543832
					,IPAddress:""
					}
				}
				{"table":"APIKey",
				 "data":{
					,Name:""
					,UserID:"1983723"
					,APPID:"ios"
					,MemberID:"1983723"
					,AccessKey:""
					,SecretKey:""
					,Auth:""
					,AccessLimit:0
					,CreateTime:1657543832
					,ExpireTime:1657543832
					,IPAddress:""
					,RemainDays:0
					,Status:0
					}
				}
	]
}
```

+ **Response Parameters**

APIKeyInsert:

parameter | shortName | type | description | remark
-------- | -------- | -------- | -------- | -------- 					
Name | Name | string | 备注 | 
UserID | UserID | string | 交易用户代码 | 
APPID | APPID | string | 应用编号 | 
MemberID | MemberID | string | 成员代码 | 
AccessKey | AccessKey | string | 访问密钥 | 
SecretKey | SecretKey | string | 签名加密密钥 | 
Auth | Auth | string | 权限，由8位二进制数组成，0：关闭，1：开通。由高位到低位依次代表：只读;提币;交易。如：10100000表示只读、交易权限开通，提币权限关闭 | 
AccessLimit | AccessLimit | int | 每秒访问的次数限制 | 
CreateTime | CreateTime | double | 创建时间 | 
ExpireTime | ExpireTime | double | 失效时间 | 
IPAddress | IPAddress | string | 绑定IP地址，多个IP地址用半角分号分隔，如：192.168.1.1;192.168.1.2 | 

APIKey:

parameter | shortName | type | description | remark
-------- | -------- | -------- | -------- | -------- 					
Name | Name | string | 备注 | 
UserID | UserID | string | 交易用户代码 | 
APPID | APPID | string | 应用编号 | 
MemberID | MemberID | string | 成员代码 | 
AccessKey | AccessKey | string | 访问密钥 | 
SecretKey | SecretKey | string | 签名加密密钥 | 
Auth | Auth | string | 权限，由8位二进制数组成，0：关闭，1：开通。由高位到低位依次代表：只读;提币;交易。如：10100000表示只读、交易权限开通，提币权限关闭 | 
AccessLimit | AccessLimit | int | 每秒访问的次数限制 | 
CreateTime | CreateTime | double | 创建时间 | 
ExpireTime | ExpireTime | double | 失效时间 | 
IPAddress | IPAddress | string | 绑定IP地址，多个IP地址用半角分号分隔，如：192.168.1.1;192.168.1.2 | 
RemainDays | RemainDays | int | 剩余有效天数 | 
Status | Status | int | 状态，1：正常，0：失效 | 


## **APIKEY操作请求**

+ **Request URL**

`POST /action/v1.0/SendAPIKeyAction`


+ **Request Post Body**

```
{
	,"Name":""
	,"AccessKey":""
	,"Auth":""
	,"AccessLimit":0
	,"IPAddress":""
	,"ExpireTime":1657543832
	,"ActionFlag":"1"
}
```
+ **curl Example**

````
curl -d '{"Name":"","AccessKey":"","Auth":"","AccessLimit":"0,"IPAddress":"","ExpireTime":"1657543832,"ActionFlag":"1"}'  -X POST  http://127.0.0.1:10000/action/v1.0/SendAPIKeyAction
````
+ **ws Example**

````
{"RequestNo":2,"SendAPIKeyAction":{"Name":"","AccessKey":"","Auth":"","AccessLimit":"0,"IPAddress":"","ExpireTime":"1657543832,"ActionFlag":"1"}}
````
+ **Web rowser Example**

````
http://127.0.0.1:10000/action/v1.0/SendAPIKeyAction?Name=&AccessKey=&Auth=&AccessLimit=0&IPAddress=&ExpireTime=1657543832&ActionFlag=1
````
+ **Request Parameters**

parameter | shortName | required | type | description | remark
-------- | -------- | -------- | -------- | -------- | --------
Name | Name | yes | string | 备注 | 
AccessKey | AccessKey | yes | string | 访问密钥 | 
Auth | Auth | yes | string | 权限，由8位二进制数组成，0：关闭，1：开通。由高位到低位依次代表：只读;提币;交易。如：10100000表示只读、交易权限开通，提币权限关闭 | 
AccessLimit | AccessLimit | no | int | 每秒访问的次数限制 | 
IPAddress | IPAddress | no | string | 绑定IP地址，多个IP地址用半角分号分隔，如：192.168.1.1;192.168.1.2 | 
ExpireTime | ExpireTime | no | double | 失效时间 | 
ActionFlag | ActionFlag | yes | string | 操作标志 | 1:删除;2:挂起;3:激活;4:修改;5:触发;6:增减;

+ **Response**

```
{
	"action": "RecvAPIKeyAction",
	"errorCode": 0,
	"errorMsg": "Success",
	"result": [			
				{"table":"APIKeyAction",
				 "data":{
					,Name:""
					,AccessKey:""
					,Auth:""
					,AccessLimit:0
					,IPAddress:""
					,ExpireTime:1657543832
					,ActionFlag:"1"
					}
				}
				{"table":"APIKey",
				 "data":{
					,Name:""
					,UserID:"1983723"
					,APPID:"ios"
					,MemberID:"1983723"
					,AccessKey:""
					,SecretKey:""
					,Auth:""
					,AccessLimit:0
					,CreateTime:1657543832
					,ExpireTime:1657543832
					,IPAddress:""
					,RemainDays:0
					,Status:0
					}
				}
	]
}
```

+ **Response Parameters**

APIKeyAction:

parameter | shortName | type | description | remark
-------- | -------- | -------- | -------- | -------- 					
Name | Name | string | 备注 | 
AccessKey | AccessKey | string | 访问密钥 | 
Auth | Auth | string | 权限，由8位二进制数组成，0：关闭，1：开通。由高位到低位依次代表：只读;提币;交易。如：10100000表示只读、交易权限开通，提币权限关闭 | 
AccessLimit | AccessLimit | int | 每秒访问的次数限制 | 
IPAddress | IPAddress | string | 绑定IP地址，多个IP地址用半角分号分隔，如：192.168.1.1;192.168.1.2 | 
ExpireTime | ExpireTime | double | 失效时间 | 
ActionFlag | ActionFlag | string | 操作标志 | 1:删除;2:挂起;3:激活;4:修改;5:触发;6:增减;

APIKey:

parameter | shortName | type | description | remark
-------- | -------- | -------- | -------- | -------- 					
Name | Name | string | 备注 | 
UserID | UserID | string | 交易用户代码 | 
APPID | APPID | string | 应用编号 | 
MemberID | MemberID | string | 成员代码 | 
AccessKey | AccessKey | string | 访问密钥 | 
SecretKey | SecretKey | string | 签名加密密钥 | 
Auth | Auth | string | 权限，由8位二进制数组成，0：关闭，1：开通。由高位到低位依次代表：只读;提币;交易。如：10100000表示只读、交易权限开通，提币权限关闭 | 
AccessLimit | AccessLimit | int | 每秒访问的次数限制 | 
CreateTime | CreateTime | double | 创建时间 | 
ExpireTime | ExpireTime | double | 失效时间 | 
IPAddress | IPAddress | string | 绑定IP地址，多个IP地址用半角分号分隔，如：192.168.1.1;192.168.1.2 | 
RemainDays | RemainDays | int | 剩余有效天数 | 
Status | Status | int | 状态，1：正常，0：失效 | 


## **仓位合并操作请求**

+ **Request URL**

`POST /action/v1.0/SendPositionMerge`


+ **Request Post Body**

```
{
	,"LocalID":"1983723"
	,"MemberID":"1983723"
	,"PositionID":"1983723"
	,"PositionID1":"1983723"
	,"PositionID2":"1983723"
	,"PositionID3":"1983723"
	,"PositionID4":"1983723"
	,"PositionID5":"1983723"
	,"PositionID6":"1983723"
	,"PositionID7":"1983723"
	,"PositionID8":"1983723"
	,"PositionID9":"1983723"
	,"PositionID10":"1983723"
}
```
+ **curl Example**

````
curl -d '{"LocalID":"1983723","MemberID":"1983723","PositionID":"1983723","PositionID1":"1983723","PositionID2":"1983723","PositionID3":"1983723","PositionID4":"1983723","PositionID5":"1983723","PositionID6":"1983723","PositionID7":"1983723","PositionID8":"1983723","PositionID9":"1983723","PositionID10":"1983723"}'  -X POST  http://127.0.0.1:10000/action/v1.0/SendPositionMerge
````
+ **ws Example**

````
{"RequestNo":2,"SendPositionMerge":{"LocalID":"1983723","MemberID":"1983723","PositionID":"1983723","PositionID1":"1983723","PositionID2":"1983723","PositionID3":"1983723","PositionID4":"1983723","PositionID5":"1983723","PositionID6":"1983723","PositionID7":"1983723","PositionID8":"1983723","PositionID9":"1983723","PositionID10":"1983723"}}
````
+ **Web rowser Example**

````
http://127.0.0.1:10000/action/v1.0/SendPositionMerge?LocalID=1983723&MemberID=1983723&PositionID=1983723&PositionID1=1983723&PositionID2=1983723&PositionID3=1983723&PositionID4=1983723&PositionID5=1983723&PositionID6=1983723&PositionID7=1983723&PositionID8=1983723&PositionID9=1983723&PositionID10=1983723
````
+ **Request Parameters**

parameter | shortName | required | type | description | remark
-------- | -------- | -------- | -------- | -------- | --------
LocalID | LocalID | no | string | 操作本地号 | 
MemberID | MemberID | no | string | 成员代码 | 
PositionID | PositionID | no | string | 持仓代码 | 
PositionID1 | PositionID1 | no | string | 持仓代码1 | 
PositionID2 | PositionID2 | no | string | 持仓代码2 | 
PositionID3 | PositionID3 | no | string | 持仓代码3 | 
PositionID4 | PositionID4 | no | string | 持仓代码4 | 
PositionID5 | PositionID5 | no | string | 持仓代码5 | 
PositionID6 | PositionID6 | no | string | 持仓代码6 | 
PositionID7 | PositionID7 | no | string | 持仓代码7 | 
PositionID8 | PositionID8 | no | string | 持仓代码8 | 
PositionID9 | PositionID9 | no | string | 持仓代码9 | 
PositionID10 | PositionID10 | no | string | 持仓代码10 | 

+ **Response**

```
{
	"action": "RecvPositionMerge",
	"errorCode": 0,
	"errorMsg": "Success",
	"result": [			
				{"table":"PositionMerge",
				 "data":{
					,LocalID:"1983723"
					,MemberID:"1983723"
					,PositionID:"1983723"
					,PositionID1:"1983723"
					,PositionID2:"1983723"
					,PositionID3:"1983723"
					,PositionID4:"1983723"
					,PositionID5:"1983723"
					,PositionID6:"1983723"
					,PositionID7:"1983723"
					,PositionID8:"1983723"
					,PositionID9:"1983723"
					,PositionID10:"1983723"
					}
				}
				{"table":"Position",
				 "data":{
					,PositionID:"1983723"
					,MemberID:"1983723"
					,TradeUnitID:"1983723"
					,ExchangeID:"CEX"
					,InstrumentID:"BTCUSD"
					,PositionClass:"1"
					,PosiDirection:"1"
					,PrePosition:182582.18
					,Position:182582.18
					,LongFrozen:182582.18
					,ShortFrozen:182582.18
					,PreLongFrozen:182582.18
					,PreShortFrozen:182582.18
					,HighestPosition:182582.18
					,ClosePosition:182582.18
					,PositionCost:182582.18
					,CostPrice:182582.18
					,UseMargin:182582.18
					,FrozenMargin:182582.18
					,LongFrozenMargin:182582.18
					,ShortFrozenMargin:182582.18
					,CloseProfit:10
					,TotalPositionCost:182582.18
					,TotalCloseProfit:182582.18
					,OpenPrice:182582.18
					,ClosePrice:182582.18
					,TradeFee:182582.18
					,PositionFee:182582.18
					,Leverage:125
					,AccountID:"1983723"
					,PriceCurrency:"BTC"
					,ClearCurrency:"BTC"
					,SettlementGroup:"999"
					,IsCrossMargin:1
					,CloseOrderID:"1983723"
					,SLTriggerPrice:182582.18
					,TPTriggerPrice:182582.18
					,BeginTime:1657543832
					,InsertTime:1657543832
					,LastOpenTime:1657543832
					,UpdateTime:1657543832
					,BusinessNo:11176422300333
					,IsAutoAddMargin:1
					,Frequency:0
					,MaintMargin:182582.18
					,UnRealProfit:182582.18
					,LiquidPrice:182582.18
					,CreateTime:""
					,CopyMemberID:"1983723"
					,CopyProfitRate:182582.18
					,CopyProfit:182582.18
					,FirstTradeID:"1983723"
					,LastTradeID:"1983723"
					,BusinessType:"1"
					,BusinessValue:""
					,Reserve:182582.18
					,ReserveProfit:182582.18
					,Remark:"my_little_tips"
					}
				}
	]
}
```

+ **Response Parameters**

PositionMerge:

parameter | shortName | type | description | remark
-------- | -------- | -------- | -------- | -------- 					
LocalID | LocalID | string | 操作本地号 | 
MemberID | MemberID | string | 成员代码 | 
PositionID | PositionID | string | 持仓代码 | 
PositionID1 | PositionID1 | string | 持仓代码1 | 
PositionID2 | PositionID2 | string | 持仓代码2 | 
PositionID3 | PositionID3 | string | 持仓代码3 | 
PositionID4 | PositionID4 | string | 持仓代码4 | 
PositionID5 | PositionID5 | string | 持仓代码5 | 
PositionID6 | PositionID6 | string | 持仓代码6 | 
PositionID7 | PositionID7 | string | 持仓代码7 | 
PositionID8 | PositionID8 | string | 持仓代码8 | 
PositionID9 | PositionID9 | string | 持仓代码9 | 
PositionID10 | PositionID10 | string | 持仓代码10 | 

Position:

parameter | shortName | type | description | remark
-------- | -------- | -------- | -------- | -------- 					
PositionID | PositionID | string | 持仓代码 | 
MemberID | MemberID | string | 成员代码 | 
TradeUnitID | TradeUnitID | string | 交易单元代码 | 
ExchangeID | ExchangeID | string | 交易所代码 | 
InstrumentID | InstrumentID | string | 标的代码 | 
PositionClass | PositionClass | string | 持仓类型 | 0:不设置;1:保证金交易;3:资产;4:持币成本;
PosiDirection | PosiDirection | string | 持仓多空方向 | 0:多头;1:空头;2:净;
PrePosition | PrePosition | double | 上次持仓 | 
Position | Position | double | 总持仓 | 
LongFrozen | LongFrozen | double | 多头冻结 | 
ShortFrozen | ShortFrozen | double | 空头冻结 | 
PreLongFrozen | PreLongFrozen | double | 昨日多头冻结 | 
PreShortFrozen | PreShortFrozen | double | 昨日空头冻结 | 
HighestPosition | HighestPosition | double | 最大持仓 | 
ClosePosition | ClosePosition | double | 可平持仓 | 
PositionCost | PositionCost | double | 持仓成本 | 
CostPrice | CostPrice | double | 成本价 | 
UseMargin | UseMargin | double | 占用保证金 | 
FrozenMargin | FrozenMargin | double | 冻结保证金 | 
LongFrozenMargin | LongFrozenMargin | double | 多头冻结保证金 | 
ShortFrozenMargin | ShortFrozenMargin | double | 空头冻结保证金 | 
CloseProfit | CloseProfit | double | 平仓盈亏 | 
TotalPositionCost | TotalPositionCost | double | 开仓成本 | 
TotalCloseProfit | TotalCloseProfit | double | 总平仓盈亏 | 
OpenPrice | OpenPrice | double | 开仓均价 | 
ClosePrice | ClosePrice | double | 平仓均价 | 
TradeFee | TradeFee | double | 交易费用 | 
PositionFee | PositionFee | double | 持仓费用或者股票分红等 | 
Leverage | Leverage | double | 杠杆倍数 | 
AccountID | AccountID | string | 资金账号 | 
PriceCurrency | PriceCurrency | string | 计价币种 | 
ClearCurrency | ClearCurrency | string | 清算币种 | 
SettlementGroup | SettlementGroup | string | 结算组编号 | 
IsCrossMargin | IsCrossMargin | int | 是否全仓 | 
CloseOrderID | CloseOrderID | string | 平仓单代码 | 
SLTriggerPrice | SLTriggerPrice | double | 止损触发价 | 
TPTriggerPrice | TPTriggerPrice | double | 止盈触发价 | 
BeginTime | BeginTime | double | 持仓不为0的开始时间 | 
InsertTime | InsertTime | double | 插入时间(新持仓时间) | 
LastOpenTime | LastOpenTime | double | 最后开仓时间 | 
UpdateTime | UpdateTime | double | 更新时间 | 
BusinessNo | BusinessNo | double | 业务序列号 | 
IsAutoAddMargin | IsAutoAddMargin | int | 是否自动追加保证金 | 
Frequency | Frequency | int | 每秒更新的次数 | 
MaintMargin | MaintMargin | double | 维持保证金 | 
UnRealProfit | UnRealProfit | double | 未实现盈亏 | 
LiquidPrice | LiquidPrice | double | 清算价格 | 
CreateTime | CreateTime | string | 创建时间 | 
CopyMemberID | CopyMemberID | string | 带单员代码 | 
CopyProfitRate | CopyProfitRate | double | 带单分配比例 | 
CopyProfit | CopyProfit | double | 带单盈利分配 | 
FirstTradeID | FID | string | 初始成交订单号 | 
LastTradeID | LID | string | 结束成交订单号 | 
BusinessType | BT | string | 业务类别 | 0:普通;1:申赎;2:行权;3:中立仓申报;4:递延交割申报;5:互换定单;6:质押;7:合并分拆;8:转股;9:回售回购;a:投票;b:融资融券;c:期权执行;d:传递被平仓订单BusinessValue;P:自定义11;Q:自定义10;R:自定义9;S:连续交易;T:自定义7;U:画线委托;V:限价市价;W:条件;X:止盈止损;Y:追踪出场;Z:指标类型;
BusinessValue | BV | string | 业务值 | 
Reserve | Reserve | double | 保留资金 | 
ReserveProfit | ReserveProfit | double | 保留资金盈亏 | 
Remark | R | string | 备注 | 


## **用户入金操作请求**

+ **Request URL**

`POST /action/v1.0/SendDeposit`


+ **Request Post Body**

```
{
	,"LocalID":"1983723"
	,"MemberID":"1983723"
	,"AccountID":"1983723"
	,"SettlementGroup":"999"
	,"Amount":182582.18
	,"Currency":"BTC"
	,"Remark":"my_little_tips"
}
```
+ **curl Example**

````
curl -d '{"LocalID":"1983723","MemberID":"1983723","AccountID":"1983723","SettlementGroup":"999","Amount":"182582.18,"Currency":"BTC","Remark":"my_little_tips"}'  -X POST  http://127.0.0.1:10000/action/v1.0/SendDeposit
````
+ **ws Example**

````
{"RequestNo":2,"SendDeposit":{"LocalID":"1983723","MemberID":"1983723","AccountID":"1983723","SettlementGroup":"999","Amount":"182582.18,"Currency":"BTC","Remark":"my_little_tips"}}
````
+ **Web rowser Example**

````
http://127.0.0.1:10000/action/v1.0/SendDeposit?LocalID=1983723&MemberID=1983723&AccountID=1983723&SettlementGroup=999&Amount=182582.18&Currency=BTC&Remark=my_little_tips
````
+ **Request Parameters**

parameter | shortName | required | type | description | remark
-------- | -------- | -------- | -------- | -------- | --------
LocalID | LocalID | no | string | 出入金本地号 | 
MemberID | MemberID | yes | string | 成员代码 | 
AccountID | AccountID | yes | string | 资金账号 | 
SettlementGroup | SettlementGroup | yes | string | 结算组编号 | 
Amount | Amount | yes | double | 发生额 | 
Currency | Currency | yes | string | 币种 | 
Remark | Remark | no | string | 备注 | 

+ **Response**

```
{
	"action": "RecvDeposit",
	"errorCode": 0,
	"errorMsg": "Success",
	"result": [			
				{"table":"Deposit",
				 "data":{
					,LocalID:"1983723"
					,MemberID:"1983723"
					,AccountID:"1983723"
					,SettlementGroup:"999"
					,Amount:182582.18
					,Currency:"BTC"
					,Remark:"my_little_tips"
					}
				}
				{"table":"Order",
				 "data":{
					,MemberID:"1983723"
					,TradeUnitID:"1983723"
					,AccountID:"1983723"
					,LocalID:"1983723"
					,ExchangeID:"CEX"
					,InstrumentID:"BTCUSD"
					,OrderPriceType:"1"
					,Direction:"1"
					,OffsetFlag:"1"
					,Price:182582.18
					,Volume:182582.18
					,VolumeDisplay:182582.18
					,VolumeMode:"1"
					,Cost:182582.18
					,OrderType:"1"
					,GTDTime:1657543832
					,MinVolume:182582.18
					,BusinessType:"1"
					,BusinessValue:""
					,CloseOrderID:"1983723"
					,IsCrossMargin:1
					,Remark:"my_little_tips"
					,OrderID:"12376422300333"
					,CopyMemberID:"1983723"
					,CopyOrderID:"1983723"
					,Leverage:125
					,CopyProfitRate:182582.18
					,APPID:"ios"
					,PositionID:"1983723"
					,TriggerPrice:182582.18
					,Reserve:182582.18
					,OrderStatus:"1"
					,DeriveSource:"1"
					,DeriveDetail:""
					,VolumeTraded:182582.18
					,VolumeRemain:182582.18
					,VolumeCancled:182582.18
					,InsertTime:1657543832
					,UpdateTime:1657543832
					,Priority:0
					,TimeSortNo:1657543832
					,FrontNo:1
					,PriceCurrency:"BTC"
					,FeeCurrency:"BTC"
					,ClearCurrency:"BTC"
					,FrozenMoney:182582.18
					,FrozenFee:182582.18
					,FrozenMargin:182582.18
					,Fee:0.22364
					,CloseProfit:10
					,Turnover:182582.18
					,RelatedOrderID:"1983723"
					,BusinessResult:""
					,BusinessNo:11176422300333
					,Tradable:0
					,SettlementGroup:"999"
					,PosiDirection:"1"
					,TradePrice:182582.18
					,OpenPrice:182582.18
					,TriggerOrderID:"1983723"
					,SLTriggerPrice:182582.18
					,TPTriggerPrice:182582.18
					,CopyProfit:182582.18
					,Position:182582.18
					,UserID:"1983723"
					,LastPriceByInsert:182582.18
					,BidPrice1ByInsert:182582.18
					,AskPrice1ByInsert:182582.18
					,Available:182582.18
					,CreateTime:""
					}
				}
	]
}
```

+ **Response Parameters**

Deposit:

parameter | shortName | type | description | remark
-------- | -------- | -------- | -------- | -------- 					
LocalID | LocalID | string | 出入金本地号 | 
MemberID | MemberID | string | 成员代码 | 
AccountID | AccountID | string | 资金账号 | 
SettlementGroup | SettlementGroup | string | 结算组编号 | 
Amount | Amount | double | 发生额 | 
Currency | Currency | string | 币种 | 
Remark | Remark | string | 备注 | 

Order:

parameter | shortName | type | description | remark
-------- | -------- | -------- | -------- | -------- 					
MemberID | M | string | 成员代码 | 
TradeUnitID | T | string | 交易单元代码 | 
AccountID | A | string | 资金账号 | 
LocalID | L | string | 报单本地标识 | 
ExchangeID | E | string | 交易所代码 | 
InstrumentID | I | string | 标的代码 | 
OrderPriceType | PT | string | 报单价格类型 | 0:限价(手动指定Price值);1:任意价(Price按最高最低限价取值);2:最优价/对手价(Price按对方1档价取值);3:五档价(Price按对方5档价取值);4:十档价(类同上);7:最优Marker价(Price按本方1档价或其+1Tick取值);8:Price按委托额Cost和订单簿情况确定值;9:保底点差价:成交价格Price不会劣于系统设置差价成交;
Direction | D | string | 买卖方向 | 0:买;1:卖;
OffsetFlag | O | string | 开平标志 | 0:开仓;1:平仓;2:强平;3:平今;4:平昨;5:全平;6:指定订单平仓;7:指定成交平仓;8:最大平仓;
Price | P | double | 报单价格 | 
Volume | V | double | 数量 | 
VolumeDisplay | D | double | 显示数量 | 
VolumeMode | VM | string | 数量取值方式 | 0:百分比;1:绝对值;
Cost | C | double | 委托额 | 
OrderType | OT | string | 订单类型 | 0:普通订单;1:(FillAndKill/IOC)立即完成并且剩余撤销;2:(FillOrKill)立即完成全部或者最小订单要求否则全部撤销;3:需要进入订单簿被动成交,否则全部撤销(只做Maker/PostOnly);4:不需要撮合直接成交;
GTDTime | GT | double | GTD时间 | 
MinVolume | MV | double | 最小成交量 | 
BusinessType | BT | string | 业务类别 | 0:普通;1:申赎;2:行权;3:中立仓申报;4:递延交割申报;5:互换定单;6:质押;7:合并分拆;8:转股;9:回售回购;a:投票;b:融资融券;c:期权执行;d:传递被平仓订单BusinessValue;P:自定义11;Q:自定义10;R:自定义9;S:连续交易;T:自定义7;U:画线委托;V:限价市价;W:条件;X:止盈止损;Y:追踪出场;Z:指标类型;
BusinessValue | BV | string | 业务值 | 
CloseOrderID | CI | string | 平仓指定开仓的订单号 | 
IsCrossMargin | IsCrossMargin | int | 是否全仓 | 
Remark | R | string | 备注 | 
OrderID | OrderID | string | 报单系统唯一代码 | 
CopyMemberID | CopyMemberID | string | 带单员代码 | 
CopyOrderID | CopyOrderID | string | 带单员报单号 | 
Leverage | Leverage | double | 委托单杠杆倍数 | 
CopyProfitRate | CopyProfitRate | double | 带单分配比例 | 
APPID | APPID | string | 应用编号 | 
PositionID | B | string | 持仓代码 | 
TriggerPrice | TriggerPrice | double | 触发价 | 
Reserve | Reserve | double | 保留资金 | 
OrderStatus | OrderStatus | string | 报单状态 | 0:未设置状态;1:全部成交;2:部分成交未撤单;3:部分成交已撤单;4:未成交未撤单;6:无成交已撤单;
DeriveSource | DeriveSource | string | 衍生来源 | 0:正常;1:报价衍生;2:期权执行;3:组合衍生;4:场外成交强平衍生;5:场外成交大宗交易衍生;6:场外成交期转现衍生;7:场外成交只刷单衍生;8:触发订单衍生;9:止损触发订单衍生;a:止盈触发订单衍生;b:资金操作衍生;c:仓位合并衍生;d:CFD订单衍生;e:场外成交ADL衍生;f:回购手续费不能是币;
DeriveDetail | DeriveDetail | string | 衍生明细 | 
VolumeTraded | VolumeTraded | double | 成交数量 | 
VolumeRemain | VolumeRemain | double | 剩余数量 | 
VolumeCancled | VolumeCancled | double | 已经撤单数量 | 
InsertTime | InsertTime | double | 插入时间 | 
UpdateTime | UpdateTime | double | 更新时间 | 
Priority |  | int | 优先权 | 
TimeSortNo |  | double | 按时间排队的序号 | 
FrontNo | FrontNo | int | 前置编号 | 
PriceCurrency | PriceCurrency | string | 计价币种 | 
FeeCurrency | FeeCurrency | string | 手续费币种 | 
ClearCurrency | ClearCurrency | string | 清算币种 | 
FrozenMoney | FrozenMoney | double | 冻结资金 | 
FrozenFee | FrozenFee | double | 冻结手续费 | 
FrozenMargin | FrozenMargin | double | 冻结保证金 | 
Fee | Fee | double | 手续费 | 
CloseProfit | CloseProfit | double | 平仓盈亏 | 
Turnover | Turnover | double | 成交金额 | 
RelatedOrderID | RelatedOrderID | string | 关联报单号 | 
BusinessResult | BusinessResult | string | 业务执行结果 | 
BusinessNo | BusinessNo | double | 业务序列号 | 
Tradable |  | int | 是否可以参加交易 | 
SettlementGroup |  | string | 结算组编号 | 
PosiDirection | PosiDirection | string | 持仓多空方向 | 0:多头;1:空头;2:净;
TradePrice | TradePrice | double | 成交均价 | 
OpenPrice | OpenPrice | double | 平仓成交时的开仓均价 | 
TriggerOrderID | TriggerOrderID | string | 触发报单号 | 
SLTriggerPrice | SLTriggerPrice | double | 开仓报单成交之后的止损触发价 | 
TPTriggerPrice | TPTriggerPrice | double | 开仓报单成交之后的止盈触发价 | 
CopyProfit | CopyProfit | double | 带单盈利分配 | 
Position | Position | double | 成交时的持仓量 | 
UserID | U | string | 交易用户代码 | 
LastPriceByInsert |  | double | 插入时的最新价 | 
BidPrice1ByInsert |  | double | 插入时的买一价 | 
AskPrice1ByInsert |  | double | 插入时的卖一价 | 
Available | Available | double | 可用资金 | 
CreateTime |  | string | 创建时间 | 


## **用户出金操作请求**

+ **Request URL**

`POST /action/v1.0/SendWithdraw`


+ **Request Post Body**

```
{
	,"LocalID":"1983723"
	,"OrderID":"12376422300333"
	,"MemberID":"1983723"
	,"AccountID":"1983723"
	,"SettlementGroup":"999"
	,"OtherSettlementGroup":"999"
	,"Amount":182582.18
	,"Currency":"BTC"
	,"Remark":"my_little_tips"
}
```
+ **curl Example**

````
curl -d '{"LocalID":"1983723","OrderID":"12376422300333","MemberID":"1983723","AccountID":"1983723","SettlementGroup":"999","OtherSettlementGroup":"999","Amount":"182582.18,"Currency":"BTC","Remark":"my_little_tips"}'  -X POST  http://127.0.0.1:10000/action/v1.0/SendWithdraw
````
+ **ws Example**

````
{"RequestNo":2,"SendWithdraw":{"LocalID":"1983723","OrderID":"12376422300333","MemberID":"1983723","AccountID":"1983723","SettlementGroup":"999","OtherSettlementGroup":"999","Amount":"182582.18,"Currency":"BTC","Remark":"my_little_tips"}}
````
+ **Web rowser Example**

````
http://127.0.0.1:10000/action/v1.0/SendWithdraw?LocalID=1983723&OrderID=12376422300333&MemberID=1983723&AccountID=1983723&SettlementGroup=999&OtherSettlementGroup=999&Amount=182582.18&Currency=BTC&Remark=my_little_tips
````
+ **Request Parameters**

parameter | shortName | required | type | description | remark
-------- | -------- | -------- | -------- | -------- | --------
LocalID | LocalID | no | string | 出入金本地号 | 
OrderID | OrderID | no | string | 冻结无需填,真正出金的订单号 | 
MemberID | MemberID | yes | string | 成员代码 | 
AccountID | AccountID | yes | string | 资金账号 | 
SettlementGroup | SettlementGroup | yes | string | 转出结算组编号 | 
OtherSettlementGroup | OtherSettlementGroup | yes | string | 转入结算组编号 | 
Amount | Amount | yes | double | 发生额 | 
Currency | Currency | yes | string | 币种 | 
Remark | Remark | no | string | 备注 | 

+ **Response**

```
{
	"action": "RecvWithdraw",
	"errorCode": 0,
	"errorMsg": "Success",
	"result": [			
				{"table":"Withdraw",
				 "data":{
					,LocalID:"1983723"
					,OrderID:"12376422300333"
					,MemberID:"1983723"
					,AccountID:"1983723"
					,SettlementGroup:"999"
					,OtherSettlementGroup:"999"
					,Amount:182582.18
					,Currency:"BTC"
					,Remark:"my_little_tips"
					}
				}
				{"table":"Order",
				 "data":{
					,MemberID:"1983723"
					,TradeUnitID:"1983723"
					,AccountID:"1983723"
					,LocalID:"1983723"
					,ExchangeID:"CEX"
					,InstrumentID:"BTCUSD"
					,OrderPriceType:"1"
					,Direction:"1"
					,OffsetFlag:"1"
					,Price:182582.18
					,Volume:182582.18
					,VolumeDisplay:182582.18
					,VolumeMode:"1"
					,Cost:182582.18
					,OrderType:"1"
					,GTDTime:1657543832
					,MinVolume:182582.18
					,BusinessType:"1"
					,BusinessValue:""
					,CloseOrderID:"1983723"
					,IsCrossMargin:1
					,Remark:"my_little_tips"
					,OrderID:"12376422300333"
					,CopyMemberID:"1983723"
					,CopyOrderID:"1983723"
					,Leverage:125
					,CopyProfitRate:182582.18
					,APPID:"ios"
					,PositionID:"1983723"
					,TriggerPrice:182582.18
					,Reserve:182582.18
					,OrderStatus:"1"
					,DeriveSource:"1"
					,DeriveDetail:""
					,VolumeTraded:182582.18
					,VolumeRemain:182582.18
					,VolumeCancled:182582.18
					,InsertTime:1657543832
					,UpdateTime:1657543832
					,Priority:0
					,TimeSortNo:1657543832
					,FrontNo:1
					,PriceCurrency:"BTC"
					,FeeCurrency:"BTC"
					,ClearCurrency:"BTC"
					,FrozenMoney:182582.18
					,FrozenFee:182582.18
					,FrozenMargin:182582.18
					,Fee:0.22364
					,CloseProfit:10
					,Turnover:182582.18
					,RelatedOrderID:"1983723"
					,BusinessResult:""
					,BusinessNo:11176422300333
					,Tradable:0
					,SettlementGroup:"999"
					,PosiDirection:"1"
					,TradePrice:182582.18
					,OpenPrice:182582.18
					,TriggerOrderID:"1983723"
					,SLTriggerPrice:182582.18
					,TPTriggerPrice:182582.18
					,CopyProfit:182582.18
					,Position:182582.18
					,UserID:"1983723"
					,LastPriceByInsert:182582.18
					,BidPrice1ByInsert:182582.18
					,AskPrice1ByInsert:182582.18
					,Available:182582.18
					,CreateTime:""
					}
				}
	]
}
```

+ **Response Parameters**

Withdraw:

parameter | shortName | type | description | remark
-------- | -------- | -------- | -------- | -------- 					
LocalID | LocalID | string | 出入金本地号 | 
OrderID | OrderID | string | 冻结无需填,真正出金的订单号 | 
MemberID | MemberID | string | 成员代码 | 
AccountID | AccountID | string | 资金账号 | 
SettlementGroup | SettlementGroup | string | 转出结算组编号 | 
OtherSettlementGroup | OtherSettlementGroup | string | 转入结算组编号 | 
Amount | Amount | double | 发生额 | 
Currency | Currency | string | 币种 | 
Remark | Remark | string | 备注 | 

Order:

parameter | shortName | type | description | remark
-------- | -------- | -------- | -------- | -------- 					
MemberID | M | string | 成员代码 | 
TradeUnitID | T | string | 交易单元代码 | 
AccountID | A | string | 资金账号 | 
LocalID | L | string | 报单本地标识 | 
ExchangeID | E | string | 交易所代码 | 
InstrumentID | I | string | 标的代码 | 
OrderPriceType | PT | string | 报单价格类型 | 0:限价(手动指定Price值);1:任意价(Price按最高最低限价取值);2:最优价/对手价(Price按对方1档价取值);3:五档价(Price按对方5档价取值);4:十档价(类同上);7:最优Marker价(Price按本方1档价或其+1Tick取值);8:Price按委托额Cost和订单簿情况确定值;9:保底点差价:成交价格Price不会劣于系统设置差价成交;
Direction | D | string | 买卖方向 | 0:买;1:卖;
OffsetFlag | O | string | 开平标志 | 0:开仓;1:平仓;2:强平;3:平今;4:平昨;5:全平;6:指定订单平仓;7:指定成交平仓;8:最大平仓;
Price | P | double | 报单价格 | 
Volume | V | double | 数量 | 
VolumeDisplay | D | double | 显示数量 | 
VolumeMode | VM | string | 数量取值方式 | 0:百分比;1:绝对值;
Cost | C | double | 委托额 | 
OrderType | OT | string | 订单类型 | 0:普通订单;1:(FillAndKill/IOC)立即完成并且剩余撤销;2:(FillOrKill)立即完成全部或者最小订单要求否则全部撤销;3:需要进入订单簿被动成交,否则全部撤销(只做Maker/PostOnly);4:不需要撮合直接成交;
GTDTime | GT | double | GTD时间 | 
MinVolume | MV | double | 最小成交量 | 
BusinessType | BT | string | 业务类别 | 0:普通;1:申赎;2:行权;3:中立仓申报;4:递延交割申报;5:互换定单;6:质押;7:合并分拆;8:转股;9:回售回购;a:投票;b:融资融券;c:期权执行;d:传递被平仓订单BusinessValue;P:自定义11;Q:自定义10;R:自定义9;S:连续交易;T:自定义7;U:画线委托;V:限价市价;W:条件;X:止盈止损;Y:追踪出场;Z:指标类型;
BusinessValue | BV | string | 业务值 | 
CloseOrderID | CI | string | 平仓指定开仓的订单号 | 
IsCrossMargin | IsCrossMargin | int | 是否全仓 | 
Remark | R | string | 备注 | 
OrderID | OrderID | string | 报单系统唯一代码 | 
CopyMemberID | CopyMemberID | string | 带单员代码 | 
CopyOrderID | CopyOrderID | string | 带单员报单号 | 
Leverage | Leverage | double | 委托单杠杆倍数 | 
CopyProfitRate | CopyProfitRate | double | 带单分配比例 | 
APPID | APPID | string | 应用编号 | 
PositionID | B | string | 持仓代码 | 
TriggerPrice | TriggerPrice | double | 触发价 | 
Reserve | Reserve | double | 保留资金 | 
OrderStatus | OrderStatus | string | 报单状态 | 0:未设置状态;1:全部成交;2:部分成交未撤单;3:部分成交已撤单;4:未成交未撤单;6:无成交已撤单;
DeriveSource | DeriveSource | string | 衍生来源 | 0:正常;1:报价衍生;2:期权执行;3:组合衍生;4:场外成交强平衍生;5:场外成交大宗交易衍生;6:场外成交期转现衍生;7:场外成交只刷单衍生;8:触发订单衍生;9:止损触发订单衍生;a:止盈触发订单衍生;b:资金操作衍生;c:仓位合并衍生;d:CFD订单衍生;e:场外成交ADL衍生;f:回购手续费不能是币;
DeriveDetail | DeriveDetail | string | 衍生明细 | 
VolumeTraded | VolumeTraded | double | 成交数量 | 
VolumeRemain | VolumeRemain | double | 剩余数量 | 
VolumeCancled | VolumeCancled | double | 已经撤单数量 | 
InsertTime | InsertTime | double | 插入时间 | 
UpdateTime | UpdateTime | double | 更新时间 | 
Priority |  | int | 优先权 | 
TimeSortNo |  | double | 按时间排队的序号 | 
FrontNo | FrontNo | int | 前置编号 | 
PriceCurrency | PriceCurrency | string | 计价币种 | 
FeeCurrency | FeeCurrency | string | 手续费币种 | 
ClearCurrency | ClearCurrency | string | 清算币种 | 
FrozenMoney | FrozenMoney | double | 冻结资金 | 
FrozenFee | FrozenFee | double | 冻结手续费 | 
FrozenMargin | FrozenMargin | double | 冻结保证金 | 
Fee | Fee | double | 手续费 | 
CloseProfit | CloseProfit | double | 平仓盈亏 | 
Turnover | Turnover | double | 成交金额 | 
RelatedOrderID | RelatedOrderID | string | 关联报单号 | 
BusinessResult | BusinessResult | string | 业务执行结果 | 
BusinessNo | BusinessNo | double | 业务序列号 | 
Tradable |  | int | 是否可以参加交易 | 
SettlementGroup |  | string | 结算组编号 | 
PosiDirection | PosiDirection | string | 持仓多空方向 | 0:多头;1:空头;2:净;
TradePrice | TradePrice | double | 成交均价 | 
OpenPrice | OpenPrice | double | 平仓成交时的开仓均价 | 
TriggerOrderID | TriggerOrderID | string | 触发报单号 | 
SLTriggerPrice | SLTriggerPrice | double | 开仓报单成交之后的止损触发价 | 
TPTriggerPrice | TPTriggerPrice | double | 开仓报单成交之后的止盈触发价 | 
CopyProfit | CopyProfit | double | 带单盈利分配 | 
Position | Position | double | 成交时的持仓量 | 
UserID | U | string | 交易用户代码 | 
LastPriceByInsert |  | double | 插入时的最新价 | 
BidPrice1ByInsert |  | double | 插入时的买一价 | 
AskPrice1ByInsert |  | double | 插入时的卖一价 | 
Available | Available | double | 可用资金 | 
CreateTime |  | string | 创建时间 | 


## **用户转账操作请求**

+ **Request URL**

`POST /action/v1.0/SendTransfer`


+ **Request Post Body**

```
{
	,"LocalID":"1983723"
	,"MemberID":"1983723"
	,"AccountID":"1983723"
	,"SettlementGroup":"999"
	,"OtherAccountID":"1983723"
	,"OtherSettlementGroup":"999"
	,"OtherMemberID":"1983723"
	,"Amount":182582.18
	,"Currency":"BTC"
	,"Remark":"my_little_tips"
}
```
+ **curl Example**

````
curl -d '{"LocalID":"1983723","MemberID":"1983723","AccountID":"1983723","SettlementGroup":"999","OtherAccountID":"1983723","OtherSettlementGroup":"999","OtherMemberID":"1983723","Amount":"182582.18,"Currency":"BTC","Remark":"my_little_tips"}'  -X POST  http://127.0.0.1:10000/action/v1.0/SendTransfer
````
+ **ws Example**

````
{"RequestNo":2,"SendTransfer":{"LocalID":"1983723","MemberID":"1983723","AccountID":"1983723","SettlementGroup":"999","OtherAccountID":"1983723","OtherSettlementGroup":"999","OtherMemberID":"1983723","Amount":"182582.18,"Currency":"BTC","Remark":"my_little_tips"}}
````
+ **Web rowser Example**

````
http://127.0.0.1:10000/action/v1.0/SendTransfer?LocalID=1983723&MemberID=1983723&AccountID=1983723&SettlementGroup=999&OtherAccountID=1983723&OtherSettlementGroup=999&OtherMemberID=1983723&Amount=182582.18&Currency=BTC&Remark=my_little_tips
````
+ **Request Parameters**

parameter | shortName | required | type | description | remark
-------- | -------- | -------- | -------- | -------- | --------
LocalID | LocalID | no | string | 出入金本地号 | 
MemberID | MemberID | yes | string | 成员代码 | 
AccountID | AccountID | yes | string | 资金账号 | 
SettlementGroup | SettlementGroup | yes | string | 结算组编号 | 
OtherAccountID | OtherAccountID | no | string | 内部转账资金账号 | 
OtherSettlementGroup | OtherSettlementGroup | no | string | 内部转账结算组编号 | 
OtherMemberID | OtherMemberID | no | string | 内部转账成员代码 | 
Amount | Amount | yes | double | 发生额 | 
Currency | Currency | yes | string | 币种 | 
Remark | Remark | no | string | 备注 | 

+ **Response**

```
{
	"action": "RecvTransfer",
	"errorCode": 0,
	"errorMsg": "Success",
	"result": [			
				{"table":"Transfer",
				 "data":{
					,LocalID:"1983723"
					,MemberID:"1983723"
					,AccountID:"1983723"
					,SettlementGroup:"999"
					,OtherAccountID:"1983723"
					,OtherSettlementGroup:"999"
					,OtherMemberID:"1983723"
					,Amount:182582.18
					,Currency:"BTC"
					,Remark:"my_little_tips"
					}
				}
				{"table":"Order",
				 "data":{
					,MemberID:"1983723"
					,TradeUnitID:"1983723"
					,AccountID:"1983723"
					,LocalID:"1983723"
					,ExchangeID:"CEX"
					,InstrumentID:"BTCUSD"
					,OrderPriceType:"1"
					,Direction:"1"
					,OffsetFlag:"1"
					,Price:182582.18
					,Volume:182582.18
					,VolumeDisplay:182582.18
					,VolumeMode:"1"
					,Cost:182582.18
					,OrderType:"1"
					,GTDTime:1657543832
					,MinVolume:182582.18
					,BusinessType:"1"
					,BusinessValue:""
					,CloseOrderID:"1983723"
					,IsCrossMargin:1
					,Remark:"my_little_tips"
					,OrderID:"12376422300333"
					,CopyMemberID:"1983723"
					,CopyOrderID:"1983723"
					,Leverage:125
					,CopyProfitRate:182582.18
					,APPID:"ios"
					,PositionID:"1983723"
					,TriggerPrice:182582.18
					,Reserve:182582.18
					,OrderStatus:"1"
					,DeriveSource:"1"
					,DeriveDetail:""
					,VolumeTraded:182582.18
					,VolumeRemain:182582.18
					,VolumeCancled:182582.18
					,InsertTime:1657543832
					,UpdateTime:1657543832
					,Priority:0
					,TimeSortNo:1657543832
					,FrontNo:1
					,PriceCurrency:"BTC"
					,FeeCurrency:"BTC"
					,ClearCurrency:"BTC"
					,FrozenMoney:182582.18
					,FrozenFee:182582.18
					,FrozenMargin:182582.18
					,Fee:0.22364
					,CloseProfit:10
					,Turnover:182582.18
					,RelatedOrderID:"1983723"
					,BusinessResult:""
					,BusinessNo:11176422300333
					,Tradable:0
					,SettlementGroup:"999"
					,PosiDirection:"1"
					,TradePrice:182582.18
					,OpenPrice:182582.18
					,TriggerOrderID:"1983723"
					,SLTriggerPrice:182582.18
					,TPTriggerPrice:182582.18
					,CopyProfit:182582.18
					,Position:182582.18
					,UserID:"1983723"
					,LastPriceByInsert:182582.18
					,BidPrice1ByInsert:182582.18
					,AskPrice1ByInsert:182582.18
					,Available:182582.18
					,CreateTime:""
					}
				}
	]
}
```

+ **Response Parameters**

Transfer:

parameter | shortName | type | description | remark
-------- | -------- | -------- | -------- | -------- 					
LocalID | LocalID | string | 出入金本地号 | 
MemberID | MemberID | string | 成员代码 | 
AccountID | AccountID | string | 资金账号 | 
SettlementGroup | SettlementGroup | string | 结算组编号 | 
OtherAccountID | OtherAccountID | string | 内部转账资金账号 | 
OtherSettlementGroup | OtherSettlementGroup | string | 内部转账结算组编号 | 
OtherMemberID | OtherMemberID | string | 内部转账成员代码 | 
Amount | Amount | double | 发生额 | 
Currency | Currency | string | 币种 | 
Remark | Remark | string | 备注 | 

Order:

parameter | shortName | type | description | remark
-------- | -------- | -------- | -------- | -------- 					
MemberID | M | string | 成员代码 | 
TradeUnitID | T | string | 交易单元代码 | 
AccountID | A | string | 资金账号 | 
LocalID | L | string | 报单本地标识 | 
ExchangeID | E | string | 交易所代码 | 
InstrumentID | I | string | 标的代码 | 
OrderPriceType | PT | string | 报单价格类型 | 0:限价(手动指定Price值);1:任意价(Price按最高最低限价取值);2:最优价/对手价(Price按对方1档价取值);3:五档价(Price按对方5档价取值);4:十档价(类同上);7:最优Marker价(Price按本方1档价或其+1Tick取值);8:Price按委托额Cost和订单簿情况确定值;9:保底点差价:成交价格Price不会劣于系统设置差价成交;
Direction | D | string | 买卖方向 | 0:买;1:卖;
OffsetFlag | O | string | 开平标志 | 0:开仓;1:平仓;2:强平;3:平今;4:平昨;5:全平;6:指定订单平仓;7:指定成交平仓;8:最大平仓;
Price | P | double | 报单价格 | 
Volume | V | double | 数量 | 
VolumeDisplay | D | double | 显示数量 | 
VolumeMode | VM | string | 数量取值方式 | 0:百分比;1:绝对值;
Cost | C | double | 委托额 | 
OrderType | OT | string | 订单类型 | 0:普通订单;1:(FillAndKill/IOC)立即完成并且剩余撤销;2:(FillOrKill)立即完成全部或者最小订单要求否则全部撤销;3:需要进入订单簿被动成交,否则全部撤销(只做Maker/PostOnly);4:不需要撮合直接成交;
GTDTime | GT | double | GTD时间 | 
MinVolume | MV | double | 最小成交量 | 
BusinessType | BT | string | 业务类别 | 0:普通;1:申赎;2:行权;3:中立仓申报;4:递延交割申报;5:互换定单;6:质押;7:合并分拆;8:转股;9:回售回购;a:投票;b:融资融券;c:期权执行;d:传递被平仓订单BusinessValue;P:自定义11;Q:自定义10;R:自定义9;S:连续交易;T:自定义7;U:画线委托;V:限价市价;W:条件;X:止盈止损;Y:追踪出场;Z:指标类型;
BusinessValue | BV | string | 业务值 | 
CloseOrderID | CI | string | 平仓指定开仓的订单号 | 
IsCrossMargin | IsCrossMargin | int | 是否全仓 | 
Remark | R | string | 备注 | 
OrderID | OrderID | string | 报单系统唯一代码 | 
CopyMemberID | CopyMemberID | string | 带单员代码 | 
CopyOrderID | CopyOrderID | string | 带单员报单号 | 
Leverage | Leverage | double | 委托单杠杆倍数 | 
CopyProfitRate | CopyProfitRate | double | 带单分配比例 | 
APPID | APPID | string | 应用编号 | 
PositionID | B | string | 持仓代码 | 
TriggerPrice | TriggerPrice | double | 触发价 | 
Reserve | Reserve | double | 保留资金 | 
OrderStatus | OrderStatus | string | 报单状态 | 0:未设置状态;1:全部成交;2:部分成交未撤单;3:部分成交已撤单;4:未成交未撤单;6:无成交已撤单;
DeriveSource | DeriveSource | string | 衍生来源 | 0:正常;1:报价衍生;2:期权执行;3:组合衍生;4:场外成交强平衍生;5:场外成交大宗交易衍生;6:场外成交期转现衍生;7:场外成交只刷单衍生;8:触发订单衍生;9:止损触发订单衍生;a:止盈触发订单衍生;b:资金操作衍生;c:仓位合并衍生;d:CFD订单衍生;e:场外成交ADL衍生;f:回购手续费不能是币;
DeriveDetail | DeriveDetail | string | 衍生明细 | 
VolumeTraded | VolumeTraded | double | 成交数量 | 
VolumeRemain | VolumeRemain | double | 剩余数量 | 
VolumeCancled | VolumeCancled | double | 已经撤单数量 | 
InsertTime | InsertTime | double | 插入时间 | 
UpdateTime | UpdateTime | double | 更新时间 | 
Priority |  | int | 优先权 | 
TimeSortNo |  | double | 按时间排队的序号 | 
FrontNo | FrontNo | int | 前置编号 | 
PriceCurrency | PriceCurrency | string | 计价币种 | 
FeeCurrency | FeeCurrency | string | 手续费币种 | 
ClearCurrency | ClearCurrency | string | 清算币种 | 
FrozenMoney | FrozenMoney | double | 冻结资金 | 
FrozenFee | FrozenFee | double | 冻结手续费 | 
FrozenMargin | FrozenMargin | double | 冻结保证金 | 
Fee | Fee | double | 手续费 | 
CloseProfit | CloseProfit | double | 平仓盈亏 | 
Turnover | Turnover | double | 成交金额 | 
RelatedOrderID | RelatedOrderID | string | 关联报单号 | 
BusinessResult | BusinessResult | string | 业务执行结果 | 
BusinessNo | BusinessNo | double | 业务序列号 | 
Tradable |  | int | 是否可以参加交易 | 
SettlementGroup |  | string | 结算组编号 | 
PosiDirection | PosiDirection | string | 持仓多空方向 | 0:多头;1:空头;2:净;
TradePrice | TradePrice | double | 成交均价 | 
OpenPrice | OpenPrice | double | 平仓成交时的开仓均价 | 
TriggerOrderID | TriggerOrderID | string | 触发报单号 | 
SLTriggerPrice | SLTriggerPrice | double | 开仓报单成交之后的止损触发价 | 
TPTriggerPrice | TPTriggerPrice | double | 开仓报单成交之后的止盈触发价 | 
CopyProfit | CopyProfit | double | 带单盈利分配 | 
Position | Position | double | 成交时的持仓量 | 
UserID | U | string | 交易用户代码 | 
LastPriceByInsert |  | double | 插入时的最新价 | 
BidPrice1ByInsert |  | double | 插入时的买一价 | 
AskPrice1ByInsert |  | double | 插入时的卖一价 | 
Available | Available | double | 可用资金 | 
CreateTime |  | string | 创建时间 | 


## **注册用户请求**

+ **Request URL**

`POST /action/v1.0/SendUserRegister`


+ **Request Post Body**

```
{
	,"UserID":"1983723"
	,"Password":""
	,"UserProductID":"1983723"
	,"MacAddress":""
	,"ClientIPAddress":""
	,"HDSerialID":"1983723"
	,"AuthCode":""
	,"APPID":"ios"
	,"Token":""
	,"Remark":"my_little_tips"
	,"APIID":"1983723"
	,"LoginTime":1657543832
	,"ExpireTime":1657543832
	,"IPAddress":""
	,"MaxLocalID":"1983723"
	,"SessionNo":1033201232
	,"FrontNo":1
	,"AccessLimit":0
	,"UserType":"1"
	,"MemberID":"1983723"
	,"LimitAccesses":0
	,"SinkType":"1"
	,"InsertTime":1657543832
	,"UpdateTime":1657543832
}
```
+ **curl Example**

````
curl -d '{"UserID":"1983723","Password":"","UserProductID":"1983723","MacAddress":"","ClientIPAddress":"","HDSerialID":"1983723","AuthCode":"","APPID":"ios","Token":"","Remark":"my_little_tips","APIID":"1983723","LoginTime":"1657543832,"ExpireTime":"1657543832,"IPAddress":"","MaxLocalID":"1983723","SessionNo":"1033201232,"FrontNo":"1,"AccessLimit":"0,"UserType":"1","MemberID":"1983723","LimitAccesses":"0,"SinkType":"1","InsertTime":"1657543832,"UpdateTime":"1657543832}'  -X POST  http://127.0.0.1:10000/action/v1.0/SendUserRegister
````
+ **ws Example**

````
{"RequestNo":2,"SendUserRegister":{"UserID":"1983723","Password":"","UserProductID":"1983723","MacAddress":"","ClientIPAddress":"","HDSerialID":"1983723","AuthCode":"","APPID":"ios","Token":"","Remark":"my_little_tips","APIID":"1983723","LoginTime":"1657543832,"ExpireTime":"1657543832,"IPAddress":"","MaxLocalID":"1983723","SessionNo":"1033201232,"FrontNo":"1,"AccessLimit":"0,"UserType":"1","MemberID":"1983723","LimitAccesses":"0,"SinkType":"1","InsertTime":"1657543832,"UpdateTime":"1657543832}}
````
+ **Web rowser Example**

````
http://127.0.0.1:10000/action/v1.0/SendUserRegister?UserID=1983723&Password=&UserProductID=1983723&MacAddress=&ClientIPAddress=&HDSerialID=1983723&AuthCode=&APPID=ios&Token=&Remark=my_little_tips&APIID=1983723&LoginTime=1657543832&ExpireTime=1657543832&IPAddress=&MaxLocalID=1983723&SessionNo=1033201232&FrontNo=1&AccessLimit=0&UserType=1&MemberID=1983723&LimitAccesses=0&SinkType=1&InsertTime=1657543832&UpdateTime=1657543832
````
+ **Request Parameters**

parameter | shortName | required | type | description | remark
-------- | -------- | -------- | -------- | -------- | --------
UserID | UserID | yes | string | 交易用户代码 | 
Password | Password | yes | string | 密码 | 
UserProductID | UserProductID | no | string | 用户端产品信息 | 
MacAddress | MacAddress | no | string | Mac地址 | 
ClientIPAddress | ClientIPAddress | no | string | 终端IP地址 | 
HDSerialID | HDSerialID | no | string | 硬盘序列号 | 
AuthCode | AuthCode | no | string | 授权编码 | 
APPID | APPID | no | string | 应用编号 | 
Token | Token | no | string | 令牌 | 
Remark | Remark | no | string | 备注 | 
APIID | APIID | no | string | 接口端产品信息 | 
LoginTime | LoginTime | no | double | 登录时间 | 
ExpireTime | ExpireTime | no | double | 失效时间 | 
IPAddress | IPAddress | no | string | 允许登录的IP地址 | 
MaxLocalID | MaxLocalID | no | string | 最大本地标识 | 
SessionNo | SessionNo | no | int | 会话编号 | 
FrontNo | FrontNo | no | int | 前置编号 | 
AccessLimit | AccessLimit | no | int | 每秒访问的次数限制 | 
UserType | UserType | no | string | 用户类型 | 1:普通用户(订阅自己/可下自己订单);2:观察用户OB(订阅其他用户/不可下单);5:超级管理员(订阅所有/可给所有用户下单/可爆仓);
MemberID | MemberID | no | string | 成员代码 | 
LimitAccesses | LimitAccesses | no | int | 该秒已经访问的次数 | 
SinkType | SinkType | no | string | 下游系统同步类型 | 0:同步到数据库;1:同步到Redis不到数据库;2:不同步;
InsertTime | InsertTime | no | double | 插入时间 | 
UpdateTime | UpdateTime | no | double | 更新时间 | 

+ **Response**

```
{
	"action": "RecvUserRegister",
	"errorCode": 0,
	"errorMsg": "Success",
	"result": [			
				{"table":"UserSession",
				 "data":{
					,UserID:"1983723"
					,Password:""
					,UserProductID:"1983723"
					,MacAddress:""
					,ClientIPAddress:""
					,HDSerialID:"1983723"
					,AuthCode:""
					,APPID:"ios"
					,Token:""
					,Remark:"my_little_tips"
					,APIID:"1983723"
					,LoginTime:1657543832
					,ExpireTime:1657543832
					,IPAddress:""
					,MaxLocalID:"1983723"
					,SessionNo:1033201232
					,FrontNo:1
					,AccessLimit:0
					,UserType:"1"
					,MemberID:"1983723"
					,LimitAccesses:0
					,SinkType:"1"
					,InsertTime:1657543832
					,UpdateTime:1657543832
					}
				}
	]
}
```

+ **Response Parameters**

UserSession:

parameter | shortName | type | description | remark
-------- | -------- | -------- | -------- | -------- 					
UserID | UserID | string | 交易用户代码 | 
Password | Password | string | 密码 | 
UserProductID | UserProductID | string | 用户端产品信息 | 
MacAddress | MacAddress | string | Mac地址 | 
ClientIPAddress | ClientIPAddress | string | 终端IP地址 | 
HDSerialID | HDSerialID | string | 硬盘序列号 | 
AuthCode | AuthCode | string | 授权编码 | 
APPID | APPID | string | 应用编号 | 
Token | Token | string | 令牌 | 
Remark | Remark | string | 备注 | 
APIID | APIID | string | 接口端产品信息 | 
LoginTime | LoginTime | double | 登录时间 | 
ExpireTime | ExpireTime | double | 失效时间 | 
IPAddress | IPAddress | string | 允许登录的IP地址 | 
MaxLocalID | MaxLocalID | string | 最大本地标识 | 
SessionNo | SessionNo | int | 会话编号 | 
FrontNo | FrontNo | int | 前置编号 | 
AccessLimit | AccessLimit | int | 每秒访问的次数限制 | 
UserType | UserType | string | 用户类型 | 1:普通用户(订阅自己/可下自己订单);2:观察用户OB(订阅其他用户/不可下单);5:超级管理员(订阅所有/可给所有用户下单/可爆仓);
MemberID | MemberID | string | 成员代码 | 
LimitAccesses | LimitAccesses | int | 该秒已经访问的次数 | 
SinkType | SinkType | string | 下游系统同步类型 | 0:同步到数据库;1:同步到Redis不到数据库;2:不同步;
InsertTime | InsertTime | double | 插入时间 | 
UpdateTime | UpdateTime | double | 更新时间 | 


## **反向开仓录入请求**

+ **Request URL**

`POST /action/v1.0/SendPositionReverse`


+ **Request Post Body**

```
{
	,"LocalID":"1983723"
	,"MemberID":"1983723"
	,"PositionID":"1983723"
	,"Volume":182582.18
	,"Remark":"my_little_tips"
	,"CopyMemberID":"1983723"
	,"APPID":"ios"
}
```
+ **curl Example**

````
curl -d '{"LocalID":"1983723","MemberID":"1983723","PositionID":"1983723","Volume":"182582.18,"Remark":"my_little_tips","CopyMemberID":"1983723","APPID":"ios"}'  -X POST  http://127.0.0.1:10000/action/v1.0/SendPositionReverse
````
+ **ws Example**

````
{"RequestNo":2,"SendPositionReverse":{"LocalID":"1983723","MemberID":"1983723","PositionID":"1983723","Volume":"182582.18,"Remark":"my_little_tips","CopyMemberID":"1983723","APPID":"ios"}}
````
+ **Web rowser Example**

````
http://127.0.0.1:10000/action/v1.0/SendPositionReverse?LocalID=1983723&MemberID=1983723&PositionID=1983723&Volume=182582.18&Remark=my_little_tips&CopyMemberID=1983723&APPID=ios
````
+ **Request Parameters**

parameter | shortName | required | type | description | remark
-------- | -------- | -------- | -------- | -------- | --------
LocalID | LocalID | no | string | 操作本地号 | 
MemberID | MemberID | yes | string | 成员代码 | 
PositionID | PositionID | yes | string | 持仓代码 | 
Volume | V | no | double | 反向开仓数量 | 
Remark | R | no | string | 备注 | 
CopyMemberID | CopyMemberID | no | string | 带单员代码 | 
APPID | APPID | no | string | 应用编号 | 

+ **Response**

```
{
	"action": "RecvPositionReverse",
	"errorCode": 0,
	"errorMsg": "Success",
	"result": [			
				{"table":"Order",
				 "data":{
					,MemberID:"1983723"
					,TradeUnitID:"1983723"
					,AccountID:"1983723"
					,LocalID:"1983723"
					,ExchangeID:"CEX"
					,InstrumentID:"BTCUSD"
					,OrderPriceType:"1"
					,Direction:"1"
					,OffsetFlag:"1"
					,Price:182582.18
					,Volume:182582.18
					,VolumeDisplay:182582.18
					,VolumeMode:"1"
					,Cost:182582.18
					,OrderType:"1"
					,GTDTime:1657543832
					,MinVolume:182582.18
					,BusinessType:"1"
					,BusinessValue:""
					,CloseOrderID:"1983723"
					,IsCrossMargin:1
					,Remark:"my_little_tips"
					,OrderID:"12376422300333"
					,CopyMemberID:"1983723"
					,CopyOrderID:"1983723"
					,Leverage:125
					,CopyProfitRate:182582.18
					,APPID:"ios"
					,PositionID:"1983723"
					,TriggerPrice:182582.18
					,Reserve:182582.18
					,OrderStatus:"1"
					,DeriveSource:"1"
					,DeriveDetail:""
					,VolumeTraded:182582.18
					,VolumeRemain:182582.18
					,VolumeCancled:182582.18
					,InsertTime:1657543832
					,UpdateTime:1657543832
					,Priority:0
					,TimeSortNo:1657543832
					,FrontNo:1
					,PriceCurrency:"BTC"
					,FeeCurrency:"BTC"
					,ClearCurrency:"BTC"
					,FrozenMoney:182582.18
					,FrozenFee:182582.18
					,FrozenMargin:182582.18
					,Fee:0.22364
					,CloseProfit:10
					,Turnover:182582.18
					,RelatedOrderID:"1983723"
					,BusinessResult:""
					,BusinessNo:11176422300333
					,Tradable:0
					,SettlementGroup:"999"
					,PosiDirection:"1"
					,TradePrice:182582.18
					,OpenPrice:182582.18
					,TriggerOrderID:"1983723"
					,SLTriggerPrice:182582.18
					,TPTriggerPrice:182582.18
					,CopyProfit:182582.18
					,Position:182582.18
					,UserID:"1983723"
					,LastPriceByInsert:182582.18
					,BidPrice1ByInsert:182582.18
					,AskPrice1ByInsert:182582.18
					,Available:182582.18
					,CreateTime:""
					}
				}
	]
}
```

+ **Response Parameters**

Order:

parameter | shortName | type | description | remark
-------- | -------- | -------- | -------- | -------- 					
MemberID | M | string | 成员代码 | 
TradeUnitID | T | string | 交易单元代码 | 
AccountID | A | string | 资金账号 | 
LocalID | L | string | 报单本地标识 | 
ExchangeID | E | string | 交易所代码 | 
InstrumentID | I | string | 标的代码 | 
OrderPriceType | PT | string | 报单价格类型 | 0:限价(手动指定Price值);1:任意价(Price按最高最低限价取值);2:最优价/对手价(Price按对方1档价取值);3:五档价(Price按对方5档价取值);4:十档价(类同上);7:最优Marker价(Price按本方1档价或其+1Tick取值);8:Price按委托额Cost和订单簿情况确定值;9:保底点差价:成交价格Price不会劣于系统设置差价成交;
Direction | D | string | 买卖方向 | 0:买;1:卖;
OffsetFlag | O | string | 开平标志 | 0:开仓;1:平仓;2:强平;3:平今;4:平昨;5:全平;6:指定订单平仓;7:指定成交平仓;8:最大平仓;
Price | P | double | 报单价格 | 
Volume | V | double | 数量 | 
VolumeDisplay | D | double | 显示数量 | 
VolumeMode | VM | string | 数量取值方式 | 0:百分比;1:绝对值;
Cost | C | double | 委托额 | 
OrderType | OT | string | 订单类型 | 0:普通订单;1:(FillAndKill/IOC)立即完成并且剩余撤销;2:(FillOrKill)立即完成全部或者最小订单要求否则全部撤销;3:需要进入订单簿被动成交,否则全部撤销(只做Maker/PostOnly);4:不需要撮合直接成交;
GTDTime | GT | double | GTD时间 | 
MinVolume | MV | double | 最小成交量 | 
BusinessType | BT | string | 业务类别 | 0:普通;1:申赎;2:行权;3:中立仓申报;4:递延交割申报;5:互换定单;6:质押;7:合并分拆;8:转股;9:回售回购;a:投票;b:融资融券;c:期权执行;d:传递被平仓订单BusinessValue;P:自定义11;Q:自定义10;R:自定义9;S:连续交易;T:自定义7;U:画线委托;V:限价市价;W:条件;X:止盈止损;Y:追踪出场;Z:指标类型;
BusinessValue | BV | string | 业务值 | 
CloseOrderID | CI | string | 平仓指定开仓的订单号 | 
IsCrossMargin | IsCrossMargin | int | 是否全仓 | 
Remark | R | string | 备注 | 
OrderID | OrderID | string | 报单系统唯一代码 | 
CopyMemberID | CopyMemberID | string | 带单员代码 | 
CopyOrderID | CopyOrderID | string | 带单员报单号 | 
Leverage | Leverage | double | 委托单杠杆倍数 | 
CopyProfitRate | CopyProfitRate | double | 带单分配比例 | 
APPID | APPID | string | 应用编号 | 
PositionID | B | string | 持仓代码 | 
TriggerPrice | TriggerPrice | double | 触发价 | 
Reserve | Reserve | double | 保留资金 | 
OrderStatus | OrderStatus | string | 报单状态 | 0:未设置状态;1:全部成交;2:部分成交未撤单;3:部分成交已撤单;4:未成交未撤单;6:无成交已撤单;
DeriveSource | DeriveSource | string | 衍生来源 | 0:正常;1:报价衍生;2:期权执行;3:组合衍生;4:场外成交强平衍生;5:场外成交大宗交易衍生;6:场外成交期转现衍生;7:场外成交只刷单衍生;8:触发订单衍生;9:止损触发订单衍生;a:止盈触发订单衍生;b:资金操作衍生;c:仓位合并衍生;d:CFD订单衍生;e:场外成交ADL衍生;f:回购手续费不能是币;
DeriveDetail | DeriveDetail | string | 衍生明细 | 
VolumeTraded | VolumeTraded | double | 成交数量 | 
VolumeRemain | VolumeRemain | double | 剩余数量 | 
VolumeCancled | VolumeCancled | double | 已经撤单数量 | 
InsertTime | InsertTime | double | 插入时间 | 
UpdateTime | UpdateTime | double | 更新时间 | 
Priority |  | int | 优先权 | 
TimeSortNo |  | double | 按时间排队的序号 | 
FrontNo | FrontNo | int | 前置编号 | 
PriceCurrency | PriceCurrency | string | 计价币种 | 
FeeCurrency | FeeCurrency | string | 手续费币种 | 
ClearCurrency | ClearCurrency | string | 清算币种 | 
FrozenMoney | FrozenMoney | double | 冻结资金 | 
FrozenFee | FrozenFee | double | 冻结手续费 | 
FrozenMargin | FrozenMargin | double | 冻结保证金 | 
Fee | Fee | double | 手续费 | 
CloseProfit | CloseProfit | double | 平仓盈亏 | 
Turnover | Turnover | double | 成交金额 | 
RelatedOrderID | RelatedOrderID | string | 关联报单号 | 
BusinessResult | BusinessResult | string | 业务执行结果 | 
BusinessNo | BusinessNo | double | 业务序列号 | 
Tradable |  | int | 是否可以参加交易 | 
SettlementGroup |  | string | 结算组编号 | 
PosiDirection | PosiDirection | string | 持仓多空方向 | 0:多头;1:空头;2:净;
TradePrice | TradePrice | double | 成交均价 | 
OpenPrice | OpenPrice | double | 平仓成交时的开仓均价 | 
TriggerOrderID | TriggerOrderID | string | 触发报单号 | 
SLTriggerPrice | SLTriggerPrice | double | 开仓报单成交之后的止损触发价 | 
TPTriggerPrice | TPTriggerPrice | double | 开仓报单成交之后的止盈触发价 | 
CopyProfit | CopyProfit | double | 带单盈利分配 | 
Position | Position | double | 成交时的持仓量 | 
UserID | U | string | 交易用户代码 | 
LastPriceByInsert |  | double | 插入时的最新价 | 
BidPrice1ByInsert |  | double | 插入时的买一价 | 
AskPrice1ByInsert |  | double | 插入时的卖一价 | 
Available | Available | double | 可用资金 | 
CreateTime |  | string | 创建时间 | 


## **避免自成交报单录入请求**

+ **Request URL**

`POST /action/v1.0/SendCancelOrderInsert`


+ **Request Post Body**

```
{
	,"MemberID":"1983723"
	,"TradeUnitID":"1983723"
	,"AccountID":"1983723"
	,"LocalID":"1983723"
	,"ExchangeID":"CEX"
	,"InstrumentID":"BTCUSD"
	,"OrderPriceType":"1"
	,"Direction":"1"
	,"OffsetFlag":"1"
	,"Price":182582.18
	,"Volume":182582.18
	,"VolumeDisplay":182582.18
	,"VolumeMode":"1"
	,"Cost":182582.18
	,"OrderType":"1"
	,"GTDTime":1657543832
	,"MinVolume":182582.18
	,"BusinessType":"1"
	,"BusinessValue":""
	,"CloseOrderID":"1983723"
	,"IsCrossMargin":1
	,"Remark":"my_little_tips"
	,"OrderID":"12376422300333"
	,"CopyMemberID":"1983723"
	,"CopyOrderID":"1983723"
	,"Leverage":125
	,"CopyProfitRate":182582.18
	,"APPID":"ios"
	,"PositionID":"1983723"
	,"TriggerPrice":182582.18
	,"Reserve":182582.18
}
```
+ **curl Example**

````
curl -d '{"MemberID":"1983723","TradeUnitID":"1983723","AccountID":"1983723","LocalID":"1983723","ExchangeID":"CEX","InstrumentID":"BTCUSD","OrderPriceType":"1","Direction":"1","OffsetFlag":"1","Price":"182582.18,"Volume":"182582.18,"VolumeDisplay":"182582.18,"VolumeMode":"1","Cost":"182582.18,"OrderType":"1","GTDTime":"1657543832,"MinVolume":"182582.18,"BusinessType":"1","BusinessValue":"","CloseOrderID":"1983723","IsCrossMargin":"1,"Remark":"my_little_tips","OrderID":"12376422300333","CopyMemberID":"1983723","CopyOrderID":"1983723","Leverage":"125,"CopyProfitRate":"182582.18,"APPID":"ios","PositionID":"1983723","TriggerPrice":"182582.18,"Reserve":"182582.18}'  -X POST  http://127.0.0.1:10000/action/v1.0/SendCancelOrderInsert
````
+ **ws Example**

````
{"RequestNo":2,"SendCancelOrderInsert":{"MemberID":"1983723","TradeUnitID":"1983723","AccountID":"1983723","LocalID":"1983723","ExchangeID":"CEX","InstrumentID":"BTCUSD","OrderPriceType":"1","Direction":"1","OffsetFlag":"1","Price":"182582.18,"Volume":"182582.18,"VolumeDisplay":"182582.18,"VolumeMode":"1","Cost":"182582.18,"OrderType":"1","GTDTime":"1657543832,"MinVolume":"182582.18,"BusinessType":"1","BusinessValue":"","CloseOrderID":"1983723","IsCrossMargin":"1,"Remark":"my_little_tips","OrderID":"12376422300333","CopyMemberID":"1983723","CopyOrderID":"1983723","Leverage":"125,"CopyProfitRate":"182582.18,"APPID":"ios","PositionID":"1983723","TriggerPrice":"182582.18,"Reserve":"182582.18}}
````
+ **Web rowser Example**

````
http://127.0.0.1:10000/action/v1.0/SendCancelOrderInsert?MemberID=1983723&TradeUnitID=1983723&AccountID=1983723&LocalID=1983723&ExchangeID=CEX&InstrumentID=BTCUSD&OrderPriceType=1&Direction=1&OffsetFlag=1&Price=182582.18&Volume=182582.18&VolumeDisplay=182582.18&VolumeMode=1&Cost=182582.18&OrderType=1&GTDTime=1657543832&MinVolume=182582.18&BusinessType=1&BusinessValue=&CloseOrderID=1983723&IsCrossMargin=1&Remark=my_little_tips&OrderID=12376422300333&CopyMemberID=1983723&CopyOrderID=1983723&Leverage=125&CopyProfitRate=182582.18&APPID=ios&PositionID=1983723&TriggerPrice=182582.18&Reserve=182582.18
````
+ **Request Parameters**

parameter | shortName | required | type | description | remark
-------- | -------- | -------- | -------- | -------- | --------
MemberID | M | no | string | 成员代码 | 
TradeUnitID | T | no | string | 交易单元代码 | 
AccountID | A | no | string | 资金账号 | 
LocalID | L | no | string | 报单本地标识 | 
ExchangeID | E | yes | string | 交易所代码 | 
InstrumentID | I | yes | string | 标的代码 | 
OrderPriceType | PT | yes | string | 报单价格类型 | 0:限价(手动指定Price值);1:任意价(Price按最高最低限价取值);2:最优价/对手价(Price按对方1档价取值);3:五档价(Price按对方5档价取值);4:十档价(类同上);7:最优Marker价(Price按本方1档价或其+1Tick取值);8:Price按委托额Cost和订单簿情况确定值;9:保底点差价:成交价格Price不会劣于系统设置差价成交;
Direction | D | yes | string | 买卖方向 | 0:买;1:卖;
OffsetFlag | O | no | string | 开平标志 | 0:开仓;1:平仓;2:强平;3:平今;4:平昨;5:全平;6:指定订单平仓;7:指定成交平仓;8:最大平仓;
Price | P | no | double | 报单价格 | 
Volume | V | no | double | 数量 | 
VolumeDisplay | D | no | double | 显示数量 | 
VolumeMode | VM | no | string | 数量取值方式 | 0:百分比;1:绝对值;
Cost | C | no | double | 委托额 | 
OrderType | OT | no | string | 订单类型 | 0:普通订单;1:(FillAndKill/IOC)立即完成并且剩余撤销;2:(FillOrKill)立即完成全部或者最小订单要求否则全部撤销;3:需要进入订单簿被动成交,否则全部撤销(只做Maker/PostOnly);4:不需要撮合直接成交;
GTDTime | GT | no | double | GTD时间 | 
MinVolume | MV | no | double | 最小成交量 | 
BusinessType | BT | no | string | 业务类别 | 0:普通;1:申赎;2:行权;3:中立仓申报;4:递延交割申报;5:互换定单;6:质押;7:合并分拆;8:转股;9:回售回购;a:投票;b:融资融券;c:期权执行;d:传递被平仓订单BusinessValue;P:自定义11;Q:自定义10;R:自定义9;S:连续交易;T:自定义7;U:画线委托;V:限价市价;W:条件;X:止盈止损;Y:追踪出场;Z:指标类型;
BusinessValue | BV | no | string | 业务值 | 
CloseOrderID | CI | no | string | 平仓指定开仓的订单号 | 
IsCrossMargin | IsCrossMargin | no | int | 是否全仓 | 
Remark | R | no | string | 备注 | 
OrderID | OrderID | no | string | 报单系统唯一代码 | 
CopyMemberID | CopyMemberID | no | string | 带单员代码 | 
CopyOrderID | CopyOrderID | no | string | 带单员报单号 | 
Leverage | Leverage | no | double | 委托单杠杆倍数 | 
CopyProfitRate | CopyProfitRate | no | double | 带单分配比例 | 
APPID | APPID | no | string | 应用编号 | 
PositionID | B | no | string | 持仓代码 | 
TriggerPrice | TriggerPrice | no | double | 触发价 | 
Reserve | Reserve | no | double | 保留资金 | 

+ **Response**

```
{
	"action": "RecvCancelOrderInsert",
	"errorCode": 0,
	"errorMsg": "Success",
	"result": [			
				{"table":"OrderInsert",
				 "data":{
					,MemberID:"1983723"
					,TradeUnitID:"1983723"
					,AccountID:"1983723"
					,LocalID:"1983723"
					,ExchangeID:"CEX"
					,InstrumentID:"BTCUSD"
					,OrderPriceType:"1"
					,Direction:"1"
					,OffsetFlag:"1"
					,Price:182582.18
					,Volume:182582.18
					,VolumeDisplay:182582.18
					,VolumeMode:"1"
					,Cost:182582.18
					,OrderType:"1"
					,GTDTime:1657543832
					,MinVolume:182582.18
					,BusinessType:"1"
					,BusinessValue:""
					,CloseOrderID:"1983723"
					,IsCrossMargin:1
					,Remark:"my_little_tips"
					,OrderID:"12376422300333"
					,CopyMemberID:"1983723"
					,CopyOrderID:"1983723"
					,Leverage:125
					,CopyProfitRate:182582.18
					,APPID:"ios"
					,PositionID:"1983723"
					,TriggerPrice:182582.18
					,Reserve:182582.18
					}
				}
				{"table":"Order",
				 "data":{
					,MemberID:"1983723"
					,TradeUnitID:"1983723"
					,AccountID:"1983723"
					,LocalID:"1983723"
					,ExchangeID:"CEX"
					,InstrumentID:"BTCUSD"
					,OrderPriceType:"1"
					,Direction:"1"
					,OffsetFlag:"1"
					,Price:182582.18
					,Volume:182582.18
					,VolumeDisplay:182582.18
					,VolumeMode:"1"
					,Cost:182582.18
					,OrderType:"1"
					,GTDTime:1657543832
					,MinVolume:182582.18
					,BusinessType:"1"
					,BusinessValue:""
					,CloseOrderID:"1983723"
					,IsCrossMargin:1
					,Remark:"my_little_tips"
					,OrderID:"12376422300333"
					,CopyMemberID:"1983723"
					,CopyOrderID:"1983723"
					,Leverage:125
					,CopyProfitRate:182582.18
					,APPID:"ios"
					,PositionID:"1983723"
					,TriggerPrice:182582.18
					,Reserve:182582.18
					,OrderStatus:"1"
					,DeriveSource:"1"
					,DeriveDetail:""
					,VolumeTraded:182582.18
					,VolumeRemain:182582.18
					,VolumeCancled:182582.18
					,InsertTime:1657543832
					,UpdateTime:1657543832
					,Priority:0
					,TimeSortNo:1657543832
					,FrontNo:1
					,PriceCurrency:"BTC"
					,FeeCurrency:"BTC"
					,ClearCurrency:"BTC"
					,FrozenMoney:182582.18
					,FrozenFee:182582.18
					,FrozenMargin:182582.18
					,Fee:0.22364
					,CloseProfit:10
					,Turnover:182582.18
					,RelatedOrderID:"1983723"
					,BusinessResult:""
					,BusinessNo:11176422300333
					,Tradable:0
					,SettlementGroup:"999"
					,PosiDirection:"1"
					,TradePrice:182582.18
					,OpenPrice:182582.18
					,TriggerOrderID:"1983723"
					,SLTriggerPrice:182582.18
					,TPTriggerPrice:182582.18
					,CopyProfit:182582.18
					,Position:182582.18
					,UserID:"1983723"
					,LastPriceByInsert:182582.18
					,BidPrice1ByInsert:182582.18
					,AskPrice1ByInsert:182582.18
					,Available:182582.18
					,CreateTime:""
					}
				}
	]
}
```

+ **Response Parameters**

OrderInsert:

parameter | shortName | type | description | remark
-------- | -------- | -------- | -------- | -------- 					
MemberID | M | string | 成员代码 | 
TradeUnitID | T | string | 交易单元代码 | 
AccountID | A | string | 资金账号 | 
LocalID | L | string | 报单本地标识 | 
ExchangeID | E | string | 交易所代码 | 
InstrumentID | I | string | 标的代码 | 
OrderPriceType | PT | string | 报单价格类型 | 0:限价(手动指定Price值);1:任意价(Price按最高最低限价取值);2:最优价/对手价(Price按对方1档价取值);3:五档价(Price按对方5档价取值);4:十档价(类同上);7:最优Marker价(Price按本方1档价或其+1Tick取值);8:Price按委托额Cost和订单簿情况确定值;9:保底点差价:成交价格Price不会劣于系统设置差价成交;
Direction | D | string | 买卖方向 | 0:买;1:卖;
OffsetFlag | O | string | 开平标志 | 0:开仓;1:平仓;2:强平;3:平今;4:平昨;5:全平;6:指定订单平仓;7:指定成交平仓;8:最大平仓;
Price | P | double | 报单价格 | 
Volume | V | double | 数量 | 
VolumeDisplay | D | double | 显示数量 | 
VolumeMode | VM | string | 数量取值方式 | 0:百分比;1:绝对值;
Cost | C | double | 委托额 | 
OrderType | OT | string | 订单类型 | 0:普通订单;1:(FillAndKill/IOC)立即完成并且剩余撤销;2:(FillOrKill)立即完成全部或者最小订单要求否则全部撤销;3:需要进入订单簿被动成交,否则全部撤销(只做Maker/PostOnly);4:不需要撮合直接成交;
GTDTime | GT | double | GTD时间 | 
MinVolume | MV | double | 最小成交量 | 
BusinessType | BT | string | 业务类别 | 0:普通;1:申赎;2:行权;3:中立仓申报;4:递延交割申报;5:互换定单;6:质押;7:合并分拆;8:转股;9:回售回购;a:投票;b:融资融券;c:期权执行;d:传递被平仓订单BusinessValue;P:自定义11;Q:自定义10;R:自定义9;S:连续交易;T:自定义7;U:画线委托;V:限价市价;W:条件;X:止盈止损;Y:追踪出场;Z:指标类型;
BusinessValue | BV | string | 业务值 | 
CloseOrderID | CI | string | 平仓指定开仓的订单号 | 
IsCrossMargin | IsCrossMargin | int | 是否全仓 | 
Remark | R | string | 备注 | 
OrderID | OrderID | string | 报单系统唯一代码 | 
CopyMemberID | CopyMemberID | string | 带单员代码 | 
CopyOrderID | CopyOrderID | string | 带单员报单号 | 
Leverage | Leverage | double | 委托单杠杆倍数 | 
CopyProfitRate | CopyProfitRate | double | 带单分配比例 | 
APPID | APPID | string | 应用编号 | 
PositionID | B | string | 持仓代码 | 
TriggerPrice | TriggerPrice | double | 触发价 | 
Reserve | Reserve | double | 保留资金 | 

Order:

parameter | shortName | type | description | remark
-------- | -------- | -------- | -------- | -------- 					
MemberID | M | string | 成员代码 | 
TradeUnitID | T | string | 交易单元代码 | 
AccountID | A | string | 资金账号 | 
LocalID | L | string | 报单本地标识 | 
ExchangeID | E | string | 交易所代码 | 
InstrumentID | I | string | 标的代码 | 
OrderPriceType | PT | string | 报单价格类型 | 0:限价(手动指定Price值);1:任意价(Price按最高最低限价取值);2:最优价/对手价(Price按对方1档价取值);3:五档价(Price按对方5档价取值);4:十档价(类同上);7:最优Marker价(Price按本方1档价或其+1Tick取值);8:Price按委托额Cost和订单簿情况确定值;9:保底点差价:成交价格Price不会劣于系统设置差价成交;
Direction | D | string | 买卖方向 | 0:买;1:卖;
OffsetFlag | O | string | 开平标志 | 0:开仓;1:平仓;2:强平;3:平今;4:平昨;5:全平;6:指定订单平仓;7:指定成交平仓;8:最大平仓;
Price | P | double | 报单价格 | 
Volume | V | double | 数量 | 
VolumeDisplay | D | double | 显示数量 | 
VolumeMode | VM | string | 数量取值方式 | 0:百分比;1:绝对值;
Cost | C | double | 委托额 | 
OrderType | OT | string | 订单类型 | 0:普通订单;1:(FillAndKill/IOC)立即完成并且剩余撤销;2:(FillOrKill)立即完成全部或者最小订单要求否则全部撤销;3:需要进入订单簿被动成交,否则全部撤销(只做Maker/PostOnly);4:不需要撮合直接成交;
GTDTime | GT | double | GTD时间 | 
MinVolume | MV | double | 最小成交量 | 
BusinessType | BT | string | 业务类别 | 0:普通;1:申赎;2:行权;3:中立仓申报;4:递延交割申报;5:互换定单;6:质押;7:合并分拆;8:转股;9:回售回购;a:投票;b:融资融券;c:期权执行;d:传递被平仓订单BusinessValue;P:自定义11;Q:自定义10;R:自定义9;S:连续交易;T:自定义7;U:画线委托;V:限价市价;W:条件;X:止盈止损;Y:追踪出场;Z:指标类型;
BusinessValue | BV | string | 业务值 | 
CloseOrderID | CI | string | 平仓指定开仓的订单号 | 
IsCrossMargin | IsCrossMargin | int | 是否全仓 | 
Remark | R | string | 备注 | 
OrderID | OrderID | string | 报单系统唯一代码 | 
CopyMemberID | CopyMemberID | string | 带单员代码 | 
CopyOrderID | CopyOrderID | string | 带单员报单号 | 
Leverage | Leverage | double | 委托单杠杆倍数 | 
CopyProfitRate | CopyProfitRate | double | 带单分配比例 | 
APPID | APPID | string | 应用编号 | 
PositionID | B | string | 持仓代码 | 
TriggerPrice | TriggerPrice | double | 触发价 | 
Reserve | Reserve | double | 保留资金 | 
OrderStatus | OrderStatus | string | 报单状态 | 0:未设置状态;1:全部成交;2:部分成交未撤单;3:部分成交已撤单;4:未成交未撤单;6:无成交已撤单;
DeriveSource | DeriveSource | string | 衍生来源 | 0:正常;1:报价衍生;2:期权执行;3:组合衍生;4:场外成交强平衍生;5:场外成交大宗交易衍生;6:场外成交期转现衍生;7:场外成交只刷单衍生;8:触发订单衍生;9:止损触发订单衍生;a:止盈触发订单衍生;b:资金操作衍生;c:仓位合并衍生;d:CFD订单衍生;e:场外成交ADL衍生;f:回购手续费不能是币;
DeriveDetail | DeriveDetail | string | 衍生明细 | 
VolumeTraded | VolumeTraded | double | 成交数量 | 
VolumeRemain | VolumeRemain | double | 剩余数量 | 
VolumeCancled | VolumeCancled | double | 已经撤单数量 | 
InsertTime | InsertTime | double | 插入时间 | 
UpdateTime | UpdateTime | double | 更新时间 | 
Priority |  | int | 优先权 | 
TimeSortNo |  | double | 按时间排队的序号 | 
FrontNo | FrontNo | int | 前置编号 | 
PriceCurrency | PriceCurrency | string | 计价币种 | 
FeeCurrency | FeeCurrency | string | 手续费币种 | 
ClearCurrency | ClearCurrency | string | 清算币种 | 
FrozenMoney | FrozenMoney | double | 冻结资金 | 
FrozenFee | FrozenFee | double | 冻结手续费 | 
FrozenMargin | FrozenMargin | double | 冻结保证金 | 
Fee | Fee | double | 手续费 | 
CloseProfit | CloseProfit | double | 平仓盈亏 | 
Turnover | Turnover | double | 成交金额 | 
RelatedOrderID | RelatedOrderID | string | 关联报单号 | 
BusinessResult | BusinessResult | string | 业务执行结果 | 
BusinessNo | BusinessNo | double | 业务序列号 | 
Tradable |  | int | 是否可以参加交易 | 
SettlementGroup |  | string | 结算组编号 | 
PosiDirection | PosiDirection | string | 持仓多空方向 | 0:多头;1:空头;2:净;
TradePrice | TradePrice | double | 成交均价 | 
OpenPrice | OpenPrice | double | 平仓成交时的开仓均价 | 
TriggerOrderID | TriggerOrderID | string | 触发报单号 | 
SLTriggerPrice | SLTriggerPrice | double | 开仓报单成交之后的止损触发价 | 
TPTriggerPrice | TPTriggerPrice | double | 开仓报单成交之后的止盈触发价 | 
CopyProfit | CopyProfit | double | 带单盈利分配 | 
Position | Position | double | 成交时的持仓量 | 
UserID | U | string | 交易用户代码 | 
LastPriceByInsert |  | double | 插入时的最新价 | 
BidPrice1ByInsert |  | double | 插入时的买一价 | 
AskPrice1ByInsert |  | double | 插入时的卖一价 | 
Available | Available | double | 可用资金 | 
CreateTime |  | string | 创建时间 | 


## **私有主题分类请求**

+ **Request URL**

`POST /action/v1.0/SendPrivateDispatch`


+ **Request Post Body**

```
{
	,"TopicID":"1983723"
	,"MemberID":"1983723"
	,"PrivateBase":""
}
```
+ **curl Example**

````
curl -d '{"TopicID":"1983723","MemberID":"1983723","PrivateBase":""}'  -X POST  http://127.0.0.1:10000/action/v1.0/SendPrivateDispatch
````
+ **ws Example**

````
{"RequestNo":2,"SendPrivateDispatch":{"TopicID":"1983723","MemberID":"1983723","PrivateBase":""}}
````
+ **Web rowser Example**

````
http://127.0.0.1:10000/action/v1.0/SendPrivateDispatch?TopicID=1983723&MemberID=1983723&PrivateBase=
````
+ **Request Parameters**

parameter | shortName | required | type | description | remark
-------- | -------- | -------- | -------- | -------- | --------
TopicID | T | yes | string | 请参考TOPICID类型定义 | 
MemberID | MemberID | yes | string | 成员代码 | 
PrivateBase | P | yes | string | 私有主题起点 | 

+ **Response**

```
{
	"action": "RecvPrivateDispatch",
	"errorCode": 0,
	"errorMsg": "Success",
	"result": [			
				{"table":"PrivateDispatch",
				 "data":{
					,TopicID:"1983723"
					,MemberID:"1983723"
					,PrivateBase:""
					}
				}
				{"table":"Member",
				 "data":{
					,MemberID:"1983723"
					,MemberType:"1"
					,Grade:""
					,Priority:0
					,FeeGrade:""
					,MarginRateGrade:""
					,TradingRightGrade:""
					,CFDGrade:""
					,LiquidityGrade:""
					,RiskLevel:"1"
					,PositionType:"1"
					,TriggerOrders:0
					,OpenOrders:0
					,OpenPositions:0
					,MaxLocalID:"1983723"
					,SinkType:"1"
					,CopyMemberID:"1983723"
					,Region:""
					,FeeDeduceDisCount:182582.18
					,FeeDeduceCurrency:"BTC"
					,CanBorrow:0
					,Remark:"my_little_tips"
					,InsertTime:1657543832
					,UpdateTime:1657543832
					}
				}
	]
}
```

+ **Response Parameters**

PrivateDispatch:

parameter | shortName | type | description | remark
-------- | -------- | -------- | -------- | -------- 					
TopicID | T | string | 请参考TOPICID类型定义 | 
MemberID | MemberID | string | 成员代码 | 
PrivateBase | P | string | 私有主题起点 | 

Member:

parameter | shortName | type | description | remark
-------- | -------- | -------- | -------- | -------- 					
MemberID | MemberID | string | 成员代码 | 
MemberType | MemberType | string | 成员类型 | a:普通;b:内部做市商;c:系统手续费使用;d:风控使用;e:运营使用;f:运营保留资金使用;g:外部做市商;
Grade | Grade | string | 等级 | 
Priority | Priority | int | 优先权 | 
FeeGrade | FeeGrade | string | 手续费等级 | 
MarginRateGrade | MarginRateGrade | string | 保证金等级 | 
TradingRightGrade | TradingRightGrade | string | 权限等级 | 
CFDGrade | CFDGrade | string | 差价等级 | 
LiquidityGrade | LiquidityGrade | string | 流动性等级 | 
RiskLevel | RiskLevel | string | 风控类型 | 0:本币种资金权益;1:结算组所有资金权益;2:资金账号所有资金权益;3:会员账号所有资金权益;
PositionType | PositionType | string | 持仓类型 | 0:不设置;1:综合持仓(双边持仓);2:净持仓(单边持仓);
TriggerOrders | TriggerOrders | int | 触发订单个数 | 
OpenOrders | OpenOrders | int | 未成交订单数 | 
OpenPositions | OpenPositions | int | 用户持仓个数 | 
MaxLocalID | MaxLocalID | string | 最大本地标识 | 
SinkType | SinkType | string | 下游系统同步类型 | 0:同步到数据库;1:同步到Redis不到数据库;2:不同步;
CopyMemberID | CopyMemberID | string | 带单员代码 | 
Region | Region | string |  | 
FeeDeduceDisCount | FeeDeduceDisCount | double | 作为手续费的抵扣优惠比例 | 
FeeDeduceCurrency | FeeDeduceCurrency | string | 作为手续费的抵扣比例 | 
CanBorrow | CanBorrow | int | 是否需要借钱交易 | 
Remark | Remark | string | 备注 | 
InsertTime | InsertTime | double | 插入时间 | 
UpdateTime | UpdateTime | double | 更新时间 | 


## **调整多空杠杠请求**

+ **Request URL**

`POST /action/v1.0/SendPositionLeverage`


+ **Request Post Body**

```
{
	,"LocalID":"1983723"
	,"PositionID":"1983723"
	,"ExchangeID":"CEX"
	,"InstrumentID":"BTCUSD"
	,"ProductGroup":"Swap"
	,"MemberID":"1983723"
	,"TradeUnitID":"1983723"
	,"AccountID":"1983723"
	,"PosiDirection":"1"
	,"Amount":182582.18
	,"LongLeverage":125
	,"ShortLeverage":125
	,"NetLeverage":125
	,"ActionType":"1"
	,"IsCrossMargin":1
	,"IsAutoAddMargin":1
	,"Remark":"my_little_tips"
}
```
+ **curl Example**

````
curl -d '{"LocalID":"1983723","PositionID":"1983723","ExchangeID":"CEX","InstrumentID":"BTCUSD","ProductGroup":"Swap","MemberID":"1983723","TradeUnitID":"1983723","AccountID":"1983723","PosiDirection":"1","Amount":"182582.18,"LongLeverage":"125,"ShortLeverage":"125,"NetLeverage":"125,"ActionType":"1","IsCrossMargin":"1,"IsAutoAddMargin":"1,"Remark":"my_little_tips"}'  -X POST  http://127.0.0.1:10000/action/v1.0/SendPositionLeverage
````
+ **ws Example**

````
{"RequestNo":2,"SendPositionLeverage":{"LocalID":"1983723","PositionID":"1983723","ExchangeID":"CEX","InstrumentID":"BTCUSD","ProductGroup":"Swap","MemberID":"1983723","TradeUnitID":"1983723","AccountID":"1983723","PosiDirection":"1","Amount":"182582.18,"LongLeverage":"125,"ShortLeverage":"125,"NetLeverage":"125,"ActionType":"1","IsCrossMargin":"1,"IsAutoAddMargin":"1,"Remark":"my_little_tips"}}
````
+ **Web rowser Example**

````
http://127.0.0.1:10000/action/v1.0/SendPositionLeverage?LocalID=1983723&PositionID=1983723&ExchangeID=CEX&InstrumentID=BTCUSD&ProductGroup=Swap&MemberID=1983723&TradeUnitID=1983723&AccountID=1983723&PosiDirection=1&Amount=182582.18&LongLeverage=125&ShortLeverage=125&NetLeverage=125&ActionType=1&IsCrossMargin=1&IsAutoAddMargin=1&Remark=my_little_tips
````
+ **Request Parameters**

parameter | shortName | required | type | description | remark
-------- | -------- | -------- | -------- | -------- | --------
LocalID | LocalID | no | string | 出入金本地号 | 
PositionID | PositionID | no | string | 持仓代码 | 
ExchangeID | ExchangeID | yes | string | 交易所代码 | 
InstrumentID | InstrumentID | yes | string | 标的代码 | 
ProductGroup | ProductGroup | no | string | 产品组 | 
MemberID | MemberID | yes | string | 成员代码 | 
TradeUnitID | TradeUnitID | yes | string | 交易单元代码 | 
AccountID | AccountID | no | string | 资金账号 | 
PosiDirection | PosiDirection | no | string | 持仓多空方向，0:多;1:空;2:净;空值:多空方向同时操作 | 0:多头;1:空头;2:净;
Amount | Amount | yes | double | 发生额 | 
LongLeverage | LongLeverage | no | double | 杠杆倍数 | 
ShortLeverage | ShortLeverage | no | double | 杠杆倍数 | 
NetLeverage | NetLeverage | no | double | 杠杆倍数 | 
ActionType | ActionType | yes | string | 操作类型 | 1:调整杠杆倍数;2:增加保证金;3:减少保证金;4:全仓逐仓模式调换;5:调整不超过最大杠杆倍数;6:调整默认杠杆倍数;7:调整默认全逐仓;8:调整成本价;a:全平;b:最大平仓;c:清理之后才能切换的全逐仓切换;
IsCrossMargin | IsCrossMargin | no | int | 是否全仓 | 
IsAutoAddMargin | IsAutoAddMargin | no | int | 是否自动追加保证金 | 
Remark | R | no | string | 备注 | 

+ **Response**

```
{
	"action": "RecvPositionLeverage",
	"errorCode": 0,
	"errorMsg": "Success",
	"result": [			
				{"table":"PositionAction",
				 "data":{
					,LocalID:"1983723"
					,PositionID:"1983723"
					,ExchangeID:"CEX"
					,InstrumentID:"BTCUSD"
					,ProductGroup:"Swap"
					,MemberID:"1983723"
					,TradeUnitID:"1983723"
					,AccountID:"1983723"
					,PosiDirection:"1"
					,Amount:182582.18
					,LongLeverage:125
					,ShortLeverage:125
					,NetLeverage:125
					,ActionType:"1"
					,IsCrossMargin:1
					,IsAutoAddMargin:1
					,Remark:"my_little_tips"
					}
				}
				{"table":"Position",
				 "data":{
					,PositionID:"1983723"
					,MemberID:"1983723"
					,TradeUnitID:"1983723"
					,ExchangeID:"CEX"
					,InstrumentID:"BTCUSD"
					,PositionClass:"1"
					,PosiDirection:"1"
					,PrePosition:182582.18
					,Position:182582.18
					,LongFrozen:182582.18
					,ShortFrozen:182582.18
					,PreLongFrozen:182582.18
					,PreShortFrozen:182582.18
					,HighestPosition:182582.18
					,ClosePosition:182582.18
					,PositionCost:182582.18
					,CostPrice:182582.18
					,UseMargin:182582.18
					,FrozenMargin:182582.18
					,LongFrozenMargin:182582.18
					,ShortFrozenMargin:182582.18
					,CloseProfit:10
					,TotalPositionCost:182582.18
					,TotalCloseProfit:182582.18
					,OpenPrice:182582.18
					,ClosePrice:182582.18
					,TradeFee:182582.18
					,PositionFee:182582.18
					,Leverage:125
					,AccountID:"1983723"
					,PriceCurrency:"BTC"
					,ClearCurrency:"BTC"
					,SettlementGroup:"999"
					,IsCrossMargin:1
					,CloseOrderID:"1983723"
					,SLTriggerPrice:182582.18
					,TPTriggerPrice:182582.18
					,BeginTime:1657543832
					,InsertTime:1657543832
					,LastOpenTime:1657543832
					,UpdateTime:1657543832
					,BusinessNo:11176422300333
					,IsAutoAddMargin:1
					,Frequency:0
					,MaintMargin:182582.18
					,UnRealProfit:182582.18
					,LiquidPrice:182582.18
					,CreateTime:""
					,CopyMemberID:"1983723"
					,CopyProfitRate:182582.18
					,CopyProfit:182582.18
					,FirstTradeID:"1983723"
					,LastTradeID:"1983723"
					,BusinessType:"1"
					,BusinessValue:""
					,Reserve:182582.18
					,ReserveProfit:182582.18
					,Remark:"my_little_tips"
					}
				}
	]
}
```

+ **Response Parameters**

PositionAction:

parameter | shortName | type | description | remark
-------- | -------- | -------- | -------- | -------- 					
LocalID | LocalID | string | 出入金本地号 | 
PositionID | PositionID | string | 持仓代码 | 
ExchangeID | ExchangeID | string | 交易所代码 | 
InstrumentID | InstrumentID | string | 标的代码 | 
ProductGroup | ProductGroup | string | 产品组 | 
MemberID | MemberID | string | 成员代码 | 
TradeUnitID | TradeUnitID | string | 交易单元代码 | 
AccountID | AccountID | string | 资金账号 | 
PosiDirection | PosiDirection | string | 持仓多空方向，0:多;1:空;2:净;空值:多空方向同时操作 | 0:多头;1:空头;2:净;
Amount | Amount | double | 发生额 | 
LongLeverage | LongLeverage | double | 杠杆倍数 | 
ShortLeverage | ShortLeverage | double | 杠杆倍数 | 
NetLeverage | NetLeverage | double | 杠杆倍数 | 
ActionType | ActionType | string | 操作类型 | 1:调整杠杆倍数;2:增加保证金;3:减少保证金;4:全仓逐仓模式调换;5:调整不超过最大杠杆倍数;6:调整默认杠杆倍数;7:调整默认全逐仓;8:调整成本价;a:全平;b:最大平仓;c:清理之后才能切换的全逐仓切换;
IsCrossMargin | IsCrossMargin | int | 是否全仓 | 
IsAutoAddMargin | IsAutoAddMargin | int | 是否自动追加保证金 | 
Remark | R | string | 备注 | 

Position:

parameter | shortName | type | description | remark
-------- | -------- | -------- | -------- | -------- 					
PositionID | PositionID | string | 持仓代码 | 
MemberID | MemberID | string | 成员代码 | 
TradeUnitID | TradeUnitID | string | 交易单元代码 | 
ExchangeID | ExchangeID | string | 交易所代码 | 
InstrumentID | InstrumentID | string | 标的代码 | 
PositionClass | PositionClass | string | 持仓类型 | 0:不设置;1:保证金交易;3:资产;4:持币成本;
PosiDirection | PosiDirection | string | 持仓多空方向 | 0:多头;1:空头;2:净;
PrePosition | PrePosition | double | 上次持仓 | 
Position | Position | double | 总持仓 | 
LongFrozen | LongFrozen | double | 多头冻结 | 
ShortFrozen | ShortFrozen | double | 空头冻结 | 
PreLongFrozen | PreLongFrozen | double | 昨日多头冻结 | 
PreShortFrozen | PreShortFrozen | double | 昨日空头冻结 | 
HighestPosition | HighestPosition | double | 最大持仓 | 
ClosePosition | ClosePosition | double | 可平持仓 | 
PositionCost | PositionCost | double | 持仓成本 | 
CostPrice | CostPrice | double | 成本价 | 
UseMargin | UseMargin | double | 占用保证金 | 
FrozenMargin | FrozenMargin | double | 冻结保证金 | 
LongFrozenMargin | LongFrozenMargin | double | 多头冻结保证金 | 
ShortFrozenMargin | ShortFrozenMargin | double | 空头冻结保证金 | 
CloseProfit | CloseProfit | double | 平仓盈亏 | 
TotalPositionCost | TotalPositionCost | double | 开仓成本 | 
TotalCloseProfit | TotalCloseProfit | double | 总平仓盈亏 | 
OpenPrice | OpenPrice | double | 开仓均价 | 
ClosePrice | ClosePrice | double | 平仓均价 | 
TradeFee | TradeFee | double | 交易费用 | 
PositionFee | PositionFee | double | 持仓费用或者股票分红等 | 
Leverage | Leverage | double | 杠杆倍数 | 
AccountID | AccountID | string | 资金账号 | 
PriceCurrency | PriceCurrency | string | 计价币种 | 
ClearCurrency | ClearCurrency | string | 清算币种 | 
SettlementGroup | SettlementGroup | string | 结算组编号 | 
IsCrossMargin | IsCrossMargin | int | 是否全仓 | 
CloseOrderID | CloseOrderID | string | 平仓单代码 | 
SLTriggerPrice | SLTriggerPrice | double | 止损触发价 | 
TPTriggerPrice | TPTriggerPrice | double | 止盈触发价 | 
BeginTime | BeginTime | double | 持仓不为0的开始时间 | 
InsertTime | InsertTime | double | 插入时间(新持仓时间) | 
LastOpenTime | LastOpenTime | double | 最后开仓时间 | 
UpdateTime | UpdateTime | double | 更新时间 | 
BusinessNo | BusinessNo | double | 业务序列号 | 
IsAutoAddMargin | IsAutoAddMargin | int | 是否自动追加保证金 | 
Frequency | Frequency | int | 每秒更新的次数 | 
MaintMargin | MaintMargin | double | 维持保证金 | 
UnRealProfit | UnRealProfit | double | 未实现盈亏 | 
LiquidPrice | LiquidPrice | double | 清算价格 | 
CreateTime | CreateTime | string | 创建时间 | 
CopyMemberID | CopyMemberID | string | 带单员代码 | 
CopyProfitRate | CopyProfitRate | double | 带单分配比例 | 
CopyProfit | CopyProfit | double | 带单盈利分配 | 
FirstTradeID | FID | string | 初始成交订单号 | 
LastTradeID | LID | string | 结束成交订单号 | 
BusinessType | BT | string | 业务类别 | 0:普通;1:申赎;2:行权;3:中立仓申报;4:递延交割申报;5:互换定单;6:质押;7:合并分拆;8:转股;9:回售回购;a:投票;b:融资融券;c:期权执行;d:传递被平仓订单BusinessValue;P:自定义11;Q:自定义10;R:自定义9;S:连续交易;T:自定义7;U:画线委托;V:限价市价;W:条件;X:止盈止损;Y:追踪出场;Z:指标类型;
BusinessValue | BV | string | 业务值 | 
Reserve | Reserve | double | 保留资金 | 
ReserveProfit | ReserveProfit | double | 保留资金盈亏 | 
Remark | R | string | 备注 | 


## **设置合约请求**

+ **Request URL**

`POST /action/v1.0/SendInstrumentUpdate`


+ **Request Post Body**

```
{
	,"ExchangeID":"CEX"
	,"InstrumentID":"BTCUSD"
	,"MinOrderVolume":182582.18
	,"MinOrderCost":182582.18
	,"MaxOrderVolume":182582.18
	,"PriceTick":182582.18
	,"VolumeTick":182582.18
	,"MaxOpenInterest":182582.18
	,"BasisPrice":182582.18
	,"UpperLimitPrice":182582.18
	,"LowerLimitPrice":182582.18
}
```
+ **curl Example**

````
curl -d '{"ExchangeID":"CEX","InstrumentID":"BTCUSD","MinOrderVolume":"182582.18,"MinOrderCost":"182582.18,"MaxOrderVolume":"182582.18,"PriceTick":"182582.18,"VolumeTick":"182582.18,"MaxOpenInterest":"182582.18,"BasisPrice":"182582.18,"UpperLimitPrice":"182582.18,"LowerLimitPrice":"182582.18}'  -X POST  http://127.0.0.1:10000/action/v1.0/SendInstrumentUpdate
````
+ **ws Example**

````
{"RequestNo":2,"SendInstrumentUpdate":{"ExchangeID":"CEX","InstrumentID":"BTCUSD","MinOrderVolume":"182582.18,"MinOrderCost":"182582.18,"MaxOrderVolume":"182582.18,"PriceTick":"182582.18,"VolumeTick":"182582.18,"MaxOpenInterest":"182582.18,"BasisPrice":"182582.18,"UpperLimitPrice":"182582.18,"LowerLimitPrice":"182582.18}}
````
+ **Web rowser Example**

````
http://127.0.0.1:10000/action/v1.0/SendInstrumentUpdate?ExchangeID=CEX&InstrumentID=BTCUSD&MinOrderVolume=182582.18&MinOrderCost=182582.18&MaxOrderVolume=182582.18&PriceTick=182582.18&VolumeTick=182582.18&MaxOpenInterest=182582.18&BasisPrice=182582.18&UpperLimitPrice=182582.18&LowerLimitPrice=182582.18
````
+ **Request Parameters**

parameter | shortName | required | type | description | remark
-------- | -------- | -------- | -------- | -------- | --------
ExchangeID | ExchangeID | no | string | 交易所代码 | 
InstrumentID | InstrumentID | no | string | 标的代码 | 
MinOrderVolume | MinOrderVolume | no | double | 最小下单量 | 
MinOrderCost | MinOrderCost | no | double | 最小下单额 | 
MaxOrderVolume | MaxOrderVolume | no | double | 最大下单量 | 
PriceTick | PriceTick | no | double | 最小变动价位 | 
VolumeTick | VolumeTick | no | double | 最小变动量 | 
MaxOpenInterest | MaxOpenInterest | no | double | 最大持仓量 | 
BasisPrice | BasisPrice | no | double | 挂牌基准价 | 
UpperLimitPrice | UpperLimitPrice | no | double | 最高限价 | 
LowerLimitPrice | LowerLimitPrice | no | double | 最低限价 | 

+ **Response**

```
{
	"action": "RecvInstrumentUpdate",
	"errorCode": 0,
	"errorMsg": "Success",
	"result": [			
				{"table":"InstrumentUpdate",
				 "data":{
					,ExchangeID:"CEX"
					,InstrumentID:"BTCUSD"
					,MinOrderVolume:182582.18
					,MinOrderCost:182582.18
					,MaxOrderVolume:182582.18
					,PriceTick:182582.18
					,VolumeTick:182582.18
					,MaxOpenInterest:182582.18
					,BasisPrice:182582.18
					,UpperLimitPrice:182582.18
					,LowerLimitPrice:182582.18
					}
				}
				{"table":"Instrument",
				 "data":{
					,ExchangeID:"CEX"
					,InstrumentID:"BTCUSD"
					,InstrumentName:""
					,SettlementGroup:"999"
					,ProductGroup:"Swap"
					,UnderlyingID:"1983723"
					,ProductClass:"1"
					,ProductType:"1"
					,StrikePrice:182582.18
					,OptionsType:"1"
					,VolumeMultiple:182582.18
					,StartTime:1657543832
					,ExpireTime:1657543832
					,PriceCurrency:"BTC"
					,ClearCurrency:"BTC"
					,IsInverse:1
					,BaseCurrency:"BTC"
					,MarginPriceType:"1"
					,BasisPrice:182582.18
					,MinOrderVolume:182582.18
					,MinOrderCost:182582.18
					,MaxOrderVolume:182582.18
					,PriceTick:182582.18
					,VolumeTick:182582.18
					,ShowVolumeTick:182582.18
					,TradingModel:"1"
					,DayStartTime:""
					,SortNumber:-1
					,Remark:"my_little_tips"
					,DefaultLeverage:125
					,PriceLimitValueMode:"1"
					,PriceLimitPrice:"1"
					,PriceLimitUpperValue:182582.18
					,PriceLimitLowerValue:182582.18
					,MarginRateGroup:"999"
					,CFDGroup:"999"
					,FeeGroup:"999"
					,TradingRightGroup:"999"
					,MaxOpenInterest:182582.18
					,FundingRateGroup:"999"
					,InsertTime:1657543832
					,UpdateTime:1657543832
					,MarkedPriceTimes:0
					}
				}
	]
}
```

+ **Response Parameters**

InstrumentUpdate:

parameter | shortName | type | description | remark
-------- | -------- | -------- | -------- | -------- 					
ExchangeID | ExchangeID | string | 交易所代码 | 
InstrumentID | InstrumentID | string | 标的代码 | 
MinOrderVolume | MinOrderVolume | double | 最小下单量 | 
MinOrderCost | MinOrderCost | double | 最小下单额 | 
MaxOrderVolume | MaxOrderVolume | double | 最大下单量 | 
PriceTick | PriceTick | double | 最小变动价位 | 
VolumeTick | VolumeTick | double | 最小变动量 | 
MaxOpenInterest | MaxOpenInterest | double | 最大持仓量 | 
BasisPrice | BasisPrice | double | 挂牌基准价 | 
UpperLimitPrice | UpperLimitPrice | double | 最高限价 | 
LowerLimitPrice | LowerLimitPrice | double | 最低限价 | 

Instrument:

parameter | shortName | type | description | remark
-------- | -------- | -------- | -------- | -------- 					
ExchangeID | X | string | 交易所代码 | 
InstrumentID | I | string | 标的代码 | 
InstrumentName | N | string | 标的名称 | 
SettlementGroup |  | string | 结算组编号 | 
ProductGroup | G | string | 产品组 | 
UnderlyingID |  | string | 基础标的代码 | 
ProductClass |  | string | 产品大类 | 1:保证金交易;2:权利金交易;3:资产转让（股票/基金/国债等）;4:兑换交易（资金之间的互换）;5:组合;6:行情;
ProductType |  | string | 产品类型 | 1:交割期货;2:永续、利率互换期货;3:即期/日交割期货;4:递延交割期货;5:权证;6:美式期权;7:欧式期权;8:币币交易;9:外汇兑换;a:股票;b:债券;c:基金;
StrikePrice |  | double | 执行价 | 
OptionsType |  | string | 期权类型 | 0:非期权;1:看涨;2:看跌;
VolumeMultiple | VM | double | 数量乘数 | 
StartTime |  | double | 启动时间 | 
ExpireTime |  | double | 到期时间 | 
PriceCurrency | PC | string | 计价币种 | 
ClearCurrency | CC | string | 清算币种 | 
IsInverse | II | int | 是否反向标的 | 
BaseCurrency | BB | string | 标的基础货币 | 
MarginPriceType |  | string | 保证金价格类型 | 0:不按比例按固定值,无需价格;1:昨结算价;2:开仓价;3:标记价;4:最新价;
BasisPrice |  | double | 挂牌基准价 | 
MinOrderVolume | MOV | double | 最小下单量 | 
MinOrderCost | MOC | double | 最小下单额 | 
MaxOrderVolume |  | double | 最大下单量 | 
PriceTick | PT | double | 最小变动价位 | 
VolumeTick | VT | double | 最小变动量 | 
ShowVolumeTick |  | double | 前端行情显示最小变动量(服务端不用) | 
TradingModel |  | string | 交易模式 | 0:价格优先时间优先;1:价格优先按比例分配;2:只能和做市商报价成交;3:没有撮合;
DayStartTime |  | string | 每日开始交易时间 | 
SortNumber |  | int | 排序字段 | 
Remark | RE | string | 备注 | 
DefaultLeverage | DL | double | 默认杠杆倍数 | 
PriceLimitValueMode |  | string | 取值方式 | 0:百分比;1:绝对值;
PriceLimitPrice |  | string | 限价价格类型 | 1:昨结算价;2:最新价;3:基础标的价;4:标记价;
PriceLimitUpperValue |  | double | 上限 | 
PriceLimitLowerValue |  | double | 下限 | 
MarginRateGroup |  | string | 保证金组 | 
CFDGroup |  | string | 差价组 | 
FeeGroup |  | string | 手续费组 | 
TradingRightGroup |  | string | 权限组 | 
MaxOpenInterest |  | double | 最大持仓量 | 
FundingRateGroup |  | string | 资金费用组 | 
InsertTime |  | double | 插入时间 | 
UpdateTime |  | double | 更新时间 | 
MarkedPriceTimes |  | int | 标记价格计算次数(0:最新价;1-n:次数;空:不生成标记价) | 


## **充币兑换请求**

+ **Request URL**

`POST /action/v1.0/SendChangeCurrency`


+ **Request Post Body**

```
{
	,"MemberID":"1983723"
	,"AccountID":"1983723"
	,"LocalID":"1983723"
	,"ExchangeID":"CEX"
	,"InstrumentID1":"1983723"
	,"InstrumentID2":"1983723"
	,"Currency":"BTC"
	,"Amount":182582.18
	,"Remark":"my_little_tips"
}
```
+ **curl Example**

````
curl -d '{"MemberID":"1983723","AccountID":"1983723","LocalID":"1983723","ExchangeID":"CEX","InstrumentID1":"1983723","InstrumentID2":"1983723","Currency":"BTC","Amount":"182582.18,"Remark":"my_little_tips"}'  -X POST  http://127.0.0.1:10000/action/v1.0/SendChangeCurrency
````
+ **ws Example**

````
{"RequestNo":2,"SendChangeCurrency":{"MemberID":"1983723","AccountID":"1983723","LocalID":"1983723","ExchangeID":"CEX","InstrumentID1":"1983723","InstrumentID2":"1983723","Currency":"BTC","Amount":"182582.18,"Remark":"my_little_tips"}}
````
+ **Web rowser Example**

````
http://127.0.0.1:10000/action/v1.0/SendChangeCurrency?MemberID=1983723&AccountID=1983723&LocalID=1983723&ExchangeID=CEX&InstrumentID1=1983723&InstrumentID2=1983723&Currency=BTC&Amount=182582.18&Remark=my_little_tips
````
+ **Request Parameters**

parameter | shortName | required | type | description | remark
-------- | -------- | -------- | -------- | -------- | --------
MemberID | MemberID | no | string | 成员代码 | 
AccountID | AccountID | no | string | 资金账号 | 
LocalID | LocalID | no | string | 报单本地标识 | 
ExchangeID | ExchangeID | yes | string | 交易所代码 | 
InstrumentID1 | InstrumentID1 | yes | string | 第一个标的代码 | 
InstrumentID2 | InstrumentID2 | no | string | 第二个标的代码 | 
Currency | Currency | yes | string | 币种 | 
Amount | Amount | yes | double | 发生额 | 
Remark | Remark | no | string | 备注 | 

+ **Response**

```
{
	"action": "RecvChangeCurrency",
	"errorCode": 0,
	"errorMsg": "Success",
	"result": [			
				{"table":"Account",
				 "data":{
					,SettlementGroup:"999"
					,AccountID:"1983723"
					,Currency:"BTC"
					,MemberID:"1983723"
					,Available:182582.18
					,Withdrawable:182582.18
					,PreBalance:182582.18
					,Balance:182582.18
					,PositionCost:182582.18
					,UseMargin:182582.18
					,CrossMargin:182582.18
					,CloseProfit:10
					,Deposit:182582.18
					,Withdraw:182582.18
					,FrozenMargin:182582.18
					,FrozenMoney:182582.18
					,FrozenFee:182582.18
					,Fee:0.22364
					,MoneyChange:182582.18
					,Reserve:182582.18
					,ReserveAvailable:182582.18
					,ReserveLimit:182582.18
					,ReserveProfit:182582.18
					,ReserveType:"1"
					,ReserveActive:0
					,TotalCloseProfit:182582.18
					,TotalDeposit:182582.18
					,TotalWithdraw:182582.18
					,TotalFee:182582.18
					,TotalMoneyChange:182582.18
					,InsertTime:1657543832
					,UpdateTime:1657543832
					,AccountType:""
					,BusinessNo:11176422300333
					,MarginRateGrade:""
					,TradingRightGrade:""
					,Frequency:0
					,MaintMargin:182582.18
					,UnRealProfit:182582.18
					,CopyProfit:182582.18
					,CFDType:"1"
					,FeeGrade:""
					,CFDGrade:""
					,Borrow:182582.18
					,Remark:"my_little_tips"
					,MaxLocalID:"1983723"
					,CurrValue:182582.18
					,CanReduce:0
					}
				}
				{"table":"Order",
				 "data":{
					,MemberID:"1983723"
					,TradeUnitID:"1983723"
					,AccountID:"1983723"
					,LocalID:"1983723"
					,ExchangeID:"CEX"
					,InstrumentID:"BTCUSD"
					,OrderPriceType:"1"
					,Direction:"1"
					,OffsetFlag:"1"
					,Price:182582.18
					,Volume:182582.18
					,VolumeDisplay:182582.18
					,VolumeMode:"1"
					,Cost:182582.18
					,OrderType:"1"
					,GTDTime:1657543832
					,MinVolume:182582.18
					,BusinessType:"1"
					,BusinessValue:""
					,CloseOrderID:"1983723"
					,IsCrossMargin:1
					,Remark:"my_little_tips"
					,OrderID:"12376422300333"
					,CopyMemberID:"1983723"
					,CopyOrderID:"1983723"
					,Leverage:125
					,CopyProfitRate:182582.18
					,APPID:"ios"
					,PositionID:"1983723"
					,TriggerPrice:182582.18
					,Reserve:182582.18
					,OrderStatus:"1"
					,DeriveSource:"1"
					,DeriveDetail:""
					,VolumeTraded:182582.18
					,VolumeRemain:182582.18
					,VolumeCancled:182582.18
					,InsertTime:1657543832
					,UpdateTime:1657543832
					,Priority:0
					,TimeSortNo:1657543832
					,FrontNo:1
					,PriceCurrency:"BTC"
					,FeeCurrency:"BTC"
					,ClearCurrency:"BTC"
					,FrozenMoney:182582.18
					,FrozenFee:182582.18
					,FrozenMargin:182582.18
					,Fee:0.22364
					,CloseProfit:10
					,Turnover:182582.18
					,RelatedOrderID:"1983723"
					,BusinessResult:""
					,BusinessNo:11176422300333
					,Tradable:0
					,SettlementGroup:"999"
					,PosiDirection:"1"
					,TradePrice:182582.18
					,OpenPrice:182582.18
					,TriggerOrderID:"1983723"
					,SLTriggerPrice:182582.18
					,TPTriggerPrice:182582.18
					,CopyProfit:182582.18
					,Position:182582.18
					,UserID:"1983723"
					,LastPriceByInsert:182582.18
					,BidPrice1ByInsert:182582.18
					,AskPrice1ByInsert:182582.18
					,Available:182582.18
					,CreateTime:""
					}
				}
	]
}
```

+ **Response Parameters**

Account:

parameter | shortName | type | description | remark
-------- | -------- | -------- | -------- | -------- 					
SettlementGroup | SettlementGroup | string | 结算组编号 | 
AccountID | AccountID | string | 资金账号 | 
Currency | Currency | string | 币种 | 
MemberID | MemberID | string | 成员代码 | 
Available | Available | double | 可用资金 | 
Withdrawable | Withdrawable | double | 可取资金 | 
PreBalance | PreBalance | double | 上次静态权益 | 
Balance | Balance | double | 静态权益 | 
PositionCost | PositionCost | double | 持仓成本 | 
UseMargin | UseMargin | double | 所有占用保证金 | 
CrossMargin | CrossMargin | double | 全仓保证金 | 
CloseProfit | CloseProfit | double | 平仓盈亏 | 
Deposit | Deposit | double | 入金金额 | 
Withdraw | Withdraw | double | 出金金额 | 
FrozenMargin | FrozenMargin | double | 冻结保证金 | 
FrozenMoney | FrozenMoney | double | 冻结资金 | 
FrozenFee | FrozenFee | double | 冻结手续费 | 
Fee | Fee | double | 手续费 | 
MoneyChange | MoneyChange | double | 资金变化 | 
Reserve | Reserve | double | 保留资金 | 
ReserveAvailable | ReserveAvailable | double | 保留资金可用 | 
ReserveLimit | ReserveLimit | double | 保留资金限制 | 
ReserveProfit | ReserveProfit | double | 保留资金盈亏 | 
ReserveType | ReserveType | string | 保留资金类型 | 0:先亏损保留资金再亏损真钱(劣后);1:先亏损真钱再亏损保留资金(优先);2:保留资金不做亏损(抵押金);
ReserveActive | ReserveActive | int | 保留资金是否活跃 | 
TotalCloseProfit | TotalCloseProfit | double | 总平仓盈亏 | 
TotalDeposit | TotalDeposit | double | 总入金金额 | 
TotalWithdraw | TotalWithdraw | double | 总出金金额 | 
TotalFee | TotalFee | double | 总手续费 | 
TotalMoneyChange | TotalMoneyChange | double | 总资金变化 | 
InsertTime | InsertTime | double | 插入时间 | 
UpdateTime | UpdateTime | double | 更新时间 | 
AccountType | AccountType | string | 账户类型 | 
BusinessNo | BusinessNo | double | 业务序列号 | 
MarginRateGrade | MarginRateGrade | string | 保证金等级 | 
TradingRightGrade | TradingRightGrade | string | 权限等级 | 
Frequency | Frequency | int | 每秒更新的次数 | 
MaintMargin | MaintMargin | double | 维持保证金 | 
UnRealProfit | UnRealProfit | double | 未实现盈亏 | 
CopyProfit | CopyProfit | double | 带单盈利分配 | 
CFDType | CFDType | string | 点差接管类型 | 0:先撮合后点差接管;1:只撮合不点差接管;2:不撮合只点差接管;
FeeGrade | FeeGrade | string | 手续费等级 | 
CFDGrade | CFDGrade | string | 差价等级 | 
Borrow | Borrow | double | 借款周期结算量 | 
Remark | Remark | string | 备注 | 
MaxLocalID | MaxLocalID | string | 最大本地标识 | 
CurrValue | CurrValue | double | 当前价值 | 
CanReduce | CanReduce | int | 是否可以出金 | 

Order:

parameter | shortName | type | description | remark
-------- | -------- | -------- | -------- | -------- 					
MemberID | M | string | 成员代码 | 
TradeUnitID | T | string | 交易单元代码 | 
AccountID | A | string | 资金账号 | 
LocalID | L | string | 报单本地标识 | 
ExchangeID | E | string | 交易所代码 | 
InstrumentID | I | string | 标的代码 | 
OrderPriceType | PT | string | 报单价格类型 | 0:限价(手动指定Price值);1:任意价(Price按最高最低限价取值);2:最优价/对手价(Price按对方1档价取值);3:五档价(Price按对方5档价取值);4:十档价(类同上);7:最优Marker价(Price按本方1档价或其+1Tick取值);8:Price按委托额Cost和订单簿情况确定值;9:保底点差价:成交价格Price不会劣于系统设置差价成交;
Direction | D | string | 买卖方向 | 0:买;1:卖;
OffsetFlag | O | string | 开平标志 | 0:开仓;1:平仓;2:强平;3:平今;4:平昨;5:全平;6:指定订单平仓;7:指定成交平仓;8:最大平仓;
Price | P | double | 报单价格 | 
Volume | V | double | 数量 | 
VolumeDisplay | D | double | 显示数量 | 
VolumeMode | VM | string | 数量取值方式 | 0:百分比;1:绝对值;
Cost | C | double | 委托额 | 
OrderType | OT | string | 订单类型 | 0:普通订单;1:(FillAndKill/IOC)立即完成并且剩余撤销;2:(FillOrKill)立即完成全部或者最小订单要求否则全部撤销;3:需要进入订单簿被动成交,否则全部撤销(只做Maker/PostOnly);4:不需要撮合直接成交;
GTDTime | GT | double | GTD时间 | 
MinVolume | MV | double | 最小成交量 | 
BusinessType | BT | string | 业务类别 | 0:普通;1:申赎;2:行权;3:中立仓申报;4:递延交割申报;5:互换定单;6:质押;7:合并分拆;8:转股;9:回售回购;a:投票;b:融资融券;c:期权执行;d:传递被平仓订单BusinessValue;P:自定义11;Q:自定义10;R:自定义9;S:连续交易;T:自定义7;U:画线委托;V:限价市价;W:条件;X:止盈止损;Y:追踪出场;Z:指标类型;
BusinessValue | BV | string | 业务值 | 
CloseOrderID | CI | string | 平仓指定开仓的订单号 | 
IsCrossMargin | IsCrossMargin | int | 是否全仓 | 
Remark | R | string | 备注 | 
OrderID | OrderID | string | 报单系统唯一代码 | 
CopyMemberID | CopyMemberID | string | 带单员代码 | 
CopyOrderID | CopyOrderID | string | 带单员报单号 | 
Leverage | Leverage | double | 委托单杠杆倍数 | 
CopyProfitRate | CopyProfitRate | double | 带单分配比例 | 
APPID | APPID | string | 应用编号 | 
PositionID | B | string | 持仓代码 | 
TriggerPrice | TriggerPrice | double | 触发价 | 
Reserve | Reserve | double | 保留资金 | 
OrderStatus | OrderStatus | string | 报单状态 | 0:未设置状态;1:全部成交;2:部分成交未撤单;3:部分成交已撤单;4:未成交未撤单;6:无成交已撤单;
DeriveSource | DeriveSource | string | 衍生来源 | 0:正常;1:报价衍生;2:期权执行;3:组合衍生;4:场外成交强平衍生;5:场外成交大宗交易衍生;6:场外成交期转现衍生;7:场外成交只刷单衍生;8:触发订单衍生;9:止损触发订单衍生;a:止盈触发订单衍生;b:资金操作衍生;c:仓位合并衍生;d:CFD订单衍生;e:场外成交ADL衍生;f:回购手续费不能是币;
DeriveDetail | DeriveDetail | string | 衍生明细 | 
VolumeTraded | VolumeTraded | double | 成交数量 | 
VolumeRemain | VolumeRemain | double | 剩余数量 | 
VolumeCancled | VolumeCancled | double | 已经撤单数量 | 
InsertTime | InsertTime | double | 插入时间 | 
UpdateTime | UpdateTime | double | 更新时间 | 
Priority |  | int | 优先权 | 
TimeSortNo |  | double | 按时间排队的序号 | 
FrontNo | FrontNo | int | 前置编号 | 
PriceCurrency | PriceCurrency | string | 计价币种 | 
FeeCurrency | FeeCurrency | string | 手续费币种 | 
ClearCurrency | ClearCurrency | string | 清算币种 | 
FrozenMoney | FrozenMoney | double | 冻结资金 | 
FrozenFee | FrozenFee | double | 冻结手续费 | 
FrozenMargin | FrozenMargin | double | 冻结保证金 | 
Fee | Fee | double | 手续费 | 
CloseProfit | CloseProfit | double | 平仓盈亏 | 
Turnover | Turnover | double | 成交金额 | 
RelatedOrderID | RelatedOrderID | string | 关联报单号 | 
BusinessResult | BusinessResult | string | 业务执行结果 | 
BusinessNo | BusinessNo | double | 业务序列号 | 
Tradable |  | int | 是否可以参加交易 | 
SettlementGroup |  | string | 结算组编号 | 
PosiDirection | PosiDirection | string | 持仓多空方向 | 0:多头;1:空头;2:净;
TradePrice | TradePrice | double | 成交均价 | 
OpenPrice | OpenPrice | double | 平仓成交时的开仓均价 | 
TriggerOrderID | TriggerOrderID | string | 触发报单号 | 
SLTriggerPrice | SLTriggerPrice | double | 开仓报单成交之后的止损触发价 | 
TPTriggerPrice | TPTriggerPrice | double | 开仓报单成交之后的止盈触发价 | 
CopyProfit | CopyProfit | double | 带单盈利分配 | 
Position | Position | double | 成交时的持仓量 | 
UserID | U | string | 交易用户代码 | 
LastPriceByInsert |  | double | 插入时的最新价 | 
BidPrice1ByInsert |  | double | 插入时的买一价 | 
AskPrice1ByInsert |  | double | 插入时的卖一价 | 
Available | Available | double | 可用资金 | 
CreateTime |  | string | 创建时间 | 


## **钱包登录获取随机数请求**

+ **Request URL**

`POST /action/v1.0/SendWalletNonce`


+ **Request Post Body**

```
{
	,"UserID":"1983723"
	,"MemberID":"1983723"
	,"Nonce":""
}
```
+ **curl Example**

````
curl -d '{"UserID":"1983723","MemberID":"1983723","Nonce":""}'  -X POST  http://127.0.0.1:10000/action/v1.0/SendWalletNonce
````
+ **ws Example**

````
{"RequestNo":2,"SendWalletNonce":{"UserID":"1983723","MemberID":"1983723","Nonce":""}}
````
+ **Web rowser Example**

````
http://127.0.0.1:10000/action/v1.0/SendWalletNonce?UserID=1983723&MemberID=1983723&Nonce=
````
+ **Request Parameters**

parameter | shortName | required | type | description | remark
-------- | -------- | -------- | -------- | -------- | --------
UserID | UserID | no | string | 交易用户代码 | 
MemberID | MemberID | no | string | 成员代码 | 
Nonce | Nonce | no | string | 随机数 | 

+ **Response**

```
{
	"action": "RecvWalletNonce",
	"errorCode": 0,
	"errorMsg": "Success",
	"result": [			
				{"table":"WalletNonce",
				 "data":{
					,UserID:"1983723"
					,MemberID:"1983723"
					,Nonce:""
					}
				}
	]
}
```

+ **Response Parameters**

WalletNonce:

parameter | shortName | type | description | remark
-------- | -------- | -------- | -------- | -------- 					
UserID | UserID | string | 交易用户代码 | 
MemberID | MemberID | string | 成员代码 | 
Nonce | Nonce | string | 随机数 | 


## **钱包登录获取随机数请求**

+ **Request URL**

`POST /action/v1.0/SendWalletLogin`


+ **Request Post Body**

```
{
	,"UserID":"1983723"
	,"Signature":""
}
```
+ **curl Example**

````
curl -d '{"UserID":"1983723","Signature":""}'  -X POST  http://127.0.0.1:10000/action/v1.0/SendWalletLogin
````
+ **ws Example**

````
{"RequestNo":2,"SendWalletLogin":{"UserID":"1983723","Signature":""}}
````
+ **Web rowser Example**

````
http://127.0.0.1:10000/action/v1.0/SendWalletLogin?UserID=1983723&Signature=
````
+ **Request Parameters**

parameter | shortName | required | type | description | remark
-------- | -------- | -------- | -------- | -------- | --------
UserID | UserID | no | string | 交易用户代码 | 
Signature | Signature | no | string | 签名 | 

+ **Response**

```
{
	"action": "RecvWalletLogin",
	"errorCode": 0,
	"errorMsg": "Success",
	"result": [			
				{"table":"UserSession",
				 "data":{
					,UserID:"1983723"
					,Password:""
					,UserProductID:"1983723"
					,MacAddress:""
					,ClientIPAddress:""
					,HDSerialID:"1983723"
					,AuthCode:""
					,APPID:"ios"
					,Token:""
					,Remark:"my_little_tips"
					,APIID:"1983723"
					,LoginTime:1657543832
					,ExpireTime:1657543832
					,IPAddress:""
					,MaxLocalID:"1983723"
					,SessionNo:1033201232
					,FrontNo:1
					,AccessLimit:0
					,UserType:"1"
					,MemberID:"1983723"
					,LimitAccesses:0
					,SinkType:"1"
					,InsertTime:1657543832
					,UpdateTime:1657543832
					}
				}
	]
}
```

+ **Response Parameters**

UserSession:

parameter | shortName | type | description | remark
-------- | -------- | -------- | -------- | -------- 					
UserID | UserID | string | 交易用户代码 | 
Password | Password | string | 密码 | 
UserProductID | UserProductID | string | 用户端产品信息 | 
MacAddress | MacAddress | string | Mac地址 | 
ClientIPAddress | ClientIPAddress | string | 终端IP地址 | 
HDSerialID | HDSerialID | string | 硬盘序列号 | 
AuthCode | AuthCode | string | 授权编码 | 
APPID | APPID | string | 应用编号 | 
Token | Token | string | 令牌 | 
Remark | Remark | string | 备注 | 
APIID | APIID | string | 接口端产品信息 | 
LoginTime | LoginTime | double | 登录时间 | 
ExpireTime | ExpireTime | double | 失效时间 | 
IPAddress | IPAddress | string | 允许登录的IP地址 | 
MaxLocalID | MaxLocalID | string | 最大本地标识 | 
SessionNo | SessionNo | int | 会话编号 | 
FrontNo | FrontNo | int | 前置编号 | 
AccessLimit | AccessLimit | int | 每秒访问的次数限制 | 
UserType | UserType | string | 用户类型 | 1:普通用户(订阅自己/可下自己订单);2:观察用户OB(订阅其他用户/不可下单);5:超级管理员(订阅所有/可给所有用户下单/可爆仓);
MemberID | MemberID | string | 成员代码 | 
LimitAccesses | LimitAccesses | int | 该秒已经访问的次数 | 
SinkType | SinkType | string | 下游系统同步类型 | 0:同步到数据库;1:同步到Redis不到数据库;2:不同步;
InsertTime | InsertTime | double | 插入时间 | 
UpdateTime | UpdateTime | double | 更新时间 | 


## **资金查询请求**

+ **Request URL**

`POST /action/v1.0/SendQryAccount`


+ **Request Post Body**

```
{
	,"MemberID":"1983723"
	,"AccountID":"1983723"
	,"SettlementGroup":"999"
	,"Currency":"BTC"
}
```
+ **curl Example**

````
curl -d '{"MemberID":"1983723","AccountID":"1983723","SettlementGroup":"999","Currency":"BTC"}'  -X POST  http://127.0.0.1:10000/action/v1.0/SendQryAccount
````
+ **ws Example**

````
{"RequestNo":2,"SendQryAccount":{"MemberID":"1983723","AccountID":"1983723","SettlementGroup":"999","Currency":"BTC"}}
````
+ **Web rowser Example**

````
http://127.0.0.1:10000/action/v1.0/SendQryAccount?MemberID=1983723&AccountID=1983723&SettlementGroup=999&Currency=BTC
````
+ **Request Parameters**

parameter | shortName | required | type | description | remark
-------- | -------- | -------- | -------- | -------- | --------
MemberID | MemberID | no | string | 成员代码 | 
AccountID | AccountID | no | string | 资金账号 | 
SettlementGroup | SettlementGroup | no | string | 结算组 | 
Currency | Currency | no | string | 币种 | 

+ **Response**

```
{
	"action": "RecvQryAccount",
	"errorCode": 0,
	"errorMsg": "Success",
	"result": [			
				{"table":"Account",
				 "data":{
					,SettlementGroup:"999"
					,AccountID:"1983723"
					,Currency:"BTC"
					,MemberID:"1983723"
					,Available:182582.18
					,Withdrawable:182582.18
					,PreBalance:182582.18
					,Balance:182582.18
					,PositionCost:182582.18
					,UseMargin:182582.18
					,CrossMargin:182582.18
					,CloseProfit:10
					,Deposit:182582.18
					,Withdraw:182582.18
					,FrozenMargin:182582.18
					,FrozenMoney:182582.18
					,FrozenFee:182582.18
					,Fee:0.22364
					,MoneyChange:182582.18
					,Reserve:182582.18
					,ReserveAvailable:182582.18
					,ReserveLimit:182582.18
					,ReserveProfit:182582.18
					,ReserveType:"1"
					,ReserveActive:0
					,TotalCloseProfit:182582.18
					,TotalDeposit:182582.18
					,TotalWithdraw:182582.18
					,TotalFee:182582.18
					,TotalMoneyChange:182582.18
					,InsertTime:1657543832
					,UpdateTime:1657543832
					,AccountType:""
					,BusinessNo:11176422300333
					,MarginRateGrade:""
					,TradingRightGrade:""
					,Frequency:0
					,MaintMargin:182582.18
					,UnRealProfit:182582.18
					,CopyProfit:182582.18
					,CFDType:"1"
					,FeeGrade:""
					,CFDGrade:""
					,Borrow:182582.18
					,Remark:"my_little_tips"
					,MaxLocalID:"1983723"
					,CurrValue:182582.18
					,CanReduce:0
					}
				}
	]
}
```

+ **Response Parameters**

Account:

parameter | shortName | type | description | remark
-------- | -------- | -------- | -------- | -------- 					
SettlementGroup | SettlementGroup | string | 结算组编号 | 
AccountID | AccountID | string | 资金账号 | 
Currency | Currency | string | 币种 | 
MemberID | MemberID | string | 成员代码 | 
Available | Available | double | 可用资金 | 
Withdrawable | Withdrawable | double | 可取资金 | 
PreBalance | PreBalance | double | 上次静态权益 | 
Balance | Balance | double | 静态权益 | 
PositionCost | PositionCost | double | 持仓成本 | 
UseMargin | UseMargin | double | 所有占用保证金 | 
CrossMargin | CrossMargin | double | 全仓保证金 | 
CloseProfit | CloseProfit | double | 平仓盈亏 | 
Deposit | Deposit | double | 入金金额 | 
Withdraw | Withdraw | double | 出金金额 | 
FrozenMargin | FrozenMargin | double | 冻结保证金 | 
FrozenMoney | FrozenMoney | double | 冻结资金 | 
FrozenFee | FrozenFee | double | 冻结手续费 | 
Fee | Fee | double | 手续费 | 
MoneyChange | MoneyChange | double | 资金变化 | 
Reserve | Reserve | double | 保留资金 | 
ReserveAvailable | ReserveAvailable | double | 保留资金可用 | 
ReserveLimit | ReserveLimit | double | 保留资金限制 | 
ReserveProfit | ReserveProfit | double | 保留资金盈亏 | 
ReserveType | ReserveType | string | 保留资金类型 | 0:先亏损保留资金再亏损真钱(劣后);1:先亏损真钱再亏损保留资金(优先);2:保留资金不做亏损(抵押金);
ReserveActive | ReserveActive | int | 保留资金是否活跃 | 
TotalCloseProfit | TotalCloseProfit | double | 总平仓盈亏 | 
TotalDeposit | TotalDeposit | double | 总入金金额 | 
TotalWithdraw | TotalWithdraw | double | 总出金金额 | 
TotalFee | TotalFee | double | 总手续费 | 
TotalMoneyChange | TotalMoneyChange | double | 总资金变化 | 
InsertTime | InsertTime | double | 插入时间 | 
UpdateTime | UpdateTime | double | 更新时间 | 
AccountType | AccountType | string | 账户类型 | 
BusinessNo | BusinessNo | double | 业务序列号 | 
MarginRateGrade | MarginRateGrade | string | 保证金等级 | 
TradingRightGrade | TradingRightGrade | string | 权限等级 | 
Frequency | Frequency | int | 每秒更新的次数 | 
MaintMargin | MaintMargin | double | 维持保证金 | 
UnRealProfit | UnRealProfit | double | 未实现盈亏 | 
CopyProfit | CopyProfit | double | 带单盈利分配 | 
CFDType | CFDType | string | 点差接管类型 | 0:先撮合后点差接管;1:只撮合不点差接管;2:不撮合只点差接管;
FeeGrade | FeeGrade | string | 手续费等级 | 
CFDGrade | CFDGrade | string | 差价等级 | 
Borrow | Borrow | double | 借款周期结算量 | 
Remark | Remark | string | 备注 | 
MaxLocalID | MaxLocalID | string | 最大本地标识 | 
CurrValue | CurrValue | double | 当前价值 | 
CanReduce | CanReduce | int | 是否可以出金 | 


## **报单查询请求**

+ **Request URL**

`POST /action/v1.0/SendQryOrder`


+ **Request Post Body**

```
{
	,"MemberID":"1983723"
	,"AccountID":"1983723"
	,"ExchangeID":"CEX"
	,"InstrumentID":"BTCUSD"
	,"LocalID":"1983723"
	,"OrderID":"12376422300333"
}
```
+ **curl Example**

````
curl -d '{"MemberID":"1983723","AccountID":"1983723","ExchangeID":"CEX","InstrumentID":"BTCUSD","LocalID":"1983723","OrderID":"12376422300333"}'  -X POST  http://127.0.0.1:10000/action/v1.0/SendQryOrder
````
+ **ws Example**

````
{"RequestNo":2,"SendQryOrder":{"MemberID":"1983723","AccountID":"1983723","ExchangeID":"CEX","InstrumentID":"BTCUSD","LocalID":"1983723","OrderID":"12376422300333"}}
````
+ **Web rowser Example**

````
http://127.0.0.1:10000/action/v1.0/SendQryOrder?MemberID=1983723&AccountID=1983723&ExchangeID=CEX&InstrumentID=BTCUSD&LocalID=1983723&OrderID=12376422300333
````
+ **Request Parameters**

parameter | shortName | required | type | description | remark
-------- | -------- | -------- | -------- | -------- | --------
MemberID | MemberID | no | string | 成员代码 | 
AccountID | AccountID | no | string | 资金账号 | 
ExchangeID | ExchangeID | no | string | 交易所代码 | 
InstrumentID | InstrumentID | no | string | 标的代码 | 
LocalID | LocalID | no | string | 报单本地标识 | 
OrderID | OrderID | no | string | 订单号 | 

+ **Response**

```
{
	"action": "RecvQryOrder",
	"errorCode": 0,
	"errorMsg": "Success",
	"result": [			
				{"table":"Order",
				 "data":{
					,MemberID:"1983723"
					,TradeUnitID:"1983723"
					,AccountID:"1983723"
					,LocalID:"1983723"
					,ExchangeID:"CEX"
					,InstrumentID:"BTCUSD"
					,OrderPriceType:"1"
					,Direction:"1"
					,OffsetFlag:"1"
					,Price:182582.18
					,Volume:182582.18
					,VolumeDisplay:182582.18
					,VolumeMode:"1"
					,Cost:182582.18
					,OrderType:"1"
					,GTDTime:1657543832
					,MinVolume:182582.18
					,BusinessType:"1"
					,BusinessValue:""
					,CloseOrderID:"1983723"
					,IsCrossMargin:1
					,Remark:"my_little_tips"
					,OrderID:"12376422300333"
					,CopyMemberID:"1983723"
					,CopyOrderID:"1983723"
					,Leverage:125
					,CopyProfitRate:182582.18
					,APPID:"ios"
					,PositionID:"1983723"
					,TriggerPrice:182582.18
					,Reserve:182582.18
					,OrderStatus:"1"
					,DeriveSource:"1"
					,DeriveDetail:""
					,VolumeTraded:182582.18
					,VolumeRemain:182582.18
					,VolumeCancled:182582.18
					,InsertTime:1657543832
					,UpdateTime:1657543832
					,Priority:0
					,TimeSortNo:1657543832
					,FrontNo:1
					,PriceCurrency:"BTC"
					,FeeCurrency:"BTC"
					,ClearCurrency:"BTC"
					,FrozenMoney:182582.18
					,FrozenFee:182582.18
					,FrozenMargin:182582.18
					,Fee:0.22364
					,CloseProfit:10
					,Turnover:182582.18
					,RelatedOrderID:"1983723"
					,BusinessResult:""
					,BusinessNo:11176422300333
					,Tradable:0
					,SettlementGroup:"999"
					,PosiDirection:"1"
					,TradePrice:182582.18
					,OpenPrice:182582.18
					,TriggerOrderID:"1983723"
					,SLTriggerPrice:182582.18
					,TPTriggerPrice:182582.18
					,CopyProfit:182582.18
					,Position:182582.18
					,UserID:"1983723"
					,LastPriceByInsert:182582.18
					,BidPrice1ByInsert:182582.18
					,AskPrice1ByInsert:182582.18
					,Available:182582.18
					,CreateTime:""
					}
				}
	]
}
```

+ **Response Parameters**

Order:

parameter | shortName | type | description | remark
-------- | -------- | -------- | -------- | -------- 					
MemberID | M | string | 成员代码 | 
TradeUnitID | T | string | 交易单元代码 | 
AccountID | A | string | 资金账号 | 
LocalID | L | string | 报单本地标识 | 
ExchangeID | E | string | 交易所代码 | 
InstrumentID | I | string | 标的代码 | 
OrderPriceType | PT | string | 报单价格类型 | 0:限价(手动指定Price值);1:任意价(Price按最高最低限价取值);2:最优价/对手价(Price按对方1档价取值);3:五档价(Price按对方5档价取值);4:十档价(类同上);7:最优Marker价(Price按本方1档价或其+1Tick取值);8:Price按委托额Cost和订单簿情况确定值;9:保底点差价:成交价格Price不会劣于系统设置差价成交;
Direction | D | string | 买卖方向 | 0:买;1:卖;
OffsetFlag | O | string | 开平标志 | 0:开仓;1:平仓;2:强平;3:平今;4:平昨;5:全平;6:指定订单平仓;7:指定成交平仓;8:最大平仓;
Price | P | double | 报单价格 | 
Volume | V | double | 数量 | 
VolumeDisplay | D | double | 显示数量 | 
VolumeMode | VM | string | 数量取值方式 | 0:百分比;1:绝对值;
Cost | C | double | 委托额 | 
OrderType | OT | string | 订单类型 | 0:普通订单;1:(FillAndKill/IOC)立即完成并且剩余撤销;2:(FillOrKill)立即完成全部或者最小订单要求否则全部撤销;3:需要进入订单簿被动成交,否则全部撤销(只做Maker/PostOnly);4:不需要撮合直接成交;
GTDTime | GT | double | GTD时间 | 
MinVolume | MV | double | 最小成交量 | 
BusinessType | BT | string | 业务类别 | 0:普通;1:申赎;2:行权;3:中立仓申报;4:递延交割申报;5:互换定单;6:质押;7:合并分拆;8:转股;9:回售回购;a:投票;b:融资融券;c:期权执行;d:传递被平仓订单BusinessValue;P:自定义11;Q:自定义10;R:自定义9;S:连续交易;T:自定义7;U:画线委托;V:限价市价;W:条件;X:止盈止损;Y:追踪出场;Z:指标类型;
BusinessValue | BV | string | 业务值 | 
CloseOrderID | CI | string | 平仓指定开仓的订单号 | 
IsCrossMargin | IsCrossMargin | int | 是否全仓 | 
Remark | R | string | 备注 | 
OrderID | OrderID | string | 报单系统唯一代码 | 
CopyMemberID | CopyMemberID | string | 带单员代码 | 
CopyOrderID | CopyOrderID | string | 带单员报单号 | 
Leverage | Leverage | double | 委托单杠杆倍数 | 
CopyProfitRate | CopyProfitRate | double | 带单分配比例 | 
APPID | APPID | string | 应用编号 | 
PositionID | B | string | 持仓代码 | 
TriggerPrice | TriggerPrice | double | 触发价 | 
Reserve | Reserve | double | 保留资金 | 
OrderStatus | OrderStatus | string | 报单状态 | 0:未设置状态;1:全部成交;2:部分成交未撤单;3:部分成交已撤单;4:未成交未撤单;6:无成交已撤单;
DeriveSource | DeriveSource | string | 衍生来源 | 0:正常;1:报价衍生;2:期权执行;3:组合衍生;4:场外成交强平衍生;5:场外成交大宗交易衍生;6:场外成交期转现衍生;7:场外成交只刷单衍生;8:触发订单衍生;9:止损触发订单衍生;a:止盈触发订单衍生;b:资金操作衍生;c:仓位合并衍生;d:CFD订单衍生;e:场外成交ADL衍生;f:回购手续费不能是币;
DeriveDetail | DeriveDetail | string | 衍生明细 | 
VolumeTraded | VolumeTraded | double | 成交数量 | 
VolumeRemain | VolumeRemain | double | 剩余数量 | 
VolumeCancled | VolumeCancled | double | 已经撤单数量 | 
InsertTime | InsertTime | double | 插入时间 | 
UpdateTime | UpdateTime | double | 更新时间 | 
Priority |  | int | 优先权 | 
TimeSortNo |  | double | 按时间排队的序号 | 
FrontNo | FrontNo | int | 前置编号 | 
PriceCurrency | PriceCurrency | string | 计价币种 | 
FeeCurrency | FeeCurrency | string | 手续费币种 | 
ClearCurrency | ClearCurrency | string | 清算币种 | 
FrozenMoney | FrozenMoney | double | 冻结资金 | 
FrozenFee | FrozenFee | double | 冻结手续费 | 
FrozenMargin | FrozenMargin | double | 冻结保证金 | 
Fee | Fee | double | 手续费 | 
CloseProfit | CloseProfit | double | 平仓盈亏 | 
Turnover | Turnover | double | 成交金额 | 
RelatedOrderID | RelatedOrderID | string | 关联报单号 | 
BusinessResult | BusinessResult | string | 业务执行结果 | 
BusinessNo | BusinessNo | double | 业务序列号 | 
Tradable |  | int | 是否可以参加交易 | 
SettlementGroup |  | string | 结算组编号 | 
PosiDirection | PosiDirection | string | 持仓多空方向 | 0:多头;1:空头;2:净;
TradePrice | TradePrice | double | 成交均价 | 
OpenPrice | OpenPrice | double | 平仓成交时的开仓均价 | 
TriggerOrderID | TriggerOrderID | string | 触发报单号 | 
SLTriggerPrice | SLTriggerPrice | double | 开仓报单成交之后的止损触发价 | 
TPTriggerPrice | TPTriggerPrice | double | 开仓报单成交之后的止盈触发价 | 
CopyProfit | CopyProfit | double | 带单盈利分配 | 
Position | Position | double | 成交时的持仓量 | 
UserID | U | string | 交易用户代码 | 
LastPriceByInsert |  | double | 插入时的最新价 | 
BidPrice1ByInsert |  | double | 插入时的买一价 | 
AskPrice1ByInsert |  | double | 插入时的卖一价 | 
Available | Available | double | 可用资金 | 
CreateTime |  | string | 创建时间 | 


## **持仓汇总查询请求**

+ **Request URL**

`POST /action/v1.0/SendQryMemberPosition`


+ **Request Post Body**

```
{
	,"MemberID":"1983723"
	,"AccountID":"1983723"
	,"ExchangeID":"CEX"
	,"InstrumentID":"BTCUSD"
}
```
+ **curl Example**

````
curl -d '{"MemberID":"1983723","AccountID":"1983723","ExchangeID":"CEX","InstrumentID":"BTCUSD"}'  -X POST  http://127.0.0.1:10000/action/v1.0/SendQryMemberPosition
````
+ **ws Example**

````
{"RequestNo":2,"SendQryMemberPosition":{"MemberID":"1983723","AccountID":"1983723","ExchangeID":"CEX","InstrumentID":"BTCUSD"}}
````
+ **Web rowser Example**

````
http://127.0.0.1:10000/action/v1.0/SendQryMemberPosition?MemberID=1983723&AccountID=1983723&ExchangeID=CEX&InstrumentID=BTCUSD
````
+ **Request Parameters**

parameter | shortName | required | type | description | remark
-------- | -------- | -------- | -------- | -------- | --------
MemberID | MemberID | no | string | 成员代码 | 
AccountID | AccountID | no | string | 资金账号 | 
ExchangeID | ExchangeID | no | string | 交易所代码 | 
InstrumentID | InstrumentID | no | string | 标的代码 | 

+ **Response**

```
{
	"action": "RecvQryMemberPosition",
	"errorCode": 0,
	"errorMsg": "Success",
	"result": [			
				{"table":"MemberPosition",
				 "data":{
					,MemberID:"1983723"
					,AccountID:"1983723"
					,ExchangeID:"CEX"
					,InstrumentID:"BTCUSD"
					,Position:182582.18
					,LongFrozen:182582.18
					,ShortFrozen:182582.18
					,DefaultLeverage:125
					,DefaultCrossMargin:0
					,InsertTime:1657543832
					,UpdateTime:1657543832
					}
				}
	]
}
```

+ **Response Parameters**

MemberPosition:

parameter | shortName | type | description | remark
-------- | -------- | -------- | -------- | -------- 					
MemberID | MemberID | string | 成员代码 | 
AccountID | AccountID | string | 资金账号 | 
ExchangeID | ExchangeID | string | 交易所代码 | 
InstrumentID | InstrumentID | string | 标的代码 | 
Position | Position | double | 总持仓 | 
LongFrozen | LongFrozen | double | 多头冻结 | 
ShortFrozen | ShortFrozen | double | 空头冻结 | 
DefaultLeverage | DefaultLeverage | double | 默认杠杆倍数 | 
DefaultCrossMargin | DefaultCrossMargin | int | 默认是否全仓 | 
InsertTime | InsertTime | double | 插入时间 | 
UpdateTime | UpdateTime | double | 更新时间 | 


## **持仓查询请求**

+ **Request URL**

`POST /action/v1.0/SendQryPosition`


+ **Request Post Body**

```
{
	,"MemberID":"1983723"
	,"AccountID":"1983723"
	,"ExchangeID":"CEX"
	,"InstrumentID":"BTCUSD"
	,"PositionClass":"1"
}
```
+ **curl Example**

````
curl -d '{"MemberID":"1983723","AccountID":"1983723","ExchangeID":"CEX","InstrumentID":"BTCUSD","PositionClass":"1"}'  -X POST  http://127.0.0.1:10000/action/v1.0/SendQryPosition
````
+ **ws Example**

````
{"RequestNo":2,"SendQryPosition":{"MemberID":"1983723","AccountID":"1983723","ExchangeID":"CEX","InstrumentID":"BTCUSD","PositionClass":"1"}}
````
+ **Web rowser Example**

````
http://127.0.0.1:10000/action/v1.0/SendQryPosition?MemberID=1983723&AccountID=1983723&ExchangeID=CEX&InstrumentID=BTCUSD&PositionClass=1
````
+ **Request Parameters**

parameter | shortName | required | type | description | remark
-------- | -------- | -------- | -------- | -------- | --------
MemberID | MemberID | no | string | 成员代码 | 
AccountID | AccountID | no | string | 资金账号 | 
ExchangeID | ExchangeID | no | string | 交易所代码 | 
InstrumentID | InstrumentID | no | string | 标的代码 | 
PositionClass | PositionClass | no | string | 持仓大类 | 0:不设置;1:保证金交易;3:资产;4:持币成本;

+ **Response**

```
{
	"action": "RecvQryPosition",
	"errorCode": 0,
	"errorMsg": "Success",
	"result": [			
				{"table":"Position",
				 "data":{
					,PositionID:"1983723"
					,MemberID:"1983723"
					,TradeUnitID:"1983723"
					,ExchangeID:"CEX"
					,InstrumentID:"BTCUSD"
					,PositionClass:"1"
					,PosiDirection:"1"
					,PrePosition:182582.18
					,Position:182582.18
					,LongFrozen:182582.18
					,ShortFrozen:182582.18
					,PreLongFrozen:182582.18
					,PreShortFrozen:182582.18
					,HighestPosition:182582.18
					,ClosePosition:182582.18
					,PositionCost:182582.18
					,CostPrice:182582.18
					,UseMargin:182582.18
					,FrozenMargin:182582.18
					,LongFrozenMargin:182582.18
					,ShortFrozenMargin:182582.18
					,CloseProfit:10
					,TotalPositionCost:182582.18
					,TotalCloseProfit:182582.18
					,OpenPrice:182582.18
					,ClosePrice:182582.18
					,TradeFee:182582.18
					,PositionFee:182582.18
					,Leverage:125
					,AccountID:"1983723"
					,PriceCurrency:"BTC"
					,ClearCurrency:"BTC"
					,SettlementGroup:"999"
					,IsCrossMargin:1
					,CloseOrderID:"1983723"
					,SLTriggerPrice:182582.18
					,TPTriggerPrice:182582.18
					,BeginTime:1657543832
					,InsertTime:1657543832
					,LastOpenTime:1657543832
					,UpdateTime:1657543832
					,BusinessNo:11176422300333
					,IsAutoAddMargin:1
					,Frequency:0
					,MaintMargin:182582.18
					,UnRealProfit:182582.18
					,LiquidPrice:182582.18
					,CreateTime:""
					,CopyMemberID:"1983723"
					,CopyProfitRate:182582.18
					,CopyProfit:182582.18
					,FirstTradeID:"1983723"
					,LastTradeID:"1983723"
					,BusinessType:"1"
					,BusinessValue:""
					,Reserve:182582.18
					,ReserveProfit:182582.18
					,Remark:"my_little_tips"
					}
				}
	]
}
```

+ **Response Parameters**

Position:

parameter | shortName | type | description | remark
-------- | -------- | -------- | -------- | -------- 					
PositionID | PositionID | string | 持仓代码 | 
MemberID | MemberID | string | 成员代码 | 
TradeUnitID | TradeUnitID | string | 交易单元代码 | 
ExchangeID | ExchangeID | string | 交易所代码 | 
InstrumentID | InstrumentID | string | 标的代码 | 
PositionClass | PositionClass | string | 持仓类型 | 0:不设置;1:保证金交易;3:资产;4:持币成本;
PosiDirection | PosiDirection | string | 持仓多空方向 | 0:多头;1:空头;2:净;
PrePosition | PrePosition | double | 上次持仓 | 
Position | Position | double | 总持仓 | 
LongFrozen | LongFrozen | double | 多头冻结 | 
ShortFrozen | ShortFrozen | double | 空头冻结 | 
PreLongFrozen | PreLongFrozen | double | 昨日多头冻结 | 
PreShortFrozen | PreShortFrozen | double | 昨日空头冻结 | 
HighestPosition | HighestPosition | double | 最大持仓 | 
ClosePosition | ClosePosition | double | 可平持仓 | 
PositionCost | PositionCost | double | 持仓成本 | 
CostPrice | CostPrice | double | 成本价 | 
UseMargin | UseMargin | double | 占用保证金 | 
FrozenMargin | FrozenMargin | double | 冻结保证金 | 
LongFrozenMargin | LongFrozenMargin | double | 多头冻结保证金 | 
ShortFrozenMargin | ShortFrozenMargin | double | 空头冻结保证金 | 
CloseProfit | CloseProfit | double | 平仓盈亏 | 
TotalPositionCost | TotalPositionCost | double | 开仓成本 | 
TotalCloseProfit | TotalCloseProfit | double | 总平仓盈亏 | 
OpenPrice | OpenPrice | double | 开仓均价 | 
ClosePrice | ClosePrice | double | 平仓均价 | 
TradeFee | TradeFee | double | 交易费用 | 
PositionFee | PositionFee | double | 持仓费用或者股票分红等 | 
Leverage | Leverage | double | 杠杆倍数 | 
AccountID | AccountID | string | 资金账号 | 
PriceCurrency | PriceCurrency | string | 计价币种 | 
ClearCurrency | ClearCurrency | string | 清算币种 | 
SettlementGroup | SettlementGroup | string | 结算组编号 | 
IsCrossMargin | IsCrossMargin | int | 是否全仓 | 
CloseOrderID | CloseOrderID | string | 平仓单代码 | 
SLTriggerPrice | SLTriggerPrice | double | 止损触发价 | 
TPTriggerPrice | TPTriggerPrice | double | 止盈触发价 | 
BeginTime | BeginTime | double | 持仓不为0的开始时间 | 
InsertTime | InsertTime | double | 插入时间(新持仓时间) | 
LastOpenTime | LastOpenTime | double | 最后开仓时间 | 
UpdateTime | UpdateTime | double | 更新时间 | 
BusinessNo | BusinessNo | double | 业务序列号 | 
IsAutoAddMargin | IsAutoAddMargin | int | 是否自动追加保证金 | 
Frequency | Frequency | int | 每秒更新的次数 | 
MaintMargin | MaintMargin | double | 维持保证金 | 
UnRealProfit | UnRealProfit | double | 未实现盈亏 | 
LiquidPrice | LiquidPrice | double | 清算价格 | 
CreateTime | CreateTime | string | 创建时间 | 
CopyMemberID | CopyMemberID | string | 带单员代码 | 
CopyProfitRate | CopyProfitRate | double | 带单分配比例 | 
CopyProfit | CopyProfit | double | 带单盈利分配 | 
FirstTradeID | FID | string | 初始成交订单号 | 
LastTradeID | LID | string | 结束成交订单号 | 
BusinessType | BT | string | 业务类别 | 0:普通;1:申赎;2:行权;3:中立仓申报;4:递延交割申报;5:互换定单;6:质押;7:合并分拆;8:转股;9:回售回购;a:投票;b:融资融券;c:期权执行;d:传递被平仓订单BusinessValue;P:自定义11;Q:自定义10;R:自定义9;S:连续交易;T:自定义7;U:画线委托;V:限价市价;W:条件;X:止盈止损;Y:追踪出场;Z:指标类型;
BusinessValue | BV | string | 业务值 | 
Reserve | Reserve | double | 保留资金 | 
ReserveProfit | ReserveProfit | double | 保留资金盈亏 | 
Remark | R | string | 备注 | 


## **标的查询请求**

+ **Request URL**

`POST /action/v1.0/SendQryInstrument`


+ **Request Post Body**

```
{
	,"ExchangeID":"CEX"
	,"ProductGroup":"Swap"
	,"InstrumentID":"BTCUSD"
}
```
+ **curl Example**

````
curl -d '{"ExchangeID":"CEX","ProductGroup":"Swap","InstrumentID":"BTCUSD"}'  -X POST  http://127.0.0.1:10000/action/v1.0/SendQryInstrument
````
+ **ws Example**

````
{"RequestNo":2,"SendQryInstrument":{"ExchangeID":"CEX","ProductGroup":"Swap","InstrumentID":"BTCUSD"}}
````
+ **Web rowser Example**

````
http://127.0.0.1:10000/action/v1.0/SendQryInstrument?ExchangeID=CEX&ProductGroup=Swap&InstrumentID=BTCUSD
````
+ **Request Parameters**

parameter | shortName | required | type | description | remark
-------- | -------- | -------- | -------- | -------- | --------
ExchangeID | ExchangeID | no | string | 交易所代码 | 
ProductGroup | ProductGroup | no | string | 产品组 | 
InstrumentID | InstrumentID | no | string | 标的代码 | 

+ **Response**

```
{
	"action": "RecvQryInstrument",
	"errorCode": 0,
	"errorMsg": "Success",
	"result": [			
				{"table":"Instrument",
				 "data":{
					,ExchangeID:"CEX"
					,InstrumentID:"BTCUSD"
					,InstrumentName:""
					,SettlementGroup:"999"
					,ProductGroup:"Swap"
					,UnderlyingID:"1983723"
					,ProductClass:"1"
					,ProductType:"1"
					,StrikePrice:182582.18
					,OptionsType:"1"
					,VolumeMultiple:182582.18
					,StartTime:1657543832
					,ExpireTime:1657543832
					,PriceCurrency:"BTC"
					,ClearCurrency:"BTC"
					,IsInverse:1
					,BaseCurrency:"BTC"
					,MarginPriceType:"1"
					,BasisPrice:182582.18
					,MinOrderVolume:182582.18
					,MinOrderCost:182582.18
					,MaxOrderVolume:182582.18
					,PriceTick:182582.18
					,VolumeTick:182582.18
					,ShowVolumeTick:182582.18
					,TradingModel:"1"
					,DayStartTime:""
					,SortNumber:-1
					,Remark:"my_little_tips"
					,DefaultLeverage:125
					,PriceLimitValueMode:"1"
					,PriceLimitPrice:"1"
					,PriceLimitUpperValue:182582.18
					,PriceLimitLowerValue:182582.18
					,MarginRateGroup:"999"
					,CFDGroup:"999"
					,FeeGroup:"999"
					,TradingRightGroup:"999"
					,MaxOpenInterest:182582.18
					,FundingRateGroup:"999"
					,InsertTime:1657543832
					,UpdateTime:1657543832
					,MarkedPriceTimes:0
					}
				}
	]
}
```

+ **Response Parameters**

Instrument:

parameter | shortName | type | description | remark
-------- | -------- | -------- | -------- | -------- 					
ExchangeID | X | string | 交易所代码 | 
InstrumentID | I | string | 标的代码 | 
InstrumentName | N | string | 标的名称 | 
SettlementGroup |  | string | 结算组编号 | 
ProductGroup | G | string | 产品组 | 
UnderlyingID |  | string | 基础标的代码 | 
ProductClass |  | string | 产品大类 | 1:保证金交易;2:权利金交易;3:资产转让（股票/基金/国债等）;4:兑换交易（资金之间的互换）;5:组合;6:行情;
ProductType |  | string | 产品类型 | 1:交割期货;2:永续、利率互换期货;3:即期/日交割期货;4:递延交割期货;5:权证;6:美式期权;7:欧式期权;8:币币交易;9:外汇兑换;a:股票;b:债券;c:基金;
StrikePrice |  | double | 执行价 | 
OptionsType |  | string | 期权类型 | 0:非期权;1:看涨;2:看跌;
VolumeMultiple | VM | double | 数量乘数 | 
StartTime |  | double | 启动时间 | 
ExpireTime |  | double | 到期时间 | 
PriceCurrency | PC | string | 计价币种 | 
ClearCurrency | CC | string | 清算币种 | 
IsInverse | II | int | 是否反向标的 | 
BaseCurrency | BB | string | 标的基础货币 | 
MarginPriceType |  | string | 保证金价格类型 | 0:不按比例按固定值,无需价格;1:昨结算价;2:开仓价;3:标记价;4:最新价;
BasisPrice |  | double | 挂牌基准价 | 
MinOrderVolume | MOV | double | 最小下单量 | 
MinOrderCost | MOC | double | 最小下单额 | 
MaxOrderVolume |  | double | 最大下单量 | 
PriceTick | PT | double | 最小变动价位 | 
VolumeTick | VT | double | 最小变动量 | 
ShowVolumeTick |  | double | 前端行情显示最小变动量(服务端不用) | 
TradingModel |  | string | 交易模式 | 0:价格优先时间优先;1:价格优先按比例分配;2:只能和做市商报价成交;3:没有撮合;
DayStartTime |  | string | 每日开始交易时间 | 
SortNumber |  | int | 排序字段 | 
Remark | RE | string | 备注 | 
DefaultLeverage | DL | double | 默认杠杆倍数 | 
PriceLimitValueMode |  | string | 取值方式 | 0:百分比;1:绝对值;
PriceLimitPrice |  | string | 限价价格类型 | 1:昨结算价;2:最新价;3:基础标的价;4:标记价;
PriceLimitUpperValue |  | double | 上限 | 
PriceLimitLowerValue |  | double | 下限 | 
MarginRateGroup |  | string | 保证金组 | 
CFDGroup |  | string | 差价组 | 
FeeGroup |  | string | 手续费组 | 
TradingRightGroup |  | string | 权限组 | 
MaxOpenInterest |  | double | 最大持仓量 | 
FundingRateGroup |  | string | 资金费用组 | 
InsertTime |  | double | 插入时间 | 
UpdateTime |  | double | 更新时间 | 
MarkedPriceTimes |  | int | 标记价格计算次数(0:最新价;1-n:次数;空:不生成标记价) | 


## **币种查询请求**

+ **Request URL**

`POST /action/v1.0/SendQryCurrency`


+ **Request Post Body**

```
{
	,"SettlementGroup":"999"
	,"Currency":"BTC"
}
```
+ **curl Example**

````
curl -d '{"SettlementGroup":"999","Currency":"BTC"}'  -X POST  http://127.0.0.1:10000/action/v1.0/SendQryCurrency
````
+ **ws Example**

````
{"RequestNo":2,"SendQryCurrency":{"SettlementGroup":"999","Currency":"BTC"}}
````
+ **Web rowser Example**

````
http://127.0.0.1:10000/action/v1.0/SendQryCurrency?SettlementGroup=999&Currency=BTC
````
+ **Request Parameters**

parameter | shortName | required | type | description | remark
-------- | -------- | -------- | -------- | -------- | --------
SettlementGroup | SettlementGroup | no | string | 结算组 | 
Currency | Currency | no | string | 币种 | 

+ **Response**

```
{
	"action": "RecvQryCurrency",
	"errorCode": 0,
	"errorMsg": "Success",
	"result": [			
				{"table":"Currency",
				 "data":{
					,SettlementGroup:"999"
					,Currency:"BTC"
					,Deposit:182582.18
					,Withdraw:182582.18
					,Balance:182582.18
					,PreBalance:182582.18
					,SortNumber:-1
					,CanReduce:0
					,IsTrading:1
					,SettleSegment:""
					,MoneyTick:182582.18
					,ExchangeID:"CEX"
					,InstrumentID:"BTCUSD"
					,LoanUse:182582.18
					,LoanLimit:182582.18
					,LoanBorrow:182582.18
					,LoanDeposit:182582.18
					,LoanBase:182582.18
					,LoanRate:182582.18
					,LoanHighBase:182582.18
					,LoanHighRate:182582.18
					,Remark:"my_little_tips"
					,CurrValue:182582.18
					,HairCut:182582.18
					,MaxValue:182582.18
					,MarginRateGroup:"999"
					,PrivateEnd:182582.18
					,InsertTime:1657543832
					,UpdateTime:1657543832
					}
				}
	]
}
```

+ **Response Parameters**

Currency:

parameter | shortName | type | description | remark
-------- | -------- | -------- | -------- | -------- 					
SettlementGroup | S | string | 结算组编号 | 
Currency | C | string | 币种 | 
Deposit |  | double | 入金金额 | 
Withdraw |  | double | 出金金额 | 
Balance | B | double | 静态权益 | 
PreBalance |  | double | 上次静态权益 | 
SortNumber |  | int | 排序字段 | 
CanReduce |  | int | 是否可以出金 | 
IsTrading |  | int | 是否允许相关交易 | 
SettleSegment |  | string | 结算段 | 
MoneyTick |  | double | 最小单位 | 
ExchangeID |  | string | 价格参考交易所代码 | 
InstrumentID |  | string | 价格参考标的代码 | 
LoanUse | LU | double | 借款使用量 | 
LoanLimit |  | double | 最大可借比例 | 
LoanBorrow | LB | double | 借款利息 | 
LoanDeposit | LD | double | 存款利息 | 
LoanBase |  | double | 利息起点 | 
LoanRate |  | double | 利息斜率 | 
LoanHighBase |  | double | 高利息起点 | 
LoanHighRate |  | double | 高利息斜率 | 
Remark | RE | string | 备注 | 
CurrValue | V | double | 当前价值 | 
HairCut |  | double | 抵押比例 | 
MaxValue |  | double | 最大抵押数额 | 
MarginRateGroup |  | string | 保证金组 | 
PrivateEnd |  | double | 私有化停止交易线 | 
InsertTime |  | double | 插入时间 | 
UpdateTime |  | double | 更新时间 | 


## **成员查询请求**

+ **Request URL**

`POST /action/v1.0/SendQryMember`


+ **Request Post Body**

```
{
	,"MemberType":"1"
	,"MemberID":"1983723"
}
```
+ **curl Example**

````
curl -d '{"MemberType":"1","MemberID":"1983723"}'  -X POST  http://127.0.0.1:10000/action/v1.0/SendQryMember
````
+ **ws Example**

````
{"RequestNo":2,"SendQryMember":{"MemberType":"1","MemberID":"1983723"}}
````
+ **Web rowser Example**

````
http://127.0.0.1:10000/action/v1.0/SendQryMember?MemberType=1&MemberID=1983723
````
+ **Request Parameters**

parameter | shortName | required | type | description | remark
-------- | -------- | -------- | -------- | -------- | --------
MemberType | MemberType | no | string | 成员类型 | a:普通;b:内部做市商;c:系统手续费使用;d:风控使用;e:运营使用;f:运营保留资金使用;g:外部做市商;
MemberID | MemberID | no | string | 成员代码 | 

+ **Response**

```
{
	"action": "RecvQryMember",
	"errorCode": 0,
	"errorMsg": "Success",
	"result": [			
				{"table":"Member",
				 "data":{
					,MemberID:"1983723"
					,MemberType:"1"
					,Grade:""
					,Priority:0
					,FeeGrade:""
					,MarginRateGrade:""
					,TradingRightGrade:""
					,CFDGrade:""
					,LiquidityGrade:""
					,RiskLevel:"1"
					,PositionType:"1"
					,TriggerOrders:0
					,OpenOrders:0
					,OpenPositions:0
					,MaxLocalID:"1983723"
					,SinkType:"1"
					,CopyMemberID:"1983723"
					,Region:""
					,FeeDeduceDisCount:182582.18
					,FeeDeduceCurrency:"BTC"
					,CanBorrow:0
					,Remark:"my_little_tips"
					,InsertTime:1657543832
					,UpdateTime:1657543832
					}
				}
	]
}
```

+ **Response Parameters**

Member:

parameter | shortName | type | description | remark
-------- | -------- | -------- | -------- | -------- 					
MemberID | MemberID | string | 成员代码 | 
MemberType | MemberType | string | 成员类型 | a:普通;b:内部做市商;c:系统手续费使用;d:风控使用;e:运营使用;f:运营保留资金使用;g:外部做市商;
Grade | Grade | string | 等级 | 
Priority | Priority | int | 优先权 | 
FeeGrade | FeeGrade | string | 手续费等级 | 
MarginRateGrade | MarginRateGrade | string | 保证金等级 | 
TradingRightGrade | TradingRightGrade | string | 权限等级 | 
CFDGrade | CFDGrade | string | 差价等级 | 
LiquidityGrade | LiquidityGrade | string | 流动性等级 | 
RiskLevel | RiskLevel | string | 风控类型 | 0:本币种资金权益;1:结算组所有资金权益;2:资金账号所有资金权益;3:会员账号所有资金权益;
PositionType | PositionType | string | 持仓类型 | 0:不设置;1:综合持仓(双边持仓);2:净持仓(单边持仓);
TriggerOrders | TriggerOrders | int | 触发订单个数 | 
OpenOrders | OpenOrders | int | 未成交订单数 | 
OpenPositions | OpenPositions | int | 用户持仓个数 | 
MaxLocalID | MaxLocalID | string | 最大本地标识 | 
SinkType | SinkType | string | 下游系统同步类型 | 0:同步到数据库;1:同步到Redis不到数据库;2:不同步;
CopyMemberID | CopyMemberID | string | 带单员代码 | 
Region | Region | string |  | 
FeeDeduceDisCount | FeeDeduceDisCount | double | 作为手续费的抵扣优惠比例 | 
FeeDeduceCurrency | FeeDeduceCurrency | string | 作为手续费的抵扣比例 | 
CanBorrow | CanBorrow | int | 是否需要借钱交易 | 
Remark | Remark | string | 备注 | 
InsertTime | InsertTime | double | 插入时间 | 
UpdateTime | UpdateTime | double | 更新时间 | 


## **价位查询请求**

+ **Request URL**

`POST /action/v1.0/SendQryMarketOrder`

`POST /market/v1.0/SendQryMarketOrder`

+ **Request Post Body**

```
{
	,"ExchangeID":"CEX"
	,"InstrumentID":"BTCUSD"
	,"Direction":"1"
	,"Depth":0
	,"Tick":182582.18
}
```
+ **curl Example**

````
curl -d '{"ExchangeID":"CEX","InstrumentID":"BTCUSD","Direction":"1","Depth":"0,"Tick":"182582.18}'  -X POST  http://127.0.0.1:10000/action/v1.0/SendQryMarketOrder
````
+ **ws Example**

````
{"RequestNo":2,"SendQryMarketOrder":{"ExchangeID":"CEX","InstrumentID":"BTCUSD","Direction":"1","Depth":"0,"Tick":"182582.18}}
````
+ **Web rowser Example**

````
http://127.0.0.1:10000/action/v1.0/SendQryMarketOrder?ExchangeID=CEX&InstrumentID=BTCUSD&Direction=1&Depth=0&Tick=182582.18
````
+ **Request Parameters**

parameter | shortName | required | type | description | remark
-------- | -------- | -------- | -------- | -------- | --------
ExchangeID | ExchangeID | no | string | 交易所代码 | 
InstrumentID | InstrumentID | no | string | 标的代码 | 
Direction | Direction | no | string | 买卖方向 | 0:买;1:卖;
Depth | Depth | no | int | 深度 | 
Tick | Tick | no | double | 最小变动价位 | 

+ **Response**

```
{
	"action": "RecvQryMarketOrder",
	"errorCode": 0,
	"errorMsg": "Success",
	"result": [			
				{"table":"MarketOrder",
				 "data":{
					,ExchangeID:"CEX"
					,InstrumentID:"BTCUSD"
					,Direction:"1"
					,Price:182582.18
					,Volume:182582.18
					,Orders:0
					,BusinessNo:11176422300333
					,TrueVolume:182582.18
					}
				}
	]
}
```

+ **Response Parameters**

MarketOrder:

parameter | shortName | type | description | remark
-------- | -------- | -------- | -------- | -------- 					
ExchangeID |  | string | 交易所代码 | 
InstrumentID |  | string | 标的代码 | 
Direction | D | string | 买卖方向 | 0:买;1:卖;
Price | P | double | 价格 | 
Volume | V | double | 数量 | 
Orders |  | int | 订单数量 | 
BusinessNo |  | double | 最后变化序列号 | 
TrueVolume |  | double | 真实报单数量 | 


## **APIKEY查询请求**

+ **Request URL**

`POST /action/v1.0/SendQryAPIKey`


+ **Request Post Body**

```
{
	,"UserID":"1983723"
}
```
+ **curl Example**

````
curl -d '{"UserID":"1983723"}'  -X POST  http://127.0.0.1:10000/action/v1.0/SendQryAPIKey
````
+ **ws Example**

````
{"RequestNo":2,"SendQryAPIKey":{"UserID":"1983723"}}
````
+ **Web rowser Example**

````
http://127.0.0.1:10000/action/v1.0/SendQryAPIKey?UserID=1983723
````
+ **Request Parameters**

parameter | shortName | required | type | description | remark
-------- | -------- | -------- | -------- | -------- | --------
UserID | UserID | no | string | 交易用户代码 | 

+ **Response**

```
{
	"action": "RecvQryAPIKey",
	"errorCode": 0,
	"errorMsg": "Success",
	"result": [			
				{"table":"APIKey",
				 "data":{
					,Name:""
					,UserID:"1983723"
					,APPID:"ios"
					,MemberID:"1983723"
					,AccessKey:""
					,SecretKey:""
					,Auth:""
					,AccessLimit:0
					,CreateTime:1657543832
					,ExpireTime:1657543832
					,IPAddress:""
					,RemainDays:0
					,Status:0
					}
				}
	]
}
```

+ **Response Parameters**

APIKey:

parameter | shortName | type | description | remark
-------- | -------- | -------- | -------- | -------- 					
Name | Name | string | 备注 | 
UserID | UserID | string | 交易用户代码 | 
APPID | APPID | string | 应用编号 | 
MemberID | MemberID | string | 成员代码 | 
AccessKey | AccessKey | string | 访问密钥 | 
SecretKey | SecretKey | string | 签名加密密钥 | 
Auth | Auth | string | 权限，由8位二进制数组成，0：关闭，1：开通。由高位到低位依次代表：只读;提币;交易。如：10100000表示只读、交易权限开通，提币权限关闭 | 
AccessLimit | AccessLimit | int | 每秒访问的次数限制 | 
CreateTime | CreateTime | double | 创建时间 | 
ExpireTime | ExpireTime | double | 失效时间 | 
IPAddress | IPAddress | string | 绑定IP地址，多个IP地址用半角分号分隔，如：192.168.1.1;192.168.1.2 | 
RemainDays | RemainDays | int | 剩余有效天数 | 
Status | Status | int | 状态，1：正常，0：失效 | 


## **保证金率查询请求**

+ **Request URL**

`POST /action/v1.0/SendQryMarginRate`


+ **Request Post Body**

```
{
	,"MemberID":"1983723"
	,"ExchangeID":"CEX"
	,"InstrumentID":"BTCUSD"
	,"AccountID":"1983723"
}
```
+ **curl Example**

````
curl -d '{"MemberID":"1983723","ExchangeID":"CEX","InstrumentID":"BTCUSD","AccountID":"1983723"}'  -X POST  http://127.0.0.1:10000/action/v1.0/SendQryMarginRate
````
+ **ws Example**

````
{"RequestNo":2,"SendQryMarginRate":{"MemberID":"1983723","ExchangeID":"CEX","InstrumentID":"BTCUSD","AccountID":"1983723"}}
````
+ **Web rowser Example**

````
http://127.0.0.1:10000/action/v1.0/SendQryMarginRate?MemberID=1983723&ExchangeID=CEX&InstrumentID=BTCUSD&AccountID=1983723
````
+ **Request Parameters**

parameter | shortName | required | type | description | remark
-------- | -------- | -------- | -------- | -------- | --------
MemberID | MemberID | yes | string | 成员代码 | 
ExchangeID | ExchangeID | yes | string | 交易所代码 | 
InstrumentID | InstrumentID | yes | string | 标的代码 | 
AccountID | AccountID | no | string | 资金账号 | 

+ **Response**

```
{
	"action": "RecvQryMarginRate",
	"errorCode": 0,
	"errorMsg": "Success",
	"result": [			
				{"table":"MarginRate",
				 "data":{
					,MemberID:"1983723"
					,InstrumentID:"BTCUSD"
					,ValueMode:"1"
					,MarginSequenceType:"1"
					,RiskSequenceType:"1"
					,LongMarginRatio:182582.18
					,LongMaintMarginRatio:182582.18
					,LongMarginStep:182582.18
					,LongMaintMarginStep:182582.18
					,LongRiskLimit:182582.18
					,LongRiskStep:182582.18
					,ShortMarginRatio:182582.18
					,ShortMaintMarginRatio:182582.18
					,ShortMarginStep:182582.18
					,ShortMaintMarginStep:182582.18
					,ShortRiskLimit:182582.18
					,ShortRiskStep:182582.18
					,DefaultLeverage:125
					,Remark:"my_little_tips"
					,InsertTime:1657543832
					,UpdateTime:1657543832
					}
				}
	]
}
```

+ **Response Parameters**

MarginRate:

parameter | shortName | type | description | remark
-------- | -------- | -------- | -------- | -------- 					
MemberID | MemberID | string | 成员代码 | 
InstrumentID | InstrumentID | string | 标的代码 | 
ValueMode | ValueMode | string | 取值方式 | 0:百分比;1:绝对值;
MarginSequenceType | MarginSequenceType | string | 保证金数列 | 1:等差数列;2:等比数列;
RiskSequenceType | RiskSequenceType | string | 风险限额数列 | 1:等差数列;2:等比数列;
LongMarginRatio | LongMarginRatio | double | 多头起始保证金率 | 
LongMaintMarginRatio | LongMaintMarginRatio | double | 多头起始维持保证金率 | 
LongMarginStep | LongMarginStep | double | 多头保证金率步长 | 
LongMaintMarginStep | LongMaintMarginStep | double | 多头维持保证金率步长 | 
LongRiskLimit | LongRiskLimit | double | 多头起始风险限额 | 
LongRiskStep | LongRiskStep | double | 多头风险限额步长 | 
ShortMarginRatio | ShortMarginRatio | double | 空头起始保证金率 | 
ShortMaintMarginRatio | ShortMaintMarginRatio | double | 空头起始维持保证金率 | 
ShortMarginStep | ShortMarginStep | double | 空头保证金率步长 | 
ShortMaintMarginStep | ShortMaintMarginStep | double | 空头维持保证金率步长 | 
ShortRiskLimit | ShortRiskLimit | double | 空头起始风险限额 | 
ShortRiskStep | ShortRiskStep | double | 空头风险限额步长 | 
DefaultLeverage | DefaultLeverage | double | 默认杠杆倍数 | 
Remark | Remark | string | 备注 | 
InsertTime | InsertTime | double | 插入时间 | 
UpdateTime | UpdateTime | double | 更新时间 | 


## **手续费率查询请求**

+ **Request URL**

`POST /action/v1.0/SendQryFee`


+ **Request Post Body**

```
{
	,"MemberID":"1983723"
	,"ExchangeID":"CEX"
	,"InstrumentID":"BTCUSD"
	,"MatchRole":"1"
}
```
+ **curl Example**

````
curl -d '{"MemberID":"1983723","ExchangeID":"CEX","InstrumentID":"BTCUSD","MatchRole":"1"}'  -X POST  http://127.0.0.1:10000/action/v1.0/SendQryFee
````
+ **ws Example**

````
{"RequestNo":2,"SendQryFee":{"MemberID":"1983723","ExchangeID":"CEX","InstrumentID":"BTCUSD","MatchRole":"1"}}
````
+ **Web rowser Example**

````
http://127.0.0.1:10000/action/v1.0/SendQryFee?MemberID=1983723&ExchangeID=CEX&InstrumentID=BTCUSD&MatchRole=1
````
+ **Request Parameters**

parameter | shortName | required | type | description | remark
-------- | -------- | -------- | -------- | -------- | --------
MemberID | MemberID | yes | string | 成员代码 | 
ExchangeID | ExchangeID | yes | string | 交易所代码 | 
InstrumentID | InstrumentID | yes | string | 标的代码 | 
MatchRole | MatchRole | no | string | 成交角色 | 0:不区分;1:主动成交方;2:被动成交方;3:集合竞价;4:交割;5:场外交易;

+ **Response**

```
{
	"action": "RecvQryFee",
	"errorCode": 0,
	"errorMsg": "Success",
	"result": [			
				{"table":"Fee",
				 "data":{
					,MemberID:"1983723"
					,InstrumentID:"BTCUSD"
					,MatchRole:"1"
					,OpenFeeRate:182582.18
					,OpenFeeAmount:182582.18
					,CloseFeeRate:182582.18
					,CloseFeeAmount:182582.18
					,CloseTodayFeeRate:182582.18
					,CloseTodayFeeAmount:182582.18
					,Remark:"my_little_tips"
					,InsertTime:1657543832
					,UpdateTime:1657543832
					}
				}
	]
}
```

+ **Response Parameters**

Fee:

parameter | shortName | type | description | remark
-------- | -------- | -------- | -------- | -------- 					
MemberID | MemberID | string | 成员代码 | 
InstrumentID | InstrumentID | string | 标的代码 | 
MatchRole | MatchRole | string | 成交角色 | 0:不区分;1:主动成交方;2:被动成交方;3:集合竞价;4:交割;5:场外交易;
OpenFeeRate | OpenFeeRate | double | 开仓手续费按比例 | 
OpenFeeAmount | OpenFeeAmount | double | 开仓手续费按手数 | 
CloseFeeRate | CloseFeeRate | double | 平仓手续费按比例 | 
CloseFeeAmount | CloseFeeAmount | double | 平仓手续费按手数 | 
CloseTodayFeeRate | CloseTodayFeeRate | double | 平今仓手续费按比例 | 
CloseTodayFeeAmount | CloseTodayFeeAmount | double | 平今仓手续费按手数 | 
Remark | Remark | string | 备注 | 
InsertTime | InsertTime | double | 插入时间 | 
UpdateTime | UpdateTime | double | 更新时间 | 


## **触发订单查询请求**

+ **Request URL**

`POST /action/v1.0/SendQryTriggerOrder`


+ **Request Post Body**

```
{
	,"MemberID":"1983723"
	,"AccountID":"1983723"
	,"ExchangeID":"CEX"
	,"InstrumentID":"BTCUSD"
	,"LocalID":"1983723"
}
```
+ **curl Example**

````
curl -d '{"MemberID":"1983723","AccountID":"1983723","ExchangeID":"CEX","InstrumentID":"BTCUSD","LocalID":"1983723"}'  -X POST  http://127.0.0.1:10000/action/v1.0/SendQryTriggerOrder
````
+ **ws Example**

````
{"RequestNo":2,"SendQryTriggerOrder":{"MemberID":"1983723","AccountID":"1983723","ExchangeID":"CEX","InstrumentID":"BTCUSD","LocalID":"1983723"}}
````
+ **Web rowser Example**

````
http://127.0.0.1:10000/action/v1.0/SendQryTriggerOrder?MemberID=1983723&AccountID=1983723&ExchangeID=CEX&InstrumentID=BTCUSD&LocalID=1983723
````
+ **Request Parameters**

parameter | shortName | required | type | description | remark
-------- | -------- | -------- | -------- | -------- | --------
MemberID | MemberID | no | string | 成员代码 | 
AccountID | AccountID | no | string | 资金账号 | 
ExchangeID | ExchangeID | no | string | 交易所代码 | 
InstrumentID | InstrumentID | no | string | 标的代码 | 
LocalID | LocalID | no | string | 报单本地标识 | 

+ **Response**

```
{
	"action": "RecvQryTriggerOrder",
	"errorCode": 0,
	"errorMsg": "Success",
	"result": [			
				{"table":"TriggerOrder",
				 "data":{
					,MemberID:"1983723"
					,TradeUnitID:"1983723"
					,AccountID:"1983723"
					,LocalID:"1983723"
					,ExchangeID:"CEX"
					,InstrumentID:"BTCUSD"
					,OrderPriceType:"1"
					,Direction:"1"
					,OffsetFlag:"1"
					,Price:182582.18
					,Volume:182582.18
					,VolumeDisplay:182582.18
					,VolumeMode:"1"
					,Cost:182582.18
					,OrderType:"1"
					,GTDTime:1657543832
					,MinVolume:182582.18
					,BusinessType:"1"
					,BusinessValue:""
					,CloseOrderID:"1983723"
					,IsCrossMargin:1
					,Remark:"my_little_tips"
					,OrderID:"12376422300333"
					,CopyMemberID:"1983723"
					,CopyOrderID:"1983723"
					,Leverage:125
					,CopyProfitRate:182582.18
					,APPID:"ios"
					,PositionID:"1983723"
					,TriggerPrice:182582.18
					,Reserve:182582.18
					,SLPrice:182582.18
					,SLTriggerPrice:182582.18
					,TPPrice:182582.18
					,TPTriggerPrice:182582.18
					,RiskBefore:0
					,TriggerOrderType:"1"
					,TriggerDetail:""
					,TriggerPriceType:"1"
					,TriggerValue:""
					,CloseSLPrice:182582.18
					,CloseSLTriggerPrice:182582.18
					,CloseTPPrice:182582.18
					,CloseTPTriggerPrice:182582.18
					,CloseOrderPriceType:"1"
					,ClosePrice:182582.18
					,CloseTriggerPrice:182582.18
					,RelatedOrderID:"1983723"
					,ActiveTime:1657543832
					,TriggerTime:1657543832
					,TimeSortNo:1657543832
					,TriggerStatus:"1"
					,PosiDirection:"1"
					,FrontNo:1
					,ErrorNo:1033201232
					,ErrorMsg:""
					,InsertTime:1657543832
					,UpdateTime:1657543832
					,BusinessNo:11176422300333
					,CreateTime:""
					}
				}
	]
}
```

+ **Response Parameters**

TriggerOrder:

parameter | shortName | type | description | remark
-------- | -------- | -------- | -------- | -------- 					
MemberID | M | string | 成员代码 | 
TradeUnitID | T | string | 交易单元代码 | 
AccountID | A | string | 资金账号 | 
LocalID | L | string | 报单本地标识 | 
ExchangeID | E | string | 交易所代码 | 
InstrumentID | I | string | 标的代码 | 
OrderPriceType | PT | string | 报单价格类型 | 0:限价(手动指定Price值);1:任意价(Price按最高最低限价取值);2:最优价/对手价(Price按对方1档价取值);3:五档价(Price按对方5档价取值);4:十档价(类同上);7:最优Marker价(Price按本方1档价或其+1Tick取值);8:Price按委托额Cost和订单簿情况确定值;9:保底点差价:成交价格Price不会劣于系统设置差价成交;
Direction | D | string | 买卖方向 | 0:买;1:卖;
OffsetFlag | O | string | 开平标志 | 0:开仓;1:平仓;2:强平;3:平今;4:平昨;5:全平;6:指定订单平仓;7:指定成交平仓;8:最大平仓;
Price | P | double | 报单价格 | 
Volume | V | double | 数量 | 
VolumeDisplay | D | double | 显示数量 | 
VolumeMode | VM | string | 数量取值方式 | 0:百分比;1:绝对值;
Cost | C | double | 委托额 | 
OrderType | OT | string | 订单类型 | 0:普通订单;1:(FillAndKill/IOC)立即完成并且剩余撤销;2:(FillOrKill)立即完成全部或者最小订单要求否则全部撤销;3:需要进入订单簿被动成交,否则全部撤销(只做Maker/PostOnly);4:不需要撮合直接成交;
GTDTime | GT | double | GTD时间 | 
MinVolume | MV | double | 最小成交量 | 
BusinessType | BT | string | 业务类别 | 0:普通;1:申赎;2:行权;3:中立仓申报;4:递延交割申报;5:互换定单;6:质押;7:合并分拆;8:转股;9:回售回购;a:投票;b:融资融券;c:期权执行;d:传递被平仓订单BusinessValue;P:自定义11;Q:自定义10;R:自定义9;S:连续交易;T:自定义7;U:画线委托;V:限价市价;W:条件;X:止盈止损;Y:追踪出场;Z:指标类型;
BusinessValue | BV | string | 业务值 | 
CloseOrderID | CI | string | 平仓指定开仓的订单号 | 
IsCrossMargin | IsCrossMargin | int | 是否全仓 | 
Remark | R | string | 备注 | 
OrderID | OrderID | string | 报单系统唯一代码 | 
CopyMemberID | CopyMemberID | string | 带单员代码 | 
CopyOrderID | CopyOrderID | string | 带单员报单号 | 
Leverage | Leverage | double | 委托单杠杆倍数 | 
CopyProfitRate | CopyProfitRate | double | 带单分配比例 | 
APPID | APPID | string | 应用编号 | 
PositionID | B | string | 持仓代码 | 
TriggerPrice | TriggerPrice | double | 触发价 | 
Reserve | Reserve | double | 保留资金 | 
SLPrice | SLPrice | double | 止损价 | 
SLTriggerPrice | SLTriggerPrice | double | 止损触发价 | 
TPPrice | TPPrice | double | 止盈价 | 
TPTriggerPrice | TPTriggerPrice | double | 止盈触发价 | 
RiskBefore | RiskBefore | int | 是否提前检查风控 | 
TriggerOrderType | TriggerOrderType | string | 触发的订单类型 | 0:未定义;1:持仓止盈止损;2:订单止盈止损;3:条件单;4:追踪(跟踪)订单;5:其他算法交易;6:指标类型;7:指标类型止盈止损;8:跟单类型;9:划线下单类型;a:其他类型1;b:其他类型2;c:其他类型3;d:其他类型4;e:其他类型5;
TriggerDetail | TriggerDetail | string | 触发类型明细 | 
TriggerPriceType | TriggerPriceType | string | 触发价类型 | 0:最新价;1:标记价;2:标的指数价;
TriggerValue | TriggerValue | string | 触发单具体设置信息 | 
CloseSLPrice | CloseSLPrice | double | 平仓止损价 | 
CloseSLTriggerPrice | CloseSLTriggerPrice | double | 平仓止损触发价 | 
CloseTPPrice | CloseTPPrice | double | 平仓止盈价 | 
CloseTPTriggerPrice | CloseTPTriggerPrice | double | 平仓止盈触发价 | 
CloseOrderPriceType | CloseOrderPriceType | string | 报单价格类型 | 0:限价(手动指定Price值);1:任意价(Price按最高最低限价取值);2:最优价/对手价(Price按对方1档价取值);3:五档价(Price按对方5档价取值);4:十档价(类同上);7:最优Marker价(Price按本方1档价或其+1Tick取值);8:Price按委托额Cost和订单簿情况确定值;9:保底点差价:成交价格Price不会劣于系统设置差价成交;
ClosePrice | ClosePrice | double | 平仓价 | 
CloseTriggerPrice | CloseTriggerPrice | double | 平仓触发价 | 
RelatedOrderID | RelatedOrderID | string | 关联报单号 | 
ActiveTime | ActiveTime | double | 激活时间 | 
TriggerTime | TriggerTime | double | 触发时间 | 
TimeSortNo | TimeSortNo | double | 按时间排队的序号 | 
TriggerStatus | TriggerStatus | string | 触发报单状态 | 0:未设置状态;1:活跃(运行)状态;2:已经触发;3:触发失败;4:撤单;
PosiDirection | PosiDirection | string | 持仓多空方向 | 0:多头;1:空头;2:净;
FrontNo | FrontNo | int | 前置编号 | 
ErrorNo | ErrorNo | int | 错误代码 | 
ErrorMsg | ErrorMsg | string | 错误信息 | 
InsertTime | InsertTime | double | 插入时间 | 
UpdateTime | UpdateTime | double | 更新时间 | 
BusinessNo | BusinessNo | double | 业务序列号 | 
CreateTime | CreateTime | string | 创建时间 | 


## **杠杆倍数查询请求**

+ **Request URL**

`POST /action/v1.0/SendQryLeverage`


+ **Request Post Body**

```
{
	,"MemberID":"1983723"
	,"AccountID":"1983723"
	,"TradeUnitID":"1983723"
	,"ExchangeID":"CEX"
	,"InstrumentID":"BTCUSD"
	,"PosiDirection":"1"
}
```
+ **curl Example**

````
curl -d '{"MemberID":"1983723","AccountID":"1983723","TradeUnitID":"1983723","ExchangeID":"CEX","InstrumentID":"BTCUSD","PosiDirection":"1"}'  -X POST  http://127.0.0.1:10000/action/v1.0/SendQryLeverage
````
+ **ws Example**

````
{"RequestNo":2,"SendQryLeverage":{"MemberID":"1983723","AccountID":"1983723","TradeUnitID":"1983723","ExchangeID":"CEX","InstrumentID":"BTCUSD","PosiDirection":"1"}}
````
+ **Web rowser Example**

````
http://127.0.0.1:10000/action/v1.0/SendQryLeverage?MemberID=1983723&AccountID=1983723&TradeUnitID=1983723&ExchangeID=CEX&InstrumentID=BTCUSD&PosiDirection=1
````
+ **Request Parameters**

parameter | shortName | required | type | description | remark
-------- | -------- | -------- | -------- | -------- | --------
MemberID | MemberID | yes | string | 成员代码 | 
AccountID | AccountID | yes | string | 资金账号 | 
TradeUnitID | TradeUnitID | yes | string | 交易单元代码 | 
ExchangeID | ExchangeID | yes | string | 交易所代码 | 
InstrumentID | InstrumentID | yes | string | 标的代码 | 
PosiDirection | PosiDirection | yes | string | 持仓多空方向 | 0:多头;1:空头;2:净;

+ **Response**

```
{
	"action": "RecvQryLeverage",
	"errorCode": 0,
	"errorMsg": "Success",
	"result": [			
				{"table":"Leverage",
				 "data":{
					,MemberID:"1983723"
					,AccountID:"1983723"
					,TradeUnitID:"1983723"
					,ExchangeID:"CEX"
					,InstrumentID:"BTCUSD"
					,PosiDirection:"1"
					,DefaultMaxLeverage:125
					,LongLeverage:125
					,LongMaxLeverage:125
					,LongMaxVolume:182582.18
					,LongLastVolume:182582.18
					,LongLevel:0
					,LongMaintRatio:182582.18
					,ShortLeverage:125
					,ShortMaxLeverage:125
					,ShortMaxVolume:182582.18
					,ShortLastVolume:182582.18
					,ShortLevel:0
					,ShortMaintRatio:182582.18
					,IsCrossMargin:1
					}
				}
	]
}
```

+ **Response Parameters**

Leverage:

parameter | shortName | type | description | remark
-------- | -------- | -------- | -------- | -------- 					
MemberID | MemberID | string | 成员代码 | 
AccountID | AccountID | string | 资金账号 | 
TradeUnitID | TradeUnitID | string | 交易单元代码 | 
ExchangeID | ExchangeID | string | 交易所代码 | 
InstrumentID | InstrumentID | string | 标的代码 | 
PosiDirection | PosiDirection | string | 持仓多空方向 | 0:多头;1:空头;2:净;
DefaultMaxLeverage | DefaultMaxLeverage | double | 默认最大杠杆倍数 | 
LongLeverage | LongLeverage | double | 多头杠杆倍数 | 
LongMaxLeverage | LongMaxLeverage | double | 当前多头梯度最大杠杆倍数 | 
LongMaxVolume | LongMaxVolume | double | 当前多头梯度最大张数 | 
LongLastVolume | LongLastVolume | double | 当前多头梯度剩余可开张数 | 
LongLevel | LongLevel | int | 多头梯度档位 | 
LongMaintRatio | LongMaintRatio | double | 当前多头维持保证金率 | 
ShortLeverage | ShortLeverage | double | 空头杠杆倍数 | 
ShortMaxLeverage | ShortMaxLeverage | double | 当前空头梯度最大杠杆倍数 | 
ShortMaxVolume | ShortMaxVolume | double | 当前空头梯度最大张数 | 
ShortLastVolume | ShortLastVolume | double | 当前空头梯度剩余可开张数 | 
ShortLevel | ShortLevel | int | 空头梯度档位 | 
ShortMaintRatio | ShortMaintRatio | double | 当前空头维持保证金率 | 
IsCrossMargin | IsCrossMargin | int | 是否全仓 | 


## **行情查询请求**

+ **Request URL**

`POST /action/v1.0/SendQryMarketData`


+ **Request Post Body**

```
{
	,"ExchangeID":"CEX"
	,"ProductGroup":"Swap"
	,"InstrumentID":"BTCUSD"
	,"From":0
	,"Limit":0
}
```
+ **curl Example**

````
curl -d '{"ExchangeID":"CEX","ProductGroup":"Swap","InstrumentID":"BTCUSD","From":"0,"Limit":"0}'  -X POST  http://127.0.0.1:10000/action/v1.0/SendQryMarketData
````
+ **ws Example**

````
{"RequestNo":2,"SendQryMarketData":{"ExchangeID":"CEX","ProductGroup":"Swap","InstrumentID":"BTCUSD","From":"0,"Limit":"0}}
````
+ **Web rowser Example**

````
http://127.0.0.1:10000/action/v1.0/SendQryMarketData?ExchangeID=CEX&ProductGroup=Swap&InstrumentID=BTCUSD&From=0&Limit=0
````
+ **Request Parameters**

parameter | shortName | required | type | description | remark
-------- | -------- | -------- | -------- | -------- | --------
ExchangeID | ExchangeID | no | string | 交易所代码 | 
ProductGroup | ProductGroup | no | string | 产品组 | 
InstrumentID | InstrumentID | no | string | 标的代码 | 
From | From | no | int | 开始序号 | 
Limit | Limit | no | int | 限制数量 | 

+ **Response**

```
{
	"action": "RecvQryMarketData",
	"errorCode": 0,
	"errorMsg": "Success",
	"result": [			
				{"table":"MarketData",
				 "data":{
					,ExchangeID:"CEX"
					,InstrumentID:"BTCUSD"
					,ProductGroup:"Swap"
					,UpdateTime:1657543832
					,PreClosePrice:182582.18
					,PreSettlementPrice:182582.18
					,PreOpenInterest:182582.18
					,PrePositionFeeRate:182582.18
					,ClosePrice:182582.18
					,PositionFeeTime:1657543832
					,SettlementTime:1657543832
					,UpperLimitPrice:182582.18
					,LowerLimitPrice:182582.18
					,UnderlyingPrice:182582.18
					,MarkedPrice:182582.18
					,PositionFeeRate:182582.18
					,HighestPrice:182582.18
					,LowestPrice:182582.18
					,LastPrice:182582.18
					,Volume:182582.18
					,Turnover:182582.18
					,OpenInterest:182582.18
					,SettlementPrice:182582.18
					,OpenPrice:182582.18
					,IsPubMarketData:1
					,InstrumentStatus:"1"
					,Remark:"my_little_tips"
					,BusinessNo:11176422300333
					,MarkedPriceTotal:182582.18
					,MarkedPriceWeight:182582.18
					,L25UpperPrice:182582.18
					,L25LowerPrice:182582.18
					,L25UpperPrice1:182582.18
					,L25LowerPrice1:182582.18
					,L25UpperPrice2:182582.18
					,L25LowerPrice2:182582.18
					,BidPrice1:182582.18
					,BidVolume1:182582.18
					,AskPrice1:182582.18
					,AskVolume1:182582.18
					,TheoryPrice:182582.18
					,HighestPrice24:182582.18
					,LowestPrice24:182582.18
					,Volume24:182582.18
					,Turnover24:182582.18
					,OpenPrice24:182582.18
					,Frequency:0
					,InsertTime:1657543832
					}
				}
	]
}
```

+ **Response Parameters**

MarketData:

parameter | shortName | type | description | remark
-------- | -------- | -------- | -------- | -------- 					
ExchangeID | X | string | 交易所代码 | 
InstrumentID | I | string | 标的代码 | 
ProductGroup | G | string | 产品组 | 
UpdateTime | U | double | 更新时间 | 
PreClosePrice |  | double | 上次收盘 | 
PreSettlementPrice |  | double | 上次结算价 | 
PreOpenInterest |  | double | 上次持仓量 | 
PrePositionFeeRate |  | double | 上次持仓费按比例 | 
ClosePrice | ClosePrice | double | 收盘价 | 
PositionFeeTime | PF | double | 收取持仓费用时间 | 
SettlementTime | SettlementTime | double | 结算时间 | 
UpperLimitPrice | C | double | 最高限价 | 
LowerLimitPrice | F | double | 最低限价 | 
UnderlyingPrice | D | double | 基础标的价格 | 
MarkedPrice | M | double | 标记价格 | 
PositionFeeRate | R | double | 预计持仓费按比例 | 
HighestPrice | H | double | 当日最高价 | 
LowestPrice | L | double | 当日最低价 | 
LastPrice | N | double | 最新价 | 
Volume | V | double | 当日数量 | 
Turnover | T | double | 当日成交金额 | 
OpenInterest | P | double | 持仓量 | 
SettlementPrice | S | double | 预估结算价 | 
OpenPrice | O | double | 当日开盘价 | 
IsPubMarketData |  | int | 行情发布状态 | 
InstrumentStatus | B | string | 标的交易状态 | 0:开盘前;1:非交易;2:连续交易;3:集合竞价报单;4:集合竞价价格平衡;5:集合竞价撮合;6:收盘;7:不活跃;
Remark | RE | string | 备注 | 
BusinessNo |  | double | 业务序列号 | 
MarkedPriceTotal |  | double | 标记价格汇总 | 
MarkedPriceWeight |  | double | 标记价格权重 | 
L25UpperPrice |  | double | 25档行情最高价 | 
L25LowerPrice |  | double | 25档行情最低价 | 
L25UpperPrice1 |  | double | 25档行情最高价1 | 
L25LowerPrice1 |  | double | 25档行情最低价1 | 
L25UpperPrice2 |  | double | 25档行情最高价2 | 
L25LowerPrice2 |  | double | 25档行情最低价2 | 
BidPrice1 | BP1 | double | 申买价一 | 
BidVolume1 | BV1 | double | 申买量一 | 
AskPrice1 | AP1 | double | 申卖价一 | 
AskVolume1 | AV1 | double | 申卖量一 | 
TheoryPrice | TP | double | 理论价 | 
HighestPrice24 | H2 | double | 24小时最高价 | 
LowestPrice24 | L2 | double | 24小时最低价 | 
Volume24 | V2 | double | 24小时数量 | 
Turnover24 | T2 | double | 24小时成交金额 | 
OpenPrice24 | O2 | double | 24小时开盘价 | 
Frequency |  | int | 每秒更新的次数 | 
InsertTime |  | double | 插入时间 | 


## **成交查询请求**

+ **Request URL**

`POST /action/v1.0/SendQryTrade`


+ **Request Post Body**

```
{
	,"OrderID":"12376422300333"
	,"MemberID":"1983723"
	,"AccountID":"1983723"
	,"ExchangeID":"CEX"
	,"InstrumentID":"BTCUSD"
	,"Limit":0
	,"Before":""
	,"After":""
	,"StartTime":1657543832
	,"EndTime":1657543832
}
```
+ **curl Example**

````
curl -d '{"OrderID":"12376422300333","MemberID":"1983723","AccountID":"1983723","ExchangeID":"CEX","InstrumentID":"BTCUSD","Limit":"0,"Before":"","After":"","StartTime":"1657543832,"EndTime":"1657543832}'  -X POST  http://127.0.0.1:10000/action/v1.0/SendQryTrade
````
+ **ws Example**

````
{"RequestNo":2,"SendQryTrade":{"OrderID":"12376422300333","MemberID":"1983723","AccountID":"1983723","ExchangeID":"CEX","InstrumentID":"BTCUSD","Limit":"0,"Before":"","After":"","StartTime":"1657543832,"EndTime":"1657543832}}
````
+ **Web rowser Example**

````
http://127.0.0.1:10000/action/v1.0/SendQryTrade?OrderID=12376422300333&MemberID=1983723&AccountID=1983723&ExchangeID=CEX&InstrumentID=BTCUSD&Limit=0&Before=&After=&StartTime=1657543832&EndTime=1657543832
````
+ **Request Parameters**

parameter | shortName | required | type | description | remark
-------- | -------- | -------- | -------- | -------- | --------
OrderID | OrderID | no | string | 报单系统唯一代码 | 
MemberID | MemberID | no | string | 成员代码 | 
AccountID | AccountID | no | string | 资金账号 | 
ExchangeID | ExchangeID | no | string | 交易所代码 | 
InstrumentID | InstrumentID | no | string | 标的代码 | 
Limit | Limit | no | int | 限制数量 | 
Before | Before | no | string | 请求此 ID 之前（更旧的数据）的分页内容 | 
After | After | no | string | 请求此 ID 之后（更新的数据）的分页内容 | 
StartTime | StartTime | no | double | 起始时间 | 
EndTime | EndTime | no | double | 结束时间 | 

+ **Response**

```
{
	"action": "RecvQryTrade",
	"errorCode": 0,
	"errorMsg": "Success",
	"result": [			
				{"table":"Trade",
				 "data":{
					,TradeID:"16576422300333"
					,Direction:"1"
					,OrderID:"12376422300333"
					,MemberID:"1983723"
					,PositionID:"1983723"
					,AccountID:"1983723"
					,ExchangeID:"CEX"
					,InstrumentID:"BTCUSD"
					,OffsetFlag:"1"
					,Price:182582.18
					,Volume:182582.18
					,DeriveSource:"1"
					,MatchRole:"1"
					,PriceCurrency:"BTC"
					,ClearCurrency:"BTC"
					,Fee:0.22364
					,FeeCurrency:"BTC"
					,CloseProfit:10
					,Turnover:182582.18
					,UseMargin:182582.18
					,Leverage:125
					,OrderPrice:182582.18
					,TriggerPrice:182582.18
					,IsSelfTrade:1
					,Remark:"my_little_tips"
					,BusinessNo:11176422300333
					,OpenPrice:182582.18
					,APPID:"ios"
					,InsertTime:1657543832
					,CreateTime:""
					,CopyOrderID:"1983723"
					,CopyMemberID:"1983723"
					,CopyProfit:182582.18
					,Position:182582.18
					,ReserveProfit:182582.18
					,ReserveFee:182582.18
					,TradeRemark:""
					,BusinessType:"1"
					,BusinessValue:""
					}
				}
	]
}
```

+ **Response Parameters**

Trade:

parameter | shortName | type | description | remark
-------- | -------- | -------- | -------- | -------- 					
TradeID | TradeID | string | 成交代码 | 
Direction | Direction | string | 买卖方向 | 0:买;1:卖;
OrderID | OrderID | string | 报单系统唯一代码 | 
MemberID | MemberID | string | 成员代码 | 
PositionID | PositionID | string | 持仓代码 | 
AccountID | AccountID | string | 资金账号 | 
ExchangeID | ExchangeID | string | 交易所代码 | 
InstrumentID | InstrumentID | string | 标的代码 | 
OffsetFlag | OffsetFlag | string | 开平标志 | 0:开仓;1:平仓;2:强平;3:平今;4:平昨;5:全平;6:指定订单平仓;7:指定成交平仓;8:最大平仓;
Price | Price | double | 成交价格 | 
Volume | Volume | double | 成交数量 | 
DeriveSource | DeriveSource | string | 衍生类型 | 0:正常;1:报价衍生;2:期权执行;3:组合衍生;4:场外成交强平衍生;5:场外成交大宗交易衍生;6:场外成交期转现衍生;7:场外成交只刷单衍生;8:触发订单衍生;9:止损触发订单衍生;a:止盈触发订单衍生;b:资金操作衍生;c:仓位合并衍生;d:CFD订单衍生;e:场外成交ADL衍生;f:回购手续费不能是币;
MatchRole | MatchRole | string | 成交角色 | 0:不区分;1:主动成交方;2:被动成交方;3:集合竞价;4:交割;5:场外交易;
PriceCurrency | PriceCurrency | string | 计价币种 | 
ClearCurrency | ClearCurrency | string | 清算币种 | 
Fee | Fee | double | 手续费 | 
FeeCurrency | FeeCurrency | string | 手续费币种 | 
CloseProfit | CloseProfit | double | 平仓盈亏 | 
Turnover | Turnover | double | 成交金额 | 
UseMargin | UseMargin | double | 占用或者减少的保证金 | 
Leverage | Leverage | double | 杠杆倍数 | 
OrderPrice | OrderPrice | double | 委托价格 | 
TriggerPrice | TriggerPrice | double | 触发价 | 
IsSelfTrade | IsSelfTrade | int | 是否自成交 | 
Remark | Remark | string | 备注 | 
BusinessNo | BusinessNo | double | 业务序列号 | 
OpenPrice | OpenPrice | double | 开仓均价 | 
APPID | APPID | string | 应用编号 | 
InsertTime | InsertTime | double | 插入时间 | 
CreateTime | CreateTime | string | 创建时间 | 
CopyOrderID | CopyOrderID | string | 带单员报单号 | 
CopyMemberID | CopyMemberID | string | 带单员代码 | 
CopyProfit | CopyProfit | double | 带单盈利分配 | 
Position | Position | double | 成交时的持仓量 | 
ReserveProfit | ReserveProfit | double | 保留资金盈亏 | 
ReserveFee | ReserveFee | double | 保留资金手续费 | 
TradeRemark | TradeRemark | string | Trade备注 | 
BusinessType | BT | string | 业务类别 | 0:普通;1:申赎;2:行权;3:中立仓申报;4:递延交割申报;5:互换定单;6:质押;7:合并分拆;8:转股;9:回售回购;a:投票;b:融资融券;c:期权执行;d:传递被平仓订单BusinessValue;P:自定义11;Q:自定义10;R:自定义9;S:连续交易;T:自定义7;U:画线委托;V:限价市价;W:条件;X:止盈止损;Y:追踪出场;Z:指标类型;
BusinessValue | BV | string | 业务值 | 


## **历史委托查询请求**

+ **Request URL**

`POST /action/v1.0/SendQryFinishOrder`


+ **Request Post Body**

```
{
	,"MemberID":"1983723"
	,"OrderID":"12376422300333"
	,"AccountID":"1983723"
	,"ExchangeID":"CEX"
	,"InstrumentID":"BTCUSD"
	,"OrderStatus":"1"
	,"Limit":0
	,"Before":""
	,"After":""
	,"StartTime":1657543832
	,"EndTime":1657543832
}
```
+ **curl Example**

````
curl -d '{"MemberID":"1983723","OrderID":"12376422300333","AccountID":"1983723","ExchangeID":"CEX","InstrumentID":"BTCUSD","OrderStatus":"1","Limit":"0,"Before":"","After":"","StartTime":"1657543832,"EndTime":"1657543832}'  -X POST  http://127.0.0.1:10000/action/v1.0/SendQryFinishOrder
````
+ **ws Example**

````
{"RequestNo":2,"SendQryFinishOrder":{"MemberID":"1983723","OrderID":"12376422300333","AccountID":"1983723","ExchangeID":"CEX","InstrumentID":"BTCUSD","OrderStatus":"1","Limit":"0,"Before":"","After":"","StartTime":"1657543832,"EndTime":"1657543832}}
````
+ **Web rowser Example**

````
http://127.0.0.1:10000/action/v1.0/SendQryFinishOrder?MemberID=1983723&OrderID=12376422300333&AccountID=1983723&ExchangeID=CEX&InstrumentID=BTCUSD&OrderStatus=1&Limit=0&Before=&After=&StartTime=1657543832&EndTime=1657543832
````
+ **Request Parameters**

parameter | shortName | required | type | description | remark
-------- | -------- | -------- | -------- | -------- | --------
MemberID | MemberID | no | string | 成员代码 | 
OrderID | OrderID | no | string | 报单系统唯一代码 | 
AccountID | AccountID | no | string | 资金账号 | 
ExchangeID | ExchangeID | no | string | 交易所代码 | 
InstrumentID | InstrumentID | no | string | 标的代码 | 
OrderStatus | OrderStatus | no | string | 报单状态 | 0:未设置状态;1:全部成交;2:部分成交未撤单;3:部分成交已撤单;4:未成交未撤单;6:无成交已撤单;
Limit | Limit | no | int | 限制数量 | 
Before | Before | no | string | 请求此 ID 之前（更旧的数据）的分页内容 | 
After | After | no | string | 请求此 ID 之后（更新的数据）的分页内容 | 
StartTime | StartTime | no | double | 起始时间 | 
EndTime | EndTime | no | double | 结束时间 | 

+ **Response**

```
{
	"action": "RecvQryFinishOrder",
	"errorCode": 0,
	"errorMsg": "Success",
	"result": [			
				{"table":"FinishOrder",
				 "data":{
					,MemberID:"1983723"
					,TradeUnitID:"1983723"
					,AccountID:"1983723"
					,LocalID:"1983723"
					,ExchangeID:"CEX"
					,InstrumentID:"BTCUSD"
					,OrderPriceType:"1"
					,Direction:"1"
					,OffsetFlag:"1"
					,Price:182582.18
					,Volume:182582.18
					,VolumeDisplay:182582.18
					,VolumeMode:"1"
					,Cost:182582.18
					,OrderType:"1"
					,GTDTime:1657543832
					,MinVolume:182582.18
					,BusinessType:"1"
					,BusinessValue:""
					,CloseOrderID:"1983723"
					,IsCrossMargin:1
					,Remark:"my_little_tips"
					,OrderID:"12376422300333"
					,CopyMemberID:"1983723"
					,CopyOrderID:"1983723"
					,Leverage:125
					,CopyProfitRate:182582.18
					,APPID:"ios"
					,PositionID:"1983723"
					,TriggerPrice:182582.18
					,Reserve:182582.18
					,OrderStatus:"1"
					,DeriveSource:"1"
					,DeriveDetail:""
					,VolumeTraded:182582.18
					,VolumeRemain:182582.18
					,VolumeCancled:182582.18
					,InsertTime:1657543832
					,UpdateTime:1657543832
					,Priority:0
					,TimeSortNo:1657543832
					,FrontNo:1
					,PriceCurrency:"BTC"
					,FeeCurrency:"BTC"
					,ClearCurrency:"BTC"
					,FrozenMoney:182582.18
					,FrozenFee:182582.18
					,FrozenMargin:182582.18
					,Fee:0.22364
					,CloseProfit:10
					,Turnover:182582.18
					,RelatedOrderID:"1983723"
					,BusinessResult:""
					,BusinessNo:11176422300333
					,Tradable:0
					,SettlementGroup:"999"
					,PosiDirection:"1"
					,TradePrice:182582.18
					,OpenPrice:182582.18
					,TriggerOrderID:"1983723"
					,SLTriggerPrice:182582.18
					,TPTriggerPrice:182582.18
					,CopyProfit:182582.18
					,Position:182582.18
					,UserID:"1983723"
					,LastPriceByInsert:182582.18
					,BidPrice1ByInsert:182582.18
					,AskPrice1ByInsert:182582.18
					,Available:182582.18
					,CreateTime:""
					}
				}
	]
}
```

+ **Response Parameters**

FinishOrder:

parameter | shortName | type | description | remark
-------- | -------- | -------- | -------- | -------- 					
MemberID | M | string | 成员代码 | 
TradeUnitID | T | string | 交易单元代码 | 
AccountID | A | string | 资金账号 | 
LocalID | L | string | 报单本地标识 | 
ExchangeID | E | string | 交易所代码 | 
InstrumentID | I | string | 标的代码 | 
OrderPriceType | PT | string | 报单价格类型 | 0:限价(手动指定Price值);1:任意价(Price按最高最低限价取值);2:最优价/对手价(Price按对方1档价取值);3:五档价(Price按对方5档价取值);4:十档价(类同上);7:最优Marker价(Price按本方1档价或其+1Tick取值);8:Price按委托额Cost和订单簿情况确定值;9:保底点差价:成交价格Price不会劣于系统设置差价成交;
Direction | D | string | 买卖方向 | 0:买;1:卖;
OffsetFlag | O | string | 开平标志 | 0:开仓;1:平仓;2:强平;3:平今;4:平昨;5:全平;6:指定订单平仓;7:指定成交平仓;8:最大平仓;
Price | P | double | 报单价格 | 
Volume | V | double | 数量 | 
VolumeDisplay | D | double | 显示数量 | 
VolumeMode | VM | string | 数量取值方式 | 0:百分比;1:绝对值;
Cost | C | double | 委托额 | 
OrderType | OT | string | 订单类型 | 0:普通订单;1:(FillAndKill/IOC)立即完成并且剩余撤销;2:(FillOrKill)立即完成全部或者最小订单要求否则全部撤销;3:需要进入订单簿被动成交,否则全部撤销(只做Maker/PostOnly);4:不需要撮合直接成交;
GTDTime | GT | double | GTD时间 | 
MinVolume | MV | double | 最小成交量 | 
BusinessType | BT | string | 业务类别 | 0:普通;1:申赎;2:行权;3:中立仓申报;4:递延交割申报;5:互换定单;6:质押;7:合并分拆;8:转股;9:回售回购;a:投票;b:融资融券;c:期权执行;d:传递被平仓订单BusinessValue;P:自定义11;Q:自定义10;R:自定义9;S:连续交易;T:自定义7;U:画线委托;V:限价市价;W:条件;X:止盈止损;Y:追踪出场;Z:指标类型;
BusinessValue | BV | string | 业务值 | 
CloseOrderID | CI | string | 平仓指定开仓的订单号 | 
IsCrossMargin | IsCrossMargin | int | 是否全仓 | 
Remark | R | string | 备注 | 
OrderID | OrderID | string | 报单系统唯一代码 | 
CopyMemberID | CopyMemberID | string | 带单员代码 | 
CopyOrderID | CopyOrderID | string | 带单员报单号 | 
Leverage | Leverage | double | 委托单杠杆倍数 | 
CopyProfitRate | CopyProfitRate | double | 带单分配比例 | 
APPID | APPID | string | 应用编号 | 
PositionID | B | string | 持仓代码 | 
TriggerPrice | TriggerPrice | double | 触发价 | 
Reserve | Reserve | double | 保留资金 | 
OrderStatus | OrderStatus | string | 报单状态 | 0:未设置状态;1:全部成交;2:部分成交未撤单;3:部分成交已撤单;4:未成交未撤单;6:无成交已撤单;
DeriveSource | DeriveSource | string | 衍生来源 | 0:正常;1:报价衍生;2:期权执行;3:组合衍生;4:场外成交强平衍生;5:场外成交大宗交易衍生;6:场外成交期转现衍生;7:场外成交只刷单衍生;8:触发订单衍生;9:止损触发订单衍生;a:止盈触发订单衍生;b:资金操作衍生;c:仓位合并衍生;d:CFD订单衍生;e:场外成交ADL衍生;f:回购手续费不能是币;
DeriveDetail | DeriveDetail | string | 衍生明细 | 
VolumeTraded | VolumeTraded | double | 成交数量 | 
VolumeRemain | VolumeRemain | double | 剩余数量 | 
VolumeCancled | VolumeCancled | double | 已经撤单数量 | 
InsertTime | InsertTime | double | 插入时间 | 
UpdateTime | UpdateTime | double | 更新时间 | 
Priority |  | int | 优先权 | 
TimeSortNo |  | double | 按时间排队的序号 | 
FrontNo | FrontNo | int | 前置编号 | 
PriceCurrency | PriceCurrency | string | 计价币种 | 
FeeCurrency | FeeCurrency | string | 手续费币种 | 
ClearCurrency | ClearCurrency | string | 清算币种 | 
FrozenMoney | FrozenMoney | double | 冻结资金 | 
FrozenFee | FrozenFee | double | 冻结手续费 | 
FrozenMargin | FrozenMargin | double | 冻结保证金 | 
Fee | Fee | double | 手续费 | 
CloseProfit | CloseProfit | double | 平仓盈亏 | 
Turnover | Turnover | double | 成交金额 | 
RelatedOrderID | RelatedOrderID | string | 关联报单号 | 
BusinessResult | BusinessResult | string | 业务执行结果 | 
BusinessNo | BusinessNo | double | 业务序列号 | 
Tradable |  | int | 是否可以参加交易 | 
SettlementGroup |  | string | 结算组编号 | 
PosiDirection | PosiDirection | string | 持仓多空方向 | 0:多头;1:空头;2:净;
TradePrice | TradePrice | double | 成交均价 | 
OpenPrice | OpenPrice | double | 平仓成交时的开仓均价 | 
TriggerOrderID | TriggerOrderID | string | 触发报单号 | 
SLTriggerPrice | SLTriggerPrice | double | 开仓报单成交之后的止损触发价 | 
TPTriggerPrice | TPTriggerPrice | double | 开仓报单成交之后的止盈触发价 | 
CopyProfit | CopyProfit | double | 带单盈利分配 | 
Position | Position | double | 成交时的持仓量 | 
UserID | U | string | 交易用户代码 | 
LastPriceByInsert |  | double | 插入时的最新价 | 
BidPrice1ByInsert |  | double | 插入时的买一价 | 
AskPrice1ByInsert |  | double | 插入时的卖一价 | 
Available | Available | double | 可用资金 | 
CreateTime |  | string | 创建时间 | 


## **历史条件单查询请求**

+ **Request URL**

`POST /action/v1.0/SendQryFinishTriggerOrder`


+ **Request Post Body**

```
{
	,"MemberID":"1983723"
	,"OrderID":"12376422300333"
	,"AccountID":"1983723"
	,"ExchangeID":"CEX"
	,"InstrumentID":"BTCUSD"
	,"TriggerStatus":"1"
	,"TriggerOrderType":"1"
	,"Limit":0
	,"Before":""
	,"After":""
	,"StartTime":1657543832
	,"EndTime":1657543832
}
```
+ **curl Example**

````
curl -d '{"MemberID":"1983723","OrderID":"12376422300333","AccountID":"1983723","ExchangeID":"CEX","InstrumentID":"BTCUSD","TriggerStatus":"1","TriggerOrderType":"1","Limit":"0,"Before":"","After":"","StartTime":"1657543832,"EndTime":"1657543832}'  -X POST  http://127.0.0.1:10000/action/v1.0/SendQryFinishTriggerOrder
````
+ **ws Example**

````
{"RequestNo":2,"SendQryFinishTriggerOrder":{"MemberID":"1983723","OrderID":"12376422300333","AccountID":"1983723","ExchangeID":"CEX","InstrumentID":"BTCUSD","TriggerStatus":"1","TriggerOrderType":"1","Limit":"0,"Before":"","After":"","StartTime":"1657543832,"EndTime":"1657543832}}
````
+ **Web rowser Example**

````
http://127.0.0.1:10000/action/v1.0/SendQryFinishTriggerOrder?MemberID=1983723&OrderID=12376422300333&AccountID=1983723&ExchangeID=CEX&InstrumentID=BTCUSD&TriggerStatus=1&TriggerOrderType=1&Limit=0&Before=&After=&StartTime=1657543832&EndTime=1657543832
````
+ **Request Parameters**

parameter | shortName | required | type | description | remark
-------- | -------- | -------- | -------- | -------- | --------
MemberID | MemberID | no | string | 成员代码 | 
OrderID | OrderID | no | string | 报单系统唯一代码 | 
AccountID | AccountID | no | string | 资金账号 | 
ExchangeID | ExchangeID | no | string | 交易所代码 | 
InstrumentID | InstrumentID | no | string | 标的代码 | 
TriggerStatus | TriggerStatus | no | string | 触发报单状态 | 0:未设置状态;1:活跃(运行)状态;2:已经触发;3:触发失败;4:撤单;
TriggerOrderType | TriggerOrderType | no | string | 触发的订单类型 | 0:未定义;1:持仓止盈止损;2:订单止盈止损;3:条件单;4:追踪(跟踪)订单;5:其他算法交易;6:指标类型;7:指标类型止盈止损;8:跟单类型;9:划线下单类型;a:其他类型1;b:其他类型2;c:其他类型3;d:其他类型4;e:其他类型5;
Limit | Limit | no | int | 限制数量 | 
Before | Before | no | string | 请求此 ID 之前（更旧的数据）的分页内容 | 
After | After | no | string | 请求此 ID 之后（更新的数据）的分页内容 | 
StartTime | StartTime | no | double | 起始时间 | 
EndTime | EndTime | no | double | 结束时间 | 

+ **Response**

```
{
	"action": "RecvQryFinishTriggerOrder",
	"errorCode": 0,
	"errorMsg": "Success",
	"result": [			
				{"table":"FinishTriggerOrder",
				 "data":{
					,MemberID:"1983723"
					,TradeUnitID:"1983723"
					,AccountID:"1983723"
					,LocalID:"1983723"
					,ExchangeID:"CEX"
					,InstrumentID:"BTCUSD"
					,OrderPriceType:"1"
					,Direction:"1"
					,OffsetFlag:"1"
					,Price:182582.18
					,Volume:182582.18
					,VolumeDisplay:182582.18
					,VolumeMode:"1"
					,Cost:182582.18
					,OrderType:"1"
					,GTDTime:1657543832
					,MinVolume:182582.18
					,BusinessType:"1"
					,BusinessValue:""
					,CloseOrderID:"1983723"
					,IsCrossMargin:1
					,Remark:"my_little_tips"
					,OrderID:"12376422300333"
					,CopyMemberID:"1983723"
					,CopyOrderID:"1983723"
					,Leverage:125
					,CopyProfitRate:182582.18
					,APPID:"ios"
					,PositionID:"1983723"
					,TriggerPrice:182582.18
					,Reserve:182582.18
					,SLPrice:182582.18
					,SLTriggerPrice:182582.18
					,TPPrice:182582.18
					,TPTriggerPrice:182582.18
					,RiskBefore:0
					,TriggerOrderType:"1"
					,TriggerDetail:""
					,TriggerPriceType:"1"
					,TriggerValue:""
					,CloseSLPrice:182582.18
					,CloseSLTriggerPrice:182582.18
					,CloseTPPrice:182582.18
					,CloseTPTriggerPrice:182582.18
					,CloseOrderPriceType:"1"
					,ClosePrice:182582.18
					,CloseTriggerPrice:182582.18
					,RelatedOrderID:"1983723"
					,ActiveTime:1657543832
					,TriggerTime:1657543832
					,TimeSortNo:1657543832
					,TriggerStatus:"1"
					,PosiDirection:"1"
					,FrontNo:1
					,ErrorNo:1033201232
					,ErrorMsg:""
					,InsertTime:1657543832
					,UpdateTime:1657543832
					,BusinessNo:11176422300333
					,CreateTime:""
					}
				}
	]
}
```

+ **Response Parameters**

FinishTriggerOrder:

parameter | shortName | type | description | remark
-------- | -------- | -------- | -------- | -------- 					
MemberID | M | string | 成员代码 | 
TradeUnitID | T | string | 交易单元代码 | 
AccountID | A | string | 资金账号 | 
LocalID | L | string | 报单本地标识 | 
ExchangeID | E | string | 交易所代码 | 
InstrumentID | I | string | 标的代码 | 
OrderPriceType | PT | string | 报单价格类型 | 0:限价(手动指定Price值);1:任意价(Price按最高最低限价取值);2:最优价/对手价(Price按对方1档价取值);3:五档价(Price按对方5档价取值);4:十档价(类同上);7:最优Marker价(Price按本方1档价或其+1Tick取值);8:Price按委托额Cost和订单簿情况确定值;9:保底点差价:成交价格Price不会劣于系统设置差价成交;
Direction | D | string | 买卖方向 | 0:买;1:卖;
OffsetFlag | O | string | 开平标志 | 0:开仓;1:平仓;2:强平;3:平今;4:平昨;5:全平;6:指定订单平仓;7:指定成交平仓;8:最大平仓;
Price | P | double | 报单价格 | 
Volume | V | double | 数量 | 
VolumeDisplay | D | double | 显示数量 | 
VolumeMode | VM | string | 数量取值方式 | 0:百分比;1:绝对值;
Cost | C | double | 委托额 | 
OrderType | OT | string | 订单类型 | 0:普通订单;1:(FillAndKill/IOC)立即完成并且剩余撤销;2:(FillOrKill)立即完成全部或者最小订单要求否则全部撤销;3:需要进入订单簿被动成交,否则全部撤销(只做Maker/PostOnly);4:不需要撮合直接成交;
GTDTime | GT | double | GTD时间 | 
MinVolume | MV | double | 最小成交量 | 
BusinessType | BT | string | 业务类别 | 0:普通;1:申赎;2:行权;3:中立仓申报;4:递延交割申报;5:互换定单;6:质押;7:合并分拆;8:转股;9:回售回购;a:投票;b:融资融券;c:期权执行;d:传递被平仓订单BusinessValue;P:自定义11;Q:自定义10;R:自定义9;S:连续交易;T:自定义7;U:画线委托;V:限价市价;W:条件;X:止盈止损;Y:追踪出场;Z:指标类型;
BusinessValue | BV | string | 业务值 | 
CloseOrderID | CI | string | 平仓指定开仓的订单号 | 
IsCrossMargin | IsCrossMargin | int | 是否全仓 | 
Remark | R | string | 备注 | 
OrderID | OrderID | string | 报单系统唯一代码 | 
CopyMemberID | CopyMemberID | string | 带单员代码 | 
CopyOrderID | CopyOrderID | string | 带单员报单号 | 
Leverage | Leverage | double | 委托单杠杆倍数 | 
CopyProfitRate | CopyProfitRate | double | 带单分配比例 | 
APPID | APPID | string | 应用编号 | 
PositionID | B | string | 持仓代码 | 
TriggerPrice | TriggerPrice | double | 触发价 | 
Reserve | Reserve | double | 保留资金 | 
SLPrice | SLPrice | double | 止损价 | 
SLTriggerPrice | SLTriggerPrice | double | 止损触发价 | 
TPPrice | TPPrice | double | 止盈价 | 
TPTriggerPrice | TPTriggerPrice | double | 止盈触发价 | 
RiskBefore | RiskBefore | int | 是否提前检查风控 | 
TriggerOrderType | TriggerOrderType | string | 触发的订单类型 | 0:未定义;1:持仓止盈止损;2:订单止盈止损;3:条件单;4:追踪(跟踪)订单;5:其他算法交易;6:指标类型;7:指标类型止盈止损;8:跟单类型;9:划线下单类型;a:其他类型1;b:其他类型2;c:其他类型3;d:其他类型4;e:其他类型5;
TriggerDetail | TriggerDetail | string | 触发类型明细 | 
TriggerPriceType | TriggerPriceType | string | 触发价类型 | 0:最新价;1:标记价;2:标的指数价;
TriggerValue | TriggerValue | string | 触发单具体设置信息 | 
CloseSLPrice | CloseSLPrice | double | 平仓止损价 | 
CloseSLTriggerPrice | CloseSLTriggerPrice | double | 平仓止损触发价 | 
CloseTPPrice | CloseTPPrice | double | 平仓止盈价 | 
CloseTPTriggerPrice | CloseTPTriggerPrice | double | 平仓止盈触发价 | 
CloseOrderPriceType | CloseOrderPriceType | string | 报单价格类型 | 0:限价(手动指定Price值);1:任意价(Price按最高最低限价取值);2:最优价/对手价(Price按对方1档价取值);3:五档价(Price按对方5档价取值);4:十档价(类同上);7:最优Marker价(Price按本方1档价或其+1Tick取值);8:Price按委托额Cost和订单簿情况确定值;9:保底点差价:成交价格Price不会劣于系统设置差价成交;
ClosePrice | ClosePrice | double | 平仓价 | 
CloseTriggerPrice | CloseTriggerPrice | double | 平仓触发价 | 
RelatedOrderID | RelatedOrderID | string | 关联报单号 | 
ActiveTime | ActiveTime | double | 激活时间 | 
TriggerTime | TriggerTime | double | 触发时间 | 
TimeSortNo | TimeSortNo | double | 按时间排队的序号 | 
TriggerStatus | TriggerStatus | string | 触发报单状态 | 0:未设置状态;1:活跃(运行)状态;2:已经触发;3:触发失败;4:撤单;
PosiDirection | PosiDirection | string | 持仓多空方向 | 0:多头;1:空头;2:净;
FrontNo | FrontNo | int | 前置编号 | 
ErrorNo | ErrorNo | int | 错误代码 | 
ErrorMsg | ErrorMsg | string | 错误信息 | 
InsertTime | InsertTime | double | 插入时间 | 
UpdateTime | UpdateTime | double | 更新时间 | 
BusinessNo | BusinessNo | double | 业务序列号 | 
CreateTime | CreateTime | string | 创建时间 | 


## **指定杠杆倍数查询请求**

+ **Request URL**

`POST /action/v1.0/SendQryWithLeverage`


+ **Request Post Body**

```
{
	,"MemberID":"1983723"
	,"AccountID":"1983723"
	,"TradeUnitID":"1983723"
	,"ExchangeID":"CEX"
	,"PosiDirection":"1"
	,"InstrumentID":"BTCUSD"
	,"LongLeverage":125
	,"ShortLeverage":125
	,"NetLeverage":125
}
```
+ **curl Example**

````
curl -d '{"MemberID":"1983723","AccountID":"1983723","TradeUnitID":"1983723","ExchangeID":"CEX","PosiDirection":"1","InstrumentID":"BTCUSD","LongLeverage":"125,"ShortLeverage":"125,"NetLeverage":"125}'  -X POST  http://127.0.0.1:10000/action/v1.0/SendQryWithLeverage
````
+ **ws Example**

````
{"RequestNo":2,"SendQryWithLeverage":{"MemberID":"1983723","AccountID":"1983723","TradeUnitID":"1983723","ExchangeID":"CEX","PosiDirection":"1","InstrumentID":"BTCUSD","LongLeverage":"125,"ShortLeverage":"125,"NetLeverage":"125}}
````
+ **Web rowser Example**

````
http://127.0.0.1:10000/action/v1.0/SendQryWithLeverage?MemberID=1983723&AccountID=1983723&TradeUnitID=1983723&ExchangeID=CEX&PosiDirection=1&InstrumentID=BTCUSD&LongLeverage=125&ShortLeverage=125&NetLeverage=125
````
+ **Request Parameters**

parameter | shortName | required | type | description | remark
-------- | -------- | -------- | -------- | -------- | --------
MemberID | MemberID | yes | string | 成员代码 | 
AccountID | AccountID | yes | string | 资金账号 | 
TradeUnitID | TradeUnitID | yes | string | 交易单元代码 | 
ExchangeID | ExchangeID | yes | string | 交易所代码 | 
PosiDirection | PosiDirection | no | string | 持仓多空方向 | 0:多头;1:空头;2:净;
InstrumentID | InstrumentID | yes | string | 标的代码 | 
LongLeverage | LongLeverage | no | double | 多头杠杆倍数 | 
ShortLeverage | ShortLeverage | no | double | 空头杠杆倍数 | 
NetLeverage | NetLeverage | no | double | 净仓杠杆倍数 | 

+ **Response**

```
{
	"action": "RecvQryWithLeverage",
	"errorCode": 0,
	"errorMsg": "Success",
	"result": [			
				{"table":"Leverage",
				 "data":{
					,MemberID:"1983723"
					,AccountID:"1983723"
					,TradeUnitID:"1983723"
					,ExchangeID:"CEX"
					,InstrumentID:"BTCUSD"
					,PosiDirection:"1"
					,DefaultMaxLeverage:125
					,LongLeverage:125
					,LongMaxLeverage:125
					,LongMaxVolume:182582.18
					,LongLastVolume:182582.18
					,LongLevel:0
					,LongMaintRatio:182582.18
					,ShortLeverage:125
					,ShortMaxLeverage:125
					,ShortMaxVolume:182582.18
					,ShortLastVolume:182582.18
					,ShortLevel:0
					,ShortMaintRatio:182582.18
					,IsCrossMargin:1
					}
				}
	]
}
```

+ **Response Parameters**

Leverage:

parameter | shortName | type | description | remark
-------- | -------- | -------- | -------- | -------- 					
MemberID | MemberID | string | 成员代码 | 
AccountID | AccountID | string | 资金账号 | 
TradeUnitID | TradeUnitID | string | 交易单元代码 | 
ExchangeID | ExchangeID | string | 交易所代码 | 
InstrumentID | InstrumentID | string | 标的代码 | 
PosiDirection | PosiDirection | string | 持仓多空方向 | 0:多头;1:空头;2:净;
DefaultMaxLeverage | DefaultMaxLeverage | double | 默认最大杠杆倍数 | 
LongLeverage | LongLeverage | double | 多头杠杆倍数 | 
LongMaxLeverage | LongMaxLeverage | double | 当前多头梯度最大杠杆倍数 | 
LongMaxVolume | LongMaxVolume | double | 当前多头梯度最大张数 | 
LongLastVolume | LongLastVolume | double | 当前多头梯度剩余可开张数 | 
LongLevel | LongLevel | int | 多头梯度档位 | 
LongMaintRatio | LongMaintRatio | double | 当前多头维持保证金率 | 
ShortLeverage | ShortLeverage | double | 空头杠杆倍数 | 
ShortMaxLeverage | ShortMaxLeverage | double | 当前空头梯度最大杠杆倍数 | 
ShortMaxVolume | ShortMaxVolume | double | 当前空头梯度最大张数 | 
ShortLastVolume | ShortLastVolume | double | 当前空头梯度剩余可开张数 | 
ShortLevel | ShortLevel | int | 空头梯度档位 | 
ShortMaintRatio | ShortMaintRatio | double | 当前空头维持保证金率 | 
IsCrossMargin | IsCrossMargin | int | 是否全仓 | 


## **K线查询请求**

+ **Request URL**

`POST /action/v1.0/SendQryKLine`


+ **Request Post Body**

```
{
	,"ExchangeID":"CEX"
	,"InstrumentID":"BTCUSD"
	,"Bar":""
	,"EndTime":1657543832
	,"Limit":0
}
```
+ **curl Example**

````
curl -d '{"ExchangeID":"CEX","InstrumentID":"BTCUSD","Bar":"","EndTime":"1657543832,"Limit":"0}'  -X POST  http://127.0.0.1:10000/action/v1.0/SendQryKLine
````
+ **ws Example**

````
{"RequestNo":2,"SendQryKLine":{"ExchangeID":"CEX","InstrumentID":"BTCUSD","Bar":"","EndTime":"1657543832,"Limit":"0}}
````
+ **Web rowser Example**

````
http://127.0.0.1:10000/action/v1.0/SendQryKLine?ExchangeID=CEX&InstrumentID=BTCUSD&Bar=&EndTime=1657543832&Limit=0
````
+ **Request Parameters**

parameter | shortName | required | type | description | remark
-------- | -------- | -------- | -------- | -------- | --------
ExchangeID | ExchangeID | no | string | 交易所代码 | 
InstrumentID | InstrumentID | no | string | 标的代码 | 
Bar | Bar | no | string | K线周期代码(n[m/h/d/o]) | 
EndTime | EndTime | no | double | 结束时间 | 
Limit | Limit | no | int | 数量 | 

+ **Response**

```
{
	"action": "RecvQryKLine",
	"errorCode": 0,
	"errorMsg": "Success",
	"result": [			
				{"table":"KLineBase",
				 "data":{
					,InsertTime:1657543832
					,OpenPrice:182582.18
					,HighestPrice:182582.18
					,LowestPrice:182582.18
					,ClosePrice:182582.18
					,Volume:182582.18
					,Turnover:182582.18
					}
				}
	]
}
```

+ **Response Parameters**

KLineBase:

parameter | shortName | type | description | remark
-------- | -------- | -------- | -------- | -------- 					
InsertTime | I | double | 插入时间 | 
OpenPrice | O | double | 开盘价 | 
HighestPrice | H | double | 最高价 | 
LowestPrice | L | double | 最低价 | 
ClosePrice | C | double | 收盘价 | 
Volume | V | double | 数量 | 
Turnover | T | double | 成交金额 | 


## **线查询请求**

+ **Request URL**

`POST /action/v1.0/SendQryLine`


+ **Request Post Body**

```
{
	,"ExchangeID":"CEX"
	,"InstrumentID":"BTCUSD"
	,"Bar":""
	,"Limit":0
}
```
+ **curl Example**

````
curl -d '{"ExchangeID":"CEX","InstrumentID":"BTCUSD","Bar":"","Limit":"0}'  -X POST  http://127.0.0.1:10000/action/v1.0/SendQryLine
````
+ **ws Example**

````
{"RequestNo":2,"SendQryLine":{"ExchangeID":"CEX","InstrumentID":"BTCUSD","Bar":"","Limit":"0}}
````
+ **Web rowser Example**

````
http://127.0.0.1:10000/action/v1.0/SendQryLine?ExchangeID=CEX&InstrumentID=BTCUSD&Bar=&Limit=0
````
+ **Request Parameters**

parameter | shortName | required | type | description | remark
-------- | -------- | -------- | -------- | -------- | --------
ExchangeID | ExchangeID | no | string | 交易所代码 | 
InstrumentID | InstrumentID | no | string | 标的代码 | 
Bar | Bar | no | string | K线周期代码(n[m/h/d/o]) | 
Limit | Limit | no | int | 数量 | 

+ **Response**

```
{
	"action": "RecvQryLine",
	"errorCode": 0,
	"errorMsg": "Success",
	"result": [			
				{"table":"Line",
				 "data":{
					,InsertTime:1657543832
					,ClosePrice:182582.18
					}
				}
	]
}
```

+ **Response Parameters**

Line:

parameter | shortName | type | description | remark
-------- | -------- | -------- | -------- | -------- 					
InsertTime | I | double | 插入时间 | 
ClosePrice | C | double | 收盘价 | 


## **资金流水查询请求**

+ **Request URL**

`POST /action/v1.0/SendQryAccountDetail`


+ **Request Post Body**

```
{
	,"ProductGroup":"Swap"
	,"AccountID":"1983723"
	,"MemberID":"1983723"
	,"Currency":"BTC"
	,"SettlementGroup":"999"
	,"InstrumentID":"BTCUSD"
	,"Source":""
	,"Limit":0
	,"Before":""
	,"After":""
	,"StartTime":1657543832
	,"EndTime":1657543832
}
```
+ **curl Example**

````
curl -d '{"ProductGroup":"Swap","AccountID":"1983723","MemberID":"1983723","Currency":"BTC","SettlementGroup":"999","InstrumentID":"BTCUSD","Source":"","Limit":"0,"Before":"","After":"","StartTime":"1657543832,"EndTime":"1657543832}'  -X POST  http://127.0.0.1:10000/action/v1.0/SendQryAccountDetail
````
+ **ws Example**

````
{"RequestNo":2,"SendQryAccountDetail":{"ProductGroup":"Swap","AccountID":"1983723","MemberID":"1983723","Currency":"BTC","SettlementGroup":"999","InstrumentID":"BTCUSD","Source":"","Limit":"0,"Before":"","After":"","StartTime":"1657543832,"EndTime":"1657543832}}
````
+ **Web rowser Example**

````
http://127.0.0.1:10000/action/v1.0/SendQryAccountDetail?ProductGroup=Swap&AccountID=1983723&MemberID=1983723&Currency=BTC&SettlementGroup=999&InstrumentID=BTCUSD&Source=&Limit=0&Before=&After=&StartTime=1657543832&EndTime=1657543832
````
+ **Request Parameters**

parameter | shortName | required | type | description | remark
-------- | -------- | -------- | -------- | -------- | --------
ProductGroup | ProductGroup | no | string | 产品组 | 
AccountID | AccountID | no | string | 资金账号 | 
MemberID | MemberID | no | string | 成员代码 | 
Currency | Currency | no | string | 币种 | 
SettlementGroup | SettlementGroup | no | string | 结算组 | 
InstrumentID | InstrumentID | no | string | 标的代码 | 
Source | Source | no | string | 财务流水类型 | 
Limit | Limit | no | int | 限制数量 | 
Before | Before | no | string | 请求此 ID 之前（更旧的数据）的分页内容 | 
After | After | no | string | 请求此 ID 之后（更新的数据）的分页内容 | 
StartTime | StartTime | no | double | 起始时间 | 
EndTime | EndTime | no | double | 结束时间 | 

+ **Response**

```
{
	"action": "RecvQryAccountDetail",
	"errorCode": 0,
	"errorMsg": "Success",
	"result": [			
				{"table":"AccountDetail",
				 "data":{
					,AccountDetailID:"1983723"
					,MemberID:"1983723"
					,ExchangeID:"CEX"
					,InstrumentID:"BTCUSD"
					,SettlementGroup:"999"
					,AccountID:"1983723"
					,Currency:"BTC"
					,Amount:182582.18
					,PreBalance:182582.18
					,Balance:182582.18
					,ReserveAmount:182582.18
					,ReserveBalance:182582.18
					Source:""
					,Remark:"my_little_tips"
					,LocalID:"1983723"
					,SettleSegment:""
					,BusinessNo:11176422300333
					,RelatedID:"1983723"
					,InsertTime:1657543832
					,CreateTime:""
					}
				}
	]
}
```

+ **Response Parameters**

AccountDetail:

parameter | shortName | type | description | remark
-------- | -------- | -------- | -------- | -------- 					
AccountDetailID | AccountDetailID | string | 资金明细号 | 
MemberID | MemberID | string | 成员代码 | 
ExchangeID | ExchangeID | string | 交易所代码 | 
InstrumentID | InstrumentID | string | 标的代码 | 
SettlementGroup | SettlementGroup | string | 结算组编号 | 
AccountID | AccountID | string | 资金账号 | 
Currency | Currency | string | 币种 | 
Amount | Amount | double | 发生额 | 
PreBalance | PreBalance | double | 上次静态权益 | 
Balance | Balance | double | 静态权益 | 
ReserveAmount | ReserveAmount | double | 体验金发生额 | 
ReserveBalance | ReserveBalance | double | 体验金静态权益 | 
Source | Source | string | 财务流水类型 | 
Remark | Remark | string | 备注 | 
LocalID | LocalID | string | 本地标识 | 
SettleSegment | SettleSegment | string | 结算段 | 
BusinessNo | BusinessNo | double | 业务序列号 | 
RelatedID | RelatedID | string | 内外对账ID | 
InsertTime | InsertTime | double | 插入时间 | 
CreateTime | CreateTime | string | 创建时间 | 


## **结算流水查询请求**

+ **Request URL**

`POST /action/v1.0/SendQrySettleDetail`


+ **Request Post Body**

```
{
	,"ProductGroup":"Swap"
	,"InstrumentID":"BTCUSD"
	,"SettleAction":"1"
}
```
+ **curl Example**

````
curl -d '{"ProductGroup":"Swap","InstrumentID":"BTCUSD","SettleAction":"1"}'  -X POST  http://127.0.0.1:10000/action/v1.0/SendQrySettleDetail
````
+ **ws Example**

````
{"RequestNo":2,"SendQrySettleDetail":{"ProductGroup":"Swap","InstrumentID":"BTCUSD","SettleAction":"1"}}
````
+ **Web rowser Example**

````
http://127.0.0.1:10000/action/v1.0/SendQrySettleDetail?ProductGroup=Swap&InstrumentID=BTCUSD&SettleAction=1
````
+ **Request Parameters**

parameter | shortName | required | type | description | remark
-------- | -------- | -------- | -------- | -------- | --------
ProductGroup | ProductGroup | no | string | 产品组 | 
InstrumentID | InstrumentID | no | string | 标的代码 | 
SettleAction | SettleAction | no | string | 结算操作 | 0:结算操作开始;1:定期无负债结算;2:到期交割(行权);3:资金费率交换;4:隔夜费划转;5:分摊;6:资金初始化;7:结算操作结束;8:数据检查;9:数据修复;a:平账;b:清理;c:清空没有持仓的产品;d:借贷利息;

+ **Response**

```
{
	"action": "RecvQrySettleDetail",
	"errorCode": 0,
	"errorMsg": "Success",
	"result": [			
				{"table":"SettleDetail",
				 "data":{
					,SettleDetailID:"1983723"
					,APPID:"ios"
					,LocalID:"1983723"
					,ExchangeID:"CEX"
					,InstrumentID:"BTCUSD"
					,SettlementGroup:"999"
					,SettleAction:"1"
					,Value:182582.18
					,Value1:182582.18
					,Value2:182582.18
					,Value3:182582.18
					,Remark:"my_little_tips"
					,FundingRateGroup:"999"
					,SettleSegment:""
					,InsertTime:""
					,BusinessNo:11176422300333
					}
				}
	]
}
```

+ **Response Parameters**

SettleDetail:

parameter | shortName | type | description | remark
-------- | -------- | -------- | -------- | -------- 					
SettleDetailID | SettleDetailID | string | 操作明细号 | 
APPID | APPID | string | 应用编号 | 
LocalID | LocalID | string | 成交对本地标识 | 
ExchangeID | ExchangeID | string | 交易所代码 | 
InstrumentID | InstrumentID | string | 标的代码 | 
SettlementGroup | SettlementGroup | string | 结算组编号 | 
SettleAction | SettleAction | string | 结算操作 | 0:结算操作开始;1:定期无负债结算;2:到期交割(行权);3:资金费率交换;4:隔夜费划转;5:分摊;6:资金初始化;7:结算操作结束;8:数据检查;9:数据修复;a:平账;b:清理;c:清空没有持仓的产品;d:借贷利息;
Value | Value | double | 操作使用值 | 
Value1 | Value1 | double | 发生值1 | 
Value2 | Value2 | double | 发生值2 | 
Value3 | Value3 | double | 发生值3 | 
Remark | Remark | string | 备注 | 
FundingRateGroup | FundingRateGroup | string | 资金费用组 | 
SettleSegment | SettleSegment | string | 结算段 | 
InsertTime | InsertTime | string | 创建时间 | 
BusinessNo | BusinessNo | double | 业务序列号 | 


## **检查TOKEN请求**

+ **Request URL**

`POST /action/v1.0/SendCheckToken`


+ **Request Post Body**

```
{
	,"Token":""
	,"UserID":"1983723"
	,"MaxLocalID":"1983723"
	,"MemberID":"1983723"
}
```
+ **curl Example**

````
curl -d '{"Token":"","UserID":"1983723","MaxLocalID":"1983723","MemberID":"1983723"}'  -X POST  http://127.0.0.1:10000/action/v1.0/SendCheckToken
````
+ **ws Example**

````
{"RequestNo":2,"SendCheckToken":{"Token":"","UserID":"1983723","MaxLocalID":"1983723","MemberID":"1983723"}}
````
+ **Web rowser Example**

````
http://127.0.0.1:10000/action/v1.0/SendCheckToken?Token=&UserID=1983723&MaxLocalID=1983723&MemberID=1983723
````
+ **Request Parameters**

parameter | shortName | required | type | description | remark
-------- | -------- | -------- | -------- | -------- | --------
Token | Token | no | string | 令牌 | 
UserID | UserID | no | string | 交易用户代码 | 
MaxLocalID | MaxLocalID | no | string | 最大本地标识 | 
MemberID | MemberID | no | string | 成员代码 | 

+ **Response**

```
{
	"action": "RecvCheckToken",
	"errorCode": 0,
	"errorMsg": "Success",
	"result": [			
				{"table":"CheckToken",
				 "data":{
					,Token:""
					,UserID:"1983723"
					,MaxLocalID:"1983723"
					,MemberID:"1983723"
					}
				}
	]
}
```

+ **Response Parameters**

CheckToken:

parameter | shortName | type | description | remark
-------- | -------- | -------- | -------- | -------- 					
Token | Token | string | 令牌 | 
UserID | UserID | string | 交易用户代码 | 
MaxLocalID | MaxLocalID | string | 最大本地标识 | 
MemberID | MemberID | string | 成员代码 | 


## **批量操作请求(HTTP接口专用)**

+ **Request URL**

`POST /action/v1.0/SendBatch`


+ **Request Post Body**

```
{
	,"IsLast":1
}
```
+ **curl Example**

````
curl -d '{"IsLast":"1}'  -X POST  http://127.0.0.1:10000/action/v1.0/SendBatch
````
+ **ws Example**

````
{"RequestNo":2,"SendBatch":{"IsLast":"1}}
````
+ **Web rowser Example**

````
http://127.0.0.1:10000/action/v1.0/SendBatch?IsLast=1
````
+ **Request Parameters**

parameter | shortName | required | type | description | remark
-------- | -------- | -------- | -------- | -------- | --------
IsLast | IsLast | no | int | 是否最后 | 

+ **Response**

```
{
}
```

+ **Response Parameters**


# **WebSocket数据推送接口**

## **订阅接口汇总**
name | TopicID | Comments | usage
-------- | -------- | -------- | --------
Topic_Private_Order | 3 | 私有交易主题 | FilterValue=MemberID
Topic_Private_SnapShot | 12 | 私有快照主题 | FilterValue=MemberID)
Topic_Market_LastTrade | 2 | 最近成交 | FilterValue=ExchangeID_InstrumentID
Topic_Market_LastKLine | 11 | 最近K线(主要是1分钟)推送主题 | FilterValue=ExchangeID_InstrumentID_Period
Topic_Market_KLine | 4 | 历史K线主题 | FilterValue=ExchangeID_InstrumentID_Period
Topic_Market_SnapShot | 5 | 5档行情快照主题[可通过Numbers降频] | FilterValue=ExchangeID_InstrumentID[_Numbers]
Topic_Market_Broad | 6 | 广播主题,合约变化,时钟心跳等 | 
Topic_Market_OverView | 7 | 行情概要快照主题 | FilterValue=ExchangeID_InstrumentID
Topic_Market_OverView_Delay | 17 | 行情概要延时快照主题 | FilterValue=ExchangeID_InstrumentID)
Topic_Market_Depth_All | 8 | 全档行情增量主题 | FilterValue=ExchangeID_InstrumentID
Topic_Market_Depth_Delay | 18 | 全档行情增量延时发送主题 | FilterValue=ExchangeID_InstrumentID
Topic_Market_Depth_25 | 25 | 25档行情增量主题 | FilterValue=ExchangeID_InstrumentID
Topic_Market_Depth_50 | 50 | 50档行情增量主题 | FilterValue=ExchangeID_InstrumentID
Topic_Market_Depth_100 | 100 | 100档行情增量主题 | FilterValue=ExchangeID_InstrumentID
Topic_Private_Risk | 9 | 风控数据(持仓、资金、行情、时间),分控用户才能订阅 | 
Topic_Private_Trade | 13 | 私有成交主题 | FilterValue=MemberID
Topic_Private_TriggerOrder | 14 | 策略触发程序使用的私有主题 | FilterValue=MemberID
Topic_Private_FinishOrder | 16 | 管理后台私有完成订单主题 | FilterValue=MemberID
Topic_Private_Copy_Finish | 10 | 跟单完成订单主题 | FilterValue=MemberID
Topic_Private_UserSession | 99 | 用户Session主题 | 


## **时间同步通知**

+ **Request URL**  `/ws`

+ **Request**

```
{
	"SendTopicAction": {
		"Action": "1",
		"FilterValue": "",
		"LocalNo": 5,
		"ResumeNo": -1,
		"TopicID": "6",
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
	"action": "PushTimeSync",
	"localNo": 232,
	"seq": 123,
	"changeType": "3",
	"result": [			
				{"table":"CurrentTime",
				 "data":{
					,SystemID:"1983723"
					,ZoneDate:""
					,ZoneTime:""
					,ZoneDays:""
					,CurrTime:1657543832
					,InsertTime:1657543832
					,UpdateTime:1657543832
					,TimeZone:0
					,LastTradeID:"1983723"
					,LastOrderID:"1983723"
					,LastAccountDetailID:"1983723"
					,LastTimeSortNo:1657543832
					,LastBusinessNo:182582.18
					}
				}
	]
}
```

+ **Response Parameters**

CurrentTime:

parameter | shortName | type | description | remark
-------- | -------- | -------- | -------- | -------- 					
SystemID |  | string | 系统名 | 
ZoneDate | D | string | 本地日期 | 
ZoneTime | T | string | 本地时间 | 
ZoneDays | Z | string | 本地天数 | 
CurrTime | C | double | 当前时间（秒） | 
InsertTime | InsertTime | double | 插入时间 | 
UpdateTime | U | double | 更新时间（毫秒） | 
TimeZone | TimeZone | int | 本地时区 | 
LastTradeID | LastTradeID | string | 最新最新成交序列号 | 
LastOrderID | LastOrderID | string | 最新订单序列号 | 
LastAccountDetailID | LastAccountDetailID | string | 最新资金明细序列号 | 
LastTimeSortNo | LastTimeSortNo | double | 最新排序序列号 | 
LastBusinessNo | LastBusinessNo | double | 最新事务序列号 | 
## **消息通知**

+ **Request URL**  `/ws`

+ **Request**

```
{
	"SendTopicAction": {
		"Action": "1",
		"FilterValue": "",
		"LocalNo": 5,
		"ResumeNo": -1,
		"TopicID": "3",
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
	"action": "PushCommand",
	"localNo": 232,
	"seq": 123,
	"changeType": "3",
	"result": [			
				{"table":"Command",
				 "data":{
					,UserID:"1983723"
					,LocalID:"1983723"
					,Command:""
					,Content:""
					}
				}
	]
}
```

+ **Response Parameters**

Command:

parameter | shortName | type | description | remark
-------- | -------- | -------- | -------- | -------- 					
UserID | UserID | string | 交易用户代码 | 
LocalID | LocalID | string | 命令本地号 | 
Command | Command | string | 命令 | 
Content | Content | string | 字段内容 | 
## **成交通知**

+ **Request URL**  `/ws`

+ **Request**

```
{
	"SendTopicAction": {
		"Action": "1",
		"FilterValue": "",
		"LocalNo": 5,
		"ResumeNo": -1,
		"TopicID": "3",
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
	"action": "PushTrade",
	"localNo": 232,
	"seq": 123,
	"changeType": "3",
	"result": [			
				{"table":"Trade",
				 "data":{
					,TradeID:"16576422300333"
					,Direction:"1"
					,OrderID:"12376422300333"
					,MemberID:"1983723"
					,PositionID:"1983723"
					,AccountID:"1983723"
					,ExchangeID:"CEX"
					,InstrumentID:"BTCUSD"
					,OffsetFlag:"1"
					,Price:182582.18
					,Volume:182582.18
					,DeriveSource:"1"
					,MatchRole:"1"
					,PriceCurrency:"BTC"
					,ClearCurrency:"BTC"
					,Fee:0.22364
					,FeeCurrency:"BTC"
					,CloseProfit:10
					,Turnover:182582.18
					,UseMargin:182582.18
					,Leverage:125
					,OrderPrice:182582.18
					,TriggerPrice:182582.18
					,IsSelfTrade:1
					,Remark:"my_little_tips"
					,BusinessNo:11176422300333
					,OpenPrice:182582.18
					,APPID:"ios"
					,InsertTime:1657543832
					,CreateTime:""
					,CopyOrderID:"1983723"
					,CopyMemberID:"1983723"
					,CopyProfit:182582.18
					,Position:182582.18
					,ReserveProfit:182582.18
					,ReserveFee:182582.18
					,TradeRemark:""
					,BusinessType:"1"
					,BusinessValue:""
					}
				}
	]
}
```

+ **Response Parameters**

Trade:

parameter | shortName | type | description | remark
-------- | -------- | -------- | -------- | -------- 					
TradeID | TradeID | string | 成交代码 | 
Direction | Direction | string | 买卖方向 | 0:买;1:卖;
OrderID | OrderID | string | 报单系统唯一代码 | 
MemberID | MemberID | string | 成员代码 | 
PositionID | PositionID | string | 持仓代码 | 
AccountID | AccountID | string | 资金账号 | 
ExchangeID | ExchangeID | string | 交易所代码 | 
InstrumentID | InstrumentID | string | 标的代码 | 
OffsetFlag | OffsetFlag | string | 开平标志 | 0:开仓;1:平仓;2:强平;3:平今;4:平昨;5:全平;6:指定订单平仓;7:指定成交平仓;8:最大平仓;
Price | Price | double | 成交价格 | 
Volume | Volume | double | 成交数量 | 
DeriveSource | DeriveSource | string | 衍生类型 | 0:正常;1:报价衍生;2:期权执行;3:组合衍生;4:场外成交强平衍生;5:场外成交大宗交易衍生;6:场外成交期转现衍生;7:场外成交只刷单衍生;8:触发订单衍生;9:止损触发订单衍生;a:止盈触发订单衍生;b:资金操作衍生;c:仓位合并衍生;d:CFD订单衍生;e:场外成交ADL衍生;f:回购手续费不能是币;
MatchRole | MatchRole | string | 成交角色 | 0:不区分;1:主动成交方;2:被动成交方;3:集合竞价;4:交割;5:场外交易;
PriceCurrency | PriceCurrency | string | 计价币种 | 
ClearCurrency | ClearCurrency | string | 清算币种 | 
Fee | Fee | double | 手续费 | 
FeeCurrency | FeeCurrency | string | 手续费币种 | 
CloseProfit | CloseProfit | double | 平仓盈亏 | 
Turnover | Turnover | double | 成交金额 | 
UseMargin | UseMargin | double | 占用或者减少的保证金 | 
Leverage | Leverage | double | 杠杆倍数 | 
OrderPrice | OrderPrice | double | 委托价格 | 
TriggerPrice | TriggerPrice | double | 触发价 | 
IsSelfTrade | IsSelfTrade | int | 是否自成交 | 
Remark | Remark | string | 备注 | 
BusinessNo | BusinessNo | double | 业务序列号 | 
OpenPrice | OpenPrice | double | 开仓均价 | 
APPID | APPID | string | 应用编号 | 
InsertTime | InsertTime | double | 插入时间 | 
CreateTime | CreateTime | string | 创建时间 | 
CopyOrderID | CopyOrderID | string | 带单员报单号 | 
CopyMemberID | CopyMemberID | string | 带单员代码 | 
CopyProfit | CopyProfit | double | 带单盈利分配 | 
Position | Position | double | 成交时的持仓量 | 
ReserveProfit | ReserveProfit | double | 保留资金盈亏 | 
ReserveFee | ReserveFee | double | 保留资金手续费 | 
TradeRemark | TradeRemark | string | Trade备注 | 
BusinessType | BT | string | 业务类别 | 0:普通;1:申赎;2:行权;3:中立仓申报;4:递延交割申报;5:互换定单;6:质押;7:合并分拆;8:转股;9:回售回购;a:投票;b:融资融券;c:期权执行;d:传递被平仓订单BusinessValue;P:自定义11;Q:自定义10;R:自定义9;S:连续交易;T:自定义7;U:画线委托;V:限价市价;W:条件;X:止盈止损;Y:追踪出场;Z:指标类型;
BusinessValue | BV | string | 业务值 | 
## **报单通知**

+ **Request URL**  `/ws`

+ **Request**

```
{
	"SendTopicAction": {
		"Action": "1",
		"FilterValue": "",
		"LocalNo": 5,
		"ResumeNo": -1,
		"TopicID": "3",
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
	"action": "PushOrder",
	"localNo": 232,
	"seq": 123,
	"changeType": "3",
	"result": [			
				{"table":"Order",
				 "data":{
					,MemberID:"1983723"
					,TradeUnitID:"1983723"
					,AccountID:"1983723"
					,LocalID:"1983723"
					,ExchangeID:"CEX"
					,InstrumentID:"BTCUSD"
					,OrderPriceType:"1"
					,Direction:"1"
					,OffsetFlag:"1"
					,Price:182582.18
					,Volume:182582.18
					,VolumeDisplay:182582.18
					,VolumeMode:"1"
					,Cost:182582.18
					,OrderType:"1"
					,GTDTime:1657543832
					,MinVolume:182582.18
					,BusinessType:"1"
					,BusinessValue:""
					,CloseOrderID:"1983723"
					,IsCrossMargin:1
					,Remark:"my_little_tips"
					,OrderID:"12376422300333"
					,CopyMemberID:"1983723"
					,CopyOrderID:"1983723"
					,Leverage:125
					,CopyProfitRate:182582.18
					,APPID:"ios"
					,PositionID:"1983723"
					,TriggerPrice:182582.18
					,Reserve:182582.18
					,OrderStatus:"1"
					,DeriveSource:"1"
					,DeriveDetail:""
					,VolumeTraded:182582.18
					,VolumeRemain:182582.18
					,VolumeCancled:182582.18
					,InsertTime:1657543832
					,UpdateTime:1657543832
					,Priority:0
					,TimeSortNo:1657543832
					,FrontNo:1
					,PriceCurrency:"BTC"
					,FeeCurrency:"BTC"
					,ClearCurrency:"BTC"
					,FrozenMoney:182582.18
					,FrozenFee:182582.18
					,FrozenMargin:182582.18
					,Fee:0.22364
					,CloseProfit:10
					,Turnover:182582.18
					,RelatedOrderID:"1983723"
					,BusinessResult:""
					,BusinessNo:11176422300333
					,Tradable:0
					,SettlementGroup:"999"
					,PosiDirection:"1"
					,TradePrice:182582.18
					,OpenPrice:182582.18
					,TriggerOrderID:"1983723"
					,SLTriggerPrice:182582.18
					,TPTriggerPrice:182582.18
					,CopyProfit:182582.18
					,Position:182582.18
					,UserID:"1983723"
					,LastPriceByInsert:182582.18
					,BidPrice1ByInsert:182582.18
					,AskPrice1ByInsert:182582.18
					,Available:182582.18
					,CreateTime:""
					}
				}
	]
}
```

+ **Response Parameters**

Order:

parameter | shortName | type | description | remark
-------- | -------- | -------- | -------- | -------- 					
MemberID | M | string | 成员代码 | 
TradeUnitID | T | string | 交易单元代码 | 
AccountID | A | string | 资金账号 | 
LocalID | L | string | 报单本地标识 | 
ExchangeID | E | string | 交易所代码 | 
InstrumentID | I | string | 标的代码 | 
OrderPriceType | PT | string | 报单价格类型 | 0:限价(手动指定Price值);1:任意价(Price按最高最低限价取值);2:最优价/对手价(Price按对方1档价取值);3:五档价(Price按对方5档价取值);4:十档价(类同上);7:最优Marker价(Price按本方1档价或其+1Tick取值);8:Price按委托额Cost和订单簿情况确定值;9:保底点差价:成交价格Price不会劣于系统设置差价成交;
Direction | D | string | 买卖方向 | 0:买;1:卖;
OffsetFlag | O | string | 开平标志 | 0:开仓;1:平仓;2:强平;3:平今;4:平昨;5:全平;6:指定订单平仓;7:指定成交平仓;8:最大平仓;
Price | P | double | 报单价格 | 
Volume | V | double | 数量 | 
VolumeDisplay | D | double | 显示数量 | 
VolumeMode | VM | string | 数量取值方式 | 0:百分比;1:绝对值;
Cost | C | double | 委托额 | 
OrderType | OT | string | 订单类型 | 0:普通订单;1:(FillAndKill/IOC)立即完成并且剩余撤销;2:(FillOrKill)立即完成全部或者最小订单要求否则全部撤销;3:需要进入订单簿被动成交,否则全部撤销(只做Maker/PostOnly);4:不需要撮合直接成交;
GTDTime | GT | double | GTD时间 | 
MinVolume | MV | double | 最小成交量 | 
BusinessType | BT | string | 业务类别 | 0:普通;1:申赎;2:行权;3:中立仓申报;4:递延交割申报;5:互换定单;6:质押;7:合并分拆;8:转股;9:回售回购;a:投票;b:融资融券;c:期权执行;d:传递被平仓订单BusinessValue;P:自定义11;Q:自定义10;R:自定义9;S:连续交易;T:自定义7;U:画线委托;V:限价市价;W:条件;X:止盈止损;Y:追踪出场;Z:指标类型;
BusinessValue | BV | string | 业务值 | 
CloseOrderID | CI | string | 平仓指定开仓的订单号 | 
IsCrossMargin | IsCrossMargin | int | 是否全仓 | 
Remark | R | string | 备注 | 
OrderID | OrderID | string | 报单系统唯一代码 | 
CopyMemberID | CopyMemberID | string | 带单员代码 | 
CopyOrderID | CopyOrderID | string | 带单员报单号 | 
Leverage | Leverage | double | 委托单杠杆倍数 | 
CopyProfitRate | CopyProfitRate | double | 带单分配比例 | 
APPID | APPID | string | 应用编号 | 
PositionID | B | string | 持仓代码 | 
TriggerPrice | TriggerPrice | double | 触发价 | 
Reserve | Reserve | double | 保留资金 | 
OrderStatus | OrderStatus | string | 报单状态 | 0:未设置状态;1:全部成交;2:部分成交未撤单;3:部分成交已撤单;4:未成交未撤单;6:无成交已撤单;
DeriveSource | DeriveSource | string | 衍生来源 | 0:正常;1:报价衍生;2:期权执行;3:组合衍生;4:场外成交强平衍生;5:场外成交大宗交易衍生;6:场外成交期转现衍生;7:场外成交只刷单衍生;8:触发订单衍生;9:止损触发订单衍生;a:止盈触发订单衍生;b:资金操作衍生;c:仓位合并衍生;d:CFD订单衍生;e:场外成交ADL衍生;f:回购手续费不能是币;
DeriveDetail | DeriveDetail | string | 衍生明细 | 
VolumeTraded | VolumeTraded | double | 成交数量 | 
VolumeRemain | VolumeRemain | double | 剩余数量 | 
VolumeCancled | VolumeCancled | double | 已经撤单数量 | 
InsertTime | InsertTime | double | 插入时间 | 
UpdateTime | UpdateTime | double | 更新时间 | 
Priority |  | int | 优先权 | 
TimeSortNo |  | double | 按时间排队的序号 | 
FrontNo | FrontNo | int | 前置编号 | 
PriceCurrency | PriceCurrency | string | 计价币种 | 
FeeCurrency | FeeCurrency | string | 手续费币种 | 
ClearCurrency | ClearCurrency | string | 清算币种 | 
FrozenMoney | FrozenMoney | double | 冻结资金 | 
FrozenFee | FrozenFee | double | 冻结手续费 | 
FrozenMargin | FrozenMargin | double | 冻结保证金 | 
Fee | Fee | double | 手续费 | 
CloseProfit | CloseProfit | double | 平仓盈亏 | 
Turnover | Turnover | double | 成交金额 | 
RelatedOrderID | RelatedOrderID | string | 关联报单号 | 
BusinessResult | BusinessResult | string | 业务执行结果 | 
BusinessNo | BusinessNo | double | 业务序列号 | 
Tradable |  | int | 是否可以参加交易 | 
SettlementGroup |  | string | 结算组编号 | 
PosiDirection | PosiDirection | string | 持仓多空方向 | 0:多头;1:空头;2:净;
TradePrice | TradePrice | double | 成交均价 | 
OpenPrice | OpenPrice | double | 平仓成交时的开仓均价 | 
TriggerOrderID | TriggerOrderID | string | 触发报单号 | 
SLTriggerPrice | SLTriggerPrice | double | 开仓报单成交之后的止损触发价 | 
TPTriggerPrice | TPTriggerPrice | double | 开仓报单成交之后的止盈触发价 | 
CopyProfit | CopyProfit | double | 带单盈利分配 | 
Position | Position | double | 成交时的持仓量 | 
UserID | U | string | 交易用户代码 | 
LastPriceByInsert |  | double | 插入时的最新价 | 
BidPrice1ByInsert |  | double | 插入时的买一价 | 
AskPrice1ByInsert |  | double | 插入时的卖一价 | 
Available | Available | double | 可用资金 | 
CreateTime |  | string | 创建时间 | 
## **持仓汇总通知**

+ **Request URL**  `/ws`

+ **Request**

```
{
	"SendTopicAction": {
		"Action": "1",
		"FilterValue": "",
		"LocalNo": 5,
		"ResumeNo": -1,
		"TopicID": "3",
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
	"action": "PushMemberPosition",
	"localNo": 232,
	"seq": 123,
	"changeType": "3",
	"result": [			
				{"table":"MemberPosition",
				 "data":{
					,MemberID:"1983723"
					,AccountID:"1983723"
					,ExchangeID:"CEX"
					,InstrumentID:"BTCUSD"
					,Position:182582.18
					,LongFrozen:182582.18
					,ShortFrozen:182582.18
					,DefaultLeverage:125
					,DefaultCrossMargin:0
					,InsertTime:1657543832
					,UpdateTime:1657543832
					}
				}
	]
}
```

+ **Response Parameters**

MemberPosition:

parameter | shortName | type | description | remark
-------- | -------- | -------- | -------- | -------- 					
MemberID | MemberID | string | 成员代码 | 
AccountID | AccountID | string | 资金账号 | 
ExchangeID | ExchangeID | string | 交易所代码 | 
InstrumentID | InstrumentID | string | 标的代码 | 
Position | Position | double | 总持仓 | 
LongFrozen | LongFrozen | double | 多头冻结 | 
ShortFrozen | ShortFrozen | double | 空头冻结 | 
DefaultLeverage | DefaultLeverage | double | 默认杠杆倍数 | 
DefaultCrossMargin | DefaultCrossMargin | int | 默认是否全仓 | 
InsertTime | InsertTime | double | 插入时间 | 
UpdateTime | UpdateTime | double | 更新时间 | 
## **持仓通知**

+ **Request URL**  `/ws`

+ **Request**

```
{
	"SendTopicAction": {
		"Action": "1",
		"FilterValue": "",
		"LocalNo": 5,
		"ResumeNo": -1,
		"TopicID": "3",
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
	"action": "PushPosition",
	"localNo": 232,
	"seq": 123,
	"changeType": "3",
	"result": [			
				{"table":"Position",
				 "data":{
					,PositionID:"1983723"
					,MemberID:"1983723"
					,TradeUnitID:"1983723"
					,ExchangeID:"CEX"
					,InstrumentID:"BTCUSD"
					,PositionClass:"1"
					,PosiDirection:"1"
					,PrePosition:182582.18
					,Position:182582.18
					,LongFrozen:182582.18
					,ShortFrozen:182582.18
					,PreLongFrozen:182582.18
					,PreShortFrozen:182582.18
					,HighestPosition:182582.18
					,ClosePosition:182582.18
					,PositionCost:182582.18
					,CostPrice:182582.18
					,UseMargin:182582.18
					,FrozenMargin:182582.18
					,LongFrozenMargin:182582.18
					,ShortFrozenMargin:182582.18
					,CloseProfit:10
					,TotalPositionCost:182582.18
					,TotalCloseProfit:182582.18
					,OpenPrice:182582.18
					,ClosePrice:182582.18
					,TradeFee:182582.18
					,PositionFee:182582.18
					,Leverage:125
					,AccountID:"1983723"
					,PriceCurrency:"BTC"
					,ClearCurrency:"BTC"
					,SettlementGroup:"999"
					,IsCrossMargin:1
					,CloseOrderID:"1983723"
					,SLTriggerPrice:182582.18
					,TPTriggerPrice:182582.18
					,BeginTime:1657543832
					,InsertTime:1657543832
					,LastOpenTime:1657543832
					,UpdateTime:1657543832
					,BusinessNo:11176422300333
					,IsAutoAddMargin:1
					,Frequency:0
					,MaintMargin:182582.18
					,UnRealProfit:182582.18
					,LiquidPrice:182582.18
					,CreateTime:""
					,CopyMemberID:"1983723"
					,CopyProfitRate:182582.18
					,CopyProfit:182582.18
					,FirstTradeID:"1983723"
					,LastTradeID:"1983723"
					,BusinessType:"1"
					,BusinessValue:""
					,Reserve:182582.18
					,ReserveProfit:182582.18
					,Remark:"my_little_tips"
					}
				}
	]
}
```

+ **Response Parameters**

Position:

parameter | shortName | type | description | remark
-------- | -------- | -------- | -------- | -------- 					
PositionID | PositionID | string | 持仓代码 | 
MemberID | MemberID | string | 成员代码 | 
TradeUnitID | TradeUnitID | string | 交易单元代码 | 
ExchangeID | ExchangeID | string | 交易所代码 | 
InstrumentID | InstrumentID | string | 标的代码 | 
PositionClass | PositionClass | string | 持仓类型 | 0:不设置;1:保证金交易;3:资产;4:持币成本;
PosiDirection | PosiDirection | string | 持仓多空方向 | 0:多头;1:空头;2:净;
PrePosition | PrePosition | double | 上次持仓 | 
Position | Position | double | 总持仓 | 
LongFrozen | LongFrozen | double | 多头冻结 | 
ShortFrozen | ShortFrozen | double | 空头冻结 | 
PreLongFrozen | PreLongFrozen | double | 昨日多头冻结 | 
PreShortFrozen | PreShortFrozen | double | 昨日空头冻结 | 
HighestPosition | HighestPosition | double | 最大持仓 | 
ClosePosition | ClosePosition | double | 可平持仓 | 
PositionCost | PositionCost | double | 持仓成本 | 
CostPrice | CostPrice | double | 成本价 | 
UseMargin | UseMargin | double | 占用保证金 | 
FrozenMargin | FrozenMargin | double | 冻结保证金 | 
LongFrozenMargin | LongFrozenMargin | double | 多头冻结保证金 | 
ShortFrozenMargin | ShortFrozenMargin | double | 空头冻结保证金 | 
CloseProfit | CloseProfit | double | 平仓盈亏 | 
TotalPositionCost | TotalPositionCost | double | 开仓成本 | 
TotalCloseProfit | TotalCloseProfit | double | 总平仓盈亏 | 
OpenPrice | OpenPrice | double | 开仓均价 | 
ClosePrice | ClosePrice | double | 平仓均价 | 
TradeFee | TradeFee | double | 交易费用 | 
PositionFee | PositionFee | double | 持仓费用或者股票分红等 | 
Leverage | Leverage | double | 杠杆倍数 | 
AccountID | AccountID | string | 资金账号 | 
PriceCurrency | PriceCurrency | string | 计价币种 | 
ClearCurrency | ClearCurrency | string | 清算币种 | 
SettlementGroup | SettlementGroup | string | 结算组编号 | 
IsCrossMargin | IsCrossMargin | int | 是否全仓 | 
CloseOrderID | CloseOrderID | string | 平仓单代码 | 
SLTriggerPrice | SLTriggerPrice | double | 止损触发价 | 
TPTriggerPrice | TPTriggerPrice | double | 止盈触发价 | 
BeginTime | BeginTime | double | 持仓不为0的开始时间 | 
InsertTime | InsertTime | double | 插入时间(新持仓时间) | 
LastOpenTime | LastOpenTime | double | 最后开仓时间 | 
UpdateTime | UpdateTime | double | 更新时间 | 
BusinessNo | BusinessNo | double | 业务序列号 | 
IsAutoAddMargin | IsAutoAddMargin | int | 是否自动追加保证金 | 
Frequency | Frequency | int | 每秒更新的次数 | 
MaintMargin | MaintMargin | double | 维持保证金 | 
UnRealProfit | UnRealProfit | double | 未实现盈亏 | 
LiquidPrice | LiquidPrice | double | 清算价格 | 
CreateTime | CreateTime | string | 创建时间 | 
CopyMemberID | CopyMemberID | string | 带单员代码 | 
CopyProfitRate | CopyProfitRate | double | 带单分配比例 | 
CopyProfit | CopyProfit | double | 带单盈利分配 | 
FirstTradeID | FID | string | 初始成交订单号 | 
LastTradeID | LID | string | 结束成交订单号 | 
BusinessType | BT | string | 业务类别 | 0:普通;1:申赎;2:行权;3:中立仓申报;4:递延交割申报;5:互换定单;6:质押;7:合并分拆;8:转股;9:回售回购;a:投票;b:融资融券;c:期权执行;d:传递被平仓订单BusinessValue;P:自定义11;Q:自定义10;R:自定义9;S:连续交易;T:自定义7;U:画线委托;V:限价市价;W:条件;X:止盈止损;Y:追踪出场;Z:指标类型;
BusinessValue | BV | string | 业务值 | 
Reserve | Reserve | double | 保留资金 | 
ReserveProfit | ReserveProfit | double | 保留资金盈亏 | 
Remark | R | string | 备注 | 
## **资金通知**

+ **Request URL**  `/ws`

+ **Request**

```
{
	"SendTopicAction": {
		"Action": "1",
		"FilterValue": "",
		"LocalNo": 5,
		"ResumeNo": -1,
		"TopicID": "3",
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
	"action": "PushAccount",
	"localNo": 232,
	"seq": 123,
	"changeType": "3",
	"result": [			
				{"table":"Account",
				 "data":{
					,SettlementGroup:"999"
					,AccountID:"1983723"
					,Currency:"BTC"
					,MemberID:"1983723"
					,Available:182582.18
					,Withdrawable:182582.18
					,PreBalance:182582.18
					,Balance:182582.18
					,PositionCost:182582.18
					,UseMargin:182582.18
					,CrossMargin:182582.18
					,CloseProfit:10
					,Deposit:182582.18
					,Withdraw:182582.18
					,FrozenMargin:182582.18
					,FrozenMoney:182582.18
					,FrozenFee:182582.18
					,Fee:0.22364
					,MoneyChange:182582.18
					,Reserve:182582.18
					,ReserveAvailable:182582.18
					,ReserveLimit:182582.18
					,ReserveProfit:182582.18
					,ReserveType:"1"
					,ReserveActive:0
					,TotalCloseProfit:182582.18
					,TotalDeposit:182582.18
					,TotalWithdraw:182582.18
					,TotalFee:182582.18
					,TotalMoneyChange:182582.18
					,InsertTime:1657543832
					,UpdateTime:1657543832
					,AccountType:""
					,BusinessNo:11176422300333
					,MarginRateGrade:""
					,TradingRightGrade:""
					,Frequency:0
					,MaintMargin:182582.18
					,UnRealProfit:182582.18
					,CopyProfit:182582.18
					,CFDType:"1"
					,FeeGrade:""
					,CFDGrade:""
					,Borrow:182582.18
					,Remark:"my_little_tips"
					,MaxLocalID:"1983723"
					,CurrValue:182582.18
					,CanReduce:0
					}
				}
	]
}
```

+ **Response Parameters**

Account:

parameter | shortName | type | description | remark
-------- | -------- | -------- | -------- | -------- 					
SettlementGroup | SettlementGroup | string | 结算组编号 | 
AccountID | AccountID | string | 资金账号 | 
Currency | Currency | string | 币种 | 
MemberID | MemberID | string | 成员代码 | 
Available | Available | double | 可用资金 | 
Withdrawable | Withdrawable | double | 可取资金 | 
PreBalance | PreBalance | double | 上次静态权益 | 
Balance | Balance | double | 静态权益 | 
PositionCost | PositionCost | double | 持仓成本 | 
UseMargin | UseMargin | double | 所有占用保证金 | 
CrossMargin | CrossMargin | double | 全仓保证金 | 
CloseProfit | CloseProfit | double | 平仓盈亏 | 
Deposit | Deposit | double | 入金金额 | 
Withdraw | Withdraw | double | 出金金额 | 
FrozenMargin | FrozenMargin | double | 冻结保证金 | 
FrozenMoney | FrozenMoney | double | 冻结资金 | 
FrozenFee | FrozenFee | double | 冻结手续费 | 
Fee | Fee | double | 手续费 | 
MoneyChange | MoneyChange | double | 资金变化 | 
Reserve | Reserve | double | 保留资金 | 
ReserveAvailable | ReserveAvailable | double | 保留资金可用 | 
ReserveLimit | ReserveLimit | double | 保留资金限制 | 
ReserveProfit | ReserveProfit | double | 保留资金盈亏 | 
ReserveType | ReserveType | string | 保留资金类型 | 0:先亏损保留资金再亏损真钱(劣后);1:先亏损真钱再亏损保留资金(优先);2:保留资金不做亏损(抵押金);
ReserveActive | ReserveActive | int | 保留资金是否活跃 | 
TotalCloseProfit | TotalCloseProfit | double | 总平仓盈亏 | 
TotalDeposit | TotalDeposit | double | 总入金金额 | 
TotalWithdraw | TotalWithdraw | double | 总出金金额 | 
TotalFee | TotalFee | double | 总手续费 | 
TotalMoneyChange | TotalMoneyChange | double | 总资金变化 | 
InsertTime | InsertTime | double | 插入时间 | 
UpdateTime | UpdateTime | double | 更新时间 | 
AccountType | AccountType | string | 账户类型 | 
BusinessNo | BusinessNo | double | 业务序列号 | 
MarginRateGrade | MarginRateGrade | string | 保证金等级 | 
TradingRightGrade | TradingRightGrade | string | 权限等级 | 
Frequency | Frequency | int | 每秒更新的次数 | 
MaintMargin | MaintMargin | double | 维持保证金 | 
UnRealProfit | UnRealProfit | double | 未实现盈亏 | 
CopyProfit | CopyProfit | double | 带单盈利分配 | 
CFDType | CFDType | string | 点差接管类型 | 0:先撮合后点差接管;1:只撮合不点差接管;2:不撮合只点差接管;
FeeGrade | FeeGrade | string | 手续费等级 | 
CFDGrade | CFDGrade | string | 差价等级 | 
Borrow | Borrow | double | 借款周期结算量 | 
Remark | Remark | string | 备注 | 
MaxLocalID | MaxLocalID | string | 最大本地标识 | 
CurrValue | CurrValue | double | 当前价值 | 
CanReduce | CanReduce | int | 是否可以出金 | 
## **市场成交通知**

+ **Request URL**  `/ws`

+ **Request**

```
{
	"SendTopicAction": {
		"Action": "1",
		"FilterValue": "DEX_BTCUSD",
		"LocalNo": 5,
		"ResumeNo": -1,
		"TopicID": "2",
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
	"action": "PushMarketTrade",
	"localNo": 232,
	"seq": 123,
	"changeType": "3",
	"result": [			
				{"table":"MarketTrade",
				 "data":{
					,TradeID:"16576422300333"
					,ExchangeID:"CEX"
					,InstrumentID:"BTCUSD"
					,Direction:"1"
					,Price:182582.18
					,Volume:182582.18
					,InsertTime:1657543832
					}
				}
	]
}
```

+ **Response Parameters**

MarketTrade:

parameter | shortName | type | description | remark
-------- | -------- | -------- | -------- | -------- 					
TradeID |  | string | 成交代码 | 
ExchangeID |  | string | 交易所代码 | 
InstrumentID |  | string | 标的代码 | 
Direction | D | string | Taker方买卖方向 | 0:买;1:卖;
Price | P | double | 成交价格 | 
Volume | V | double | 成交数量 | 
InsertTime | I | double | 插入时间 | 
## **订单簿变化通知**

+ **Request URL**  `/ws`

+ **Request**

```
{
	"SendTopicAction": {
		"Action": "1",
		"FilterValue": "",
		"LocalNo": 5,
		"ResumeNo": -1,
		"TopicID": "3",
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
	"action": "PushMarketOrder",
	"localNo": 232,
	"seq": 123,
	"changeType": "3",
	"result": [			
				{"table":"MarketOrder",
				 "data":{
					,ExchangeID:"CEX"
					,InstrumentID:"BTCUSD"
					,Direction:"1"
					,Price:182582.18
					,Volume:182582.18
					,Orders:0
					,BusinessNo:11176422300333
					,TrueVolume:182582.18
					}
				}
				{"table":"TickMarketOrder",
				 "data":{
					,ExchangeID:"CEX"
					,InstrumentID:"BTCUSD"
					,Direction:"1"
					,Price:182582.18
					,Volume:182582.18
					,Orders:0
					,BusinessNo:11176422300333
					,TrueVolume:182582.18
					,Tick:182582.18
					,SubIndex:""
					}
				}
				{"table":"CurrentTime",
				 "data":{
					,SystemID:"1983723"
					,ZoneDate:""
					,ZoneTime:""
					,ZoneDays:""
					,CurrTime:1657543832
					,InsertTime:1657543832
					,UpdateTime:1657543832
					,TimeZone:0
					,LastTradeID:"1983723"
					,LastOrderID:"1983723"
					,LastAccountDetailID:"1983723"
					,LastTimeSortNo:1657543832
					,LastBusinessNo:182582.18
					}
				}
	]
}
```

+ **Response Parameters**

MarketOrder:

parameter | shortName | type | description | remark
-------- | -------- | -------- | -------- | -------- 					
ExchangeID |  | string | 交易所代码 | 
InstrumentID |  | string | 标的代码 | 
Direction | D | string | 买卖方向 | 0:买;1:卖;
Price | P | double | 价格 | 
Volume | V | double | 数量 | 
Orders |  | int | 订单数量 | 
BusinessNo |  | double | 最后变化序列号 | 
TrueVolume |  | double | 真实报单数量 | 

TickMarketOrder:

parameter | shortName | type | description | remark
-------- | -------- | -------- | -------- | -------- 					
ExchangeID |  | string | 交易所代码 | 
InstrumentID |  | string | 标的代码 | 
Direction | D | string | 买卖方向 | 0:买;1:卖;
Price | P | double | 价格 | 
Volume | V | double | 数量 | 
Orders |  | int | 订单数量 | 
BusinessNo |  | double | 最后变化序列号 | 
TrueVolume |  | double | 真实报单数量 | 
Tick |  | double | 最小变动价位 | 
SubIndex |  | string | 索引 | 

CurrentTime:

parameter | shortName | type | description | remark
-------- | -------- | -------- | -------- | -------- 					
SystemID |  | string | 系统名 | 
ZoneDate | D | string | 本地日期 | 
ZoneTime | T | string | 本地时间 | 
ZoneDays | Z | string | 本地天数 | 
CurrTime | C | double | 当前时间（秒） | 
InsertTime | InsertTime | double | 插入时间 | 
UpdateTime | U | double | 更新时间（毫秒） | 
TimeZone | TimeZone | int | 本地时区 | 
LastTradeID | LastTradeID | string | 最新最新成交序列号 | 
LastOrderID | LastOrderID | string | 最新订单序列号 | 
LastAccountDetailID | LastAccountDetailID | string | 最新资金明细序列号 | 
LastTimeSortNo | LastTimeSortNo | double | 最新排序序列号 | 
LastBusinessNo | LastBusinessNo | double | 最新事务序列号 | 
## **K线通知**

+ **Request URL**  `/ws`

+ **Request**

```
{
	"SendTopicAction": {
		"Action": "1",
		"FilterValue": "DEX_BTCUSD_15m",
		"LocalNo": 5,
		"ResumeNo": -1,
		"TopicID": "4",
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
	"action": "PushKLine",
	"localNo": 232,
	"seq": 123,
	"changeType": "3",
	"result": [			
				{"table":"KLine",
				 "data":{
					,ExchangeID:"CEX"
					,InstrumentID:"BTCUSD"
					,Bar:""
					,InsertTime:1657543832
					,OpenPrice:182582.18
					,HighestPrice:182582.18
					,LowestPrice:182582.18
					,ClosePrice:182582.18
					,Volume:182582.18
					,Turnover:182582.18
					}
				}
				{"table":"LastKLine",
				 "data":{
					,ExchangeID:"CEX"
					,InstrumentID:"BTCUSD"
					,Bar:""
					,InsertTime:1657543832
					,OpenPrice:182582.18
					,HighestPrice:182582.18
					,LowestPrice:182582.18
					,ClosePrice:182582.18
					,Volume:182582.18
					,Turnover:182582.18
					,UpdateTime:1657543832
					}
				}
	]
}
```

+ **Response Parameters**

KLine:

parameter | shortName | type | description | remark
-------- | -------- | -------- | -------- | -------- 					
ExchangeID |  | string | 交易所代码 | 
InstrumentID |  | string | 标的代码 | 
Bar |  | string | K线周期代码(n[m/h/d/o]) | 
InsertTime | I | double | 插入时间 | 
OpenPrice | O | double | 开盘价 | 
HighestPrice | H | double | 最高价 | 
LowestPrice | L | double | 最低价 | 
ClosePrice | C | double | 收盘价 | 
Volume | V | double | 数量 | 
Turnover | T | double | 成交金额 | 

LastKLine:

parameter | shortName | type | description | remark
-------- | -------- | -------- | -------- | -------- 					
ExchangeID |  | string | 交易所代码 | 
InstrumentID |  | string | 标的代码 | 
Bar |  | string | K线周期代码(n[m/h/d/o]) | 
InsertTime | I | double | 插入时间 | 
OpenPrice | O | double | 开盘价 | 
HighestPrice | H | double | 最高价 | 
LowestPrice | L | double | 最低价 | 
ClosePrice | C | double | 收盘价 | 
Volume | V | double | 数量 | 
Turnover | T | double | 成交金额 | 
UpdateTime | UpdateTime | double | 更新时间 | 
## **普通行情通知**

+ **Request URL**  `/ws`

+ **Request**

```
{
	"SendTopicAction": {
		"Action": "1",
		"FilterValue": "",
		"LocalNo": 5,
		"ResumeNo": -1,
		"TopicID": "3",
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
	"action": "PushMarketOverView",
	"localNo": 232,
	"seq": 123,
	"changeType": "3",
	"result": [			
				{"table":"MarketOverView",
				 "data":{
					,ExchangeID:"CEX"
					,InstrumentID:"BTCUSD"
					,ProductGroup:"Swap"
					,UpdateTime:1657543832
					,UpperLimitPrice:182582.18
					,LowerLimitPrice:182582.18
					,UnderlyingPrice:182582.18
					,MarkedPrice:182582.18
					,PositionFeeRate:182582.18
					,HighestPrice:182582.18
					,LowestPrice:182582.18
					,LastPrice:182582.18
					,Volume:182582.18
					,Turnover:182582.18
					,OpenInterest:182582.18
					,SettlementPrice:182582.18
					,OpenPrice:182582.18
					,InstrumentStatus:"1"
					,PrePositionFeeRate:182582.18
					,PositionFeeTime:1657543832
					,BidPrice1:182582.18
					,AskPrice1:182582.18
					,HighestPrice24:182582.18
					,LowestPrice24:182582.18
					,Volume24:182582.18
					,Turnover24:182582.18
					,OpenPrice24:182582.18
					}
				}
	]
}
```

+ **Response Parameters**

MarketOverView:

parameter | shortName | type | description | remark
-------- | -------- | -------- | -------- | -------- 					
ExchangeID | X | string | 交易所代码 | 
InstrumentID | I | string | 标的代码 | 
ProductGroup | G | string | 产品组 | 
UpdateTime | U | double | 更新时间 | 
UpperLimitPrice | C | double | 最高限价 | 
LowerLimitPrice | F | double | 最低限价 | 
UnderlyingPrice | D | double | 基础标的价格 | 
MarkedPrice | M | double | 标记价格 | 
PositionFeeRate | R | double | 预计持仓费按比例 | 
HighestPrice | H | double | 当日最高价 | 
LowestPrice | L | double | 当日最低价 | 
LastPrice | N | double | 最新价 | 
Volume | V | double | 当日数量 | 
Turnover | T | double | 当日成交金额 | 
OpenInterest | P | double | 持仓量 | 
SettlementPrice | S | double | 预估结算价 | 
OpenPrice | O | double | 当日开盘价 | 
InstrumentStatus | B | string | 标的交易状态 | 0:开盘前;1:非交易;2:连续交易;3:集合竞价报单;4:集合竞价价格平衡;5:集合竞价撮合;6:收盘;7:不活跃;
PrePositionFeeRate | E | double | 上次持仓费按比例 | 
PositionFeeTime | PF | double | 收取持仓费用时间 | 
BidPrice1 | BP1 | double | 申买价一 | 
AskPrice1 | AP1 | double | 申卖价一 | 
HighestPrice24 | H2 | double | 24小时最高价 | 
LowestPrice24 | L2 | double | 24小时最低价 | 
Volume24 | V2 | double | 24小时数量 | 
Turnover24 | T2 | double | 24小时成交金额 | 
OpenPrice24 | O2 | double | 24小时开盘价 | 
## **全量延时订单簿变化通知**

+ **Request URL**  `/ws`

+ **Request**

```
{
	"SendTopicAction": {
		"Action": "1",
		"FilterValue": "",
		"LocalNo": 5,
		"ResumeNo": -1,
		"TopicID": "3",
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
	"action": "PushDelayMarketOrder",
	"localNo": 232,
	"seq": 123,
	"changeType": "3",
	"result": [			
				{"table":"MarketOrder",
				 "data":{
					,ExchangeID:"CEX"
					,InstrumentID:"BTCUSD"
					,Direction:"1"
					,Price:182582.18
					,Volume:182582.18
					,Orders:0
					,BusinessNo:11176422300333
					,TrueVolume:182582.18
					}
				}
				{"table":"CurrentTime",
				 "data":{
					,SystemID:"1983723"
					,ZoneDate:""
					,ZoneTime:""
					,ZoneDays:""
					,CurrTime:1657543832
					,InsertTime:1657543832
					,UpdateTime:1657543832
					,TimeZone:0
					,LastTradeID:"1983723"
					,LastOrderID:"1983723"
					,LastAccountDetailID:"1983723"
					,LastTimeSortNo:1657543832
					,LastBusinessNo:182582.18
					}
				}
	]
}
```

+ **Response Parameters**

MarketOrder:

parameter | shortName | type | description | remark
-------- | -------- | -------- | -------- | -------- 					
ExchangeID |  | string | 交易所代码 | 
InstrumentID |  | string | 标的代码 | 
Direction | D | string | 买卖方向 | 0:买;1:卖;
Price | P | double | 价格 | 
Volume | V | double | 数量 | 
Orders |  | int | 订单数量 | 
BusinessNo |  | double | 最后变化序列号 | 
TrueVolume |  | double | 真实报单数量 | 

CurrentTime:

parameter | shortName | type | description | remark
-------- | -------- | -------- | -------- | -------- 					
SystemID |  | string | 系统名 | 
ZoneDate | D | string | 本地日期 | 
ZoneTime | T | string | 本地时间 | 
ZoneDays | Z | string | 本地天数 | 
CurrTime | C | double | 当前时间（秒） | 
InsertTime | InsertTime | double | 插入时间 | 
UpdateTime | U | double | 更新时间（毫秒） | 
TimeZone | TimeZone | int | 本地时区 | 
LastTradeID | LastTradeID | string | 最新最新成交序列号 | 
LastOrderID | LastOrderID | string | 最新订单序列号 | 
LastAccountDetailID | LastAccountDetailID | string | 最新资金明细序列号 | 
LastTimeSortNo | LastTimeSortNo | double | 最新排序序列号 | 
LastBusinessNo | LastBusinessNo | double | 最新事务序列号 | 
## **五档深度行情通知**

+ **Request URL**  `/ws`

+ **Request**

```
{
	"SendTopicAction": {
		"Action": "1",
		"FilterValue": "DEX_BTCUSD_3",
		"LocalNo": 5,
		"ResumeNo": -1,
		"TopicID": "5",
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
	"action": "PushFiveDepthMarketData",
	"localNo": 232,
	"seq": 123,
	"changeType": "3",
	"result": [			
				{"table":"FiveDepthMarketData",
				 "data":{
					,ExchangeID:"CEX"
					,InstrumentID:"BTCUSD"
					,ProductGroup:"Swap"
					,UpdateTime:1657543832
					,BidPrice1:182582.18
					,BidVolume1:182582.18
					,AskPrice1:182582.18
					,AskVolume1:182582.18
					,BidPrice2:182582.18
					,BidVolume2:182582.18
					,AskPrice2:182582.18
					,AskVolume2:182582.18
					,BidPrice3:182582.18
					,BidVolume3:182582.18
					,AskPrice3:182582.18
					,AskVolume3:182582.18
					,BidPrice4:182582.18
					,BidVolume4:182582.18
					,AskPrice4:182582.18
					,AskVolume4:182582.18
					,BidPrice5:182582.18
					,BidVolume5:182582.18
					,AskPrice5:182582.18
					,AskVolume5:182582.18
					}
				}
	]
}
```

+ **Response Parameters**

FiveDepthMarketData:

parameter | shortName | type | description | remark
-------- | -------- | -------- | -------- | -------- 					
ExchangeID | X | string | 交易所代码 | 
InstrumentID | I | string | 标的代码 | 
ProductGroup | G | string | 产品组 | 
UpdateTime | U | double | 更新时间 | 
BidPrice1 | BP1 | double | 申买价一 | 
BidVolume1 | BV1 | double | 申买量一 | 
AskPrice1 | AP1 | double | 申卖价一 | 
AskVolume1 | AV1 | double | 申卖量一 | 
BidPrice2 | BP2 | double | 申买价二 | 
BidVolume2 | BV2 | double | 申买量二 | 
AskPrice2 | AP2 | double | 申卖价二 | 
AskVolume2 | AV2 | double | 申卖量二 | 
BidPrice3 | BP3 | double | 申买价三 | 
BidVolume3 | BV3 | double | 申买量三 | 
AskPrice3 | AP3 | double | 申卖价三 | 
AskVolume3 | AV3 | double | 申卖量三 | 
BidPrice4 | BP4 | double | 申买价四 | 
BidVolume4 | BV4 | double | 申买量四 | 
AskPrice4 | AP4 | double | 申卖价四 | 
AskVolume4 | AV4 | double | 申卖量四 | 
BidPrice5 | BP5 | double | 申买价五 | 
BidVolume5 | BV5 | double | 申买量五 | 
AskPrice5 | AP5 | double | 申卖价五 | 
AskVolume5 | AV5 | double | 申卖量五 | 
## **十档深度行情通知**

+ **Request URL**  `/ws`

+ **Request**

```
{
	"SendTopicAction": {
		"Action": "1",
		"FilterValue": "DEX_BTCUSD_2",
		"LocalNo": 5,
		"ResumeNo": -1,
		"TopicID": "10",
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
	"action": "PushTenDepthMarketData",
	"localNo": 232,
	"seq": 123,
	"changeType": "3",
	"result": [			
				{"table":"TenDepthMarketData",
				 "data":{
					,ExchangeID:"CEX"
					,InstrumentID:"BTCUSD"
					,ProductGroup:"Swap"
					,UpdateTime:1657543832
					,BidPrice1:182582.18
					,BidVolume1:182582.18
					,AskPrice1:182582.18
					,AskVolume1:182582.18
					,BidPrice2:182582.18
					,BidVolume2:182582.18
					,AskPrice2:182582.18
					,AskVolume2:182582.18
					,BidPrice3:182582.18
					,BidVolume3:182582.18
					,AskPrice3:182582.18
					,AskVolume3:182582.18
					,BidPrice4:182582.18
					,BidVolume4:182582.18
					,AskPrice4:182582.18
					,AskVolume4:182582.18
					,BidPrice5:182582.18
					,BidVolume5:182582.18
					,AskPrice5:182582.18
					,AskVolume5:182582.18
					,BidPrice6:182582.18
					,BidVolume6:182582.18
					,AskPrice6:182582.18
					,AskVolume6:182582.18
					,BidPrice7:182582.18
					,BidVolume7:182582.18
					,AskPrice7:182582.18
					,AskVolume7:182582.18
					,BidPrice8:182582.18
					,BidVolume8:182582.18
					,AskPrice8:182582.18
					,AskVolume8:182582.18
					,BidPrice9:182582.18
					,BidVolume9:182582.18
					,AskPrice9:182582.18
					,AskVolume9:182582.18
					,BidPrice10:182582.18
					,BidVolume10:182582.18
					,AskPrice10:182582.18
					,AskVolume10:182582.18
					}
				}
	]
}
```

+ **Response Parameters**

TenDepthMarketData:

parameter | shortName | type | description | remark
-------- | -------- | -------- | -------- | -------- 					
ExchangeID | X | string | 交易所代码 | 
InstrumentID | I | string | 标的代码 | 
ProductGroup | G | string | 产品组 | 
UpdateTime | U | double | 更新时间 | 
BidPrice1 | BP1 | double | 申买价一 | 
BidVolume1 | BV1 | double | 申买量一 | 
AskPrice1 | AP1 | double | 申卖价一 | 
AskVolume1 | AV1 | double | 申卖量一 | 
BidPrice2 | BP2 | double | 申买价二 | 
BidVolume2 | BV2 | double | 申买量二 | 
AskPrice2 | AP2 | double | 申卖价二 | 
AskVolume2 | AV2 | double | 申卖量二 | 
BidPrice3 | BP3 | double | 申买价三 | 
BidVolume3 | BV3 | double | 申买量三 | 
AskPrice3 | AP3 | double | 申卖价三 | 
AskVolume3 | AV3 | double | 申卖量三 | 
BidPrice4 | BP4 | double | 申买价四 | 
BidVolume4 | BV4 | double | 申买量四 | 
AskPrice4 | AP4 | double | 申卖价四 | 
AskVolume4 | AV4 | double | 申卖量四 | 
BidPrice5 | BP5 | double | 申买价五 | 
BidVolume5 | BV5 | double | 申买量五 | 
AskPrice5 | AP5 | double | 申卖价五 | 
AskVolume5 | AV5 | double | 申卖量五 | 
BidPrice6 | BP6 | double | 申买价六 | 
BidVolume6 | BV6 | double | 申买量六 | 
AskPrice6 | AP6 | double | 申卖价六 | 
AskVolume6 | AV6 | double | 申卖量六 | 
BidPrice7 | BP7 | double | 申买价七 | 
BidVolume7 | BV7 | double | 申买量七 | 
AskPrice7 | AP7 | double | 申卖价七 | 
AskVolume7 | AV7 | double | 申卖量七 | 
BidPrice8 | BP8 | double | 申买价八 | 
BidVolume8 | BV8 | double | 申买量八 | 
AskPrice8 | AP8 | double | 申卖价八 | 
AskVolume8 | AV8 | double | 申卖量八 | 
BidPrice9 | BP9 | double | 申买价九 | 
BidVolume9 | BV9 | double | 申买量九 | 
AskPrice9 | AP9 | double | 申卖价九 | 
AskVolume9 | AV9 | double | 申卖量九 | 
BidPrice10 | BP10 | double | 申买价十 | 
BidVolume10 | BV10 | double | 申买量十 | 
AskPrice10 | AP10 | double | 申卖价十 | 
AskVolume10 | AV10 | double | 申卖量十 | 
## **期权行情通知**

+ **Request URL**  `/ws`

+ **Request**

```
{
	"SendTopicAction": {
		"Action": "1",
		"FilterValue": "DEX_BTCUSD",
		"LocalNo": 5,
		"ResumeNo": -1,
		"TopicID": "7",
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
	"action": "PushOptionsMarketData",
	"localNo": 232,
	"seq": 123,
	"changeType": "3",
	"result": [			
				{"table":"OptionsMarketData",
				 "data":{
					,ExchangeID:"CEX"
					,InstrumentID:"BTCUSD"
					,ProductGroup:"Swap"
					,UpdateTime:1657543832
					,PreClosePrice:182582.18
					,PreSettlementPrice:182582.18
					,PreOpenInterest:182582.18
					,PrePositionFeeRate:182582.18
					,ClosePrice:182582.18
					,PositionFeeTime:1657543832
					,SettlementTime:1657543832
					,UpperLimitPrice:182582.18
					,LowerLimitPrice:182582.18
					,UnderlyingPrice:182582.18
					,MarkedPrice:182582.18
					,PositionFeeRate:182582.18
					,HighestPrice:182582.18
					,LowestPrice:182582.18
					,LastPrice:182582.18
					,Volume:182582.18
					,Turnover:182582.18
					,OpenInterest:182582.18
					,SettlementPrice:182582.18
					,OpenPrice:182582.18
					,Delta:182582.18
					,Gamma:182582.18
					,Vega:182582.18
					,Theta:182582.18
					,Rho:182582.18
					,LastVolatility:182582.18
					,Bid1Volatility:182582.18
					,Ask1Volatility:182582.18
					,MaskedVolatility:182582.18
					}
				}
	]
}
```

+ **Response Parameters**

OptionsMarketData:

parameter | shortName | type | description | remark
-------- | -------- | -------- | -------- | -------- 					
ExchangeID | X | string | 交易所代码 | 
InstrumentID | I | string | 标的代码 | 
ProductGroup | G | string | 产品组 | 
UpdateTime | U | double | 更新时间 | 
PreClosePrice |  | double | 上次收盘 | 
PreSettlementPrice |  | double | 上次结算价 | 
PreOpenInterest |  | double | 上次持仓量 | 
PrePositionFeeRate |  | double | 上次持仓费按比例 | 
ClosePrice | ClosePrice | double | 收盘价 | 
PositionFeeTime | PF | double | 收取持仓费用时间 | 
SettlementTime | SettlementTime | double | 结算时间 | 
UpperLimitPrice | C | double | 最高限价 | 
LowerLimitPrice | F | double | 最低限价 | 
UnderlyingPrice | D | double | 基础标的价格 | 
MarkedPrice | M | double | 标记价格 | 
PositionFeeRate | R | double | 预计持仓费按比例 | 
HighestPrice | H | double | 当日最高价 | 
LowestPrice | L | double | 当日最低价 | 
LastPrice | N | double | 最新价 | 
Volume | V | double | 当日数量 | 
Turnover | T | double | 当日成交金额 | 
OpenInterest | P | double | 持仓量 | 
SettlementPrice | S | double | 预估结算价 | 
OpenPrice | O | double | 当日开盘价 | 
Delta | Delta | double | 虚实度 | 
Gamma | Gamma | double | 曲线凸度 | 
Vega | Vega | double | 波动率度 | 
Theta | Theta | double | 时间价值度 | 
Rho | Rho | double | 利率度 | 
LastVolatility | LastVolatility | double | 最新波动率 | 
Bid1Volatility | Bid1Volatility | double | 买一波动率 | 
Ask1Volatility | Ask1Volatility | double | 卖一波动率 | 
MaskedVolatility | MaskedVolatility | double | 标记波动率 | 
## **手续费通知**

+ **Request URL**  `/ws`

+ **Request**

```
{
	"SendTopicAction": {
		"Action": "1",
		"FilterValue": "",
		"LocalNo": 5,
		"ResumeNo": -1,
		"TopicID": "3",
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
	"action": "PushFee",
	"localNo": 232,
	"seq": 123,
	"changeType": "3",
	"result": [			
				{"table":"Fee",
				 "data":{
					,MemberID:"1983723"
					,InstrumentID:"BTCUSD"
					,MatchRole:"1"
					,OpenFeeRate:182582.18
					,OpenFeeAmount:182582.18
					,CloseFeeRate:182582.18
					,CloseFeeAmount:182582.18
					,CloseTodayFeeRate:182582.18
					,CloseTodayFeeAmount:182582.18
					,Remark:"my_little_tips"
					,InsertTime:1657543832
					,UpdateTime:1657543832
					}
				}
	]
}
```

+ **Response Parameters**

Fee:

parameter | shortName | type | description | remark
-------- | -------- | -------- | -------- | -------- 					
MemberID | MemberID | string | 成员代码 | 
InstrumentID | InstrumentID | string | 标的代码 | 
MatchRole | MatchRole | string | 成交角色 | 0:不区分;1:主动成交方;2:被动成交方;3:集合竞价;4:交割;5:场外交易;
OpenFeeRate | OpenFeeRate | double | 开仓手续费按比例 | 
OpenFeeAmount | OpenFeeAmount | double | 开仓手续费按手数 | 
CloseFeeRate | CloseFeeRate | double | 平仓手续费按比例 | 
CloseFeeAmount | CloseFeeAmount | double | 平仓手续费按手数 | 
CloseTodayFeeRate | CloseTodayFeeRate | double | 平今仓手续费按比例 | 
CloseTodayFeeAmount | CloseTodayFeeAmount | double | 平今仓手续费按手数 | 
Remark | Remark | string | 备注 | 
InsertTime | InsertTime | double | 插入时间 | 
UpdateTime | UpdateTime | double | 更新时间 | 
## **保证金率通知**

+ **Request URL**  `/ws`

+ **Request**

```
{
	"SendTopicAction": {
		"Action": "1",
		"FilterValue": "",
		"LocalNo": 5,
		"ResumeNo": -1,
		"TopicID": "3",
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
	"action": "PushMarginRate",
	"localNo": 232,
	"seq": 123,
	"changeType": "3",
	"result": [			
				{"table":"MarginRate",
				 "data":{
					,MemberID:"1983723"
					,InstrumentID:"BTCUSD"
					,ValueMode:"1"
					,MarginSequenceType:"1"
					,RiskSequenceType:"1"
					,LongMarginRatio:182582.18
					,LongMaintMarginRatio:182582.18
					,LongMarginStep:182582.18
					,LongMaintMarginStep:182582.18
					,LongRiskLimit:182582.18
					,LongRiskStep:182582.18
					,ShortMarginRatio:182582.18
					,ShortMaintMarginRatio:182582.18
					,ShortMarginStep:182582.18
					,ShortMaintMarginStep:182582.18
					,ShortRiskLimit:182582.18
					,ShortRiskStep:182582.18
					,DefaultLeverage:125
					,Remark:"my_little_tips"
					,InsertTime:1657543832
					,UpdateTime:1657543832
					}
				}
	]
}
```

+ **Response Parameters**

MarginRate:

parameter | shortName | type | description | remark
-------- | -------- | -------- | -------- | -------- 					
MemberID | MemberID | string | 成员代码 | 
InstrumentID | InstrumentID | string | 标的代码 | 
ValueMode | ValueMode | string | 取值方式 | 0:百分比;1:绝对值;
MarginSequenceType | MarginSequenceType | string | 保证金数列 | 1:等差数列;2:等比数列;
RiskSequenceType | RiskSequenceType | string | 风险限额数列 | 1:等差数列;2:等比数列;
LongMarginRatio | LongMarginRatio | double | 多头起始保证金率 | 
LongMaintMarginRatio | LongMaintMarginRatio | double | 多头起始维持保证金率 | 
LongMarginStep | LongMarginStep | double | 多头保证金率步长 | 
LongMaintMarginStep | LongMaintMarginStep | double | 多头维持保证金率步长 | 
LongRiskLimit | LongRiskLimit | double | 多头起始风险限额 | 
LongRiskStep | LongRiskStep | double | 多头风险限额步长 | 
ShortMarginRatio | ShortMarginRatio | double | 空头起始保证金率 | 
ShortMaintMarginRatio | ShortMaintMarginRatio | double | 空头起始维持保证金率 | 
ShortMarginStep | ShortMarginStep | double | 空头保证金率步长 | 
ShortMaintMarginStep | ShortMaintMarginStep | double | 空头维持保证金率步长 | 
ShortRiskLimit | ShortRiskLimit | double | 空头起始风险限额 | 
ShortRiskStep | ShortRiskStep | double | 空头风险限额步长 | 
DefaultLeverage | DefaultLeverage | double | 默认杠杆倍数 | 
Remark | Remark | string | 备注 | 
InsertTime | InsertTime | double | 插入时间 | 
UpdateTime | UpdateTime | double | 更新时间 | 
## **成员通知**

+ **Request URL**  `/ws`

+ **Request**

```
{
	"SendTopicAction": {
		"Action": "1",
		"FilterValue": "",
		"LocalNo": 5,
		"ResumeNo": -1,
		"TopicID": "3",
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
	"action": "PushMember",
	"localNo": 232,
	"seq": 123,
	"changeType": "3",
	"result": [			
				{"table":"Member",
				 "data":{
					,MemberID:"1983723"
					,MemberType:"1"
					,Grade:""
					,Priority:0
					,FeeGrade:""
					,MarginRateGrade:""
					,TradingRightGrade:""
					,CFDGrade:""
					,LiquidityGrade:""
					,RiskLevel:"1"
					,PositionType:"1"
					,TriggerOrders:0
					,OpenOrders:0
					,OpenPositions:0
					,MaxLocalID:"1983723"
					,SinkType:"1"
					,CopyMemberID:"1983723"
					,Region:""
					,FeeDeduceDisCount:182582.18
					,FeeDeduceCurrency:"BTC"
					,CanBorrow:0
					,Remark:"my_little_tips"
					,InsertTime:1657543832
					,UpdateTime:1657543832
					}
				}
	]
}
```

+ **Response Parameters**

Member:

parameter | shortName | type | description | remark
-------- | -------- | -------- | -------- | -------- 					
MemberID | MemberID | string | 成员代码 | 
MemberType | MemberType | string | 成员类型 | a:普通;b:内部做市商;c:系统手续费使用;d:风控使用;e:运营使用;f:运营保留资金使用;g:外部做市商;
Grade | Grade | string | 等级 | 
Priority | Priority | int | 优先权 | 
FeeGrade | FeeGrade | string | 手续费等级 | 
MarginRateGrade | MarginRateGrade | string | 保证金等级 | 
TradingRightGrade | TradingRightGrade | string | 权限等级 | 
CFDGrade | CFDGrade | string | 差价等级 | 
LiquidityGrade | LiquidityGrade | string | 流动性等级 | 
RiskLevel | RiskLevel | string | 风控类型 | 0:本币种资金权益;1:结算组所有资金权益;2:资金账号所有资金权益;3:会员账号所有资金权益;
PositionType | PositionType | string | 持仓类型 | 0:不设置;1:综合持仓(双边持仓);2:净持仓(单边持仓);
TriggerOrders | TriggerOrders | int | 触发订单个数 | 
OpenOrders | OpenOrders | int | 未成交订单数 | 
OpenPositions | OpenPositions | int | 用户持仓个数 | 
MaxLocalID | MaxLocalID | string | 最大本地标识 | 
SinkType | SinkType | string | 下游系统同步类型 | 0:同步到数据库;1:同步到Redis不到数据库;2:不同步;
CopyMemberID | CopyMemberID | string | 带单员代码 | 
Region | Region | string |  | 
FeeDeduceDisCount | FeeDeduceDisCount | double | 作为手续费的抵扣优惠比例 | 
FeeDeduceCurrency | FeeDeduceCurrency | string | 作为手续费的抵扣比例 | 
CanBorrow | CanBorrow | int | 是否需要借钱交易 | 
Remark | Remark | string | 备注 | 
InsertTime | InsertTime | double | 插入时间 | 
UpdateTime | UpdateTime | double | 更新时间 | 
## **触发单通知**

+ **Request URL**  `/ws`

+ **Request**

```
{
	"SendTopicAction": {
		"Action": "1",
		"FilterValue": "",
		"LocalNo": 5,
		"ResumeNo": -1,
		"TopicID": "3",
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
	"action": "PushTriggerOrder",
	"localNo": 232,
	"seq": 123,
	"changeType": "3",
	"result": [			
				{"table":"TriggerOrder",
				 "data":{
					,MemberID:"1983723"
					,TradeUnitID:"1983723"
					,AccountID:"1983723"
					,LocalID:"1983723"
					,ExchangeID:"CEX"
					,InstrumentID:"BTCUSD"
					,OrderPriceType:"1"
					,Direction:"1"
					,OffsetFlag:"1"
					,Price:182582.18
					,Volume:182582.18
					,VolumeDisplay:182582.18
					,VolumeMode:"1"
					,Cost:182582.18
					,OrderType:"1"
					,GTDTime:1657543832
					,MinVolume:182582.18
					,BusinessType:"1"
					,BusinessValue:""
					,CloseOrderID:"1983723"
					,IsCrossMargin:1
					,Remark:"my_little_tips"
					,OrderID:"12376422300333"
					,CopyMemberID:"1983723"
					,CopyOrderID:"1983723"
					,Leverage:125
					,CopyProfitRate:182582.18
					,APPID:"ios"
					,PositionID:"1983723"
					,TriggerPrice:182582.18
					,Reserve:182582.18
					,SLPrice:182582.18
					,SLTriggerPrice:182582.18
					,TPPrice:182582.18
					,TPTriggerPrice:182582.18
					,RiskBefore:0
					,TriggerOrderType:"1"
					,TriggerDetail:""
					,TriggerPriceType:"1"
					,TriggerValue:""
					,CloseSLPrice:182582.18
					,CloseSLTriggerPrice:182582.18
					,CloseTPPrice:182582.18
					,CloseTPTriggerPrice:182582.18
					,CloseOrderPriceType:"1"
					,ClosePrice:182582.18
					,CloseTriggerPrice:182582.18
					,RelatedOrderID:"1983723"
					,ActiveTime:1657543832
					,TriggerTime:1657543832
					,TimeSortNo:1657543832
					,TriggerStatus:"1"
					,PosiDirection:"1"
					,FrontNo:1
					,ErrorNo:1033201232
					,ErrorMsg:""
					,InsertTime:1657543832
					,UpdateTime:1657543832
					,BusinessNo:11176422300333
					,CreateTime:""
					}
				}
	]
}
```

+ **Response Parameters**

TriggerOrder:

parameter | shortName | type | description | remark
-------- | -------- | -------- | -------- | -------- 					
MemberID | M | string | 成员代码 | 
TradeUnitID | T | string | 交易单元代码 | 
AccountID | A | string | 资金账号 | 
LocalID | L | string | 报单本地标识 | 
ExchangeID | E | string | 交易所代码 | 
InstrumentID | I | string | 标的代码 | 
OrderPriceType | PT | string | 报单价格类型 | 0:限价(手动指定Price值);1:任意价(Price按最高最低限价取值);2:最优价/对手价(Price按对方1档价取值);3:五档价(Price按对方5档价取值);4:十档价(类同上);7:最优Marker价(Price按本方1档价或其+1Tick取值);8:Price按委托额Cost和订单簿情况确定值;9:保底点差价:成交价格Price不会劣于系统设置差价成交;
Direction | D | string | 买卖方向 | 0:买;1:卖;
OffsetFlag | O | string | 开平标志 | 0:开仓;1:平仓;2:强平;3:平今;4:平昨;5:全平;6:指定订单平仓;7:指定成交平仓;8:最大平仓;
Price | P | double | 报单价格 | 
Volume | V | double | 数量 | 
VolumeDisplay | D | double | 显示数量 | 
VolumeMode | VM | string | 数量取值方式 | 0:百分比;1:绝对值;
Cost | C | double | 委托额 | 
OrderType | OT | string | 订单类型 | 0:普通订单;1:(FillAndKill/IOC)立即完成并且剩余撤销;2:(FillOrKill)立即完成全部或者最小订单要求否则全部撤销;3:需要进入订单簿被动成交,否则全部撤销(只做Maker/PostOnly);4:不需要撮合直接成交;
GTDTime | GT | double | GTD时间 | 
MinVolume | MV | double | 最小成交量 | 
BusinessType | BT | string | 业务类别 | 0:普通;1:申赎;2:行权;3:中立仓申报;4:递延交割申报;5:互换定单;6:质押;7:合并分拆;8:转股;9:回售回购;a:投票;b:融资融券;c:期权执行;d:传递被平仓订单BusinessValue;P:自定义11;Q:自定义10;R:自定义9;S:连续交易;T:自定义7;U:画线委托;V:限价市价;W:条件;X:止盈止损;Y:追踪出场;Z:指标类型;
BusinessValue | BV | string | 业务值 | 
CloseOrderID | CI | string | 平仓指定开仓的订单号 | 
IsCrossMargin | IsCrossMargin | int | 是否全仓 | 
Remark | R | string | 备注 | 
OrderID | OrderID | string | 报单系统唯一代码 | 
CopyMemberID | CopyMemberID | string | 带单员代码 | 
CopyOrderID | CopyOrderID | string | 带单员报单号 | 
Leverage | Leverage | double | 委托单杠杆倍数 | 
CopyProfitRate | CopyProfitRate | double | 带单分配比例 | 
APPID | APPID | string | 应用编号 | 
PositionID | B | string | 持仓代码 | 
TriggerPrice | TriggerPrice | double | 触发价 | 
Reserve | Reserve | double | 保留资金 | 
SLPrice | SLPrice | double | 止损价 | 
SLTriggerPrice | SLTriggerPrice | double | 止损触发价 | 
TPPrice | TPPrice | double | 止盈价 | 
TPTriggerPrice | TPTriggerPrice | double | 止盈触发价 | 
RiskBefore | RiskBefore | int | 是否提前检查风控 | 
TriggerOrderType | TriggerOrderType | string | 触发的订单类型 | 0:未定义;1:持仓止盈止损;2:订单止盈止损;3:条件单;4:追踪(跟踪)订单;5:其他算法交易;6:指标类型;7:指标类型止盈止损;8:跟单类型;9:划线下单类型;a:其他类型1;b:其他类型2;c:其他类型3;d:其他类型4;e:其他类型5;
TriggerDetail | TriggerDetail | string | 触发类型明细 | 
TriggerPriceType | TriggerPriceType | string | 触发价类型 | 0:最新价;1:标记价;2:标的指数价;
TriggerValue | TriggerValue | string | 触发单具体设置信息 | 
CloseSLPrice | CloseSLPrice | double | 平仓止损价 | 
CloseSLTriggerPrice | CloseSLTriggerPrice | double | 平仓止损触发价 | 
CloseTPPrice | CloseTPPrice | double | 平仓止盈价 | 
CloseTPTriggerPrice | CloseTPTriggerPrice | double | 平仓止盈触发价 | 
CloseOrderPriceType | CloseOrderPriceType | string | 报单价格类型 | 0:限价(手动指定Price值);1:任意价(Price按最高最低限价取值);2:最优价/对手价(Price按对方1档价取值);3:五档价(Price按对方5档价取值);4:十档价(类同上);7:最优Marker价(Price按本方1档价或其+1Tick取值);8:Price按委托额Cost和订单簿情况确定值;9:保底点差价:成交价格Price不会劣于系统设置差价成交;
ClosePrice | ClosePrice | double | 平仓价 | 
CloseTriggerPrice | CloseTriggerPrice | double | 平仓触发价 | 
RelatedOrderID | RelatedOrderID | string | 关联报单号 | 
ActiveTime | ActiveTime | double | 激活时间 | 
TriggerTime | TriggerTime | double | 触发时间 | 
TimeSortNo | TimeSortNo | double | 按时间排队的序号 | 
TriggerStatus | TriggerStatus | string | 触发报单状态 | 0:未设置状态;1:活跃(运行)状态;2:已经触发;3:触发失败;4:撤单;
PosiDirection | PosiDirection | string | 持仓多空方向 | 0:多头;1:空头;2:净;
FrontNo | FrontNo | int | 前置编号 | 
ErrorNo | ErrorNo | int | 错误代码 | 
ErrorMsg | ErrorMsg | string | 错误信息 | 
InsertTime | InsertTime | double | 插入时间 | 
UpdateTime | UpdateTime | double | 更新时间 | 
BusinessNo | BusinessNo | double | 业务序列号 | 
CreateTime | CreateTime | string | 创建时间 | 
## **合约通知**

+ **Request URL**  `/ws`

+ **Request**

```
{
	"SendTopicAction": {
		"Action": "1",
		"FilterValue": "",
		"LocalNo": 5,
		"ResumeNo": -1,
		"TopicID": "6",
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
	"action": "PushInstrument",
	"localNo": 232,
	"seq": 123,
	"changeType": "3",
	"result": [			
				{"table":"Instrument",
				 "data":{
					,ExchangeID:"CEX"
					,InstrumentID:"BTCUSD"
					,InstrumentName:""
					,SettlementGroup:"999"
					,ProductGroup:"Swap"
					,UnderlyingID:"1983723"
					,ProductClass:"1"
					,ProductType:"1"
					,StrikePrice:182582.18
					,OptionsType:"1"
					,VolumeMultiple:182582.18
					,StartTime:1657543832
					,ExpireTime:1657543832
					,PriceCurrency:"BTC"
					,ClearCurrency:"BTC"
					,IsInverse:1
					,BaseCurrency:"BTC"
					,MarginPriceType:"1"
					,BasisPrice:182582.18
					,MinOrderVolume:182582.18
					,MinOrderCost:182582.18
					,MaxOrderVolume:182582.18
					,PriceTick:182582.18
					,VolumeTick:182582.18
					,ShowVolumeTick:182582.18
					,TradingModel:"1"
					,DayStartTime:""
					,SortNumber:-1
					,Remark:"my_little_tips"
					,DefaultLeverage:125
					,PriceLimitValueMode:"1"
					,PriceLimitPrice:"1"
					,PriceLimitUpperValue:182582.18
					,PriceLimitLowerValue:182582.18
					,MarginRateGroup:"999"
					,CFDGroup:"999"
					,FeeGroup:"999"
					,TradingRightGroup:"999"
					,MaxOpenInterest:182582.18
					,FundingRateGroup:"999"
					,InsertTime:1657543832
					,UpdateTime:1657543832
					,MarkedPriceTimes:0
					}
				}
	]
}
```

+ **Response Parameters**

Instrument:

parameter | shortName | type | description | remark
-------- | -------- | -------- | -------- | -------- 					
ExchangeID | X | string | 交易所代码 | 
InstrumentID | I | string | 标的代码 | 
InstrumentName | N | string | 标的名称 | 
SettlementGroup |  | string | 结算组编号 | 
ProductGroup | G | string | 产品组 | 
UnderlyingID |  | string | 基础标的代码 | 
ProductClass |  | string | 产品大类 | 1:保证金交易;2:权利金交易;3:资产转让（股票/基金/国债等）;4:兑换交易（资金之间的互换）;5:组合;6:行情;
ProductType |  | string | 产品类型 | 1:交割期货;2:永续、利率互换期货;3:即期/日交割期货;4:递延交割期货;5:权证;6:美式期权;7:欧式期权;8:币币交易;9:外汇兑换;a:股票;b:债券;c:基金;
StrikePrice |  | double | 执行价 | 
OptionsType |  | string | 期权类型 | 0:非期权;1:看涨;2:看跌;
VolumeMultiple | VM | double | 数量乘数 | 
StartTime |  | double | 启动时间 | 
ExpireTime |  | double | 到期时间 | 
PriceCurrency | PC | string | 计价币种 | 
ClearCurrency | CC | string | 清算币种 | 
IsInverse | II | int | 是否反向标的 | 
BaseCurrency | BB | string | 标的基础货币 | 
MarginPriceType |  | string | 保证金价格类型 | 0:不按比例按固定值,无需价格;1:昨结算价;2:开仓价;3:标记价;4:最新价;
BasisPrice |  | double | 挂牌基准价 | 
MinOrderVolume | MOV | double | 最小下单量 | 
MinOrderCost | MOC | double | 最小下单额 | 
MaxOrderVolume |  | double | 最大下单量 | 
PriceTick | PT | double | 最小变动价位 | 
VolumeTick | VT | double | 最小变动量 | 
ShowVolumeTick |  | double | 前端行情显示最小变动量(服务端不用) | 
TradingModel |  | string | 交易模式 | 0:价格优先时间优先;1:价格优先按比例分配;2:只能和做市商报价成交;3:没有撮合;
DayStartTime |  | string | 每日开始交易时间 | 
SortNumber |  | int | 排序字段 | 
Remark | RE | string | 备注 | 
DefaultLeverage | DL | double | 默认杠杆倍数 | 
PriceLimitValueMode |  | string | 取值方式 | 0:百分比;1:绝对值;
PriceLimitPrice |  | string | 限价价格类型 | 1:昨结算价;2:最新价;3:基础标的价;4:标记价;
PriceLimitUpperValue |  | double | 上限 | 
PriceLimitLowerValue |  | double | 下限 | 
MarginRateGroup |  | string | 保证金组 | 
CFDGroup |  | string | 差价组 | 
FeeGroup |  | string | 手续费组 | 
TradingRightGroup |  | string | 权限组 | 
MaxOpenInterest |  | double | 最大持仓量 | 
FundingRateGroup |  | string | 资金费用组 | 
InsertTime |  | double | 插入时间 | 
UpdateTime |  | double | 更新时间 | 
MarkedPriceTimes |  | int | 标记价格计算次数(0:最新价;1-n:次数;空:不生成标记价) | 
## **用户登陆通知**

+ **Request URL**  `/ws`

+ **Request**

```
{
	"SendTopicAction": {
		"Action": "1",
		"FilterValue": "",
		"LocalNo": 5,
		"ResumeNo": -1,
		"TopicID": "3",
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
	"action": "PushUserSession",
	"localNo": 232,
	"seq": 123,
	"changeType": "3",
	"result": [			
				{"table":"UserSession",
				 "data":{
					,UserID:"1983723"
					,Password:""
					,UserProductID:"1983723"
					,MacAddress:""
					,ClientIPAddress:""
					,HDSerialID:"1983723"
					,AuthCode:""
					,APPID:"ios"
					,Token:""
					,Remark:"my_little_tips"
					,APIID:"1983723"
					,LoginTime:1657543832
					,ExpireTime:1657543832
					,IPAddress:""
					,MaxLocalID:"1983723"
					,SessionNo:1033201232
					,FrontNo:1
					,AccessLimit:0
					,UserType:"1"
					,MemberID:"1983723"
					,LimitAccesses:0
					,SinkType:"1"
					,InsertTime:1657543832
					,UpdateTime:1657543832
					}
				}
	]
}
```

+ **Response Parameters**

UserSession:

parameter | shortName | type | description | remark
-------- | -------- | -------- | -------- | -------- 					
UserID | UserID | string | 交易用户代码 | 
Password | Password | string | 密码 | 
UserProductID | UserProductID | string | 用户端产品信息 | 
MacAddress | MacAddress | string | Mac地址 | 
ClientIPAddress | ClientIPAddress | string | 终端IP地址 | 
HDSerialID | HDSerialID | string | 硬盘序列号 | 
AuthCode | AuthCode | string | 授权编码 | 
APPID | APPID | string | 应用编号 | 
Token | Token | string | 令牌 | 
Remark | Remark | string | 备注 | 
APIID | APIID | string | 接口端产品信息 | 
LoginTime | LoginTime | double | 登录时间 | 
ExpireTime | ExpireTime | double | 失效时间 | 
IPAddress | IPAddress | string | 允许登录的IP地址 | 
MaxLocalID | MaxLocalID | string | 最大本地标识 | 
SessionNo | SessionNo | int | 会话编号 | 
FrontNo | FrontNo | int | 前置编号 | 
AccessLimit | AccessLimit | int | 每秒访问的次数限制 | 
UserType | UserType | string | 用户类型 | 1:普通用户(订阅自己/可下自己订单);2:观察用户OB(订阅其他用户/不可下单);5:超级管理员(订阅所有/可给所有用户下单/可爆仓);
MemberID | MemberID | string | 成员代码 | 
LimitAccesses | LimitAccesses | int | 该秒已经访问的次数 | 
SinkType | SinkType | string | 下游系统同步类型 | 0:同步到数据库;1:同步到Redis不到数据库;2:不同步;
InsertTime | InsertTime | double | 插入时间 | 
UpdateTime | UpdateTime | double | 更新时间 | 
## **币种通知**

+ **Request URL**  `/ws`

+ **Request**

```
{
	"SendTopicAction": {
		"Action": "1",
		"FilterValue": "",
		"LocalNo": 5,
		"ResumeNo": -1,
		"TopicID": "3",
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
	"action": "PushCurrency",
	"localNo": 232,
	"seq": 123,
	"changeType": "3",
	"result": [			
				{"table":"Currency",
				 "data":{
					,SettlementGroup:"999"
					,Currency:"BTC"
					,Deposit:182582.18
					,Withdraw:182582.18
					,Balance:182582.18
					,PreBalance:182582.18
					,SortNumber:-1
					,CanReduce:0
					,IsTrading:1
					,SettleSegment:""
					,MoneyTick:182582.18
					,ExchangeID:"CEX"
					,InstrumentID:"BTCUSD"
					,LoanUse:182582.18
					,LoanLimit:182582.18
					,LoanBorrow:182582.18
					,LoanDeposit:182582.18
					,LoanBase:182582.18
					,LoanRate:182582.18
					,LoanHighBase:182582.18
					,LoanHighRate:182582.18
					,Remark:"my_little_tips"
					,CurrValue:182582.18
					,HairCut:182582.18
					,MaxValue:182582.18
					,MarginRateGroup:"999"
					,PrivateEnd:182582.18
					,InsertTime:1657543832
					,UpdateTime:1657543832
					}
				}
	]
}
```

+ **Response Parameters**

Currency:

parameter | shortName | type | description | remark
-------- | -------- | -------- | -------- | -------- 					
SettlementGroup | S | string | 结算组编号 | 
Currency | C | string | 币种 | 
Deposit |  | double | 入金金额 | 
Withdraw |  | double | 出金金额 | 
Balance | B | double | 静态权益 | 
PreBalance |  | double | 上次静态权益 | 
SortNumber |  | int | 排序字段 | 
CanReduce |  | int | 是否可以出金 | 
IsTrading |  | int | 是否允许相关交易 | 
SettleSegment |  | string | 结算段 | 
MoneyTick |  | double | 最小单位 | 
ExchangeID |  | string | 价格参考交易所代码 | 
InstrumentID |  | string | 价格参考标的代码 | 
LoanUse | LU | double | 借款使用量 | 
LoanLimit |  | double | 最大可借比例 | 
LoanBorrow | LB | double | 借款利息 | 
LoanDeposit | LD | double | 存款利息 | 
LoanBase |  | double | 利息起点 | 
LoanRate |  | double | 利息斜率 | 
LoanHighBase |  | double | 高利息起点 | 
LoanHighRate |  | double | 高利息斜率 | 
Remark | RE | string | 备注 | 
CurrValue | V | double | 当前价值 | 
HairCut |  | double | 抵押比例 | 
MaxValue |  | double | 最大抵押数额 | 
MarginRateGroup |  | string | 保证金组 | 
PrivateEnd |  | double | 私有化停止交易线 | 
InsertTime |  | double | 插入时间 | 
UpdateTime |  | double | 更新时间 | 

# **内存数据库操作接口**

## **业务配置**
+ **Request URL**  

```
 插入操作请求:  POST /action/v1.0/SendInsert/ServiceConfig
 更新操作请求:  POST /action/v1.0/SendUpdate/ServiceConfig
 删除操作请求:  POST /action/v1.0/SendDelete/ServiceConfig
 插入没有就更新请求:  POST /action/v1.0/SendInsUpd/ServiceConfig
 查询唯一索引记录请求:  POST /action/v1.0/SendQryOne/ServiceConfig
 设置字段值清空请求:  POST /action/v1.0/SendClear/ServiceConfig
 初始化操作请求:  POST /action/v1.0/SendInit/ServiceConfig
```

+ **Request**

```
{
	ConfigName:""	
	,Index1:""	
	,Index2:""	
	,ConfigValue:""	
	,Remark:"my_little_tips"	
	,InsertTime:1657543832	
	,UpdateTime:1657543832	
}
```

+ **Response**

```
{
	"action": "Recv*",
	"errorCode": 0,
	"errorMsg": "Success",
	"result": [
		{"table":"ServiceConfig",
		"data":{	
			ConfigName:""
			,Index1:""
			,Index2:""
			,ConfigValue:""
			,Remark:"my_little_tips"
			,InsertTime:1657543832
			,UpdateTime:1657543832
		}
	}
}
```	

+ **Request & Response Parameters**

parameter | shortName | required | type | description | remark
-------- | -------- | -------- | -------- | -------- | --------
ConfigName | ConfigName | yes | string | 配置名称 | 
Index1 | Index1 | yes | string | 索引1 | 
Index2 | Index2 | yes | string | 索引2 | 
ConfigValue | ConfigValue | no | string | 配置值 | 
Remark | Remark | no | string | 备注 | 
InsertTime | InsertTime | no | double | 插入时间 | 
UpdateTime | UpdateTime | no | double | 更新时间 | 
## **币种信息**
+ **Request URL**  

```
 插入操作请求:  POST /action/v1.0/SendInsert/Currency
 更新操作请求:  POST /action/v1.0/SendUpdate/Currency
 删除操作请求:  POST /action/v1.0/SendDelete/Currency
 插入没有就更新请求:  POST /action/v1.0/SendInsUpd/Currency
 查询唯一索引记录请求:  POST /action/v1.0/SendQryOne/Currency
 设置字段值清空请求:  POST /action/v1.0/SendClear/Currency
 初始化操作请求:  POST /action/v1.0/SendInit/Currency
```

+ **Request**

```
{
	SettlementGroup:"999"	
	,Currency:"BTC"	
	,Deposit:182582.18	
	,Withdraw:182582.18	
	,Balance:182582.18	
	,PreBalance:182582.18	
	,SortNumber:-1	
	,CanReduce:0	
	,IsTrading:1	
	,SettleSegment:""	
	,MoneyTick:182582.18	
	,ExchangeID:"CEX"	
	,InstrumentID:"BTCUSD"	
	,LoanUse:182582.18	
	,LoanLimit:182582.18	
	,LoanBorrow:182582.18	
	,LoanDeposit:182582.18	
	,LoanBase:182582.18	
	,LoanRate:182582.18	
	,LoanHighBase:182582.18	
	,LoanHighRate:182582.18	
	,Remark:"my_little_tips"	
	,CurrValue:182582.18	
	,HairCut:182582.18	
	,MaxValue:182582.18	
	,MarginRateGroup:"999"	
	,PrivateEnd:182582.18	
	,InsertTime:1657543832	
	,UpdateTime:1657543832	
}
```

+ **Response**

```
{
	"action": "Recv*",
	"errorCode": 0,
	"errorMsg": "Success",
	"result": [
		{"table":"Currency",
		"data":{	
			SettlementGroup:"999"
			,Currency:"BTC"
			,Deposit:182582.18
			,Withdraw:182582.18
			,Balance:182582.18
			,PreBalance:182582.18
			,SortNumber:-1
			,CanReduce:0
			,IsTrading:1
			,SettleSegment:""
			,MoneyTick:182582.18
			,ExchangeID:"CEX"
			,InstrumentID:"BTCUSD"
			,LoanUse:182582.18
			,LoanLimit:182582.18
			,LoanBorrow:182582.18
			,LoanDeposit:182582.18
			,LoanBase:182582.18
			,LoanRate:182582.18
			,LoanHighBase:182582.18
			,LoanHighRate:182582.18
			,Remark:"my_little_tips"
			,CurrValue:182582.18
			,HairCut:182582.18
			,MaxValue:182582.18
			,MarginRateGroup:"999"
			,PrivateEnd:182582.18
			,InsertTime:1657543832
			,UpdateTime:1657543832
		}
	}
}
```	

+ **Request & Response Parameters**

parameter | shortName | required | type | description | remark
-------- | -------- | -------- | -------- | -------- | --------
SettlementGroup | S | yes | string | 结算组编号 | 
Currency | C | yes | string | 币种 | 
Deposit |  | no | double | 入金金额 | 
Withdraw |  | no | double | 出金金额 | 
Balance | B | no | double | 静态权益 | 
PreBalance |  | no | double | 上次静态权益 | 
SortNumber |  | no | int | 排序字段 | 
CanReduce |  | no | int | 是否可以出金 | 
IsTrading |  | no | int | 是否允许相关交易 | 
SettleSegment |  | no | string | 结算段 | 
MoneyTick |  | no | double | 最小单位 | 
ExchangeID |  | no | string | 价格参考交易所代码 | 
InstrumentID |  | no | string | 价格参考标的代码 | 
LoanUse | LU | no | double | 借款使用量 | 
LoanLimit |  | no | double | 最大可借比例 | 
LoanBorrow | LB | no | double | 借款利息 | 
LoanDeposit | LD | no | double | 存款利息 | 
LoanBase |  | no | double | 利息起点 | 
LoanRate |  | no | double | 利息斜率 | 
LoanHighBase |  | no | double | 高利息起点 | 
LoanHighRate |  | no | double | 高利息斜率 | 
Remark | RE | no | string | 备注 | 
CurrValue | V | no | double | 当前价值 | 
HairCut |  | no | double | 抵押比例 | 
MaxValue |  | no | double | 最大抵押数额 | 
MarginRateGroup |  | no | string | 保证金组 | 
PrivateEnd |  | no | double | 私有化停止交易线 | 
InsertTime |  | no | double | 插入时间 | 
UpdateTime |  | no | double | 更新时间 | 
## **市场行情**
+ **Request URL**  

```
 插入操作请求:  POST /action/v1.0/SendInsert/MarketData
 更新操作请求:  POST /action/v1.0/SendUpdate/MarketData
 删除操作请求:  POST /action/v1.0/SendDelete/MarketData
 插入没有就更新请求:  POST /action/v1.0/SendInsUpd/MarketData
 查询唯一索引记录请求:  POST /action/v1.0/SendQryOne/MarketData
 设置字段值清空请求:  POST /action/v1.0/SendClear/MarketData
 初始化操作请求:  POST /action/v1.0/SendInit/MarketData
```

+ **Request**

```
{
	ExchangeID:"CEX"	
	,InstrumentID:"BTCUSD"	
	,ProductGroup:"Swap"	
	,UpdateTime:1657543832	
	,PreClosePrice:182582.18	
	,PreSettlementPrice:182582.18	
	,PreOpenInterest:182582.18	
	,PrePositionFeeRate:182582.18	
	,ClosePrice:182582.18	
	,PositionFeeTime:1657543832	
	,SettlementTime:1657543832	
	,UpperLimitPrice:182582.18	
	,LowerLimitPrice:182582.18	
	,UnderlyingPrice:182582.18	
	,MarkedPrice:182582.18	
	,PositionFeeRate:182582.18	
	,HighestPrice:182582.18	
	,LowestPrice:182582.18	
	,LastPrice:182582.18	
	,Volume:182582.18	
	,Turnover:182582.18	
	,OpenInterest:182582.18	
	,SettlementPrice:182582.18	
	,OpenPrice:182582.18	
	,IsPubMarketData:1	
	,InstrumentStatus:"1"	
	,Remark:"my_little_tips"	
	,BusinessNo:11176422300333	
	,MarkedPriceTotal:182582.18	
	,MarkedPriceWeight:182582.18	
	,L25UpperPrice:182582.18	
	,L25LowerPrice:182582.18	
	,L25UpperPrice1:182582.18	
	,L25LowerPrice1:182582.18	
	,L25UpperPrice2:182582.18	
	,L25LowerPrice2:182582.18	
	,BidPrice1:182582.18	
	,BidVolume1:182582.18	
	,AskPrice1:182582.18	
	,AskVolume1:182582.18	
	,TheoryPrice:182582.18	
	,HighestPrice24:182582.18	
	,LowestPrice24:182582.18	
	,Volume24:182582.18	
	,Turnover24:182582.18	
	,OpenPrice24:182582.18	
	,Frequency:0	
	,InsertTime:1657543832	
}
```

+ **Response**

```
{
	"action": "Recv*",
	"errorCode": 0,
	"errorMsg": "Success",
	"result": [
		{"table":"MarketData",
		"data":{	
			ExchangeID:"CEX"
			,InstrumentID:"BTCUSD"
			,ProductGroup:"Swap"
			,UpdateTime:1657543832
			,PreClosePrice:182582.18
			,PreSettlementPrice:182582.18
			,PreOpenInterest:182582.18
			,PrePositionFeeRate:182582.18
			,ClosePrice:182582.18
			,PositionFeeTime:1657543832
			,SettlementTime:1657543832
			,UpperLimitPrice:182582.18
			,LowerLimitPrice:182582.18
			,UnderlyingPrice:182582.18
			,MarkedPrice:182582.18
			,PositionFeeRate:182582.18
			,HighestPrice:182582.18
			,LowestPrice:182582.18
			,LastPrice:182582.18
			,Volume:182582.18
			,Turnover:182582.18
			,OpenInterest:182582.18
			,SettlementPrice:182582.18
			,OpenPrice:182582.18
			,IsPubMarketData:1
			,InstrumentStatus:"1"
			,Remark:"my_little_tips"
			,BusinessNo:11176422300333
			,MarkedPriceTotal:182582.18
			,MarkedPriceWeight:182582.18
			,L25UpperPrice:182582.18
			,L25LowerPrice:182582.18
			,L25UpperPrice1:182582.18
			,L25LowerPrice1:182582.18
			,L25UpperPrice2:182582.18
			,L25LowerPrice2:182582.18
			,BidPrice1:182582.18
			,BidVolume1:182582.18
			,AskPrice1:182582.18
			,AskVolume1:182582.18
			,TheoryPrice:182582.18
			,HighestPrice24:182582.18
			,LowestPrice24:182582.18
			,Volume24:182582.18
			,Turnover24:182582.18
			,OpenPrice24:182582.18
			,Frequency:0
			,InsertTime:1657543832
		}
	}
}
```	

+ **Request & Response Parameters**

parameter | shortName | required | type | description | remark
-------- | -------- | -------- | -------- | -------- | --------
ExchangeID | X | yes | string | 交易所代码 | 
InstrumentID | I | yes | string | 标的代码 | 
ProductGroup | G | no | string | 产品组 | 
UpdateTime | U | no | double | 更新时间 | 
PreClosePrice |  | no | double | 上次收盘 | 
PreSettlementPrice |  | no | double | 上次结算价 | 
PreOpenInterest |  | no | double | 上次持仓量 | 
PrePositionFeeRate |  | no | double | 上次持仓费按比例 | 
ClosePrice | ClosePrice | no | double | 收盘价 | 
PositionFeeTime | PF | no | double | 收取持仓费用时间 | 
SettlementTime | SettlementTime | no | double | 结算时间 | 
UpperLimitPrice | C | no | double | 最高限价 | 
LowerLimitPrice | F | no | double | 最低限价 | 
UnderlyingPrice | D | no | double | 基础标的价格 | 
MarkedPrice | M | no | double | 标记价格 | 
PositionFeeRate | R | no | double | 预计持仓费按比例 | 
HighestPrice | H | no | double | 当日最高价 | 
LowestPrice | L | no | double | 当日最低价 | 
LastPrice | N | no | double | 最新价 | 
Volume | V | no | double | 当日数量 | 
Turnover | T | no | double | 当日成交金额 | 
OpenInterest | P | no | double | 持仓量 | 
SettlementPrice | S | no | double | 预估结算价 | 
OpenPrice | O | no | double | 当日开盘价 | 
IsPubMarketData |  | no | int | 行情发布状态 | 
InstrumentStatus | B | no | string | 标的交易状态 | 0:开盘前;1:非交易;2:连续交易;3:集合竞价报单;4:集合竞价价格平衡;5:集合竞价撮合;6:收盘;7:不活跃;
Remark | RE | no | string | 备注 | 
BusinessNo |  | no | double | 业务序列号 | 
MarkedPriceTotal |  | no | double | 标记价格汇总 | 
MarkedPriceWeight |  | no | double | 标记价格权重 | 
L25UpperPrice |  | no | double | 25档行情最高价 | 
L25LowerPrice |  | no | double | 25档行情最低价 | 
L25UpperPrice1 |  | no | double | 25档行情最高价1 | 
L25LowerPrice1 |  | no | double | 25档行情最低价1 | 
L25UpperPrice2 |  | no | double | 25档行情最高价2 | 
L25LowerPrice2 |  | no | double | 25档行情最低价2 | 
BidPrice1 | BP1 | no | double | 申买价一 | 
BidVolume1 | BV1 | no | double | 申买量一 | 
AskPrice1 | AP1 | no | double | 申卖价一 | 
AskVolume1 | AV1 | no | double | 申卖量一 | 
TheoryPrice | TP | no | double | 理论价 | 
HighestPrice24 | H2 | no | double | 24小时最高价 | 
LowestPrice24 | L2 | no | double | 24小时最低价 | 
Volume24 | V2 | no | double | 24小时数量 | 
Turnover24 | T2 | no | double | 24小时成交金额 | 
OpenPrice24 | O2 | no | double | 24小时开盘价 | 
Frequency |  | no | int | 每秒更新的次数 | 
InsertTime |  | no | double | 插入时间 | 
## **交易标的**
+ **Request URL**  

```
 插入操作请求:  POST /action/v1.0/SendInsert/Instrument
 更新操作请求:  POST /action/v1.0/SendUpdate/Instrument
 删除操作请求:  POST /action/v1.0/SendDelete/Instrument
 插入没有就更新请求:  POST /action/v1.0/SendInsUpd/Instrument
 查询唯一索引记录请求:  POST /action/v1.0/SendQryOne/Instrument
 设置字段值清空请求:  POST /action/v1.0/SendClear/Instrument
 初始化操作请求:  POST /action/v1.0/SendInit/Instrument
```

+ **Request**

```
{
	ExchangeID:"CEX"	
	,InstrumentID:"BTCUSD"	
	,InstrumentName:""	
	,SettlementGroup:"999"	
	,ProductGroup:"Swap"	
	,UnderlyingID:"1983723"	
	,ProductClass:"1"	
	,ProductType:"1"	
	,StrikePrice:182582.18	
	,OptionsType:"1"	
	,VolumeMultiple:182582.18	
	,StartTime:1657543832	
	,ExpireTime:1657543832	
	,PriceCurrency:"BTC"	
	,ClearCurrency:"BTC"	
	,IsInverse:1	
	,BaseCurrency:"BTC"	
	,MarginPriceType:"1"	
	,BasisPrice:182582.18	
	,MinOrderVolume:182582.18	
	,MinOrderCost:182582.18	
	,MaxOrderVolume:182582.18	
	,PriceTick:182582.18	
	,VolumeTick:182582.18	
	,ShowVolumeTick:182582.18	
	,TradingModel:"1"	
	,DayStartTime:""	
	,SortNumber:-1	
	,Remark:"my_little_tips"	
	,DefaultLeverage:125	
	,PriceLimitValueMode:"1"	
	,PriceLimitPrice:"1"	
	,PriceLimitUpperValue:182582.18	
	,PriceLimitLowerValue:182582.18	
	,MarginRateGroup:"999"	
	,CFDGroup:"999"	
	,FeeGroup:"999"	
	,TradingRightGroup:"999"	
	,MaxOpenInterest:182582.18	
	,FundingRateGroup:"999"	
	,InsertTime:1657543832	
	,UpdateTime:1657543832	
	,MarkedPriceTimes:0	
}
```

+ **Response**

```
{
	"action": "Recv*",
	"errorCode": 0,
	"errorMsg": "Success",
	"result": [
		{"table":"Instrument",
		"data":{	
			ExchangeID:"CEX"
			,InstrumentID:"BTCUSD"
			,InstrumentName:""
			,SettlementGroup:"999"
			,ProductGroup:"Swap"
			,UnderlyingID:"1983723"
			,ProductClass:"1"
			,ProductType:"1"
			,StrikePrice:182582.18
			,OptionsType:"1"
			,VolumeMultiple:182582.18
			,StartTime:1657543832
			,ExpireTime:1657543832
			,PriceCurrency:"BTC"
			,ClearCurrency:"BTC"
			,IsInverse:1
			,BaseCurrency:"BTC"
			,MarginPriceType:"1"
			,BasisPrice:182582.18
			,MinOrderVolume:182582.18
			,MinOrderCost:182582.18
			,MaxOrderVolume:182582.18
			,PriceTick:182582.18
			,VolumeTick:182582.18
			,ShowVolumeTick:182582.18
			,TradingModel:"1"
			,DayStartTime:""
			,SortNumber:-1
			,Remark:"my_little_tips"
			,DefaultLeverage:125
			,PriceLimitValueMode:"1"
			,PriceLimitPrice:"1"
			,PriceLimitUpperValue:182582.18
			,PriceLimitLowerValue:182582.18
			,MarginRateGroup:"999"
			,CFDGroup:"999"
			,FeeGroup:"999"
			,TradingRightGroup:"999"
			,MaxOpenInterest:182582.18
			,FundingRateGroup:"999"
			,InsertTime:1657543832
			,UpdateTime:1657543832
			,MarkedPriceTimes:0
		}
	}
}
```	

+ **Request & Response Parameters**

parameter | shortName | required | type | description | remark
-------- | -------- | -------- | -------- | -------- | --------
ExchangeID | X | yes | string | 交易所代码 | 
InstrumentID | I | yes | string | 标的代码 | 
InstrumentName | N | no | string | 标的名称 | 
SettlementGroup |  | no | string | 结算组编号 | 
ProductGroup | G | no | string | 产品组 | 
UnderlyingID |  | no | string | 基础标的代码 | 
ProductClass |  | no | string | 产品大类 | 1:保证金交易;2:权利金交易;3:资产转让（股票/基金/国债等）;4:兑换交易（资金之间的互换）;5:组合;6:行情;
ProductType |  | no | string | 产品类型 | 1:交割期货;2:永续、利率互换期货;3:即期/日交割期货;4:递延交割期货;5:权证;6:美式期权;7:欧式期权;8:币币交易;9:外汇兑换;a:股票;b:债券;c:基金;
StrikePrice |  | no | double | 执行价 | 
OptionsType |  | no | string | 期权类型 | 0:非期权;1:看涨;2:看跌;
VolumeMultiple | VM | no | double | 数量乘数 | 
StartTime |  | no | double | 启动时间 | 
ExpireTime |  | no | double | 到期时间 | 
PriceCurrency | PC | no | string | 计价币种 | 
ClearCurrency | CC | no | string | 清算币种 | 
IsInverse | II | no | int | 是否反向标的 | 
BaseCurrency | BB | no | string | 标的基础货币 | 
MarginPriceType |  | no | string | 保证金价格类型 | 0:不按比例按固定值,无需价格;1:昨结算价;2:开仓价;3:标记价;4:最新价;
BasisPrice |  | no | double | 挂牌基准价 | 
MinOrderVolume | MOV | no | double | 最小下单量 | 
MinOrderCost | MOC | no | double | 最小下单额 | 
MaxOrderVolume |  | no | double | 最大下单量 | 
PriceTick | PT | no | double | 最小变动价位 | 
VolumeTick | VT | no | double | 最小变动量 | 
ShowVolumeTick |  | no | double | 前端行情显示最小变动量(服务端不用) | 
TradingModel |  | no | string | 交易模式 | 0:价格优先时间优先;1:价格优先按比例分配;2:只能和做市商报价成交;3:没有撮合;
DayStartTime |  | no | string | 每日开始交易时间 | 
SortNumber |  | no | int | 排序字段 | 
Remark | RE | no | string | 备注 | 
DefaultLeverage | DL | no | double | 默认杠杆倍数 | 
PriceLimitValueMode |  | no | string | 取值方式 | 0:百分比;1:绝对值;
PriceLimitPrice |  | no | string | 限价价格类型 | 1:昨结算价;2:最新价;3:基础标的价;4:标记价;
PriceLimitUpperValue |  | no | double | 上限 | 
PriceLimitLowerValue |  | no | double | 下限 | 
MarginRateGroup |  | no | string | 保证金组 | 
CFDGroup |  | no | string | 差价组 | 
FeeGroup |  | no | string | 手续费组 | 
TradingRightGroup |  | no | string | 权限组 | 
MaxOpenInterest |  | no | double | 最大持仓量 | 
FundingRateGroup |  | no | string | 资金费用组 | 
InsertTime |  | no | double | 插入时间 | 
UpdateTime |  | no | double | 更新时间 | 
MarkedPriceTimes |  | no | int | 标记价格计算次数(0:最新价;1-n:次数;空:不生成标记价) | 
## **当前时间**
+ **Request URL**  

```
 插入操作请求:  POST /action/v1.0/SendInsert/CurrentTime
 更新操作请求:  POST /action/v1.0/SendUpdate/CurrentTime
 删除操作请求:  POST /action/v1.0/SendDelete/CurrentTime
 插入没有就更新请求:  POST /action/v1.0/SendInsUpd/CurrentTime
 查询唯一索引记录请求:  POST /action/v1.0/SendQryOne/CurrentTime
 设置字段值清空请求:  POST /action/v1.0/SendClear/CurrentTime
 初始化操作请求:  POST /action/v1.0/SendInit/CurrentTime
```

+ **Request**

```
{
	SystemID:"1983723"	
	,ZoneDate:""	
	,ZoneTime:""	
	,ZoneDays:""	
	,CurrTime:1657543832	
	,InsertTime:1657543832	
	,UpdateTime:1657543832	
	,TimeZone:0	
	,LastTradeID:"1983723"	
	,LastOrderID:"1983723"	
	,LastAccountDetailID:"1983723"	
	,LastTimeSortNo:1657543832	
	,LastBusinessNo:182582.18	
}
```

+ **Response**

```
{
	"action": "Recv*",
	"errorCode": 0,
	"errorMsg": "Success",
	"result": [
		{"table":"CurrentTime",
		"data":{	
			SystemID:"1983723"
			,ZoneDate:""
			,ZoneTime:""
			,ZoneDays:""
			,CurrTime:1657543832
			,InsertTime:1657543832
			,UpdateTime:1657543832
			,TimeZone:0
			,LastTradeID:"1983723"
			,LastOrderID:"1983723"
			,LastAccountDetailID:"1983723"
			,LastTimeSortNo:1657543832
			,LastBusinessNo:182582.18
		}
	}
}
```	

+ **Request & Response Parameters**

parameter | shortName | required | type | description | remark
-------- | -------- | -------- | -------- | -------- | --------
SystemID |  | yes | string | 系统名 | 
ZoneDate | D | no | string | 本地日期 | 
ZoneTime | T | no | string | 本地时间 | 
ZoneDays | Z | no | string | 本地天数 | 
CurrTime | C | no | double | 当前时间（秒） | 
InsertTime | InsertTime | no | double | 插入时间 | 
UpdateTime | U | no | double | 更新时间（毫秒） | 
TimeZone | TimeZone | no | int | 本地时区 | 
LastTradeID | LastTradeID | no | string | 最新最新成交序列号 | 
LastOrderID | LastOrderID | no | string | 最新订单序列号 | 
LastAccountDetailID | LastAccountDetailID | no | string | 最新资金明细序列号 | 
LastTimeSortNo | LastTimeSortNo | no | double | 最新排序序列号 | 
LastBusinessNo | LastBusinessNo | no | double | 最新事务序列号 | 
## **成员**
+ **Request URL**  

```
 插入操作请求:  POST /action/v1.0/SendInsert/Member
 更新操作请求:  POST /action/v1.0/SendUpdate/Member
 删除操作请求:  POST /action/v1.0/SendDelete/Member
 插入没有就更新请求:  POST /action/v1.0/SendInsUpd/Member
 查询唯一索引记录请求:  POST /action/v1.0/SendQryOne/Member
 设置字段值清空请求:  POST /action/v1.0/SendClear/Member
 初始化操作请求:  POST /action/v1.0/SendInit/Member
```

+ **Request**

```
{
	MemberID:"1983723"	
	,MemberType:"1"	
	,Grade:""	
	,Priority:0	
	,FeeGrade:""	
	,MarginRateGrade:""	
	,TradingRightGrade:""	
	,CFDGrade:""	
	,LiquidityGrade:""	
	,RiskLevel:"1"	
	,PositionType:"1"	
	,TriggerOrders:0	
	,OpenOrders:0	
	,OpenPositions:0	
	,MaxLocalID:"1983723"	
	,SinkType:"1"	
	,CopyMemberID:"1983723"	
	,Region:""	
	,FeeDeduceDisCount:182582.18	
	,FeeDeduceCurrency:"BTC"	
	,CanBorrow:0	
	,Remark:"my_little_tips"	
	,InsertTime:1657543832	
	,UpdateTime:1657543832	
}
```

+ **Response**

```
{
	"action": "Recv*",
	"errorCode": 0,
	"errorMsg": "Success",
	"result": [
		{"table":"Member",
		"data":{	
			MemberID:"1983723"
			,MemberType:"1"
			,Grade:""
			,Priority:0
			,FeeGrade:""
			,MarginRateGrade:""
			,TradingRightGrade:""
			,CFDGrade:""
			,LiquidityGrade:""
			,RiskLevel:"1"
			,PositionType:"1"
			,TriggerOrders:0
			,OpenOrders:0
			,OpenPositions:0
			,MaxLocalID:"1983723"
			,SinkType:"1"
			,CopyMemberID:"1983723"
			,Region:""
			,FeeDeduceDisCount:182582.18
			,FeeDeduceCurrency:"BTC"
			,CanBorrow:0
			,Remark:"my_little_tips"
			,InsertTime:1657543832
			,UpdateTime:1657543832
		}
	}
}
```	

+ **Request & Response Parameters**

parameter | shortName | required | type | description | remark
-------- | -------- | -------- | -------- | -------- | --------
MemberID | MemberID | yes | string | 成员代码 | 
MemberType | MemberType | no | string | 成员类型 | a:普通;b:内部做市商;c:系统手续费使用;d:风控使用;e:运营使用;f:运营保留资金使用;g:外部做市商;
Grade | Grade | no | string | 等级 | 
Priority | Priority | no | int | 优先权 | 
FeeGrade | FeeGrade | no | string | 手续费等级 | 
MarginRateGrade | MarginRateGrade | no | string | 保证金等级 | 
TradingRightGrade | TradingRightGrade | no | string | 权限等级 | 
CFDGrade | CFDGrade | no | string | 差价等级 | 
LiquidityGrade | LiquidityGrade | no | string | 流动性等级 | 
RiskLevel | RiskLevel | no | string | 风控类型 | 0:本币种资金权益;1:结算组所有资金权益;2:资金账号所有资金权益;3:会员账号所有资金权益;
PositionType | PositionType | no | string | 持仓类型 | 0:不设置;1:综合持仓(双边持仓);2:净持仓(单边持仓);
TriggerOrders | TriggerOrders | no | int | 触发订单个数 | 
OpenOrders | OpenOrders | no | int | 未成交订单数 | 
OpenPositions | OpenPositions | no | int | 用户持仓个数 | 
MaxLocalID | MaxLocalID | no | string | 最大本地标识 | 
SinkType | SinkType | no | string | 下游系统同步类型 | 0:同步到数据库;1:同步到Redis不到数据库;2:不同步;
CopyMemberID | CopyMemberID | no | string | 带单员代码 | 
Region | Region | no | string |  | 
FeeDeduceDisCount | FeeDeduceDisCount | no | double | 作为手续费的抵扣优惠比例 | 
FeeDeduceCurrency | FeeDeduceCurrency | no | string | 作为手续费的抵扣比例 | 
CanBorrow | CanBorrow | no | int | 是否需要借钱交易 | 
Remark | Remark | no | string | 备注 | 
InsertTime | InsertTime | no | double | 插入时间 | 
UpdateTime | UpdateTime | no | double | 更新时间 | 
## **用户会话**
+ **Request URL**  

```
 插入操作请求:  POST /action/v1.0/SendInsert/UserSession
 更新操作请求:  POST /action/v1.0/SendUpdate/UserSession
 删除操作请求:  POST /action/v1.0/SendDelete/UserSession
 插入没有就更新请求:  POST /action/v1.0/SendInsUpd/UserSession
 查询唯一索引记录请求:  POST /action/v1.0/SendQryOne/UserSession
 设置字段值清空请求:  POST /action/v1.0/SendClear/UserSession
 初始化操作请求:  POST /action/v1.0/SendInit/UserSession
```

+ **Request**

```
{
	UserID:"1983723"	
	,Password:""	
	,UserProductID:"1983723"	
	,MacAddress:""	
	,ClientIPAddress:""	
	,HDSerialID:"1983723"	
	,AuthCode:""	
	,APPID:"ios"	
	,Token:""	
	,Remark:"my_little_tips"	
	,APIID:"1983723"	
	,LoginTime:1657543832	
	,ExpireTime:1657543832	
	,IPAddress:""	
	,MaxLocalID:"1983723"	
	,SessionNo:1033201232	
	,FrontNo:1	
	,AccessLimit:0	
	,UserType:"1"	
	,MemberID:"1983723"	
	,LimitAccesses:0	
	,SinkType:"1"	
	,InsertTime:1657543832	
	,UpdateTime:1657543832	
}
```

+ **Response**

```
{
	"action": "Recv*",
	"errorCode": 0,
	"errorMsg": "Success",
	"result": [
		{"table":"UserSession",
		"data":{	
			UserID:"1983723"
			,Password:""
			,UserProductID:"1983723"
			,MacAddress:""
			,ClientIPAddress:""
			,HDSerialID:"1983723"
			,AuthCode:""
			,APPID:"ios"
			,Token:""
			,Remark:"my_little_tips"
			,APIID:"1983723"
			,LoginTime:1657543832
			,ExpireTime:1657543832
			,IPAddress:""
			,MaxLocalID:"1983723"
			,SessionNo:1033201232
			,FrontNo:1
			,AccessLimit:0
			,UserType:"1"
			,MemberID:"1983723"
			,LimitAccesses:0
			,SinkType:"1"
			,InsertTime:1657543832
			,UpdateTime:1657543832
		}
	}
}
```	

+ **Request & Response Parameters**

parameter | shortName | required | type | description | remark
-------- | -------- | -------- | -------- | -------- | --------
UserID | UserID | no | string | 交易用户代码 | 
Password | Password | no | string | 密码 | 
UserProductID | UserProductID | no | string | 用户端产品信息 | 
MacAddress | MacAddress | no | string | Mac地址 | 
ClientIPAddress | ClientIPAddress | no | string | 终端IP地址 | 
HDSerialID | HDSerialID | no | string | 硬盘序列号 | 
AuthCode | AuthCode | no | string | 授权编码 | 
APPID | APPID | no | string | 应用编号 | 
Token | Token | yes | string | 令牌 | 
Remark | Remark | no | string | 备注 | 
APIID | APIID | no | string | 接口端产品信息 | 
LoginTime | LoginTime | no | double | 登录时间 | 
ExpireTime | ExpireTime | no | double | 失效时间 | 
IPAddress | IPAddress | no | string | 允许登录的IP地址 | 
MaxLocalID | MaxLocalID | no | string | 最大本地标识 | 
SessionNo | SessionNo | no | int | 会话编号 | 
FrontNo | FrontNo | no | int | 前置编号 | 
AccessLimit | AccessLimit | no | int | 每秒访问的次数限制 | 
UserType | UserType | no | string | 用户类型 | 1:普通用户(订阅自己/可下自己订单);2:观察用户OB(订阅其他用户/不可下单);5:超级管理员(订阅所有/可给所有用户下单/可爆仓);
MemberID | MemberID | no | string | 成员代码 | 
LimitAccesses | LimitAccesses | no | int | 该秒已经访问的次数 | 
SinkType | SinkType | no | string | 下游系统同步类型 | 0:同步到数据库;1:同步到Redis不到数据库;2:不同步;
InsertTime | InsertTime | no | double | 插入时间 | 
UpdateTime | UpdateTime | no | double | 更新时间 | 
## **最新K线**
+ **Request URL**  

```
 插入操作请求:  POST /action/v1.0/SendInsert/LastKLine
 更新操作请求:  POST /action/v1.0/SendUpdate/LastKLine
 删除操作请求:  POST /action/v1.0/SendDelete/LastKLine
 插入没有就更新请求:  POST /action/v1.0/SendInsUpd/LastKLine
 查询唯一索引记录请求:  POST /action/v1.0/SendQryOne/LastKLine
 设置字段值清空请求:  POST /action/v1.0/SendClear/LastKLine
 初始化操作请求:  POST /action/v1.0/SendInit/LastKLine
```

+ **Request**

```
{
	ExchangeID:"CEX"	
	,InstrumentID:"BTCUSD"	
	,Bar:""	
	,InsertTime:1657543832	
	,OpenPrice:182582.18	
	,HighestPrice:182582.18	
	,LowestPrice:182582.18	
	,ClosePrice:182582.18	
	,Volume:182582.18	
	,Turnover:182582.18	
	,UpdateTime:1657543832	
}
```

+ **Response**

```
{
	"action": "Recv*",
	"errorCode": 0,
	"errorMsg": "Success",
	"result": [
		{"table":"LastKLine",
		"data":{	
			ExchangeID:"CEX"
			,InstrumentID:"BTCUSD"
			,Bar:""
			,InsertTime:1657543832
			,OpenPrice:182582.18
			,HighestPrice:182582.18
			,LowestPrice:182582.18
			,ClosePrice:182582.18
			,Volume:182582.18
			,Turnover:182582.18
			,UpdateTime:1657543832
		}
	}
}
```	

+ **Request & Response Parameters**

parameter | shortName | required | type | description | remark
-------- | -------- | -------- | -------- | -------- | --------
ExchangeID |  | yes | string | 交易所代码 | 
InstrumentID |  | yes | string | 标的代码 | 
Bar |  | yes | string | K线周期代码(n[m/h/d/o]) | 
InsertTime | I | no | double | 插入时间 | 
OpenPrice | O | no | double | 开盘价 | 
HighestPrice | H | no | double | 最高价 | 
LowestPrice | L | no | double | 最低价 | 
ClosePrice | C | no | double | 收盘价 | 
Volume | V | no | double | 数量 | 
Turnover | T | no | double | 成交金额 | 
UpdateTime | UpdateTime | no | double | 更新时间 | 
## **CFD溢价率**
+ **Request URL**  

```
 插入操作请求:  POST /action/v1.0/SendInsert/CFDRate
 更新操作请求:  POST /action/v1.0/SendUpdate/CFDRate
 删除操作请求:  POST /action/v1.0/SendDelete/CFDRate
 插入没有就更新请求:  POST /action/v1.0/SendInsUpd/CFDRate
 查询唯一索引记录请求:  POST /action/v1.0/SendQryOne/CFDRate
 设置字段值清空请求:  POST /action/v1.0/SendClear/CFDRate
 初始化操作请求:  POST /action/v1.0/SendInit/CFDRate
```

+ **Request**

```
{
	MemberID:"1983723"	
	,InstrumentID:"BTCUSD"	
	,TriggerOrderType:""	
	,RateBegin:182582.18	
	,RateEnd:182582.18	
	,CompVolume:182582.18	
	,CompRate:182582.18	
	,HardRate:182582.18	
	,Remark:"my_little_tips"	
	,InsertTime:1657543832	
	,UpdateTime:1657543832	
}
```

+ **Response**

```
{
	"action": "Recv*",
	"errorCode": 0,
	"errorMsg": "Success",
	"result": [
		{"table":"CFDRate",
		"data":{	
			MemberID:"1983723"
			,InstrumentID:"BTCUSD"
			,TriggerOrderType:""
			,RateBegin:182582.18
			,RateEnd:182582.18
			,CompVolume:182582.18
			,CompRate:182582.18
			,HardRate:182582.18
			,Remark:"my_little_tips"
			,InsertTime:1657543832
			,UpdateTime:1657543832
		}
	}
}
```	

+ **Request & Response Parameters**

parameter | shortName | required | type | description | remark
-------- | -------- | -------- | -------- | -------- | --------
MemberID | MemberID | yes | string | 成员代码或组 | 
InstrumentID | InstrumentID | yes | string | 标的代码 | 
TriggerOrderType | TriggerOrderType | yes | string | 报单价格类型 | 
RateBegin | RateBegin | no | double | 多头起始溢价率 | 
RateEnd | RateEnd | no | double | 多头最大溢价率 | 
CompVolume | CompVolume | no | double | 点差比较基准 | 
CompRate | CompRate | no | double | 点差比较基准率 | 
HardRate | HardRate | no | double | 硬顶率 | 
Remark | Remark | no | string | 备注 | 
InsertTime | InsertTime | no | double | 插入时间 | 
UpdateTime | UpdateTime | no | double | 更新时间 | 
## **交易权限**
+ **Request URL**  

```
 插入操作请求:  POST /action/v1.0/SendInsert/TradingRight
 更新操作请求:  POST /action/v1.0/SendUpdate/TradingRight
 删除操作请求:  POST /action/v1.0/SendDelete/TradingRight
 插入没有就更新请求:  POST /action/v1.0/SendInsUpd/TradingRight
 查询唯一索引记录请求:  POST /action/v1.0/SendQryOne/TradingRight
 设置字段值清空请求:  POST /action/v1.0/SendClear/TradingRight
 初始化操作请求:  POST /action/v1.0/SendInit/TradingRight
```

+ **Request**

```
{
	MemberID:"1983723"	
	,InstrumentID:"BTCUSD"	
	,Direction:"1"	
	,TradingRight:"1"	
	,Remark:"my_little_tips"	
	,InsertTime:1657543832	
	,UpdateTime:1657543832	
}
```

+ **Response**

```
{
	"action": "Recv*",
	"errorCode": 0,
	"errorMsg": "Success",
	"result": [
		{"table":"TradingRight",
		"data":{	
			MemberID:"1983723"
			,InstrumentID:"BTCUSD"
			,Direction:"1"
			,TradingRight:"1"
			,Remark:"my_little_tips"
			,InsertTime:1657543832
			,UpdateTime:1657543832
		}
	}
}
```	

+ **Request & Response Parameters**

parameter | shortName | required | type | description | remark
-------- | -------- | -------- | -------- | -------- | --------
MemberID | MemberID | yes | string | 成员代码 | 
InstrumentID | InstrumentID | yes | string | 标的代码 | 
Direction | Direction | yes | string | 买卖方向 | 0:买;1:卖;
TradingRight | TradingRight | no | string | 交易权限 | 0:可以交易;1:只能平仓;2:不能交易;
Remark | Remark | no | string | 备注 | 
InsertTime | InsertTime | no | double | 插入时间 | 
UpdateTime | UpdateTime | no | double | 更新时间 | 
## **保证金率**
+ **Request URL**  

```
 插入操作请求:  POST /action/v1.0/SendInsert/MarginRate
 更新操作请求:  POST /action/v1.0/SendUpdate/MarginRate
 删除操作请求:  POST /action/v1.0/SendDelete/MarginRate
 插入没有就更新请求:  POST /action/v1.0/SendInsUpd/MarginRate
 查询唯一索引记录请求:  POST /action/v1.0/SendQryOne/MarginRate
 设置字段值清空请求:  POST /action/v1.0/SendClear/MarginRate
 初始化操作请求:  POST /action/v1.0/SendInit/MarginRate
```

+ **Request**

```
{
	MemberID:"1983723"	
	,InstrumentID:"BTCUSD"	
	,ValueMode:"1"	
	,MarginSequenceType:"1"	
	,RiskSequenceType:"1"	
	,LongMarginRatio:182582.18	
	,LongMaintMarginRatio:182582.18	
	,LongMarginStep:182582.18	
	,LongMaintMarginStep:182582.18	
	,LongRiskLimit:182582.18	
	,LongRiskStep:182582.18	
	,ShortMarginRatio:182582.18	
	,ShortMaintMarginRatio:182582.18	
	,ShortMarginStep:182582.18	
	,ShortMaintMarginStep:182582.18	
	,ShortRiskLimit:182582.18	
	,ShortRiskStep:182582.18	
	,DefaultLeverage:125	
	,Remark:"my_little_tips"	
	,InsertTime:1657543832	
	,UpdateTime:1657543832	
}
```

+ **Response**

```
{
	"action": "Recv*",
	"errorCode": 0,
	"errorMsg": "Success",
	"result": [
		{"table":"MarginRate",
		"data":{	
			MemberID:"1983723"
			,InstrumentID:"BTCUSD"
			,ValueMode:"1"
			,MarginSequenceType:"1"
			,RiskSequenceType:"1"
			,LongMarginRatio:182582.18
			,LongMaintMarginRatio:182582.18
			,LongMarginStep:182582.18
			,LongMaintMarginStep:182582.18
			,LongRiskLimit:182582.18
			,LongRiskStep:182582.18
			,ShortMarginRatio:182582.18
			,ShortMaintMarginRatio:182582.18
			,ShortMarginStep:182582.18
			,ShortMaintMarginStep:182582.18
			,ShortRiskLimit:182582.18
			,ShortRiskStep:182582.18
			,DefaultLeverage:125
			,Remark:"my_little_tips"
			,InsertTime:1657543832
			,UpdateTime:1657543832
		}
	}
}
```	

+ **Request & Response Parameters**

parameter | shortName | required | type | description | remark
-------- | -------- | -------- | -------- | -------- | --------
MemberID | MemberID | yes | string | 成员代码 | 
InstrumentID | InstrumentID | yes | string | 标的代码 | 
ValueMode | ValueMode | no | string | 取值方式 | 0:百分比;1:绝对值;
MarginSequenceType | MarginSequenceType | no | string | 保证金数列 | 1:等差数列;2:等比数列;
RiskSequenceType | RiskSequenceType | no | string | 风险限额数列 | 1:等差数列;2:等比数列;
LongMarginRatio | LongMarginRatio | no | double | 多头起始保证金率 | 
LongMaintMarginRatio | LongMaintMarginRatio | no | double | 多头起始维持保证金率 | 
LongMarginStep | LongMarginStep | no | double | 多头保证金率步长 | 
LongMaintMarginStep | LongMaintMarginStep | no | double | 多头维持保证金率步长 | 
LongRiskLimit | LongRiskLimit | no | double | 多头起始风险限额 | 
LongRiskStep | LongRiskStep | no | double | 多头风险限额步长 | 
ShortMarginRatio | ShortMarginRatio | no | double | 空头起始保证金率 | 
ShortMaintMarginRatio | ShortMaintMarginRatio | no | double | 空头起始维持保证金率 | 
ShortMarginStep | ShortMarginStep | no | double | 空头保证金率步长 | 
ShortMaintMarginStep | ShortMaintMarginStep | no | double | 空头维持保证金率步长 | 
ShortRiskLimit | ShortRiskLimit | no | double | 空头起始风险限额 | 
ShortRiskStep | ShortRiskStep | no | double | 空头风险限额步长 | 
DefaultLeverage | DefaultLeverage | no | double | 默认杠杆倍数 | 
Remark | Remark | no | string | 备注 | 
InsertTime | InsertTime | no | double | 插入时间 | 
UpdateTime | UpdateTime | no | double | 更新时间 | 
## **手续费率**
+ **Request URL**  

```
 插入操作请求:  POST /action/v1.0/SendInsert/Fee
 更新操作请求:  POST /action/v1.0/SendUpdate/Fee
 删除操作请求:  POST /action/v1.0/SendDelete/Fee
 插入没有就更新请求:  POST /action/v1.0/SendInsUpd/Fee
 查询唯一索引记录请求:  POST /action/v1.0/SendQryOne/Fee
 设置字段值清空请求:  POST /action/v1.0/SendClear/Fee
 初始化操作请求:  POST /action/v1.0/SendInit/Fee
```

+ **Request**

```
{
	MemberID:"1983723"	
	,InstrumentID:"BTCUSD"	
	,MatchRole:"1"	
	,OpenFeeRate:182582.18	
	,OpenFeeAmount:182582.18	
	,CloseFeeRate:182582.18	
	,CloseFeeAmount:182582.18	
	,CloseTodayFeeRate:182582.18	
	,CloseTodayFeeAmount:182582.18	
	,Remark:"my_little_tips"	
	,InsertTime:1657543832	
	,UpdateTime:1657543832	
}
```

+ **Response**

```
{
	"action": "Recv*",
	"errorCode": 0,
	"errorMsg": "Success",
	"result": [
		{"table":"Fee",
		"data":{	
			MemberID:"1983723"
			,InstrumentID:"BTCUSD"
			,MatchRole:"1"
			,OpenFeeRate:182582.18
			,OpenFeeAmount:182582.18
			,CloseFeeRate:182582.18
			,CloseFeeAmount:182582.18
			,CloseTodayFeeRate:182582.18
			,CloseTodayFeeAmount:182582.18
			,Remark:"my_little_tips"
			,InsertTime:1657543832
			,UpdateTime:1657543832
		}
	}
}
```	

+ **Request & Response Parameters**

parameter | shortName | required | type | description | remark
-------- | -------- | -------- | -------- | -------- | --------
MemberID | MemberID | yes | string | 成员代码 | 
InstrumentID | InstrumentID | yes | string | 标的代码 | 
MatchRole | MatchRole | yes | string | 成交角色 | 0:不区分;1:主动成交方;2:被动成交方;3:集合竞价;4:交割;5:场外交易;
OpenFeeRate | OpenFeeRate | no | double | 开仓手续费按比例 | 
OpenFeeAmount | OpenFeeAmount | no | double | 开仓手续费按手数 | 
CloseFeeRate | CloseFeeRate | no | double | 平仓手续费按比例 | 
CloseFeeAmount | CloseFeeAmount | no | double | 平仓手续费按手数 | 
CloseTodayFeeRate | CloseTodayFeeRate | no | double | 平今仓手续费按比例 | 
CloseTodayFeeAmount | CloseTodayFeeAmount | no | double | 平今仓手续费按手数 | 
Remark | Remark | no | string | 备注 | 
InsertTime | InsertTime | no | double | 插入时间 | 
UpdateTime | UpdateTime | no | double | 更新时间 | 
## **流动性设置表**
+ **Request URL**  

```
 插入操作请求:  POST /action/v1.0/SendInsert/Liquidity
 更新操作请求:  POST /action/v1.0/SendUpdate/Liquidity
 删除操作请求:  POST /action/v1.0/SendDelete/Liquidity
 插入没有就更新请求:  POST /action/v1.0/SendInsUpd/Liquidity
 查询唯一索引记录请求:  POST /action/v1.0/SendQryOne/Liquidity
 设置字段值清空请求:  POST /action/v1.0/SendClear/Liquidity
 初始化操作请求:  POST /action/v1.0/SendInit/Liquidity
```

+ **Request**

```
{
	LiquidityGrade:""	
	,LiquidityGroup:"999"	
	,BasePriceType:"1"	
	,VolumeBegin:182582.18	
	,BuyOpen:182582.18	
	,BuyClose:182582.18	
	,SellOpen:182582.18	
	,SellClose:182582.18	
	,InsertTime:1657543832	
	,UpdateTime:1657543832	
}
```

+ **Response**

```
{
	"action": "Recv*",
	"errorCode": 0,
	"errorMsg": "Success",
	"result": [
		{"table":"Liquidity",
		"data":{	
			LiquidityGrade:""
			,LiquidityGroup:"999"
			,BasePriceType:"1"
			,VolumeBegin:182582.18
			,BuyOpen:182582.18
			,BuyClose:182582.18
			,SellOpen:182582.18
			,SellClose:182582.18
			,InsertTime:1657543832
			,UpdateTime:1657543832
		}
	}
}
```	

+ **Request & Response Parameters**

parameter | shortName | required | type | description | remark
-------- | -------- | -------- | -------- | -------- | --------
LiquidityGrade | LiquidityGrade | yes | string | 流动性等级 | 
LiquidityGroup | LiquidityGroup | yes | string | 标的代码 | 
BasePriceType | BasePriceType | no | string | 基于的价格 | 0:买一和买一中间值;1:标记价;2:最新价;
VolumeBegin | VolumeBegin | no | double | 起始量 | 
BuyOpen | BuyOpen | no | double | 买开流动性比例 | 
BuyClose | BuyClose | no | double | 买平流动性比例 | 
SellOpen | SellOpen | no | double | 卖开流动性比例 | 
SellClose | SellClose | no | double | 卖平流动性比例 | 
InsertTime | InsertTime | no | double | 插入时间 | 
UpdateTime | UpdateTime | no | double | 更新时间 | 
## **账户资金**
+ **Request URL**  

```
 插入操作请求:  POST /action/v1.0/SendInsert/Account
 更新操作请求:  POST /action/v1.0/SendUpdate/Account
 删除操作请求:  POST /action/v1.0/SendDelete/Account
 插入没有就更新请求:  POST /action/v1.0/SendInsUpd/Account
 查询唯一索引记录请求:  POST /action/v1.0/SendQryOne/Account
 设置字段值清空请求:  POST /action/v1.0/SendClear/Account
 初始化操作请求:  POST /action/v1.0/SendInit/Account
```

+ **Request**

```
{
	SettlementGroup:"999"	
	,AccountID:"1983723"	
	,Currency:"BTC"	
	,MemberID:"1983723"	
	,Available:182582.18	
	,Withdrawable:182582.18	
	,PreBalance:182582.18	
	,Balance:182582.18	
	,PositionCost:182582.18	
	,UseMargin:182582.18	
	,CrossMargin:182582.18	
	,CloseProfit:10	
	,Deposit:182582.18	
	,Withdraw:182582.18	
	,FrozenMargin:182582.18	
	,FrozenMoney:182582.18	
	,FrozenFee:182582.18	
	,Fee:0.22364	
	,MoneyChange:182582.18	
	,Reserve:182582.18	
	,ReserveAvailable:182582.18	
	,ReserveLimit:182582.18	
	,ReserveProfit:182582.18	
	,ReserveType:"1"	
	,ReserveActive:0	
	,TotalCloseProfit:182582.18	
	,TotalDeposit:182582.18	
	,TotalWithdraw:182582.18	
	,TotalFee:182582.18	
	,TotalMoneyChange:182582.18	
	,InsertTime:1657543832	
	,UpdateTime:1657543832	
	,AccountType:""	
	,BusinessNo:11176422300333	
	,MarginRateGrade:""	
	,TradingRightGrade:""	
	,Frequency:0	
	,MaintMargin:182582.18	
	,UnRealProfit:182582.18	
	,CopyProfit:182582.18	
	,CFDType:"1"	
	,FeeGrade:""	
	,CFDGrade:""	
	,Borrow:182582.18	
	,Remark:"my_little_tips"	
	,MaxLocalID:"1983723"	
	,CurrValue:182582.18	
	,CanReduce:0	
}
```

+ **Response**

```
{
	"action": "Recv*",
	"errorCode": 0,
	"errorMsg": "Success",
	"result": [
		{"table":"Account",
		"data":{	
			SettlementGroup:"999"
			,AccountID:"1983723"
			,Currency:"BTC"
			,MemberID:"1983723"
			,Available:182582.18
			,Withdrawable:182582.18
			,PreBalance:182582.18
			,Balance:182582.18
			,PositionCost:182582.18
			,UseMargin:182582.18
			,CrossMargin:182582.18
			,CloseProfit:10
			,Deposit:182582.18
			,Withdraw:182582.18
			,FrozenMargin:182582.18
			,FrozenMoney:182582.18
			,FrozenFee:182582.18
			,Fee:0.22364
			,MoneyChange:182582.18
			,Reserve:182582.18
			,ReserveAvailable:182582.18
			,ReserveLimit:182582.18
			,ReserveProfit:182582.18
			,ReserveType:"1"
			,ReserveActive:0
			,TotalCloseProfit:182582.18
			,TotalDeposit:182582.18
			,TotalWithdraw:182582.18
			,TotalFee:182582.18
			,TotalMoneyChange:182582.18
			,InsertTime:1657543832
			,UpdateTime:1657543832
			,AccountType:""
			,BusinessNo:11176422300333
			,MarginRateGrade:""
			,TradingRightGrade:""
			,Frequency:0
			,MaintMargin:182582.18
			,UnRealProfit:182582.18
			,CopyProfit:182582.18
			,CFDType:"1"
			,FeeGrade:""
			,CFDGrade:""
			,Borrow:182582.18
			,Remark:"my_little_tips"
			,MaxLocalID:"1983723"
			,CurrValue:182582.18
			,CanReduce:0
		}
	}
}
```	

+ **Request & Response Parameters**

parameter | shortName | required | type | description | remark
-------- | -------- | -------- | -------- | -------- | --------
SettlementGroup | SettlementGroup | yes | string | 结算组编号 | 
AccountID | AccountID | yes | string | 资金账号 | 
Currency | Currency | yes | string | 币种 | 
MemberID | MemberID | no | string | 成员代码 | 
Available | Available | no | double | 可用资金 | 
Withdrawable | Withdrawable | no | double | 可取资金 | 
PreBalance | PreBalance | no | double | 上次静态权益 | 
Balance | Balance | no | double | 静态权益 | 
PositionCost | PositionCost | no | double | 持仓成本 | 
UseMargin | UseMargin | no | double | 所有占用保证金 | 
CrossMargin | CrossMargin | no | double | 全仓保证金 | 
CloseProfit | CloseProfit | no | double | 平仓盈亏 | 
Deposit | Deposit | no | double | 入金金额 | 
Withdraw | Withdraw | no | double | 出金金额 | 
FrozenMargin | FrozenMargin | no | double | 冻结保证金 | 
FrozenMoney | FrozenMoney | no | double | 冻结资金 | 
FrozenFee | FrozenFee | no | double | 冻结手续费 | 
Fee | Fee | no | double | 手续费 | 
MoneyChange | MoneyChange | no | double | 资金变化 | 
Reserve | Reserve | no | double | 保留资金 | 
ReserveAvailable | ReserveAvailable | no | double | 保留资金可用 | 
ReserveLimit | ReserveLimit | no | double | 保留资金限制 | 
ReserveProfit | ReserveProfit | no | double | 保留资金盈亏 | 
ReserveType | ReserveType | no | string | 保留资金类型 | 0:先亏损保留资金再亏损真钱(劣后);1:先亏损真钱再亏损保留资金(优先);2:保留资金不做亏损(抵押金);
ReserveActive | ReserveActive | no | int | 保留资金是否活跃 | 
TotalCloseProfit | TotalCloseProfit | no | double | 总平仓盈亏 | 
TotalDeposit | TotalDeposit | no | double | 总入金金额 | 
TotalWithdraw | TotalWithdraw | no | double | 总出金金额 | 
TotalFee | TotalFee | no | double | 总手续费 | 
TotalMoneyChange | TotalMoneyChange | no | double | 总资金变化 | 
InsertTime | InsertTime | no | double | 插入时间 | 
UpdateTime | UpdateTime | no | double | 更新时间 | 
AccountType | AccountType | no | string | 账户类型 | 
BusinessNo | BusinessNo | no | double | 业务序列号 | 
MarginRateGrade | MarginRateGrade | no | string | 保证金等级 | 
TradingRightGrade | TradingRightGrade | no | string | 权限等级 | 
Frequency | Frequency | no | int | 每秒更新的次数 | 
MaintMargin | MaintMargin | no | double | 维持保证金 | 
UnRealProfit | UnRealProfit | no | double | 未实现盈亏 | 
CopyProfit | CopyProfit | no | double | 带单盈利分配 | 
CFDType | CFDType | no | string | 点差接管类型 | 0:先撮合后点差接管;1:只撮合不点差接管;2:不撮合只点差接管;
FeeGrade | FeeGrade | no | string | 手续费等级 | 
CFDGrade | CFDGrade | no | string | 差价等级 | 
Borrow | Borrow | no | double | 借款周期结算量 | 
Remark | Remark | no | string | 备注 | 
MaxLocalID | MaxLocalID | no | string | 最大本地标识 | 
CurrValue | CurrValue | no | double | 当前价值 | 
CanReduce | CanReduce | no | int | 是否可以出金 | 
## **成员持仓汇总**
+ **Request URL**  

```
 插入操作请求:  POST /action/v1.0/SendInsert/MemberPosition
 更新操作请求:  POST /action/v1.0/SendUpdate/MemberPosition
 删除操作请求:  POST /action/v1.0/SendDelete/MemberPosition
 插入没有就更新请求:  POST /action/v1.0/SendInsUpd/MemberPosition
 查询唯一索引记录请求:  POST /action/v1.0/SendQryOne/MemberPosition
 设置字段值清空请求:  POST /action/v1.0/SendClear/MemberPosition
 初始化操作请求:  POST /action/v1.0/SendInit/MemberPosition
```

+ **Request**

```
{
	MemberID:"1983723"	
	,AccountID:"1983723"	
	,ExchangeID:"CEX"	
	,InstrumentID:"BTCUSD"	
	,Position:182582.18	
	,LongFrozen:182582.18	
	,ShortFrozen:182582.18	
	,DefaultLeverage:125	
	,DefaultCrossMargin:0	
	,InsertTime:1657543832	
	,UpdateTime:1657543832	
}
```

+ **Response**

```
{
	"action": "Recv*",
	"errorCode": 0,
	"errorMsg": "Success",
	"result": [
		{"table":"MemberPosition",
		"data":{	
			MemberID:"1983723"
			,AccountID:"1983723"
			,ExchangeID:"CEX"
			,InstrumentID:"BTCUSD"
			,Position:182582.18
			,LongFrozen:182582.18
			,ShortFrozen:182582.18
			,DefaultLeverage:125
			,DefaultCrossMargin:0
			,InsertTime:1657543832
			,UpdateTime:1657543832
		}
	}
}
```	

+ **Request & Response Parameters**

parameter | shortName | required | type | description | remark
-------- | -------- | -------- | -------- | -------- | --------
MemberID | MemberID | yes | string | 成员代码 | 
AccountID | AccountID | yes | string | 资金账号 | 
ExchangeID | ExchangeID | yes | string | 交易所代码 | 
InstrumentID | InstrumentID | yes | string | 标的代码 | 
Position | Position | no | double | 总持仓 | 
LongFrozen | LongFrozen | no | double | 多头冻结 | 
ShortFrozen | ShortFrozen | no | double | 空头冻结 | 
DefaultLeverage | DefaultLeverage | no | double | 默认杠杆倍数 | 
DefaultCrossMargin | DefaultCrossMargin | no | int | 默认是否全仓 | 
InsertTime | InsertTime | no | double | 插入时间 | 
UpdateTime | UpdateTime | no | double | 更新时间 | 
## **标的持仓**
+ **Request URL**  

```
 插入操作请求:  POST /action/v1.0/SendInsert/Position
 更新操作请求:  POST /action/v1.0/SendUpdate/Position
 删除操作请求:  POST /action/v1.0/SendDelete/Position
 插入没有就更新请求:  POST /action/v1.0/SendInsUpd/Position
 查询唯一索引记录请求:  POST /action/v1.0/SendQryOne/Position
 设置字段值清空请求:  POST /action/v1.0/SendClear/Position
 初始化操作请求:  POST /action/v1.0/SendInit/Position
```

+ **Request**

```
{
	PositionID:"1983723"	
	,MemberID:"1983723"	
	,TradeUnitID:"1983723"	
	,ExchangeID:"CEX"	
	,InstrumentID:"BTCUSD"	
	,PositionClass:"1"	
	,PosiDirection:"1"	
	,PrePosition:182582.18	
	,Position:182582.18	
	,LongFrozen:182582.18	
	,ShortFrozen:182582.18	
	,PreLongFrozen:182582.18	
	,PreShortFrozen:182582.18	
	,HighestPosition:182582.18	
	,ClosePosition:182582.18	
	,PositionCost:182582.18	
	,CostPrice:182582.18	
	,UseMargin:182582.18	
	,FrozenMargin:182582.18	
	,LongFrozenMargin:182582.18	
	,ShortFrozenMargin:182582.18	
	,CloseProfit:10	
	,TotalPositionCost:182582.18	
	,TotalCloseProfit:182582.18	
	,OpenPrice:182582.18	
	,ClosePrice:182582.18	
	,TradeFee:182582.18	
	,PositionFee:182582.18	
	,Leverage:125	
	,AccountID:"1983723"	
	,PriceCurrency:"BTC"	
	,ClearCurrency:"BTC"	
	,SettlementGroup:"999"	
	,IsCrossMargin:1	
	,CloseOrderID:"1983723"	
	,SLTriggerPrice:182582.18	
	,TPTriggerPrice:182582.18	
	,BeginTime:1657543832	
	,InsertTime:1657543832	
	,LastOpenTime:1657543832	
	,UpdateTime:1657543832	
	,BusinessNo:11176422300333	
	,IsAutoAddMargin:1	
	,Frequency:0	
	,MaintMargin:182582.18	
	,UnRealProfit:182582.18	
	,LiquidPrice:182582.18	
	,CreateTime:""	
	,CopyMemberID:"1983723"	
	,CopyProfitRate:182582.18	
	,CopyProfit:182582.18	
	,FirstTradeID:"1983723"	
	,LastTradeID:"1983723"	
	,BusinessType:"1"	
	,BusinessValue:""	
	,Reserve:182582.18	
	,ReserveProfit:182582.18	
	,Remark:"my_little_tips"	
}
```

+ **Response**

```
{
	"action": "Recv*",
	"errorCode": 0,
	"errorMsg": "Success",
	"result": [
		{"table":"Position",
		"data":{	
			PositionID:"1983723"
			,MemberID:"1983723"
			,TradeUnitID:"1983723"
			,ExchangeID:"CEX"
			,InstrumentID:"BTCUSD"
			,PositionClass:"1"
			,PosiDirection:"1"
			,PrePosition:182582.18
			,Position:182582.18
			,LongFrozen:182582.18
			,ShortFrozen:182582.18
			,PreLongFrozen:182582.18
			,PreShortFrozen:182582.18
			,HighestPosition:182582.18
			,ClosePosition:182582.18
			,PositionCost:182582.18
			,CostPrice:182582.18
			,UseMargin:182582.18
			,FrozenMargin:182582.18
			,LongFrozenMargin:182582.18
			,ShortFrozenMargin:182582.18
			,CloseProfit:10
			,TotalPositionCost:182582.18
			,TotalCloseProfit:182582.18
			,OpenPrice:182582.18
			,ClosePrice:182582.18
			,TradeFee:182582.18
			,PositionFee:182582.18
			,Leverage:125
			,AccountID:"1983723"
			,PriceCurrency:"BTC"
			,ClearCurrency:"BTC"
			,SettlementGroup:"999"
			,IsCrossMargin:1
			,CloseOrderID:"1983723"
			,SLTriggerPrice:182582.18
			,TPTriggerPrice:182582.18
			,BeginTime:1657543832
			,InsertTime:1657543832
			,LastOpenTime:1657543832
			,UpdateTime:1657543832
			,BusinessNo:11176422300333
			,IsAutoAddMargin:1
			,Frequency:0
			,MaintMargin:182582.18
			,UnRealProfit:182582.18
			,LiquidPrice:182582.18
			,CreateTime:""
			,CopyMemberID:"1983723"
			,CopyProfitRate:182582.18
			,CopyProfit:182582.18
			,FirstTradeID:"1983723"
			,LastTradeID:"1983723"
			,BusinessType:"1"
			,BusinessValue:""
			,Reserve:182582.18
			,ReserveProfit:182582.18
			,Remark:"my_little_tips"
		}
	}
}
```	

+ **Request & Response Parameters**

parameter | shortName | required | type | description | remark
-------- | -------- | -------- | -------- | -------- | --------
PositionID | PositionID | yes | string | 持仓代码 | 
MemberID | MemberID | no | string | 成员代码 | 
TradeUnitID | TradeUnitID | no | string | 交易单元代码 | 
ExchangeID | ExchangeID | no | string | 交易所代码 | 
InstrumentID | InstrumentID | no | string | 标的代码 | 
PositionClass | PositionClass | no | string | 持仓类型 | 0:不设置;1:保证金交易;3:资产;4:持币成本;
PosiDirection | PosiDirection | no | string | 持仓多空方向 | 0:多头;1:空头;2:净;
PrePosition | PrePosition | no | double | 上次持仓 | 
Position | Position | yes | double | 总持仓 | 
LongFrozen | LongFrozen | no | double | 多头冻结 | 
ShortFrozen | ShortFrozen | no | double | 空头冻结 | 
PreLongFrozen | PreLongFrozen | no | double | 昨日多头冻结 | 
PreShortFrozen | PreShortFrozen | no | double | 昨日空头冻结 | 
HighestPosition | HighestPosition | no | double | 最大持仓 | 
ClosePosition | ClosePosition | no | double | 可平持仓 | 
PositionCost | PositionCost | no | double | 持仓成本 | 
CostPrice | CostPrice | no | double | 成本价 | 
UseMargin | UseMargin | no | double | 占用保证金 | 
FrozenMargin | FrozenMargin | no | double | 冻结保证金 | 
LongFrozenMargin | LongFrozenMargin | no | double | 多头冻结保证金 | 
ShortFrozenMargin | ShortFrozenMargin | no | double | 空头冻结保证金 | 
CloseProfit | CloseProfit | no | double | 平仓盈亏 | 
TotalPositionCost | TotalPositionCost | no | double | 开仓成本 | 
TotalCloseProfit | TotalCloseProfit | no | double | 总平仓盈亏 | 
OpenPrice | OpenPrice | no | double | 开仓均价 | 
ClosePrice | ClosePrice | no | double | 平仓均价 | 
TradeFee | TradeFee | no | double | 交易费用 | 
PositionFee | PositionFee | no | double | 持仓费用或者股票分红等 | 
Leverage | Leverage | no | double | 杠杆倍数 | 
AccountID | AccountID | no | string | 资金账号 | 
PriceCurrency | PriceCurrency | no | string | 计价币种 | 
ClearCurrency | ClearCurrency | no | string | 清算币种 | 
SettlementGroup | SettlementGroup | no | string | 结算组编号 | 
IsCrossMargin | IsCrossMargin | no | int | 是否全仓 | 
CloseOrderID | CloseOrderID | no | string | 平仓单代码 | 
SLTriggerPrice | SLTriggerPrice | no | double | 止损触发价 | 
TPTriggerPrice | TPTriggerPrice | no | double | 止盈触发价 | 
BeginTime | BeginTime | no | double | 持仓不为0的开始时间 | 
InsertTime | InsertTime | no | double | 插入时间(新持仓时间) | 
LastOpenTime | LastOpenTime | no | double | 最后开仓时间 | 
UpdateTime | UpdateTime | no | double | 更新时间 | 
BusinessNo | BusinessNo | no | double | 业务序列号 | 
IsAutoAddMargin | IsAutoAddMargin | no | int | 是否自动追加保证金 | 
Frequency | Frequency | no | int | 每秒更新的次数 | 
MaintMargin | MaintMargin | no | double | 维持保证金 | 
UnRealProfit | UnRealProfit | no | double | 未实现盈亏 | 
LiquidPrice | LiquidPrice | no | double | 清算价格 | 
CreateTime | CreateTime | no | string | 创建时间 | 
CopyMemberID | CopyMemberID | no | string | 带单员代码 | 
CopyProfitRate | CopyProfitRate | no | double | 带单分配比例 | 
CopyProfit | CopyProfit | no | double | 带单盈利分配 | 
FirstTradeID | FID | no | string | 初始成交订单号 | 
LastTradeID | LID | no | string | 结束成交订单号 | 
BusinessType | BT | no | string | 业务类别 | 0:普通;1:申赎;2:行权;3:中立仓申报;4:递延交割申报;5:互换定单;6:质押;7:合并分拆;8:转股;9:回售回购;a:投票;b:融资融券;c:期权执行;d:传递被平仓订单BusinessValue;P:自定义11;Q:自定义10;R:自定义9;S:连续交易;T:自定义7;U:画线委托;V:限价市价;W:条件;X:止盈止损;Y:追踪出场;Z:指标类型;
BusinessValue | BV | no | string | 业务值 | 
Reserve | Reserve | no | double | 保留资金 | 
ReserveProfit | ReserveProfit | no | double | 保留资金盈亏 | 
Remark | R | no | string | 备注 | 
## **报单**
+ **Request URL**  

```
 插入操作请求:  POST /action/v1.0/SendInsert/Order
 更新操作请求:  POST /action/v1.0/SendUpdate/Order
 删除操作请求:  POST /action/v1.0/SendDelete/Order
 插入没有就更新请求:  POST /action/v1.0/SendInsUpd/Order
 查询唯一索引记录请求:  POST /action/v1.0/SendQryOne/Order
 设置字段值清空请求:  POST /action/v1.0/SendClear/Order
 初始化操作请求:  POST /action/v1.0/SendInit/Order
```

+ **Request**

```
{
	MemberID:"1983723"	
	,TradeUnitID:"1983723"	
	,AccountID:"1983723"	
	,LocalID:"1983723"	
	,ExchangeID:"CEX"	
	,InstrumentID:"BTCUSD"	
	,OrderPriceType:"1"	
	,Direction:"1"	
	,OffsetFlag:"1"	
	,Price:182582.18	
	,Volume:182582.18	
	,VolumeDisplay:182582.18	
	,VolumeMode:"1"	
	,Cost:182582.18	
	,OrderType:"1"	
	,GTDTime:1657543832	
	,MinVolume:182582.18	
	,BusinessType:"1"	
	,BusinessValue:""	
	,CloseOrderID:"1983723"	
	,IsCrossMargin:1	
	,Remark:"my_little_tips"	
	,OrderID:"12376422300333"	
	,CopyMemberID:"1983723"	
	,CopyOrderID:"1983723"	
	,Leverage:125	
	,CopyProfitRate:182582.18	
	,APPID:"ios"	
	,PositionID:"1983723"	
	,TriggerPrice:182582.18	
	,Reserve:182582.18	
	,OrderStatus:"1"	
	,DeriveSource:"1"	
	,DeriveDetail:""	
	,VolumeTraded:182582.18	
	,VolumeRemain:182582.18	
	,VolumeCancled:182582.18	
	,InsertTime:1657543832	
	,UpdateTime:1657543832	
	,Priority:0	
	,TimeSortNo:1657543832	
	,FrontNo:1	
	,PriceCurrency:"BTC"	
	,FeeCurrency:"BTC"	
	,ClearCurrency:"BTC"	
	,FrozenMoney:182582.18	
	,FrozenFee:182582.18	
	,FrozenMargin:182582.18	
	,Fee:0.22364	
	,CloseProfit:10	
	,Turnover:182582.18	
	,RelatedOrderID:"1983723"	
	,BusinessResult:""	
	,BusinessNo:11176422300333	
	,Tradable:0	
	,SettlementGroup:"999"	
	,PosiDirection:"1"	
	,TradePrice:182582.18	
	,OpenPrice:182582.18	
	,TriggerOrderID:"1983723"	
	,SLTriggerPrice:182582.18	
	,TPTriggerPrice:182582.18	
	,CopyProfit:182582.18	
	,Position:182582.18	
	,UserID:"1983723"	
	,LastPriceByInsert:182582.18	
	,BidPrice1ByInsert:182582.18	
	,AskPrice1ByInsert:182582.18	
	,Available:182582.18	
	,CreateTime:""	
}
```

+ **Response**

```
{
	"action": "Recv*",
	"errorCode": 0,
	"errorMsg": "Success",
	"result": [
		{"table":"Order",
		"data":{	
			MemberID:"1983723"
			,TradeUnitID:"1983723"
			,AccountID:"1983723"
			,LocalID:"1983723"
			,ExchangeID:"CEX"
			,InstrumentID:"BTCUSD"
			,OrderPriceType:"1"
			,Direction:"1"
			,OffsetFlag:"1"
			,Price:182582.18
			,Volume:182582.18
			,VolumeDisplay:182582.18
			,VolumeMode:"1"
			,Cost:182582.18
			,OrderType:"1"
			,GTDTime:1657543832
			,MinVolume:182582.18
			,BusinessType:"1"
			,BusinessValue:""
			,CloseOrderID:"1983723"
			,IsCrossMargin:1
			,Remark:"my_little_tips"
			,OrderID:"12376422300333"
			,CopyMemberID:"1983723"
			,CopyOrderID:"1983723"
			,Leverage:125
			,CopyProfitRate:182582.18
			,APPID:"ios"
			,PositionID:"1983723"
			,TriggerPrice:182582.18
			,Reserve:182582.18
			,OrderStatus:"1"
			,DeriveSource:"1"
			,DeriveDetail:""
			,VolumeTraded:182582.18
			,VolumeRemain:182582.18
			,VolumeCancled:182582.18
			,InsertTime:1657543832
			,UpdateTime:1657543832
			,Priority:0
			,TimeSortNo:1657543832
			,FrontNo:1
			,PriceCurrency:"BTC"
			,FeeCurrency:"BTC"
			,ClearCurrency:"BTC"
			,FrozenMoney:182582.18
			,FrozenFee:182582.18
			,FrozenMargin:182582.18
			,Fee:0.22364
			,CloseProfit:10
			,Turnover:182582.18
			,RelatedOrderID:"1983723"
			,BusinessResult:""
			,BusinessNo:11176422300333
			,Tradable:0
			,SettlementGroup:"999"
			,PosiDirection:"1"
			,TradePrice:182582.18
			,OpenPrice:182582.18
			,TriggerOrderID:"1983723"
			,SLTriggerPrice:182582.18
			,TPTriggerPrice:182582.18
			,CopyProfit:182582.18
			,Position:182582.18
			,UserID:"1983723"
			,LastPriceByInsert:182582.18
			,BidPrice1ByInsert:182582.18
			,AskPrice1ByInsert:182582.18
			,Available:182582.18
			,CreateTime:""
		}
	}
}
```	

+ **Request & Response Parameters**

parameter | shortName | required | type | description | remark
-------- | -------- | -------- | -------- | -------- | --------
MemberID | M | no | string | 成员代码 | 
TradeUnitID | T | no | string | 交易单元代码 | 
AccountID | A | no | string | 资金账号 | 
LocalID | L | no | string | 报单本地标识 | 
ExchangeID | E | no | string | 交易所代码 | 
InstrumentID | I | no | string | 标的代码 | 
OrderPriceType | PT | no | string | 报单价格类型 | 0:限价(手动指定Price值);1:任意价(Price按最高最低限价取值);2:最优价/对手价(Price按对方1档价取值);3:五档价(Price按对方5档价取值);4:十档价(类同上);7:最优Marker价(Price按本方1档价或其+1Tick取值);8:Price按委托额Cost和订单簿情况确定值;9:保底点差价:成交价格Price不会劣于系统设置差价成交;
Direction | D | no | string | 买卖方向 | 0:买;1:卖;
OffsetFlag | O | no | string | 开平标志 | 0:开仓;1:平仓;2:强平;3:平今;4:平昨;5:全平;6:指定订单平仓;7:指定成交平仓;8:最大平仓;
Price | P | no | double | 报单价格 | 
Volume | V | no | double | 数量 | 
VolumeDisplay | D | no | double | 显示数量 | 
VolumeMode | VM | no | string | 数量取值方式 | 0:百分比;1:绝对值;
Cost | C | no | double | 委托额 | 
OrderType | OT | no | string | 订单类型 | 0:普通订单;1:(FillAndKill/IOC)立即完成并且剩余撤销;2:(FillOrKill)立即完成全部或者最小订单要求否则全部撤销;3:需要进入订单簿被动成交,否则全部撤销(只做Maker/PostOnly);4:不需要撮合直接成交;
GTDTime | GT | no | double | GTD时间 | 
MinVolume | MV | no | double | 最小成交量 | 
BusinessType | BT | no | string | 业务类别 | 0:普通;1:申赎;2:行权;3:中立仓申报;4:递延交割申报;5:互换定单;6:质押;7:合并分拆;8:转股;9:回售回购;a:投票;b:融资融券;c:期权执行;d:传递被平仓订单BusinessValue;P:自定义11;Q:自定义10;R:自定义9;S:连续交易;T:自定义7;U:画线委托;V:限价市价;W:条件;X:止盈止损;Y:追踪出场;Z:指标类型;
BusinessValue | BV | no | string | 业务值 | 
CloseOrderID | CI | no | string | 平仓指定开仓的订单号 | 
IsCrossMargin | IsCrossMargin | no | int | 是否全仓 | 
Remark | R | no | string | 备注 | 
OrderID | OrderID | yes | string | 报单系统唯一代码 | 
CopyMemberID | CopyMemberID | no | string | 带单员代码 | 
CopyOrderID | CopyOrderID | no | string | 带单员报单号 | 
Leverage | Leverage | no | double | 委托单杠杆倍数 | 
CopyProfitRate | CopyProfitRate | no | double | 带单分配比例 | 
APPID | APPID | no | string | 应用编号 | 
PositionID | B | no | string | 持仓代码 | 
TriggerPrice | TriggerPrice | no | double | 触发价 | 
Reserve | Reserve | no | double | 保留资金 | 
OrderStatus | OrderStatus | no | string | 报单状态 | 0:未设置状态;1:全部成交;2:部分成交未撤单;3:部分成交已撤单;4:未成交未撤单;6:无成交已撤单;
DeriveSource | DeriveSource | no | string | 衍生来源 | 0:正常;1:报价衍生;2:期权执行;3:组合衍生;4:场外成交强平衍生;5:场外成交大宗交易衍生;6:场外成交期转现衍生;7:场外成交只刷单衍生;8:触发订单衍生;9:止损触发订单衍生;a:止盈触发订单衍生;b:资金操作衍生;c:仓位合并衍生;d:CFD订单衍生;e:场外成交ADL衍生;f:回购手续费不能是币;
DeriveDetail | DeriveDetail | no | string | 衍生明细 | 
VolumeTraded | VolumeTraded | no | double | 成交数量 | 
VolumeRemain | VolumeRemain | no | double | 剩余数量 | 
VolumeCancled | VolumeCancled | no | double | 已经撤单数量 | 
InsertTime | InsertTime | no | double | 插入时间 | 
UpdateTime | UpdateTime | no | double | 更新时间 | 
Priority |  | no | int | 优先权 | 
TimeSortNo |  | no | double | 按时间排队的序号 | 
FrontNo | FrontNo | no | int | 前置编号 | 
PriceCurrency | PriceCurrency | no | string | 计价币种 | 
FeeCurrency | FeeCurrency | no | string | 手续费币种 | 
ClearCurrency | ClearCurrency | no | string | 清算币种 | 
FrozenMoney | FrozenMoney | no | double | 冻结资金 | 
FrozenFee | FrozenFee | no | double | 冻结手续费 | 
FrozenMargin | FrozenMargin | no | double | 冻结保证金 | 
Fee | Fee | no | double | 手续费 | 
CloseProfit | CloseProfit | no | double | 平仓盈亏 | 
Turnover | Turnover | no | double | 成交金额 | 
RelatedOrderID | RelatedOrderID | no | string | 关联报单号 | 
BusinessResult | BusinessResult | no | string | 业务执行结果 | 
BusinessNo | BusinessNo | no | double | 业务序列号 | 
Tradable |  | no | int | 是否可以参加交易 | 
SettlementGroup |  | no | string | 结算组编号 | 
PosiDirection | PosiDirection | no | string | 持仓多空方向 | 0:多头;1:空头;2:净;
TradePrice | TradePrice | no | double | 成交均价 | 
OpenPrice | OpenPrice | no | double | 平仓成交时的开仓均价 | 
TriggerOrderID | TriggerOrderID | no | string | 触发报单号 | 
SLTriggerPrice | SLTriggerPrice | no | double | 开仓报单成交之后的止损触发价 | 
TPTriggerPrice | TPTriggerPrice | no | double | 开仓报单成交之后的止盈触发价 | 
CopyProfit | CopyProfit | no | double | 带单盈利分配 | 
Position | Position | no | double | 成交时的持仓量 | 
UserID | U | no | string | 交易用户代码 | 
LastPriceByInsert |  | no | double | 插入时的最新价 | 
BidPrice1ByInsert |  | no | double | 插入时的买一价 | 
AskPrice1ByInsert |  | no | double | 插入时的卖一价 | 
Available | Available | no | double | 可用资金 | 
CreateTime |  | no | string | 创建时间 | 
## **条件报单**
+ **Request URL**  

```
 插入操作请求:  POST /action/v1.0/SendInsert/TriggerOrder
 更新操作请求:  POST /action/v1.0/SendUpdate/TriggerOrder
 删除操作请求:  POST /action/v1.0/SendDelete/TriggerOrder
 插入没有就更新请求:  POST /action/v1.0/SendInsUpd/TriggerOrder
 查询唯一索引记录请求:  POST /action/v1.0/SendQryOne/TriggerOrder
 设置字段值清空请求:  POST /action/v1.0/SendClear/TriggerOrder
 初始化操作请求:  POST /action/v1.0/SendInit/TriggerOrder
```

+ **Request**

```
{
	MemberID:"1983723"	
	,TradeUnitID:"1983723"	
	,AccountID:"1983723"	
	,LocalID:"1983723"	
	,ExchangeID:"CEX"	
	,InstrumentID:"BTCUSD"	
	,OrderPriceType:"1"	
	,Direction:"1"	
	,OffsetFlag:"1"	
	,Price:182582.18	
	,Volume:182582.18	
	,VolumeDisplay:182582.18	
	,VolumeMode:"1"	
	,Cost:182582.18	
	,OrderType:"1"	
	,GTDTime:1657543832	
	,MinVolume:182582.18	
	,BusinessType:"1"	
	,BusinessValue:""	
	,CloseOrderID:"1983723"	
	,IsCrossMargin:1	
	,Remark:"my_little_tips"	
	,OrderID:"12376422300333"	
	,CopyMemberID:"1983723"	
	,CopyOrderID:"1983723"	
	,Leverage:125	
	,CopyProfitRate:182582.18	
	,APPID:"ios"	
	,PositionID:"1983723"	
	,TriggerPrice:182582.18	
	,Reserve:182582.18	
	,SLPrice:182582.18	
	,SLTriggerPrice:182582.18	
	,TPPrice:182582.18	
	,TPTriggerPrice:182582.18	
	,RiskBefore:0	
	,TriggerOrderType:"1"	
	,TriggerDetail:""	
	,TriggerPriceType:"1"	
	,TriggerValue:""	
	,CloseSLPrice:182582.18	
	,CloseSLTriggerPrice:182582.18	
	,CloseTPPrice:182582.18	
	,CloseTPTriggerPrice:182582.18	
	,CloseOrderPriceType:"1"	
	,ClosePrice:182582.18	
	,CloseTriggerPrice:182582.18	
	,RelatedOrderID:"1983723"	
	,ActiveTime:1657543832	
	,TriggerTime:1657543832	
	,TimeSortNo:1657543832	
	,TriggerStatus:"1"	
	,PosiDirection:"1"	
	,FrontNo:1	
	,ErrorNo:1033201232	
	,ErrorMsg:""	
	,InsertTime:1657543832	
	,UpdateTime:1657543832	
	,BusinessNo:11176422300333	
	,CreateTime:""	
}
```

+ **Response**

```
{
	"action": "Recv*",
	"errorCode": 0,
	"errorMsg": "Success",
	"result": [
		{"table":"TriggerOrder",
		"data":{	
			MemberID:"1983723"
			,TradeUnitID:"1983723"
			,AccountID:"1983723"
			,LocalID:"1983723"
			,ExchangeID:"CEX"
			,InstrumentID:"BTCUSD"
			,OrderPriceType:"1"
			,Direction:"1"
			,OffsetFlag:"1"
			,Price:182582.18
			,Volume:182582.18
			,VolumeDisplay:182582.18
			,VolumeMode:"1"
			,Cost:182582.18
			,OrderType:"1"
			,GTDTime:1657543832
			,MinVolume:182582.18
			,BusinessType:"1"
			,BusinessValue:""
			,CloseOrderID:"1983723"
			,IsCrossMargin:1
			,Remark:"my_little_tips"
			,OrderID:"12376422300333"
			,CopyMemberID:"1983723"
			,CopyOrderID:"1983723"
			,Leverage:125
			,CopyProfitRate:182582.18
			,APPID:"ios"
			,PositionID:"1983723"
			,TriggerPrice:182582.18
			,Reserve:182582.18
			,SLPrice:182582.18
			,SLTriggerPrice:182582.18
			,TPPrice:182582.18
			,TPTriggerPrice:182582.18
			,RiskBefore:0
			,TriggerOrderType:"1"
			,TriggerDetail:""
			,TriggerPriceType:"1"
			,TriggerValue:""
			,CloseSLPrice:182582.18
			,CloseSLTriggerPrice:182582.18
			,CloseTPPrice:182582.18
			,CloseTPTriggerPrice:182582.18
			,CloseOrderPriceType:"1"
			,ClosePrice:182582.18
			,CloseTriggerPrice:182582.18
			,RelatedOrderID:"1983723"
			,ActiveTime:1657543832
			,TriggerTime:1657543832
			,TimeSortNo:1657543832
			,TriggerStatus:"1"
			,PosiDirection:"1"
			,FrontNo:1
			,ErrorNo:1033201232
			,ErrorMsg:""
			,InsertTime:1657543832
			,UpdateTime:1657543832
			,BusinessNo:11176422300333
			,CreateTime:""
		}
	}
}
```	

+ **Request & Response Parameters**

parameter | shortName | required | type | description | remark
-------- | -------- | -------- | -------- | -------- | --------
MemberID | M | no | string | 成员代码 | 
TradeUnitID | T | no | string | 交易单元代码 | 
AccountID | A | no | string | 资金账号 | 
LocalID | L | no | string | 报单本地标识 | 
ExchangeID | E | no | string | 交易所代码 | 
InstrumentID | I | no | string | 标的代码 | 
OrderPriceType | PT | no | string | 报单价格类型 | 0:限价(手动指定Price值);1:任意价(Price按最高最低限价取值);2:最优价/对手价(Price按对方1档价取值);3:五档价(Price按对方5档价取值);4:十档价(类同上);7:最优Marker价(Price按本方1档价或其+1Tick取值);8:Price按委托额Cost和订单簿情况确定值;9:保底点差价:成交价格Price不会劣于系统设置差价成交;
Direction | D | no | string | 买卖方向 | 0:买;1:卖;
OffsetFlag | O | no | string | 开平标志 | 0:开仓;1:平仓;2:强平;3:平今;4:平昨;5:全平;6:指定订单平仓;7:指定成交平仓;8:最大平仓;
Price | P | no | double | 报单价格 | 
Volume | V | no | double | 数量 | 
VolumeDisplay | D | no | double | 显示数量 | 
VolumeMode | VM | no | string | 数量取值方式 | 0:百分比;1:绝对值;
Cost | C | no | double | 委托额 | 
OrderType | OT | no | string | 订单类型 | 0:普通订单;1:(FillAndKill/IOC)立即完成并且剩余撤销;2:(FillOrKill)立即完成全部或者最小订单要求否则全部撤销;3:需要进入订单簿被动成交,否则全部撤销(只做Maker/PostOnly);4:不需要撮合直接成交;
GTDTime | GT | no | double | GTD时间 | 
MinVolume | MV | no | double | 最小成交量 | 
BusinessType | BT | no | string | 业务类别 | 0:普通;1:申赎;2:行权;3:中立仓申报;4:递延交割申报;5:互换定单;6:质押;7:合并分拆;8:转股;9:回售回购;a:投票;b:融资融券;c:期权执行;d:传递被平仓订单BusinessValue;P:自定义11;Q:自定义10;R:自定义9;S:连续交易;T:自定义7;U:画线委托;V:限价市价;W:条件;X:止盈止损;Y:追踪出场;Z:指标类型;
BusinessValue | BV | no | string | 业务值 | 
CloseOrderID | CI | no | string | 平仓指定开仓的订单号 | 
IsCrossMargin | IsCrossMargin | no | int | 是否全仓 | 
Remark | R | no | string | 备注 | 
OrderID | OrderID | yes | string | 报单系统唯一代码 | 
CopyMemberID | CopyMemberID | no | string | 带单员代码 | 
CopyOrderID | CopyOrderID | no | string | 带单员报单号 | 
Leverage | Leverage | no | double | 委托单杠杆倍数 | 
CopyProfitRate | CopyProfitRate | no | double | 带单分配比例 | 
APPID | APPID | no | string | 应用编号 | 
PositionID | B | no | string | 持仓代码 | 
TriggerPrice | TriggerPrice | no | double | 触发价 | 
Reserve | Reserve | no | double | 保留资金 | 
SLPrice | SLPrice | no | double | 止损价 | 
SLTriggerPrice | SLTriggerPrice | no | double | 止损触发价 | 
TPPrice | TPPrice | no | double | 止盈价 | 
TPTriggerPrice | TPTriggerPrice | no | double | 止盈触发价 | 
RiskBefore | RiskBefore | no | int | 是否提前检查风控 | 
TriggerOrderType | TriggerOrderType | no | string | 触发的订单类型 | 0:未定义;1:持仓止盈止损;2:订单止盈止损;3:条件单;4:追踪(跟踪)订单;5:其他算法交易;6:指标类型;7:指标类型止盈止损;8:跟单类型;9:划线下单类型;a:其他类型1;b:其他类型2;c:其他类型3;d:其他类型4;e:其他类型5;
TriggerDetail | TriggerDetail | no | string | 触发类型明细 | 
TriggerPriceType | TriggerPriceType | no | string | 触发价类型 | 0:最新价;1:标记价;2:标的指数价;
TriggerValue | TriggerValue | no | string | 触发单具体设置信息 | 
CloseSLPrice | CloseSLPrice | no | double | 平仓止损价 | 
CloseSLTriggerPrice | CloseSLTriggerPrice | no | double | 平仓止损触发价 | 
CloseTPPrice | CloseTPPrice | no | double | 平仓止盈价 | 
CloseTPTriggerPrice | CloseTPTriggerPrice | no | double | 平仓止盈触发价 | 
CloseOrderPriceType | CloseOrderPriceType | no | string | 报单价格类型 | 0:限价(手动指定Price值);1:任意价(Price按最高最低限价取值);2:最优价/对手价(Price按对方1档价取值);3:五档价(Price按对方5档价取值);4:十档价(类同上);7:最优Marker价(Price按本方1档价或其+1Tick取值);8:Price按委托额Cost和订单簿情况确定值;9:保底点差价:成交价格Price不会劣于系统设置差价成交;
ClosePrice | ClosePrice | no | double | 平仓价 | 
CloseTriggerPrice | CloseTriggerPrice | no | double | 平仓触发价 | 
RelatedOrderID | RelatedOrderID | no | string | 关联报单号 | 
ActiveTime | ActiveTime | no | double | 激活时间 | 
TriggerTime | TriggerTime | no | double | 触发时间 | 
TimeSortNo | TimeSortNo | no | double | 按时间排队的序号 | 
TriggerStatus | TriggerStatus | no | string | 触发报单状态 | 0:未设置状态;1:活跃(运行)状态;2:已经触发;3:触发失败;4:撤单;
PosiDirection | PosiDirection | no | string | 持仓多空方向 | 0:多头;1:空头;2:净;
FrontNo | FrontNo | no | int | 前置编号 | 
ErrorNo | ErrorNo | no | int | 错误代码 | 
ErrorMsg | ErrorMsg | no | string | 错误信息 | 
InsertTime | InsertTime | no | double | 插入时间 | 
UpdateTime | UpdateTime | no | double | 更新时间 | 
BusinessNo | BusinessNo | no | double | 业务序列号 | 
CreateTime | CreateTime | no | string | 创建时间 | 
## **持仓订单组成**
+ **Request URL**  

```
 插入操作请求:  POST /action/v1.0/SendInsert/PositionOrder
 更新操作请求:  POST /action/v1.0/SendUpdate/PositionOrder
 删除操作请求:  POST /action/v1.0/SendDelete/PositionOrder
 插入没有就更新请求:  POST /action/v1.0/SendInsUpd/PositionOrder
 查询唯一索引记录请求:  POST /action/v1.0/SendQryOne/PositionOrder
 设置字段值清空请求:  POST /action/v1.0/SendClear/PositionOrder
 初始化操作请求:  POST /action/v1.0/SendInit/PositionOrder
```

+ **Request**

```
{
	MemberID:"1983723"	
	,TradeUnitID:"1983723"	
	,AccountID:"1983723"	
	,LocalID:"1983723"	
	,ExchangeID:"CEX"	
	,InstrumentID:"BTCUSD"	
	,OrderPriceType:"1"	
	,Direction:"1"	
	,OffsetFlag:"1"	
	,Price:182582.18	
	,Volume:182582.18	
	,VolumeDisplay:182582.18	
	,VolumeMode:"1"	
	,Cost:182582.18	
	,OrderType:"1"	
	,GTDTime:1657543832	
	,MinVolume:182582.18	
	,BusinessType:"1"	
	,BusinessValue:""	
	,CloseOrderID:"1983723"	
	,IsCrossMargin:1	
	,Remark:"my_little_tips"	
	,OrderID:"12376422300333"	
	,CopyMemberID:"1983723"	
	,CopyOrderID:"1983723"	
	,Leverage:125	
	,CopyProfitRate:182582.18	
	,APPID:"ios"	
	,PositionID:"1983723"	
	,TriggerPrice:182582.18	
	,Reserve:182582.18	
	,OrderStatus:"1"	
	,DeriveSource:"1"	
	,DeriveDetail:""	
	,VolumeTraded:182582.18	
	,VolumeRemain:182582.18	
	,VolumeCancled:182582.18	
	,InsertTime:1657543832	
	,UpdateTime:1657543832	
	,Priority:0	
	,TimeSortNo:1657543832	
	,FrontNo:1	
	,PriceCurrency:"BTC"	
	,FeeCurrency:"BTC"	
	,ClearCurrency:"BTC"	
	,FrozenMoney:182582.18	
	,FrozenFee:182582.18	
	,FrozenMargin:182582.18	
	,Fee:0.22364	
	,CloseProfit:10	
	,Turnover:182582.18	
	,RelatedOrderID:"1983723"	
	,BusinessResult:""	
	,BusinessNo:11176422300333	
	,Tradable:0	
	,SettlementGroup:"999"	
	,PosiDirection:"1"	
	,TradePrice:182582.18	
	,OpenPrice:182582.18	
	,TriggerOrderID:"1983723"	
	,SLTriggerPrice:182582.18	
	,TPTriggerPrice:182582.18	
	,CopyProfit:182582.18	
	,Position:182582.18	
	,UserID:"1983723"	
	,LastPriceByInsert:182582.18	
	,BidPrice1ByInsert:182582.18	
	,AskPrice1ByInsert:182582.18	
	,Available:182582.18	
	,CreateTime:""	
	,VolumeToClose:182582.18	
	,VolumeClosed:182582.18	
	,VolumeOnClose:182582.18	
}
```

+ **Response**

```
{
	"action": "Recv*",
	"errorCode": 0,
	"errorMsg": "Success",
	"result": [
		{"table":"PositionOrder",
		"data":{	
			MemberID:"1983723"
			,TradeUnitID:"1983723"
			,AccountID:"1983723"
			,LocalID:"1983723"
			,ExchangeID:"CEX"
			,InstrumentID:"BTCUSD"
			,OrderPriceType:"1"
			,Direction:"1"
			,OffsetFlag:"1"
			,Price:182582.18
			,Volume:182582.18
			,VolumeDisplay:182582.18
			,VolumeMode:"1"
			,Cost:182582.18
			,OrderType:"1"
			,GTDTime:1657543832
			,MinVolume:182582.18
			,BusinessType:"1"
			,BusinessValue:""
			,CloseOrderID:"1983723"
			,IsCrossMargin:1
			,Remark:"my_little_tips"
			,OrderID:"12376422300333"
			,CopyMemberID:"1983723"
			,CopyOrderID:"1983723"
			,Leverage:125
			,CopyProfitRate:182582.18
			,APPID:"ios"
			,PositionID:"1983723"
			,TriggerPrice:182582.18
			,Reserve:182582.18
			,OrderStatus:"1"
			,DeriveSource:"1"
			,DeriveDetail:""
			,VolumeTraded:182582.18
			,VolumeRemain:182582.18
			,VolumeCancled:182582.18
			,InsertTime:1657543832
			,UpdateTime:1657543832
			,Priority:0
			,TimeSortNo:1657543832
			,FrontNo:1
			,PriceCurrency:"BTC"
			,FeeCurrency:"BTC"
			,ClearCurrency:"BTC"
			,FrozenMoney:182582.18
			,FrozenFee:182582.18
			,FrozenMargin:182582.18
			,Fee:0.22364
			,CloseProfit:10
			,Turnover:182582.18
			,RelatedOrderID:"1983723"
			,BusinessResult:""
			,BusinessNo:11176422300333
			,Tradable:0
			,SettlementGroup:"999"
			,PosiDirection:"1"
			,TradePrice:182582.18
			,OpenPrice:182582.18
			,TriggerOrderID:"1983723"
			,SLTriggerPrice:182582.18
			,TPTriggerPrice:182582.18
			,CopyProfit:182582.18
			,Position:182582.18
			,UserID:"1983723"
			,LastPriceByInsert:182582.18
			,BidPrice1ByInsert:182582.18
			,AskPrice1ByInsert:182582.18
			,Available:182582.18
			,CreateTime:""
			,VolumeToClose:182582.18
			,VolumeClosed:182582.18
			,VolumeOnClose:182582.18
		}
	}
}
```	

+ **Request & Response Parameters**

parameter | shortName | required | type | description | remark
-------- | -------- | -------- | -------- | -------- | --------
MemberID | M | no | string | 成员代码 | 
TradeUnitID | T | no | string | 交易单元代码 | 
AccountID | A | no | string | 资金账号 | 
LocalID | L | no | string | 报单本地标识 | 
ExchangeID | E | no | string | 交易所代码 | 
InstrumentID | I | no | string | 标的代码 | 
OrderPriceType | PT | no | string | 报单价格类型 | 0:限价(手动指定Price值);1:任意价(Price按最高最低限价取值);2:最优价/对手价(Price按对方1档价取值);3:五档价(Price按对方5档价取值);4:十档价(类同上);7:最优Marker价(Price按本方1档价或其+1Tick取值);8:Price按委托额Cost和订单簿情况确定值;9:保底点差价:成交价格Price不会劣于系统设置差价成交;
Direction | D | no | string | 买卖方向 | 0:买;1:卖;
OffsetFlag | O | no | string | 开平标志 | 0:开仓;1:平仓;2:强平;3:平今;4:平昨;5:全平;6:指定订单平仓;7:指定成交平仓;8:最大平仓;
Price | P | no | double | 报单价格 | 
Volume | V | no | double | 数量 | 
VolumeDisplay | D | no | double | 显示数量 | 
VolumeMode | VM | no | string | 数量取值方式 | 0:百分比;1:绝对值;
Cost | C | no | double | 委托额 | 
OrderType | OT | no | string | 订单类型 | 0:普通订单;1:(FillAndKill/IOC)立即完成并且剩余撤销;2:(FillOrKill)立即完成全部或者最小订单要求否则全部撤销;3:需要进入订单簿被动成交,否则全部撤销(只做Maker/PostOnly);4:不需要撮合直接成交;
GTDTime | GT | no | double | GTD时间 | 
MinVolume | MV | no | double | 最小成交量 | 
BusinessType | BT | no | string | 业务类别 | 0:普通;1:申赎;2:行权;3:中立仓申报;4:递延交割申报;5:互换定单;6:质押;7:合并分拆;8:转股;9:回售回购;a:投票;b:融资融券;c:期权执行;d:传递被平仓订单BusinessValue;P:自定义11;Q:自定义10;R:自定义9;S:连续交易;T:自定义7;U:画线委托;V:限价市价;W:条件;X:止盈止损;Y:追踪出场;Z:指标类型;
BusinessValue | BV | no | string | 业务值 | 
CloseOrderID | CI | no | string | 平仓指定开仓的订单号 | 
IsCrossMargin | IsCrossMargin | no | int | 是否全仓 | 
Remark | R | no | string | 备注 | 
OrderID | OrderID | yes | string | 报单系统唯一代码 | 
CopyMemberID | CopyMemberID | no | string | 带单员代码 | 
CopyOrderID | CopyOrderID | no | string | 带单员报单号 | 
Leverage | Leverage | no | double | 委托单杠杆倍数 | 
CopyProfitRate | CopyProfitRate | no | double | 带单分配比例 | 
APPID | APPID | no | string | 应用编号 | 
PositionID | B | no | string | 持仓代码 | 
TriggerPrice | TriggerPrice | no | double | 触发价 | 
Reserve | Reserve | no | double | 保留资金 | 
OrderStatus | OrderStatus | no | string | 报单状态 | 0:未设置状态;1:全部成交;2:部分成交未撤单;3:部分成交已撤单;4:未成交未撤单;6:无成交已撤单;
DeriveSource | DeriveSource | no | string | 衍生来源 | 0:正常;1:报价衍生;2:期权执行;3:组合衍生;4:场外成交强平衍生;5:场外成交大宗交易衍生;6:场外成交期转现衍生;7:场外成交只刷单衍生;8:触发订单衍生;9:止损触发订单衍生;a:止盈触发订单衍生;b:资金操作衍生;c:仓位合并衍生;d:CFD订单衍生;e:场外成交ADL衍生;f:回购手续费不能是币;
DeriveDetail | DeriveDetail | no | string | 衍生明细 | 
VolumeTraded | VolumeTraded | no | double | 成交数量 | 
VolumeRemain | VolumeRemain | no | double | 剩余数量 | 
VolumeCancled | VolumeCancled | no | double | 已经撤单数量 | 
InsertTime | InsertTime | no | double | 插入时间 | 
UpdateTime | UpdateTime | no | double | 更新时间 | 
Priority |  | no | int | 优先权 | 
TimeSortNo |  | no | double | 按时间排队的序号 | 
FrontNo | FrontNo | no | int | 前置编号 | 
PriceCurrency | PriceCurrency | no | string | 计价币种 | 
FeeCurrency | FeeCurrency | no | string | 手续费币种 | 
ClearCurrency | ClearCurrency | no | string | 清算币种 | 
FrozenMoney | FrozenMoney | no | double | 冻结资金 | 
FrozenFee | FrozenFee | no | double | 冻结手续费 | 
FrozenMargin | FrozenMargin | no | double | 冻结保证金 | 
Fee | Fee | no | double | 手续费 | 
CloseProfit | CloseProfit | no | double | 平仓盈亏 | 
Turnover | Turnover | no | double | 成交金额 | 
RelatedOrderID | RelatedOrderID | no | string | 关联报单号 | 
BusinessResult | BusinessResult | no | string | 业务执行结果 | 
BusinessNo | BusinessNo | no | double | 业务序列号 | 
Tradable |  | no | int | 是否可以参加交易 | 
SettlementGroup |  | no | string | 结算组编号 | 
PosiDirection | PosiDirection | no | string | 持仓多空方向 | 0:多头;1:空头;2:净;
TradePrice | TradePrice | no | double | 成交均价 | 
OpenPrice | OpenPrice | no | double | 平仓成交时的开仓均价 | 
TriggerOrderID | TriggerOrderID | no | string | 触发报单号 | 
SLTriggerPrice | SLTriggerPrice | no | double | 开仓报单成交之后的止损触发价 | 
TPTriggerPrice | TPTriggerPrice | no | double | 开仓报单成交之后的止盈触发价 | 
CopyProfit | CopyProfit | no | double | 带单盈利分配 | 
Position | Position | no | double | 成交时的持仓量 | 
UserID | U | no | string | 交易用户代码 | 
LastPriceByInsert |  | no | double | 插入时的最新价 | 
BidPrice1ByInsert |  | no | double | 插入时的买一价 | 
AskPrice1ByInsert |  | no | double | 插入时的卖一价 | 
Available | Available | no | double | 可用资金 | 
CreateTime |  | no | string | 创建时间 | 
VolumeToClose | VolumeToClose | no | double | 需要平仓的数量 | 
VolumeClosed | VolumeClosed | no | double | 已经平仓的数量 | 
VolumeOnClose | VolumeOnClose | no | double | 正在平仓未成交的数量 | 

# **错误码**

## **错误码说明**
error code | description
-------- | --------
0 | None
1 | SystemNotReady
2 | RecordNotFound
3 | RecordExist
4 | InvalidAction
5 | InvalidValue
6 | InvalidStatus
7 | InvalidSession
8 | InstrumentNotFound
9 | MemberNotFound
11 | MarketDataNotFound
12 | BadField
14 | DuplicateAction
18 | MarketOrderCanNotQueue
20 | GtdOrderExpired
21 | MvOrderExceedVolumn
22 | OrderExist
24 | OrderNotFound
25 | QuoteNotFound
26 | InvalidInstrumentStatus
27 | InvalidInstrumentStatusShift
30 | NotEnoughVolumeToModify
31 | NotEnoughPositionToClose
32 | PositionLimit
33 | AccountLessThanZeroAfterClose
34 | MemberPositionLimit
35 | AccountNotFound
36 | InsufficientMoney
37 | InvalidVolume
44 | VolumeNotOnTick
48 | PriceNotOnTick
49 | PriceOutOfUpperLimit
50 | PriceOutOfLowerLimit
51 | NoTradingRight
52 | CloseOnly
54 | UserNotLogin
56 | OnlyTradeManagerCanDoThis
58 | UserMismatch
59 | UserRelogin
60 | InvalidUserOrPassword
62 | UserNotActive
65 | InvalidLoginIpaddress
71 | CanNotOperateDerivedOrder
76 | OrderHadSuspended
77 | OrderHadActived
78 | GtdOrderDateMissed
79 | UnSupportOrderType
80 | UserNoPermission
88 | UserNotFound
99 | CanNotActionForOtherUser
100 | NotEnoughMargin
118 | CombinationOnSingleInstrument
139 | OtcTypeError
172 | NotEnoughLeverage
175 | PriceMustGreaterThanZero
176 | InvalidAPIKey
177 | APIKeyExpired
178 | APIKeyLimitExceed
179 | KeyIsNull
180 | CanNotFoundMarginRate
181 | DuplicateAPIKey
182 | NoLimitPrice
183 | ExceedMaxQueryCountPerSecond
184 | OrderLimitExceed
185 | NotEnoughOpenorders
186 | SessionNotFound
187 | PriceOutOfAskPrice1
188 | PriceOutOfBidPrice1
189 | PositionExist
190 | UnderlyingPriceError
191 | RecordAnalysisError
192 | DuplicateRecord
193 | OutOfMaxVolume
194 | LessThanMinVolume
195 | PositionLessThanMinVolume
196 | TradeForbidden
197 | CanNotFoundFee
198 | PositionCountLimitExceed
199 | LeverageTooHigh
200 | PositionUnderDeficit
201 | CanNotChangePosType
