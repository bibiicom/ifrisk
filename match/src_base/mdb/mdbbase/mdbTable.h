/////////////////////////////////////////////////////////////////////////
///mdbTable.h
///定义了若干个内存数据库的对象工厂
/////////////////////////////////////////////////////////////////////////

#ifndef DATABASEFACTORIES_H
#define DATABASEFACTORIES_H

#include "platform.h"
#include "CBaseObject.h"
#include "CAVLTree.h"
#include "CHashIndex.h"
#include "DataType.h"
#include "CDatabaseDataFactory.h"
#include "CTransaction.h"
#include "mdbStruct.h"
#include "CDatabaseResource.h"
#include "Allocator.h"
#include "FieldData.h"
#include "mdb.h"

class CCurrentTimeFactory;
class CCurrentTimeResource;
class CMemberFactory;
class CMemberResource;
class CServiceConfigFactory;
class CServiceConfigResource;
class CUserSessionFactory;
class CUserSessionResource;
class CInstrumentFactory;
class CInstrumentResource;
class CTradingRightFactory;
class CTradingRightResource;
class CMarginRateFactory;
class CMarginRateResource;
class CCFDRateFactory;
class CCFDRateResource;
class CFeeFactory;
class CFeeResource;
class CCurrencyFactory;
class CCurrencyResource;
class CLiquidityFactory;
class CLiquidityResource;
class CAccountFactory;
class CAccountResource;
class CMemberPositionFactory;
class CMemberPositionResource;
class CPositionFactory;
class CPositionResource;
class CMarketDataFactory;
class CMarketDataResource;
class COrderFactory;
class COrderResource;
class CPositionOrderFactory;
class CPositionOrderResource;
class CTriggerOrderFactory;
class CTriggerOrderResource;
class CMarketOrderFactory;
class CMarketOrderResource;
class CTickMarketOrderFactory;
class CTickMarketOrderResource;
class CTradeFactory;
class CTradeResource;
class CKLineFactory;
class CKLineResource;
class CPublishKLineFactory;
class CPublishKLineResource;
class CLastKLineFactory;
class CLastKLineResource;
class CAccountDetailFactory;
class CAccountDetailResource;
class CSettleDetailFactory;
class CSettleDetailResource;
class COrderActionFactory;
class COrderActionResource;

class CMDB;

/////////////////////////////////////////////////////////////////////////
///CCurrentTimeFactory是一个当前时间的对象工厂。它包含了一批当前时间，
///同时又建立了如下索引，以方便查询：
///	SystemIDIndex
///使用本对象工厂，可以完成对当前时间的增删改查操作。具体方法如下：
///	增加：叫用add，完成加入
///	删除：叫用remove就可以删除
///	查询：有如下方法可以查询：
///		使用findBySystemID，唯一找到对象
///	修改：对于查到的结果，可以通过retrieve取出，修改后，通过update放回
///	事务管理：所有的增加、删除和修改都可以指定事务，在事务commit时，真正写入
///	在retrieve时，也可以指定事务，此时将取出经过update的新数据
/////////////////////////////////////////////////////////////////////////
class CCurrentTimeFactory :public CDatabaseDataFactory
{
public:
	///构造方法
	///@param	maxUnit	初始的数据对象个数
	///@param	pAllocator	进行内存管理的分配器
	///@param	pIndexMap	索引名称和内存编号的对应表，在非重用时会写入，在重用时只读出
	///@param	reuse	是否重用
	///@param	reuseID	重用编号，非重用时应当置为-1
	///@param	readOnly	1表示该CFixMem是只读的，0表示可以读写
	CCurrentTimeFactory(CMDB* pMDB,int maxUnit,IMemoryAllocator *pAllocator,map<string,int>* pIndexMap,bool reuse=false, int reuseID=-1, int readOnly=0);
	
	///构造方法
	///@param	pAllocator	进行内存管理的分配器
	///@param	pIndexMap	索引名称和内存编号的对应表，在非重用时会写入，在重用时只读出
	///@param	reuse	是否重用
	///@param	reuseID	重用编号，非重用时应当置为-1
	///@param	readOnly	1表示该CFixMem是只读的，0表示可以读写
	CCurrentTimeFactory(CMDB* pMDB,IMemoryAllocator *pAllocator,map<string,int>* pIndexMap,bool reuse=false, int reuseID = -1,int readOnly=0);
	
	///析构方法，将负责释放所占用的空间
	virtual ~CCurrentTimeFactory();

	///从CSV文件中读取所有的CurrentTime
	///@param	filename	文件名
	///@return	1表示完成，0表示没有完成
	virtual int readCSV(char *filename);

	///从CSV文件中读取所有的CurrentTime，文件名将根据配置文件中的定义CurrentTimeCSVFile
	///@return	1表示完成，0表示没有完成
	virtual int readCSVByPath(char *pPath);
	
	///向CSV文件中写出所有的CurrentTime
	///@param	filename	文件名
	///@return	1表示完成，0表示没有完成
	virtual int writeCSV(char *filename);
	
	///向CSV文件中写出上一个版本的CurrentTime
	///@param	filename	文件名
	///@return	1表示完成，0表示没有完成
	virtual int writeCSVPreVersion(char *filename);	

	///向CSV文件中写出所有的CurrentTime，文件名将根据配置文件中的定义CurrentTimeCSVFile
	///@return	1表示完成，0表示没有完成
	virtual int writeCSVByPath(char *pPath);

	///将内容写到文件中,用于DEBUG
	virtual void dump(FILE *);

	///将内容写到文件中,用于DEBUG
	virtual void dump(char *filename);

	///清空所有的记录
	virtual void clearAll();

	friend class CCurrentTimeResource;

	///将一个CCurrentTime加入到该对象工厂，实质上是完成修改索引的工作
	///@param	pCurrentTime	要加入的CurrentTime
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	///@return	加入后该对象的位置，当pTransaction!=NULL时，总是返回NULL
	CCurrentTime* add(CWriteableCurrentTime *pCurrentTime, CTransaction *pTransaction=NULL);
	
	///刷新该CCurrentTime的值
	///@param	pCurrentTime	被刷新的CCurrentTime
	///@param	pNewCurrentTime	新的值
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	CCurrentTime* update(CCurrentTime *pCurrentTime, CWriteableCurrentTime *pNewCurrentTime, CTransaction *pTransaction=NULL, bool bNoTransaction = true);
	
	///增加或者更新数据
	///@param	pCurrentTime	  需要被刷新或者新增的CCurrentTime,等于NULL表示是需要新增的
	///@param	pNewCurrentTime	新的值
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	///@param	新增或者修改后的数据
	CCurrentTime* addOrUpdate(CCurrentTime *pCurrentTime, CWriteableCurrentTime *pNewCurrentTime, CTransaction *pTransaction=NULL);
	
	///删除一个CCurrentTime，同时删除其索引
	///@param	pCurrentTime	要删除的CCurrentTime
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	void remove(CCurrentTime *pCurrentTime, CTransaction *pTransaction=NULL, bool bNoTransaction=true);
	
	///获取某个CCurrentTime
	///@param	pCurrentTime	要读取的CCurrentTime
	///@param	pTragetCurrentTime	存放结果的CCurrentTime
	void retrieve(CCurrentTime *pCurrentTime, CWriteableCurrentTime *pTargetCurrentTime);
	
	///获取第一个CCurrentTime
	///@return	得到的第一个CCurrentTime，如果没有，则返回NULL
	CCurrentTime* getFirst();
	
	///获取下一个CCurrentTime
	///@return	得到下一个CCurrentTime，如果没有，则返回NULL
	CCurrentTime* getNext();
	
	///加入一个操作触发器
	///@param	pTrigger	要加入的操作触发器
	///@return	1表示成功，0表示失败
	int addActionTrigger(CCurrentTimeActionTrigger *pTrigger);
	
	///删除一个操作触发器
	///@param	pTrigger	要删除的操作触发器
	///@return	1表示成功，0表示失败
	int removeActionTrigger(CCurrentTimeActionTrigger *pTrigger);

	///加入一个确认触发器
	///@param	pTrigger	要加入的确认触发器
	///@return	1表示成功，0表示失败
	int addCommitTrigger(CCurrentTimeCommitTrigger *pTrigger);
	
	///删除一个确认触发器
	///@param	pTrigger	要删除的确认触发器
	///@return	1表示成功，0表示失败
	int removeCommitTrigger(CCurrentTimeCommitTrigger *pTrigger);
	
	///检查是否字段为NULL
	///@param pFile 输出错误日志的文件
	void checkNull(FILE *pFile);
	
	///检查是否字段合法
	///@param pFile 输出错误日志的文件
	void checkValid(FILE *pFile);	

	///寻找CCurrentTime
	///@return	找到的CCurrentTime，如果找不到，返回NULL
	CCurrentTime *findBySystemID(const CReadOnlyShortIndexIDType&  SystemID);

	///根据主码寻找
	///@return	找到的CCurrentTime，如果找不到，返回NULL
	CCurrentTime *findByPK(const CReadOnlyShortIndexIDType& SystemID);
		
	///根据主键删除CCurrentTime
	bool removeByPK(const CReadOnlyShortIndexIDType& SystemID, CTransaction *pTransaction=NULL);
	
	///判断字段是否是是主键
	bool isPK(const char* pName);
	
	///根据主键寻找CCurrentTime
	///@param	pCurrentTime	要找的值
	///@return	找到的CCurrentTime，如果找不到，返回NULL
	CCurrentTime *findByPK(CCurrentTimeField *pCurrentTime);
	
	///根据主键寻找CCurrentTime
	///@param	pCurrentTime	要找的值
	///@return	找到的CCurrentTime，如果找不到，返回NULL
	CCurrentTime *findByPK(CCurrentTime *pCurrentTime);

	///根据主键获取CCurrentTime
	///@param	pCurrentTime	要获取的值，同时用于存放返回的结果
	///@return	找到的CCurrentTime，如果找不到，返回NULL
	bool retrieveByPK(CWriteableCurrentTime *pCurrentTime);

	///根据主键增加新的CCurrentTime
	///@param	pCurrentTime	新的值
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	///@return	返回插入的CCurrentTime，如果插入失败，则返回NULL
	CCurrentTime *addByPK(CWriteableCurrentTime *pCurrentTime, CTransaction *pTransaction=NULL);
	
	///根据主键增加或更新新的CCurrentTime
	///@param	pCurrentTime	新的值
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	///@return	返回插入的CCurrentTime，如果插入失败，则返回NULL
	CCurrentTime *addOrUpdateByPK(CWriteableCurrentTime *pCurrentTime, CTransaction *pTransaction=NULL);

	///根据主键更新CCurrentTime的值
	///@param	pCurrentTime	新的值
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	///@return	true表示成功，false表示失败
	bool updateByPK(CWriteableCurrentTime *pCurrentTime, CTransaction *pTransaction=NULL);

	///根据主键删除CCurrentTime
	///@param	pCurrentTime	要删除的值
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	///@return	true表示成功，false表示失败
	bool removeByPK(CCurrentTime *pCurrentTime, CTransaction *pTransaction=NULL);
	
	///根据主键删除CCurrentTime
	///@param	pCurrentTime	要删除的值
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	///@return	true表示成功，false表示失败
	bool removeByPK(CCurrentTimeField *pCurrentTime, CTransaction *pTransaction=NULL);	


private:
	///完成自己的初始化工作
	///@param	maxUnit	最多的数据对象个数
	///@param	pAllocator	进行内存管理的分配器
	///@param	pIndexMap	索引名称和内存编号的对应表，在非重用时会写入，在重用时只读出
	///@param	reuse	是否重用
	///@param	reuseID	重用编号，非重用时应当置为-1
	///@param	readOnly	1表示该CFixMem是只读的，0表示可以读写
	void init(int maxUnit,IMemoryAllocator *pAllocator,map<string,int>* pIndexMap,bool reuse,int reuseID,int readOnly);
	
	///将一个CCurrentTime加入到该对象工厂，实质上是完成修改索引的工作
	///@param	pCurrentTime	要加入的CurrentTime
	///@param	bNoTransaction 是否不属于任何事务
	///@return	加入后该对象的位置，当pTransaction!=NULL时，总是返回NULL
	CCurrentTime* internalAdd(CWriteableCurrentTime *pCurrentTime, bool bNoTransaction);
	
	
	///刷新该CCurrentTime的键值
	///@param	pCurrentTime	被刷新的CCurrentTime
	///@param	pNewCurrentTime	新的值
	///@param	bNoTransaction 是否不属于任何事务
	CCurrentTime* internalUpdate(CCurrentTime *pCurrentTime, CWriteableCurrentTime *pNewCurrentTime, bool bNoTransaction);
	
	///删除一个CCurrentTime，同时删除其索引
	///@param	pCurrentTime	要删除的CCurrentTime
	///@param	bNoTransaction 是否不属于任何事务
	void internalRemove(CCurrentTime *pCurrentTime, bool bNoTransaction);
/*
	///检查某个CCurrentTime是否属于本对象工厂
	///@param	pCurrentTime	要检查的对象
	///@exception	CDesignError	如果不属于，则抛出此异常
	void checkAddress(CCurrentTime *pCurrentTime);
*/

	///基于SystemID的索引
	CAVLTree *pSystemIDIndex;
	
	
		
	///确认加入后触发
	///@param	pCurrentTime	已经加入的CurrentTime
	virtual void commitAdd(CCurrentTime *pCurrentTime);

	///确认更新后触发
	///@param	pCurrentTime	被刷新的CCurrentTime
	///@param	poldCurrentTime	原来的值
	virtual void commitUpdate(CCurrentTime *pCurrentTime, CWriteableCurrentTime *pOldCurrentTime);
	
	///确认删除后触发
	///@param	pCurrentTime	已经删除的CCurrentTime
	virtual void commitRemove(CWriteableCurrentTime *pCurrentTime);

	///所有相关的触发器
	vector<CCurrentTimeActionTrigger *> *pActionTriggers;
	vector<CCurrentTimeCommitTrigger *> *pCommitTriggers;
	
	///进行查询时，用于比较的对象
	CWriteableCurrentTime compareObject;
	CMDB* m_pMDB;
};

/////////////////////////////////////////////////////////////////////////
///CMemberFactory是一个成员的对象工厂。它包含了一批成员，
///同时又建立了如下索引，以方便查询：
///使用本对象工厂，可以完成对成员的增删改查操作。具体方法如下：
///	增加：叫用add，完成加入
///	删除：叫用remove就可以删除
///	查询：有如下方法可以查询：
///		使用findByMemberID，唯一找到对象
///	修改：对于查到的结果，可以通过retrieve取出，修改后，通过update放回
///	事务管理：所有的增加、删除和修改都可以指定事务，在事务commit时，真正写入
///	在retrieve时，也可以指定事务，此时将取出经过update的新数据
/////////////////////////////////////////////////////////////////////////
class CMemberFactory :public CDatabaseDataFactory
{
public:
	///构造方法
	///@param	maxUnit	初始的数据对象个数
	///@param	pAllocator	进行内存管理的分配器
	///@param	pIndexMap	索引名称和内存编号的对应表，在非重用时会写入，在重用时只读出
	///@param	reuse	是否重用
	///@param	reuseID	重用编号，非重用时应当置为-1
	///@param	readOnly	1表示该CFixMem是只读的，0表示可以读写
	CMemberFactory(CMDB* pMDB,int maxUnit,IMemoryAllocator *pAllocator,map<string,int>* pIndexMap,bool reuse=false, int reuseID=-1, int readOnly=0);
	
	///构造方法
	///@param	pAllocator	进行内存管理的分配器
	///@param	pIndexMap	索引名称和内存编号的对应表，在非重用时会写入，在重用时只读出
	///@param	reuse	是否重用
	///@param	reuseID	重用编号，非重用时应当置为-1
	///@param	readOnly	1表示该CFixMem是只读的，0表示可以读写
	CMemberFactory(CMDB* pMDB,IMemoryAllocator *pAllocator,map<string,int>* pIndexMap,bool reuse=false, int reuseID = -1,int readOnly=0);
	
	///析构方法，将负责释放所占用的空间
	virtual ~CMemberFactory();

	///从CSV文件中读取所有的Member
	///@param	filename	文件名
	///@return	1表示完成，0表示没有完成
	virtual int readCSV(char *filename);

	///从CSV文件中读取所有的Member，文件名将根据配置文件中的定义MemberCSVFile
	///@return	1表示完成，0表示没有完成
	virtual int readCSVByPath(char *pPath);
	
	///向CSV文件中写出所有的Member
	///@param	filename	文件名
	///@return	1表示完成，0表示没有完成
	virtual int writeCSV(char *filename);
	
	///向CSV文件中写出上一个版本的Member
	///@param	filename	文件名
	///@return	1表示完成，0表示没有完成
	virtual int writeCSVPreVersion(char *filename);	

	///向CSV文件中写出所有的Member，文件名将根据配置文件中的定义MemberCSVFile
	///@return	1表示完成，0表示没有完成
	virtual int writeCSVByPath(char *pPath);

	///将内容写到文件中,用于DEBUG
	virtual void dump(FILE *);

	///将内容写到文件中,用于DEBUG
	virtual void dump(char *filename);

	///清空所有的记录
	virtual void clearAll();

	friend class CMemberResource;

	///将一个CMember加入到该对象工厂，实质上是完成修改索引的工作
	///@param	pMember	要加入的Member
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	///@return	加入后该对象的位置，当pTransaction!=NULL时，总是返回NULL
	CMember* add(CWriteableMember *pMember, CTransaction *pTransaction=NULL);
	
	///刷新该CMember的值
	///@param	pMember	被刷新的CMember
	///@param	pNewMember	新的值
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	CMember* update(CMember *pMember, CWriteableMember *pNewMember, CTransaction *pTransaction=NULL, bool bNoTransaction = true);
	
	///增加或者更新数据
	///@param	pMember	  需要被刷新或者新增的CMember,等于NULL表示是需要新增的
	///@param	pNewMember	新的值
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	///@param	新增或者修改后的数据
	CMember* addOrUpdate(CMember *pMember, CWriteableMember *pNewMember, CTransaction *pTransaction=NULL);
	
	///删除一个CMember，同时删除其索引
	///@param	pMember	要删除的CMember
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	void remove(CMember *pMember, CTransaction *pTransaction=NULL, bool bNoTransaction=true);
	
	///获取某个CMember
	///@param	pMember	要读取的CMember
	///@param	pTragetMember	存放结果的CMember
	void retrieve(CMember *pMember, CWriteableMember *pTargetMember);
	
	///获取第一个CMember
	///@return	得到的第一个CMember，如果没有，则返回NULL
	CMember* getFirst();
	
	///获取下一个CMember
	///@return	得到下一个CMember，如果没有，则返回NULL
	CMember* getNext();
	
	///加入一个操作触发器
	///@param	pTrigger	要加入的操作触发器
	///@return	1表示成功，0表示失败
	int addActionTrigger(CMemberActionTrigger *pTrigger);
	
	///删除一个操作触发器
	///@param	pTrigger	要删除的操作触发器
	///@return	1表示成功，0表示失败
	int removeActionTrigger(CMemberActionTrigger *pTrigger);

	///加入一个确认触发器
	///@param	pTrigger	要加入的确认触发器
	///@return	1表示成功，0表示失败
	int addCommitTrigger(CMemberCommitTrigger *pTrigger);
	
	///删除一个确认触发器
	///@param	pTrigger	要删除的确认触发器
	///@return	1表示成功，0表示失败
	int removeCommitTrigger(CMemberCommitTrigger *pTrigger);
	
	///检查是否字段为NULL
	///@param pFile 输出错误日志的文件
	void checkNull(FILE *pFile);
	
	///检查是否字段合法
	///@param pFile 输出错误日志的文件
	void checkValid(FILE *pFile);	

	///寻找CMember
	///@return	找到的CMember，如果找不到，返回NULL
	CMember *findByMemberID(const CReadOnlyLongIndexIDType&  MemberID);

	///根据主码寻找
	///@return	找到的CMember，如果找不到，返回NULL
	CMember *findByPK(const CReadOnlyLongIndexIDType& MemberID);
		
	///根据主键删除CMember
	bool removeByPK(const CReadOnlyLongIndexIDType& MemberID, CTransaction *pTransaction=NULL);
	
	///判断字段是否是是主键
	bool isPK(const char* pName);
	
	///根据主键寻找CMember
	///@param	pMember	要找的值
	///@return	找到的CMember，如果找不到，返回NULL
	CMember *findByPK(CMemberField *pMember);
	
	///根据主键寻找CMember
	///@param	pMember	要找的值
	///@return	找到的CMember，如果找不到，返回NULL
	CMember *findByPK(CMember *pMember);

	///根据主键获取CMember
	///@param	pMember	要获取的值，同时用于存放返回的结果
	///@return	找到的CMember，如果找不到，返回NULL
	bool retrieveByPK(CWriteableMember *pMember);

	///根据主键增加新的CMember
	///@param	pMember	新的值
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	///@return	返回插入的CMember，如果插入失败，则返回NULL
	CMember *addByPK(CWriteableMember *pMember, CTransaction *pTransaction=NULL);
	
	///根据主键增加或更新新的CMember
	///@param	pMember	新的值
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	///@return	返回插入的CMember，如果插入失败，则返回NULL
	CMember *addOrUpdateByPK(CWriteableMember *pMember, CTransaction *pTransaction=NULL);

	///根据主键更新CMember的值
	///@param	pMember	新的值
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	///@return	true表示成功，false表示失败
	bool updateByPK(CWriteableMember *pMember, CTransaction *pTransaction=NULL);

	///根据主键删除CMember
	///@param	pMember	要删除的值
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	///@return	true表示成功，false表示失败
	bool removeByPK(CMember *pMember, CTransaction *pTransaction=NULL);
	
	///根据主键删除CMember
	///@param	pMember	要删除的值
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	///@return	true表示成功，false表示失败
	bool removeByPK(CMemberField *pMember, CTransaction *pTransaction=NULL);	


private:
	///完成自己的初始化工作
	///@param	maxUnit	最多的数据对象个数
	///@param	pAllocator	进行内存管理的分配器
	///@param	pIndexMap	索引名称和内存编号的对应表，在非重用时会写入，在重用时只读出
	///@param	reuse	是否重用
	///@param	reuseID	重用编号，非重用时应当置为-1
	///@param	readOnly	1表示该CFixMem是只读的，0表示可以读写
	void init(int maxUnit,IMemoryAllocator *pAllocator,map<string,int>* pIndexMap,bool reuse,int reuseID,int readOnly);
	
	///将一个CMember加入到该对象工厂，实质上是完成修改索引的工作
	///@param	pMember	要加入的Member
	///@param	bNoTransaction 是否不属于任何事务
	///@return	加入后该对象的位置，当pTransaction!=NULL时，总是返回NULL
	CMember* internalAdd(CWriteableMember *pMember, bool bNoTransaction);
	
	
	///刷新该CMember的键值
	///@param	pMember	被刷新的CMember
	///@param	pNewMember	新的值
	///@param	bNoTransaction 是否不属于任何事务
	CMember* internalUpdate(CMember *pMember, CWriteableMember *pNewMember, bool bNoTransaction);
	
	///删除一个CMember，同时删除其索引
	///@param	pMember	要删除的CMember
	///@param	bNoTransaction 是否不属于任何事务
	void internalRemove(CMember *pMember, bool bNoTransaction);
/*
	///检查某个CMember是否属于本对象工厂
	///@param	pMember	要检查的对象
	///@exception	CDesignError	如果不属于，则抛出此异常
	void checkAddress(CMember *pMember);
*/

	
	///基于MemberID的hash索引
	CHashIndex *pMemberIDHashIndex;
	
		
	///确认加入后触发
	///@param	pMember	已经加入的Member
	virtual void commitAdd(CMember *pMember);

	///确认更新后触发
	///@param	pMember	被刷新的CMember
	///@param	poldMember	原来的值
	virtual void commitUpdate(CMember *pMember, CWriteableMember *pOldMember);
	
	///确认删除后触发
	///@param	pMember	已经删除的CMember
	virtual void commitRemove(CWriteableMember *pMember);

	///所有相关的触发器
	vector<CMemberActionTrigger *> *pActionTriggers;
	vector<CMemberCommitTrigger *> *pCommitTriggers;
	
	///进行查询时，用于比较的对象
	CWriteableMember compareObject;
	CMDB* m_pMDB;
};

/////////////////////////////////////////////////////////////////////////
///CServiceConfigFactory是一个业务配置的对象工厂。它包含了一批业务配置，
///同时又建立了如下索引，以方便查询：
///	ConfigIndex
///使用本对象工厂，可以完成对业务配置的增删改查操作。具体方法如下：
///	增加：叫用add，完成加入
///	删除：叫用remove就可以删除
///	查询：有如下方法可以查询：
///		使用findByConfig，唯一找到对象
///		使用startFindByIndex1，findNextByIndex1完成查询操作
///	修改：对于查到的结果，可以通过retrieve取出，修改后，通过update放回
///	事务管理：所有的增加、删除和修改都可以指定事务，在事务commit时，真正写入
///	在retrieve时，也可以指定事务，此时将取出经过update的新数据
/////////////////////////////////////////////////////////////////////////
class CServiceConfigFactory :public CDatabaseDataFactory
{
public:
	///构造方法
	///@param	maxUnit	初始的数据对象个数
	///@param	pAllocator	进行内存管理的分配器
	///@param	pIndexMap	索引名称和内存编号的对应表，在非重用时会写入，在重用时只读出
	///@param	reuse	是否重用
	///@param	reuseID	重用编号，非重用时应当置为-1
	///@param	readOnly	1表示该CFixMem是只读的，0表示可以读写
	CServiceConfigFactory(CMDB* pMDB,int maxUnit,IMemoryAllocator *pAllocator,map<string,int>* pIndexMap,bool reuse=false, int reuseID=-1, int readOnly=0);
	
	///构造方法
	///@param	pAllocator	进行内存管理的分配器
	///@param	pIndexMap	索引名称和内存编号的对应表，在非重用时会写入，在重用时只读出
	///@param	reuse	是否重用
	///@param	reuseID	重用编号，非重用时应当置为-1
	///@param	readOnly	1表示该CFixMem是只读的，0表示可以读写
	CServiceConfigFactory(CMDB* pMDB,IMemoryAllocator *pAllocator,map<string,int>* pIndexMap,bool reuse=false, int reuseID = -1,int readOnly=0);
	
	///析构方法，将负责释放所占用的空间
	virtual ~CServiceConfigFactory();

	///从CSV文件中读取所有的ServiceConfig
	///@param	filename	文件名
	///@return	1表示完成，0表示没有完成
	virtual int readCSV(char *filename);

	///从CSV文件中读取所有的ServiceConfig，文件名将根据配置文件中的定义ServiceConfigCSVFile
	///@return	1表示完成，0表示没有完成
	virtual int readCSVByPath(char *pPath);
	
	///向CSV文件中写出所有的ServiceConfig
	///@param	filename	文件名
	///@return	1表示完成，0表示没有完成
	virtual int writeCSV(char *filename);
	
	///向CSV文件中写出上一个版本的ServiceConfig
	///@param	filename	文件名
	///@return	1表示完成，0表示没有完成
	virtual int writeCSVPreVersion(char *filename);	

	///向CSV文件中写出所有的ServiceConfig，文件名将根据配置文件中的定义ServiceConfigCSVFile
	///@return	1表示完成，0表示没有完成
	virtual int writeCSVByPath(char *pPath);

	///将内容写到文件中,用于DEBUG
	virtual void dump(FILE *);

	///将内容写到文件中,用于DEBUG
	virtual void dump(char *filename);

	///清空所有的记录
	virtual void clearAll();

	friend class CServiceConfigResource;

	///将一个CServiceConfig加入到该对象工厂，实质上是完成修改索引的工作
	///@param	pServiceConfig	要加入的ServiceConfig
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	///@return	加入后该对象的位置，当pTransaction!=NULL时，总是返回NULL
	CServiceConfig* add(CWriteableServiceConfig *pServiceConfig, CTransaction *pTransaction=NULL);
	
	///刷新该CServiceConfig的值
	///@param	pServiceConfig	被刷新的CServiceConfig
	///@param	pNewServiceConfig	新的值
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	CServiceConfig* update(CServiceConfig *pServiceConfig, CWriteableServiceConfig *pNewServiceConfig, CTransaction *pTransaction=NULL, bool bNoTransaction = true);
	
	///增加或者更新数据
	///@param	pServiceConfig	  需要被刷新或者新增的CServiceConfig,等于NULL表示是需要新增的
	///@param	pNewServiceConfig	新的值
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	///@param	新增或者修改后的数据
	CServiceConfig* addOrUpdate(CServiceConfig *pServiceConfig, CWriteableServiceConfig *pNewServiceConfig, CTransaction *pTransaction=NULL);
	
	///删除一个CServiceConfig，同时删除其索引
	///@param	pServiceConfig	要删除的CServiceConfig
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	void remove(CServiceConfig *pServiceConfig, CTransaction *pTransaction=NULL, bool bNoTransaction=true);
	
	///获取某个CServiceConfig
	///@param	pServiceConfig	要读取的CServiceConfig
	///@param	pTragetServiceConfig	存放结果的CServiceConfig
	void retrieve(CServiceConfig *pServiceConfig, CWriteableServiceConfig *pTargetServiceConfig);
	
	///获取第一个CServiceConfig
	///@return	得到的第一个CServiceConfig，如果没有，则返回NULL
	CServiceConfig* getFirst();
	
	///获取下一个CServiceConfig
	///@return	得到下一个CServiceConfig，如果没有，则返回NULL
	CServiceConfig* getNext();
	
	///加入一个操作触发器
	///@param	pTrigger	要加入的操作触发器
	///@return	1表示成功，0表示失败
	int addActionTrigger(CServiceConfigActionTrigger *pTrigger);
	
	///删除一个操作触发器
	///@param	pTrigger	要删除的操作触发器
	///@return	1表示成功，0表示失败
	int removeActionTrigger(CServiceConfigActionTrigger *pTrigger);

	///加入一个确认触发器
	///@param	pTrigger	要加入的确认触发器
	///@return	1表示成功，0表示失败
	int addCommitTrigger(CServiceConfigCommitTrigger *pTrigger);
	
	///删除一个确认触发器
	///@param	pTrigger	要删除的确认触发器
	///@return	1表示成功，0表示失败
	int removeCommitTrigger(CServiceConfigCommitTrigger *pTrigger);
	
	///检查是否字段为NULL
	///@param pFile 输出错误日志的文件
	void checkNull(FILE *pFile);
	
	///检查是否字段合法
	///@param pFile 输出错误日志的文件
	void checkValid(FILE *pFile);	

	///寻找CServiceConfig
	///@return	找到的CServiceConfig，如果找不到，返回NULL
	CServiceConfig *findByConfig(const CReadOnlyNameType&  ConfigName, const CReadOnlyNameType&  Index1, const CReadOnlyNameType&  Index2);

	///根据主码寻找
	///@return	找到的CServiceConfig，如果找不到，返回NULL
	CServiceConfig *findByPK(const CReadOnlyNameType& ConfigName, const CReadOnlyNameType& Index1, const CReadOnlyNameType& Index2);
		
	///根据主键删除CServiceConfig
	bool removeByPK(const CReadOnlyNameType& ConfigName, const CReadOnlyNameType& Index1, const CReadOnlyNameType& Index2, CTransaction *pTransaction=NULL);
	
	///判断字段是否是是主键
	bool isPK(const char* pName);
	
	///根据主键寻找CServiceConfig
	///@param	pServiceConfig	要找的值
	///@return	找到的CServiceConfig，如果找不到，返回NULL
	CServiceConfig *findByPK(CServiceConfigField *pServiceConfig);
	
	///根据主键寻找CServiceConfig
	///@param	pServiceConfig	要找的值
	///@return	找到的CServiceConfig，如果找不到，返回NULL
	CServiceConfig *findByPK(CServiceConfig *pServiceConfig);

	///根据主键获取CServiceConfig
	///@param	pServiceConfig	要获取的值，同时用于存放返回的结果
	///@return	找到的CServiceConfig，如果找不到，返回NULL
	bool retrieveByPK(CWriteableServiceConfig *pServiceConfig);

	///根据主键增加新的CServiceConfig
	///@param	pServiceConfig	新的值
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	///@return	返回插入的CServiceConfig，如果插入失败，则返回NULL
	CServiceConfig *addByPK(CWriteableServiceConfig *pServiceConfig, CTransaction *pTransaction=NULL);
	
	///根据主键增加或更新新的CServiceConfig
	///@param	pServiceConfig	新的值
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	///@return	返回插入的CServiceConfig，如果插入失败，则返回NULL
	CServiceConfig *addOrUpdateByPK(CWriteableServiceConfig *pServiceConfig, CTransaction *pTransaction=NULL);

	///根据主键更新CServiceConfig的值
	///@param	pServiceConfig	新的值
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	///@return	true表示成功，false表示失败
	bool updateByPK(CWriteableServiceConfig *pServiceConfig, CTransaction *pTransaction=NULL);

	///根据主键删除CServiceConfig
	///@param	pServiceConfig	要删除的值
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	///@return	true表示成功，false表示失败
	bool removeByPK(CServiceConfig *pServiceConfig, CTransaction *pTransaction=NULL);
	
	///根据主键删除CServiceConfig
	///@param	pServiceConfig	要删除的值
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	///@return	true表示成功，false表示失败
	bool removeByPK(CServiceConfigField *pServiceConfig, CTransaction *pTransaction=NULL);	

	friend class CServiceConfigIteratorByIndex1;

	///寻找下一个符合条件的CServiceConfig，必须在startFindByIndex1以后叫用
	///@return	下一个满足条件CServiceConfig，如果已经没有一个满足要求了，则返回NULL
	CServiceConfig *findNextByIndex1();
	
	///寻找第一个CServiceConfig
	///@return	只寻找满足条件的第一个CServiceConfig，如果找不到，返回NULL
	CServiceConfig *findFirstByIndex1( const CReadOnlyNameType& ConfigName,  const CReadOnlyNameType& Index1);
	

private:
	///完成自己的初始化工作
	///@param	maxUnit	最多的数据对象个数
	///@param	pAllocator	进行内存管理的分配器
	///@param	pIndexMap	索引名称和内存编号的对应表，在非重用时会写入，在重用时只读出
	///@param	reuse	是否重用
	///@param	reuseID	重用编号，非重用时应当置为-1
	///@param	readOnly	1表示该CFixMem是只读的，0表示可以读写
	void init(int maxUnit,IMemoryAllocator *pAllocator,map<string,int>* pIndexMap,bool reuse,int reuseID,int readOnly);
	
	///将一个CServiceConfig加入到该对象工厂，实质上是完成修改索引的工作
	///@param	pServiceConfig	要加入的ServiceConfig
	///@param	bNoTransaction 是否不属于任何事务
	///@return	加入后该对象的位置，当pTransaction!=NULL时，总是返回NULL
	CServiceConfig* internalAdd(CWriteableServiceConfig *pServiceConfig, bool bNoTransaction);
	
	
	///刷新该CServiceConfig的键值
	///@param	pServiceConfig	被刷新的CServiceConfig
	///@param	pNewServiceConfig	新的值
	///@param	bNoTransaction 是否不属于任何事务
	CServiceConfig* internalUpdate(CServiceConfig *pServiceConfig, CWriteableServiceConfig *pNewServiceConfig, bool bNoTransaction);
	
	///删除一个CServiceConfig，同时删除其索引
	///@param	pServiceConfig	要删除的CServiceConfig
	///@param	bNoTransaction 是否不属于任何事务
	void internalRemove(CServiceConfig *pServiceConfig, bool bNoTransaction);
/*
	///检查某个CServiceConfig是否属于本对象工厂
	///@param	pServiceConfig	要检查的对象
	///@exception	CDesignError	如果不属于，则抛出此异常
	void checkAddress(CServiceConfig *pServiceConfig);
*/

	///基于ConfigName+Index1+Index2的索引
	CAVLTree *pConfigIndex;
	
	
	///按照ConfigName，Index1检索时，存储的查询参数
	CNameType queryConfigNameInSearchByIndex1;
	CNameType queryIndex1InSearchByIndex1;
	
	///按照ConfigName，Index1检索时，存储最后一次找到的索引节点
	CAVLNode *pLastFoundInSearchByIndex1;
		
	///确认加入后触发
	///@param	pServiceConfig	已经加入的ServiceConfig
	virtual void commitAdd(CServiceConfig *pServiceConfig);

	///确认更新后触发
	///@param	pServiceConfig	被刷新的CServiceConfig
	///@param	poldServiceConfig	原来的值
	virtual void commitUpdate(CServiceConfig *pServiceConfig, CWriteableServiceConfig *pOldServiceConfig);
	
	///确认删除后触发
	///@param	pServiceConfig	已经删除的CServiceConfig
	virtual void commitRemove(CWriteableServiceConfig *pServiceConfig);

	///所有相关的触发器
	vector<CServiceConfigActionTrigger *> *pActionTriggers;
	vector<CServiceConfigCommitTrigger *> *pCommitTriggers;
	
	///进行查询时，用于比较的对象
	CWriteableServiceConfig compareObject;
	CMDB* m_pMDB;
};

/////////////////////////////////////////////////////////////////////////
///CUserSessionFactory是一个交易员在线会话的对象工厂。它包含了一批交易员在线会话，
///同时又建立了如下索引，以方便查询：
///	UserIDAndAppIDIndex
///使用本对象工厂，可以完成对交易员在线会话的增删改查操作。具体方法如下：
///	增加：叫用add，完成加入
///	删除：叫用remove就可以删除
///	查询：有如下方法可以查询：
///		使用findByToken，唯一找到对象
///		使用findByUserIDAndAppID，唯一找到对象
///	修改：对于查到的结果，可以通过retrieve取出，修改后，通过update放回
///	事务管理：所有的增加、删除和修改都可以指定事务，在事务commit时，真正写入
///	在retrieve时，也可以指定事务，此时将取出经过update的新数据
/////////////////////////////////////////////////////////////////////////
class CUserSessionFactory :public CDatabaseDataFactory
{
public:
	///构造方法
	///@param	maxUnit	初始的数据对象个数
	///@param	pAllocator	进行内存管理的分配器
	///@param	pIndexMap	索引名称和内存编号的对应表，在非重用时会写入，在重用时只读出
	///@param	reuse	是否重用
	///@param	reuseID	重用编号，非重用时应当置为-1
	///@param	readOnly	1表示该CFixMem是只读的，0表示可以读写
	CUserSessionFactory(CMDB* pMDB,int maxUnit,IMemoryAllocator *pAllocator,map<string,int>* pIndexMap,bool reuse=false, int reuseID=-1, int readOnly=0);
	
	///构造方法
	///@param	pAllocator	进行内存管理的分配器
	///@param	pIndexMap	索引名称和内存编号的对应表，在非重用时会写入，在重用时只读出
	///@param	reuse	是否重用
	///@param	reuseID	重用编号，非重用时应当置为-1
	///@param	readOnly	1表示该CFixMem是只读的，0表示可以读写
	CUserSessionFactory(CMDB* pMDB,IMemoryAllocator *pAllocator,map<string,int>* pIndexMap,bool reuse=false, int reuseID = -1,int readOnly=0);
	
	///析构方法，将负责释放所占用的空间
	virtual ~CUserSessionFactory();

	///从CSV文件中读取所有的UserSession
	///@param	filename	文件名
	///@return	1表示完成，0表示没有完成
	virtual int readCSV(char *filename);

	///从CSV文件中读取所有的UserSession，文件名将根据配置文件中的定义UserSessionCSVFile
	///@return	1表示完成，0表示没有完成
	virtual int readCSVByPath(char *pPath);
	
	///向CSV文件中写出所有的UserSession
	///@param	filename	文件名
	///@return	1表示完成，0表示没有完成
	virtual int writeCSV(char *filename);
	
	///向CSV文件中写出上一个版本的UserSession
	///@param	filename	文件名
	///@return	1表示完成，0表示没有完成
	virtual int writeCSVPreVersion(char *filename);	

	///向CSV文件中写出所有的UserSession，文件名将根据配置文件中的定义UserSessionCSVFile
	///@return	1表示完成，0表示没有完成
	virtual int writeCSVByPath(char *pPath);

	///将内容写到文件中,用于DEBUG
	virtual void dump(FILE *);

	///将内容写到文件中,用于DEBUG
	virtual void dump(char *filename);

	///清空所有的记录
	virtual void clearAll();

	friend class CUserSessionResource;

	///将一个CUserSession加入到该对象工厂，实质上是完成修改索引的工作
	///@param	pUserSession	要加入的UserSession
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	///@return	加入后该对象的位置，当pTransaction!=NULL时，总是返回NULL
	CUserSession* add(CWriteableUserSession *pUserSession, CTransaction *pTransaction=NULL);
	
	///刷新该CUserSession的值
	///@param	pUserSession	被刷新的CUserSession
	///@param	pNewUserSession	新的值
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	CUserSession* update(CUserSession *pUserSession, CWriteableUserSession *pNewUserSession, CTransaction *pTransaction=NULL, bool bNoTransaction = true);
	
	///增加或者更新数据
	///@param	pUserSession	  需要被刷新或者新增的CUserSession,等于NULL表示是需要新增的
	///@param	pNewUserSession	新的值
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	///@param	新增或者修改后的数据
	CUserSession* addOrUpdate(CUserSession *pUserSession, CWriteableUserSession *pNewUserSession, CTransaction *pTransaction=NULL);
	
	///删除一个CUserSession，同时删除其索引
	///@param	pUserSession	要删除的CUserSession
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	void remove(CUserSession *pUserSession, CTransaction *pTransaction=NULL, bool bNoTransaction=true);
	
	///获取某个CUserSession
	///@param	pUserSession	要读取的CUserSession
	///@param	pTragetUserSession	存放结果的CUserSession
	void retrieve(CUserSession *pUserSession, CWriteableUserSession *pTargetUserSession);
	
	///获取第一个CUserSession
	///@return	得到的第一个CUserSession，如果没有，则返回NULL
	CUserSession* getFirst();
	
	///获取下一个CUserSession
	///@return	得到下一个CUserSession，如果没有，则返回NULL
	CUserSession* getNext();
	
	///加入一个操作触发器
	///@param	pTrigger	要加入的操作触发器
	///@return	1表示成功，0表示失败
	int addActionTrigger(CUserSessionActionTrigger *pTrigger);
	
	///删除一个操作触发器
	///@param	pTrigger	要删除的操作触发器
	///@return	1表示成功，0表示失败
	int removeActionTrigger(CUserSessionActionTrigger *pTrigger);

	///加入一个确认触发器
	///@param	pTrigger	要加入的确认触发器
	///@return	1表示成功，0表示失败
	int addCommitTrigger(CUserSessionCommitTrigger *pTrigger);
	
	///删除一个确认触发器
	///@param	pTrigger	要删除的确认触发器
	///@return	1表示成功，0表示失败
	int removeCommitTrigger(CUserSessionCommitTrigger *pTrigger);
	
	///检查是否字段为NULL
	///@param pFile 输出错误日志的文件
	void checkNull(FILE *pFile);
	
	///检查是否字段合法
	///@param pFile 输出错误日志的文件
	void checkValid(FILE *pFile);	

	///寻找CUserSession
	///@return	找到的CUserSession，如果找不到，返回NULL
	CUserSession *findByToken(const CReadOnlyShortContentType&  Token);

	///根据主码寻找
	///@return	找到的CUserSession，如果找不到，返回NULL
	CUserSession *findByPK(const CReadOnlyShortContentType& Token);
		
	///根据主键删除CUserSession
	bool removeByPK(const CReadOnlyShortContentType& Token, CTransaction *pTransaction=NULL);
	
	///判断字段是否是是主键
	bool isPK(const char* pName);
	
	///根据主键寻找CUserSession
	///@param	pUserSession	要找的值
	///@return	找到的CUserSession，如果找不到，返回NULL
	CUserSession *findByPK(CUserSessionField *pUserSession);
	
	///根据主键寻找CUserSession
	///@param	pUserSession	要找的值
	///@return	找到的CUserSession，如果找不到，返回NULL
	CUserSession *findByPK(CUserSession *pUserSession);

	///根据主键获取CUserSession
	///@param	pUserSession	要获取的值，同时用于存放返回的结果
	///@return	找到的CUserSession，如果找不到，返回NULL
	bool retrieveByPK(CWriteableUserSession *pUserSession);

	///根据主键增加新的CUserSession
	///@param	pUserSession	新的值
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	///@return	返回插入的CUserSession，如果插入失败，则返回NULL
	CUserSession *addByPK(CWriteableUserSession *pUserSession, CTransaction *pTransaction=NULL);
	
	///根据主键增加或更新新的CUserSession
	///@param	pUserSession	新的值
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	///@return	返回插入的CUserSession，如果插入失败，则返回NULL
	CUserSession *addOrUpdateByPK(CWriteableUserSession *pUserSession, CTransaction *pTransaction=NULL);

	///根据主键更新CUserSession的值
	///@param	pUserSession	新的值
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	///@return	true表示成功，false表示失败
	bool updateByPK(CWriteableUserSession *pUserSession, CTransaction *pTransaction=NULL);

	///根据主键删除CUserSession
	///@param	pUserSession	要删除的值
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	///@return	true表示成功，false表示失败
	bool removeByPK(CUserSession *pUserSession, CTransaction *pTransaction=NULL);
	
	///根据主键删除CUserSession
	///@param	pUserSession	要删除的值
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	///@return	true表示成功，false表示失败
	bool removeByPK(CUserSessionField *pUserSession, CTransaction *pTransaction=NULL);	

	///寻找CUserSession
	///@return	找到的CUserSession，如果找不到，返回NULL
	CUserSession *findByUserIDAndAppID(const CReadOnlyLongIndexIDType&  UserID, const CReadOnlyShortIndexIDType&  APPID);



private:
	///完成自己的初始化工作
	///@param	maxUnit	最多的数据对象个数
	///@param	pAllocator	进行内存管理的分配器
	///@param	pIndexMap	索引名称和内存编号的对应表，在非重用时会写入，在重用时只读出
	///@param	reuse	是否重用
	///@param	reuseID	重用编号，非重用时应当置为-1
	///@param	readOnly	1表示该CFixMem是只读的，0表示可以读写
	void init(int maxUnit,IMemoryAllocator *pAllocator,map<string,int>* pIndexMap,bool reuse,int reuseID,int readOnly);
	
	///将一个CUserSession加入到该对象工厂，实质上是完成修改索引的工作
	///@param	pUserSession	要加入的UserSession
	///@param	bNoTransaction 是否不属于任何事务
	///@return	加入后该对象的位置，当pTransaction!=NULL时，总是返回NULL
	CUserSession* internalAdd(CWriteableUserSession *pUserSession, bool bNoTransaction);
	
	
	///刷新该CUserSession的键值
	///@param	pUserSession	被刷新的CUserSession
	///@param	pNewUserSession	新的值
	///@param	bNoTransaction 是否不属于任何事务
	CUserSession* internalUpdate(CUserSession *pUserSession, CWriteableUserSession *pNewUserSession, bool bNoTransaction);
	
	///删除一个CUserSession，同时删除其索引
	///@param	pUserSession	要删除的CUserSession
	///@param	bNoTransaction 是否不属于任何事务
	void internalRemove(CUserSession *pUserSession, bool bNoTransaction);
/*
	///检查某个CUserSession是否属于本对象工厂
	///@param	pUserSession	要检查的对象
	///@exception	CDesignError	如果不属于，则抛出此异常
	void checkAddress(CUserSession *pUserSession);
*/

	///基于UserID+APPID的索引
	CAVLTree *pUserIDAndAppIDIndex;
	
	///基于Token的hash索引
	CHashIndex *pTokenIndex;
	
		
	///确认加入后触发
	///@param	pUserSession	已经加入的UserSession
	virtual void commitAdd(CUserSession *pUserSession);

	///确认更新后触发
	///@param	pUserSession	被刷新的CUserSession
	///@param	poldUserSession	原来的值
	virtual void commitUpdate(CUserSession *pUserSession, CWriteableUserSession *pOldUserSession);
	
	///确认删除后触发
	///@param	pUserSession	已经删除的CUserSession
	virtual void commitRemove(CWriteableUserSession *pUserSession);

	///所有相关的触发器
	vector<CUserSessionActionTrigger *> *pActionTriggers;
	vector<CUserSessionCommitTrigger *> *pCommitTriggers;
	
	///进行查询时，用于比较的对象
	CWriteableUserSession compareObject;
	CMDB* m_pMDB;
};

/////////////////////////////////////////////////////////////////////////
///CInstrumentFactory是一个标的的对象工厂。它包含了一批标的，
///同时又建立了如下索引，以方便查询：
///	SettlementGroupIndex
///	ProductGroupIndex
///使用本对象工厂，可以完成对标的的增删改查操作。具体方法如下：
///	增加：叫用add，完成加入
///	删除：叫用remove就可以删除
///	查询：有如下方法可以查询：
///		使用findByInstrumentID，唯一找到对象
///		使用startFindBySettlementGroup，findNextBySettlementGroup完成查询操作
///		使用startFindByProductGroup，findNextByProductGroup完成查询操作
///	修改：对于查到的结果，可以通过retrieve取出，修改后，通过update放回
///	事务管理：所有的增加、删除和修改都可以指定事务，在事务commit时，真正写入
///	在retrieve时，也可以指定事务，此时将取出经过update的新数据
/////////////////////////////////////////////////////////////////////////
class CInstrumentFactory :public CDatabaseDataFactory
{
public:
	///构造方法
	///@param	maxUnit	初始的数据对象个数
	///@param	pAllocator	进行内存管理的分配器
	///@param	pIndexMap	索引名称和内存编号的对应表，在非重用时会写入，在重用时只读出
	///@param	reuse	是否重用
	///@param	reuseID	重用编号，非重用时应当置为-1
	///@param	readOnly	1表示该CFixMem是只读的，0表示可以读写
	CInstrumentFactory(CMDB* pMDB,int maxUnit,IMemoryAllocator *pAllocator,map<string,int>* pIndexMap,bool reuse=false, int reuseID=-1, int readOnly=0);
	
	///构造方法
	///@param	pAllocator	进行内存管理的分配器
	///@param	pIndexMap	索引名称和内存编号的对应表，在非重用时会写入，在重用时只读出
	///@param	reuse	是否重用
	///@param	reuseID	重用编号，非重用时应当置为-1
	///@param	readOnly	1表示该CFixMem是只读的，0表示可以读写
	CInstrumentFactory(CMDB* pMDB,IMemoryAllocator *pAllocator,map<string,int>* pIndexMap,bool reuse=false, int reuseID = -1,int readOnly=0);
	
	///析构方法，将负责释放所占用的空间
	virtual ~CInstrumentFactory();

	///从CSV文件中读取所有的Instrument
	///@param	filename	文件名
	///@return	1表示完成，0表示没有完成
	virtual int readCSV(char *filename);

	///从CSV文件中读取所有的Instrument，文件名将根据配置文件中的定义InstrumentCSVFile
	///@return	1表示完成，0表示没有完成
	virtual int readCSVByPath(char *pPath);
	
	///向CSV文件中写出所有的Instrument
	///@param	filename	文件名
	///@return	1表示完成，0表示没有完成
	virtual int writeCSV(char *filename);
	
	///向CSV文件中写出上一个版本的Instrument
	///@param	filename	文件名
	///@return	1表示完成，0表示没有完成
	virtual int writeCSVPreVersion(char *filename);	

	///向CSV文件中写出所有的Instrument，文件名将根据配置文件中的定义InstrumentCSVFile
	///@return	1表示完成，0表示没有完成
	virtual int writeCSVByPath(char *pPath);

	///将内容写到文件中,用于DEBUG
	virtual void dump(FILE *);

	///将内容写到文件中,用于DEBUG
	virtual void dump(char *filename);

	///清空所有的记录
	virtual void clearAll();

	friend class CInstrumentResource;

	///将一个CInstrument加入到该对象工厂，实质上是完成修改索引的工作
	///@param	pInstrument	要加入的Instrument
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	///@return	加入后该对象的位置，当pTransaction!=NULL时，总是返回NULL
	CInstrument* add(CWriteableInstrument *pInstrument, CTransaction *pTransaction=NULL);
	
	///刷新该CInstrument的值
	///@param	pInstrument	被刷新的CInstrument
	///@param	pNewInstrument	新的值
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	CInstrument* update(CInstrument *pInstrument, CWriteableInstrument *pNewInstrument, CTransaction *pTransaction=NULL, bool bNoTransaction = true);
	
	///增加或者更新数据
	///@param	pInstrument	  需要被刷新或者新增的CInstrument,等于NULL表示是需要新增的
	///@param	pNewInstrument	新的值
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	///@param	新增或者修改后的数据
	CInstrument* addOrUpdate(CInstrument *pInstrument, CWriteableInstrument *pNewInstrument, CTransaction *pTransaction=NULL);
	
	///删除一个CInstrument，同时删除其索引
	///@param	pInstrument	要删除的CInstrument
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	void remove(CInstrument *pInstrument, CTransaction *pTransaction=NULL, bool bNoTransaction=true);
	
	///获取某个CInstrument
	///@param	pInstrument	要读取的CInstrument
	///@param	pTragetInstrument	存放结果的CInstrument
	void retrieve(CInstrument *pInstrument, CWriteableInstrument *pTargetInstrument);
	
	///获取第一个CInstrument
	///@return	得到的第一个CInstrument，如果没有，则返回NULL
	CInstrument* getFirst();
	
	///获取下一个CInstrument
	///@return	得到下一个CInstrument，如果没有，则返回NULL
	CInstrument* getNext();
	
	///加入一个操作触发器
	///@param	pTrigger	要加入的操作触发器
	///@return	1表示成功，0表示失败
	int addActionTrigger(CInstrumentActionTrigger *pTrigger);
	
	///删除一个操作触发器
	///@param	pTrigger	要删除的操作触发器
	///@return	1表示成功，0表示失败
	int removeActionTrigger(CInstrumentActionTrigger *pTrigger);

	///加入一个确认触发器
	///@param	pTrigger	要加入的确认触发器
	///@return	1表示成功，0表示失败
	int addCommitTrigger(CInstrumentCommitTrigger *pTrigger);
	
	///删除一个确认触发器
	///@param	pTrigger	要删除的确认触发器
	///@return	1表示成功，0表示失败
	int removeCommitTrigger(CInstrumentCommitTrigger *pTrigger);
	
	///构造所有对应的行情的指针
	///@param	pFactory	构造对应的行情的Link的指针时，寻找的对象工厂
	void linkAllMarketData();

	///构造所有对应的清算币种的指针
	///@param	pFactory	构造对应的清算币种的Link的指针时，寻找的对象工厂
	void linkAllClearCurrency();

	///构造所有对应的基础币种的指针
	///@param	pFactory	构造对应的基础币种的Link的指针时，寻找的对象工厂
	void linkAllBaseCurrency();

	///检查对应的行情的指针
	///@param pFile 输出错误日志的文件
	void checkLinkMarketData(FILE *pFile);
	
	///检查对应的清算币种的指针
	///@param pFile 输出错误日志的文件
	void checkLinkClearCurrency(FILE *pFile);
	
	///检查对应的基础币种的指针
	///@param pFile 输出错误日志的文件
	void checkLinkBaseCurrency(FILE *pFile);
	
	///清空对应的行情的外键
	void clearLinkMarketData();
	
	///清空对应的清算币种的外键
	void clearLinkClearCurrency();
	
	///清空对应的基础币种的外键
	void clearLinkBaseCurrency();
	
	///检查是否字段为NULL
	///@param pFile 输出错误日志的文件
	void checkNull(FILE *pFile);
	
	///检查是否字段合法
	///@param pFile 输出错误日志的文件
	void checkValid(FILE *pFile);	

	///寻找CInstrument
	///@return	找到的CInstrument，如果找不到，返回NULL
	CInstrument *findByInstrumentID(const CReadOnlyExchangeIDType&  ExchangeID, const CReadOnlyInstrumentIDType&  InstrumentID);

	///根据主码寻找
	///@return	找到的CInstrument，如果找不到，返回NULL
	CInstrument *findByPK(const CReadOnlyExchangeIDType& ExchangeID, const CReadOnlyInstrumentIDType& InstrumentID);
		
	///根据主键删除CInstrument
	bool removeByPK(const CReadOnlyExchangeIDType& ExchangeID, const CReadOnlyInstrumentIDType& InstrumentID, CTransaction *pTransaction=NULL);
	
	///判断字段是否是是主键
	bool isPK(const char* pName);
	
	///根据主键寻找CInstrument
	///@param	pInstrument	要找的值
	///@return	找到的CInstrument，如果找不到，返回NULL
	CInstrument *findByPK(CInstrumentField *pInstrument);
	
	///根据主键寻找CInstrument
	///@param	pInstrument	要找的值
	///@return	找到的CInstrument，如果找不到，返回NULL
	CInstrument *findByPK(CInstrument *pInstrument);

	///根据主键获取CInstrument
	///@param	pInstrument	要获取的值，同时用于存放返回的结果
	///@return	找到的CInstrument，如果找不到，返回NULL
	bool retrieveByPK(CWriteableInstrument *pInstrument);

	///根据主键增加新的CInstrument
	///@param	pInstrument	新的值
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	///@return	返回插入的CInstrument，如果插入失败，则返回NULL
	CInstrument *addByPK(CWriteableInstrument *pInstrument, CTransaction *pTransaction=NULL);
	
	///根据主键增加或更新新的CInstrument
	///@param	pInstrument	新的值
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	///@return	返回插入的CInstrument，如果插入失败，则返回NULL
	CInstrument *addOrUpdateByPK(CWriteableInstrument *pInstrument, CTransaction *pTransaction=NULL);

	///根据主键更新CInstrument的值
	///@param	pInstrument	新的值
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	///@return	true表示成功，false表示失败
	bool updateByPK(CWriteableInstrument *pInstrument, CTransaction *pTransaction=NULL);

	///根据主键删除CInstrument
	///@param	pInstrument	要删除的值
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	///@return	true表示成功，false表示失败
	bool removeByPK(CInstrument *pInstrument, CTransaction *pTransaction=NULL);
	
	///根据主键删除CInstrument
	///@param	pInstrument	要删除的值
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	///@return	true表示成功，false表示失败
	bool removeByPK(CInstrumentField *pInstrument, CTransaction *pTransaction=NULL);	

	friend class CInstrumentIteratorBySettlementGroup;

	///寻找下一个符合条件的CInstrument，必须在startFindBySettlementGroup以后叫用
	///@return	下一个满足条件CInstrument，如果已经没有一个满足要求了，则返回NULL
	CInstrument *findNextBySettlementGroup();
	
	///寻找第一个CInstrument
	///@return	只寻找满足条件的第一个CInstrument，如果找不到，返回NULL
	CInstrument *findFirstBySettlementGroup( const CReadOnlyShortIndexIDType& SettlementGroup);
	
	friend class CInstrumentIteratorByProductGroup;

	///寻找下一个符合条件的CInstrument，必须在startFindByProductGroup以后叫用
	///@return	下一个满足条件CInstrument，如果已经没有一个满足要求了，则返回NULL
	CInstrument *findNextByProductGroup();
	
	///寻找第一个CInstrument
	///@return	只寻找满足条件的第一个CInstrument，如果找不到，返回NULL
	CInstrument *findFirstByProductGroup( const CReadOnlyShortIndexIDType& ProductGroup);
	

private:
	///完成自己的初始化工作
	///@param	maxUnit	最多的数据对象个数
	///@param	pAllocator	进行内存管理的分配器
	///@param	pIndexMap	索引名称和内存编号的对应表，在非重用时会写入，在重用时只读出
	///@param	reuse	是否重用
	///@param	reuseID	重用编号，非重用时应当置为-1
	///@param	readOnly	1表示该CFixMem是只读的，0表示可以读写
	void init(int maxUnit,IMemoryAllocator *pAllocator,map<string,int>* pIndexMap,bool reuse,int reuseID,int readOnly);
	
	///将一个CInstrument加入到该对象工厂，实质上是完成修改索引的工作
	///@param	pInstrument	要加入的Instrument
	///@param	bNoTransaction 是否不属于任何事务
	///@return	加入后该对象的位置，当pTransaction!=NULL时，总是返回NULL
	CInstrument* internalAdd(CWriteableInstrument *pInstrument, bool bNoTransaction);
	
	
	///刷新该CInstrument的键值
	///@param	pInstrument	被刷新的CInstrument
	///@param	pNewInstrument	新的值
	///@param	bNoTransaction 是否不属于任何事务
	CInstrument* internalUpdate(CInstrument *pInstrument, CWriteableInstrument *pNewInstrument, bool bNoTransaction);
	
	///删除一个CInstrument，同时删除其索引
	///@param	pInstrument	要删除的CInstrument
	///@param	bNoTransaction 是否不属于任何事务
	void internalRemove(CInstrument *pInstrument, bool bNoTransaction);
/*
	///检查某个CInstrument是否属于本对象工厂
	///@param	pInstrument	要检查的对象
	///@exception	CDesignError	如果不属于，则抛出此异常
	void checkAddress(CInstrument *pInstrument);
*/

	///基于SettlementGroup的索引
	CAVLTree *pSettlementGroupIndex;
	///基于ProductGroup的索引
	CAVLTree *pProductGroupIndex;
	
	///基于InstrumentID的hash索引
	CHashIndex *pInstrumentIDHashIndex;
	
	///按照SettlementGroup检索时，存储的查询参数
	CShortIndexIDType querySettlementGroupInSearchBySettlementGroup;
	
	///按照SettlementGroup检索时，存储最后一次找到的索引节点
	CAVLNode *pLastFoundInSearchBySettlementGroup;
	///按照ProductGroup检索时，存储的查询参数
	CShortIndexIDType queryProductGroupInSearchByProductGroup;
	
	///按照ProductGroup检索时，存储最后一次找到的索引节点
	CAVLNode *pLastFoundInSearchByProductGroup;
		
	///确认加入后触发
	///@param	pInstrument	已经加入的Instrument
	virtual void commitAdd(CInstrument *pInstrument);

	///确认更新后触发
	///@param	pInstrument	被刷新的CInstrument
	///@param	poldInstrument	原来的值
	virtual void commitUpdate(CInstrument *pInstrument, CWriteableInstrument *pOldInstrument);
	
	///确认删除后触发
	///@param	pInstrument	已经删除的CInstrument
	virtual void commitRemove(CWriteableInstrument *pInstrument);

	///所有相关的触发器
	vector<CInstrumentActionTrigger *> *pActionTriggers;
	vector<CInstrumentCommitTrigger *> *pCommitTriggers;
	
	///进行查询时，用于比较的对象
	CWriteableInstrument compareObject;
	CMDB* m_pMDB;
};

/////////////////////////////////////////////////////////////////////////
///CTradingRightFactory是一个交易权限的对象工厂。它包含了一批交易权限，
///同时又建立了如下索引，以方便查询：
///使用本对象工厂，可以完成对交易权限的增删改查操作。具体方法如下：
///	增加：叫用add，完成加入
///	删除：叫用remove就可以删除
///	查询：有如下方法可以查询：
///		使用findByMemberIDAndInstrument，唯一找到对象
///	修改：对于查到的结果，可以通过retrieve取出，修改后，通过update放回
///	事务管理：所有的增加、删除和修改都可以指定事务，在事务commit时，真正写入
///	在retrieve时，也可以指定事务，此时将取出经过update的新数据
/////////////////////////////////////////////////////////////////////////
class CTradingRightFactory :public CDatabaseDataFactory
{
public:
	///构造方法
	///@param	maxUnit	初始的数据对象个数
	///@param	pAllocator	进行内存管理的分配器
	///@param	pIndexMap	索引名称和内存编号的对应表，在非重用时会写入，在重用时只读出
	///@param	reuse	是否重用
	///@param	reuseID	重用编号，非重用时应当置为-1
	///@param	readOnly	1表示该CFixMem是只读的，0表示可以读写
	CTradingRightFactory(CMDB* pMDB,int maxUnit,IMemoryAllocator *pAllocator,map<string,int>* pIndexMap,bool reuse=false, int reuseID=-1, int readOnly=0);
	
	///构造方法
	///@param	pAllocator	进行内存管理的分配器
	///@param	pIndexMap	索引名称和内存编号的对应表，在非重用时会写入，在重用时只读出
	///@param	reuse	是否重用
	///@param	reuseID	重用编号，非重用时应当置为-1
	///@param	readOnly	1表示该CFixMem是只读的，0表示可以读写
	CTradingRightFactory(CMDB* pMDB,IMemoryAllocator *pAllocator,map<string,int>* pIndexMap,bool reuse=false, int reuseID = -1,int readOnly=0);
	
	///析构方法，将负责释放所占用的空间
	virtual ~CTradingRightFactory();

	///从CSV文件中读取所有的TradingRight
	///@param	filename	文件名
	///@return	1表示完成，0表示没有完成
	virtual int readCSV(char *filename);

	///从CSV文件中读取所有的TradingRight，文件名将根据配置文件中的定义TradingRightCSVFile
	///@return	1表示完成，0表示没有完成
	virtual int readCSVByPath(char *pPath);
	
	///向CSV文件中写出所有的TradingRight
	///@param	filename	文件名
	///@return	1表示完成，0表示没有完成
	virtual int writeCSV(char *filename);
	
	///向CSV文件中写出上一个版本的TradingRight
	///@param	filename	文件名
	///@return	1表示完成，0表示没有完成
	virtual int writeCSVPreVersion(char *filename);	

	///向CSV文件中写出所有的TradingRight，文件名将根据配置文件中的定义TradingRightCSVFile
	///@return	1表示完成，0表示没有完成
	virtual int writeCSVByPath(char *pPath);

	///将内容写到文件中,用于DEBUG
	virtual void dump(FILE *);

	///将内容写到文件中,用于DEBUG
	virtual void dump(char *filename);

	///清空所有的记录
	virtual void clearAll();

	friend class CTradingRightResource;

	///将一个CTradingRight加入到该对象工厂，实质上是完成修改索引的工作
	///@param	pTradingRight	要加入的TradingRight
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	///@return	加入后该对象的位置，当pTransaction!=NULL时，总是返回NULL
	CTradingRight* add(CWriteableTradingRight *pTradingRight, CTransaction *pTransaction=NULL);
	
	///刷新该CTradingRight的值
	///@param	pTradingRight	被刷新的CTradingRight
	///@param	pNewTradingRight	新的值
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	CTradingRight* update(CTradingRight *pTradingRight, CWriteableTradingRight *pNewTradingRight, CTransaction *pTransaction=NULL, bool bNoTransaction = true);
	
	///增加或者更新数据
	///@param	pTradingRight	  需要被刷新或者新增的CTradingRight,等于NULL表示是需要新增的
	///@param	pNewTradingRight	新的值
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	///@param	新增或者修改后的数据
	CTradingRight* addOrUpdate(CTradingRight *pTradingRight, CWriteableTradingRight *pNewTradingRight, CTransaction *pTransaction=NULL);
	
	///删除一个CTradingRight，同时删除其索引
	///@param	pTradingRight	要删除的CTradingRight
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	void remove(CTradingRight *pTradingRight, CTransaction *pTransaction=NULL, bool bNoTransaction=true);
	
	///获取某个CTradingRight
	///@param	pTradingRight	要读取的CTradingRight
	///@param	pTragetTradingRight	存放结果的CTradingRight
	void retrieve(CTradingRight *pTradingRight, CWriteableTradingRight *pTargetTradingRight);
	
	///获取第一个CTradingRight
	///@return	得到的第一个CTradingRight，如果没有，则返回NULL
	CTradingRight* getFirst();
	
	///获取下一个CTradingRight
	///@return	得到下一个CTradingRight，如果没有，则返回NULL
	CTradingRight* getNext();
	
	///加入一个操作触发器
	///@param	pTrigger	要加入的操作触发器
	///@return	1表示成功，0表示失败
	int addActionTrigger(CTradingRightActionTrigger *pTrigger);
	
	///删除一个操作触发器
	///@param	pTrigger	要删除的操作触发器
	///@return	1表示成功，0表示失败
	int removeActionTrigger(CTradingRightActionTrigger *pTrigger);

	///加入一个确认触发器
	///@param	pTrigger	要加入的确认触发器
	///@return	1表示成功，0表示失败
	int addCommitTrigger(CTradingRightCommitTrigger *pTrigger);
	
	///删除一个确认触发器
	///@param	pTrigger	要删除的确认触发器
	///@return	1表示成功，0表示失败
	int removeCommitTrigger(CTradingRightCommitTrigger *pTrigger);
	
	///检查是否字段为NULL
	///@param pFile 输出错误日志的文件
	void checkNull(FILE *pFile);
	
	///检查是否字段合法
	///@param pFile 输出错误日志的文件
	void checkValid(FILE *pFile);	

	///寻找CTradingRight
	///@return	找到的CTradingRight，如果找不到，返回NULL
	CTradingRight *findByMemberIDAndInstrument(const CReadOnlyLongIndexIDType&  MemberID, const CReadOnlyInstrumentIDType&  InstrumentID, const CReadOnlyDirectionType&  Direction);

	///根据主码寻找
	///@return	找到的CTradingRight，如果找不到，返回NULL
	CTradingRight *findByPK(const CReadOnlyLongIndexIDType& MemberID, const CReadOnlyInstrumentIDType& InstrumentID, const CReadOnlyDirectionType& Direction);
		
	///根据主键删除CTradingRight
	bool removeByPK(const CReadOnlyLongIndexIDType& MemberID, const CReadOnlyInstrumentIDType& InstrumentID, const CReadOnlyDirectionType& Direction, CTransaction *pTransaction=NULL);
	
	///判断字段是否是是主键
	bool isPK(const char* pName);
	
	///根据主键寻找CTradingRight
	///@param	pTradingRight	要找的值
	///@return	找到的CTradingRight，如果找不到，返回NULL
	CTradingRight *findByPK(CTradingRightField *pTradingRight);
	
	///根据主键寻找CTradingRight
	///@param	pTradingRight	要找的值
	///@return	找到的CTradingRight，如果找不到，返回NULL
	CTradingRight *findByPK(CTradingRight *pTradingRight);

	///根据主键获取CTradingRight
	///@param	pTradingRight	要获取的值，同时用于存放返回的结果
	///@return	找到的CTradingRight，如果找不到，返回NULL
	bool retrieveByPK(CWriteableTradingRight *pTradingRight);

	///根据主键增加新的CTradingRight
	///@param	pTradingRight	新的值
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	///@return	返回插入的CTradingRight，如果插入失败，则返回NULL
	CTradingRight *addByPK(CWriteableTradingRight *pTradingRight, CTransaction *pTransaction=NULL);
	
	///根据主键增加或更新新的CTradingRight
	///@param	pTradingRight	新的值
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	///@return	返回插入的CTradingRight，如果插入失败，则返回NULL
	CTradingRight *addOrUpdateByPK(CWriteableTradingRight *pTradingRight, CTransaction *pTransaction=NULL);

	///根据主键更新CTradingRight的值
	///@param	pTradingRight	新的值
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	///@return	true表示成功，false表示失败
	bool updateByPK(CWriteableTradingRight *pTradingRight, CTransaction *pTransaction=NULL);

	///根据主键删除CTradingRight
	///@param	pTradingRight	要删除的值
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	///@return	true表示成功，false表示失败
	bool removeByPK(CTradingRight *pTradingRight, CTransaction *pTransaction=NULL);
	
	///根据主键删除CTradingRight
	///@param	pTradingRight	要删除的值
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	///@return	true表示成功，false表示失败
	bool removeByPK(CTradingRightField *pTradingRight, CTransaction *pTransaction=NULL);	


private:
	///完成自己的初始化工作
	///@param	maxUnit	最多的数据对象个数
	///@param	pAllocator	进行内存管理的分配器
	///@param	pIndexMap	索引名称和内存编号的对应表，在非重用时会写入，在重用时只读出
	///@param	reuse	是否重用
	///@param	reuseID	重用编号，非重用时应当置为-1
	///@param	readOnly	1表示该CFixMem是只读的，0表示可以读写
	void init(int maxUnit,IMemoryAllocator *pAllocator,map<string,int>* pIndexMap,bool reuse,int reuseID,int readOnly);
	
	///将一个CTradingRight加入到该对象工厂，实质上是完成修改索引的工作
	///@param	pTradingRight	要加入的TradingRight
	///@param	bNoTransaction 是否不属于任何事务
	///@return	加入后该对象的位置，当pTransaction!=NULL时，总是返回NULL
	CTradingRight* internalAdd(CWriteableTradingRight *pTradingRight, bool bNoTransaction);
	
	
	///刷新该CTradingRight的键值
	///@param	pTradingRight	被刷新的CTradingRight
	///@param	pNewTradingRight	新的值
	///@param	bNoTransaction 是否不属于任何事务
	CTradingRight* internalUpdate(CTradingRight *pTradingRight, CWriteableTradingRight *pNewTradingRight, bool bNoTransaction);
	
	///删除一个CTradingRight，同时删除其索引
	///@param	pTradingRight	要删除的CTradingRight
	///@param	bNoTransaction 是否不属于任何事务
	void internalRemove(CTradingRight *pTradingRight, bool bNoTransaction);
/*
	///检查某个CTradingRight是否属于本对象工厂
	///@param	pTradingRight	要检查的对象
	///@exception	CDesignError	如果不属于，则抛出此异常
	void checkAddress(CTradingRight *pTradingRight);
*/

	
	///基于MemberIDAndInstrument的hash索引
	CHashIndex *pMemberIDAndInstrumentHashIndex;
	
		
	///确认加入后触发
	///@param	pTradingRight	已经加入的TradingRight
	virtual void commitAdd(CTradingRight *pTradingRight);

	///确认更新后触发
	///@param	pTradingRight	被刷新的CTradingRight
	///@param	poldTradingRight	原来的值
	virtual void commitUpdate(CTradingRight *pTradingRight, CWriteableTradingRight *pOldTradingRight);
	
	///确认删除后触发
	///@param	pTradingRight	已经删除的CTradingRight
	virtual void commitRemove(CWriteableTradingRight *pTradingRight);

	///所有相关的触发器
	vector<CTradingRightActionTrigger *> *pActionTriggers;
	vector<CTradingRightCommitTrigger *> *pCommitTriggers;
	
	///进行查询时，用于比较的对象
	CWriteableTradingRight compareObject;
	CMDB* m_pMDB;
};

/////////////////////////////////////////////////////////////////////////
///CMarginRateFactory是一个标的保证金率的对象工厂。它包含了一批标的保证金率，
///同时又建立了如下索引，以方便查询：
///使用本对象工厂，可以完成对标的保证金率的增删改查操作。具体方法如下：
///	增加：叫用add，完成加入
///	删除：叫用remove就可以删除
///	查询：有如下方法可以查询：
///		使用findByMemberAndInstrument，唯一找到对象
///	修改：对于查到的结果，可以通过retrieve取出，修改后，通过update放回
///	事务管理：所有的增加、删除和修改都可以指定事务，在事务commit时，真正写入
///	在retrieve时，也可以指定事务，此时将取出经过update的新数据
/////////////////////////////////////////////////////////////////////////
class CMarginRateFactory :public CDatabaseDataFactory
{
public:
	///构造方法
	///@param	maxUnit	初始的数据对象个数
	///@param	pAllocator	进行内存管理的分配器
	///@param	pIndexMap	索引名称和内存编号的对应表，在非重用时会写入，在重用时只读出
	///@param	reuse	是否重用
	///@param	reuseID	重用编号，非重用时应当置为-1
	///@param	readOnly	1表示该CFixMem是只读的，0表示可以读写
	CMarginRateFactory(CMDB* pMDB,int maxUnit,IMemoryAllocator *pAllocator,map<string,int>* pIndexMap,bool reuse=false, int reuseID=-1, int readOnly=0);
	
	///构造方法
	///@param	pAllocator	进行内存管理的分配器
	///@param	pIndexMap	索引名称和内存编号的对应表，在非重用时会写入，在重用时只读出
	///@param	reuse	是否重用
	///@param	reuseID	重用编号，非重用时应当置为-1
	///@param	readOnly	1表示该CFixMem是只读的，0表示可以读写
	CMarginRateFactory(CMDB* pMDB,IMemoryAllocator *pAllocator,map<string,int>* pIndexMap,bool reuse=false, int reuseID = -1,int readOnly=0);
	
	///析构方法，将负责释放所占用的空间
	virtual ~CMarginRateFactory();

	///从CSV文件中读取所有的MarginRate
	///@param	filename	文件名
	///@return	1表示完成，0表示没有完成
	virtual int readCSV(char *filename);

	///从CSV文件中读取所有的MarginRate，文件名将根据配置文件中的定义MarginRateCSVFile
	///@return	1表示完成，0表示没有完成
	virtual int readCSVByPath(char *pPath);
	
	///向CSV文件中写出所有的MarginRate
	///@param	filename	文件名
	///@return	1表示完成，0表示没有完成
	virtual int writeCSV(char *filename);
	
	///向CSV文件中写出上一个版本的MarginRate
	///@param	filename	文件名
	///@return	1表示完成，0表示没有完成
	virtual int writeCSVPreVersion(char *filename);	

	///向CSV文件中写出所有的MarginRate，文件名将根据配置文件中的定义MarginRateCSVFile
	///@return	1表示完成，0表示没有完成
	virtual int writeCSVByPath(char *pPath);

	///将内容写到文件中,用于DEBUG
	virtual void dump(FILE *);

	///将内容写到文件中,用于DEBUG
	virtual void dump(char *filename);

	///清空所有的记录
	virtual void clearAll();

	friend class CMarginRateResource;

	///将一个CMarginRate加入到该对象工厂，实质上是完成修改索引的工作
	///@param	pMarginRate	要加入的MarginRate
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	///@return	加入后该对象的位置，当pTransaction!=NULL时，总是返回NULL
	CMarginRate* add(CWriteableMarginRate *pMarginRate, CTransaction *pTransaction=NULL);
	
	///刷新该CMarginRate的值
	///@param	pMarginRate	被刷新的CMarginRate
	///@param	pNewMarginRate	新的值
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	CMarginRate* update(CMarginRate *pMarginRate, CWriteableMarginRate *pNewMarginRate, CTransaction *pTransaction=NULL, bool bNoTransaction = true);
	
	///增加或者更新数据
	///@param	pMarginRate	  需要被刷新或者新增的CMarginRate,等于NULL表示是需要新增的
	///@param	pNewMarginRate	新的值
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	///@param	新增或者修改后的数据
	CMarginRate* addOrUpdate(CMarginRate *pMarginRate, CWriteableMarginRate *pNewMarginRate, CTransaction *pTransaction=NULL);
	
	///删除一个CMarginRate，同时删除其索引
	///@param	pMarginRate	要删除的CMarginRate
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	void remove(CMarginRate *pMarginRate, CTransaction *pTransaction=NULL, bool bNoTransaction=true);
	
	///获取某个CMarginRate
	///@param	pMarginRate	要读取的CMarginRate
	///@param	pTragetMarginRate	存放结果的CMarginRate
	void retrieve(CMarginRate *pMarginRate, CWriteableMarginRate *pTargetMarginRate);
	
	///获取第一个CMarginRate
	///@return	得到的第一个CMarginRate，如果没有，则返回NULL
	CMarginRate* getFirst();
	
	///获取下一个CMarginRate
	///@return	得到下一个CMarginRate，如果没有，则返回NULL
	CMarginRate* getNext();
	
	///加入一个操作触发器
	///@param	pTrigger	要加入的操作触发器
	///@return	1表示成功，0表示失败
	int addActionTrigger(CMarginRateActionTrigger *pTrigger);
	
	///删除一个操作触发器
	///@param	pTrigger	要删除的操作触发器
	///@return	1表示成功，0表示失败
	int removeActionTrigger(CMarginRateActionTrigger *pTrigger);

	///加入一个确认触发器
	///@param	pTrigger	要加入的确认触发器
	///@return	1表示成功，0表示失败
	int addCommitTrigger(CMarginRateCommitTrigger *pTrigger);
	
	///删除一个确认触发器
	///@param	pTrigger	要删除的确认触发器
	///@return	1表示成功，0表示失败
	int removeCommitTrigger(CMarginRateCommitTrigger *pTrigger);
	
	///检查是否字段为NULL
	///@param pFile 输出错误日志的文件
	void checkNull(FILE *pFile);
	
	///检查是否字段合法
	///@param pFile 输出错误日志的文件
	void checkValid(FILE *pFile);	

	///寻找CMarginRate
	///@return	找到的CMarginRate，如果找不到，返回NULL
	CMarginRate *findByMemberAndInstrument(const CReadOnlyLongIndexIDType&  MemberID, const CReadOnlyInstrumentIDType&  InstrumentID);

	///根据主码寻找
	///@return	找到的CMarginRate，如果找不到，返回NULL
	CMarginRate *findByPK(const CReadOnlyLongIndexIDType& MemberID, const CReadOnlyInstrumentIDType& InstrumentID);
		
	///根据主键删除CMarginRate
	bool removeByPK(const CReadOnlyLongIndexIDType& MemberID, const CReadOnlyInstrumentIDType& InstrumentID, CTransaction *pTransaction=NULL);
	
	///判断字段是否是是主键
	bool isPK(const char* pName);
	
	///根据主键寻找CMarginRate
	///@param	pMarginRate	要找的值
	///@return	找到的CMarginRate，如果找不到，返回NULL
	CMarginRate *findByPK(CMarginRateField *pMarginRate);
	
	///根据主键寻找CMarginRate
	///@param	pMarginRate	要找的值
	///@return	找到的CMarginRate，如果找不到，返回NULL
	CMarginRate *findByPK(CMarginRate *pMarginRate);

	///根据主键获取CMarginRate
	///@param	pMarginRate	要获取的值，同时用于存放返回的结果
	///@return	找到的CMarginRate，如果找不到，返回NULL
	bool retrieveByPK(CWriteableMarginRate *pMarginRate);

	///根据主键增加新的CMarginRate
	///@param	pMarginRate	新的值
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	///@return	返回插入的CMarginRate，如果插入失败，则返回NULL
	CMarginRate *addByPK(CWriteableMarginRate *pMarginRate, CTransaction *pTransaction=NULL);
	
	///根据主键增加或更新新的CMarginRate
	///@param	pMarginRate	新的值
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	///@return	返回插入的CMarginRate，如果插入失败，则返回NULL
	CMarginRate *addOrUpdateByPK(CWriteableMarginRate *pMarginRate, CTransaction *pTransaction=NULL);

	///根据主键更新CMarginRate的值
	///@param	pMarginRate	新的值
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	///@return	true表示成功，false表示失败
	bool updateByPK(CWriteableMarginRate *pMarginRate, CTransaction *pTransaction=NULL);

	///根据主键删除CMarginRate
	///@param	pMarginRate	要删除的值
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	///@return	true表示成功，false表示失败
	bool removeByPK(CMarginRate *pMarginRate, CTransaction *pTransaction=NULL);
	
	///根据主键删除CMarginRate
	///@param	pMarginRate	要删除的值
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	///@return	true表示成功，false表示失败
	bool removeByPK(CMarginRateField *pMarginRate, CTransaction *pTransaction=NULL);	


private:
	///完成自己的初始化工作
	///@param	maxUnit	最多的数据对象个数
	///@param	pAllocator	进行内存管理的分配器
	///@param	pIndexMap	索引名称和内存编号的对应表，在非重用时会写入，在重用时只读出
	///@param	reuse	是否重用
	///@param	reuseID	重用编号，非重用时应当置为-1
	///@param	readOnly	1表示该CFixMem是只读的，0表示可以读写
	void init(int maxUnit,IMemoryAllocator *pAllocator,map<string,int>* pIndexMap,bool reuse,int reuseID,int readOnly);
	
	///将一个CMarginRate加入到该对象工厂，实质上是完成修改索引的工作
	///@param	pMarginRate	要加入的MarginRate
	///@param	bNoTransaction 是否不属于任何事务
	///@return	加入后该对象的位置，当pTransaction!=NULL时，总是返回NULL
	CMarginRate* internalAdd(CWriteableMarginRate *pMarginRate, bool bNoTransaction);
	
	
	///刷新该CMarginRate的键值
	///@param	pMarginRate	被刷新的CMarginRate
	///@param	pNewMarginRate	新的值
	///@param	bNoTransaction 是否不属于任何事务
	CMarginRate* internalUpdate(CMarginRate *pMarginRate, CWriteableMarginRate *pNewMarginRate, bool bNoTransaction);
	
	///删除一个CMarginRate，同时删除其索引
	///@param	pMarginRate	要删除的CMarginRate
	///@param	bNoTransaction 是否不属于任何事务
	void internalRemove(CMarginRate *pMarginRate, bool bNoTransaction);
/*
	///检查某个CMarginRate是否属于本对象工厂
	///@param	pMarginRate	要检查的对象
	///@exception	CDesignError	如果不属于，则抛出此异常
	void checkAddress(CMarginRate *pMarginRate);
*/

	
	///基于MemberAndInstrument的hash索引
	CHashIndex *pMemberAndInstrumentHashIndex;
	
		
	///确认加入后触发
	///@param	pMarginRate	已经加入的MarginRate
	virtual void commitAdd(CMarginRate *pMarginRate);

	///确认更新后触发
	///@param	pMarginRate	被刷新的CMarginRate
	///@param	poldMarginRate	原来的值
	virtual void commitUpdate(CMarginRate *pMarginRate, CWriteableMarginRate *pOldMarginRate);
	
	///确认删除后触发
	///@param	pMarginRate	已经删除的CMarginRate
	virtual void commitRemove(CWriteableMarginRate *pMarginRate);

	///所有相关的触发器
	vector<CMarginRateActionTrigger *> *pActionTriggers;
	vector<CMarginRateCommitTrigger *> *pCommitTriggers;
	
	///进行查询时，用于比较的对象
	CWriteableMarginRate compareObject;
	CMDB* m_pMDB;
};

/////////////////////////////////////////////////////////////////////////
///CCFDRateFactory是一个CFD溢价率的对象工厂。它包含了一批CFD溢价率，
///同时又建立了如下索引，以方便查询：
///使用本对象工厂，可以完成对CFD溢价率的增删改查操作。具体方法如下：
///	增加：叫用add，完成加入
///	删除：叫用remove就可以删除
///	查询：有如下方法可以查询：
///		使用findByMemberAndInstrument，唯一找到对象
///	修改：对于查到的结果，可以通过retrieve取出，修改后，通过update放回
///	事务管理：所有的增加、删除和修改都可以指定事务，在事务commit时，真正写入
///	在retrieve时，也可以指定事务，此时将取出经过update的新数据
/////////////////////////////////////////////////////////////////////////
class CCFDRateFactory :public CDatabaseDataFactory
{
public:
	///构造方法
	///@param	maxUnit	初始的数据对象个数
	///@param	pAllocator	进行内存管理的分配器
	///@param	pIndexMap	索引名称和内存编号的对应表，在非重用时会写入，在重用时只读出
	///@param	reuse	是否重用
	///@param	reuseID	重用编号，非重用时应当置为-1
	///@param	readOnly	1表示该CFixMem是只读的，0表示可以读写
	CCFDRateFactory(CMDB* pMDB,int maxUnit,IMemoryAllocator *pAllocator,map<string,int>* pIndexMap,bool reuse=false, int reuseID=-1, int readOnly=0);
	
	///构造方法
	///@param	pAllocator	进行内存管理的分配器
	///@param	pIndexMap	索引名称和内存编号的对应表，在非重用时会写入，在重用时只读出
	///@param	reuse	是否重用
	///@param	reuseID	重用编号，非重用时应当置为-1
	///@param	readOnly	1表示该CFixMem是只读的，0表示可以读写
	CCFDRateFactory(CMDB* pMDB,IMemoryAllocator *pAllocator,map<string,int>* pIndexMap,bool reuse=false, int reuseID = -1,int readOnly=0);
	
	///析构方法，将负责释放所占用的空间
	virtual ~CCFDRateFactory();

	///从CSV文件中读取所有的CFDRate
	///@param	filename	文件名
	///@return	1表示完成，0表示没有完成
	virtual int readCSV(char *filename);

	///从CSV文件中读取所有的CFDRate，文件名将根据配置文件中的定义CFDRateCSVFile
	///@return	1表示完成，0表示没有完成
	virtual int readCSVByPath(char *pPath);
	
	///向CSV文件中写出所有的CFDRate
	///@param	filename	文件名
	///@return	1表示完成，0表示没有完成
	virtual int writeCSV(char *filename);
	
	///向CSV文件中写出上一个版本的CFDRate
	///@param	filename	文件名
	///@return	1表示完成，0表示没有完成
	virtual int writeCSVPreVersion(char *filename);	

	///向CSV文件中写出所有的CFDRate，文件名将根据配置文件中的定义CFDRateCSVFile
	///@return	1表示完成，0表示没有完成
	virtual int writeCSVByPath(char *pPath);

	///将内容写到文件中,用于DEBUG
	virtual void dump(FILE *);

	///将内容写到文件中,用于DEBUG
	virtual void dump(char *filename);

	///清空所有的记录
	virtual void clearAll();

	friend class CCFDRateResource;

	///将一个CCFDRate加入到该对象工厂，实质上是完成修改索引的工作
	///@param	pCFDRate	要加入的CFDRate
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	///@return	加入后该对象的位置，当pTransaction!=NULL时，总是返回NULL
	CCFDRate* add(CWriteableCFDRate *pCFDRate, CTransaction *pTransaction=NULL);
	
	///刷新该CCFDRate的值
	///@param	pCFDRate	被刷新的CCFDRate
	///@param	pNewCFDRate	新的值
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	CCFDRate* update(CCFDRate *pCFDRate, CWriteableCFDRate *pNewCFDRate, CTransaction *pTransaction=NULL, bool bNoTransaction = true);
	
	///增加或者更新数据
	///@param	pCFDRate	  需要被刷新或者新增的CCFDRate,等于NULL表示是需要新增的
	///@param	pNewCFDRate	新的值
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	///@param	新增或者修改后的数据
	CCFDRate* addOrUpdate(CCFDRate *pCFDRate, CWriteableCFDRate *pNewCFDRate, CTransaction *pTransaction=NULL);
	
	///删除一个CCFDRate，同时删除其索引
	///@param	pCFDRate	要删除的CCFDRate
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	void remove(CCFDRate *pCFDRate, CTransaction *pTransaction=NULL, bool bNoTransaction=true);
	
	///获取某个CCFDRate
	///@param	pCFDRate	要读取的CCFDRate
	///@param	pTragetCFDRate	存放结果的CCFDRate
	void retrieve(CCFDRate *pCFDRate, CWriteableCFDRate *pTargetCFDRate);
	
	///获取第一个CCFDRate
	///@return	得到的第一个CCFDRate，如果没有，则返回NULL
	CCFDRate* getFirst();
	
	///获取下一个CCFDRate
	///@return	得到下一个CCFDRate，如果没有，则返回NULL
	CCFDRate* getNext();
	
	///加入一个操作触发器
	///@param	pTrigger	要加入的操作触发器
	///@return	1表示成功，0表示失败
	int addActionTrigger(CCFDRateActionTrigger *pTrigger);
	
	///删除一个操作触发器
	///@param	pTrigger	要删除的操作触发器
	///@return	1表示成功，0表示失败
	int removeActionTrigger(CCFDRateActionTrigger *pTrigger);

	///加入一个确认触发器
	///@param	pTrigger	要加入的确认触发器
	///@return	1表示成功，0表示失败
	int addCommitTrigger(CCFDRateCommitTrigger *pTrigger);
	
	///删除一个确认触发器
	///@param	pTrigger	要删除的确认触发器
	///@return	1表示成功，0表示失败
	int removeCommitTrigger(CCFDRateCommitTrigger *pTrigger);
	
	///检查是否字段为NULL
	///@param pFile 输出错误日志的文件
	void checkNull(FILE *pFile);
	
	///检查是否字段合法
	///@param pFile 输出错误日志的文件
	void checkValid(FILE *pFile);	

	///寻找CCFDRate
	///@return	找到的CCFDRate，如果找不到，返回NULL
	CCFDRate *findByMemberAndInstrument(const CReadOnlyLongIndexIDType&  MemberID, const CReadOnlyInstrumentIDType&  InstrumentID, const CReadOnlyLongIndexIDType&  TriggerOrderType);

	///根据主码寻找
	///@return	找到的CCFDRate，如果找不到，返回NULL
	CCFDRate *findByPK(const CReadOnlyLongIndexIDType& MemberID, const CReadOnlyInstrumentIDType& InstrumentID, const CReadOnlyLongIndexIDType& TriggerOrderType);
		
	///根据主键删除CCFDRate
	bool removeByPK(const CReadOnlyLongIndexIDType& MemberID, const CReadOnlyInstrumentIDType& InstrumentID, const CReadOnlyLongIndexIDType& TriggerOrderType, CTransaction *pTransaction=NULL);
	
	///判断字段是否是是主键
	bool isPK(const char* pName);
	
	///根据主键寻找CCFDRate
	///@param	pCFDRate	要找的值
	///@return	找到的CCFDRate，如果找不到，返回NULL
	CCFDRate *findByPK(CCFDRateField *pCFDRate);
	
	///根据主键寻找CCFDRate
	///@param	pCFDRate	要找的值
	///@return	找到的CCFDRate，如果找不到，返回NULL
	CCFDRate *findByPK(CCFDRate *pCFDRate);

	///根据主键获取CCFDRate
	///@param	pCFDRate	要获取的值，同时用于存放返回的结果
	///@return	找到的CCFDRate，如果找不到，返回NULL
	bool retrieveByPK(CWriteableCFDRate *pCFDRate);

	///根据主键增加新的CCFDRate
	///@param	pCFDRate	新的值
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	///@return	返回插入的CCFDRate，如果插入失败，则返回NULL
	CCFDRate *addByPK(CWriteableCFDRate *pCFDRate, CTransaction *pTransaction=NULL);
	
	///根据主键增加或更新新的CCFDRate
	///@param	pCFDRate	新的值
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	///@return	返回插入的CCFDRate，如果插入失败，则返回NULL
	CCFDRate *addOrUpdateByPK(CWriteableCFDRate *pCFDRate, CTransaction *pTransaction=NULL);

	///根据主键更新CCFDRate的值
	///@param	pCFDRate	新的值
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	///@return	true表示成功，false表示失败
	bool updateByPK(CWriteableCFDRate *pCFDRate, CTransaction *pTransaction=NULL);

	///根据主键删除CCFDRate
	///@param	pCFDRate	要删除的值
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	///@return	true表示成功，false表示失败
	bool removeByPK(CCFDRate *pCFDRate, CTransaction *pTransaction=NULL);
	
	///根据主键删除CCFDRate
	///@param	pCFDRate	要删除的值
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	///@return	true表示成功，false表示失败
	bool removeByPK(CCFDRateField *pCFDRate, CTransaction *pTransaction=NULL);	


private:
	///完成自己的初始化工作
	///@param	maxUnit	最多的数据对象个数
	///@param	pAllocator	进行内存管理的分配器
	///@param	pIndexMap	索引名称和内存编号的对应表，在非重用时会写入，在重用时只读出
	///@param	reuse	是否重用
	///@param	reuseID	重用编号，非重用时应当置为-1
	///@param	readOnly	1表示该CFixMem是只读的，0表示可以读写
	void init(int maxUnit,IMemoryAllocator *pAllocator,map<string,int>* pIndexMap,bool reuse,int reuseID,int readOnly);
	
	///将一个CCFDRate加入到该对象工厂，实质上是完成修改索引的工作
	///@param	pCFDRate	要加入的CFDRate
	///@param	bNoTransaction 是否不属于任何事务
	///@return	加入后该对象的位置，当pTransaction!=NULL时，总是返回NULL
	CCFDRate* internalAdd(CWriteableCFDRate *pCFDRate, bool bNoTransaction);
	
	
	///刷新该CCFDRate的键值
	///@param	pCFDRate	被刷新的CCFDRate
	///@param	pNewCFDRate	新的值
	///@param	bNoTransaction 是否不属于任何事务
	CCFDRate* internalUpdate(CCFDRate *pCFDRate, CWriteableCFDRate *pNewCFDRate, bool bNoTransaction);
	
	///删除一个CCFDRate，同时删除其索引
	///@param	pCFDRate	要删除的CCFDRate
	///@param	bNoTransaction 是否不属于任何事务
	void internalRemove(CCFDRate *pCFDRate, bool bNoTransaction);
/*
	///检查某个CCFDRate是否属于本对象工厂
	///@param	pCFDRate	要检查的对象
	///@exception	CDesignError	如果不属于，则抛出此异常
	void checkAddress(CCFDRate *pCFDRate);
*/

	
	///基于MemberAndInstrument的hash索引
	CHashIndex *pMemberAndInstrumentHashIndex;
	
		
	///确认加入后触发
	///@param	pCFDRate	已经加入的CFDRate
	virtual void commitAdd(CCFDRate *pCFDRate);

	///确认更新后触发
	///@param	pCFDRate	被刷新的CCFDRate
	///@param	poldCFDRate	原来的值
	virtual void commitUpdate(CCFDRate *pCFDRate, CWriteableCFDRate *pOldCFDRate);
	
	///确认删除后触发
	///@param	pCFDRate	已经删除的CCFDRate
	virtual void commitRemove(CWriteableCFDRate *pCFDRate);

	///所有相关的触发器
	vector<CCFDRateActionTrigger *> *pActionTriggers;
	vector<CCFDRateCommitTrigger *> *pCommitTriggers;
	
	///进行查询时，用于比较的对象
	CWriteableCFDRate compareObject;
	CMDB* m_pMDB;
};

/////////////////////////////////////////////////////////////////////////
///CFeeFactory是一个手续费率的对象工厂。它包含了一批手续费率，
///同时又建立了如下索引，以方便查询：
///使用本对象工厂，可以完成对手续费率的增删改查操作。具体方法如下：
///	增加：叫用add，完成加入
///	删除：叫用remove就可以删除
///	查询：有如下方法可以查询：
///		使用findByMemberIDAndInstrument，唯一找到对象
///	修改：对于查到的结果，可以通过retrieve取出，修改后，通过update放回
///	事务管理：所有的增加、删除和修改都可以指定事务，在事务commit时，真正写入
///	在retrieve时，也可以指定事务，此时将取出经过update的新数据
/////////////////////////////////////////////////////////////////////////
class CFeeFactory :public CDatabaseDataFactory
{
public:
	///构造方法
	///@param	maxUnit	初始的数据对象个数
	///@param	pAllocator	进行内存管理的分配器
	///@param	pIndexMap	索引名称和内存编号的对应表，在非重用时会写入，在重用时只读出
	///@param	reuse	是否重用
	///@param	reuseID	重用编号，非重用时应当置为-1
	///@param	readOnly	1表示该CFixMem是只读的，0表示可以读写
	CFeeFactory(CMDB* pMDB,int maxUnit,IMemoryAllocator *pAllocator,map<string,int>* pIndexMap,bool reuse=false, int reuseID=-1, int readOnly=0);
	
	///构造方法
	///@param	pAllocator	进行内存管理的分配器
	///@param	pIndexMap	索引名称和内存编号的对应表，在非重用时会写入，在重用时只读出
	///@param	reuse	是否重用
	///@param	reuseID	重用编号，非重用时应当置为-1
	///@param	readOnly	1表示该CFixMem是只读的，0表示可以读写
	CFeeFactory(CMDB* pMDB,IMemoryAllocator *pAllocator,map<string,int>* pIndexMap,bool reuse=false, int reuseID = -1,int readOnly=0);
	
	///析构方法，将负责释放所占用的空间
	virtual ~CFeeFactory();

	///从CSV文件中读取所有的Fee
	///@param	filename	文件名
	///@return	1表示完成，0表示没有完成
	virtual int readCSV(char *filename);

	///从CSV文件中读取所有的Fee，文件名将根据配置文件中的定义FeeCSVFile
	///@return	1表示完成，0表示没有完成
	virtual int readCSVByPath(char *pPath);
	
	///向CSV文件中写出所有的Fee
	///@param	filename	文件名
	///@return	1表示完成，0表示没有完成
	virtual int writeCSV(char *filename);
	
	///向CSV文件中写出上一个版本的Fee
	///@param	filename	文件名
	///@return	1表示完成，0表示没有完成
	virtual int writeCSVPreVersion(char *filename);	

	///向CSV文件中写出所有的Fee，文件名将根据配置文件中的定义FeeCSVFile
	///@return	1表示完成，0表示没有完成
	virtual int writeCSVByPath(char *pPath);

	///将内容写到文件中,用于DEBUG
	virtual void dump(FILE *);

	///将内容写到文件中,用于DEBUG
	virtual void dump(char *filename);

	///清空所有的记录
	virtual void clearAll();

	friend class CFeeResource;

	///将一个CFee加入到该对象工厂，实质上是完成修改索引的工作
	///@param	pFee	要加入的Fee
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	///@return	加入后该对象的位置，当pTransaction!=NULL时，总是返回NULL
	CFee* add(CWriteableFee *pFee, CTransaction *pTransaction=NULL);
	
	///刷新该CFee的值
	///@param	pFee	被刷新的CFee
	///@param	pNewFee	新的值
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	CFee* update(CFee *pFee, CWriteableFee *pNewFee, CTransaction *pTransaction=NULL, bool bNoTransaction = true);
	
	///增加或者更新数据
	///@param	pFee	  需要被刷新或者新增的CFee,等于NULL表示是需要新增的
	///@param	pNewFee	新的值
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	///@param	新增或者修改后的数据
	CFee* addOrUpdate(CFee *pFee, CWriteableFee *pNewFee, CTransaction *pTransaction=NULL);
	
	///删除一个CFee，同时删除其索引
	///@param	pFee	要删除的CFee
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	void remove(CFee *pFee, CTransaction *pTransaction=NULL, bool bNoTransaction=true);
	
	///获取某个CFee
	///@param	pFee	要读取的CFee
	///@param	pTragetFee	存放结果的CFee
	void retrieve(CFee *pFee, CWriteableFee *pTargetFee);
	
	///获取第一个CFee
	///@return	得到的第一个CFee，如果没有，则返回NULL
	CFee* getFirst();
	
	///获取下一个CFee
	///@return	得到下一个CFee，如果没有，则返回NULL
	CFee* getNext();
	
	///加入一个操作触发器
	///@param	pTrigger	要加入的操作触发器
	///@return	1表示成功，0表示失败
	int addActionTrigger(CFeeActionTrigger *pTrigger);
	
	///删除一个操作触发器
	///@param	pTrigger	要删除的操作触发器
	///@return	1表示成功，0表示失败
	int removeActionTrigger(CFeeActionTrigger *pTrigger);

	///加入一个确认触发器
	///@param	pTrigger	要加入的确认触发器
	///@return	1表示成功，0表示失败
	int addCommitTrigger(CFeeCommitTrigger *pTrigger);
	
	///删除一个确认触发器
	///@param	pTrigger	要删除的确认触发器
	///@return	1表示成功，0表示失败
	int removeCommitTrigger(CFeeCommitTrigger *pTrigger);
	
	///检查是否字段为NULL
	///@param pFile 输出错误日志的文件
	void checkNull(FILE *pFile);
	
	///检查是否字段合法
	///@param pFile 输出错误日志的文件
	void checkValid(FILE *pFile);	

	///寻找CFee
	///@return	找到的CFee，如果找不到，返回NULL
	CFee *findByMemberIDAndInstrument(const CReadOnlyLongIndexIDType&  MemberID, const CReadOnlyInstrumentIDType&  InstrumentID, const CReadOnlyMatchRoleType&  MatchRole);

	///根据主码寻找
	///@return	找到的CFee，如果找不到，返回NULL
	CFee *findByPK(const CReadOnlyLongIndexIDType& MemberID, const CReadOnlyInstrumentIDType& InstrumentID, const CReadOnlyMatchRoleType& MatchRole);
		
	///根据主键删除CFee
	bool removeByPK(const CReadOnlyLongIndexIDType& MemberID, const CReadOnlyInstrumentIDType& InstrumentID, const CReadOnlyMatchRoleType& MatchRole, CTransaction *pTransaction=NULL);
	
	///判断字段是否是是主键
	bool isPK(const char* pName);
	
	///根据主键寻找CFee
	///@param	pFee	要找的值
	///@return	找到的CFee，如果找不到，返回NULL
	CFee *findByPK(CFeeField *pFee);
	
	///根据主键寻找CFee
	///@param	pFee	要找的值
	///@return	找到的CFee，如果找不到，返回NULL
	CFee *findByPK(CFee *pFee);

	///根据主键获取CFee
	///@param	pFee	要获取的值，同时用于存放返回的结果
	///@return	找到的CFee，如果找不到，返回NULL
	bool retrieveByPK(CWriteableFee *pFee);

	///根据主键增加新的CFee
	///@param	pFee	新的值
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	///@return	返回插入的CFee，如果插入失败，则返回NULL
	CFee *addByPK(CWriteableFee *pFee, CTransaction *pTransaction=NULL);
	
	///根据主键增加或更新新的CFee
	///@param	pFee	新的值
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	///@return	返回插入的CFee，如果插入失败，则返回NULL
	CFee *addOrUpdateByPK(CWriteableFee *pFee, CTransaction *pTransaction=NULL);

	///根据主键更新CFee的值
	///@param	pFee	新的值
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	///@return	true表示成功，false表示失败
	bool updateByPK(CWriteableFee *pFee, CTransaction *pTransaction=NULL);

	///根据主键删除CFee
	///@param	pFee	要删除的值
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	///@return	true表示成功，false表示失败
	bool removeByPK(CFee *pFee, CTransaction *pTransaction=NULL);
	
	///根据主键删除CFee
	///@param	pFee	要删除的值
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	///@return	true表示成功，false表示失败
	bool removeByPK(CFeeField *pFee, CTransaction *pTransaction=NULL);	


private:
	///完成自己的初始化工作
	///@param	maxUnit	最多的数据对象个数
	///@param	pAllocator	进行内存管理的分配器
	///@param	pIndexMap	索引名称和内存编号的对应表，在非重用时会写入，在重用时只读出
	///@param	reuse	是否重用
	///@param	reuseID	重用编号，非重用时应当置为-1
	///@param	readOnly	1表示该CFixMem是只读的，0表示可以读写
	void init(int maxUnit,IMemoryAllocator *pAllocator,map<string,int>* pIndexMap,bool reuse,int reuseID,int readOnly);
	
	///将一个CFee加入到该对象工厂，实质上是完成修改索引的工作
	///@param	pFee	要加入的Fee
	///@param	bNoTransaction 是否不属于任何事务
	///@return	加入后该对象的位置，当pTransaction!=NULL时，总是返回NULL
	CFee* internalAdd(CWriteableFee *pFee, bool bNoTransaction);
	
	
	///刷新该CFee的键值
	///@param	pFee	被刷新的CFee
	///@param	pNewFee	新的值
	///@param	bNoTransaction 是否不属于任何事务
	CFee* internalUpdate(CFee *pFee, CWriteableFee *pNewFee, bool bNoTransaction);
	
	///删除一个CFee，同时删除其索引
	///@param	pFee	要删除的CFee
	///@param	bNoTransaction 是否不属于任何事务
	void internalRemove(CFee *pFee, bool bNoTransaction);
/*
	///检查某个CFee是否属于本对象工厂
	///@param	pFee	要检查的对象
	///@exception	CDesignError	如果不属于，则抛出此异常
	void checkAddress(CFee *pFee);
*/

	
	///基于MemberIDAndInstrument的hash索引
	CHashIndex *pMemberIDAndInstrumentHashIndex;
	
		
	///确认加入后触发
	///@param	pFee	已经加入的Fee
	virtual void commitAdd(CFee *pFee);

	///确认更新后触发
	///@param	pFee	被刷新的CFee
	///@param	poldFee	原来的值
	virtual void commitUpdate(CFee *pFee, CWriteableFee *pOldFee);
	
	///确认删除后触发
	///@param	pFee	已经删除的CFee
	virtual void commitRemove(CWriteableFee *pFee);

	///所有相关的触发器
	vector<CFeeActionTrigger *> *pActionTriggers;
	vector<CFeeCommitTrigger *> *pCommitTriggers;
	
	///进行查询时，用于比较的对象
	CWriteableFee compareObject;
	CMDB* m_pMDB;
};

/////////////////////////////////////////////////////////////////////////
///CCurrencyFactory是一个资金账户币种信息的对象工厂。它包含了一批资金账户币种信息，
///同时又建立了如下索引，以方便查询：
///	SettlementGroupIndex
///使用本对象工厂，可以完成对资金账户币种信息的增删改查操作。具体方法如下：
///	增加：叫用add，完成加入
///	删除：叫用remove就可以删除
///	查询：有如下方法可以查询：
///		使用findByCurrency，唯一找到对象
///		使用startFindBySettlementGroup，findNextBySettlementGroup完成查询操作
///	修改：对于查到的结果，可以通过retrieve取出，修改后，通过update放回
///	事务管理：所有的增加、删除和修改都可以指定事务，在事务commit时，真正写入
///	在retrieve时，也可以指定事务，此时将取出经过update的新数据
/////////////////////////////////////////////////////////////////////////
class CCurrencyFactory :public CDatabaseDataFactory
{
public:
	///构造方法
	///@param	maxUnit	初始的数据对象个数
	///@param	pAllocator	进行内存管理的分配器
	///@param	pIndexMap	索引名称和内存编号的对应表，在非重用时会写入，在重用时只读出
	///@param	reuse	是否重用
	///@param	reuseID	重用编号，非重用时应当置为-1
	///@param	readOnly	1表示该CFixMem是只读的，0表示可以读写
	CCurrencyFactory(CMDB* pMDB,int maxUnit,IMemoryAllocator *pAllocator,map<string,int>* pIndexMap,bool reuse=false, int reuseID=-1, int readOnly=0);
	
	///构造方法
	///@param	pAllocator	进行内存管理的分配器
	///@param	pIndexMap	索引名称和内存编号的对应表，在非重用时会写入，在重用时只读出
	///@param	reuse	是否重用
	///@param	reuseID	重用编号，非重用时应当置为-1
	///@param	readOnly	1表示该CFixMem是只读的，0表示可以读写
	CCurrencyFactory(CMDB* pMDB,IMemoryAllocator *pAllocator,map<string,int>* pIndexMap,bool reuse=false, int reuseID = -1,int readOnly=0);
	
	///析构方法，将负责释放所占用的空间
	virtual ~CCurrencyFactory();

	///从CSV文件中读取所有的Currency
	///@param	filename	文件名
	///@return	1表示完成，0表示没有完成
	virtual int readCSV(char *filename);

	///从CSV文件中读取所有的Currency，文件名将根据配置文件中的定义CurrencyCSVFile
	///@return	1表示完成，0表示没有完成
	virtual int readCSVByPath(char *pPath);
	
	///向CSV文件中写出所有的Currency
	///@param	filename	文件名
	///@return	1表示完成，0表示没有完成
	virtual int writeCSV(char *filename);
	
	///向CSV文件中写出上一个版本的Currency
	///@param	filename	文件名
	///@return	1表示完成，0表示没有完成
	virtual int writeCSVPreVersion(char *filename);	

	///向CSV文件中写出所有的Currency，文件名将根据配置文件中的定义CurrencyCSVFile
	///@return	1表示完成，0表示没有完成
	virtual int writeCSVByPath(char *pPath);

	///将内容写到文件中,用于DEBUG
	virtual void dump(FILE *);

	///将内容写到文件中,用于DEBUG
	virtual void dump(char *filename);

	///清空所有的记录
	virtual void clearAll();

	friend class CCurrencyResource;

	///将一个CCurrency加入到该对象工厂，实质上是完成修改索引的工作
	///@param	pCurrency	要加入的Currency
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	///@return	加入后该对象的位置，当pTransaction!=NULL时，总是返回NULL
	CCurrency* add(CWriteableCurrency *pCurrency, CTransaction *pTransaction=NULL);
	
	///刷新该CCurrency的值
	///@param	pCurrency	被刷新的CCurrency
	///@param	pNewCurrency	新的值
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	CCurrency* update(CCurrency *pCurrency, CWriteableCurrency *pNewCurrency, CTransaction *pTransaction=NULL, bool bNoTransaction = true);
	
	///增加或者更新数据
	///@param	pCurrency	  需要被刷新或者新增的CCurrency,等于NULL表示是需要新增的
	///@param	pNewCurrency	新的值
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	///@param	新增或者修改后的数据
	CCurrency* addOrUpdate(CCurrency *pCurrency, CWriteableCurrency *pNewCurrency, CTransaction *pTransaction=NULL);
	
	///删除一个CCurrency，同时删除其索引
	///@param	pCurrency	要删除的CCurrency
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	void remove(CCurrency *pCurrency, CTransaction *pTransaction=NULL, bool bNoTransaction=true);
	
	///获取某个CCurrency
	///@param	pCurrency	要读取的CCurrency
	///@param	pTragetCurrency	存放结果的CCurrency
	void retrieve(CCurrency *pCurrency, CWriteableCurrency *pTargetCurrency);
	
	///获取第一个CCurrency
	///@return	得到的第一个CCurrency，如果没有，则返回NULL
	CCurrency* getFirst();
	
	///获取下一个CCurrency
	///@return	得到下一个CCurrency，如果没有，则返回NULL
	CCurrency* getNext();
	
	///加入一个操作触发器
	///@param	pTrigger	要加入的操作触发器
	///@return	1表示成功，0表示失败
	int addActionTrigger(CCurrencyActionTrigger *pTrigger);
	
	///删除一个操作触发器
	///@param	pTrigger	要删除的操作触发器
	///@return	1表示成功，0表示失败
	int removeActionTrigger(CCurrencyActionTrigger *pTrigger);

	///加入一个确认触发器
	///@param	pTrigger	要加入的确认触发器
	///@return	1表示成功，0表示失败
	int addCommitTrigger(CCurrencyCommitTrigger *pTrigger);
	
	///删除一个确认触发器
	///@param	pTrigger	要删除的确认触发器
	///@return	1表示成功，0表示失败
	int removeCommitTrigger(CCurrencyCommitTrigger *pTrigger);
	
	///构造所有对应的行情的指针
	///@param	pFactory	构造对应的行情的Link的指针时，寻找的对象工厂
	void linkAllMarketData();

	///检查对应的行情的指针
	///@param pFile 输出错误日志的文件
	void checkLinkMarketData(FILE *pFile);
	
	///清空对应的行情的外键
	void clearLinkMarketData();
	
	///检查是否字段为NULL
	///@param pFile 输出错误日志的文件
	void checkNull(FILE *pFile);
	
	///检查是否字段合法
	///@param pFile 输出错误日志的文件
	void checkValid(FILE *pFile);	

	///寻找CCurrency
	///@return	找到的CCurrency，如果找不到，返回NULL
	CCurrency *findByCurrency(const CReadOnlyCurrencyType&  Currency, const CReadOnlyShortIndexIDType&  SettlementGroup);

	///根据主码寻找
	///@return	找到的CCurrency，如果找不到，返回NULL
	CCurrency *findByPK(const CReadOnlyCurrencyType& Currency, const CReadOnlyShortIndexIDType& SettlementGroup);
		
	///根据主键删除CCurrency
	bool removeByPK(const CReadOnlyCurrencyType& Currency, const CReadOnlyShortIndexIDType& SettlementGroup, CTransaction *pTransaction=NULL);
	
	///判断字段是否是是主键
	bool isPK(const char* pName);
	
	///根据主键寻找CCurrency
	///@param	pCurrency	要找的值
	///@return	找到的CCurrency，如果找不到，返回NULL
	CCurrency *findByPK(CCurrencyField *pCurrency);
	
	///根据主键寻找CCurrency
	///@param	pCurrency	要找的值
	///@return	找到的CCurrency，如果找不到，返回NULL
	CCurrency *findByPK(CCurrency *pCurrency);

	///根据主键获取CCurrency
	///@param	pCurrency	要获取的值，同时用于存放返回的结果
	///@return	找到的CCurrency，如果找不到，返回NULL
	bool retrieveByPK(CWriteableCurrency *pCurrency);

	///根据主键增加新的CCurrency
	///@param	pCurrency	新的值
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	///@return	返回插入的CCurrency，如果插入失败，则返回NULL
	CCurrency *addByPK(CWriteableCurrency *pCurrency, CTransaction *pTransaction=NULL);
	
	///根据主键增加或更新新的CCurrency
	///@param	pCurrency	新的值
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	///@return	返回插入的CCurrency，如果插入失败，则返回NULL
	CCurrency *addOrUpdateByPK(CWriteableCurrency *pCurrency, CTransaction *pTransaction=NULL);

	///根据主键更新CCurrency的值
	///@param	pCurrency	新的值
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	///@return	true表示成功，false表示失败
	bool updateByPK(CWriteableCurrency *pCurrency, CTransaction *pTransaction=NULL);

	///根据主键删除CCurrency
	///@param	pCurrency	要删除的值
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	///@return	true表示成功，false表示失败
	bool removeByPK(CCurrency *pCurrency, CTransaction *pTransaction=NULL);
	
	///根据主键删除CCurrency
	///@param	pCurrency	要删除的值
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	///@return	true表示成功，false表示失败
	bool removeByPK(CCurrencyField *pCurrency, CTransaction *pTransaction=NULL);	

	friend class CCurrencyIteratorBySettlementGroup;

	///寻找下一个符合条件的CCurrency，必须在startFindBySettlementGroup以后叫用
	///@return	下一个满足条件CCurrency，如果已经没有一个满足要求了，则返回NULL
	CCurrency *findNextBySettlementGroup();
	
	///寻找第一个CCurrency
	///@return	只寻找满足条件的第一个CCurrency，如果找不到，返回NULL
	CCurrency *findFirstBySettlementGroup( const CReadOnlyShortIndexIDType& SettlementGroup);
	

private:
	///完成自己的初始化工作
	///@param	maxUnit	最多的数据对象个数
	///@param	pAllocator	进行内存管理的分配器
	///@param	pIndexMap	索引名称和内存编号的对应表，在非重用时会写入，在重用时只读出
	///@param	reuse	是否重用
	///@param	reuseID	重用编号，非重用时应当置为-1
	///@param	readOnly	1表示该CFixMem是只读的，0表示可以读写
	void init(int maxUnit,IMemoryAllocator *pAllocator,map<string,int>* pIndexMap,bool reuse,int reuseID,int readOnly);
	
	///将一个CCurrency加入到该对象工厂，实质上是完成修改索引的工作
	///@param	pCurrency	要加入的Currency
	///@param	bNoTransaction 是否不属于任何事务
	///@return	加入后该对象的位置，当pTransaction!=NULL时，总是返回NULL
	CCurrency* internalAdd(CWriteableCurrency *pCurrency, bool bNoTransaction);
	
	
	///刷新该CCurrency的键值
	///@param	pCurrency	被刷新的CCurrency
	///@param	pNewCurrency	新的值
	///@param	bNoTransaction 是否不属于任何事务
	CCurrency* internalUpdate(CCurrency *pCurrency, CWriteableCurrency *pNewCurrency, bool bNoTransaction);
	
	///删除一个CCurrency，同时删除其索引
	///@param	pCurrency	要删除的CCurrency
	///@param	bNoTransaction 是否不属于任何事务
	void internalRemove(CCurrency *pCurrency, bool bNoTransaction);
/*
	///检查某个CCurrency是否属于本对象工厂
	///@param	pCurrency	要检查的对象
	///@exception	CDesignError	如果不属于，则抛出此异常
	void checkAddress(CCurrency *pCurrency);
*/

	///基于SettlementGroup的索引
	CAVLTree *pSettlementGroupIndex;
	
	///基于Currency的hash索引
	CHashIndex *pCurrencyHashIndex;
	
	///按照SettlementGroup检索时，存储的查询参数
	CShortIndexIDType querySettlementGroupInSearchBySettlementGroup;
	
	///按照SettlementGroup检索时，存储最后一次找到的索引节点
	CAVLNode *pLastFoundInSearchBySettlementGroup;
		
	///确认加入后触发
	///@param	pCurrency	已经加入的Currency
	virtual void commitAdd(CCurrency *pCurrency);

	///确认更新后触发
	///@param	pCurrency	被刷新的CCurrency
	///@param	poldCurrency	原来的值
	virtual void commitUpdate(CCurrency *pCurrency, CWriteableCurrency *pOldCurrency);
	
	///确认删除后触发
	///@param	pCurrency	已经删除的CCurrency
	virtual void commitRemove(CWriteableCurrency *pCurrency);

	///所有相关的触发器
	vector<CCurrencyActionTrigger *> *pActionTriggers;
	vector<CCurrencyCommitTrigger *> *pCommitTriggers;
	
	///进行查询时，用于比较的对象
	CWriteableCurrency compareObject;
	CMDB* m_pMDB;
};

/////////////////////////////////////////////////////////////////////////
///CLiquidityFactory是一个流动性设置表的对象工厂。它包含了一批流动性设置表，
///同时又建立了如下索引，以方便查询：
///使用本对象工厂，可以完成对流动性设置表的增删改查操作。具体方法如下：
///	增加：叫用add，完成加入
///	删除：叫用remove就可以删除
///	查询：有如下方法可以查询：
///		使用findByLiquidity，唯一找到对象
///	修改：对于查到的结果，可以通过retrieve取出，修改后，通过update放回
///	事务管理：所有的增加、删除和修改都可以指定事务，在事务commit时，真正写入
///	在retrieve时，也可以指定事务，此时将取出经过update的新数据
/////////////////////////////////////////////////////////////////////////
class CLiquidityFactory :public CDatabaseDataFactory
{
public:
	///构造方法
	///@param	maxUnit	初始的数据对象个数
	///@param	pAllocator	进行内存管理的分配器
	///@param	pIndexMap	索引名称和内存编号的对应表，在非重用时会写入，在重用时只读出
	///@param	reuse	是否重用
	///@param	reuseID	重用编号，非重用时应当置为-1
	///@param	readOnly	1表示该CFixMem是只读的，0表示可以读写
	CLiquidityFactory(CMDB* pMDB,int maxUnit,IMemoryAllocator *pAllocator,map<string,int>* pIndexMap,bool reuse=false, int reuseID=-1, int readOnly=0);
	
	///构造方法
	///@param	pAllocator	进行内存管理的分配器
	///@param	pIndexMap	索引名称和内存编号的对应表，在非重用时会写入，在重用时只读出
	///@param	reuse	是否重用
	///@param	reuseID	重用编号，非重用时应当置为-1
	///@param	readOnly	1表示该CFixMem是只读的，0表示可以读写
	CLiquidityFactory(CMDB* pMDB,IMemoryAllocator *pAllocator,map<string,int>* pIndexMap,bool reuse=false, int reuseID = -1,int readOnly=0);
	
	///析构方法，将负责释放所占用的空间
	virtual ~CLiquidityFactory();

	///从CSV文件中读取所有的Liquidity
	///@param	filename	文件名
	///@return	1表示完成，0表示没有完成
	virtual int readCSV(char *filename);

	///从CSV文件中读取所有的Liquidity，文件名将根据配置文件中的定义LiquidityCSVFile
	///@return	1表示完成，0表示没有完成
	virtual int readCSVByPath(char *pPath);
	
	///向CSV文件中写出所有的Liquidity
	///@param	filename	文件名
	///@return	1表示完成，0表示没有完成
	virtual int writeCSV(char *filename);
	
	///向CSV文件中写出上一个版本的Liquidity
	///@param	filename	文件名
	///@return	1表示完成，0表示没有完成
	virtual int writeCSVPreVersion(char *filename);	

	///向CSV文件中写出所有的Liquidity，文件名将根据配置文件中的定义LiquidityCSVFile
	///@return	1表示完成，0表示没有完成
	virtual int writeCSVByPath(char *pPath);

	///将内容写到文件中,用于DEBUG
	virtual void dump(FILE *);

	///将内容写到文件中,用于DEBUG
	virtual void dump(char *filename);

	///清空所有的记录
	virtual void clearAll();

	friend class CLiquidityResource;

	///将一个CLiquidity加入到该对象工厂，实质上是完成修改索引的工作
	///@param	pLiquidity	要加入的Liquidity
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	///@return	加入后该对象的位置，当pTransaction!=NULL时，总是返回NULL
	CLiquidity* add(CWriteableLiquidity *pLiquidity, CTransaction *pTransaction=NULL);
	
	///刷新该CLiquidity的值
	///@param	pLiquidity	被刷新的CLiquidity
	///@param	pNewLiquidity	新的值
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	CLiquidity* update(CLiquidity *pLiquidity, CWriteableLiquidity *pNewLiquidity, CTransaction *pTransaction=NULL, bool bNoTransaction = true);
	
	///增加或者更新数据
	///@param	pLiquidity	  需要被刷新或者新增的CLiquidity,等于NULL表示是需要新增的
	///@param	pNewLiquidity	新的值
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	///@param	新增或者修改后的数据
	CLiquidity* addOrUpdate(CLiquidity *pLiquidity, CWriteableLiquidity *pNewLiquidity, CTransaction *pTransaction=NULL);
	
	///删除一个CLiquidity，同时删除其索引
	///@param	pLiquidity	要删除的CLiquidity
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	void remove(CLiquidity *pLiquidity, CTransaction *pTransaction=NULL, bool bNoTransaction=true);
	
	///获取某个CLiquidity
	///@param	pLiquidity	要读取的CLiquidity
	///@param	pTragetLiquidity	存放结果的CLiquidity
	void retrieve(CLiquidity *pLiquidity, CWriteableLiquidity *pTargetLiquidity);
	
	///获取第一个CLiquidity
	///@return	得到的第一个CLiquidity，如果没有，则返回NULL
	CLiquidity* getFirst();
	
	///获取下一个CLiquidity
	///@return	得到下一个CLiquidity，如果没有，则返回NULL
	CLiquidity* getNext();
	
	///加入一个操作触发器
	///@param	pTrigger	要加入的操作触发器
	///@return	1表示成功，0表示失败
	int addActionTrigger(CLiquidityActionTrigger *pTrigger);
	
	///删除一个操作触发器
	///@param	pTrigger	要删除的操作触发器
	///@return	1表示成功，0表示失败
	int removeActionTrigger(CLiquidityActionTrigger *pTrigger);

	///加入一个确认触发器
	///@param	pTrigger	要加入的确认触发器
	///@return	1表示成功，0表示失败
	int addCommitTrigger(CLiquidityCommitTrigger *pTrigger);
	
	///删除一个确认触发器
	///@param	pTrigger	要删除的确认触发器
	///@return	1表示成功，0表示失败
	int removeCommitTrigger(CLiquidityCommitTrigger *pTrigger);
	
	///检查是否字段为NULL
	///@param pFile 输出错误日志的文件
	void checkNull(FILE *pFile);
	
	///检查是否字段合法
	///@param pFile 输出错误日志的文件
	void checkValid(FILE *pFile);	

	///寻找CLiquidity
	///@return	找到的CLiquidity，如果找不到，返回NULL
	CLiquidity *findByLiquidity(const CReadOnlyShortIndexIDType&  LiquidityGrade, const CReadOnlyLongIndexIDType&  LiquidityGroup);

	///根据主码寻找
	///@return	找到的CLiquidity，如果找不到，返回NULL
	CLiquidity *findByPK(const CReadOnlyShortIndexIDType& LiquidityGrade, const CReadOnlyLongIndexIDType& LiquidityGroup);
		
	///根据主键删除CLiquidity
	bool removeByPK(const CReadOnlyShortIndexIDType& LiquidityGrade, const CReadOnlyLongIndexIDType& LiquidityGroup, CTransaction *pTransaction=NULL);
	
	///判断字段是否是是主键
	bool isPK(const char* pName);
	
	///根据主键寻找CLiquidity
	///@param	pLiquidity	要找的值
	///@return	找到的CLiquidity，如果找不到，返回NULL
	CLiquidity *findByPK(CLiquidityField *pLiquidity);
	
	///根据主键寻找CLiquidity
	///@param	pLiquidity	要找的值
	///@return	找到的CLiquidity，如果找不到，返回NULL
	CLiquidity *findByPK(CLiquidity *pLiquidity);

	///根据主键获取CLiquidity
	///@param	pLiquidity	要获取的值，同时用于存放返回的结果
	///@return	找到的CLiquidity，如果找不到，返回NULL
	bool retrieveByPK(CWriteableLiquidity *pLiquidity);

	///根据主键增加新的CLiquidity
	///@param	pLiquidity	新的值
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	///@return	返回插入的CLiquidity，如果插入失败，则返回NULL
	CLiquidity *addByPK(CWriteableLiquidity *pLiquidity, CTransaction *pTransaction=NULL);
	
	///根据主键增加或更新新的CLiquidity
	///@param	pLiquidity	新的值
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	///@return	返回插入的CLiquidity，如果插入失败，则返回NULL
	CLiquidity *addOrUpdateByPK(CWriteableLiquidity *pLiquidity, CTransaction *pTransaction=NULL);

	///根据主键更新CLiquidity的值
	///@param	pLiquidity	新的值
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	///@return	true表示成功，false表示失败
	bool updateByPK(CWriteableLiquidity *pLiquidity, CTransaction *pTransaction=NULL);

	///根据主键删除CLiquidity
	///@param	pLiquidity	要删除的值
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	///@return	true表示成功，false表示失败
	bool removeByPK(CLiquidity *pLiquidity, CTransaction *pTransaction=NULL);
	
	///根据主键删除CLiquidity
	///@param	pLiquidity	要删除的值
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	///@return	true表示成功，false表示失败
	bool removeByPK(CLiquidityField *pLiquidity, CTransaction *pTransaction=NULL);	


private:
	///完成自己的初始化工作
	///@param	maxUnit	最多的数据对象个数
	///@param	pAllocator	进行内存管理的分配器
	///@param	pIndexMap	索引名称和内存编号的对应表，在非重用时会写入，在重用时只读出
	///@param	reuse	是否重用
	///@param	reuseID	重用编号，非重用时应当置为-1
	///@param	readOnly	1表示该CFixMem是只读的，0表示可以读写
	void init(int maxUnit,IMemoryAllocator *pAllocator,map<string,int>* pIndexMap,bool reuse,int reuseID,int readOnly);
	
	///将一个CLiquidity加入到该对象工厂，实质上是完成修改索引的工作
	///@param	pLiquidity	要加入的Liquidity
	///@param	bNoTransaction 是否不属于任何事务
	///@return	加入后该对象的位置，当pTransaction!=NULL时，总是返回NULL
	CLiquidity* internalAdd(CWriteableLiquidity *pLiquidity, bool bNoTransaction);
	
	
	///刷新该CLiquidity的键值
	///@param	pLiquidity	被刷新的CLiquidity
	///@param	pNewLiquidity	新的值
	///@param	bNoTransaction 是否不属于任何事务
	CLiquidity* internalUpdate(CLiquidity *pLiquidity, CWriteableLiquidity *pNewLiquidity, bool bNoTransaction);
	
	///删除一个CLiquidity，同时删除其索引
	///@param	pLiquidity	要删除的CLiquidity
	///@param	bNoTransaction 是否不属于任何事务
	void internalRemove(CLiquidity *pLiquidity, bool bNoTransaction);
/*
	///检查某个CLiquidity是否属于本对象工厂
	///@param	pLiquidity	要检查的对象
	///@exception	CDesignError	如果不属于，则抛出此异常
	void checkAddress(CLiquidity *pLiquidity);
*/

	
	///基于Liquidity的hash索引
	CHashIndex *pLiquidityHashIndex;
	
		
	///确认加入后触发
	///@param	pLiquidity	已经加入的Liquidity
	virtual void commitAdd(CLiquidity *pLiquidity);

	///确认更新后触发
	///@param	pLiquidity	被刷新的CLiquidity
	///@param	poldLiquidity	原来的值
	virtual void commitUpdate(CLiquidity *pLiquidity, CWriteableLiquidity *pOldLiquidity);
	
	///确认删除后触发
	///@param	pLiquidity	已经删除的CLiquidity
	virtual void commitRemove(CWriteableLiquidity *pLiquidity);

	///所有相关的触发器
	vector<CLiquidityActionTrigger *> *pActionTriggers;
	vector<CLiquidityCommitTrigger *> *pCommitTriggers;
	
	///进行查询时，用于比较的对象
	CWriteableLiquidity compareObject;
	CMDB* m_pMDB;
};

/////////////////////////////////////////////////////////////////////////
///CAccountFactory是一个资金账户的对象工厂。它包含了一批资金账户，
///同时又建立了如下索引，以方便查询：
///	MemberIndex
///	CurrencyIndex
///使用本对象工厂，可以完成对资金账户的增删改查操作。具体方法如下：
///	增加：叫用add，完成加入
///	删除：叫用remove就可以删除
///	查询：有如下方法可以查询：
///		使用findByAccountIDAndCurrency，唯一找到对象
///		使用startFindByMemberID，findNextByMemberID完成查询操作
///		使用startFindByAccountID，findNextByAccountID完成查询操作
///		使用startFindBySettlementGroup，findNextBySettlementGroup完成查询操作
///		使用startFindByAll，findNextByAll完成查询操作
///		使用startFindByCurrency，findNextByCurrency完成查询操作
///	修改：对于查到的结果，可以通过retrieve取出，修改后，通过update放回
///	事务管理：所有的增加、删除和修改都可以指定事务，在事务commit时，真正写入
///	在retrieve时，也可以指定事务，此时将取出经过update的新数据
/////////////////////////////////////////////////////////////////////////
class CAccountFactory :public CDatabaseDataFactory
{
public:
	///构造方法
	///@param	maxUnit	初始的数据对象个数
	///@param	pAllocator	进行内存管理的分配器
	///@param	pIndexMap	索引名称和内存编号的对应表，在非重用时会写入，在重用时只读出
	///@param	reuse	是否重用
	///@param	reuseID	重用编号，非重用时应当置为-1
	///@param	readOnly	1表示该CFixMem是只读的，0表示可以读写
	CAccountFactory(CMDB* pMDB,int maxUnit,IMemoryAllocator *pAllocator,map<string,int>* pIndexMap,bool reuse=false, int reuseID=-1, int readOnly=0);
	
	///构造方法
	///@param	pAllocator	进行内存管理的分配器
	///@param	pIndexMap	索引名称和内存编号的对应表，在非重用时会写入，在重用时只读出
	///@param	reuse	是否重用
	///@param	reuseID	重用编号，非重用时应当置为-1
	///@param	readOnly	1表示该CFixMem是只读的，0表示可以读写
	CAccountFactory(CMDB* pMDB,IMemoryAllocator *pAllocator,map<string,int>* pIndexMap,bool reuse=false, int reuseID = -1,int readOnly=0);
	
	///析构方法，将负责释放所占用的空间
	virtual ~CAccountFactory();

	///从CSV文件中读取所有的Account
	///@param	filename	文件名
	///@return	1表示完成，0表示没有完成
	virtual int readCSV(char *filename);

	///从CSV文件中读取所有的Account，文件名将根据配置文件中的定义AccountCSVFile
	///@return	1表示完成，0表示没有完成
	virtual int readCSVByPath(char *pPath);
	
	///向CSV文件中写出所有的Account
	///@param	filename	文件名
	///@return	1表示完成，0表示没有完成
	virtual int writeCSV(char *filename);
	
	///向CSV文件中写出上一个版本的Account
	///@param	filename	文件名
	///@return	1表示完成，0表示没有完成
	virtual int writeCSVPreVersion(char *filename);	

	///向CSV文件中写出所有的Account，文件名将根据配置文件中的定义AccountCSVFile
	///@return	1表示完成，0表示没有完成
	virtual int writeCSVByPath(char *pPath);

	///将内容写到文件中,用于DEBUG
	virtual void dump(FILE *);

	///将内容写到文件中,用于DEBUG
	virtual void dump(char *filename);

	///清空所有的记录
	virtual void clearAll();

	friend class CAccountResource;

	///将一个CAccount加入到该对象工厂，实质上是完成修改索引的工作
	///@param	pAccount	要加入的Account
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	///@return	加入后该对象的位置，当pTransaction!=NULL时，总是返回NULL
	CAccount* add(CWriteableAccount *pAccount, CTransaction *pTransaction=NULL);
	
	///刷新该CAccount的值
	///@param	pAccount	被刷新的CAccount
	///@param	pNewAccount	新的值
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	CAccount* update(CAccount *pAccount, CWriteableAccount *pNewAccount, CTransaction *pTransaction=NULL, bool bNoTransaction = true);
	
	///增加或者更新数据
	///@param	pAccount	  需要被刷新或者新增的CAccount,等于NULL表示是需要新增的
	///@param	pNewAccount	新的值
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	///@param	新增或者修改后的数据
	CAccount* addOrUpdate(CAccount *pAccount, CWriteableAccount *pNewAccount, CTransaction *pTransaction=NULL);
	
	///删除一个CAccount，同时删除其索引
	///@param	pAccount	要删除的CAccount
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	void remove(CAccount *pAccount, CTransaction *pTransaction=NULL, bool bNoTransaction=true);
	
	///获取某个CAccount
	///@param	pAccount	要读取的CAccount
	///@param	pTragetAccount	存放结果的CAccount
	void retrieve(CAccount *pAccount, CWriteableAccount *pTargetAccount);
	
	///获取第一个CAccount
	///@return	得到的第一个CAccount，如果没有，则返回NULL
	CAccount* getFirst();
	
	///获取下一个CAccount
	///@return	得到下一个CAccount，如果没有，则返回NULL
	CAccount* getNext();
	
	///加入一个操作触发器
	///@param	pTrigger	要加入的操作触发器
	///@return	1表示成功，0表示失败
	int addActionTrigger(CAccountActionTrigger *pTrigger);
	
	///删除一个操作触发器
	///@param	pTrigger	要删除的操作触发器
	///@return	1表示成功，0表示失败
	int removeActionTrigger(CAccountActionTrigger *pTrigger);

	///加入一个确认触发器
	///@param	pTrigger	要加入的确认触发器
	///@return	1表示成功，0表示失败
	int addCommitTrigger(CAccountCommitTrigger *pTrigger);
	
	///删除一个确认触发器
	///@param	pTrigger	要删除的确认触发器
	///@return	1表示成功，0表示失败
	int removeCommitTrigger(CAccountCommitTrigger *pTrigger);
	
	///构造所有币种的指针
	///@param	pFactory	构造币种的Link的指针时，寻找的对象工厂
	void linkAllCurrency();

	///构造所有对应的用户的指针
	///@param	pFactory	构造对应的用户的Link的指针时，寻找的对象工厂
	void linkAllMember();

	///检查币种的指针
	///@param pFile 输出错误日志的文件
	void checkLinkCurrency(FILE *pFile);
	
	///检查对应的用户的指针
	///@param pFile 输出错误日志的文件
	void checkLinkMember(FILE *pFile);
	
	///清空币种的外键
	void clearLinkCurrency();
	
	///清空对应的用户的外键
	void clearLinkMember();
	
	///检查是否字段为NULL
	///@param pFile 输出错误日志的文件
	void checkNull(FILE *pFile);
	
	///检查是否字段合法
	///@param pFile 输出错误日志的文件
	void checkValid(FILE *pFile);	

	///寻找CAccount
	///@return	找到的CAccount，如果找不到，返回NULL
	CAccount *findByAccountIDAndCurrency(const CReadOnlyLongIndexIDType&  MemberID, const CReadOnlyAccountIDType&  AccountID, const CReadOnlyShortIndexIDType&  SettlementGroup, const CReadOnlyCurrencyType&  Currency);

	///根据主码寻找
	///@return	找到的CAccount，如果找不到，返回NULL
	CAccount *findByPK(const CReadOnlyLongIndexIDType& MemberID, const CReadOnlyAccountIDType& AccountID, const CReadOnlyShortIndexIDType& SettlementGroup, const CReadOnlyCurrencyType& Currency);
		
	///根据主键删除CAccount
	bool removeByPK(const CReadOnlyLongIndexIDType& MemberID, const CReadOnlyAccountIDType& AccountID, const CReadOnlyShortIndexIDType& SettlementGroup, const CReadOnlyCurrencyType& Currency, CTransaction *pTransaction=NULL);
	
	///判断字段是否是是主键
	bool isPK(const char* pName);
	
	///根据主键寻找CAccount
	///@param	pAccount	要找的值
	///@return	找到的CAccount，如果找不到，返回NULL
	CAccount *findByPK(CAccountField *pAccount);
	
	///根据主键寻找CAccount
	///@param	pAccount	要找的值
	///@return	找到的CAccount，如果找不到，返回NULL
	CAccount *findByPK(CAccount *pAccount);

	///根据主键获取CAccount
	///@param	pAccount	要获取的值，同时用于存放返回的结果
	///@return	找到的CAccount，如果找不到，返回NULL
	bool retrieveByPK(CWriteableAccount *pAccount);

	///根据主键增加新的CAccount
	///@param	pAccount	新的值
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	///@return	返回插入的CAccount，如果插入失败，则返回NULL
	CAccount *addByPK(CWriteableAccount *pAccount, CTransaction *pTransaction=NULL);
	
	///根据主键增加或更新新的CAccount
	///@param	pAccount	新的值
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	///@return	返回插入的CAccount，如果插入失败，则返回NULL
	CAccount *addOrUpdateByPK(CWriteableAccount *pAccount, CTransaction *pTransaction=NULL);

	///根据主键更新CAccount的值
	///@param	pAccount	新的值
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	///@return	true表示成功，false表示失败
	bool updateByPK(CWriteableAccount *pAccount, CTransaction *pTransaction=NULL);

	///根据主键删除CAccount
	///@param	pAccount	要删除的值
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	///@return	true表示成功，false表示失败
	bool removeByPK(CAccount *pAccount, CTransaction *pTransaction=NULL);
	
	///根据主键删除CAccount
	///@param	pAccount	要删除的值
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	///@return	true表示成功，false表示失败
	bool removeByPK(CAccountField *pAccount, CTransaction *pTransaction=NULL);	

	friend class CAccountIteratorByMemberID;

	///寻找下一个符合条件的CAccount，必须在startFindByMemberID以后叫用
	///@return	下一个满足条件CAccount，如果已经没有一个满足要求了，则返回NULL
	CAccount *findNextByMemberID();
	
	///寻找第一个CAccount
	///@return	只寻找满足条件的第一个CAccount，如果找不到，返回NULL
	CAccount *findFirstByMemberID( const CReadOnlyLongIndexIDType& MemberID);
	
	friend class CAccountIteratorByAccountID;

	///寻找下一个符合条件的CAccount，必须在startFindByAccountID以后叫用
	///@return	下一个满足条件CAccount，如果已经没有一个满足要求了，则返回NULL
	CAccount *findNextByAccountID();
	
	///寻找第一个CAccount
	///@return	只寻找满足条件的第一个CAccount，如果找不到，返回NULL
	CAccount *findFirstByAccountID( const CReadOnlyLongIndexIDType& MemberID,  const CReadOnlyAccountIDType& AccountID);
	
	friend class CAccountIteratorBySettlementGroup;

	///寻找下一个符合条件的CAccount，必须在startFindBySettlementGroup以后叫用
	///@return	下一个满足条件CAccount，如果已经没有一个满足要求了，则返回NULL
	CAccount *findNextBySettlementGroup();
	
	///寻找第一个CAccount
	///@return	只寻找满足条件的第一个CAccount，如果找不到，返回NULL
	CAccount *findFirstBySettlementGroup( const CReadOnlyLongIndexIDType& MemberID,  const CReadOnlyAccountIDType& AccountID,  const CReadOnlyShortIndexIDType& SettlementGroup);
	
	friend class CAccountIteratorByAll;

	///寻找下一个符合条件的CAccount，必须在startFindByAll以后叫用
	///@return	下一个满足条件CAccount，如果已经没有一个满足要求了，则返回NULL
	CAccount *findNextByAll();
	
	///寻找第一个CAccount
	///@return	只寻找满足条件的第一个CAccount，如果找不到，返回NULL
	CAccount *findFirstByAll();
	
	friend class CAccountIteratorByCurrency;

	///寻找下一个符合条件的CAccount，必须在startFindByCurrency以后叫用
	///@return	下一个满足条件CAccount，如果已经没有一个满足要求了，则返回NULL
	CAccount *findNextByCurrency();
	
	///寻找第一个CAccount
	///@return	只寻找满足条件的第一个CAccount，如果找不到，返回NULL
	CAccount *findFirstByCurrency( const CReadOnlyShortIndexIDType& SettlementGroup,  const CReadOnlyCurrencyType& Currency);
	

private:
	///完成自己的初始化工作
	///@param	maxUnit	最多的数据对象个数
	///@param	pAllocator	进行内存管理的分配器
	///@param	pIndexMap	索引名称和内存编号的对应表，在非重用时会写入，在重用时只读出
	///@param	reuse	是否重用
	///@param	reuseID	重用编号，非重用时应当置为-1
	///@param	readOnly	1表示该CFixMem是只读的，0表示可以读写
	void init(int maxUnit,IMemoryAllocator *pAllocator,map<string,int>* pIndexMap,bool reuse,int reuseID,int readOnly);
	
	///将一个CAccount加入到该对象工厂，实质上是完成修改索引的工作
	///@param	pAccount	要加入的Account
	///@param	bNoTransaction 是否不属于任何事务
	///@return	加入后该对象的位置，当pTransaction!=NULL时，总是返回NULL
	CAccount* internalAdd(CWriteableAccount *pAccount, bool bNoTransaction);
	
	
	///刷新该CAccount的键值
	///@param	pAccount	被刷新的CAccount
	///@param	pNewAccount	新的值
	///@param	bNoTransaction 是否不属于任何事务
	CAccount* internalUpdate(CAccount *pAccount, CWriteableAccount *pNewAccount, bool bNoTransaction);
	
	///删除一个CAccount，同时删除其索引
	///@param	pAccount	要删除的CAccount
	///@param	bNoTransaction 是否不属于任何事务
	void internalRemove(CAccount *pAccount, bool bNoTransaction);
/*
	///检查某个CAccount是否属于本对象工厂
	///@param	pAccount	要检查的对象
	///@exception	CDesignError	如果不属于，则抛出此异常
	void checkAddress(CAccount *pAccount);
*/

	///基于MemberID+AccountID+SettlementGroup+Currency的索引
	CAVLTree *pMemberIndex;
	///基于SettlementGroup+Currency+Balance的索引
	CAVLTree *pCurrencyIndex;
	
	
	///按照MemberID检索时，存储的查询参数
	CLongIndexIDType queryMemberIDInSearchByMemberID;
	
	///按照MemberID检索时，存储最后一次找到的索引节点
	CAVLNode *pLastFoundInSearchByMemberID;
	///按照MemberID，AccountID检索时，存储的查询参数
	CLongIndexIDType queryMemberIDInSearchByAccountID;
	CAccountIDType queryAccountIDInSearchByAccountID;
	
	///按照MemberID，AccountID检索时，存储最后一次找到的索引节点
	CAVLNode *pLastFoundInSearchByAccountID;
	///按照MemberID，AccountID，SettlementGroup检索时，存储的查询参数
	CLongIndexIDType queryMemberIDInSearchBySettlementGroup;
	CAccountIDType queryAccountIDInSearchBySettlementGroup;
	CShortIndexIDType querySettlementGroupInSearchBySettlementGroup;
	
	///按照MemberID，AccountID，SettlementGroup检索时，存储最后一次找到的索引节点
	CAVLNode *pLastFoundInSearchBySettlementGroup;
	///按照检索时，存储的查询参数
	
	///按照检索时，存储最后一次找到的索引节点
	CAVLNode *pLastFoundInSearchByAll;
	///按照SettlementGroup，Currency检索时，存储的查询参数
	CShortIndexIDType querySettlementGroupInSearchByCurrency;
	CCurrencyType queryCurrencyInSearchByCurrency;
	
	///按照SettlementGroup，Currency检索时，存储最后一次找到的索引节点
	CAVLNode *pLastFoundInSearchByCurrency;
		
	///确认加入后触发
	///@param	pAccount	已经加入的Account
	virtual void commitAdd(CAccount *pAccount);

	///确认更新后触发
	///@param	pAccount	被刷新的CAccount
	///@param	poldAccount	原来的值
	virtual void commitUpdate(CAccount *pAccount, CWriteableAccount *pOldAccount);
	
	///确认删除后触发
	///@param	pAccount	已经删除的CAccount
	virtual void commitRemove(CWriteableAccount *pAccount);

	///所有相关的触发器
	vector<CAccountActionTrigger *> *pActionTriggers;
	vector<CAccountCommitTrigger *> *pCommitTriggers;
	
	///进行查询时，用于比较的对象
	CWriteableAccount compareObject;
	CMDB* m_pMDB;
};

/////////////////////////////////////////////////////////////////////////
///CMemberPositionFactory是一个成员总持仓的对象工厂。它包含了一批成员总持仓，
///同时又建立了如下索引，以方便查询：
///使用本对象工厂，可以完成对成员总持仓的增删改查操作。具体方法如下：
///	增加：叫用add，完成加入
///	删除：叫用remove就可以删除
///	查询：有如下方法可以查询：
///		使用findByInstrument，唯一找到对象
///	修改：对于查到的结果，可以通过retrieve取出，修改后，通过update放回
///	事务管理：所有的增加、删除和修改都可以指定事务，在事务commit时，真正写入
///	在retrieve时，也可以指定事务，此时将取出经过update的新数据
/////////////////////////////////////////////////////////////////////////
class CMemberPositionFactory :public CDatabaseDataFactory
{
public:
	///构造方法
	///@param	maxUnit	初始的数据对象个数
	///@param	pAllocator	进行内存管理的分配器
	///@param	pIndexMap	索引名称和内存编号的对应表，在非重用时会写入，在重用时只读出
	///@param	reuse	是否重用
	///@param	reuseID	重用编号，非重用时应当置为-1
	///@param	readOnly	1表示该CFixMem是只读的，0表示可以读写
	CMemberPositionFactory(CMDB* pMDB,int maxUnit,IMemoryAllocator *pAllocator,map<string,int>* pIndexMap,bool reuse=false, int reuseID=-1, int readOnly=0);
	
	///构造方法
	///@param	pAllocator	进行内存管理的分配器
	///@param	pIndexMap	索引名称和内存编号的对应表，在非重用时会写入，在重用时只读出
	///@param	reuse	是否重用
	///@param	reuseID	重用编号，非重用时应当置为-1
	///@param	readOnly	1表示该CFixMem是只读的，0表示可以读写
	CMemberPositionFactory(CMDB* pMDB,IMemoryAllocator *pAllocator,map<string,int>* pIndexMap,bool reuse=false, int reuseID = -1,int readOnly=0);
	
	///析构方法，将负责释放所占用的空间
	virtual ~CMemberPositionFactory();

	///从CSV文件中读取所有的MemberPosition
	///@param	filename	文件名
	///@return	1表示完成，0表示没有完成
	virtual int readCSV(char *filename);

	///从CSV文件中读取所有的MemberPosition，文件名将根据配置文件中的定义MemberPositionCSVFile
	///@return	1表示完成，0表示没有完成
	virtual int readCSVByPath(char *pPath);
	
	///向CSV文件中写出所有的MemberPosition
	///@param	filename	文件名
	///@return	1表示完成，0表示没有完成
	virtual int writeCSV(char *filename);
	
	///向CSV文件中写出上一个版本的MemberPosition
	///@param	filename	文件名
	///@return	1表示完成，0表示没有完成
	virtual int writeCSVPreVersion(char *filename);	

	///向CSV文件中写出所有的MemberPosition，文件名将根据配置文件中的定义MemberPositionCSVFile
	///@return	1表示完成，0表示没有完成
	virtual int writeCSVByPath(char *pPath);

	///将内容写到文件中,用于DEBUG
	virtual void dump(FILE *);

	///将内容写到文件中,用于DEBUG
	virtual void dump(char *filename);

	///清空所有的记录
	virtual void clearAll();

	friend class CMemberPositionResource;

	///将一个CMemberPosition加入到该对象工厂，实质上是完成修改索引的工作
	///@param	pMemberPosition	要加入的MemberPosition
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	///@return	加入后该对象的位置，当pTransaction!=NULL时，总是返回NULL
	CMemberPosition* add(CWriteableMemberPosition *pMemberPosition, CTransaction *pTransaction=NULL);
	
	///刷新该CMemberPosition的值
	///@param	pMemberPosition	被刷新的CMemberPosition
	///@param	pNewMemberPosition	新的值
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	CMemberPosition* update(CMemberPosition *pMemberPosition, CWriteableMemberPosition *pNewMemberPosition, CTransaction *pTransaction=NULL, bool bNoTransaction = true);
	
	///增加或者更新数据
	///@param	pMemberPosition	  需要被刷新或者新增的CMemberPosition,等于NULL表示是需要新增的
	///@param	pNewMemberPosition	新的值
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	///@param	新增或者修改后的数据
	CMemberPosition* addOrUpdate(CMemberPosition *pMemberPosition, CWriteableMemberPosition *pNewMemberPosition, CTransaction *pTransaction=NULL);
	
	///删除一个CMemberPosition，同时删除其索引
	///@param	pMemberPosition	要删除的CMemberPosition
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	void remove(CMemberPosition *pMemberPosition, CTransaction *pTransaction=NULL, bool bNoTransaction=true);
	
	///获取某个CMemberPosition
	///@param	pMemberPosition	要读取的CMemberPosition
	///@param	pTragetMemberPosition	存放结果的CMemberPosition
	void retrieve(CMemberPosition *pMemberPosition, CWriteableMemberPosition *pTargetMemberPosition);
	
	///获取第一个CMemberPosition
	///@return	得到的第一个CMemberPosition，如果没有，则返回NULL
	CMemberPosition* getFirst();
	
	///获取下一个CMemberPosition
	///@return	得到下一个CMemberPosition，如果没有，则返回NULL
	CMemberPosition* getNext();
	
	///加入一个操作触发器
	///@param	pTrigger	要加入的操作触发器
	///@return	1表示成功，0表示失败
	int addActionTrigger(CMemberPositionActionTrigger *pTrigger);
	
	///删除一个操作触发器
	///@param	pTrigger	要删除的操作触发器
	///@return	1表示成功，0表示失败
	int removeActionTrigger(CMemberPositionActionTrigger *pTrigger);

	///加入一个确认触发器
	///@param	pTrigger	要加入的确认触发器
	///@return	1表示成功，0表示失败
	int addCommitTrigger(CMemberPositionCommitTrigger *pTrigger);
	
	///删除一个确认触发器
	///@param	pTrigger	要删除的确认触发器
	///@return	1表示成功，0表示失败
	int removeCommitTrigger(CMemberPositionCommitTrigger *pTrigger);
	
	///检查是否字段为NULL
	///@param pFile 输出错误日志的文件
	void checkNull(FILE *pFile);
	
	///检查是否字段合法
	///@param pFile 输出错误日志的文件
	void checkValid(FILE *pFile);	

	///寻找CMemberPosition
	///@return	找到的CMemberPosition，如果找不到，返回NULL
	CMemberPosition *findByInstrument(const CReadOnlyAccountIDType&  AccountID, const CReadOnlyExchangeIDType&  ExchangeID, const CReadOnlyInstrumentIDType&  InstrumentID);

	///根据主码寻找
	///@return	找到的CMemberPosition，如果找不到，返回NULL
	CMemberPosition *findByPK(const CReadOnlyAccountIDType& AccountID, const CReadOnlyExchangeIDType& ExchangeID, const CReadOnlyInstrumentIDType& InstrumentID);
		
	///根据主键删除CMemberPosition
	bool removeByPK(const CReadOnlyAccountIDType& AccountID, const CReadOnlyExchangeIDType& ExchangeID, const CReadOnlyInstrumentIDType& InstrumentID, CTransaction *pTransaction=NULL);
	
	///判断字段是否是是主键
	bool isPK(const char* pName);
	
	///根据主键寻找CMemberPosition
	///@param	pMemberPosition	要找的值
	///@return	找到的CMemberPosition，如果找不到，返回NULL
	CMemberPosition *findByPK(CMemberPositionField *pMemberPosition);
	
	///根据主键寻找CMemberPosition
	///@param	pMemberPosition	要找的值
	///@return	找到的CMemberPosition，如果找不到，返回NULL
	CMemberPosition *findByPK(CMemberPosition *pMemberPosition);

	///根据主键获取CMemberPosition
	///@param	pMemberPosition	要获取的值，同时用于存放返回的结果
	///@return	找到的CMemberPosition，如果找不到，返回NULL
	bool retrieveByPK(CWriteableMemberPosition *pMemberPosition);

	///根据主键增加新的CMemberPosition
	///@param	pMemberPosition	新的值
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	///@return	返回插入的CMemberPosition，如果插入失败，则返回NULL
	CMemberPosition *addByPK(CWriteableMemberPosition *pMemberPosition, CTransaction *pTransaction=NULL);
	
	///根据主键增加或更新新的CMemberPosition
	///@param	pMemberPosition	新的值
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	///@return	返回插入的CMemberPosition，如果插入失败，则返回NULL
	CMemberPosition *addOrUpdateByPK(CWriteableMemberPosition *pMemberPosition, CTransaction *pTransaction=NULL);

	///根据主键更新CMemberPosition的值
	///@param	pMemberPosition	新的值
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	///@return	true表示成功，false表示失败
	bool updateByPK(CWriteableMemberPosition *pMemberPosition, CTransaction *pTransaction=NULL);

	///根据主键删除CMemberPosition
	///@param	pMemberPosition	要删除的值
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	///@return	true表示成功，false表示失败
	bool removeByPK(CMemberPosition *pMemberPosition, CTransaction *pTransaction=NULL);
	
	///根据主键删除CMemberPosition
	///@param	pMemberPosition	要删除的值
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	///@return	true表示成功，false表示失败
	bool removeByPK(CMemberPositionField *pMemberPosition, CTransaction *pTransaction=NULL);	


private:
	///完成自己的初始化工作
	///@param	maxUnit	最多的数据对象个数
	///@param	pAllocator	进行内存管理的分配器
	///@param	pIndexMap	索引名称和内存编号的对应表，在非重用时会写入，在重用时只读出
	///@param	reuse	是否重用
	///@param	reuseID	重用编号，非重用时应当置为-1
	///@param	readOnly	1表示该CFixMem是只读的，0表示可以读写
	void init(int maxUnit,IMemoryAllocator *pAllocator,map<string,int>* pIndexMap,bool reuse,int reuseID,int readOnly);
	
	///将一个CMemberPosition加入到该对象工厂，实质上是完成修改索引的工作
	///@param	pMemberPosition	要加入的MemberPosition
	///@param	bNoTransaction 是否不属于任何事务
	///@return	加入后该对象的位置，当pTransaction!=NULL时，总是返回NULL
	CMemberPosition* internalAdd(CWriteableMemberPosition *pMemberPosition, bool bNoTransaction);
	
	
	///刷新该CMemberPosition的键值
	///@param	pMemberPosition	被刷新的CMemberPosition
	///@param	pNewMemberPosition	新的值
	///@param	bNoTransaction 是否不属于任何事务
	CMemberPosition* internalUpdate(CMemberPosition *pMemberPosition, CWriteableMemberPosition *pNewMemberPosition, bool bNoTransaction);
	
	///删除一个CMemberPosition，同时删除其索引
	///@param	pMemberPosition	要删除的CMemberPosition
	///@param	bNoTransaction 是否不属于任何事务
	void internalRemove(CMemberPosition *pMemberPosition, bool bNoTransaction);
/*
	///检查某个CMemberPosition是否属于本对象工厂
	///@param	pMemberPosition	要检查的对象
	///@exception	CDesignError	如果不属于，则抛出此异常
	void checkAddress(CMemberPosition *pMemberPosition);
*/

	
	///基于Instrument的hash索引
	CHashIndex *pInstrumentHashIndex;
	
		
	///确认加入后触发
	///@param	pMemberPosition	已经加入的MemberPosition
	virtual void commitAdd(CMemberPosition *pMemberPosition);

	///确认更新后触发
	///@param	pMemberPosition	被刷新的CMemberPosition
	///@param	poldMemberPosition	原来的值
	virtual void commitUpdate(CMemberPosition *pMemberPosition, CWriteableMemberPosition *pOldMemberPosition);
	
	///确认删除后触发
	///@param	pMemberPosition	已经删除的CMemberPosition
	virtual void commitRemove(CWriteableMemberPosition *pMemberPosition);

	///所有相关的触发器
	vector<CMemberPositionActionTrigger *> *pActionTriggers;
	vector<CMemberPositionCommitTrigger *> *pCommitTriggers;
	
	///进行查询时，用于比较的对象
	CWriteableMemberPosition compareObject;
	CMDB* m_pMDB;
};

/////////////////////////////////////////////////////////////////////////
///CPositionFactory是一个交易单元持仓的对象工厂。它包含了一批交易单元持仓，
///同时又建立了如下索引，以方便查询：
///	AccountIndex
///	LiquidPriceLongIndex
///	LiquidPriceShortIndex
///使用本对象工厂，可以完成对交易单元持仓的增删改查操作。具体方法如下：
///	增加：叫用add，完成加入
///	删除：叫用remove就可以删除
///	查询：有如下方法可以查询：
///		使用findByPositionID，唯一找到对象
///		使用startFindByAccountInstrument，findNextByAccountInstrument完成查询操作
///		使用findByTradeUnitID，唯一找到对象
///		使用startFindByAccountID，findNextByAccountID完成查询操作
///		使用startFindByLiquidPriceLong，findNextByLiquidPriceLong完成查询操作
///		使用startFindByLiquidPriceShort，findNextByLiquidPriceShort完成查询操作
///	修改：对于查到的结果，可以通过retrieve取出，修改后，通过update放回
///	事务管理：所有的增加、删除和修改都可以指定事务，在事务commit时，真正写入
///	在retrieve时，也可以指定事务，此时将取出经过update的新数据
/////////////////////////////////////////////////////////////////////////
class CPositionFactory :public CDatabaseDataFactory
{
public:
	///构造方法
	///@param	maxUnit	初始的数据对象个数
	///@param	pAllocator	进行内存管理的分配器
	///@param	pIndexMap	索引名称和内存编号的对应表，在非重用时会写入，在重用时只读出
	///@param	reuse	是否重用
	///@param	reuseID	重用编号，非重用时应当置为-1
	///@param	readOnly	1表示该CFixMem是只读的，0表示可以读写
	CPositionFactory(CMDB* pMDB,int maxUnit,IMemoryAllocator *pAllocator,map<string,int>* pIndexMap,bool reuse=false, int reuseID=-1, int readOnly=0);
	
	///构造方法
	///@param	pAllocator	进行内存管理的分配器
	///@param	pIndexMap	索引名称和内存编号的对应表，在非重用时会写入，在重用时只读出
	///@param	reuse	是否重用
	///@param	reuseID	重用编号，非重用时应当置为-1
	///@param	readOnly	1表示该CFixMem是只读的，0表示可以读写
	CPositionFactory(CMDB* pMDB,IMemoryAllocator *pAllocator,map<string,int>* pIndexMap,bool reuse=false, int reuseID = -1,int readOnly=0);
	
	///析构方法，将负责释放所占用的空间
	virtual ~CPositionFactory();

	///从CSV文件中读取所有的Position
	///@param	filename	文件名
	///@return	1表示完成，0表示没有完成
	virtual int readCSV(char *filename);

	///从CSV文件中读取所有的Position，文件名将根据配置文件中的定义PositionCSVFile
	///@return	1表示完成，0表示没有完成
	virtual int readCSVByPath(char *pPath);
	
	///向CSV文件中写出所有的Position
	///@param	filename	文件名
	///@return	1表示完成，0表示没有完成
	virtual int writeCSV(char *filename);
	
	///向CSV文件中写出上一个版本的Position
	///@param	filename	文件名
	///@return	1表示完成，0表示没有完成
	virtual int writeCSVPreVersion(char *filename);	

	///向CSV文件中写出所有的Position，文件名将根据配置文件中的定义PositionCSVFile
	///@return	1表示完成，0表示没有完成
	virtual int writeCSVByPath(char *pPath);

	///将内容写到文件中,用于DEBUG
	virtual void dump(FILE *);

	///将内容写到文件中,用于DEBUG
	virtual void dump(char *filename);

	///清空所有的记录
	virtual void clearAll();

	friend class CPositionResource;

	///将一个CPosition加入到该对象工厂，实质上是完成修改索引的工作
	///@param	pPosition	要加入的Position
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	///@return	加入后该对象的位置，当pTransaction!=NULL时，总是返回NULL
	CPosition* add(CWriteablePosition *pPosition, CTransaction *pTransaction=NULL);
	
	///刷新该CPosition的值
	///@param	pPosition	被刷新的CPosition
	///@param	pNewPosition	新的值
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	CPosition* update(CPosition *pPosition, CWriteablePosition *pNewPosition, CTransaction *pTransaction=NULL, bool bNoTransaction = true);
	
	///增加或者更新数据
	///@param	pPosition	  需要被刷新或者新增的CPosition,等于NULL表示是需要新增的
	///@param	pNewPosition	新的值
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	///@param	新增或者修改后的数据
	CPosition* addOrUpdate(CPosition *pPosition, CWriteablePosition *pNewPosition, CTransaction *pTransaction=NULL);
	
	///删除一个CPosition，同时删除其索引
	///@param	pPosition	要删除的CPosition
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	void remove(CPosition *pPosition, CTransaction *pTransaction=NULL, bool bNoTransaction=true);
	
	///获取某个CPosition
	///@param	pPosition	要读取的CPosition
	///@param	pTragetPosition	存放结果的CPosition
	void retrieve(CPosition *pPosition, CWriteablePosition *pTargetPosition);
	
	///获取第一个CPosition
	///@return	得到的第一个CPosition，如果没有，则返回NULL
	CPosition* getFirst();
	
	///获取下一个CPosition
	///@return	得到下一个CPosition，如果没有，则返回NULL
	CPosition* getNext();
	
	///加入一个操作触发器
	///@param	pTrigger	要加入的操作触发器
	///@return	1表示成功，0表示失败
	int addActionTrigger(CPositionActionTrigger *pTrigger);
	
	///删除一个操作触发器
	///@param	pTrigger	要删除的操作触发器
	///@return	1表示成功，0表示失败
	int removeActionTrigger(CPositionActionTrigger *pTrigger);

	///加入一个确认触发器
	///@param	pTrigger	要加入的确认触发器
	///@return	1表示成功，0表示失败
	int addCommitTrigger(CPositionCommitTrigger *pTrigger);
	
	///删除一个确认触发器
	///@param	pTrigger	要删除的确认触发器
	///@return	1表示成功，0表示失败
	int removeCommitTrigger(CPositionCommitTrigger *pTrigger);
	
	///构造所有对应的标的的指针
	///@param	pFactory	构造对应的标的的Link的指针时，寻找的对象工厂
	void linkAllInstrument();

	///构造所有对应的自己账号的指针
	///@param	pFactory	构造对应的自己账号的Link的指针时，寻找的对象工厂
	void linkAllAccount();

	///检查对应的标的的指针
	///@param pFile 输出错误日志的文件
	void checkLinkInstrument(FILE *pFile);
	
	///检查对应的自己账号的指针
	///@param pFile 输出错误日志的文件
	void checkLinkAccount(FILE *pFile);
	
	///清空对应的标的的外键
	void clearLinkInstrument();
	
	///清空对应的自己账号的外键
	void clearLinkAccount();
	
	///检查是否字段为NULL
	///@param pFile 输出错误日志的文件
	void checkNull(FILE *pFile);
	
	///检查是否字段合法
	///@param pFile 输出错误日志的文件
	void checkValid(FILE *pFile);	

	///寻找CPosition
	///@return	找到的CPosition，如果找不到，返回NULL
	CPosition *findByPositionID(const CReadOnlyLongIndexIDType&  PositionID);

	///根据主码寻找
	///@return	找到的CPosition，如果找不到，返回NULL
	CPosition *findByPK(const CReadOnlyLongIndexIDType& PositionID);
		
	///根据主键删除CPosition
	bool removeByPK(const CReadOnlyLongIndexIDType& PositionID, CTransaction *pTransaction=NULL);
	
	///判断字段是否是是主键
	bool isPK(const char* pName);
	
	///根据主键寻找CPosition
	///@param	pPosition	要找的值
	///@return	找到的CPosition，如果找不到，返回NULL
	CPosition *findByPK(CPositionField *pPosition);
	
	///根据主键寻找CPosition
	///@param	pPosition	要找的值
	///@return	找到的CPosition，如果找不到，返回NULL
	CPosition *findByPK(CPosition *pPosition);

	///根据主键获取CPosition
	///@param	pPosition	要获取的值，同时用于存放返回的结果
	///@return	找到的CPosition，如果找不到，返回NULL
	bool retrieveByPK(CWriteablePosition *pPosition);

	///根据主键增加新的CPosition
	///@param	pPosition	新的值
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	///@return	返回插入的CPosition，如果插入失败，则返回NULL
	CPosition *addByPK(CWriteablePosition *pPosition, CTransaction *pTransaction=NULL);
	
	///根据主键增加或更新新的CPosition
	///@param	pPosition	新的值
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	///@return	返回插入的CPosition，如果插入失败，则返回NULL
	CPosition *addOrUpdateByPK(CWriteablePosition *pPosition, CTransaction *pTransaction=NULL);

	///根据主键更新CPosition的值
	///@param	pPosition	新的值
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	///@return	true表示成功，false表示失败
	bool updateByPK(CWriteablePosition *pPosition, CTransaction *pTransaction=NULL);

	///根据主键删除CPosition
	///@param	pPosition	要删除的值
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	///@return	true表示成功，false表示失败
	bool removeByPK(CPosition *pPosition, CTransaction *pTransaction=NULL);
	
	///根据主键删除CPosition
	///@param	pPosition	要删除的值
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	///@return	true表示成功，false表示失败
	bool removeByPK(CPositionField *pPosition, CTransaction *pTransaction=NULL);	

	friend class CPositionIteratorByAccountInstrument;

	///寻找下一个符合条件的CPosition，必须在startFindByAccountInstrument以后叫用
	///@return	下一个满足条件CPosition，如果已经没有一个满足要求了，则返回NULL
	CPosition *findNextByAccountInstrument();
	
	///寻找第一个CPosition
	///@return	只寻找满足条件的第一个CPosition，如果找不到，返回NULL
	CPosition *findFirstByAccountInstrument( const CReadOnlyAccountIDType& AccountID,  const CReadOnlyExchangeIDType& ExchangeID,  const CReadOnlyInstrumentIDType& InstrumentID);
	
	///寻找CPosition
	///@return	找到的CPosition，如果找不到，返回NULL
	CPosition *findByTradeUnitID(const CReadOnlyAccountIDType&  AccountID, const CReadOnlyExchangeIDType&  ExchangeID, const CReadOnlyInstrumentIDType&  InstrumentID, const CReadOnlyIndexIDType&  TradeUnitID, const CReadOnlyPosiDirectionType&  PosiDirection);


	friend class CPositionIteratorByAccountID;

	///寻找下一个符合条件的CPosition，必须在startFindByAccountID以后叫用
	///@return	下一个满足条件CPosition，如果已经没有一个满足要求了，则返回NULL
	CPosition *findNextByAccountID();
	
	///寻找第一个CPosition
	///@return	只寻找满足条件的第一个CPosition，如果找不到，返回NULL
	CPosition *findFirstByAccountID( const CReadOnlyAccountIDType& AccountID);
	
	friend class CPositionIteratorByLiquidPriceLong;

	///寻找下一个符合条件的CPosition，必须在startFindByLiquidPriceLong以后叫用
	///@return	下一个满足条件CPosition，如果已经没有一个满足要求了，则返回NULL
	CPosition *findNextByLiquidPriceLong();
	
	///寻找第一个CPosition
	///@return	只寻找满足条件的第一个CPosition，如果找不到，返回NULL
	CPosition *findFirstByLiquidPriceLong( const CReadOnlyExchangeIDType& ExchangeID,  const CReadOnlyInstrumentIDType& InstrumentID,  const CReadOnlyPriceType& LiquidPrice);
	
	friend class CPositionIteratorByLiquidPriceShort;

	///寻找下一个符合条件的CPosition，必须在startFindByLiquidPriceShort以后叫用
	///@return	下一个满足条件CPosition，如果已经没有一个满足要求了，则返回NULL
	CPosition *findNextByLiquidPriceShort();
	
	///寻找第一个CPosition
	///@return	只寻找满足条件的第一个CPosition，如果找不到，返回NULL
	CPosition *findFirstByLiquidPriceShort( const CReadOnlyExchangeIDType& ExchangeID,  const CReadOnlyInstrumentIDType& InstrumentID,  const CReadOnlyPriceType& LiquidPrice);
	

private:
	///完成自己的初始化工作
	///@param	maxUnit	最多的数据对象个数
	///@param	pAllocator	进行内存管理的分配器
	///@param	pIndexMap	索引名称和内存编号的对应表，在非重用时会写入，在重用时只读出
	///@param	reuse	是否重用
	///@param	reuseID	重用编号，非重用时应当置为-1
	///@param	readOnly	1表示该CFixMem是只读的，0表示可以读写
	void init(int maxUnit,IMemoryAllocator *pAllocator,map<string,int>* pIndexMap,bool reuse,int reuseID,int readOnly);
	
	///将一个CPosition加入到该对象工厂，实质上是完成修改索引的工作
	///@param	pPosition	要加入的Position
	///@param	bNoTransaction 是否不属于任何事务
	///@return	加入后该对象的位置，当pTransaction!=NULL时，总是返回NULL
	CPosition* internalAdd(CWriteablePosition *pPosition, bool bNoTransaction);
	
	
	///刷新该CPosition的键值
	///@param	pPosition	被刷新的CPosition
	///@param	pNewPosition	新的值
	///@param	bNoTransaction 是否不属于任何事务
	CPosition* internalUpdate(CPosition *pPosition, CWriteablePosition *pNewPosition, bool bNoTransaction);
	
	///删除一个CPosition，同时删除其索引
	///@param	pPosition	要删除的CPosition
	///@param	bNoTransaction 是否不属于任何事务
	void internalRemove(CPosition *pPosition, bool bNoTransaction);
/*
	///检查某个CPosition是否属于本对象工厂
	///@param	pPosition	要检查的对象
	///@exception	CDesignError	如果不属于，则抛出此异常
	void checkAddress(CPosition *pPosition);
*/

	///基于AccountID+ExchangeID+InstrumentID+TradeUnitID+PosiDirection的索引
	CAVLTree *pAccountIndex;
	///基于ExchangeID+InstrumentID+LiquidPrice的索引
	CAVLTree *pLiquidPriceLongIndex;
	///基于ExchangeID+InstrumentID+LiquidPrice的索引
	CAVLTree *pLiquidPriceShortIndex;
	
	///基于PositionID的hash索引
	CHashIndex *pPositionIDHashIndex;
	
	///按照AccountID，ExchangeID，InstrumentID检索时，存储的查询参数
	CAccountIDType queryAccountIDInSearchByAccountInstrument;
	CExchangeIDType queryExchangeIDInSearchByAccountInstrument;
	CInstrumentIDType queryInstrumentIDInSearchByAccountInstrument;
	
	///按照AccountID，ExchangeID，InstrumentID检索时，存储最后一次找到的索引节点
	CAVLNode *pLastFoundInSearchByAccountInstrument;
	///按照AccountID检索时，存储的查询参数
	CAccountIDType queryAccountIDInSearchByAccountID;
	
	///按照AccountID检索时，存储最后一次找到的索引节点
	CAVLNode *pLastFoundInSearchByAccountID;
	///按照ExchangeID，InstrumentID，LiquidPrice检索时，存储的查询参数
	CExchangeIDType queryExchangeIDInSearchByLiquidPriceLong;
	CInstrumentIDType queryInstrumentIDInSearchByLiquidPriceLong;
	CPriceType queryLiquidPriceInSearchByLiquidPriceLong;
	
	///按照ExchangeID，InstrumentID，LiquidPrice检索时，存储最后一次找到的索引节点
	CAVLNode *pLastFoundInSearchByLiquidPriceLong;
	///按照ExchangeID，InstrumentID，LiquidPrice检索时，存储的查询参数
	CExchangeIDType queryExchangeIDInSearchByLiquidPriceShort;
	CInstrumentIDType queryInstrumentIDInSearchByLiquidPriceShort;
	CPriceType queryLiquidPriceInSearchByLiquidPriceShort;
	
	///按照ExchangeID，InstrumentID，LiquidPrice检索时，存储最后一次找到的索引节点
	CAVLNode *pLastFoundInSearchByLiquidPriceShort;
		
	///确认加入后触发
	///@param	pPosition	已经加入的Position
	virtual void commitAdd(CPosition *pPosition);

	///确认更新后触发
	///@param	pPosition	被刷新的CPosition
	///@param	poldPosition	原来的值
	virtual void commitUpdate(CPosition *pPosition, CWriteablePosition *pOldPosition);
	
	///确认删除后触发
	///@param	pPosition	已经删除的CPosition
	virtual void commitRemove(CWriteablePosition *pPosition);

	///所有相关的触发器
	vector<CPositionActionTrigger *> *pActionTriggers;
	vector<CPositionCommitTrigger *> *pCommitTriggers;
	
	///进行查询时，用于比较的对象
	CWriteablePosition compareObject;
	CMDB* m_pMDB;
};

/////////////////////////////////////////////////////////////////////////
///CMarketDataFactory是一个行情的对象工厂。它包含了一批行情，
///同时又建立了如下索引，以方便查询：
///	ProductGroupIndex
///使用本对象工厂，可以完成对行情的增删改查操作。具体方法如下：
///	增加：叫用add，完成加入
///	删除：叫用remove就可以删除
///	查询：有如下方法可以查询：
///		使用findByInstrumentID，唯一找到对象
///		使用startFindByProductGroup，findNextByProductGroup完成查询操作
///	修改：对于查到的结果，可以通过retrieve取出，修改后，通过update放回
///	事务管理：所有的增加、删除和修改都可以指定事务，在事务commit时，真正写入
///	在retrieve时，也可以指定事务，此时将取出经过update的新数据
/////////////////////////////////////////////////////////////////////////
class CMarketDataFactory :public CDatabaseDataFactory
{
public:
	///构造方法
	///@param	maxUnit	初始的数据对象个数
	///@param	pAllocator	进行内存管理的分配器
	///@param	pIndexMap	索引名称和内存编号的对应表，在非重用时会写入，在重用时只读出
	///@param	reuse	是否重用
	///@param	reuseID	重用编号，非重用时应当置为-1
	///@param	readOnly	1表示该CFixMem是只读的，0表示可以读写
	CMarketDataFactory(CMDB* pMDB,int maxUnit,IMemoryAllocator *pAllocator,map<string,int>* pIndexMap,bool reuse=false, int reuseID=-1, int readOnly=0);
	
	///构造方法
	///@param	pAllocator	进行内存管理的分配器
	///@param	pIndexMap	索引名称和内存编号的对应表，在非重用时会写入，在重用时只读出
	///@param	reuse	是否重用
	///@param	reuseID	重用编号，非重用时应当置为-1
	///@param	readOnly	1表示该CFixMem是只读的，0表示可以读写
	CMarketDataFactory(CMDB* pMDB,IMemoryAllocator *pAllocator,map<string,int>* pIndexMap,bool reuse=false, int reuseID = -1,int readOnly=0);
	
	///析构方法，将负责释放所占用的空间
	virtual ~CMarketDataFactory();

	///从CSV文件中读取所有的MarketData
	///@param	filename	文件名
	///@return	1表示完成，0表示没有完成
	virtual int readCSV(char *filename);

	///从CSV文件中读取所有的MarketData，文件名将根据配置文件中的定义MarketDataCSVFile
	///@return	1表示完成，0表示没有完成
	virtual int readCSVByPath(char *pPath);
	
	///向CSV文件中写出所有的MarketData
	///@param	filename	文件名
	///@return	1表示完成，0表示没有完成
	virtual int writeCSV(char *filename);
	
	///向CSV文件中写出上一个版本的MarketData
	///@param	filename	文件名
	///@return	1表示完成，0表示没有完成
	virtual int writeCSVPreVersion(char *filename);	

	///向CSV文件中写出所有的MarketData，文件名将根据配置文件中的定义MarketDataCSVFile
	///@return	1表示完成，0表示没有完成
	virtual int writeCSVByPath(char *pPath);

	///将内容写到文件中,用于DEBUG
	virtual void dump(FILE *);

	///将内容写到文件中,用于DEBUG
	virtual void dump(char *filename);

	///清空所有的记录
	virtual void clearAll();

	friend class CMarketDataResource;

	///将一个CMarketData加入到该对象工厂，实质上是完成修改索引的工作
	///@param	pMarketData	要加入的MarketData
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	///@return	加入后该对象的位置，当pTransaction!=NULL时，总是返回NULL
	CMarketData* add(CWriteableMarketData *pMarketData, CTransaction *pTransaction=NULL);
	
	///刷新该CMarketData的值
	///@param	pMarketData	被刷新的CMarketData
	///@param	pNewMarketData	新的值
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	CMarketData* update(CMarketData *pMarketData, CWriteableMarketData *pNewMarketData, CTransaction *pTransaction=NULL, bool bNoTransaction = true);
	
	///增加或者更新数据
	///@param	pMarketData	  需要被刷新或者新增的CMarketData,等于NULL表示是需要新增的
	///@param	pNewMarketData	新的值
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	///@param	新增或者修改后的数据
	CMarketData* addOrUpdate(CMarketData *pMarketData, CWriteableMarketData *pNewMarketData, CTransaction *pTransaction=NULL);
	
	///删除一个CMarketData，同时删除其索引
	///@param	pMarketData	要删除的CMarketData
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	void remove(CMarketData *pMarketData, CTransaction *pTransaction=NULL, bool bNoTransaction=true);
	
	///获取某个CMarketData
	///@param	pMarketData	要读取的CMarketData
	///@param	pTragetMarketData	存放结果的CMarketData
	void retrieve(CMarketData *pMarketData, CWriteableMarketData *pTargetMarketData);
	
	///获取第一个CMarketData
	///@return	得到的第一个CMarketData，如果没有，则返回NULL
	CMarketData* getFirst();
	
	///获取下一个CMarketData
	///@return	得到下一个CMarketData，如果没有，则返回NULL
	CMarketData* getNext();
	
	///加入一个操作触发器
	///@param	pTrigger	要加入的操作触发器
	///@return	1表示成功，0表示失败
	int addActionTrigger(CMarketDataActionTrigger *pTrigger);
	
	///删除一个操作触发器
	///@param	pTrigger	要删除的操作触发器
	///@return	1表示成功，0表示失败
	int removeActionTrigger(CMarketDataActionTrigger *pTrigger);

	///加入一个确认触发器
	///@param	pTrigger	要加入的确认触发器
	///@return	1表示成功，0表示失败
	int addCommitTrigger(CMarketDataCommitTrigger *pTrigger);
	
	///删除一个确认触发器
	///@param	pTrigger	要删除的确认触发器
	///@return	1表示成功，0表示失败
	int removeCommitTrigger(CMarketDataCommitTrigger *pTrigger);
	
	///构造所有对应的标的的指针
	///@param	pFactory	构造对应的标的的Link的指针时，寻找的对象工厂
	void linkAllInstrument();

	///检查对应的标的的指针
	///@param pFile 输出错误日志的文件
	void checkLinkInstrument(FILE *pFile);
	
	///清空对应的标的的外键
	void clearLinkInstrument();
	
	///检查是否字段为NULL
	///@param pFile 输出错误日志的文件
	void checkNull(FILE *pFile);
	
	///检查是否字段合法
	///@param pFile 输出错误日志的文件
	void checkValid(FILE *pFile);	

	///寻找CMarketData
	///@return	找到的CMarketData，如果找不到，返回NULL
	CMarketData *findByInstrumentID(const CReadOnlyExchangeIDType&  ExchangeID, const CReadOnlyInstrumentIDType&  InstrumentID);

	///根据主码寻找
	///@return	找到的CMarketData，如果找不到，返回NULL
	CMarketData *findByPK(const CReadOnlyExchangeIDType& ExchangeID, const CReadOnlyInstrumentIDType& InstrumentID);
		
	///根据主键删除CMarketData
	bool removeByPK(const CReadOnlyExchangeIDType& ExchangeID, const CReadOnlyInstrumentIDType& InstrumentID, CTransaction *pTransaction=NULL);
	
	///判断字段是否是是主键
	bool isPK(const char* pName);
	
	///根据主键寻找CMarketData
	///@param	pMarketData	要找的值
	///@return	找到的CMarketData，如果找不到，返回NULL
	CMarketData *findByPK(CMarketDataField *pMarketData);
	
	///根据主键寻找CMarketData
	///@param	pMarketData	要找的值
	///@return	找到的CMarketData，如果找不到，返回NULL
	CMarketData *findByPK(CMarketData *pMarketData);

	///根据主键获取CMarketData
	///@param	pMarketData	要获取的值，同时用于存放返回的结果
	///@return	找到的CMarketData，如果找不到，返回NULL
	bool retrieveByPK(CWriteableMarketData *pMarketData);

	///根据主键增加新的CMarketData
	///@param	pMarketData	新的值
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	///@return	返回插入的CMarketData，如果插入失败，则返回NULL
	CMarketData *addByPK(CWriteableMarketData *pMarketData, CTransaction *pTransaction=NULL);
	
	///根据主键增加或更新新的CMarketData
	///@param	pMarketData	新的值
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	///@return	返回插入的CMarketData，如果插入失败，则返回NULL
	CMarketData *addOrUpdateByPK(CWriteableMarketData *pMarketData, CTransaction *pTransaction=NULL);

	///根据主键更新CMarketData的值
	///@param	pMarketData	新的值
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	///@return	true表示成功，false表示失败
	bool updateByPK(CWriteableMarketData *pMarketData, CTransaction *pTransaction=NULL);

	///根据主键删除CMarketData
	///@param	pMarketData	要删除的值
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	///@return	true表示成功，false表示失败
	bool removeByPK(CMarketData *pMarketData, CTransaction *pTransaction=NULL);
	
	///根据主键删除CMarketData
	///@param	pMarketData	要删除的值
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	///@return	true表示成功，false表示失败
	bool removeByPK(CMarketDataField *pMarketData, CTransaction *pTransaction=NULL);	

	friend class CMarketDataIteratorByProductGroup;

	///寻找下一个符合条件的CMarketData，必须在startFindByProductGroup以后叫用
	///@return	下一个满足条件CMarketData，如果已经没有一个满足要求了，则返回NULL
	CMarketData *findNextByProductGroup();
	
	///寻找第一个CMarketData
	///@return	只寻找满足条件的第一个CMarketData，如果找不到，返回NULL
	CMarketData *findFirstByProductGroup( const CReadOnlyShortIndexIDType& ProductGroup);
	

private:
	///完成自己的初始化工作
	///@param	maxUnit	最多的数据对象个数
	///@param	pAllocator	进行内存管理的分配器
	///@param	pIndexMap	索引名称和内存编号的对应表，在非重用时会写入，在重用时只读出
	///@param	reuse	是否重用
	///@param	reuseID	重用编号，非重用时应当置为-1
	///@param	readOnly	1表示该CFixMem是只读的，0表示可以读写
	void init(int maxUnit,IMemoryAllocator *pAllocator,map<string,int>* pIndexMap,bool reuse,int reuseID,int readOnly);
	
	///将一个CMarketData加入到该对象工厂，实质上是完成修改索引的工作
	///@param	pMarketData	要加入的MarketData
	///@param	bNoTransaction 是否不属于任何事务
	///@return	加入后该对象的位置，当pTransaction!=NULL时，总是返回NULL
	CMarketData* internalAdd(CWriteableMarketData *pMarketData, bool bNoTransaction);
	
	
	///刷新该CMarketData的键值
	///@param	pMarketData	被刷新的CMarketData
	///@param	pNewMarketData	新的值
	///@param	bNoTransaction 是否不属于任何事务
	CMarketData* internalUpdate(CMarketData *pMarketData, CWriteableMarketData *pNewMarketData, bool bNoTransaction);
	
	///删除一个CMarketData，同时删除其索引
	///@param	pMarketData	要删除的CMarketData
	///@param	bNoTransaction 是否不属于任何事务
	void internalRemove(CMarketData *pMarketData, bool bNoTransaction);
/*
	///检查某个CMarketData是否属于本对象工厂
	///@param	pMarketData	要检查的对象
	///@exception	CDesignError	如果不属于，则抛出此异常
	void checkAddress(CMarketData *pMarketData);
*/

	///基于ProductGroup的索引
	CAVLTree *pProductGroupIndex;
	
	///基于InstrumentIDIndex的hash索引
	CHashIndex *pInstrumentIDHashIndex;
	
	///按照ProductGroup检索时，存储的查询参数
	CShortIndexIDType queryProductGroupInSearchByProductGroup;
	
	///按照ProductGroup检索时，存储最后一次找到的索引节点
	CAVLNode *pLastFoundInSearchByProductGroup;
		
	///确认加入后触发
	///@param	pMarketData	已经加入的MarketData
	virtual void commitAdd(CMarketData *pMarketData);

	///确认更新后触发
	///@param	pMarketData	被刷新的CMarketData
	///@param	poldMarketData	原来的值
	virtual void commitUpdate(CMarketData *pMarketData, CWriteableMarketData *pOldMarketData);
	
	///确认删除后触发
	///@param	pMarketData	已经删除的CMarketData
	virtual void commitRemove(CWriteableMarketData *pMarketData);

	///所有相关的触发器
	vector<CMarketDataActionTrigger *> *pActionTriggers;
	vector<CMarketDataCommitTrigger *> *pCommitTriggers;
	
	///进行查询时，用于比较的对象
	CWriteableMarketData compareObject;
	CMDB* m_pMDB;
};

/////////////////////////////////////////////////////////////////////////
///COrderFactory是一个报单的对象工厂。它包含了一批报单，
///同时又建立了如下索引，以方便查询：
///	PositionIDIndex
///	LocalIDIndex
///	InstrumentIDIndex
///使用本对象工厂，可以完成对报单的增删改查操作。具体方法如下：
///	增加：叫用add，完成加入
///	删除：叫用remove就可以删除
///	查询：有如下方法可以查询：
///		使用findByOrderID，唯一找到对象
///		使用findByLocalID，唯一找到对象
///		使用startFindByAccountID，findNextByAccountID完成查询操作
///		使用startFindByPositionID，findNextByPositionID完成查询操作
///		使用startFindByAccountIDPrivate，findNextByAccountIDPrivate完成查询操作
///		使用startFindByInstrumentID，findNextByInstrumentID完成查询操作
///	修改：对于查到的结果，可以通过retrieve取出，修改后，通过update放回
///	事务管理：所有的增加、删除和修改都可以指定事务，在事务commit时，真正写入
///	在retrieve时，也可以指定事务，此时将取出经过update的新数据
/////////////////////////////////////////////////////////////////////////
class COrderFactory :public CDatabaseDataFactory
{
public:
	///构造方法
	///@param	maxUnit	初始的数据对象个数
	///@param	pAllocator	进行内存管理的分配器
	///@param	pIndexMap	索引名称和内存编号的对应表，在非重用时会写入，在重用时只读出
	///@param	reuse	是否重用
	///@param	reuseID	重用编号，非重用时应当置为-1
	///@param	readOnly	1表示该CFixMem是只读的，0表示可以读写
	COrderFactory(CMDB* pMDB,int maxUnit,IMemoryAllocator *pAllocator,map<string,int>* pIndexMap,bool reuse=false, int reuseID=-1, int readOnly=0);
	
	///构造方法
	///@param	pAllocator	进行内存管理的分配器
	///@param	pIndexMap	索引名称和内存编号的对应表，在非重用时会写入，在重用时只读出
	///@param	reuse	是否重用
	///@param	reuseID	重用编号，非重用时应当置为-1
	///@param	readOnly	1表示该CFixMem是只读的，0表示可以读写
	COrderFactory(CMDB* pMDB,IMemoryAllocator *pAllocator,map<string,int>* pIndexMap,bool reuse=false, int reuseID = -1,int readOnly=0);
	
	///析构方法，将负责释放所占用的空间
	virtual ~COrderFactory();

	///从CSV文件中读取所有的Order
	///@param	filename	文件名
	///@return	1表示完成，0表示没有完成
	virtual int readCSV(char *filename);

	///从CSV文件中读取所有的Order，文件名将根据配置文件中的定义OrderCSVFile
	///@return	1表示完成，0表示没有完成
	virtual int readCSVByPath(char *pPath);
	
	///向CSV文件中写出所有的Order
	///@param	filename	文件名
	///@return	1表示完成，0表示没有完成
	virtual int writeCSV(char *filename);
	
	///向CSV文件中写出上一个版本的Order
	///@param	filename	文件名
	///@return	1表示完成，0表示没有完成
	virtual int writeCSVPreVersion(char *filename);	

	///向CSV文件中写出所有的Order，文件名将根据配置文件中的定义OrderCSVFile
	///@return	1表示完成，0表示没有完成
	virtual int writeCSVByPath(char *pPath);

	///将内容写到文件中,用于DEBUG
	virtual void dump(FILE *);

	///将内容写到文件中,用于DEBUG
	virtual void dump(char *filename);

	///清空所有的记录
	virtual void clearAll();

	friend class COrderResource;

	///将一个COrder加入到该对象工厂，实质上是完成修改索引的工作
	///@param	pOrder	要加入的Order
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	///@return	加入后该对象的位置，当pTransaction!=NULL时，总是返回NULL
	COrder* add(CWriteableOrder *pOrder, CTransaction *pTransaction=NULL);
	
	///刷新该COrder的值
	///@param	pOrder	被刷新的COrder
	///@param	pNewOrder	新的值
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	COrder* update(COrder *pOrder, CWriteableOrder *pNewOrder, CTransaction *pTransaction=NULL, bool bNoTransaction = true);
	
	///增加或者更新数据
	///@param	pOrder	  需要被刷新或者新增的COrder,等于NULL表示是需要新增的
	///@param	pNewOrder	新的值
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	///@param	新增或者修改后的数据
	COrder* addOrUpdate(COrder *pOrder, CWriteableOrder *pNewOrder, CTransaction *pTransaction=NULL);
	
	///删除一个COrder，同时删除其索引
	///@param	pOrder	要删除的COrder
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	void remove(COrder *pOrder, CTransaction *pTransaction=NULL, bool bNoTransaction=true);
	
	///获取某个COrder
	///@param	pOrder	要读取的COrder
	///@param	pTragetOrder	存放结果的COrder
	void retrieve(COrder *pOrder, CWriteableOrder *pTargetOrder);
	
	///获取第一个COrder
	///@return	得到的第一个COrder，如果没有，则返回NULL
	COrder* getFirst();
	
	///获取下一个COrder
	///@return	得到下一个COrder，如果没有，则返回NULL
	COrder* getNext();
	
	///加入一个操作触发器
	///@param	pTrigger	要加入的操作触发器
	///@return	1表示成功，0表示失败
	int addActionTrigger(COrderActionTrigger *pTrigger);
	
	///删除一个操作触发器
	///@param	pTrigger	要删除的操作触发器
	///@return	1表示成功，0表示失败
	int removeActionTrigger(COrderActionTrigger *pTrigger);

	///加入一个确认触发器
	///@param	pTrigger	要加入的确认触发器
	///@return	1表示成功，0表示失败
	int addCommitTrigger(COrderCommitTrigger *pTrigger);
	
	///删除一个确认触发器
	///@param	pTrigger	要删除的确认触发器
	///@return	1表示成功，0表示失败
	int removeCommitTrigger(COrderCommitTrigger *pTrigger);
	
	///构造所有对应的标的的指针
	///@param	pFactory	构造对应的标的的Link的指针时，寻找的对象工厂
	void linkAllInstrument();

	///构造所有对应的交易单元持仓的指针
	///@param	pFactory	构造对应的交易单元持仓的Link的指针时，寻找的对象工厂
	void linkAllPosition();

	///构造所有对应的用户的指针
	///@param	pFactory	构造对应的用户的Link的指针时，寻找的对象工厂
	void linkAllMember();

	///构造所有关联的触发报单的指针
	///@param	pFactory	构造关联的触发报单的Link的指针时，寻找的对象工厂
	void linkAllTriggerOrder();

	///检查对应的标的的指针
	///@param pFile 输出错误日志的文件
	void checkLinkInstrument(FILE *pFile);
	
	///检查对应的交易单元持仓的指针
	///@param pFile 输出错误日志的文件
	void checkLinkPosition(FILE *pFile);
	
	///检查对应的用户的指针
	///@param pFile 输出错误日志的文件
	void checkLinkMember(FILE *pFile);
	
	///检查关联的触发报单的指针
	///@param pFile 输出错误日志的文件
	void checkLinkTriggerOrder(FILE *pFile);
	
	///清空对应的标的的外键
	void clearLinkInstrument();
	
	///清空对应的交易单元持仓的外键
	void clearLinkPosition();
	
	///清空对应的用户的外键
	void clearLinkMember();
	
	///清空关联的触发报单的外键
	void clearLinkTriggerOrder();
	
	///检查是否字段为NULL
	///@param pFile 输出错误日志的文件
	void checkNull(FILE *pFile);
	
	///检查是否字段合法
	///@param pFile 输出错误日志的文件
	void checkValid(FILE *pFile);	

	///寻找COrder
	///@return	找到的COrder，如果找不到，返回NULL
	COrder *findByOrderID(const CReadOnlyIndexIDType&  OrderID);

	///根据主码寻找
	///@return	找到的COrder，如果找不到，返回NULL
	COrder *findByPK(const CReadOnlyIndexIDType& OrderID);
		
	///根据主键删除COrder
	bool removeByPK(const CReadOnlyIndexIDType& OrderID, CTransaction *pTransaction=NULL);
	
	///判断字段是否是是主键
	bool isPK(const char* pName);
	
	///根据主键寻找COrder
	///@param	pOrder	要找的值
	///@return	找到的COrder，如果找不到，返回NULL
	COrder *findByPK(COrderField *pOrder);
	
	///根据主键寻找COrder
	///@param	pOrder	要找的值
	///@return	找到的COrder，如果找不到，返回NULL
	COrder *findByPK(COrder *pOrder);

	///根据主键获取COrder
	///@param	pOrder	要获取的值，同时用于存放返回的结果
	///@return	找到的COrder，如果找不到，返回NULL
	bool retrieveByPK(CWriteableOrder *pOrder);

	///根据主键增加新的COrder
	///@param	pOrder	新的值
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	///@return	返回插入的COrder，如果插入失败，则返回NULL
	COrder *addByPK(CWriteableOrder *pOrder, CTransaction *pTransaction=NULL);
	
	///根据主键增加或更新新的COrder
	///@param	pOrder	新的值
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	///@return	返回插入的COrder，如果插入失败，则返回NULL
	COrder *addOrUpdateByPK(CWriteableOrder *pOrder, CTransaction *pTransaction=NULL);

	///根据主键更新COrder的值
	///@param	pOrder	新的值
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	///@return	true表示成功，false表示失败
	bool updateByPK(CWriteableOrder *pOrder, CTransaction *pTransaction=NULL);

	///根据主键删除COrder
	///@param	pOrder	要删除的值
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	///@return	true表示成功，false表示失败
	bool removeByPK(COrder *pOrder, CTransaction *pTransaction=NULL);
	
	///根据主键删除COrder
	///@param	pOrder	要删除的值
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	///@return	true表示成功，false表示失败
	bool removeByPK(COrderField *pOrder, CTransaction *pTransaction=NULL);	

	///寻找COrder
	///@return	找到的COrder，如果找不到，返回NULL
	COrder *findByLocalID(const CReadOnlyAccountIDType&  AccountID, const CReadOnlyIndexIDType&  LocalID);


	friend class COrderIteratorByAccountID;

	///寻找下一个符合条件的COrder，必须在startFindByAccountID以后叫用
	///@return	下一个满足条件COrder，如果已经没有一个满足要求了，则返回NULL
	COrder *findNextByAccountID();
	
	///寻找第一个COrder
	///@return	只寻找满足条件的第一个COrder，如果找不到，返回NULL
	COrder *findFirstByAccountID( const CReadOnlyAccountIDType& AccountID);
	
	friend class COrderIteratorByPositionID;

	///寻找下一个符合条件的COrder，必须在startFindByPositionID以后叫用
	///@return	下一个满足条件COrder，如果已经没有一个满足要求了，则返回NULL
	COrder *findNextByPositionID();
	
	///寻找第一个COrder
	///@return	只寻找满足条件的第一个COrder，如果找不到，返回NULL
	COrder *findFirstByPositionID( const CReadOnlyLongIndexIDType& PositionID);
	
	friend class COrderIteratorByAccountIDPrivate;

	///寻找下一个符合条件的COrder，必须在startFindByAccountIDPrivate以后叫用
	///@return	下一个满足条件COrder，如果已经没有一个满足要求了，则返回NULL
	COrder *findNextByAccountIDPrivate();
	
	///寻找第一个COrder
	///@return	只寻找满足条件的第一个COrder，如果找不到，返回NULL
	COrder *findFirstByAccountIDPrivate( const CReadOnlyAccountIDType& AccountID);
	
	friend class COrderIteratorByInstrumentID;

	///寻找下一个符合条件的COrder，必须在startFindByInstrumentID以后叫用
	///@return	下一个满足条件COrder，如果已经没有一个满足要求了，则返回NULL
	COrder *findNextByInstrumentID();
	
	///寻找第一个COrder
	///@return	只寻找满足条件的第一个COrder，如果找不到，返回NULL
	COrder *findFirstByInstrumentID( const CReadOnlyAccountIDType& AccountID,  const CReadOnlyInstrumentIDType& InstrumentID);
	

private:
	///完成自己的初始化工作
	///@param	maxUnit	最多的数据对象个数
	///@param	pAllocator	进行内存管理的分配器
	///@param	pIndexMap	索引名称和内存编号的对应表，在非重用时会写入，在重用时只读出
	///@param	reuse	是否重用
	///@param	reuseID	重用编号，非重用时应当置为-1
	///@param	readOnly	1表示该CFixMem是只读的，0表示可以读写
	void init(int maxUnit,IMemoryAllocator *pAllocator,map<string,int>* pIndexMap,bool reuse,int reuseID,int readOnly);
	
	///将一个COrder加入到该对象工厂，实质上是完成修改索引的工作
	///@param	pOrder	要加入的Order
	///@param	bNoTransaction 是否不属于任何事务
	///@return	加入后该对象的位置，当pTransaction!=NULL时，总是返回NULL
	COrder* internalAdd(CWriteableOrder *pOrder, bool bNoTransaction);
	
	
	///刷新该COrder的键值
	///@param	pOrder	被刷新的COrder
	///@param	pNewOrder	新的值
	///@param	bNoTransaction 是否不属于任何事务
	COrder* internalUpdate(COrder *pOrder, CWriteableOrder *pNewOrder, bool bNoTransaction);
	
	///删除一个COrder，同时删除其索引
	///@param	pOrder	要删除的COrder
	///@param	bNoTransaction 是否不属于任何事务
	void internalRemove(COrder *pOrder, bool bNoTransaction);
/*
	///检查某个COrder是否属于本对象工厂
	///@param	pOrder	要检查的对象
	///@exception	CDesignError	如果不属于，则抛出此异常
	void checkAddress(COrder *pOrder);
*/

	///基于PositionID的索引
	CAVLTree *pPositionIDIndex;
	///基于AccountID+LocalID的索引
	CAVLTree *pLocalIDIndex;
	///基于AccountID+InstrumentID+OrderID的索引
	CAVLTree *pInstrumentIDIndex;
	
	///基于OrderID的hash索引
	CHashIndex *pOrderIDHashIndex;
	
	///按照AccountID检索时，存储的查询参数
	CAccountIDType queryAccountIDInSearchByAccountID;
	
	///按照AccountID检索时，存储最后一次找到的索引节点
	CAVLNode *pLastFoundInSearchByAccountID;
	///按照PositionID检索时，存储的查询参数
	CLongIndexIDType queryPositionIDInSearchByPositionID;
	
	///按照PositionID检索时，存储最后一次找到的索引节点
	CAVLNode *pLastFoundInSearchByPositionID;
	///按照AccountID检索时，存储的查询参数
	CAccountIDType queryAccountIDInSearchByAccountIDPrivate;
	
	///按照AccountID检索时，存储最后一次找到的索引节点
	CAVLNode *pLastFoundInSearchByAccountIDPrivate;
	///按照AccountID，InstrumentID检索时，存储的查询参数
	CAccountIDType queryAccountIDInSearchByInstrumentID;
	CInstrumentIDType queryInstrumentIDInSearchByInstrumentID;
	
	///按照AccountID，InstrumentID检索时，存储最后一次找到的索引节点
	CAVLNode *pLastFoundInSearchByInstrumentID;
		
	///确认加入后触发
	///@param	pOrder	已经加入的Order
	virtual void commitAdd(COrder *pOrder);

	///确认更新后触发
	///@param	pOrder	被刷新的COrder
	///@param	poldOrder	原来的值
	virtual void commitUpdate(COrder *pOrder, CWriteableOrder *pOldOrder);
	
	///确认删除后触发
	///@param	pOrder	已经删除的COrder
	virtual void commitRemove(CWriteableOrder *pOrder);

	///所有相关的触发器
	vector<COrderActionTrigger *> *pActionTriggers;
	vector<COrderCommitTrigger *> *pCommitTriggers;
	
	///进行查询时，用于比较的对象
	CWriteableOrder compareObject;
	CMDB* m_pMDB;
};

/////////////////////////////////////////////////////////////////////////
///CPositionOrderFactory是一个未平仓报单的对象工厂。它包含了一批未平仓报单，
///同时又建立了如下索引，以方便查询：
///	InsertTimeIndex
///使用本对象工厂，可以完成对未平仓报单的增删改查操作。具体方法如下：
///	增加：叫用add，完成加入
///	删除：叫用remove就可以删除
///	查询：有如下方法可以查询：
///		使用findByOrderID，唯一找到对象
///		使用startFindByInsertTime，findNextByInsertTime完成查询操作
///	修改：对于查到的结果，可以通过retrieve取出，修改后，通过update放回
///	事务管理：所有的增加、删除和修改都可以指定事务，在事务commit时，真正写入
///	在retrieve时，也可以指定事务，此时将取出经过update的新数据
/////////////////////////////////////////////////////////////////////////
class CPositionOrderFactory :public CDatabaseDataFactory
{
public:
	///构造方法
	///@param	maxUnit	初始的数据对象个数
	///@param	pAllocator	进行内存管理的分配器
	///@param	pIndexMap	索引名称和内存编号的对应表，在非重用时会写入，在重用时只读出
	///@param	reuse	是否重用
	///@param	reuseID	重用编号，非重用时应当置为-1
	///@param	readOnly	1表示该CFixMem是只读的，0表示可以读写
	CPositionOrderFactory(CMDB* pMDB,int maxUnit,IMemoryAllocator *pAllocator,map<string,int>* pIndexMap,bool reuse=false, int reuseID=-1, int readOnly=0);
	
	///构造方法
	///@param	pAllocator	进行内存管理的分配器
	///@param	pIndexMap	索引名称和内存编号的对应表，在非重用时会写入，在重用时只读出
	///@param	reuse	是否重用
	///@param	reuseID	重用编号，非重用时应当置为-1
	///@param	readOnly	1表示该CFixMem是只读的，0表示可以读写
	CPositionOrderFactory(CMDB* pMDB,IMemoryAllocator *pAllocator,map<string,int>* pIndexMap,bool reuse=false, int reuseID = -1,int readOnly=0);
	
	///析构方法，将负责释放所占用的空间
	virtual ~CPositionOrderFactory();

	///从CSV文件中读取所有的PositionOrder
	///@param	filename	文件名
	///@return	1表示完成，0表示没有完成
	virtual int readCSV(char *filename);

	///从CSV文件中读取所有的PositionOrder，文件名将根据配置文件中的定义PositionOrderCSVFile
	///@return	1表示完成，0表示没有完成
	virtual int readCSVByPath(char *pPath);
	
	///向CSV文件中写出所有的PositionOrder
	///@param	filename	文件名
	///@return	1表示完成，0表示没有完成
	virtual int writeCSV(char *filename);
	
	///向CSV文件中写出上一个版本的PositionOrder
	///@param	filename	文件名
	///@return	1表示完成，0表示没有完成
	virtual int writeCSVPreVersion(char *filename);	

	///向CSV文件中写出所有的PositionOrder，文件名将根据配置文件中的定义PositionOrderCSVFile
	///@return	1表示完成，0表示没有完成
	virtual int writeCSVByPath(char *pPath);

	///将内容写到文件中,用于DEBUG
	virtual void dump(FILE *);

	///将内容写到文件中,用于DEBUG
	virtual void dump(char *filename);

	///清空所有的记录
	virtual void clearAll();

	friend class CPositionOrderResource;

	///将一个CPositionOrder加入到该对象工厂，实质上是完成修改索引的工作
	///@param	pPositionOrder	要加入的PositionOrder
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	///@return	加入后该对象的位置，当pTransaction!=NULL时，总是返回NULL
	CPositionOrder* add(CWriteablePositionOrder *pPositionOrder, CTransaction *pTransaction=NULL);
	
	///刷新该CPositionOrder的值
	///@param	pPositionOrder	被刷新的CPositionOrder
	///@param	pNewPositionOrder	新的值
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	CPositionOrder* update(CPositionOrder *pPositionOrder, CWriteablePositionOrder *pNewPositionOrder, CTransaction *pTransaction=NULL, bool bNoTransaction = true);
	
	///增加或者更新数据
	///@param	pPositionOrder	  需要被刷新或者新增的CPositionOrder,等于NULL表示是需要新增的
	///@param	pNewPositionOrder	新的值
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	///@param	新增或者修改后的数据
	CPositionOrder* addOrUpdate(CPositionOrder *pPositionOrder, CWriteablePositionOrder *pNewPositionOrder, CTransaction *pTransaction=NULL);
	
	///删除一个CPositionOrder，同时删除其索引
	///@param	pPositionOrder	要删除的CPositionOrder
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	void remove(CPositionOrder *pPositionOrder, CTransaction *pTransaction=NULL, bool bNoTransaction=true);
	
	///获取某个CPositionOrder
	///@param	pPositionOrder	要读取的CPositionOrder
	///@param	pTragetPositionOrder	存放结果的CPositionOrder
	void retrieve(CPositionOrder *pPositionOrder, CWriteablePositionOrder *pTargetPositionOrder);
	
	///获取第一个CPositionOrder
	///@return	得到的第一个CPositionOrder，如果没有，则返回NULL
	CPositionOrder* getFirst();
	
	///获取下一个CPositionOrder
	///@return	得到下一个CPositionOrder，如果没有，则返回NULL
	CPositionOrder* getNext();
	
	///加入一个操作触发器
	///@param	pTrigger	要加入的操作触发器
	///@return	1表示成功，0表示失败
	int addActionTrigger(CPositionOrderActionTrigger *pTrigger);
	
	///删除一个操作触发器
	///@param	pTrigger	要删除的操作触发器
	///@return	1表示成功，0表示失败
	int removeActionTrigger(CPositionOrderActionTrigger *pTrigger);

	///加入一个确认触发器
	///@param	pTrigger	要加入的确认触发器
	///@return	1表示成功，0表示失败
	int addCommitTrigger(CPositionOrderCommitTrigger *pTrigger);
	
	///删除一个确认触发器
	///@param	pTrigger	要删除的确认触发器
	///@return	1表示成功，0表示失败
	int removeCommitTrigger(CPositionOrderCommitTrigger *pTrigger);
	
	///检查是否字段为NULL
	///@param pFile 输出错误日志的文件
	void checkNull(FILE *pFile);
	
	///检查是否字段合法
	///@param pFile 输出错误日志的文件
	void checkValid(FILE *pFile);	

	///寻找CPositionOrder
	///@return	找到的CPositionOrder，如果找不到，返回NULL
	CPositionOrder *findByOrderID(const CReadOnlyIndexIDType&  OrderID);

	///根据主码寻找
	///@return	找到的CPositionOrder，如果找不到，返回NULL
	CPositionOrder *findByPK(const CReadOnlyIndexIDType& OrderID);
		
	///根据主键删除CPositionOrder
	bool removeByPK(const CReadOnlyIndexIDType& OrderID, CTransaction *pTransaction=NULL);
	
	///判断字段是否是是主键
	bool isPK(const char* pName);
	
	///根据主键寻找CPositionOrder
	///@param	pPositionOrder	要找的值
	///@return	找到的CPositionOrder，如果找不到，返回NULL
	CPositionOrder *findByPK(CPositionOrderField *pPositionOrder);
	
	///根据主键寻找CPositionOrder
	///@param	pPositionOrder	要找的值
	///@return	找到的CPositionOrder，如果找不到，返回NULL
	CPositionOrder *findByPK(CPositionOrder *pPositionOrder);

	///根据主键获取CPositionOrder
	///@param	pPositionOrder	要获取的值，同时用于存放返回的结果
	///@return	找到的CPositionOrder，如果找不到，返回NULL
	bool retrieveByPK(CWriteablePositionOrder *pPositionOrder);

	///根据主键增加新的CPositionOrder
	///@param	pPositionOrder	新的值
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	///@return	返回插入的CPositionOrder，如果插入失败，则返回NULL
	CPositionOrder *addByPK(CWriteablePositionOrder *pPositionOrder, CTransaction *pTransaction=NULL);
	
	///根据主键增加或更新新的CPositionOrder
	///@param	pPositionOrder	新的值
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	///@return	返回插入的CPositionOrder，如果插入失败，则返回NULL
	CPositionOrder *addOrUpdateByPK(CWriteablePositionOrder *pPositionOrder, CTransaction *pTransaction=NULL);

	///根据主键更新CPositionOrder的值
	///@param	pPositionOrder	新的值
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	///@return	true表示成功，false表示失败
	bool updateByPK(CWriteablePositionOrder *pPositionOrder, CTransaction *pTransaction=NULL);

	///根据主键删除CPositionOrder
	///@param	pPositionOrder	要删除的值
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	///@return	true表示成功，false表示失败
	bool removeByPK(CPositionOrder *pPositionOrder, CTransaction *pTransaction=NULL);
	
	///根据主键删除CPositionOrder
	///@param	pPositionOrder	要删除的值
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	///@return	true表示成功，false表示失败
	bool removeByPK(CPositionOrderField *pPositionOrder, CTransaction *pTransaction=NULL);	

	friend class CPositionOrderIteratorByInsertTime;

	///寻找下一个符合条件的CPositionOrder，必须在startFindByInsertTime以后叫用
	///@return	下一个满足条件CPositionOrder，如果已经没有一个满足要求了，则返回NULL
	CPositionOrder *findNextByInsertTime();
	
	///寻找第一个CPositionOrder
	///@return	只寻找满足条件的第一个CPositionOrder，如果找不到，返回NULL
	CPositionOrder *findFirstByInsertTime( const CReadOnlyLongIndexIDType& MemberID,  const CReadOnlyIndexIDType& TradeUnitID,  const CReadOnlyExchangeIDType& ExchangeID,  const CReadOnlyInstrumentIDType& InstrumentID,  const CReadOnlyPosiDirectionType& PosiDirection);
	

private:
	///完成自己的初始化工作
	///@param	maxUnit	最多的数据对象个数
	///@param	pAllocator	进行内存管理的分配器
	///@param	pIndexMap	索引名称和内存编号的对应表，在非重用时会写入，在重用时只读出
	///@param	reuse	是否重用
	///@param	reuseID	重用编号，非重用时应当置为-1
	///@param	readOnly	1表示该CFixMem是只读的，0表示可以读写
	void init(int maxUnit,IMemoryAllocator *pAllocator,map<string,int>* pIndexMap,bool reuse,int reuseID,int readOnly);
	
	///将一个CPositionOrder加入到该对象工厂，实质上是完成修改索引的工作
	///@param	pPositionOrder	要加入的PositionOrder
	///@param	bNoTransaction 是否不属于任何事务
	///@return	加入后该对象的位置，当pTransaction!=NULL时，总是返回NULL
	CPositionOrder* internalAdd(CWriteablePositionOrder *pPositionOrder, bool bNoTransaction);
	
	
	///刷新该CPositionOrder的键值
	///@param	pPositionOrder	被刷新的CPositionOrder
	///@param	pNewPositionOrder	新的值
	///@param	bNoTransaction 是否不属于任何事务
	CPositionOrder* internalUpdate(CPositionOrder *pPositionOrder, CWriteablePositionOrder *pNewPositionOrder, bool bNoTransaction);
	
	///删除一个CPositionOrder，同时删除其索引
	///@param	pPositionOrder	要删除的CPositionOrder
	///@param	bNoTransaction 是否不属于任何事务
	void internalRemove(CPositionOrder *pPositionOrder, bool bNoTransaction);
/*
	///检查某个CPositionOrder是否属于本对象工厂
	///@param	pPositionOrder	要检查的对象
	///@exception	CDesignError	如果不属于，则抛出此异常
	void checkAddress(CPositionOrder *pPositionOrder);
*/

	///基于MemberID+TradeUnitID+ExchangeID+InstrumentID+PosiDirection+InsertTime的索引
	CAVLTree *pInsertTimeIndex;
	
	///基于OrderID的hash索引
	CHashIndex *pOrderIDHashIndex;
	
	///按照MemberID，TradeUnitID，ExchangeID，InstrumentID，PosiDirection检索时，存储的查询参数
	CLongIndexIDType queryMemberIDInSearchByInsertTime;
	CIndexIDType queryTradeUnitIDInSearchByInsertTime;
	CExchangeIDType queryExchangeIDInSearchByInsertTime;
	CInstrumentIDType queryInstrumentIDInSearchByInsertTime;
	CPosiDirectionType queryPosiDirectionInSearchByInsertTime;
	
	///按照MemberID，TradeUnitID，ExchangeID，InstrumentID，PosiDirection检索时，存储最后一次找到的索引节点
	CAVLNode *pLastFoundInSearchByInsertTime;
		
	///确认加入后触发
	///@param	pPositionOrder	已经加入的PositionOrder
	virtual void commitAdd(CPositionOrder *pPositionOrder);

	///确认更新后触发
	///@param	pPositionOrder	被刷新的CPositionOrder
	///@param	poldPositionOrder	原来的值
	virtual void commitUpdate(CPositionOrder *pPositionOrder, CWriteablePositionOrder *pOldPositionOrder);
	
	///确认删除后触发
	///@param	pPositionOrder	已经删除的CPositionOrder
	virtual void commitRemove(CWriteablePositionOrder *pPositionOrder);

	///所有相关的触发器
	vector<CPositionOrderActionTrigger *> *pActionTriggers;
	vector<CPositionOrderCommitTrigger *> *pCommitTriggers;
	
	///进行查询时，用于比较的对象
	CWriteablePositionOrder compareObject;
	CMDB* m_pMDB;
};

/////////////////////////////////////////////////////////////////////////
///CTriggerOrderFactory是一个报单的对象工厂。它包含了一批报单，
///同时又建立了如下索引，以方便查询：
///	AccountIDLocalIDIndex
///	ActivePositionCloseIndex
///	BuySLTriggerPriceIndex
///	SellSLTriggerPriceIndex
///	BuyTPTriggerPriceIndex
///	SellTPTriggerPriceIndex
///使用本对象工厂，可以完成对报单的增删改查操作。具体方法如下：
///	增加：叫用add，完成加入
///	删除：叫用remove就可以删除
///	查询：有如下方法可以查询：
///		使用findByOrderID，唯一找到对象
///		使用findByLocalID，唯一找到对象
///		使用startFindByBuySLTriggerPrice，findNextByBuySLTriggerPrice完成查询操作
///		使用startFindBySellSLTriggerPrice，findNextBySellSLTriggerPrice完成查询操作
///		使用startFindByBuyTPTriggerPrice，findNextByBuyTPTriggerPrice完成查询操作
///		使用startFindBySellTPTriggerPrice，findNextBySellTPTriggerPrice完成查询操作
///		使用startFindByAccountID，findNextByAccountID完成查询操作
///		使用startFindByActivePositionClose，findNextByActivePositionClose完成查询操作
///	修改：对于查到的结果，可以通过retrieve取出，修改后，通过update放回
///	事务管理：所有的增加、删除和修改都可以指定事务，在事务commit时，真正写入
///	在retrieve时，也可以指定事务，此时将取出经过update的新数据
/////////////////////////////////////////////////////////////////////////
class CTriggerOrderFactory :public CDatabaseDataFactory
{
public:
	///构造方法
	///@param	maxUnit	初始的数据对象个数
	///@param	pAllocator	进行内存管理的分配器
	///@param	pIndexMap	索引名称和内存编号的对应表，在非重用时会写入，在重用时只读出
	///@param	reuse	是否重用
	///@param	reuseID	重用编号，非重用时应当置为-1
	///@param	readOnly	1表示该CFixMem是只读的，0表示可以读写
	CTriggerOrderFactory(CMDB* pMDB,int maxUnit,IMemoryAllocator *pAllocator,map<string,int>* pIndexMap,bool reuse=false, int reuseID=-1, int readOnly=0);
	
	///构造方法
	///@param	pAllocator	进行内存管理的分配器
	///@param	pIndexMap	索引名称和内存编号的对应表，在非重用时会写入，在重用时只读出
	///@param	reuse	是否重用
	///@param	reuseID	重用编号，非重用时应当置为-1
	///@param	readOnly	1表示该CFixMem是只读的，0表示可以读写
	CTriggerOrderFactory(CMDB* pMDB,IMemoryAllocator *pAllocator,map<string,int>* pIndexMap,bool reuse=false, int reuseID = -1,int readOnly=0);
	
	///析构方法，将负责释放所占用的空间
	virtual ~CTriggerOrderFactory();

	///从CSV文件中读取所有的TriggerOrder
	///@param	filename	文件名
	///@return	1表示完成，0表示没有完成
	virtual int readCSV(char *filename);

	///从CSV文件中读取所有的TriggerOrder，文件名将根据配置文件中的定义TriggerOrderCSVFile
	///@return	1表示完成，0表示没有完成
	virtual int readCSVByPath(char *pPath);
	
	///向CSV文件中写出所有的TriggerOrder
	///@param	filename	文件名
	///@return	1表示完成，0表示没有完成
	virtual int writeCSV(char *filename);
	
	///向CSV文件中写出上一个版本的TriggerOrder
	///@param	filename	文件名
	///@return	1表示完成，0表示没有完成
	virtual int writeCSVPreVersion(char *filename);	

	///向CSV文件中写出所有的TriggerOrder，文件名将根据配置文件中的定义TriggerOrderCSVFile
	///@return	1表示完成，0表示没有完成
	virtual int writeCSVByPath(char *pPath);

	///将内容写到文件中,用于DEBUG
	virtual void dump(FILE *);

	///将内容写到文件中,用于DEBUG
	virtual void dump(char *filename);

	///清空所有的记录
	virtual void clearAll();

	friend class CTriggerOrderResource;

	///将一个CTriggerOrder加入到该对象工厂，实质上是完成修改索引的工作
	///@param	pTriggerOrder	要加入的TriggerOrder
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	///@return	加入后该对象的位置，当pTransaction!=NULL时，总是返回NULL
	CTriggerOrder* add(CWriteableTriggerOrder *pTriggerOrder, CTransaction *pTransaction=NULL);
	
	///刷新该CTriggerOrder的值
	///@param	pTriggerOrder	被刷新的CTriggerOrder
	///@param	pNewTriggerOrder	新的值
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	CTriggerOrder* update(CTriggerOrder *pTriggerOrder, CWriteableTriggerOrder *pNewTriggerOrder, CTransaction *pTransaction=NULL, bool bNoTransaction = true);
	
	///增加或者更新数据
	///@param	pTriggerOrder	  需要被刷新或者新增的CTriggerOrder,等于NULL表示是需要新增的
	///@param	pNewTriggerOrder	新的值
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	///@param	新增或者修改后的数据
	CTriggerOrder* addOrUpdate(CTriggerOrder *pTriggerOrder, CWriteableTriggerOrder *pNewTriggerOrder, CTransaction *pTransaction=NULL);
	
	///删除一个CTriggerOrder，同时删除其索引
	///@param	pTriggerOrder	要删除的CTriggerOrder
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	void remove(CTriggerOrder *pTriggerOrder, CTransaction *pTransaction=NULL, bool bNoTransaction=true);
	
	///获取某个CTriggerOrder
	///@param	pTriggerOrder	要读取的CTriggerOrder
	///@param	pTragetTriggerOrder	存放结果的CTriggerOrder
	void retrieve(CTriggerOrder *pTriggerOrder, CWriteableTriggerOrder *pTargetTriggerOrder);
	
	///获取第一个CTriggerOrder
	///@return	得到的第一个CTriggerOrder，如果没有，则返回NULL
	CTriggerOrder* getFirst();
	
	///获取下一个CTriggerOrder
	///@return	得到下一个CTriggerOrder，如果没有，则返回NULL
	CTriggerOrder* getNext();
	
	///加入一个操作触发器
	///@param	pTrigger	要加入的操作触发器
	///@return	1表示成功，0表示失败
	int addActionTrigger(CTriggerOrderActionTrigger *pTrigger);
	
	///删除一个操作触发器
	///@param	pTrigger	要删除的操作触发器
	///@return	1表示成功，0表示失败
	int removeActionTrigger(CTriggerOrderActionTrigger *pTrigger);

	///加入一个确认触发器
	///@param	pTrigger	要加入的确认触发器
	///@return	1表示成功，0表示失败
	int addCommitTrigger(CTriggerOrderCommitTrigger *pTrigger);
	
	///删除一个确认触发器
	///@param	pTrigger	要删除的确认触发器
	///@return	1表示成功，0表示失败
	int removeCommitTrigger(CTriggerOrderCommitTrigger *pTrigger);
	
	///构造所有对应的报单的指针
	///@param	pFactory	构造对应的报单的Link的指针时，寻找的对象工厂
	void linkAllOrder();

	///构造所有对应的用户的指针
	///@param	pFactory	构造对应的用户的Link的指针时，寻找的对象工厂
	void linkAllMember();

	///构造所有对应的标的的指针
	///@param	pFactory	构造对应的标的的Link的指针时，寻找的对象工厂
	void linkAllInstrument();

	///构造所有对应的行情的指针
	///@param	pFactory	构造对应的行情的Link的指针时，寻找的对象工厂
	void linkAllMarketData();

	///检查对应的报单的指针
	///@param pFile 输出错误日志的文件
	void checkLinkOrder(FILE *pFile);
	
	///检查对应的用户的指针
	///@param pFile 输出错误日志的文件
	void checkLinkMember(FILE *pFile);
	
	///检查对应的标的的指针
	///@param pFile 输出错误日志的文件
	void checkLinkInstrument(FILE *pFile);
	
	///检查对应的行情的指针
	///@param pFile 输出错误日志的文件
	void checkLinkMarketData(FILE *pFile);
	
	///清空对应的报单的外键
	void clearLinkOrder();
	
	///清空对应的用户的外键
	void clearLinkMember();
	
	///清空对应的标的的外键
	void clearLinkInstrument();
	
	///清空对应的行情的外键
	void clearLinkMarketData();
	
	///检查是否字段为NULL
	///@param pFile 输出错误日志的文件
	void checkNull(FILE *pFile);
	
	///检查是否字段合法
	///@param pFile 输出错误日志的文件
	void checkValid(FILE *pFile);	

	///寻找CTriggerOrder
	///@return	找到的CTriggerOrder，如果找不到，返回NULL
	CTriggerOrder *findByOrderID(const CReadOnlyIndexIDType&  OrderID);

	///根据主码寻找
	///@return	找到的CTriggerOrder，如果找不到，返回NULL
	CTriggerOrder *findByPK(const CReadOnlyIndexIDType& OrderID);
		
	///根据主键删除CTriggerOrder
	bool removeByPK(const CReadOnlyIndexIDType& OrderID, CTransaction *pTransaction=NULL);
	
	///判断字段是否是是主键
	bool isPK(const char* pName);
	
	///根据主键寻找CTriggerOrder
	///@param	pTriggerOrder	要找的值
	///@return	找到的CTriggerOrder，如果找不到，返回NULL
	CTriggerOrder *findByPK(CTriggerOrderField *pTriggerOrder);
	
	///根据主键寻找CTriggerOrder
	///@param	pTriggerOrder	要找的值
	///@return	找到的CTriggerOrder，如果找不到，返回NULL
	CTriggerOrder *findByPK(CTriggerOrder *pTriggerOrder);

	///根据主键获取CTriggerOrder
	///@param	pTriggerOrder	要获取的值，同时用于存放返回的结果
	///@return	找到的CTriggerOrder，如果找不到，返回NULL
	bool retrieveByPK(CWriteableTriggerOrder *pTriggerOrder);

	///根据主键增加新的CTriggerOrder
	///@param	pTriggerOrder	新的值
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	///@return	返回插入的CTriggerOrder，如果插入失败，则返回NULL
	CTriggerOrder *addByPK(CWriteableTriggerOrder *pTriggerOrder, CTransaction *pTransaction=NULL);
	
	///根据主键增加或更新新的CTriggerOrder
	///@param	pTriggerOrder	新的值
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	///@return	返回插入的CTriggerOrder，如果插入失败，则返回NULL
	CTriggerOrder *addOrUpdateByPK(CWriteableTriggerOrder *pTriggerOrder, CTransaction *pTransaction=NULL);

	///根据主键更新CTriggerOrder的值
	///@param	pTriggerOrder	新的值
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	///@return	true表示成功，false表示失败
	bool updateByPK(CWriteableTriggerOrder *pTriggerOrder, CTransaction *pTransaction=NULL);

	///根据主键删除CTriggerOrder
	///@param	pTriggerOrder	要删除的值
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	///@return	true表示成功，false表示失败
	bool removeByPK(CTriggerOrder *pTriggerOrder, CTransaction *pTransaction=NULL);
	
	///根据主键删除CTriggerOrder
	///@param	pTriggerOrder	要删除的值
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	///@return	true表示成功，false表示失败
	bool removeByPK(CTriggerOrderField *pTriggerOrder, CTransaction *pTransaction=NULL);	

	///寻找CTriggerOrder
	///@return	找到的CTriggerOrder，如果找不到，返回NULL
	CTriggerOrder *findByLocalID(const CReadOnlyAccountIDType&  AccountID, const CReadOnlyIndexIDType&  LocalID);


	friend class CTriggerOrderIteratorByBuySLTriggerPrice;

	///寻找下一个符合条件的CTriggerOrder，必须在startFindByBuySLTriggerPrice以后叫用
	///@return	下一个满足条件CTriggerOrder，如果已经没有一个满足要求了，则返回NULL
	CTriggerOrder *findNextByBuySLTriggerPrice();
	
	///寻找第一个CTriggerOrder
	///@return	只寻找满足条件的第一个CTriggerOrder，如果找不到，返回NULL
	CTriggerOrder *findFirstByBuySLTriggerPrice( const CReadOnlyExchangeIDType& ExchangeID,  const CReadOnlyInstrumentIDType& InstrumentID,  const CReadOnlyPriceType& SLTriggerPrice);
	
	friend class CTriggerOrderIteratorBySellSLTriggerPrice;

	///寻找下一个符合条件的CTriggerOrder，必须在startFindBySellSLTriggerPrice以后叫用
	///@return	下一个满足条件CTriggerOrder，如果已经没有一个满足要求了，则返回NULL
	CTriggerOrder *findNextBySellSLTriggerPrice();
	
	///寻找第一个CTriggerOrder
	///@return	只寻找满足条件的第一个CTriggerOrder，如果找不到，返回NULL
	CTriggerOrder *findFirstBySellSLTriggerPrice( const CReadOnlyExchangeIDType& ExchangeID,  const CReadOnlyInstrumentIDType& InstrumentID,  const CReadOnlyPriceType& SLTriggerPrice);
	
	friend class CTriggerOrderIteratorByBuyTPTriggerPrice;

	///寻找下一个符合条件的CTriggerOrder，必须在startFindByBuyTPTriggerPrice以后叫用
	///@return	下一个满足条件CTriggerOrder，如果已经没有一个满足要求了，则返回NULL
	CTriggerOrder *findNextByBuyTPTriggerPrice();
	
	///寻找第一个CTriggerOrder
	///@return	只寻找满足条件的第一个CTriggerOrder，如果找不到，返回NULL
	CTriggerOrder *findFirstByBuyTPTriggerPrice( const CReadOnlyExchangeIDType& ExchangeID,  const CReadOnlyInstrumentIDType& InstrumentID,  const CReadOnlyPriceType& TPTriggerPrice);
	
	friend class CTriggerOrderIteratorBySellTPTriggerPrice;

	///寻找下一个符合条件的CTriggerOrder，必须在startFindBySellTPTriggerPrice以后叫用
	///@return	下一个满足条件CTriggerOrder，如果已经没有一个满足要求了，则返回NULL
	CTriggerOrder *findNextBySellTPTriggerPrice();
	
	///寻找第一个CTriggerOrder
	///@return	只寻找满足条件的第一个CTriggerOrder，如果找不到，返回NULL
	CTriggerOrder *findFirstBySellTPTriggerPrice( const CReadOnlyExchangeIDType& ExchangeID,  const CReadOnlyInstrumentIDType& InstrumentID,  const CReadOnlyPriceType& TPTriggerPrice);
	
	friend class CTriggerOrderIteratorByAccountID;

	///寻找下一个符合条件的CTriggerOrder，必须在startFindByAccountID以后叫用
	///@return	下一个满足条件CTriggerOrder，如果已经没有一个满足要求了，则返回NULL
	CTriggerOrder *findNextByAccountID();
	
	///寻找第一个CTriggerOrder
	///@return	只寻找满足条件的第一个CTriggerOrder，如果找不到，返回NULL
	CTriggerOrder *findFirstByAccountID( const CReadOnlyAccountIDType& AccountID);
	
	friend class CTriggerOrderIteratorByActivePositionClose;

	///寻找下一个符合条件的CTriggerOrder，必须在startFindByActivePositionClose以后叫用
	///@return	下一个满足条件CTriggerOrder，如果已经没有一个满足要求了，则返回NULL
	CTriggerOrder *findNextByActivePositionClose();
	
	///寻找第一个CTriggerOrder
	///@return	只寻找满足条件的第一个CTriggerOrder，如果找不到，返回NULL
	CTriggerOrder *findFirstByActivePositionClose( const CReadOnlyLongIndexIDType& PositionID);
	

private:
	///完成自己的初始化工作
	///@param	maxUnit	最多的数据对象个数
	///@param	pAllocator	进行内存管理的分配器
	///@param	pIndexMap	索引名称和内存编号的对应表，在非重用时会写入，在重用时只读出
	///@param	reuse	是否重用
	///@param	reuseID	重用编号，非重用时应当置为-1
	///@param	readOnly	1表示该CFixMem是只读的，0表示可以读写
	void init(int maxUnit,IMemoryAllocator *pAllocator,map<string,int>* pIndexMap,bool reuse,int reuseID,int readOnly);
	
	///将一个CTriggerOrder加入到该对象工厂，实质上是完成修改索引的工作
	///@param	pTriggerOrder	要加入的TriggerOrder
	///@param	bNoTransaction 是否不属于任何事务
	///@return	加入后该对象的位置，当pTransaction!=NULL时，总是返回NULL
	CTriggerOrder* internalAdd(CWriteableTriggerOrder *pTriggerOrder, bool bNoTransaction);
	
	
	///刷新该CTriggerOrder的键值
	///@param	pTriggerOrder	被刷新的CTriggerOrder
	///@param	pNewTriggerOrder	新的值
	///@param	bNoTransaction 是否不属于任何事务
	CTriggerOrder* internalUpdate(CTriggerOrder *pTriggerOrder, CWriteableTriggerOrder *pNewTriggerOrder, bool bNoTransaction);
	
	///删除一个CTriggerOrder，同时删除其索引
	///@param	pTriggerOrder	要删除的CTriggerOrder
	///@param	bNoTransaction 是否不属于任何事务
	void internalRemove(CTriggerOrder *pTriggerOrder, bool bNoTransaction);
/*
	///检查某个CTriggerOrder是否属于本对象工厂
	///@param	pTriggerOrder	要检查的对象
	///@exception	CDesignError	如果不属于，则抛出此异常
	void checkAddress(CTriggerOrder *pTriggerOrder);
*/

	///基于AccountID+LocalID的索引
	CAVLTree *pAccountIDLocalIDIndex;
	///基于PositionID的索引
	CAVLTree *pActivePositionCloseIndex;
	///基于ExchangeID+InstrumentID+Direction+SLTriggerPrice+TimeSortNo的索引
	CAVLTree *pBuySLTriggerPriceIndex;
	///基于ExchangeID+InstrumentID+Direction+SLTriggerPrice+TimeSortNo的索引
	CAVLTree *pSellSLTriggerPriceIndex;
	///基于ExchangeID+InstrumentID+Direction+TPTriggerPrice+TimeSortNo的索引
	CAVLTree *pBuyTPTriggerPriceIndex;
	///基于ExchangeID+InstrumentID+Direction+TPTriggerPrice+TimeSortNo的索引
	CAVLTree *pSellTPTriggerPriceIndex;
	
	///基于OrderID的hash索引
	CHashIndex *pOrderIDHashIndex;
	
	///按照ExchangeID，InstrumentID，SLTriggerPrice检索时，存储的查询参数
	CExchangeIDType queryExchangeIDInSearchByBuySLTriggerPrice;
	CInstrumentIDType queryInstrumentIDInSearchByBuySLTriggerPrice;
	CPriceType querySLTriggerPriceInSearchByBuySLTriggerPrice;
	
	///按照ExchangeID，InstrumentID，SLTriggerPrice检索时，存储最后一次找到的索引节点
	CAVLNode *pLastFoundInSearchByBuySLTriggerPrice;
	///按照ExchangeID，InstrumentID，SLTriggerPrice检索时，存储的查询参数
	CExchangeIDType queryExchangeIDInSearchBySellSLTriggerPrice;
	CInstrumentIDType queryInstrumentIDInSearchBySellSLTriggerPrice;
	CPriceType querySLTriggerPriceInSearchBySellSLTriggerPrice;
	
	///按照ExchangeID，InstrumentID，SLTriggerPrice检索时，存储最后一次找到的索引节点
	CAVLNode *pLastFoundInSearchBySellSLTriggerPrice;
	///按照ExchangeID，InstrumentID，TPTriggerPrice检索时，存储的查询参数
	CExchangeIDType queryExchangeIDInSearchByBuyTPTriggerPrice;
	CInstrumentIDType queryInstrumentIDInSearchByBuyTPTriggerPrice;
	CPriceType queryTPTriggerPriceInSearchByBuyTPTriggerPrice;
	
	///按照ExchangeID，InstrumentID，TPTriggerPrice检索时，存储最后一次找到的索引节点
	CAVLNode *pLastFoundInSearchByBuyTPTriggerPrice;
	///按照ExchangeID，InstrumentID，TPTriggerPrice检索时，存储的查询参数
	CExchangeIDType queryExchangeIDInSearchBySellTPTriggerPrice;
	CInstrumentIDType queryInstrumentIDInSearchBySellTPTriggerPrice;
	CPriceType queryTPTriggerPriceInSearchBySellTPTriggerPrice;
	
	///按照ExchangeID，InstrumentID，TPTriggerPrice检索时，存储最后一次找到的索引节点
	CAVLNode *pLastFoundInSearchBySellTPTriggerPrice;
	///按照AccountID检索时，存储的查询参数
	CAccountIDType queryAccountIDInSearchByAccountID;
	
	///按照AccountID检索时，存储最后一次找到的索引节点
	CAVLNode *pLastFoundInSearchByAccountID;
	///按照PositionID检索时，存储的查询参数
	CLongIndexIDType queryPositionIDInSearchByActivePositionClose;
	
	///按照PositionID检索时，存储最后一次找到的索引节点
	CAVLNode *pLastFoundInSearchByActivePositionClose;
		
	///确认加入后触发
	///@param	pTriggerOrder	已经加入的TriggerOrder
	virtual void commitAdd(CTriggerOrder *pTriggerOrder);

	///确认更新后触发
	///@param	pTriggerOrder	被刷新的CTriggerOrder
	///@param	poldTriggerOrder	原来的值
	virtual void commitUpdate(CTriggerOrder *pTriggerOrder, CWriteableTriggerOrder *pOldTriggerOrder);
	
	///确认删除后触发
	///@param	pTriggerOrder	已经删除的CTriggerOrder
	virtual void commitRemove(CWriteableTriggerOrder *pTriggerOrder);

	///所有相关的触发器
	vector<CTriggerOrderActionTrigger *> *pActionTriggers;
	vector<CTriggerOrderCommitTrigger *> *pCommitTriggers;
	
	///进行查询时，用于比较的对象
	CWriteableTriggerOrder compareObject;
	CMDB* m_pMDB;
};

/////////////////////////////////////////////////////////////////////////
///CMarketOrderFactory是一个分价表的对象工厂。它包含了一批分价表，
///同时又建立了如下索引，以方便查询：
///	BuyMBLIndex
///	SellMBLIndex
///使用本对象工厂，可以完成对分价表的增删改查操作。具体方法如下：
///	增加：叫用add，完成加入
///	删除：叫用remove就可以删除
///	查询：有如下方法可以查询：
///		使用findByPrice，唯一找到对象
///		使用startFindByBuyMBLData，findNextByBuyMBLData完成查询操作
///		使用startFindBySellMBLData，findNextBySellMBLData完成查询操作
///	修改：对于查到的结果，可以通过retrieve取出，修改后，通过update放回
///	事务管理：所有的增加、删除和修改都可以指定事务，在事务commit时，真正写入
///	在retrieve时，也可以指定事务，此时将取出经过update的新数据
/////////////////////////////////////////////////////////////////////////
class CMarketOrderFactory :public CDatabaseDataFactory
{
public:
	///构造方法
	///@param	maxUnit	初始的数据对象个数
	///@param	pAllocator	进行内存管理的分配器
	///@param	pIndexMap	索引名称和内存编号的对应表，在非重用时会写入，在重用时只读出
	///@param	reuse	是否重用
	///@param	reuseID	重用编号，非重用时应当置为-1
	///@param	readOnly	1表示该CFixMem是只读的，0表示可以读写
	CMarketOrderFactory(CMDB* pMDB,int maxUnit,IMemoryAllocator *pAllocator,map<string,int>* pIndexMap,bool reuse=false, int reuseID=-1, int readOnly=0);
	
	///构造方法
	///@param	pAllocator	进行内存管理的分配器
	///@param	pIndexMap	索引名称和内存编号的对应表，在非重用时会写入，在重用时只读出
	///@param	reuse	是否重用
	///@param	reuseID	重用编号，非重用时应当置为-1
	///@param	readOnly	1表示该CFixMem是只读的，0表示可以读写
	CMarketOrderFactory(CMDB* pMDB,IMemoryAllocator *pAllocator,map<string,int>* pIndexMap,bool reuse=false, int reuseID = -1,int readOnly=0);
	
	///析构方法，将负责释放所占用的空间
	virtual ~CMarketOrderFactory();

	///从CSV文件中读取所有的MarketOrder
	///@param	filename	文件名
	///@return	1表示完成，0表示没有完成
	virtual int readCSV(char *filename);

	///从CSV文件中读取所有的MarketOrder，文件名将根据配置文件中的定义MarketOrderCSVFile
	///@return	1表示完成，0表示没有完成
	virtual int readCSVByPath(char *pPath);
	
	///向CSV文件中写出所有的MarketOrder
	///@param	filename	文件名
	///@return	1表示完成，0表示没有完成
	virtual int writeCSV(char *filename);
	
	///向CSV文件中写出上一个版本的MarketOrder
	///@param	filename	文件名
	///@return	1表示完成，0表示没有完成
	virtual int writeCSVPreVersion(char *filename);	

	///向CSV文件中写出所有的MarketOrder，文件名将根据配置文件中的定义MarketOrderCSVFile
	///@return	1表示完成，0表示没有完成
	virtual int writeCSVByPath(char *pPath);

	///将内容写到文件中,用于DEBUG
	virtual void dump(FILE *);

	///将内容写到文件中,用于DEBUG
	virtual void dump(char *filename);

	///清空所有的记录
	virtual void clearAll();

	friend class CMarketOrderResource;

	///将一个CMarketOrder加入到该对象工厂，实质上是完成修改索引的工作
	///@param	pMarketOrder	要加入的MarketOrder
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	///@return	加入后该对象的位置，当pTransaction!=NULL时，总是返回NULL
	CMarketOrder* add(CWriteableMarketOrder *pMarketOrder, CTransaction *pTransaction=NULL);
	
	///刷新该CMarketOrder的值
	///@param	pMarketOrder	被刷新的CMarketOrder
	///@param	pNewMarketOrder	新的值
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	CMarketOrder* update(CMarketOrder *pMarketOrder, CWriteableMarketOrder *pNewMarketOrder, CTransaction *pTransaction=NULL, bool bNoTransaction = true);
	
	///增加或者更新数据
	///@param	pMarketOrder	  需要被刷新或者新增的CMarketOrder,等于NULL表示是需要新增的
	///@param	pNewMarketOrder	新的值
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	///@param	新增或者修改后的数据
	CMarketOrder* addOrUpdate(CMarketOrder *pMarketOrder, CWriteableMarketOrder *pNewMarketOrder, CTransaction *pTransaction=NULL);
	
	///删除一个CMarketOrder，同时删除其索引
	///@param	pMarketOrder	要删除的CMarketOrder
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	void remove(CMarketOrder *pMarketOrder, CTransaction *pTransaction=NULL, bool bNoTransaction=true);
	
	///获取某个CMarketOrder
	///@param	pMarketOrder	要读取的CMarketOrder
	///@param	pTragetMarketOrder	存放结果的CMarketOrder
	void retrieve(CMarketOrder *pMarketOrder, CWriteableMarketOrder *pTargetMarketOrder);
	
	///获取第一个CMarketOrder
	///@return	得到的第一个CMarketOrder，如果没有，则返回NULL
	CMarketOrder* getFirst();
	
	///获取下一个CMarketOrder
	///@return	得到下一个CMarketOrder，如果没有，则返回NULL
	CMarketOrder* getNext();
	
	///加入一个操作触发器
	///@param	pTrigger	要加入的操作触发器
	///@return	1表示成功，0表示失败
	int addActionTrigger(CMarketOrderActionTrigger *pTrigger);
	
	///删除一个操作触发器
	///@param	pTrigger	要删除的操作触发器
	///@return	1表示成功，0表示失败
	int removeActionTrigger(CMarketOrderActionTrigger *pTrigger);

	///加入一个确认触发器
	///@param	pTrigger	要加入的确认触发器
	///@return	1表示成功，0表示失败
	int addCommitTrigger(CMarketOrderCommitTrigger *pTrigger);
	
	///删除一个确认触发器
	///@param	pTrigger	要删除的确认触发器
	///@return	1表示成功，0表示失败
	int removeCommitTrigger(CMarketOrderCommitTrigger *pTrigger);
	
	///检查是否字段为NULL
	///@param pFile 输出错误日志的文件
	void checkNull(FILE *pFile);
	
	///检查是否字段合法
	///@param pFile 输出错误日志的文件
	void checkValid(FILE *pFile);	

	///寻找CMarketOrder
	///@return	找到的CMarketOrder，如果找不到，返回NULL
	CMarketOrder *findByPrice(const CReadOnlyExchangeIDType&  ExchangeID, const CReadOnlyInstrumentIDType&  InstrumentID, const CReadOnlyPriceType&  Price);

	///根据主码寻找
	///@return	找到的CMarketOrder，如果找不到，返回NULL
	CMarketOrder *findByPK(const CReadOnlyExchangeIDType& ExchangeID, const CReadOnlyInstrumentIDType& InstrumentID, const CReadOnlyPriceType& Price);
		
	///根据主键删除CMarketOrder
	bool removeByPK(const CReadOnlyExchangeIDType& ExchangeID, const CReadOnlyInstrumentIDType& InstrumentID, const CReadOnlyPriceType& Price, CTransaction *pTransaction=NULL);
	
	///判断字段是否是是主键
	bool isPK(const char* pName);
	
	///根据主键寻找CMarketOrder
	///@param	pMarketOrder	要找的值
	///@return	找到的CMarketOrder，如果找不到，返回NULL
	CMarketOrder *findByPK(CMarketOrderField *pMarketOrder);
	
	///根据主键寻找CMarketOrder
	///@param	pMarketOrder	要找的值
	///@return	找到的CMarketOrder，如果找不到，返回NULL
	CMarketOrder *findByPK(CMarketOrder *pMarketOrder);

	///根据主键获取CMarketOrder
	///@param	pMarketOrder	要获取的值，同时用于存放返回的结果
	///@return	找到的CMarketOrder，如果找不到，返回NULL
	bool retrieveByPK(CWriteableMarketOrder *pMarketOrder);

	///根据主键增加新的CMarketOrder
	///@param	pMarketOrder	新的值
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	///@return	返回插入的CMarketOrder，如果插入失败，则返回NULL
	CMarketOrder *addByPK(CWriteableMarketOrder *pMarketOrder, CTransaction *pTransaction=NULL);
	
	///根据主键增加或更新新的CMarketOrder
	///@param	pMarketOrder	新的值
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	///@return	返回插入的CMarketOrder，如果插入失败，则返回NULL
	CMarketOrder *addOrUpdateByPK(CWriteableMarketOrder *pMarketOrder, CTransaction *pTransaction=NULL);

	///根据主键更新CMarketOrder的值
	///@param	pMarketOrder	新的值
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	///@return	true表示成功，false表示失败
	bool updateByPK(CWriteableMarketOrder *pMarketOrder, CTransaction *pTransaction=NULL);

	///根据主键删除CMarketOrder
	///@param	pMarketOrder	要删除的值
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	///@return	true表示成功，false表示失败
	bool removeByPK(CMarketOrder *pMarketOrder, CTransaction *pTransaction=NULL);
	
	///根据主键删除CMarketOrder
	///@param	pMarketOrder	要删除的值
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	///@return	true表示成功，false表示失败
	bool removeByPK(CMarketOrderField *pMarketOrder, CTransaction *pTransaction=NULL);	

	friend class CMarketOrderIteratorByBuyMBLData;

	///寻找下一个符合条件的CMarketOrder，必须在startFindByBuyMBLData以后叫用
	///@return	下一个满足条件CMarketOrder，如果已经没有一个满足要求了，则返回NULL
	CMarketOrder *findNextByBuyMBLData();
	
	///寻找第一个CMarketOrder
	///@return	只寻找满足条件的第一个CMarketOrder，如果找不到，返回NULL
	CMarketOrder *findFirstByBuyMBLData( const CReadOnlyExchangeIDType& ExchangeID,  const CReadOnlyInstrumentIDType& InstrumentID);
	
	friend class CMarketOrderIteratorBySellMBLData;

	///寻找下一个符合条件的CMarketOrder，必须在startFindBySellMBLData以后叫用
	///@return	下一个满足条件CMarketOrder，如果已经没有一个满足要求了，则返回NULL
	CMarketOrder *findNextBySellMBLData();
	
	///寻找第一个CMarketOrder
	///@return	只寻找满足条件的第一个CMarketOrder，如果找不到，返回NULL
	CMarketOrder *findFirstBySellMBLData( const CReadOnlyExchangeIDType& ExchangeID,  const CReadOnlyInstrumentIDType& InstrumentID);
	

private:
	///完成自己的初始化工作
	///@param	maxUnit	最多的数据对象个数
	///@param	pAllocator	进行内存管理的分配器
	///@param	pIndexMap	索引名称和内存编号的对应表，在非重用时会写入，在重用时只读出
	///@param	reuse	是否重用
	///@param	reuseID	重用编号，非重用时应当置为-1
	///@param	readOnly	1表示该CFixMem是只读的，0表示可以读写
	void init(int maxUnit,IMemoryAllocator *pAllocator,map<string,int>* pIndexMap,bool reuse,int reuseID,int readOnly);
	
	///将一个CMarketOrder加入到该对象工厂，实质上是完成修改索引的工作
	///@param	pMarketOrder	要加入的MarketOrder
	///@param	bNoTransaction 是否不属于任何事务
	///@return	加入后该对象的位置，当pTransaction!=NULL时，总是返回NULL
	CMarketOrder* internalAdd(CWriteableMarketOrder *pMarketOrder, bool bNoTransaction);
	
	
	///刷新该CMarketOrder的键值
	///@param	pMarketOrder	被刷新的CMarketOrder
	///@param	pNewMarketOrder	新的值
	///@param	bNoTransaction 是否不属于任何事务
	CMarketOrder* internalUpdate(CMarketOrder *pMarketOrder, CWriteableMarketOrder *pNewMarketOrder, bool bNoTransaction);
	
	///删除一个CMarketOrder，同时删除其索引
	///@param	pMarketOrder	要删除的CMarketOrder
	///@param	bNoTransaction 是否不属于任何事务
	void internalRemove(CMarketOrder *pMarketOrder, bool bNoTransaction);
/*
	///检查某个CMarketOrder是否属于本对象工厂
	///@param	pMarketOrder	要检查的对象
	///@exception	CDesignError	如果不属于，则抛出此异常
	void checkAddress(CMarketOrder *pMarketOrder);
*/

	///基于ExchangeID+InstrumentID+Price的索引
	CAVLTree *pBuyMBLIndex;
	///基于ExchangeID+InstrumentID+Price的索引
	CAVLTree *pSellMBLIndex;
	
	///基于PriceHashKey的hash索引
	CHashIndex *pPriceHashIndex;
	
	///按照ExchangeID，InstrumentID检索时，存储的查询参数
	CExchangeIDType queryExchangeIDInSearchByBuyMBLData;
	CInstrumentIDType queryInstrumentIDInSearchByBuyMBLData;
	
	///按照ExchangeID，InstrumentID检索时，存储最后一次找到的索引节点
	CAVLNode *pLastFoundInSearchByBuyMBLData;
	///按照ExchangeID，InstrumentID检索时，存储的查询参数
	CExchangeIDType queryExchangeIDInSearchBySellMBLData;
	CInstrumentIDType queryInstrumentIDInSearchBySellMBLData;
	
	///按照ExchangeID，InstrumentID检索时，存储最后一次找到的索引节点
	CAVLNode *pLastFoundInSearchBySellMBLData;
		
	///确认加入后触发
	///@param	pMarketOrder	已经加入的MarketOrder
	virtual void commitAdd(CMarketOrder *pMarketOrder);

	///确认更新后触发
	///@param	pMarketOrder	被刷新的CMarketOrder
	///@param	poldMarketOrder	原来的值
	virtual void commitUpdate(CMarketOrder *pMarketOrder, CWriteableMarketOrder *pOldMarketOrder);
	
	///确认删除后触发
	///@param	pMarketOrder	已经删除的CMarketOrder
	virtual void commitRemove(CWriteableMarketOrder *pMarketOrder);

	///所有相关的触发器
	vector<CMarketOrderActionTrigger *> *pActionTriggers;
	vector<CMarketOrderCommitTrigger *> *pCommitTriggers;
	
	///进行查询时，用于比较的对象
	CWriteableMarketOrder compareObject;
	CMDB* m_pMDB;
};

/////////////////////////////////////////////////////////////////////////
///CTickMarketOrderFactory是一个Tick聚集分价表的对象工厂。它包含了一批Tick聚集分价表，
///同时又建立了如下索引，以方便查询：
///	BuyMBLIndex
///	SellMBLIndex
///使用本对象工厂，可以完成对Tick聚集分价表的增删改查操作。具体方法如下：
///	增加：叫用add，完成加入
///	删除：叫用remove就可以删除
///	查询：有如下方法可以查询：
///		使用startFindByBuyMBLData，findNextByBuyMBLData完成查询操作
///		使用startFindBySellMBLData，findNextBySellMBLData完成查询操作
///		使用findByPrice，唯一找到对象
///	修改：对于查到的结果，可以通过retrieve取出，修改后，通过update放回
///	事务管理：所有的增加、删除和修改都可以指定事务，在事务commit时，真正写入
///	在retrieve时，也可以指定事务，此时将取出经过update的新数据
/////////////////////////////////////////////////////////////////////////
class CTickMarketOrderFactory :public CDatabaseDataFactory
{
public:
	///构造方法
	///@param	maxUnit	初始的数据对象个数
	///@param	pAllocator	进行内存管理的分配器
	///@param	pIndexMap	索引名称和内存编号的对应表，在非重用时会写入，在重用时只读出
	///@param	reuse	是否重用
	///@param	reuseID	重用编号，非重用时应当置为-1
	///@param	readOnly	1表示该CFixMem是只读的，0表示可以读写
	CTickMarketOrderFactory(CMDB* pMDB,int maxUnit,IMemoryAllocator *pAllocator,map<string,int>* pIndexMap,bool reuse=false, int reuseID=-1, int readOnly=0);
	
	///构造方法
	///@param	pAllocator	进行内存管理的分配器
	///@param	pIndexMap	索引名称和内存编号的对应表，在非重用时会写入，在重用时只读出
	///@param	reuse	是否重用
	///@param	reuseID	重用编号，非重用时应当置为-1
	///@param	readOnly	1表示该CFixMem是只读的，0表示可以读写
	CTickMarketOrderFactory(CMDB* pMDB,IMemoryAllocator *pAllocator,map<string,int>* pIndexMap,bool reuse=false, int reuseID = -1,int readOnly=0);
	
	///析构方法，将负责释放所占用的空间
	virtual ~CTickMarketOrderFactory();

	///从CSV文件中读取所有的TickMarketOrder
	///@param	filename	文件名
	///@return	1表示完成，0表示没有完成
	virtual int readCSV(char *filename);

	///从CSV文件中读取所有的TickMarketOrder，文件名将根据配置文件中的定义TickMarketOrderCSVFile
	///@return	1表示完成，0表示没有完成
	virtual int readCSVByPath(char *pPath);
	
	///向CSV文件中写出所有的TickMarketOrder
	///@param	filename	文件名
	///@return	1表示完成，0表示没有完成
	virtual int writeCSV(char *filename);
	
	///向CSV文件中写出上一个版本的TickMarketOrder
	///@param	filename	文件名
	///@return	1表示完成，0表示没有完成
	virtual int writeCSVPreVersion(char *filename);	

	///向CSV文件中写出所有的TickMarketOrder，文件名将根据配置文件中的定义TickMarketOrderCSVFile
	///@return	1表示完成，0表示没有完成
	virtual int writeCSVByPath(char *pPath);

	///将内容写到文件中,用于DEBUG
	virtual void dump(FILE *);

	///将内容写到文件中,用于DEBUG
	virtual void dump(char *filename);

	///清空所有的记录
	virtual void clearAll();

	friend class CTickMarketOrderResource;

	///将一个CTickMarketOrder加入到该对象工厂，实质上是完成修改索引的工作
	///@param	pTickMarketOrder	要加入的TickMarketOrder
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	///@return	加入后该对象的位置，当pTransaction!=NULL时，总是返回NULL
	CTickMarketOrder* add(CWriteableTickMarketOrder *pTickMarketOrder, CTransaction *pTransaction=NULL);
	
	///刷新该CTickMarketOrder的值
	///@param	pTickMarketOrder	被刷新的CTickMarketOrder
	///@param	pNewTickMarketOrder	新的值
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	CTickMarketOrder* update(CTickMarketOrder *pTickMarketOrder, CWriteableTickMarketOrder *pNewTickMarketOrder, CTransaction *pTransaction=NULL, bool bNoTransaction = true);
	
	///增加或者更新数据
	///@param	pTickMarketOrder	  需要被刷新或者新增的CTickMarketOrder,等于NULL表示是需要新增的
	///@param	pNewTickMarketOrder	新的值
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	///@param	新增或者修改后的数据
	CTickMarketOrder* addOrUpdate(CTickMarketOrder *pTickMarketOrder, CWriteableTickMarketOrder *pNewTickMarketOrder, CTransaction *pTransaction=NULL);
	
	///删除一个CTickMarketOrder，同时删除其索引
	///@param	pTickMarketOrder	要删除的CTickMarketOrder
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	void remove(CTickMarketOrder *pTickMarketOrder, CTransaction *pTransaction=NULL, bool bNoTransaction=true);
	
	///获取某个CTickMarketOrder
	///@param	pTickMarketOrder	要读取的CTickMarketOrder
	///@param	pTragetTickMarketOrder	存放结果的CTickMarketOrder
	void retrieve(CTickMarketOrder *pTickMarketOrder, CWriteableTickMarketOrder *pTargetTickMarketOrder);
	
	///获取第一个CTickMarketOrder
	///@return	得到的第一个CTickMarketOrder，如果没有，则返回NULL
	CTickMarketOrder* getFirst();
	
	///获取下一个CTickMarketOrder
	///@return	得到下一个CTickMarketOrder，如果没有，则返回NULL
	CTickMarketOrder* getNext();
	
	///加入一个操作触发器
	///@param	pTrigger	要加入的操作触发器
	///@return	1表示成功，0表示失败
	int addActionTrigger(CTickMarketOrderActionTrigger *pTrigger);
	
	///删除一个操作触发器
	///@param	pTrigger	要删除的操作触发器
	///@return	1表示成功，0表示失败
	int removeActionTrigger(CTickMarketOrderActionTrigger *pTrigger);

	///加入一个确认触发器
	///@param	pTrigger	要加入的确认触发器
	///@return	1表示成功，0表示失败
	int addCommitTrigger(CTickMarketOrderCommitTrigger *pTrigger);
	
	///删除一个确认触发器
	///@param	pTrigger	要删除的确认触发器
	///@return	1表示成功，0表示失败
	int removeCommitTrigger(CTickMarketOrderCommitTrigger *pTrigger);
	
	///检查是否字段为NULL
	///@param pFile 输出错误日志的文件
	void checkNull(FILE *pFile);
	
	///检查是否字段合法
	///@param pFile 输出错误日志的文件
	void checkValid(FILE *pFile);	

	friend class CTickMarketOrderIteratorByBuyMBLData;

	///寻找下一个符合条件的CTickMarketOrder，必须在startFindByBuyMBLData以后叫用
	///@return	下一个满足条件CTickMarketOrder，如果已经没有一个满足要求了，则返回NULL
	CTickMarketOrder *findNextByBuyMBLData();
	
	///寻找第一个CTickMarketOrder
	///@return	只寻找满足条件的第一个CTickMarketOrder，如果找不到，返回NULL
	CTickMarketOrder *findFirstByBuyMBLData( const CReadOnlyExchangeIDType& ExchangeID,  const CReadOnlyInstrumentIDType& InstrumentID,  const CReadOnlyPriceType& Tick);
	
	friend class CTickMarketOrderIteratorBySellMBLData;

	///寻找下一个符合条件的CTickMarketOrder，必须在startFindBySellMBLData以后叫用
	///@return	下一个满足条件CTickMarketOrder，如果已经没有一个满足要求了，则返回NULL
	CTickMarketOrder *findNextBySellMBLData();
	
	///寻找第一个CTickMarketOrder
	///@return	只寻找满足条件的第一个CTickMarketOrder，如果找不到，返回NULL
	CTickMarketOrder *findFirstBySellMBLData( const CReadOnlyExchangeIDType& ExchangeID,  const CReadOnlyInstrumentIDType& InstrumentID,  const CReadOnlyPriceType& Tick);
	
	///寻找CTickMarketOrder
	///@return	找到的CTickMarketOrder，如果找不到，返回NULL
	CTickMarketOrder *findByPrice(const CReadOnlyExchangeIDType&  ExchangeID, const CReadOnlyInstrumentIDType&  InstrumentID, const CReadOnlyPriceType&  Tick, const CReadOnlyPriceType&  Price);



private:
	///完成自己的初始化工作
	///@param	maxUnit	最多的数据对象个数
	///@param	pAllocator	进行内存管理的分配器
	///@param	pIndexMap	索引名称和内存编号的对应表，在非重用时会写入，在重用时只读出
	///@param	reuse	是否重用
	///@param	reuseID	重用编号，非重用时应当置为-1
	///@param	readOnly	1表示该CFixMem是只读的，0表示可以读写
	void init(int maxUnit,IMemoryAllocator *pAllocator,map<string,int>* pIndexMap,bool reuse,int reuseID,int readOnly);
	
	///将一个CTickMarketOrder加入到该对象工厂，实质上是完成修改索引的工作
	///@param	pTickMarketOrder	要加入的TickMarketOrder
	///@param	bNoTransaction 是否不属于任何事务
	///@return	加入后该对象的位置，当pTransaction!=NULL时，总是返回NULL
	CTickMarketOrder* internalAdd(CWriteableTickMarketOrder *pTickMarketOrder, bool bNoTransaction);
	
	
	///刷新该CTickMarketOrder的键值
	///@param	pTickMarketOrder	被刷新的CTickMarketOrder
	///@param	pNewTickMarketOrder	新的值
	///@param	bNoTransaction 是否不属于任何事务
	CTickMarketOrder* internalUpdate(CTickMarketOrder *pTickMarketOrder, CWriteableTickMarketOrder *pNewTickMarketOrder, bool bNoTransaction);
	
	///删除一个CTickMarketOrder，同时删除其索引
	///@param	pTickMarketOrder	要删除的CTickMarketOrder
	///@param	bNoTransaction 是否不属于任何事务
	void internalRemove(CTickMarketOrder *pTickMarketOrder, bool bNoTransaction);
/*
	///检查某个CTickMarketOrder是否属于本对象工厂
	///@param	pTickMarketOrder	要检查的对象
	///@exception	CDesignError	如果不属于，则抛出此异常
	void checkAddress(CTickMarketOrder *pTickMarketOrder);
*/

	///基于ExchangeID+InstrumentID+Tick+Price的索引
	CAVLTree *pBuyMBLIndex;
	///基于ExchangeID+InstrumentID+Tick+Price的索引
	CAVLTree *pSellMBLIndex;
	
	///基于PriceHashKey的hash索引
	CHashIndex *pPriceHashIndex;
	
	///按照ExchangeID，InstrumentID，Tick检索时，存储的查询参数
	CExchangeIDType queryExchangeIDInSearchByBuyMBLData;
	CInstrumentIDType queryInstrumentIDInSearchByBuyMBLData;
	CPriceType queryTickInSearchByBuyMBLData;
	
	///按照ExchangeID，InstrumentID，Tick检索时，存储最后一次找到的索引节点
	CAVLNode *pLastFoundInSearchByBuyMBLData;
	///按照ExchangeID，InstrumentID，Tick检索时，存储的查询参数
	CExchangeIDType queryExchangeIDInSearchBySellMBLData;
	CInstrumentIDType queryInstrumentIDInSearchBySellMBLData;
	CPriceType queryTickInSearchBySellMBLData;
	
	///按照ExchangeID，InstrumentID，Tick检索时，存储最后一次找到的索引节点
	CAVLNode *pLastFoundInSearchBySellMBLData;
		
	///确认加入后触发
	///@param	pTickMarketOrder	已经加入的TickMarketOrder
	virtual void commitAdd(CTickMarketOrder *pTickMarketOrder);

	///确认更新后触发
	///@param	pTickMarketOrder	被刷新的CTickMarketOrder
	///@param	poldTickMarketOrder	原来的值
	virtual void commitUpdate(CTickMarketOrder *pTickMarketOrder, CWriteableTickMarketOrder *pOldTickMarketOrder);
	
	///确认删除后触发
	///@param	pTickMarketOrder	已经删除的CTickMarketOrder
	virtual void commitRemove(CWriteableTickMarketOrder *pTickMarketOrder);

	///所有相关的触发器
	vector<CTickMarketOrderActionTrigger *> *pActionTriggers;
	vector<CTickMarketOrderCommitTrigger *> *pCommitTriggers;
	
	///进行查询时，用于比较的对象
	CWriteableTickMarketOrder compareObject;
	CMDB* m_pMDB;
};

/////////////////////////////////////////////////////////////////////////
///CTradeFactory是一个成交的对象工厂。它包含了一批成交，
///同时又建立了如下索引，以方便查询：
///使用本对象工厂，可以完成对成交的增删改查操作。具体方法如下：
///	增加：叫用add，完成加入
///	删除：叫用remove就可以删除
///	查询：有如下方法可以查询：
///	修改：对于查到的结果，可以通过retrieve取出，修改后，通过update放回
///	事务管理：所有的增加、删除和修改都可以指定事务，在事务commit时，真正写入
///	在retrieve时，也可以指定事务，此时将取出经过update的新数据
/////////////////////////////////////////////////////////////////////////
class CTradeFactory :public CDatabaseDataFactory
{
public:
	///构造方法
	///@param	maxUnit	初始的数据对象个数
	///@param	pAllocator	进行内存管理的分配器
	///@param	pIndexMap	索引名称和内存编号的对应表，在非重用时会写入，在重用时只读出
	///@param	reuse	是否重用
	///@param	reuseID	重用编号，非重用时应当置为-1
	///@param	readOnly	1表示该CFixMem是只读的，0表示可以读写
	CTradeFactory(CMDB* pMDB,int maxUnit,IMemoryAllocator *pAllocator,map<string,int>* pIndexMap,bool reuse=false, int reuseID=-1, int readOnly=0);
	
	///构造方法
	///@param	pAllocator	进行内存管理的分配器
	///@param	pIndexMap	索引名称和内存编号的对应表，在非重用时会写入，在重用时只读出
	///@param	reuse	是否重用
	///@param	reuseID	重用编号，非重用时应当置为-1
	///@param	readOnly	1表示该CFixMem是只读的，0表示可以读写
	CTradeFactory(CMDB* pMDB,IMemoryAllocator *pAllocator,map<string,int>* pIndexMap,bool reuse=false, int reuseID = -1,int readOnly=0);
	
	///析构方法，将负责释放所占用的空间
	virtual ~CTradeFactory();

	///从CSV文件中读取所有的Trade
	///@param	filename	文件名
	///@return	1表示完成，0表示没有完成
	virtual int readCSV(char *filename);

	///从CSV文件中读取所有的Trade，文件名将根据配置文件中的定义TradeCSVFile
	///@return	1表示完成，0表示没有完成
	virtual int readCSVByPath(char *pPath);
	
	///向CSV文件中写出所有的Trade
	///@param	filename	文件名
	///@return	1表示完成，0表示没有完成
	virtual int writeCSV(char *filename);
	
	///向CSV文件中写出上一个版本的Trade
	///@param	filename	文件名
	///@return	1表示完成，0表示没有完成
	virtual int writeCSVPreVersion(char *filename);	

	///向CSV文件中写出所有的Trade，文件名将根据配置文件中的定义TradeCSVFile
	///@return	1表示完成，0表示没有完成
	virtual int writeCSVByPath(char *pPath);

	///将内容写到文件中,用于DEBUG
	virtual void dump(FILE *);

	///将内容写到文件中,用于DEBUG
	virtual void dump(char *filename);

	///清空所有的记录
	virtual void clearAll();

	friend class CTradeResource;

	///将一个CTrade加入到该对象工厂，实质上是完成修改索引的工作
	///@param	pTrade	要加入的Trade
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	///@return	加入后该对象的位置，当pTransaction!=NULL时，总是返回NULL
	CTrade* add(CWriteableTrade *pTrade, CTransaction *pTransaction=NULL);
	
	///刷新该CTrade的值
	///@param	pTrade	被刷新的CTrade
	///@param	pNewTrade	新的值
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	CTrade* update(CTrade *pTrade, CWriteableTrade *pNewTrade, CTransaction *pTransaction=NULL, bool bNoTransaction = true);
	
	///增加或者更新数据
	///@param	pTrade	  需要被刷新或者新增的CTrade,等于NULL表示是需要新增的
	///@param	pNewTrade	新的值
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	///@param	新增或者修改后的数据
	CTrade* addOrUpdate(CTrade *pTrade, CWriteableTrade *pNewTrade, CTransaction *pTransaction=NULL);
	
	///删除一个CTrade，同时删除其索引
	///@param	pTrade	要删除的CTrade
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	void remove(CTrade *pTrade, CTransaction *pTransaction=NULL, bool bNoTransaction=true);
	
	///获取某个CTrade
	///@param	pTrade	要读取的CTrade
	///@param	pTragetTrade	存放结果的CTrade
	void retrieve(CTrade *pTrade, CWriteableTrade *pTargetTrade);
	
	///获取第一个CTrade
	///@return	得到的第一个CTrade，如果没有，则返回NULL
	CTrade* getFirst();
	
	///获取下一个CTrade
	///@return	得到下一个CTrade，如果没有，则返回NULL
	CTrade* getNext();
	
	///加入一个操作触发器
	///@param	pTrigger	要加入的操作触发器
	///@return	1表示成功，0表示失败
	int addActionTrigger(CTradeActionTrigger *pTrigger);
	
	///删除一个操作触发器
	///@param	pTrigger	要删除的操作触发器
	///@return	1表示成功，0表示失败
	int removeActionTrigger(CTradeActionTrigger *pTrigger);

	///加入一个确认触发器
	///@param	pTrigger	要加入的确认触发器
	///@return	1表示成功，0表示失败
	int addCommitTrigger(CTradeCommitTrigger *pTrigger);
	
	///删除一个确认触发器
	///@param	pTrigger	要删除的确认触发器
	///@return	1表示成功，0表示失败
	int removeCommitTrigger(CTradeCommitTrigger *pTrigger);
	
	///检查是否字段为NULL
	///@param pFile 输出错误日志的文件
	void checkNull(FILE *pFile);
	
	///检查是否字段合法
	///@param pFile 输出错误日志的文件
	void checkValid(FILE *pFile);	


private:
	///完成自己的初始化工作
	///@param	maxUnit	最多的数据对象个数
	///@param	pAllocator	进行内存管理的分配器
	///@param	pIndexMap	索引名称和内存编号的对应表，在非重用时会写入，在重用时只读出
	///@param	reuse	是否重用
	///@param	reuseID	重用编号，非重用时应当置为-1
	///@param	readOnly	1表示该CFixMem是只读的，0表示可以读写
	void init(int maxUnit,IMemoryAllocator *pAllocator,map<string,int>* pIndexMap,bool reuse,int reuseID,int readOnly);
	
	///将一个CTrade加入到该对象工厂，实质上是完成修改索引的工作
	///@param	pTrade	要加入的Trade
	///@param	bNoTransaction 是否不属于任何事务
	///@return	加入后该对象的位置，当pTransaction!=NULL时，总是返回NULL
	CTrade* internalAdd(CWriteableTrade *pTrade, bool bNoTransaction);
	
	
	///刷新该CTrade的键值
	///@param	pTrade	被刷新的CTrade
	///@param	pNewTrade	新的值
	///@param	bNoTransaction 是否不属于任何事务
	CTrade* internalUpdate(CTrade *pTrade, CWriteableTrade *pNewTrade, bool bNoTransaction);
	
	///删除一个CTrade，同时删除其索引
	///@param	pTrade	要删除的CTrade
	///@param	bNoTransaction 是否不属于任何事务
	void internalRemove(CTrade *pTrade, bool bNoTransaction);
/*
	///检查某个CTrade是否属于本对象工厂
	///@param	pTrade	要检查的对象
	///@exception	CDesignError	如果不属于，则抛出此异常
	void checkAddress(CTrade *pTrade);
*/

	
	
		
	///确认加入后触发
	///@param	pTrade	已经加入的Trade
	virtual void commitAdd(CTrade *pTrade);

	///确认更新后触发
	///@param	pTrade	被刷新的CTrade
	///@param	poldTrade	原来的值
	virtual void commitUpdate(CTrade *pTrade, CWriteableTrade *pOldTrade);
	
	///确认删除后触发
	///@param	pTrade	已经删除的CTrade
	virtual void commitRemove(CWriteableTrade *pTrade);

	///所有相关的触发器
	vector<CTradeActionTrigger *> *pActionTriggers;
	vector<CTradeCommitTrigger *> *pCommitTriggers;
	
	///进行查询时，用于比较的对象
	CWriteableTrade compareObject;
	CMDB* m_pMDB;
};

/////////////////////////////////////////////////////////////////////////
///CKLineFactory是一个K线的对象工厂。它包含了一批K线，
///同时又建立了如下索引，以方便查询：
///使用本对象工厂，可以完成对K线的增删改查操作。具体方法如下：
///	增加：叫用add，完成加入
///	删除：叫用remove就可以删除
///	查询：有如下方法可以查询：
///	修改：对于查到的结果，可以通过retrieve取出，修改后，通过update放回
///	事务管理：所有的增加、删除和修改都可以指定事务，在事务commit时，真正写入
///	在retrieve时，也可以指定事务，此时将取出经过update的新数据
/////////////////////////////////////////////////////////////////////////
class CKLineFactory :public CDatabaseDataFactory
{
public:
	///构造方法
	///@param	maxUnit	初始的数据对象个数
	///@param	pAllocator	进行内存管理的分配器
	///@param	pIndexMap	索引名称和内存编号的对应表，在非重用时会写入，在重用时只读出
	///@param	reuse	是否重用
	///@param	reuseID	重用编号，非重用时应当置为-1
	///@param	readOnly	1表示该CFixMem是只读的，0表示可以读写
	CKLineFactory(CMDB* pMDB,int maxUnit,IMemoryAllocator *pAllocator,map<string,int>* pIndexMap,bool reuse=false, int reuseID=-1, int readOnly=0);
	
	///构造方法
	///@param	pAllocator	进行内存管理的分配器
	///@param	pIndexMap	索引名称和内存编号的对应表，在非重用时会写入，在重用时只读出
	///@param	reuse	是否重用
	///@param	reuseID	重用编号，非重用时应当置为-1
	///@param	readOnly	1表示该CFixMem是只读的，0表示可以读写
	CKLineFactory(CMDB* pMDB,IMemoryAllocator *pAllocator,map<string,int>* pIndexMap,bool reuse=false, int reuseID = -1,int readOnly=0);
	
	///析构方法，将负责释放所占用的空间
	virtual ~CKLineFactory();

	///从CSV文件中读取所有的KLine
	///@param	filename	文件名
	///@return	1表示完成，0表示没有完成
	virtual int readCSV(char *filename);

	///从CSV文件中读取所有的KLine，文件名将根据配置文件中的定义KLineCSVFile
	///@return	1表示完成，0表示没有完成
	virtual int readCSVByPath(char *pPath);
	
	///向CSV文件中写出所有的KLine
	///@param	filename	文件名
	///@return	1表示完成，0表示没有完成
	virtual int writeCSV(char *filename);
	
	///向CSV文件中写出上一个版本的KLine
	///@param	filename	文件名
	///@return	1表示完成，0表示没有完成
	virtual int writeCSVPreVersion(char *filename);	

	///向CSV文件中写出所有的KLine，文件名将根据配置文件中的定义KLineCSVFile
	///@return	1表示完成，0表示没有完成
	virtual int writeCSVByPath(char *pPath);

	///将内容写到文件中,用于DEBUG
	virtual void dump(FILE *);

	///将内容写到文件中,用于DEBUG
	virtual void dump(char *filename);

	///清空所有的记录
	virtual void clearAll();

	friend class CKLineResource;

	///将一个CKLine加入到该对象工厂，实质上是完成修改索引的工作
	///@param	pKLine	要加入的KLine
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	///@return	加入后该对象的位置，当pTransaction!=NULL时，总是返回NULL
	CKLine* add(CWriteableKLine *pKLine, CTransaction *pTransaction=NULL);
	
	///刷新该CKLine的值
	///@param	pKLine	被刷新的CKLine
	///@param	pNewKLine	新的值
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	CKLine* update(CKLine *pKLine, CWriteableKLine *pNewKLine, CTransaction *pTransaction=NULL, bool bNoTransaction = true);
	
	///增加或者更新数据
	///@param	pKLine	  需要被刷新或者新增的CKLine,等于NULL表示是需要新增的
	///@param	pNewKLine	新的值
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	///@param	新增或者修改后的数据
	CKLine* addOrUpdate(CKLine *pKLine, CWriteableKLine *pNewKLine, CTransaction *pTransaction=NULL);
	
	///删除一个CKLine，同时删除其索引
	///@param	pKLine	要删除的CKLine
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	void remove(CKLine *pKLine, CTransaction *pTransaction=NULL, bool bNoTransaction=true);
	
	///获取某个CKLine
	///@param	pKLine	要读取的CKLine
	///@param	pTragetKLine	存放结果的CKLine
	void retrieve(CKLine *pKLine, CWriteableKLine *pTargetKLine);
	
	///获取第一个CKLine
	///@return	得到的第一个CKLine，如果没有，则返回NULL
	CKLine* getFirst();
	
	///获取下一个CKLine
	///@return	得到下一个CKLine，如果没有，则返回NULL
	CKLine* getNext();
	
	///加入一个操作触发器
	///@param	pTrigger	要加入的操作触发器
	///@return	1表示成功，0表示失败
	int addActionTrigger(CKLineActionTrigger *pTrigger);
	
	///删除一个操作触发器
	///@param	pTrigger	要删除的操作触发器
	///@return	1表示成功，0表示失败
	int removeActionTrigger(CKLineActionTrigger *pTrigger);

	///加入一个确认触发器
	///@param	pTrigger	要加入的确认触发器
	///@return	1表示成功，0表示失败
	int addCommitTrigger(CKLineCommitTrigger *pTrigger);
	
	///删除一个确认触发器
	///@param	pTrigger	要删除的确认触发器
	///@return	1表示成功，0表示失败
	int removeCommitTrigger(CKLineCommitTrigger *pTrigger);
	
	///检查是否字段为NULL
	///@param pFile 输出错误日志的文件
	void checkNull(FILE *pFile);
	
	///检查是否字段合法
	///@param pFile 输出错误日志的文件
	void checkValid(FILE *pFile);	


private:
	///完成自己的初始化工作
	///@param	maxUnit	最多的数据对象个数
	///@param	pAllocator	进行内存管理的分配器
	///@param	pIndexMap	索引名称和内存编号的对应表，在非重用时会写入，在重用时只读出
	///@param	reuse	是否重用
	///@param	reuseID	重用编号，非重用时应当置为-1
	///@param	readOnly	1表示该CFixMem是只读的，0表示可以读写
	void init(int maxUnit,IMemoryAllocator *pAllocator,map<string,int>* pIndexMap,bool reuse,int reuseID,int readOnly);
	
	///将一个CKLine加入到该对象工厂，实质上是完成修改索引的工作
	///@param	pKLine	要加入的KLine
	///@param	bNoTransaction 是否不属于任何事务
	///@return	加入后该对象的位置，当pTransaction!=NULL时，总是返回NULL
	CKLine* internalAdd(CWriteableKLine *pKLine, bool bNoTransaction);
	
	
	///刷新该CKLine的键值
	///@param	pKLine	被刷新的CKLine
	///@param	pNewKLine	新的值
	///@param	bNoTransaction 是否不属于任何事务
	CKLine* internalUpdate(CKLine *pKLine, CWriteableKLine *pNewKLine, bool bNoTransaction);
	
	///删除一个CKLine，同时删除其索引
	///@param	pKLine	要删除的CKLine
	///@param	bNoTransaction 是否不属于任何事务
	void internalRemove(CKLine *pKLine, bool bNoTransaction);
/*
	///检查某个CKLine是否属于本对象工厂
	///@param	pKLine	要检查的对象
	///@exception	CDesignError	如果不属于，则抛出此异常
	void checkAddress(CKLine *pKLine);
*/

	
	
		
	///确认加入后触发
	///@param	pKLine	已经加入的KLine
	virtual void commitAdd(CKLine *pKLine);

	///确认更新后触发
	///@param	pKLine	被刷新的CKLine
	///@param	poldKLine	原来的值
	virtual void commitUpdate(CKLine *pKLine, CWriteableKLine *pOldKLine);
	
	///确认删除后触发
	///@param	pKLine	已经删除的CKLine
	virtual void commitRemove(CWriteableKLine *pKLine);

	///所有相关的触发器
	vector<CKLineActionTrigger *> *pActionTriggers;
	vector<CKLineCommitTrigger *> *pCommitTriggers;
	
	///进行查询时，用于比较的对象
	CWriteableKLine compareObject;
	CMDB* m_pMDB;
};

/////////////////////////////////////////////////////////////////////////
///CPublishKLineFactory是一个PublishK线的对象工厂。它包含了一批PublishK线，
///同时又建立了如下索引，以方便查询：
///	BarInsertTimeIndex
///使用本对象工厂，可以完成对PublishK线的增删改查操作。具体方法如下：
///	增加：叫用add，完成加入
///	删除：叫用remove就可以删除
///	查询：有如下方法可以查询：
///		使用findByInstrumentBar，唯一找到对象
///		使用startFindByBar，findNextByBar完成查询操作
///		使用startFindByBarInsertTime，findNextByBarInsertTime完成查询操作
///	修改：对于查到的结果，可以通过retrieve取出，修改后，通过update放回
///	事务管理：所有的增加、删除和修改都可以指定事务，在事务commit时，真正写入
///	在retrieve时，也可以指定事务，此时将取出经过update的新数据
/////////////////////////////////////////////////////////////////////////
class CPublishKLineFactory :public CDatabaseDataFactory
{
public:
	///构造方法
	///@param	maxUnit	初始的数据对象个数
	///@param	pAllocator	进行内存管理的分配器
	///@param	pIndexMap	索引名称和内存编号的对应表，在非重用时会写入，在重用时只读出
	///@param	reuse	是否重用
	///@param	reuseID	重用编号，非重用时应当置为-1
	///@param	readOnly	1表示该CFixMem是只读的，0表示可以读写
	CPublishKLineFactory(CMDB* pMDB,int maxUnit,IMemoryAllocator *pAllocator,map<string,int>* pIndexMap,bool reuse=false, int reuseID=-1, int readOnly=0);
	
	///构造方法
	///@param	pAllocator	进行内存管理的分配器
	///@param	pIndexMap	索引名称和内存编号的对应表，在非重用时会写入，在重用时只读出
	///@param	reuse	是否重用
	///@param	reuseID	重用编号，非重用时应当置为-1
	///@param	readOnly	1表示该CFixMem是只读的，0表示可以读写
	CPublishKLineFactory(CMDB* pMDB,IMemoryAllocator *pAllocator,map<string,int>* pIndexMap,bool reuse=false, int reuseID = -1,int readOnly=0);
	
	///析构方法，将负责释放所占用的空间
	virtual ~CPublishKLineFactory();

	///从CSV文件中读取所有的PublishKLine
	///@param	filename	文件名
	///@return	1表示完成，0表示没有完成
	virtual int readCSV(char *filename);

	///从CSV文件中读取所有的PublishKLine，文件名将根据配置文件中的定义PublishKLineCSVFile
	///@return	1表示完成，0表示没有完成
	virtual int readCSVByPath(char *pPath);
	
	///向CSV文件中写出所有的PublishKLine
	///@param	filename	文件名
	///@return	1表示完成，0表示没有完成
	virtual int writeCSV(char *filename);
	
	///向CSV文件中写出上一个版本的PublishKLine
	///@param	filename	文件名
	///@return	1表示完成，0表示没有完成
	virtual int writeCSVPreVersion(char *filename);	

	///向CSV文件中写出所有的PublishKLine，文件名将根据配置文件中的定义PublishKLineCSVFile
	///@return	1表示完成，0表示没有完成
	virtual int writeCSVByPath(char *pPath);

	///将内容写到文件中,用于DEBUG
	virtual void dump(FILE *);

	///将内容写到文件中,用于DEBUG
	virtual void dump(char *filename);

	///清空所有的记录
	virtual void clearAll();

	friend class CPublishKLineResource;

	///将一个CPublishKLine加入到该对象工厂，实质上是完成修改索引的工作
	///@param	pPublishKLine	要加入的PublishKLine
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	///@return	加入后该对象的位置，当pTransaction!=NULL时，总是返回NULL
	CPublishKLine* add(CWriteablePublishKLine *pPublishKLine, CTransaction *pTransaction=NULL);
	
	///刷新该CPublishKLine的值
	///@param	pPublishKLine	被刷新的CPublishKLine
	///@param	pNewPublishKLine	新的值
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	CPublishKLine* update(CPublishKLine *pPublishKLine, CWriteablePublishKLine *pNewPublishKLine, CTransaction *pTransaction=NULL, bool bNoTransaction = true);
	
	///增加或者更新数据
	///@param	pPublishKLine	  需要被刷新或者新增的CPublishKLine,等于NULL表示是需要新增的
	///@param	pNewPublishKLine	新的值
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	///@param	新增或者修改后的数据
	CPublishKLine* addOrUpdate(CPublishKLine *pPublishKLine, CWriteablePublishKLine *pNewPublishKLine, CTransaction *pTransaction=NULL);
	
	///删除一个CPublishKLine，同时删除其索引
	///@param	pPublishKLine	要删除的CPublishKLine
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	void remove(CPublishKLine *pPublishKLine, CTransaction *pTransaction=NULL, bool bNoTransaction=true);
	
	///获取某个CPublishKLine
	///@param	pPublishKLine	要读取的CPublishKLine
	///@param	pTragetPublishKLine	存放结果的CPublishKLine
	void retrieve(CPublishKLine *pPublishKLine, CWriteablePublishKLine *pTargetPublishKLine);
	
	///获取第一个CPublishKLine
	///@return	得到的第一个CPublishKLine，如果没有，则返回NULL
	CPublishKLine* getFirst();
	
	///获取下一个CPublishKLine
	///@return	得到下一个CPublishKLine，如果没有，则返回NULL
	CPublishKLine* getNext();
	
	///加入一个操作触发器
	///@param	pTrigger	要加入的操作触发器
	///@return	1表示成功，0表示失败
	int addActionTrigger(CPublishKLineActionTrigger *pTrigger);
	
	///删除一个操作触发器
	///@param	pTrigger	要删除的操作触发器
	///@return	1表示成功，0表示失败
	int removeActionTrigger(CPublishKLineActionTrigger *pTrigger);

	///加入一个确认触发器
	///@param	pTrigger	要加入的确认触发器
	///@return	1表示成功，0表示失败
	int addCommitTrigger(CPublishKLineCommitTrigger *pTrigger);
	
	///删除一个确认触发器
	///@param	pTrigger	要删除的确认触发器
	///@return	1表示成功，0表示失败
	int removeCommitTrigger(CPublishKLineCommitTrigger *pTrigger);
	
	///检查是否字段为NULL
	///@param pFile 输出错误日志的文件
	void checkNull(FILE *pFile);
	
	///检查是否字段合法
	///@param pFile 输出错误日志的文件
	void checkValid(FILE *pFile);	

	///寻找CPublishKLine
	///@return	找到的CPublishKLine，如果找不到，返回NULL
	CPublishKLine *findByInstrumentBar(const CReadOnlyExchangeIDType&  ExchangeID, const CReadOnlyInstrumentIDType&  InstrumentID, const CReadOnlyBarType&  Bar, const CReadOnlyMilliSecondsType&  InsertTime);

	///根据主码寻找
	///@return	找到的CPublishKLine，如果找不到，返回NULL
	CPublishKLine *findByPK(const CReadOnlyExchangeIDType& ExchangeID, const CReadOnlyInstrumentIDType& InstrumentID, const CReadOnlyBarType& Bar, const CReadOnlyMilliSecondsType& InsertTime);
		
	///根据主键删除CPublishKLine
	bool removeByPK(const CReadOnlyExchangeIDType& ExchangeID, const CReadOnlyInstrumentIDType& InstrumentID, const CReadOnlyBarType& Bar, const CReadOnlyMilliSecondsType& InsertTime, CTransaction *pTransaction=NULL);
	
	///判断字段是否是是主键
	bool isPK(const char* pName);
	
	///根据主键寻找CPublishKLine
	///@param	pPublishKLine	要找的值
	///@return	找到的CPublishKLine，如果找不到，返回NULL
	CPublishKLine *findByPK(CPublishKLineField *pPublishKLine);
	
	///根据主键寻找CPublishKLine
	///@param	pPublishKLine	要找的值
	///@return	找到的CPublishKLine，如果找不到，返回NULL
	CPublishKLine *findByPK(CPublishKLine *pPublishKLine);

	///根据主键获取CPublishKLine
	///@param	pPublishKLine	要获取的值，同时用于存放返回的结果
	///@return	找到的CPublishKLine，如果找不到，返回NULL
	bool retrieveByPK(CWriteablePublishKLine *pPublishKLine);

	///根据主键增加新的CPublishKLine
	///@param	pPublishKLine	新的值
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	///@return	返回插入的CPublishKLine，如果插入失败，则返回NULL
	CPublishKLine *addByPK(CWriteablePublishKLine *pPublishKLine, CTransaction *pTransaction=NULL);
	
	///根据主键增加或更新新的CPublishKLine
	///@param	pPublishKLine	新的值
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	///@return	返回插入的CPublishKLine，如果插入失败，则返回NULL
	CPublishKLine *addOrUpdateByPK(CWriteablePublishKLine *pPublishKLine, CTransaction *pTransaction=NULL);

	///根据主键更新CPublishKLine的值
	///@param	pPublishKLine	新的值
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	///@return	true表示成功，false表示失败
	bool updateByPK(CWriteablePublishKLine *pPublishKLine, CTransaction *pTransaction=NULL);

	///根据主键删除CPublishKLine
	///@param	pPublishKLine	要删除的值
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	///@return	true表示成功，false表示失败
	bool removeByPK(CPublishKLine *pPublishKLine, CTransaction *pTransaction=NULL);
	
	///根据主键删除CPublishKLine
	///@param	pPublishKLine	要删除的值
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	///@return	true表示成功，false表示失败
	bool removeByPK(CPublishKLineField *pPublishKLine, CTransaction *pTransaction=NULL);	

	friend class CPublishKLineIteratorByBar;

	///寻找下一个符合条件的CPublishKLine，必须在startFindByBar以后叫用
	///@return	下一个满足条件CPublishKLine，如果已经没有一个满足要求了，则返回NULL
	CPublishKLine *findNextByBar();
	
	///寻找第一个CPublishKLine
	///@return	只寻找满足条件的第一个CPublishKLine，如果找不到，返回NULL
	CPublishKLine *findFirstByBar( const CReadOnlyExchangeIDType& ExchangeID,  const CReadOnlyInstrumentIDType& InstrumentID,  const CReadOnlyBarType& Bar);
	
	friend class CPublishKLineIteratorByBarInsertTime;

	///寻找下一个符合条件的CPublishKLine，必须在startFindByBarInsertTime以后叫用
	///@return	下一个满足条件CPublishKLine，如果已经没有一个满足要求了，则返回NULL
	CPublishKLine *findNextByBarInsertTime();
	
	///寻找第一个CPublishKLine
	///@return	只寻找满足条件的第一个CPublishKLine，如果找不到，返回NULL
	CPublishKLine *findFirstByBarInsertTime( const CReadOnlyExchangeIDType& ExchangeID,  const CReadOnlyInstrumentIDType& InstrumentID,  const CReadOnlyBarType& Bar,  const CReadOnlyMilliSecondsType& InsertTime);
	

private:
	///完成自己的初始化工作
	///@param	maxUnit	最多的数据对象个数
	///@param	pAllocator	进行内存管理的分配器
	///@param	pIndexMap	索引名称和内存编号的对应表，在非重用时会写入，在重用时只读出
	///@param	reuse	是否重用
	///@param	reuseID	重用编号，非重用时应当置为-1
	///@param	readOnly	1表示该CFixMem是只读的，0表示可以读写
	void init(int maxUnit,IMemoryAllocator *pAllocator,map<string,int>* pIndexMap,bool reuse,int reuseID,int readOnly);
	
	///将一个CPublishKLine加入到该对象工厂，实质上是完成修改索引的工作
	///@param	pPublishKLine	要加入的PublishKLine
	///@param	bNoTransaction 是否不属于任何事务
	///@return	加入后该对象的位置，当pTransaction!=NULL时，总是返回NULL
	CPublishKLine* internalAdd(CWriteablePublishKLine *pPublishKLine, bool bNoTransaction);
	
	
	///刷新该CPublishKLine的键值
	///@param	pPublishKLine	被刷新的CPublishKLine
	///@param	pNewPublishKLine	新的值
	///@param	bNoTransaction 是否不属于任何事务
	CPublishKLine* internalUpdate(CPublishKLine *pPublishKLine, CWriteablePublishKLine *pNewPublishKLine, bool bNoTransaction);
	
	///删除一个CPublishKLine，同时删除其索引
	///@param	pPublishKLine	要删除的CPublishKLine
	///@param	bNoTransaction 是否不属于任何事务
	void internalRemove(CPublishKLine *pPublishKLine, bool bNoTransaction);
/*
	///检查某个CPublishKLine是否属于本对象工厂
	///@param	pPublishKLine	要检查的对象
	///@exception	CDesignError	如果不属于，则抛出此异常
	void checkAddress(CPublishKLine *pPublishKLine);
*/

	///基于ExchangeID+InstrumentID+Bar+InsertTime的索引
	CAVLTree *pBarInsertTimeIndex;
	
	
	///按照ExchangeID，InstrumentID，Bar检索时，存储的查询参数
	CExchangeIDType queryExchangeIDInSearchByBar;
	CInstrumentIDType queryInstrumentIDInSearchByBar;
	CBarType queryBarInSearchByBar;
	
	///按照ExchangeID，InstrumentID，Bar检索时，存储最后一次找到的索引节点
	CAVLNode *pLastFoundInSearchByBar;
	///按照ExchangeID，InstrumentID，Bar，InsertTime检索时，存储的查询参数
	CExchangeIDType queryExchangeIDInSearchByBarInsertTime;
	CInstrumentIDType queryInstrumentIDInSearchByBarInsertTime;
	CBarType queryBarInSearchByBarInsertTime;
	CMilliSecondsType queryInsertTimeInSearchByBarInsertTime;
	
	///按照ExchangeID，InstrumentID，Bar，InsertTime检索时，存储最后一次找到的索引节点
	CAVLNode *pLastFoundInSearchByBarInsertTime;
		
	///确认加入后触发
	///@param	pPublishKLine	已经加入的PublishKLine
	virtual void commitAdd(CPublishKLine *pPublishKLine);

	///确认更新后触发
	///@param	pPublishKLine	被刷新的CPublishKLine
	///@param	poldPublishKLine	原来的值
	virtual void commitUpdate(CPublishKLine *pPublishKLine, CWriteablePublishKLine *pOldPublishKLine);
	
	///确认删除后触发
	///@param	pPublishKLine	已经删除的CPublishKLine
	virtual void commitRemove(CWriteablePublishKLine *pPublishKLine);

	///所有相关的触发器
	vector<CPublishKLineActionTrigger *> *pActionTriggers;
	vector<CPublishKLineCommitTrigger *> *pCommitTriggers;
	
	///进行查询时，用于比较的对象
	CWriteablePublishKLine compareObject;
	CMDB* m_pMDB;
};

/////////////////////////////////////////////////////////////////////////
///CLastKLineFactory是一个最新K线的对象工厂。它包含了一批最新K线，
///同时又建立了如下索引，以方便查询：
///使用本对象工厂，可以完成对最新K线的增删改查操作。具体方法如下：
///	增加：叫用add，完成加入
///	删除：叫用remove就可以删除
///	查询：有如下方法可以查询：
///		使用findByInstrumentBar，唯一找到对象
///	修改：对于查到的结果，可以通过retrieve取出，修改后，通过update放回
///	事务管理：所有的增加、删除和修改都可以指定事务，在事务commit时，真正写入
///	在retrieve时，也可以指定事务，此时将取出经过update的新数据
/////////////////////////////////////////////////////////////////////////
class CLastKLineFactory :public CDatabaseDataFactory
{
public:
	///构造方法
	///@param	maxUnit	初始的数据对象个数
	///@param	pAllocator	进行内存管理的分配器
	///@param	pIndexMap	索引名称和内存编号的对应表，在非重用时会写入，在重用时只读出
	///@param	reuse	是否重用
	///@param	reuseID	重用编号，非重用时应当置为-1
	///@param	readOnly	1表示该CFixMem是只读的，0表示可以读写
	CLastKLineFactory(CMDB* pMDB,int maxUnit,IMemoryAllocator *pAllocator,map<string,int>* pIndexMap,bool reuse=false, int reuseID=-1, int readOnly=0);
	
	///构造方法
	///@param	pAllocator	进行内存管理的分配器
	///@param	pIndexMap	索引名称和内存编号的对应表，在非重用时会写入，在重用时只读出
	///@param	reuse	是否重用
	///@param	reuseID	重用编号，非重用时应当置为-1
	///@param	readOnly	1表示该CFixMem是只读的，0表示可以读写
	CLastKLineFactory(CMDB* pMDB,IMemoryAllocator *pAllocator,map<string,int>* pIndexMap,bool reuse=false, int reuseID = -1,int readOnly=0);
	
	///析构方法，将负责释放所占用的空间
	virtual ~CLastKLineFactory();

	///从CSV文件中读取所有的LastKLine
	///@param	filename	文件名
	///@return	1表示完成，0表示没有完成
	virtual int readCSV(char *filename);

	///从CSV文件中读取所有的LastKLine，文件名将根据配置文件中的定义LastKLineCSVFile
	///@return	1表示完成，0表示没有完成
	virtual int readCSVByPath(char *pPath);
	
	///向CSV文件中写出所有的LastKLine
	///@param	filename	文件名
	///@return	1表示完成，0表示没有完成
	virtual int writeCSV(char *filename);
	
	///向CSV文件中写出上一个版本的LastKLine
	///@param	filename	文件名
	///@return	1表示完成，0表示没有完成
	virtual int writeCSVPreVersion(char *filename);	

	///向CSV文件中写出所有的LastKLine，文件名将根据配置文件中的定义LastKLineCSVFile
	///@return	1表示完成，0表示没有完成
	virtual int writeCSVByPath(char *pPath);

	///将内容写到文件中,用于DEBUG
	virtual void dump(FILE *);

	///将内容写到文件中,用于DEBUG
	virtual void dump(char *filename);

	///清空所有的记录
	virtual void clearAll();

	friend class CLastKLineResource;

	///将一个CLastKLine加入到该对象工厂，实质上是完成修改索引的工作
	///@param	pLastKLine	要加入的LastKLine
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	///@return	加入后该对象的位置，当pTransaction!=NULL时，总是返回NULL
	CLastKLine* add(CWriteableLastKLine *pLastKLine, CTransaction *pTransaction=NULL);
	
	///刷新该CLastKLine的值
	///@param	pLastKLine	被刷新的CLastKLine
	///@param	pNewLastKLine	新的值
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	CLastKLine* update(CLastKLine *pLastKLine, CWriteableLastKLine *pNewLastKLine, CTransaction *pTransaction=NULL, bool bNoTransaction = true);
	
	///增加或者更新数据
	///@param	pLastKLine	  需要被刷新或者新增的CLastKLine,等于NULL表示是需要新增的
	///@param	pNewLastKLine	新的值
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	///@param	新增或者修改后的数据
	CLastKLine* addOrUpdate(CLastKLine *pLastKLine, CWriteableLastKLine *pNewLastKLine, CTransaction *pTransaction=NULL);
	
	///删除一个CLastKLine，同时删除其索引
	///@param	pLastKLine	要删除的CLastKLine
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	void remove(CLastKLine *pLastKLine, CTransaction *pTransaction=NULL, bool bNoTransaction=true);
	
	///获取某个CLastKLine
	///@param	pLastKLine	要读取的CLastKLine
	///@param	pTragetLastKLine	存放结果的CLastKLine
	void retrieve(CLastKLine *pLastKLine, CWriteableLastKLine *pTargetLastKLine);
	
	///获取第一个CLastKLine
	///@return	得到的第一个CLastKLine，如果没有，则返回NULL
	CLastKLine* getFirst();
	
	///获取下一个CLastKLine
	///@return	得到下一个CLastKLine，如果没有，则返回NULL
	CLastKLine* getNext();
	
	///加入一个操作触发器
	///@param	pTrigger	要加入的操作触发器
	///@return	1表示成功，0表示失败
	int addActionTrigger(CLastKLineActionTrigger *pTrigger);
	
	///删除一个操作触发器
	///@param	pTrigger	要删除的操作触发器
	///@return	1表示成功，0表示失败
	int removeActionTrigger(CLastKLineActionTrigger *pTrigger);

	///加入一个确认触发器
	///@param	pTrigger	要加入的确认触发器
	///@return	1表示成功，0表示失败
	int addCommitTrigger(CLastKLineCommitTrigger *pTrigger);
	
	///删除一个确认触发器
	///@param	pTrigger	要删除的确认触发器
	///@return	1表示成功，0表示失败
	int removeCommitTrigger(CLastKLineCommitTrigger *pTrigger);
	
	///检查是否字段为NULL
	///@param pFile 输出错误日志的文件
	void checkNull(FILE *pFile);
	
	///检查是否字段合法
	///@param pFile 输出错误日志的文件
	void checkValid(FILE *pFile);	

	///寻找CLastKLine
	///@return	找到的CLastKLine，如果找不到，返回NULL
	CLastKLine *findByInstrumentBar(const CReadOnlyExchangeIDType&  ExchangeID, const CReadOnlyInstrumentIDType&  InstrumentID, const CReadOnlyBarType&  Bar);

	///根据主码寻找
	///@return	找到的CLastKLine，如果找不到，返回NULL
	CLastKLine *findByPK(const CReadOnlyExchangeIDType& ExchangeID, const CReadOnlyInstrumentIDType& InstrumentID, const CReadOnlyBarType& Bar);
		
	///根据主键删除CLastKLine
	bool removeByPK(const CReadOnlyExchangeIDType& ExchangeID, const CReadOnlyInstrumentIDType& InstrumentID, const CReadOnlyBarType& Bar, CTransaction *pTransaction=NULL);
	
	///判断字段是否是是主键
	bool isPK(const char* pName);
	
	///根据主键寻找CLastKLine
	///@param	pLastKLine	要找的值
	///@return	找到的CLastKLine，如果找不到，返回NULL
	CLastKLine *findByPK(CLastKLineField *pLastKLine);
	
	///根据主键寻找CLastKLine
	///@param	pLastKLine	要找的值
	///@return	找到的CLastKLine，如果找不到，返回NULL
	CLastKLine *findByPK(CLastKLine *pLastKLine);

	///根据主键获取CLastKLine
	///@param	pLastKLine	要获取的值，同时用于存放返回的结果
	///@return	找到的CLastKLine，如果找不到，返回NULL
	bool retrieveByPK(CWriteableLastKLine *pLastKLine);

	///根据主键增加新的CLastKLine
	///@param	pLastKLine	新的值
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	///@return	返回插入的CLastKLine，如果插入失败，则返回NULL
	CLastKLine *addByPK(CWriteableLastKLine *pLastKLine, CTransaction *pTransaction=NULL);
	
	///根据主键增加或更新新的CLastKLine
	///@param	pLastKLine	新的值
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	///@return	返回插入的CLastKLine，如果插入失败，则返回NULL
	CLastKLine *addOrUpdateByPK(CWriteableLastKLine *pLastKLine, CTransaction *pTransaction=NULL);

	///根据主键更新CLastKLine的值
	///@param	pLastKLine	新的值
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	///@return	true表示成功，false表示失败
	bool updateByPK(CWriteableLastKLine *pLastKLine, CTransaction *pTransaction=NULL);

	///根据主键删除CLastKLine
	///@param	pLastKLine	要删除的值
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	///@return	true表示成功，false表示失败
	bool removeByPK(CLastKLine *pLastKLine, CTransaction *pTransaction=NULL);
	
	///根据主键删除CLastKLine
	///@param	pLastKLine	要删除的值
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	///@return	true表示成功，false表示失败
	bool removeByPK(CLastKLineField *pLastKLine, CTransaction *pTransaction=NULL);	


private:
	///完成自己的初始化工作
	///@param	maxUnit	最多的数据对象个数
	///@param	pAllocator	进行内存管理的分配器
	///@param	pIndexMap	索引名称和内存编号的对应表，在非重用时会写入，在重用时只读出
	///@param	reuse	是否重用
	///@param	reuseID	重用编号，非重用时应当置为-1
	///@param	readOnly	1表示该CFixMem是只读的，0表示可以读写
	void init(int maxUnit,IMemoryAllocator *pAllocator,map<string,int>* pIndexMap,bool reuse,int reuseID,int readOnly);
	
	///将一个CLastKLine加入到该对象工厂，实质上是完成修改索引的工作
	///@param	pLastKLine	要加入的LastKLine
	///@param	bNoTransaction 是否不属于任何事务
	///@return	加入后该对象的位置，当pTransaction!=NULL时，总是返回NULL
	CLastKLine* internalAdd(CWriteableLastKLine *pLastKLine, bool bNoTransaction);
	
	
	///刷新该CLastKLine的键值
	///@param	pLastKLine	被刷新的CLastKLine
	///@param	pNewLastKLine	新的值
	///@param	bNoTransaction 是否不属于任何事务
	CLastKLine* internalUpdate(CLastKLine *pLastKLine, CWriteableLastKLine *pNewLastKLine, bool bNoTransaction);
	
	///删除一个CLastKLine，同时删除其索引
	///@param	pLastKLine	要删除的CLastKLine
	///@param	bNoTransaction 是否不属于任何事务
	void internalRemove(CLastKLine *pLastKLine, bool bNoTransaction);
/*
	///检查某个CLastKLine是否属于本对象工厂
	///@param	pLastKLine	要检查的对象
	///@exception	CDesignError	如果不属于，则抛出此异常
	void checkAddress(CLastKLine *pLastKLine);
*/

	
	///基于InstrumentBarHashKey的hash索引
	CHashIndex *pInstrumentBarHashIndex;
	
		
	///确认加入后触发
	///@param	pLastKLine	已经加入的LastKLine
	virtual void commitAdd(CLastKLine *pLastKLine);

	///确认更新后触发
	///@param	pLastKLine	被刷新的CLastKLine
	///@param	poldLastKLine	原来的值
	virtual void commitUpdate(CLastKLine *pLastKLine, CWriteableLastKLine *pOldLastKLine);
	
	///确认删除后触发
	///@param	pLastKLine	已经删除的CLastKLine
	virtual void commitRemove(CWriteableLastKLine *pLastKLine);

	///所有相关的触发器
	vector<CLastKLineActionTrigger *> *pActionTriggers;
	vector<CLastKLineCommitTrigger *> *pCommitTriggers;
	
	///进行查询时，用于比较的对象
	CWriteableLastKLine compareObject;
	CMDB* m_pMDB;
};

/////////////////////////////////////////////////////////////////////////
///CAccountDetailFactory是一个资金明细的对象工厂。它包含了一批资金明细，
///同时又建立了如下索引，以方便查询：
///使用本对象工厂，可以完成对资金明细的增删改查操作。具体方法如下：
///	增加：叫用add，完成加入
///	删除：叫用remove就可以删除
///	查询：有如下方法可以查询：
///	修改：对于查到的结果，可以通过retrieve取出，修改后，通过update放回
///	事务管理：所有的增加、删除和修改都可以指定事务，在事务commit时，真正写入
///	在retrieve时，也可以指定事务，此时将取出经过update的新数据
/////////////////////////////////////////////////////////////////////////
class CAccountDetailFactory :public CDatabaseDataFactory
{
public:
	///构造方法
	///@param	maxUnit	初始的数据对象个数
	///@param	pAllocator	进行内存管理的分配器
	///@param	pIndexMap	索引名称和内存编号的对应表，在非重用时会写入，在重用时只读出
	///@param	reuse	是否重用
	///@param	reuseID	重用编号，非重用时应当置为-1
	///@param	readOnly	1表示该CFixMem是只读的，0表示可以读写
	CAccountDetailFactory(CMDB* pMDB,int maxUnit,IMemoryAllocator *pAllocator,map<string,int>* pIndexMap,bool reuse=false, int reuseID=-1, int readOnly=0);
	
	///构造方法
	///@param	pAllocator	进行内存管理的分配器
	///@param	pIndexMap	索引名称和内存编号的对应表，在非重用时会写入，在重用时只读出
	///@param	reuse	是否重用
	///@param	reuseID	重用编号，非重用时应当置为-1
	///@param	readOnly	1表示该CFixMem是只读的，0表示可以读写
	CAccountDetailFactory(CMDB* pMDB,IMemoryAllocator *pAllocator,map<string,int>* pIndexMap,bool reuse=false, int reuseID = -1,int readOnly=0);
	
	///析构方法，将负责释放所占用的空间
	virtual ~CAccountDetailFactory();

	///从CSV文件中读取所有的AccountDetail
	///@param	filename	文件名
	///@return	1表示完成，0表示没有完成
	virtual int readCSV(char *filename);

	///从CSV文件中读取所有的AccountDetail，文件名将根据配置文件中的定义AccountDetailCSVFile
	///@return	1表示完成，0表示没有完成
	virtual int readCSVByPath(char *pPath);
	
	///向CSV文件中写出所有的AccountDetail
	///@param	filename	文件名
	///@return	1表示完成，0表示没有完成
	virtual int writeCSV(char *filename);
	
	///向CSV文件中写出上一个版本的AccountDetail
	///@param	filename	文件名
	///@return	1表示完成，0表示没有完成
	virtual int writeCSVPreVersion(char *filename);	

	///向CSV文件中写出所有的AccountDetail，文件名将根据配置文件中的定义AccountDetailCSVFile
	///@return	1表示完成，0表示没有完成
	virtual int writeCSVByPath(char *pPath);

	///将内容写到文件中,用于DEBUG
	virtual void dump(FILE *);

	///将内容写到文件中,用于DEBUG
	virtual void dump(char *filename);

	///清空所有的记录
	virtual void clearAll();

	friend class CAccountDetailResource;

	///将一个CAccountDetail加入到该对象工厂，实质上是完成修改索引的工作
	///@param	pAccountDetail	要加入的AccountDetail
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	///@return	加入后该对象的位置，当pTransaction!=NULL时，总是返回NULL
	CAccountDetail* add(CWriteableAccountDetail *pAccountDetail, CTransaction *pTransaction=NULL);
	
	///刷新该CAccountDetail的值
	///@param	pAccountDetail	被刷新的CAccountDetail
	///@param	pNewAccountDetail	新的值
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	CAccountDetail* update(CAccountDetail *pAccountDetail, CWriteableAccountDetail *pNewAccountDetail, CTransaction *pTransaction=NULL, bool bNoTransaction = true);
	
	///增加或者更新数据
	///@param	pAccountDetail	  需要被刷新或者新增的CAccountDetail,等于NULL表示是需要新增的
	///@param	pNewAccountDetail	新的值
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	///@param	新增或者修改后的数据
	CAccountDetail* addOrUpdate(CAccountDetail *pAccountDetail, CWriteableAccountDetail *pNewAccountDetail, CTransaction *pTransaction=NULL);
	
	///删除一个CAccountDetail，同时删除其索引
	///@param	pAccountDetail	要删除的CAccountDetail
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	void remove(CAccountDetail *pAccountDetail, CTransaction *pTransaction=NULL, bool bNoTransaction=true);
	
	///获取某个CAccountDetail
	///@param	pAccountDetail	要读取的CAccountDetail
	///@param	pTragetAccountDetail	存放结果的CAccountDetail
	void retrieve(CAccountDetail *pAccountDetail, CWriteableAccountDetail *pTargetAccountDetail);
	
	///获取第一个CAccountDetail
	///@return	得到的第一个CAccountDetail，如果没有，则返回NULL
	CAccountDetail* getFirst();
	
	///获取下一个CAccountDetail
	///@return	得到下一个CAccountDetail，如果没有，则返回NULL
	CAccountDetail* getNext();
	
	///加入一个操作触发器
	///@param	pTrigger	要加入的操作触发器
	///@return	1表示成功，0表示失败
	int addActionTrigger(CAccountDetailActionTrigger *pTrigger);
	
	///删除一个操作触发器
	///@param	pTrigger	要删除的操作触发器
	///@return	1表示成功，0表示失败
	int removeActionTrigger(CAccountDetailActionTrigger *pTrigger);

	///加入一个确认触发器
	///@param	pTrigger	要加入的确认触发器
	///@return	1表示成功，0表示失败
	int addCommitTrigger(CAccountDetailCommitTrigger *pTrigger);
	
	///删除一个确认触发器
	///@param	pTrigger	要删除的确认触发器
	///@return	1表示成功，0表示失败
	int removeCommitTrigger(CAccountDetailCommitTrigger *pTrigger);
	
	///检查是否字段为NULL
	///@param pFile 输出错误日志的文件
	void checkNull(FILE *pFile);
	
	///检查是否字段合法
	///@param pFile 输出错误日志的文件
	void checkValid(FILE *pFile);	


private:
	///完成自己的初始化工作
	///@param	maxUnit	最多的数据对象个数
	///@param	pAllocator	进行内存管理的分配器
	///@param	pIndexMap	索引名称和内存编号的对应表，在非重用时会写入，在重用时只读出
	///@param	reuse	是否重用
	///@param	reuseID	重用编号，非重用时应当置为-1
	///@param	readOnly	1表示该CFixMem是只读的，0表示可以读写
	void init(int maxUnit,IMemoryAllocator *pAllocator,map<string,int>* pIndexMap,bool reuse,int reuseID,int readOnly);
	
	///将一个CAccountDetail加入到该对象工厂，实质上是完成修改索引的工作
	///@param	pAccountDetail	要加入的AccountDetail
	///@param	bNoTransaction 是否不属于任何事务
	///@return	加入后该对象的位置，当pTransaction!=NULL时，总是返回NULL
	CAccountDetail* internalAdd(CWriteableAccountDetail *pAccountDetail, bool bNoTransaction);
	
	
	///刷新该CAccountDetail的键值
	///@param	pAccountDetail	被刷新的CAccountDetail
	///@param	pNewAccountDetail	新的值
	///@param	bNoTransaction 是否不属于任何事务
	CAccountDetail* internalUpdate(CAccountDetail *pAccountDetail, CWriteableAccountDetail *pNewAccountDetail, bool bNoTransaction);
	
	///删除一个CAccountDetail，同时删除其索引
	///@param	pAccountDetail	要删除的CAccountDetail
	///@param	bNoTransaction 是否不属于任何事务
	void internalRemove(CAccountDetail *pAccountDetail, bool bNoTransaction);
/*
	///检查某个CAccountDetail是否属于本对象工厂
	///@param	pAccountDetail	要检查的对象
	///@exception	CDesignError	如果不属于，则抛出此异常
	void checkAddress(CAccountDetail *pAccountDetail);
*/

	
	
		
	///确认加入后触发
	///@param	pAccountDetail	已经加入的AccountDetail
	virtual void commitAdd(CAccountDetail *pAccountDetail);

	///确认更新后触发
	///@param	pAccountDetail	被刷新的CAccountDetail
	///@param	poldAccountDetail	原来的值
	virtual void commitUpdate(CAccountDetail *pAccountDetail, CWriteableAccountDetail *pOldAccountDetail);
	
	///确认删除后触发
	///@param	pAccountDetail	已经删除的CAccountDetail
	virtual void commitRemove(CWriteableAccountDetail *pAccountDetail);

	///所有相关的触发器
	vector<CAccountDetailActionTrigger *> *pActionTriggers;
	vector<CAccountDetailCommitTrigger *> *pCommitTriggers;
	
	///进行查询时，用于比较的对象
	CWriteableAccountDetail compareObject;
	CMDB* m_pMDB;
};

/////////////////////////////////////////////////////////////////////////
///CSettleDetailFactory是一个结算明细的对象工厂。它包含了一批结算明细，
///同时又建立了如下索引，以方便查询：
///使用本对象工厂，可以完成对结算明细的增删改查操作。具体方法如下：
///	增加：叫用add，完成加入
///	删除：叫用remove就可以删除
///	查询：有如下方法可以查询：
///		使用findByInstrument，唯一找到对象
///	修改：对于查到的结果，可以通过retrieve取出，修改后，通过update放回
///	事务管理：所有的增加、删除和修改都可以指定事务，在事务commit时，真正写入
///	在retrieve时，也可以指定事务，此时将取出经过update的新数据
/////////////////////////////////////////////////////////////////////////
class CSettleDetailFactory :public CDatabaseDataFactory
{
public:
	///构造方法
	///@param	maxUnit	初始的数据对象个数
	///@param	pAllocator	进行内存管理的分配器
	///@param	pIndexMap	索引名称和内存编号的对应表，在非重用时会写入，在重用时只读出
	///@param	reuse	是否重用
	///@param	reuseID	重用编号，非重用时应当置为-1
	///@param	readOnly	1表示该CFixMem是只读的，0表示可以读写
	CSettleDetailFactory(CMDB* pMDB,int maxUnit,IMemoryAllocator *pAllocator,map<string,int>* pIndexMap,bool reuse=false, int reuseID=-1, int readOnly=0);
	
	///构造方法
	///@param	pAllocator	进行内存管理的分配器
	///@param	pIndexMap	索引名称和内存编号的对应表，在非重用时会写入，在重用时只读出
	///@param	reuse	是否重用
	///@param	reuseID	重用编号，非重用时应当置为-1
	///@param	readOnly	1表示该CFixMem是只读的，0表示可以读写
	CSettleDetailFactory(CMDB* pMDB,IMemoryAllocator *pAllocator,map<string,int>* pIndexMap,bool reuse=false, int reuseID = -1,int readOnly=0);
	
	///析构方法，将负责释放所占用的空间
	virtual ~CSettleDetailFactory();

	///从CSV文件中读取所有的SettleDetail
	///@param	filename	文件名
	///@return	1表示完成，0表示没有完成
	virtual int readCSV(char *filename);

	///从CSV文件中读取所有的SettleDetail，文件名将根据配置文件中的定义SettleDetailCSVFile
	///@return	1表示完成，0表示没有完成
	virtual int readCSVByPath(char *pPath);
	
	///向CSV文件中写出所有的SettleDetail
	///@param	filename	文件名
	///@return	1表示完成，0表示没有完成
	virtual int writeCSV(char *filename);
	
	///向CSV文件中写出上一个版本的SettleDetail
	///@param	filename	文件名
	///@return	1表示完成，0表示没有完成
	virtual int writeCSVPreVersion(char *filename);	

	///向CSV文件中写出所有的SettleDetail，文件名将根据配置文件中的定义SettleDetailCSVFile
	///@return	1表示完成，0表示没有完成
	virtual int writeCSVByPath(char *pPath);

	///将内容写到文件中,用于DEBUG
	virtual void dump(FILE *);

	///将内容写到文件中,用于DEBUG
	virtual void dump(char *filename);

	///清空所有的记录
	virtual void clearAll();

	friend class CSettleDetailResource;

	///将一个CSettleDetail加入到该对象工厂，实质上是完成修改索引的工作
	///@param	pSettleDetail	要加入的SettleDetail
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	///@return	加入后该对象的位置，当pTransaction!=NULL时，总是返回NULL
	CSettleDetail* add(CWriteableSettleDetail *pSettleDetail, CTransaction *pTransaction=NULL);
	
	///刷新该CSettleDetail的值
	///@param	pSettleDetail	被刷新的CSettleDetail
	///@param	pNewSettleDetail	新的值
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	CSettleDetail* update(CSettleDetail *pSettleDetail, CWriteableSettleDetail *pNewSettleDetail, CTransaction *pTransaction=NULL, bool bNoTransaction = true);
	
	///增加或者更新数据
	///@param	pSettleDetail	  需要被刷新或者新增的CSettleDetail,等于NULL表示是需要新增的
	///@param	pNewSettleDetail	新的值
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	///@param	新增或者修改后的数据
	CSettleDetail* addOrUpdate(CSettleDetail *pSettleDetail, CWriteableSettleDetail *pNewSettleDetail, CTransaction *pTransaction=NULL);
	
	///删除一个CSettleDetail，同时删除其索引
	///@param	pSettleDetail	要删除的CSettleDetail
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	void remove(CSettleDetail *pSettleDetail, CTransaction *pTransaction=NULL, bool bNoTransaction=true);
	
	///获取某个CSettleDetail
	///@param	pSettleDetail	要读取的CSettleDetail
	///@param	pTragetSettleDetail	存放结果的CSettleDetail
	void retrieve(CSettleDetail *pSettleDetail, CWriteableSettleDetail *pTargetSettleDetail);
	
	///获取第一个CSettleDetail
	///@return	得到的第一个CSettleDetail，如果没有，则返回NULL
	CSettleDetail* getFirst();
	
	///获取下一个CSettleDetail
	///@return	得到下一个CSettleDetail，如果没有，则返回NULL
	CSettleDetail* getNext();
	
	///加入一个操作触发器
	///@param	pTrigger	要加入的操作触发器
	///@return	1表示成功，0表示失败
	int addActionTrigger(CSettleDetailActionTrigger *pTrigger);
	
	///删除一个操作触发器
	///@param	pTrigger	要删除的操作触发器
	///@return	1表示成功，0表示失败
	int removeActionTrigger(CSettleDetailActionTrigger *pTrigger);

	///加入一个确认触发器
	///@param	pTrigger	要加入的确认触发器
	///@return	1表示成功，0表示失败
	int addCommitTrigger(CSettleDetailCommitTrigger *pTrigger);
	
	///删除一个确认触发器
	///@param	pTrigger	要删除的确认触发器
	///@return	1表示成功，0表示失败
	int removeCommitTrigger(CSettleDetailCommitTrigger *pTrigger);
	
	///检查是否字段为NULL
	///@param pFile 输出错误日志的文件
	void checkNull(FILE *pFile);
	
	///检查是否字段合法
	///@param pFile 输出错误日志的文件
	void checkValid(FILE *pFile);	

	///寻找CSettleDetail
	///@return	找到的CSettleDetail，如果找不到，返回NULL
	CSettleDetail *findByInstrument(const CReadOnlyIndexIDType&  SettleDetailID, const CReadOnlyExchangeIDType&  ExchangeID, const CReadOnlyInstrumentIDType&  InstrumentID);

	///根据主码寻找
	///@return	找到的CSettleDetail，如果找不到，返回NULL
	CSettleDetail *findByPK(const CReadOnlyIndexIDType& SettleDetailID, const CReadOnlyExchangeIDType& ExchangeID, const CReadOnlyInstrumentIDType& InstrumentID);
		
	///根据主键删除CSettleDetail
	bool removeByPK(const CReadOnlyIndexIDType& SettleDetailID, const CReadOnlyExchangeIDType& ExchangeID, const CReadOnlyInstrumentIDType& InstrumentID, CTransaction *pTransaction=NULL);
	
	///判断字段是否是是主键
	bool isPK(const char* pName);
	
	///根据主键寻找CSettleDetail
	///@param	pSettleDetail	要找的值
	///@return	找到的CSettleDetail，如果找不到，返回NULL
	CSettleDetail *findByPK(CSettleDetailField *pSettleDetail);
	
	///根据主键寻找CSettleDetail
	///@param	pSettleDetail	要找的值
	///@return	找到的CSettleDetail，如果找不到，返回NULL
	CSettleDetail *findByPK(CSettleDetail *pSettleDetail);

	///根据主键获取CSettleDetail
	///@param	pSettleDetail	要获取的值，同时用于存放返回的结果
	///@return	找到的CSettleDetail，如果找不到，返回NULL
	bool retrieveByPK(CWriteableSettleDetail *pSettleDetail);

	///根据主键增加新的CSettleDetail
	///@param	pSettleDetail	新的值
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	///@return	返回插入的CSettleDetail，如果插入失败，则返回NULL
	CSettleDetail *addByPK(CWriteableSettleDetail *pSettleDetail, CTransaction *pTransaction=NULL);
	
	///根据主键增加或更新新的CSettleDetail
	///@param	pSettleDetail	新的值
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	///@return	返回插入的CSettleDetail，如果插入失败，则返回NULL
	CSettleDetail *addOrUpdateByPK(CWriteableSettleDetail *pSettleDetail, CTransaction *pTransaction=NULL);

	///根据主键更新CSettleDetail的值
	///@param	pSettleDetail	新的值
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	///@return	true表示成功，false表示失败
	bool updateByPK(CWriteableSettleDetail *pSettleDetail, CTransaction *pTransaction=NULL);

	///根据主键删除CSettleDetail
	///@param	pSettleDetail	要删除的值
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	///@return	true表示成功，false表示失败
	bool removeByPK(CSettleDetail *pSettleDetail, CTransaction *pTransaction=NULL);
	
	///根据主键删除CSettleDetail
	///@param	pSettleDetail	要删除的值
	///@param	pTransaction	本次操作所属的事务，如果是NULL，表示不使用事务管理，立即生效，否则需要在该事务commit时生效
	///@return	true表示成功，false表示失败
	bool removeByPK(CSettleDetailField *pSettleDetail, CTransaction *pTransaction=NULL);	


private:
	///完成自己的初始化工作
	///@param	maxUnit	最多的数据对象个数
	///@param	pAllocator	进行内存管理的分配器
	///@param	pIndexMap	索引名称和内存编号的对应表，在非重用时会写入，在重用时只读出
	///@param	reuse	是否重用
	///@param	reuseID	重用编号，非重用时应当置为-1
	///@param	readOnly	1表示该CFixMem是只读的，0表示可以读写
	void init(int maxUnit,IMemoryAllocator *pAllocator,map<string,int>* pIndexMap,bool reuse,int reuseID,int readOnly);
	
	///将一个CSettleDetail加入到该对象工厂，实质上是完成修改索引的工作
	///@param	pSettleDetail	要加入的SettleDetail
	///@param	bNoTransaction 是否不属于任何事务
	///@return	加入后该对象的位置，当pTransaction!=NULL时，总是返回NULL
	CSettleDetail* internalAdd(CWriteableSettleDetail *pSettleDetail, bool bNoTransaction);
	
	
	///刷新该CSettleDetail的键值
	///@param	pSettleDetail	被刷新的CSettleDetail
	///@param	pNewSettleDetail	新的值
	///@param	bNoTransaction 是否不属于任何事务
	CSettleDetail* internalUpdate(CSettleDetail *pSettleDetail, CWriteableSettleDetail *pNewSettleDetail, bool bNoTransaction);
	
	///删除一个CSettleDetail，同时删除其索引
	///@param	pSettleDetail	要删除的CSettleDetail
	///@param	bNoTransaction 是否不属于任何事务
	void internalRemove(CSettleDetail *pSettleDetail, bool bNoTransaction);
/*
	///检查某个CSettleDetail是否属于本对象工厂
	///@param	pSettleDetail	要检查的对象
	///@exception	CDesignError	如果不属于，则抛出此异常
	void checkAddress(CSettleDetail *pSettleDetail);
*/

	
	///基于InstrumentHashKey的hash索引
	CHashIndex *pInstrumentHashIndex;
	
		
	///确认加入后触发
	///@param	pSettleDetail	已经加入的SettleDetail
	virtual void commitAdd(CSettleDetail *pSettleDetail);

	///确认更新后触发
	///@param	pSettleDetail	被刷新的CSettleDetail
	///@param	poldSettleDetail	原来的值
	virtual void commitUpdate(CSettleDetail *pSettleDetail, CWriteableSettleDetail *pOldSettleDetail);
	
	///确认删除后触发
	///@param	pSettleDetail	已经删除的CSettleDetail
	virtual void commitRemove(CWriteableSettleDetail *pSettleDetail);

	///所有相关的触发器
	vector<CSettleDetailActionTrigger *> *pActionTriggers;
	vector<CSettleDetailCommitTrigger *> *pCommitTriggers;
	
	///进行查询时，用于比较的对象
	CWriteableSettleDetail compareObject;
	CMDB* m_pMDB;
};

#endif
