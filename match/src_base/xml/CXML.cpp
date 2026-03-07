#include "CXML.h"
#ifdef LINUX
#include <iconv.h>
#endif

typedef struct
{
	char	ch;
	char	*entityString;
}	TEntityDefine;

TEntityDefine entityDefines[] =
{
	{ '<',"lt" },
	{ '>',"gt" },
	{ '\"',"quot" },
	{ '\'',"apos" },
	{ '&',"amp" }
};

#define ENTITY_DEFINE_COUNT (sizeof(entityDefines)/sizeof(TEntityDefine))

///获取指定字符对应的entity字符串
///@param	ch	指定的字符
///@return	对应的entity字符串，如果不需要转化，则返回NULL。本实现只处理标准的entity
static char *getEntityString(char ch)
{
	int i;
	for (i = 0; i < ENTITY_DEFINE_COUNT; i++)
	{
		if (ch == entityDefines[i].ch)
		{
			return entityDefines[i].entityString;
		}
	}
	return NULL;
}

///根据读入的entity字符串，确定对应的字符
///@param	entityBuffer	读入的entity字符串
///@return	对应的字符，EOF表示没有对应的字符
static int getEntityChar(char *entityBuffer)
{
	int i;

	if (entityBuffer[0] == '#')
	{
		if (entityBuffer[1] == 'x')
		{
			char *endPtr;
			return strtol(entityBuffer + 2, &endPtr, 16);
		}
		else
		{
			return atoi(entityBuffer + 1);
		}
	}
	else
	{
		for (i = 0; i < ENTITY_DEFINE_COUNT; i++)
		{
			if (!strcmp(entityBuffer, entityDefines[i].entityString))
			{
				return entityDefines[i].ch;
			}
		}
	}
	return EOF;
}

///输出一段CData，自动进行必要的entity转化
///@param	output	要输出的文件
///@param	string	要输出的字符串
static void outputCData(FILE *output, char *string)
{
	char *p;
	char *entityString;

	p = string;
	while (*p != '\0')
	{
		entityString = getEntityString(*p);
		if (entityString != NULL)
		{
			fprintf(output, "&%s;", entityString);
		}
		else
		{
			fprintf(output, "%c", *p);
		}
		p++;
	}
}

/*
///MAX_STRING表示本系统支持的name或者value的最长长度
#define MAX_STRING 40960

///在CData字符串后面增加一段内容
///@param	target	目标字符串空间
///@param	ppTarget	指向目标字符串结束位置的指针的指针，返回时会修改
///@param	appendString	要增加的字符串
///@return	1表示成功，0表示失败，失败的唯一原因是目标字符串空间的长度不够
static int appendCData(char *target, char **ppTarget,char *appendString)
{
	int appendLength;

	appendLength=strlen(appendString);
	if (*ppTarget-target+appendLength>=MAX_STRING-1)
	{
		return 0;
	}
	strcpy(*ppTarget,appendString);
	*ppTarget+=appendLength;
	return 1;
}

///将字符串转化为XML的CData格式，实际上就是对于特殊字符进行转化处理
///@param	target	目标字符串空间，必须有MAX_STRING长度
///@param	source	源字符串
///@return	如果成功，则返回target，失败则返回NULL
static char *toCData(char *target, char *source)
{
	char *pSource,*pTarget;
	pSource=source;
	pTarget=target;
	while (*pSource!='\0')
	{
		char *entityString;
		char appendString[10];

		entityString=getEntityString(*pSource);
		if (entityString==NULL)
		{
			sprintf(appendString,"%c",*pSource);
		}
		else
		{
			sprintf(appendString,"&%s;",entityString);
		}
		if (!appendCData(target,&pTarget,appendString))
		{
			return NULL;
		}
		pSource++;
	}
	*pTarget='\0';
	return target;
}
*/

CXMLProperty::CXMLProperty(char *name, char *value) :
	m_name(name), m_value(value)
{
}

CXMLProperty::~CXMLProperty(void)
{
}

int CXMLProperty::isA(char *objectType)
{
	if (!strcmp(objectType, "CXMLProperty"))
		return 1;
	return 0;
	//	return CBaseObject::isA(objectType);
}

const char *CXMLProperty::getType(void)
{
	return "CXMLProperty";
}

// void CXMLProperty::output(CLogger *pLogger,int indent)
//  {
//  //	CHECK_TYPE("CXMLProperty");
//  	pLogger->output(indent,0,"%s=%s",getName(),getValue());
//  }

CXMLProperty *CXMLProperty::clone(void)
{
	CXMLProperty *pNewProperty;

	pNewProperty = new CXMLProperty();

	pNewProperty->setName(m_name);
	pNewProperty->setValue(m_value);

	return pNewProperty;
}

void CXMLProperty::setName(char *name)
{
	m_name = name;
}

void CXMLProperty::setValue(char *value)
{
	m_value = value;
}

char *CXMLProperty::getName(void)
{
	return m_name;
}

char *CXMLProperty::getValue(void)
{
	return m_value;
}

int CXMLProperty::write(FILE *output)
{
	fprintf(output, "%s", getName());
	/*
	if ((m_value==NULL)||(*m_value=='\0'))
	{
		return 1;
	}
	*/
	fprintf(output, "=\"");
	outputCData(output, getValue());
	fprintf(output, "\"");
	return 1;
}

CXMLPropertyList::CXMLPropertyList(void)
{
}

CXMLPropertyList::~CXMLPropertyList(void)
{
	CXMLPropertyList::iterator pos;
	for (pos = begin(); pos != end(); ++pos) {
		delete (*pos);
	}
}

int CXMLPropertyList::isA(char *objectType)
{
	if (!strcmp(objectType, "CXMLPropertyList"))
		return 1;
	return 0;
}

char *CXMLPropertyList::getType(void)
{
	return "CXMLPropertyList";
}

// void CXMLPropertyList::output(CLogger *pLogger,int indent)
// {
// 	for (int i=0;i<size();i++)
// 	{
// 		at(i)->output(pLogger,indent+1);
// 	}
// }

CXMLPropertyList *CXMLPropertyList::clone(void)
{
	CXMLPropertyList *pNewPropertyList;

	pNewPropertyList = new CXMLPropertyList();

	iterator it;
	for (it = begin(); it < end(); it++)
	{
		pNewPropertyList->push_back((*it)->clone());
	}

	return pNewPropertyList;
}

int CXMLPropertyList::addProperty(char *name, char *value)
{
	CXMLProperty *pProperty = new CXMLProperty(name, value);

	push_back(pProperty);

	return 1;
}

int CXMLPropertyList::addProperty(char *name, int value)
{
	char buffer[20];
	sprintf(buffer, "%d", value);
	CXMLProperty *pProperty = new CXMLProperty(name, buffer);

	push_back(pProperty);

	return 1;
}

int CXMLPropertyList::findID(char *name)
{
	int i;
	for (i = 0; i < size(); i++)
	{
		if (!strcmp(name, at(i)->getName()))
		{
			return i;
		}
	}
	return -1;
}

int CXMLPropertyList::removeProperty(char *name)
{
	int id = findID(name);
	if (id == -1)
	{
		return 0;
	}
	delete at(id);
	erase(begin() + id);
	return 1;
}

int CXMLPropertyList::replaceProperty(char *name, char *newValue)
{
	CXMLProperty *pProperty;
	pProperty = findProperty(name);
	if (pProperty == NULL)
	{
		return 0;
	}
	pProperty->setValue(newValue);
	return 1;
}

CXMLProperty *CXMLPropertyList::findProperty(char *name)
{
	int id;
	id = findID(name);
	if (id == -1)
	{
		return NULL;
	}
	return at(id);
}

char *CXMLPropertyList::findValue(char *name)
{
	CXMLProperty *pProperty;
	pProperty = findProperty(name);
	if (pProperty == NULL)
	{
		return NULL;
	}
	return pProperty->getValue();
}

