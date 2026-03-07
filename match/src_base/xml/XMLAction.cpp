/////////////////////////////////////////////////////////////////////////
///@file XMLAction.h
///@brief 实现了几个常用的XML操作类
/////////////////////////////////////////////////////////////////////////
#include "CXML.h"
#include "XMLAction.h"

/////////////////////////////////////////////////////////////////////////
///CXMLDisplayAction的实现
/////////////////////////////////////////////////////////////////////////

CXMLUpdateAction::CXMLUpdateAction(CXMLNode *pNode)
{
	m_pNode=pNode;
}

int CXMLUpdateAction::nodeHandler(CXMLNode *pNode)
{
	if ( m_pNode != NULL )
	{
		if (strcmp(m_pNode->getName(),""))
			pNode->setName(m_pNode->getName());
		
		CXMLPropertyList *pList=m_pNode->getPropertyList();
		if (pList!=NULL)
		{
			CXMLPropertyList::iterator it=pList->begin();
			for (int i=0;i<pList->size();i++)
			{
				pNode->setProperty(pList->at(i)->getName(),pList->at(i)->getValue());
			}
		}
		
		CXMLNodeList *pNodeList=m_pNode->getNodeList();
		if (pNodeList!=NULL)
		{
			pNode->setNodeList(pNodeList);
		}
	}
	return 1;
}

int CXMLUpdateAction::stringHandler(char *string)
{
	return 1;
}


/////////////////////////////////////////////////////////////////////////
///CXMLDisplayAction的实现
/////////////////////////////////////////////////////////////////////////

CXMLDisplayAction::CXMLDisplayAction(FILE *output)
{
	m_output=output;
}

int CXMLDisplayAction::nodeHandler(CXMLNode *pNode)
{
	pNode->write(m_output);
	return 1;
}

int CXMLDisplayAction::stringHandler(char *string)
{
	fprintf(m_output,"%s\n",string);
	return 1;
}

/////////////////////////////////////////////////////////////////////////
///CXMLListAction的实现
/////////////////////////////////////////////////////////////////////////

int CXMLListAction::nodeHandler(CXMLNode *pNode)
{
	nodeList.push_back(pNode);
	return 1;
}

int CXMLListAction::stringHandler(char *string)
{
	stringList.push_back(string);
	return 1;
}
	
vector<char *> *CXMLListAction::getStringList(void)
{
	return &stringList;
}
	
CXMLNodeList *CXMLListAction::getNodeList(void)
{
	return &nodeList;
}

void CXMLListAction::clearAll(void)
{
	nodeList.clear();
	stringList.clear();
}

/////////////////////////////////////////////////////////////////////////
///CXMLEqualCheckerAction的实现
/////////////////////////////////////////////////////////////////////////

int CXMLEqualCheckerAction::nodeHandler(CXMLNode *pNode)
{
	CXMLNodeList::iterator it;
	
	for (it=nodeList.begin();it<nodeList.end();it++)
	{
		if (isEqual(pNode,*it))
		{
			return findDuplication(pNode,*it);
		}
	}
	return CXMLListAction::nodeHandler(pNode);
}

int CXMLEqualCheckerAction::stringHandler(char *string)
{
	vector<char *>::iterator it;
	
	for (it=stringList.begin();it<stringList.end();it++)
	{
		if (isEqual(string,*it))
		{
			return findDuplication(string,*it);
		}
	}
	return CXMLListAction::stringHandler(string);
}

int CXMLEqualCheckerAction::isEqual(char *string1, char *string2)
{
	if (!strcmp(string1,string2))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

int CXMLEqualCheckerAction::isEqual(CXMLNode *pNode1, CXMLNode *pNode2)
{
	if (pNode1==pNode2)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

int CXMLEqualCheckerAction::findDuplication(char *string, char *originalString)
{
	return 1;
}

int CXMLEqualCheckerAction::findDuplication(CXMLNode *pNode, CXMLNode *pOriginalNode)
{
	return 1;
}