int CXMLPropertyList::write(FILE *output)
{
	int i;
	for (i = 0; i < size(); i++)
	{
		fprintf(output, " ");
		if (!at(i)->write(output))
		{
			return 0;
		}
	}
	return 1;
}

CXMLElement::CXMLElement(void)
{
	m_type = XMLNotElement;
	m_propertyList = NULL;
}

CXMLElement::~CXMLElement(void)
{
}

void CXMLElement::removePropertyList(void)
{
	if (m_propertyList != NULL)
	{
		delete m_propertyList;
	}
	m_propertyList = NULL;
}

int CXMLElement::isA(char *objectType)
{
	if (!strcmp(objectType, "CXMLElement"))
		return 1;
	return 0;
	//	return CBaseObject::isA(objectType);
}

const char *CXMLElement::getType(void)
{
	return "CXMLElement";
}

// void CXMLElement::output(CLogger *pLogger,int indent)
// {
// 	switch (m_type)
// 	{
// 	case XMLStartElement:
// 		pLogger->output(indent,0,"<%s",getName());
// 		if (m_propertyList!=NULL)
// 		{
// 			m_propertyList->output(pLogger,indent+1);
// 		}
// 		pLogger->output(indent,0,">");
// 		return;
// 	case XMLEndElement:
// 		pLogger->output(indent,0,"</%s>",getName());
// 		return;
// 	case XMLSingleElement:
// 		pLogger->output(indent,0,"<%s",getName());
// 		if (m_propertyList!=NULL)
// 		{
// 			m_propertyList->output(pLogger,indent+1);
// 		}
// 		pLogger->output(indent,0,"/>");
// 		return;
// 	case XMLTextElement:
// 		pLogger->output(indent,0,"%s",getName());
// 		return;
// 	case XMLCommentElement:
// 		pLogger->output(indent,0,"<!--%s-->",getName());
// 		return;
// 	case XMLDocumentElement:
// 	case XMLPIElement:
// 		pLogger->output(indent,0,"<?%s",getName());
// 		if (m_propertyList!=NULL)
// 		{
// 			m_propertyList->output(pLogger,indent+1);
// 		}
// 		pLogger->output(indent,0,"?>");
// 		return;
// 	case XMLDTDElement:
// 		pLogger->output(indent,0,"<!DOCTYPE");
// 		if (m_propertyList!=NULL)
// 		{
// 			m_propertyList->output(pLogger,indent+1);
// 		}
// 		pLogger->output(indent,0,">");
// 		return;
// 	default:
// 		pLogger->output(indent,0,"unknown XML Element");
// 	}
// }

XMLElementType CXMLElement::getElementType(void)
{
	return m_type;
}

void CXMLElement::setElementType(XMLElementType type)
{
	m_type = type;
}

char *CXMLElement::getName(void)
{
	return m_name;
}

void CXMLElement::setName(char *name)
{
	m_name = name;
}

CXMLPropertyList *CXMLElement::getPropertyList(void)
{
	return m_propertyList;
}

void CXMLElement::setPropertyList(CXMLPropertyList *propertyList)
{
	m_propertyList = propertyList;
}

int CXMLElement::addProperty(char *name, char *value)
{
	if (m_propertyList == NULL)
	{
		m_propertyList = new CXMLPropertyList();
	}
	return m_propertyList->addProperty(name, value);
}

int CXMLElement::removeProperty(char *name)
{
	if (m_propertyList == NULL)
	{
		m_propertyList = new CXMLPropertyList();
	}
	return m_propertyList->removeProperty(name);
}

int CXMLElement::replaceProperty(char *name, char *newValue)
{
	if (m_propertyList == NULL)
	{
		m_propertyList = new CXMLPropertyList();
	}
	return m_propertyList->replaceProperty(name, newValue);
}

char *CXMLElement::findProperty(char *name)
{
	if (m_propertyList == NULL)
	{
		m_propertyList = new CXMLPropertyList();
	}
	return m_propertyList->findValue(name);
}

int CXMLElement::write(FILE *output)
{
	switch (m_type)
	{
	case XMLStartElement:
		fprintf(output, "<%s", getName());
		if (m_propertyList != NULL)
		{
			if (!m_propertyList->write(output))
			{
				return 0;
			}
		}
		fprintf(output, ">");
		return 1;
	case XMLEndElement:
		fprintf(output, "</%s>", getName());
		return 1;
	case XMLSingleElement:
		fprintf(output, "<%s", getName());
		if (m_propertyList != NULL)
		{
			if (!m_propertyList->write(output))
			{
				return 0;
			}
		}
		fprintf(output, "/>");
		return 1;
	case XMLTextElement:
		outputCData(output, getName());
		return 1;
	case XMLCommentElement:
		fprintf(output, "<!--");
		outputCData(output, getName());
		fprintf(output, "-->");
		return 1;
	case XMLDocumentElement:
	case XMLPIElement:
		fprintf(output, "<?");
		outputCData(output, getName());
		if (m_propertyList != NULL)
		{
			if (!m_propertyList->write(output))
			{
				return 0;
			}
		}
		fprintf(output, "?>");
		return 1;
	case XMLDTDElement:
		fprintf(output, "<!DOCTYPE");
		if (m_propertyList != NULL)
		{
			if (!m_propertyList->write(output))
			{
				return 0;
			}
		}
		fprintf(output, ">");
		return 1;
	default:
		return 0;
	}
}

CXMLNodeList::CXMLNodeList(void)
{
}

CXMLNodeList::~CXMLNodeList(void)
{
	CXMLNodeList::iterator pos;
	for (pos = begin(); pos != end(); ++pos)
		delete (*pos);
}

int CXMLNodeList::isA(char *objectType)
{
	if (!strcmp(objectType, "CXMLNodeList"))
		return 1;
	return 0;
}

char *CXMLNodeList::getType(void)
{
	return "CXMLNodeList";
}

// void CXMLNodeList::output(CLogger *pLogger,int indent)
// {
// 	for (int i=0;i<size();i++)
// 	{
// 		at(i)->output(pLogger,indent+1);
// 	}
// }

CXMLNodeList *CXMLNodeList::clone(void)
{
	CXMLNodeList *pNewNodeList;

	pNewNodeList = new CXMLNodeList();

	iterator it;
	for (it = begin(); it < end(); it++)
	{
		pNewNodeList->push_back((*it)->clone(true));
	}

	return pNewNodeList;
}

CXMLNode *CXMLNodeList::findNode(XMLNodeType type, char *tagName, char *propertyName, char *propertyValue)
{
	for (int i = 0; i < size(); i++)
	{
		CXMLNode *pNode;
		pNode = at(i);
		if (!pNode->validNode(type, tagName, propertyName, propertyValue))
		{
			continue;
		}
		return pNode;
	}
	return NULL;
}

int CXMLNodeList::findNode(CXMLNode *pNode)
{
	for (int i = 0; i < size(); i++)
	{
		if (at(i) == pNode)
		{
			return i;
		}
	}
	return -1;
}

int CXMLNodeList::write(FILE *output, int indent)
{
	for (int i = 0; i < size(); i++)
	{
		CXMLNode *pNode;
		pNode = at(i);
		if (!pNode->write(output, indent))
		{
			return 0;
		}
	}
	return 1;
}

int CXMLAction::nodeHandler(CXMLNode *pNode)
{
	return 1;
}

int CXMLAction::stringHandler(char *string)
{
	return 1;
}

CXMLNode::CXMLNode(void)
{
	m_type = XMLNotNode;
	m_propertyList = NULL;
	m_nodeList = NULL;
	m_father = NULL;
}

CXMLNode::~CXMLNode(void)
{
	if (m_propertyList != NULL)
	{
		delete m_propertyList;
	}
	if (m_nodeList != NULL)
	{
		delete m_nodeList;
	}
}

int CXMLNode::isA(char *objectType)
{
	if (!strcmp(objectType, "CXMLNode"))
		return 1;
	return 0;
	//	return CBaseObject::isA(objectType);
}

const char *CXMLNode::getType(void)
{
	return "CXMLNode";
}

// void CXMLNode::output(CLogger *pLogger,int indent)
// {
// 	switch (m_type)
// 	{
// 	case XMLTag:
// 		pLogger->output(indent,0,"<%s",getName());
// 		if (m_propertyList!=NULL)
// 		{
// 			m_propertyList->output(pLogger,indent+1);
// 		}
// 		pLogger->output(indent,0,">");
// 		if (m_nodeList!=NULL)
// 		{
// 			m_nodeList->output(pLogger,indent+1);
// 		}
// 		pLogger->output(indent,0,"</%s>",getName());
// 		return;
// 	case XMLText:
// 		pLogger->output(indent,0,"%s",getName());
// 		return;
// 	case XMLComment:
// 		pLogger->output(indent,0,"<!--%s-->",getName());
// 	default:
// 		pLogger->output(indent,0,"unknown XML node");
// 		return;
// 	}
// }

CXMLNode *CXMLNode::clone(bool includeSon)
{
	CXMLNode *newNode;

	newNode = new CXMLNode();
	newNode->m_type = m_type;
	newNode->m_name = m_name;
	newNode->m_propertyList = m_propertyList->clone();
	if (includeSon && (m_nodeList != NULL))
	{
		newNode->m_nodeList = m_nodeList->clone();
	}
	return newNode;
}

XMLNodeType CXMLNode::getNodeType(void)
{
	return m_type;
}

void CXMLNode::setNodeType(XMLNodeType type)
{
	m_type = type;
}

char *CXMLNode::getName(void)
{
	return m_name;
}

void CXMLNode::setName(char *name)
{
	m_name = name;
}

CXMLPropertyList *CXMLNode::getPropertyList(void)
{
	return m_propertyList;
}

void CXMLNode::setPropertyList(CXMLPropertyList *propertyList)
{
	m_propertyList = propertyList;
}

int CXMLNode::addProperty(char *name, char *value)
{
	if (m_propertyList == NULL)
	{
		m_propertyList = new CXMLPropertyList();
	}
	return m_propertyList->addProperty(name, value);
}

int CXMLNode::addProperty(char *name, int value)
{
	if (m_propertyList == NULL)
	{
		m_propertyList = new CXMLPropertyList();
	}
	return m_propertyList->addProperty(name, value);
}

int CXMLNode::removeProperty(char *name)
{
	if (m_propertyList == NULL)
	{
		m_propertyList = new CXMLPropertyList();
	}
	return m_propertyList->removeProperty(name);
}

int CXMLNode::replaceProperty(char *name, char *newValue)
{
	if (m_propertyList == NULL)
	{
		m_propertyList = new CXMLPropertyList();
	}
	return m_propertyList->replaceProperty(name, newValue);
}

char *CXMLNode::findProperty(char *name)
{
	if (m_propertyList == NULL)
	{
		m_propertyList = new CXMLPropertyList();
	}
	return m_propertyList->findValue(name);
}

int CXMLNode::setProperty(char *name, char *value)
{
	if (findProperty(name) == NULL)
	{
		return addProperty(name, value);
	}
	else
	{
		return replaceProperty(name, value);
	}
}

CXMLNodeList *CXMLNode::getNodeList(void)
{
	return m_nodeList;
}

void CXMLNode::setNodeList(CXMLNodeList *nodeList)
{
	m_nodeList = nodeList;
}

void CXMLNode::addNode(CXMLNode *pNode)
{
	if (m_nodeList == NULL)
	{
		m_nodeList = new CXMLNodeList();
	}
	pNode->m_father = this;
	m_nodeList->push_back(pNode);
}

CXMLNode *CXMLNode::getFather(void)
{
	return m_father;
}

CXMLNode *CXMLNode::getBrother(int offset, int from)
{
	int startPos, resultPos;
	CXMLNodeList *pBrothers;

	if (m_father == NULL)
	{
		return NULL;
	}
	pBrothers = m_father->getNodeList();
	if (pBrothers == NULL)
	{
		return NULL;
	}

	if (from == SEEK_SET)
	{
		startPos = 0;
	}
	else if (from == SEEK_END)
	{
		startPos = (int)pBrothers->size() - 1;
	}
	else if (from == SEEK_CUR)
	{
		startPos = pBrothers->findNode(this);
		if (startPos == -1)
		{
			return NULL;
		}
	}
	else
	{
		return NULL;
	}

	resultPos = startPos + offset;
	return m_father->getSon(resultPos);
}

CXMLNode *CXMLNode::getSon(int offset, int from)
{
	int startPos, resultPos;

	if (m_nodeList == NULL)
	{
		return NULL;
	}

	if (from == SEEK_SET)
	{
		startPos = 0;
	}
	else if (from == SEEK_END)
	{
		startPos = (int)m_nodeList->size() - 1;
	}
	else
	{
		return NULL;
	}

	resultPos = startPos + offset;
	if ((resultPos < 0) || (resultPos >= m_nodeList->size()))
	{
		return NULL;
	}
	return m_nodeList->at(resultPos);
}

CXMLNode *CXMLNode::findNode(XMLNodeType type, char *tagName, char *propertyName, char *propertyValue)
{
	if (m_nodeList == NULL)
	{
		m_nodeList = new CXMLNodeList();
	}
	return m_nodeList->findNode(type, tagName, propertyName, propertyValue);
}

int CXMLNode::validNode(XMLNodeType type, char *tagName, char *propertyName, char *propertyValue)
{
	if (type != XMLNotNode)
	{
		if (getNodeType() != type)
		{
			return 0;
		}
	}
	if (tagName != NULL)
	{
		if (strcmp(getName(), tagName))
		{
			return 0;
		}
	}
	if (propertyName != NULL)
	{
		char *theValue;
		theValue = findProperty(propertyName);
		if (theValue == NULL)
		{
			return 0;
		}
		if (propertyValue != NULL)
		{
			if (strcmp(theValue, propertyValue))
			{
				return 0;
			}
		}
	}
	return 1;
}

int CXMLNode::write(FILE *output, int indent)
{
	int i;
	for (i = 0; i < indent; i++)
	{
		fprintf(output, "\t");
	}
	switch (m_type)
	{
	case XMLTag:
		fprintf(output, "<%s", getName());
		if (m_propertyList != NULL)
		{
			m_propertyList->write(output);
		}
		if ((m_nodeList != NULL) && (m_nodeList->size() != 0))
		{
			if ((m_nodeList->size() == 1) && getSon(0)->getNodeType() == XMLText)
			{
				fprintf(output, ">");
				outputCData(output, getSon(0)->getName());
				fprintf(output, "</%s>\n", getName());
			}
			else
			{
				fprintf(output, ">\n");
				m_nodeList->write(output, indent + 1);
				for (i = 0; i < indent; i++)
				{
					fprintf(output, "\t");
				}
				fprintf(output, "</%s>\n", getName());
			}
		}
		else
		{
			fprintf(output, "/>\n");
		}
		return 1;
	case XMLText:
		outputCData(output, getName());
		fprintf(output, "\n");
		return 1;
	case XMLComment:
		fprintf(output, "<!--");
		outputCData(output, getName());
		fprintf(output, "-->\n");
		return 1;
	case XMLNotNode:
		return 1;
	default:
		return 0;
	}
}

int CXMLNode::groupAction(TXMLActionFunc pFunc, void *parameter,
	XMLNodeType type, char *tagName, char *propertyName, char *propertyValue)
{
	return XMLGroupAction(getNodeList(), pFunc, parameter, type, tagName,
		propertyName, propertyValue);
}

int CXMLNode::groupAction(char *usePropertyName, TXMLPropertyActionFunc pFunc, void *parameter,
	XMLNodeType type, char *tagName, char *propertyName, char *propertyValue)
{
	return XMLGroupAction(getNodeList(), usePropertyName, pFunc, parameter, type, tagName,
		propertyName, propertyValue);
}

int CXMLNode::XAction(char *EXPath, CXMLAction *pAction)
{
	char *pPath;
	int depth = 0;

	pPath = EXPath;
	if (*pPath == '\0')
	{
		//已经没有要处理的EXPath了
		return pAction->nodeHandler(this);
	}
	if (*pPath++ != '/')
	{
		//开始字符不对
		return -2;
	}
	switch (*pPath)
	{
	case '@':
	case '?':
		//是STRING_REF
		return stringRefAction(pPath, pAction);
	case '(':
		//是指定深度的寻找
		pPath++;
		//获取深度
		while (*pPath != '\0')
		{
			if (isdigit(*pPath))
			{
				depth = depth * 10 + *pPath - '0';
			}
			else
			{
				break;
			}
			pPath++;
		}
		//如果不以括号结尾就错了
		if (*pPath != ')')
		{
			return -2;
		}
		//叫用适当的函数处理这个请求
		pPath++;
		return nodeCheck(pPath, pAction, depth);
	case '/':
		//是任意深度的寻找
		pPath++;
		return nodeCheck(pPath, pAction, -1);
	default:
		//是对子节点的寻找
		return nodeCheck(pPath, pAction, 1);
	}
}

int CXMLNode::stringRefAction(char *pPath, CXMLAction *pAction)
{
	char *pPathTail;
	char *string;
	int result;

	while (*pPath != '\0')
	{
		//获取当前STRING_REF对应的值
		result = getStringValue(pPath, &pPathTail, &string);
		if (result <= 0)
		{
			return result;
		}

		//如果已经找到，则叫用pAction即可
		if (string != NULL)
		{
			return pAction->stringHandler(string);
		}

		//如果找不到
		pPath = pPathTail;

		//后面没有了，则报出索取的值找不到错误
		if (*pPath == '\0')
		{
			return -3;
		}

		//如果后面还有，则继续找下去
		if (*pPath == ',')
		{
			pPath++;
		}
		else
		{
			break;
		}
	}
	//这个应该是语法错误
	return -2;
}

int CXMLNode::nodeCheck(char *pPath, CXMLAction *pAction, int depth)
{
	CXMLNodeList *pSons;
	CXMLNodeList::iterator it;
	char *pPathTail;
	int result = -1, thisResult;
	int isMatch;

	pSons = getNodeList();

	//检查当前节点或者所有子孙节点
	if (depth <= 0)
	{
		//如果本节点就满足要求，则处理它
		thisResult = matchNode(pPath, &pPathTail, &isMatch);
		if (thisResult <= 0)
		{
			return thisResult;
		}
		if (isMatch)
		{
			return XAction(pPathTail, pAction);
		}
	}

	//根据必要性，检查所有子节点
	if (depth != 0)
	{
		if (pSons == NULL)
		{
			return result;
		}
		for (it = pSons->begin(); it < pSons->end(); it++)
		{
			thisResult = (*it)->nodeCheck(pPath, pAction, depth - 1);
			if (thisResult != -1)
			{
				//如果不是因为没有可操作对象而终止，则使用此返回值
				result = thisResult;

				//如果因为各种原因出错终止，则没有必要继续下去了
				if (thisResult <= 0)
				{
					return thisResult;
				}
			}
		}
	}

	return result;
}

int CXMLNode::matchNode(char *pPath, char **pathTail, int *pIsMatch)
{
	int result;
	char *stringValue;

	//先检查TagName
	if (*pPath == '*')
	{
		//这个都能满足，就不检查了
		pPath++;
		*pathTail = pPath;
	}
	else
	{
		//节点名称不对，那就不用再做了
		char *tagName = getToken(pPath, pathTail);
		pPath = *pathTail;
		if (strcmp(tagName, getName()))
		{
			delete tagName;
			*pIsMatch = 0;
			return 1;
		}
		else
		{
			delete tagName;
		}
	}

	//再检查所有的条件
	while (*pPath == ',')
	{
		pPath++;

		//找到对应的字符串
		result = getStringValue(pPath, pathTail, &stringValue);
		if (result <= 0)
		{
			return result;
		}
		pPath = *pathTail;

		//找不到，那就不符合
		if (stringValue == NULL)
		{
			*pIsMatch = 0;
			return 1;
		}

		//看看有没有值的要求
		if (*pPath == '=')
		{
			pPath++;
			char *requiredValue = getToken(pPath, pathTail);
			pPath = *pathTail;
			if (strcmp(stringValue, requiredValue))
			{
				delete requiredValue;
				*pIsMatch = 0;
				return 1;
			}
			else
			{
				delete requiredValue;
			}
		}
	}

	//都满足了
	*pIsMatch = 1;
	return 1;
}

int CXMLNode::getStringValue(char *pPath, char **pathTail, char **pStringValue)
{
	int isProperty;

	//先检查是属性还是文本
	if (*pPath == '@')
	{
		isProperty = 1;
	}
	else if (*pPath == '?')
	{
		isProperty = 0;
	}
	else
	{
		return -2;
	}
	pPath++;

	//获取引用名
	char *refName = getToken(pPath, pathTail);
	if (isProperty)
	{
		*pStringValue = findProperty(refName);
	}
	else
	{
		CXMLNode *pNode = findNode(XMLTag, refName);
		if (pNode == NULL)
		{
			*pStringValue = NULL;
		}
		else
		{
			CXMLNodeList *pSons = pNode->getNodeList();
			if ((pSons == NULL) || (pSons->size() != 1) || (pSons->at(0)->getNodeType() != XMLText))
			{
				*pStringValue = NULL;
			}
			else
			{
				*pStringValue = pSons->at(0)->getName();
			}
		}
	}

	delete refName;
	return 1;
}

char *CXMLNode::getToken(char *pPath, char **pathTail)
{
	char *tail;
	char *result;
	int length;

	tail = pPath;
	while ((*tail != '\0') && (!strchr("/,=@?*", *tail)))
	{
		tail++;
	}
	*pathTail = tail;

	length = (int)(tail - pPath);
	result = new char[length + 1];
	strncpy(result, pPath, length);
	result[length] = '\0';
	return result;
}

int XMLGroupAction(CXMLNode *pNode, TXMLActionFunc pFunc, void *parameter,
	XMLNodeType type, char *tagName, char *propertyName, char *propertyValue)
{
	return XMLGroupAction(pNode->getNodeList(), pFunc, parameter, type, tagName,propertyName, propertyValue);
}

int XMLGroupAction(CXMLNodeList *pNodeList, TXMLActionFunc pFunc, void *parameter,
	XMLNodeType type, char *tagName, char *propertyName, char *propertyValue)
{
	if (pNodeList == NULL)
	{
		return -1;
	}
	int result = -1;
	CXMLNodeList::iterator it;
	for (it = pNodeList->begin(); it < pNodeList->end(); it++)
	{
		CXMLNode *pNode = *it;
		if (!pNode->validNode(type, tagName, propertyName, propertyValue))
		{
			continue;
		}
		result = pFunc(pNode, parameter);
		if (result == 0)
		{
			return result;
		}
	}
	return result;
}

int XMLGroupAction(CXMLNode *pNode, char *usePropertyName, TXMLPropertyActionFunc pFunc, void *parameter,
	XMLNodeType type, char *tagName, char *propertyName, char *propertyValue)
{
	return XMLGroupAction(pNode->getNodeList(), usePropertyName, pFunc, parameter, type, tagName,
		propertyName, propertyValue);
}

int XMLGroupAction(CXMLNodeList *pNodeList, char *usePropertyName, TXMLPropertyActionFunc pFunc, void *parameter,
	XMLNodeType type, char *tagName, char *propertyName, char *propertyValue)
{
	if (pNodeList == NULL)
	{
		return -1;
	}
	int result = -1;
	CXMLNodeList::iterator it;
	for (it = pNodeList->begin(); it < pNodeList->end(); it++)
	{
		CXMLNode *pNode = *it;
		if (!pNode->validNode(type, tagName, propertyName, propertyValue))
		{
			continue;
		}
		result = pFunc(pNode->findProperty(usePropertyName), parameter);
		if (result == 0)
		{
			return result;
		}
	}
	return result;
}

CEXPathNode::CEXPathNode(void)
{
	setNodeType(XMLTag);
	setName("EXPATH");
	prepared = 0;
}

int CEXPathNode::prepare(char *EXPath)
{
	CXMLNode *pEXNode;

	pPath = EXPath;

	while (*pPath != '\0')
	{
		pEXNode = makeEXNode();
		if (pEXNode == NULL)
		{
			return -2;
		}

		addNode(pEXNode);
		if (!strcmp(pEXNode->getName(), "STRING_VALUE"))
		{
			break;
		}
	}
	if ((*pPath != '\0') || (getNodeList() == NULL))
	{
		return -2;
	}
	prepared = 1;
	return 1;
}

int CEXPathNode::isPrepared(void)
{
	return prepared;
}

CXMLNode *CEXPathNode::makeEXNode(void)
{
	CXMLNode *pEXNode;
	CXMLNode *pCondition;
	CXMLNode *pStringRef;
	int depth = 0;

	if (*pPath++ != '/')
	{
		//开始字符不对
		return NULL;
	}
	switch (*pPath)
	{
	case '@':
	case '?':
		//是STRING_VALUE
		return makeStringValue();
	case '(':
		//是指定深度的寻找
		pPath++;
		//获取深度
		while (*pPath != '\0')
		{
			if (isdigit(*pPath))
			{
				depth = depth * 10 + *pPath - '0';
			}
			else
			{
				break;
			}
			pPath++;
		}
		//如果不以括号结尾就错了
		if (*pPath != ')')
		{
			return NULL;
		}
		//叫用适当的函数处理这个请求
		pPath++;
		break;
	case '/':
		//是任意深度的寻找
		pPath++;
		depth = -1;
		break;
	default:
		//是对子节点的寻找
		depth = 1;
		break;
	}

	pEXNode = new CXMLNode;
	pEXNode->setNodeType(XMLTag);
	pEXNode->setName("EXNODE");

	pEXNode->addProperty("DEPTH", depth);

	if (*pPath == '*')
	{
		pPath++;
		pEXNode->addProperty("TAG_NAME", "*");
	}
	else
	{
		char *tagName = getToken();
		if (tagName == NULL)
		{
			return NULL;
		}
		pEXNode->addProperty("TAG_NAME", tagName);
	}

	while (*pPath == ',')
	{
		pCondition = new CXMLNode;
		pCondition->setNodeType(XMLTag);
		pCondition->setName("CONDITION");
		pPath++;

		//找到对应的字符串
		pStringRef = getStringRef();
		if (pStringRef == NULL)
		{
			return NULL;
		}
		pCondition->addNode(pStringRef);

		//看看有没有值的要求
		if (*pPath == '=')
		{
			pPath++;
			char *requiredValue = getToken();
			if (requiredValue == NULL)
			{
				pCondition->addProperty("RESULT", "");
			}
			else
			{
				pCondition->addProperty("RESULT", requiredValue);
			}
		}
		pEXNode->addNode(pCondition);
	}

	return pEXNode;
}

CXMLNode *CEXPathNode::makeStringValue(void)
{
	CXMLNode *pStringRefNode;
	CXMLNode *pStringValueNode;

	pStringValueNode = new CXMLNode;
	pStringValueNode->setNodeType(XMLTag);
	pStringValueNode->setName("STRING_VALUE");

	while (*pPath != '\0')
	{
		//获取当前STRING_REF对应的节点
		pStringRefNode = getStringRef();
		if (pStringRefNode == NULL)
		{
			return NULL;
		}

		//加入本节点中
		pStringValueNode->addNode(pStringRefNode);

		//如果后面还有，则继续做下去
		if (*pPath == ',')
		{
			pPath++;
		}
		else
		{
			break;
		}
	}
	return pStringValueNode;
}

CXMLNode *CEXPathNode::getStringRef(void)
{
	int isProperty;
	CXMLNode *pStringRefNode;

	pStringRefNode = new CXMLNode;
	pStringRefNode->setNodeType(XMLTag);
	pStringRefNode->setName("STRING_REF");

	//先检查是属性还是文本
	if (*pPath == '@')
	{
		isProperty = 1;
	}
	else if (*pPath == '?')
	{
		isProperty = 0;
	}
	else
	{
		return NULL;
	}
	pPath++;
	pStringRefNode->addProperty("STRING_REF_TYPE", isProperty);

	//获取引用名
	char *refName = getToken();
	if (refName == NULL)
	{
		return NULL;
	}
	pStringRefNode->addProperty("NAME", refName);

	return pStringRefNode;
}

char *CEXPathNode::getToken(void)
{
	char *tail;
	char *result;
	int length;

	tail = pPath;
	while ((*tail != '\0') && (!strchr("/,=@?*", *tail)))
	{
		tail++;
	}

	length = (int)(tail - pPath);
	if (length == 0)
	{
		return NULL;
	}
	result = new char[length + 1];
	strncpy(result, pPath, length);
	result[length] = '\0';
	pPath = tail;
	return result;
}

CXMLDoc::CXMLDoc(void)
{
	m_rootNode = NULL;
	m_stringBuffer = NULL;
	m_lineNo = 1;
	m_lastChar = EOF;
}

CXMLDoc::~CXMLDoc(void)
{
	if (m_rootNode != NULL)
	{
		delete m_rootNode;
	}
}

int CXMLDoc::isA(char *objectType)
{
	if (!strcmp(objectType, "CXMLDoc"))
		return 1;
	return 0;
	//	return CBaseObject::isA(objectType);
}

const char *CXMLDoc::getType(void)
{
	return "CXMLDoc";
}

// void CXMLDoc::output(CLogger *pLogger,int indent)
// {
// 	// 直接转换 CStringPtr 类型变量m_version 等为 (char *) 有点不符号规范。
// 	// 即便它重载了 (char *)操作符，改为显式操作 
// 	//pLogger->output(indent,0,"<?xml version=\"%s\" encoding=\"%s\"?>",m_version,m_encoding);
// 	//pLogger->output(indent,0,"<!DOCTYPE %s SYSTEM \"%s\">",m_root,m_doctype);
// 	pLogger->output(indent,0,"<?xml version=\"%s\" encoding=\"%s\"?>",(char *)m_version,(char *)m_encoding);
// 	pLogger->output(indent,0,"<!DOCTYPE %s SYSTEM \"%s\">",(char *)m_root,(char *)m_doctype);
// 	if (m_rootNode!=NULL)
// 	{
// 		m_rootNode->output(pLogger,indent);
// 	}
// }

CXMLNode *CXMLDoc::getRootNode(void)
{
	return m_rootNode;
}

void CXMLDoc::setRootNode(CXMLNode *rootNode)
{
	m_rootNode = rootNode;
	m_root = rootNode->getName();
}

char *CXMLDoc::getEncoding(void)
{
	return m_encoding;
}

void CXMLDoc::setEncoding(char *encoding)
{
	m_encoding = encoding;
}

char *CXMLDoc::getVersion(void)
{
	return m_version;
}

void CXMLDoc::setVersion(char *version)
{
	m_version = version;
}

char *CXMLDoc::getRoot(void)
{
	return m_root;
}

void CXMLDoc::setRoot(char *root)
{
	m_root = root;
}

char *CXMLDoc::getDoctype(void)
{
	return m_doctype;
}

void CXMLDoc::setDoctype(char *doctype)
{
	m_doctype = doctype;
}

void CXMLDoc::read(char *filename, int ignoreText, bool ignoreComment)
{
	/*
	char *buffer;
	FILE *input;

	input=mfopen(filename,"rt");
	if (input==NULL)
	{
		throw new CParseError("Can not open file",0);
	}

	buffer=(char *)malloc(4*1024*1024);
	char *pBuffer=buffer;
	int ch;
	while ((ch=fgetc(input))!=EOF)
	{
		*pBuffer++=ch;
	}
	*pBuffer='\0';
	fclose(input);
	readString(buffer,ignoreText);
	*/

	FILE *input;

	input = mfopen(filename, "rt");
	if (input == NULL)
	{	// 增加输出错误信息，在打开配置文件出错的情况下，输出错误原因
		int errsv = errno;
		//		REPORT_EVENT(LOG_CRITICAL,"Can not open config file","FileName: %s,ErrorNo: %d,ErrMsg: %s",filename,errsv,strerror(errsv));
		//		throw new CParseError("Can not open file",0);
		printf("XML Can not open config file FileName: %s,ErrorNo: %d,ErrMsg: %s", filename, errsv, strerror(errsv));
		exit(-1);
	}
	read(input, ignoreText, ignoreComment);
	fclose(input);

}

void CXMLDoc::readString(char *string, int ignoreText)
{
	FILE *input = NULL;
	if (string == NULL)
	{
		throw new CParseError("Can not read null string", 0);
	}
	m_inputString = string;
	read(input, ignoreText);
}

void CXMLDoc::setUserData(void *data)
{
	m_userdata = data;
}

void CXMLDoc::readSax(FILE *input, StartElement start, EndElement end, int ignoreText)
{
	int depth = 0;
	bool shouldEnd = false;
	CXMLNode *thisNode;

	m_lineNo = 1;
	m_lastChar = EOF;
	while (getNextElement(input, ignoreText) != NULL) {
		if (shouldEnd) {
			throw new CParseError("Unexpected contents", m_lineNo);
		}
		switch (m_curElement.getElementType()) {
		case XMLStartElement:
			++depth;
			thisNode = new CXMLNode;
			thisNode->setNodeType(XMLTag);
			thisNode->setName(m_curElement.getName());
			thisNode->setPropertyList(m_curElement.getPropertyList());
			start(m_userdata, m_curElement.getName(), thisNode);
			break;

		case XMLSingleElement:
			if (depth == 0)
				shouldEnd = true;
			thisNode = new CXMLNode;
			thisNode->setNodeType(XMLTag);
			thisNode->setName(m_curElement.getName());
			thisNode->setPropertyList(m_curElement.getPropertyList());
			start(m_userdata, m_curElement.getName(), thisNode);
			end(m_userdata, m_curElement.getName());
			break;

		case XMLEndElement:
			if (depth == 0) {
				throw new CParseError("syntax error", m_lineNo);
			}
			if (--depth == 0)
				shouldEnd = true;
			m_curElement.removePropertyList();
			end(m_userdata, m_curElement.getName());
			break;

		case XMLTextElement:
			if (depth == 0) {
				throw new CParseError("syntax error", m_lineNo);
			}
			thisNode = new CXMLNode;
			thisNode->setNodeType(XMLText);
			thisNode->setName(m_curElement.getName());
			m_curElement.removePropertyList();
			start(m_userdata, m_curElement.getName(), thisNode);
			end(m_userdata, m_curElement.getName());
			break;

		default:
			m_curElement.removePropertyList();
			break;
		}
	}

	return;
}

void CXMLDoc::readSax2(KEY *pKey)
{
	FILE *input = pKey->input;
	StartElement start = pKey->start;
	EndElement2 end = pKey->end;
	int ignoreText = pKey->ignoreText;
	int *depth = &(pKey->depth);
	bool *shouldEnd = &(pKey->shouldEnd);
	CXMLNode *thisNode;
	bool bGo = true;

	while (bGo && getNextElement(input, ignoreText) != NULL) {
		if (*shouldEnd) {
			throw new CParseError("Unexpected contents", m_lineNo);
		}
		switch (m_curElement.getElementType()) {
		case XMLStartElement:
			++(*depth);
			thisNode = new CXMLNode;
			thisNode->setNodeType(XMLTag);
			thisNode->setName(m_curElement.getName());
			thisNode->setPropertyList(m_curElement.getPropertyList());
			start(m_userdata, m_curElement.getName(), thisNode);
			break;

		case XMLSingleElement:
			if (*depth == 0)
				*shouldEnd = true;
			thisNode = new CXMLNode;
			thisNode->setNodeType(XMLTag);
			thisNode->setName(m_curElement.getName());
			thisNode->setPropertyList(m_curElement.getPropertyList());
			start(m_userdata, m_curElement.getName(), thisNode);
			bGo = end(m_userdata, m_curElement.getName());
			break;

		case XMLEndElement:
			if (*depth == 0) {
				throw new CParseError("syntax error", m_lineNo);
			}
			if (--(*depth) == 0)
				*shouldEnd = true;
			m_curElement.removePropertyList();
			bGo = end(m_userdata, m_curElement.getName());
			break;

		case XMLTextElement:
			if (*depth == 0) {
				throw new CParseError("syntax error", m_lineNo);
			}
			thisNode = new CXMLNode;
			thisNode->setNodeType(XMLText);
			thisNode->setName(m_curElement.getName());
			m_curElement.removePropertyList();
			start(m_userdata, m_curElement.getName(), thisNode);
			bGo = end(m_userdata, m_curElement.getName());
			break;

		default:
			m_curElement.removePropertyList();
			break;
		}
	}

	return;
}

void CXMLDoc::read(FILE *input, int ignoreText, bool ignoreComment)
{
	CXMLPropertyList *pPropertyList;
	int shouldEnd = 0;
	int isRoot;
	stack<CXMLNode *> nodeStack;
	CXMLNode *thisNode;

	m_lineNo = 1;
	m_lastChar = EOF;
	while (getNextElement(input, ignoreText, ignoreComment) != NULL)
	{
		//m_curElement.write(stdout);
		//printf("\n");
		if (shouldEnd)
		{
			if (m_curElement.getElementType() == XMLCommentElement)
			{
				///后面还有注释，那就算了
				m_curElement.removePropertyList();
				continue;
			}
			throw new CParseError("Unexpected contents", m_lineNo);
		}
		switch (m_curElement.getElementType())
		{
		case XMLStartElement:
		case XMLSingleElement:
			if (nodeStack.size() == 0)		//应当是根节点
			{
				if (getRoot()[0] == '\0')
				{
					//没有定义过根节点
					setRoot(m_curElement.getName());
				}
				else if (strcmp(m_curElement.getName(), getRoot()))
				{
					char * dde = getRoot();
					//根节点不匹配
					throw new CParseError("Root node mismatch", m_lineNo);
				}
				isRoot = 1;
			}
			else
			{
				isRoot = 0;
			}
			thisNode = new CXMLNode;
			thisNode->setNodeType(XMLTag);
			thisNode->setName(m_curElement.getName());
			thisNode->setPropertyList(m_curElement.getPropertyList());
			if (isRoot)
			{
				m_rootNode = thisNode;
			}
			else
			{
				nodeStack.top()->addNode(thisNode);
			}
			if (m_curElement.getElementType() == XMLStartElement)
			{
				nodeStack.push(thisNode);
			}
			else
			{
				if (isRoot)
				{
					shouldEnd = 1;
				}
			}
			continue;
		case XMLEndElement:
			if (nodeStack.size() == 0)
			{
				throw new CParseError("syntax error", m_lineNo);
			}
			if (strcmp(nodeStack.top()->getName(), m_curElement.getName()))
			{
				char buffer[1024];
				sprintf(buffer, "end tag mismatch, expecting %s", nodeStack.top()->getName());
				throw new CParseError(buffer, m_lineNo);
			}
			nodeStack.pop();
			if (nodeStack.size() == 0)
			{
				shouldEnd = 1;
			}
			m_curElement.removePropertyList();
			continue;
		case XMLTextElement:
			if (nodeStack.size() == 0)
			{
				throw new CParseError("syntax error", m_lineNo);
			}
			thisNode = new CXMLNode;
			thisNode->setNodeType(XMLText);
			thisNode->setName(m_curElement.getName());
			nodeStack.top()->addNode(thisNode);
			m_curElement.removePropertyList();
			continue;
		case XMLDocumentElement:
			setVersion(m_curElement.findProperty("version"));
			setEncoding(m_curElement.findProperty("encoding"));
			m_curElement.removePropertyList();
			continue;
		case XMLDTDElement:
			if (!strcmp(m_curElement.getName(), "DOCTYPE"))
			{
				pPropertyList = m_curElement.getPropertyList();
				setRoot(pPropertyList->at(0)->getName());
				if (!strcmp(pPropertyList->at(1)->getName(), "SYSTEM"))
				{
					setDoctype(pPropertyList->at(2)->getName());
				}
				else
				{
					setDoctype(pPropertyList->at(3)->getName());
				}
			}
			m_curElement.removePropertyList();
			continue;
		case XMLCommentElement:
			if ((!ignoreComment) && (nodeStack.size() != 0))
			{
				//如果不忽略注释，而且已经有任何节点，那就需要此注释
				thisNode = new CXMLNode;
				thisNode->setNodeType(XMLComment);
				thisNode->setName(m_curElement.getName());
				thisNode->setPropertyList(m_curElement.getPropertyList());
				nodeStack.top()->addNode(thisNode);
			}
			m_curElement.removePropertyList();
			continue;
		case XMLPIElement:
		default:
			m_curElement.removePropertyList();
			continue;
		}
	}
	return;
}

CXMLElement *CXMLDoc::getNextElement(FILE *input, int ignoreText, bool ignoreComment)
{
	while (getPureNextElement(input) != NULL)
	{
		if (ignoreText && (m_curElement.getElementType() == XMLTextElement))
		{
			m_curElement.removePropertyList();
			continue;
		}
		if (ignoreComment && (m_curElement.getElementType() == XMLCommentElement))
		{
			m_curElement.removePropertyList();
			continue;
		}
		if (m_curElement.getElementType() == XMLPIElement)
		{
			m_curElement.removePropertyList();
			continue;
		}
		return &m_curElement;
	}
	return NULL;
}

CXMLElement *CXMLDoc::getPureNextElement(FILE *input)
{
	char *name = NULL;
	char *pString = NULL;
	int ch;
	ch = ignoreSpace(input, 1);
	if (ch == EOF)
		return NULL;
	if (ch == '<')
	{
		ch = getNextChar(input);
		if (ch == '?')		///XMLDocumentElement、XMLPIElement
		{
			m_curElement.setName(name = getNextName(input));
			m_curElement.setPropertyList(getNextPropertyList(input));
			ch = ignoreSpace(input);
			expectChar(ch, '?');
			if (!strcmp(m_curElement.getName(), "xml"))	///XMLDocumentElement
			{
				m_curElement.setElementType(XMLDocumentElement);
			}
			else						///XMLPIElement
			{
				m_curElement.setElementType(XMLPIElement);
			}
			ch = getNextChar(input);
		}
		else if (ch == '!')	///XMLCommentElement、XMLDTDElement
		{
			ch = getNextChar(input);
			if (ch == '-')					///XMLCommentElement
			{
				ch = getNextChar(input);
				m_curElement.setName(pString = getString(input, "--"));
				m_curElement.setPropertyList(NULL);
				m_curElement.setElementType(XMLCommentElement);
				ch = getNextChar(input);
			}
			else						///XMLDTDElement
			{
				ungetLastChar(input);
				m_curElement.setName(name = getNextName(input));
				if (strcmp(m_curElement.getName(), "DOCTYPE"))
				{
					///以后将需要增加对各种DTD类型的支持
					throw new CParseError("expecting DOCTYPE", m_lineNo);
				}
				m_curElement.setPropertyList(getNextPropertyList(input));
				m_curElement.setElementType(XMLDTDElement);
				ch = ignoreSpace(input);
			}
		}
		else if (ch == '/')	///XMLEndElement
		{
			m_curElement.setName(name = getNextName(input));
			m_curElement.setPropertyList(NULL);
			m_curElement.setElementType(XMLEndElement);
			ch = ignoreSpace(input);
		}
		else			///XMLStartElement、XMLSingleElement
		{
			ungetLastChar(input);
			m_curElement.setName(name = getNextName(input));
			m_curElement.setPropertyList(getNextPropertyList(input));
			ch = ignoreSpace(input);
			if (ch == '/')		///XMLSingleElement
			{
				m_curElement.setElementType(XMLSingleElement);
				ch = getNextChar(input);
			}
			else			///XMLStartElement
			{
				m_curElement.setElementType(XMLStartElement);
			}
		}
		expectChar(ch, '>');
	}
	else				///XMLTextElement
	{
		ungetLastChar(input);
		m_curElement.setName(pString = getString(input, "<"));
		ungetLastChar(input);
		m_curElement.setPropertyList(NULL);
		m_curElement.setElementType(XMLTextElement);
	}
	if (name != NULL)
	{
		// delete name;
		// 内存空间是由 malloc 分配，释放时用 free 比较规范
		free(name);
	}
	if (pString != NULL)
	{
		//delete pString;
		//内存空间是由 malloc 分配，释放时用 free 比较规范
		free(pString);
	}
	return &m_curElement;
}

int CXMLDoc::getNextChar(FILE *input, int allowEOF)
{
	if (input != NULL)
	{
		///从文件读取的情况
		m_lastChar = fgetc(input);
		if (m_lastChar == '\n')
		{
			m_lineNo++;
		}
		else if (m_lastChar == EOF)
		{
			if (!allowEOF)
			{
				throw new CParseError("unexpected end of file", m_lineNo);
			}
		}
		return m_lastChar;
	}
	else
	{
		///从字符串读取的情况
		m_lastChar = *m_inputString;
		if (m_lastChar == '\0')
		{
			if (!allowEOF)
			{
				throw new CParseError("unexpected end of file", m_lineNo);
			}
			else
			{
				m_lastChar = EOF;
			}
		}
		else
		{
			if (m_lastChar == '\n')
			{
				m_lineNo++;
			}
			m_inputString++;
		}
		return m_lastChar;
	}
}

int CXMLDoc::ignoreSpace(FILE *input, int allowEOF)
{
	while (isspace(getNextChar(input, allowEOF)))
	{
	}
	return m_lastChar;
}

void CXMLDoc::CodeUTF8ToANSIConverter()
{
	char strOut[1001];
	int OutLen = sizeof(strOut) - 1;
	memset(strOut, 0, sizeof(strOut));

#ifdef WINDOWS
	int unicodeLen = MultiByteToWideChar(CP_UTF8, 0, m_stringBuffer, -1, NULL, 0);
	wchar_t* pUnicode;
	pUnicode = new wchar_t[unicodeLen + 1];
	memset(pUnicode, 0, (unicodeLen + 1) * sizeof(wchar_t));
	MultiByteToWideChar(CP_UTF8, 0, m_stringBuffer, -1, (LPWSTR)pUnicode, unicodeLen);

	char * pTargetData = NULL;
	int targetLen = WideCharToMultiByte(CP_ACP, 0, (LPWSTR)pUnicode, -1, (char *)pTargetData, 0, NULL, NULL);
	pTargetData = new char[targetLen + 1];
	memset(pTargetData, 0, targetLen + 1);
	WideCharToMultiByte(CP_ACP, 0, (LPWSTR)pUnicode, -1, (char *)pTargetData, targetLen, NULL, NULL);

	lstrcpyn(strOut, (char*)pTargetData, OutLen);
	delete pUnicode;
	delete pTargetData;

	strcpy(m_stringBuffer, strOut);
	/*#else
		iconv_t cd = iconv_open("UTF-8","");
		char *inbuf = m_stringBuffer;
		int InLen=strlen(m_stringBuffer);
		char *outbuf = strOut;
		iconv(cd, &inbuf,(size_t*)&InLen,&outbuf, (size_t*)&OutLen);
		iconv_close(cd);

		strcpy(m_stringBuffer, strOut);*/
#endif
}

///------
///	xuzh：20070808：修正了行号处理上的问题
///------
void CXMLDoc::ungetLastChar(FILE *input)
{
	if (m_lastChar == EOF)
	{
		return;
	}
	if (input != NULL)
	{
		///读文件的情况
		ungetc(m_lastChar, input);
		if (m_lastChar == '\n')
		{
			m_lineNo--;
		}
	}
	else
	{
		///读字符串的情况
		if (*m_inputString == '\n')
		{
			m_lineNo--;
		}
		m_inputString--;
	}
	m_lastChar = EOF;
}

void CXMLDoc::expectChar(int ch, int expectingChar)
{
	if (ch == expectingChar)
	{
		return;
	}
	char msg[20];
	sprintf(msg, "expecting \'%c\'", expectingChar);
	throw new CParseError(msg, m_lineNo);
}

char *CXMLDoc::getNextName(FILE *input, int allowValue)
{
	int ch;

	ch = ignoreSpace(input);
	if ((ch == '\"') || (ch == '\''))
	{
		char endString[2];

		if (!allowValue)
		{
			throw new CParseError("syntax error", m_lineNo);
		}
		sprintf(endString, "%c", ch);
		return getString(input, endString);
	}
	if (!isalpha(ch) && !strchr("_:", ch))
	{
		ungetLastChar(input);
		return NULL;
	}
	makeNewString();
	appendChar(ch);
	for (;;)
	{
		ch = getNextChar(input);
		if (!isalnum(ch) && !strchr(".-_:", ch))
		{
			break;
		}
		appendChar(ch);
	}
	ungetLastChar(input);
	return getString();
}

char *CXMLDoc::getString(FILE *input, char *endString)
{
	///这里只是一个简单的实现，在发现不符合endString时，简单地回到开始处继续检查。
	///实际应当按照KMP算法来确定回退的个数。但是，在本应用中，情况比较简单，不需要
	///使用KMP算法
	char *checkString;
	int ch;

	checkString = endString;
	makeNewString();
	for (;;)
	{
		ch = getNextChar(input);
		if (*checkString == ch)		//本字符符合预期的结束字符串
		{
			checkString++;
			if (*checkString == '\0')		//已经到了结束字符串的最后位置
			{
				return getTransferedString();
			}
			else				//还需要检查后面的字符
			{
				continue;
			}
		}
		else				//本字符不符合预期的结束字符串
		{
			//先将已经检查符合的部分结束字符串重新加入结果中
			char *p;
			p = endString;
			while (p != checkString)
			{
				appendChar(*p);
				p++;
			}
			checkString = endString;

			//再将本字符加入结果中
			appendChar(ch);
		}
	}
}

CXMLPropertyList *CXMLDoc::getNextPropertyList(FILE *input)
{
	CXMLPropertyList *pPropertyList;
	char *name, *value;
	int ch;

	pPropertyList = new CXMLPropertyList;
	for (;;)
	{
		name = getNextName(input, 1);
		if (name == NULL)
		{
			break;
		}
		ch = ignoreSpace(input);
		if (ch == '=')		//有值的情况
		{
			value = getNextName(input, 1);
			if (value == NULL)
			{
				throw new CParseError("expecting property value", m_lineNo);
			}
			pPropertyList->addProperty(name, value);
			//delete name;
			//delete value;
			//内存空间是由 malloc 分配，释放时用 free 比较规范
			free(name);
			free(value);
		}
		else			//无值的情况
		{
			char *p = NULL;
			ungetLastChar(input);
			pPropertyList->addProperty(name, p);
			//delete name;
			//内存空间是由 malloc 分配，释放时用 free 比较规范
			free(name);
		}
	}
	return pPropertyList;
}

void CXMLDoc::makeNewString(void)
{
	/*
	if (m_stringBuffer!=NULL)
	{
		free(m_stringBuffer);
	}
	*/
	m_stringBuffer = (char *)malloc(500);
	m_curLength = 0;
	m_curBufferLength = 500;
	if (m_stringBuffer == NULL)
	{
		throw new CParseError("Insufficient memory space", m_lineNo);
	}
}

void CXMLDoc::appendChar(int ch)
{
	if (m_curLength == m_curBufferLength - 1)
	{
		m_stringBuffer = (char *)realloc(m_stringBuffer, m_curBufferLength * 2);
		if (m_stringBuffer == NULL)
		{
			throw new CParseError("Insufficient memory space", m_lineNo);
		}
		m_curBufferLength *= 2;
	}
	m_stringBuffer[m_curLength++] = ch;
}

char *CXMLDoc::getString(void)
{
	appendChar('\0');

	if (strcmp(getEncoding(), "UTF-8") == 0)
	{
		CodeUTF8ToANSIConverter();
	}

	return m_stringBuffer;
}

char *CXMLDoc::getTransferedString(void)
{
	char *from, *to;
	char entityBuffer[128];			//假设不会有太长的entity
	char *pEntity;
	int entityCh;

	from = getString();
	to = m_stringBuffer;

	while (*from != '\0')
	{
		if (*from == '&')			//是entity
		{
			from++;
			pEntity = entityBuffer;
			while (!strchr("; \t\r\n", *from))
			{
				*pEntity++ = *from++;
			}
			if (*from == ';')
			{
				from++;
			}
			*pEntity = '\0';
			entityCh = getEntityChar(entityBuffer);
			if (entityCh == EOF)
			{
				static char msg[150];
				sprintf(msg, "unknown entity &%s;", entityBuffer);
				throw new CParseError(msg, m_lineNo);
			}
			*to++ = entityCh;
		}
		else
		{
			*to++ = *from++;
		}
	}
	*to = '\0';
	return m_stringBuffer;
}

int CXMLDoc::write(char *filename)
{
	FILE *output;
	int result;

	output = mfopen(filename, "wt");
	if (output == NULL)
	{
		return 0;
	}
	result = write(output);
	fclose(output);
	return result;
}

int CXMLDoc::write(FILE *output)
{
	writeHead(output);
	if (m_rootNode != NULL)
	{
		return m_rootNode->write(output, 0);
	}
	return 1;
}

int CXMLDoc::writeHead(FILE *output)
{
	fprintf(output, "<?xml version=\"%s\" encoding=\"%s\"?>\n", getVersion(), getEncoding());
	fprintf(output, "<!DOCTYPE %s SYSTEM \"%s\">\n", getRoot(), getDoctype());
	return 1;
}

CParseError::CParseError(char *msg, int lineNo)
{
	m_msg = strdup(msg);
	m_lineNo = lineNo;
}

CParseError::~CParseError(void)
{
}

int CParseError::isA(char *objectType)
{
	if (!strcmp(objectType, "CParseError"))
	{
		return 1;
	}
	return 0;
	//	return CBaseObject::isA(objectType);
}

const char *CParseError::getType(void)
{
	return "CParseError";
}

// void CParseError::output(CLogger *pLogger,int indent)
// {
// 	pLogger->output(indent,0,"CParseError %s at %d",m_msg,m_lineNo);
// }

char *CParseError::getMsg(void)
{
	return m_msg;
}

int CParseError::getLineNo(void)
{
	return m_lineNo;
}
