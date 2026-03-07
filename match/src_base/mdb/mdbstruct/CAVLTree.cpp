#include "CAVLTree.h"

CAVLTree::CAVLTree(int maxTradeUnit, TCompareFunc compareFunc, IMemoryAllocator *pAllocator, bool reuse, int reuseID, int readOnly)
{
	this->compareFunc = compareFunc;
	pIndexMem = new CFixMem(sizeof(CAVLNode), maxTradeUnit + 1, pAllocator, reuse, reuseID, readOnly);
	if (pIndexMem == NULL)
		EMERGENCY_EXIT("Insufficient memory space");
	if (!reuse)
	{
		pIndexMem->alloc();
		setRoot(NULL);
	}
	else
	{
		if (pIndexMem->getCount() == 0)
			EMERGENCY_EXIT("Reuse invalid index");
	}
}

CAVLTree::~CAVLTree(void)
{
	CHECK_TYPE("CAVLTree");
	delete pIndexMem;
}

int CAVLTree::isA(char *objectType)
{
	if (!strcmp(objectType, "CAVLTree"))
		return 1;
	return CBaseObject::isA(objectType);
}

const char *CAVLTree::getType(void)
{
	return "CAVLTree";
}

//void CAVLTree::output(CLogger *pLogger,int indent)
//{
//	CHECK_TYPE("CAVLTree");
//	pLogger->output(indent,0,"CAVLTree");
//	pIndexMem->output(pLogger,indent+1);
//}

#define LEFT_DEPTH(pNode) (pNode->left==NULL?0:pNode->left->depth)
#define RIGHT_DEPTH(pNode) (pNode->right==NULL?0:pNode->right->depth)
#define EVAL_DEPTH(pNode) (pNode->depth=MAX(LEFT_DEPTH(pNode),RIGHT_DEPTH(pNode))+1)
#define SET_LEFT(pNode1,pNode2)         { pNode1->left=pNode2;  if (pNode2!=NULL) pNode2->father=pNode1; }
#define SET_RIGHT(pNode1,pNode2)        { pNode1->right=pNode2; if (pNode2!=NULL) pNode2->father=pNode1; }
#define SET_FATHER(pNode1,pNode2,oldSon) { pNode1->father=pNode2; if (pNode2!=NULL) { if (pNode2->left==oldSon) pNode2->left=pNode1; else pNode2->right=pNode1;        } else  setRoot(pNode1);}

///We can not use defines below because an compiler error for GNU on VOS
/*
#define SET_LEFT(pNode1,pNode2)			\
	{					\
		pNode1->left=pNode2;		\
		if (pNode2!=NULL) 		\
			pNode2->father=pNode1;	\
	}
#define SET_RIGHT(pNode1,pNode2)		\
	{					\
		pNode1->right=pNode2;		\
		if (pNode2!=NULL) 		\
			pNode2->father=pNode1;	\
	}
#define SET_FATHER(pNode1,pNode2,oldSon) 		\
	{						\
		pNode1->father=pNode2;			\
		if (pNode2!=NULL)			\
		{					\
			if (pNode2->left==oldSon)	\
				pNode2->left=pNode1;	\
			else				\
				pNode2->right=pNode1;	\
		}					\
		else					\
			setRoot(pNode1);			\
	}
*/

void CAVLTree::setRoot(CAVLNode *pNode)
{
	CAVLNode *pHead = ((CAVLNode *)(pIndexMem->getObject(0)));
	pHead->father = pNode;
	///pIndexMem->updateObject(pHead);
}

CAVLNode *CAVLTree::getRoot(void)
{
	return ((CAVLNode *)(pIndexMem->getObject(0)))->father;
}

CAVLNode *CAVLTree::addObject(const void *pObject)
{
	CAVLNode *newNode, *thisNode, *lastNode;
	int isLeft;

	newNode = (CAVLNode *)(pIndexMem->alloc());
	newNode->pObject = pObject;
	newNode->left = NULL;
	newNode->right = NULL;
	newNode->depth = 1;

	thisNode = getRoot();
	if (thisNode == NULL)
	{
		SET_FATHER(newNode, getRoot(), NULL);
		return newNode;
	}

	while (thisNode != NULL)
	{
		lastNode = thisNode;
		if (compareFunc(pObject, thisNode->pObject) < 0)
		{
			thisNode = thisNode->left;
			isLeft = 1;
		}
		else
		{
			thisNode = thisNode->right;
			isLeft = 0;
		}
	}
	newNode->father = lastNode;
	if (isLeft)
		lastNode->left = newNode;
	else
		lastNode->right = newNode;

	alterTree(lastNode);

	return newNode;
}

void CAVLTree::alterTree(CAVLNode *thisNode)
{
	while (thisNode != NULL)
	{
		int leftDepth, rightDepth, thisDepth;
		CAVLNode *lTree, *llTree, *lrTree;
		CAVLNode *rTree, *rlTree, *rrTree;
		CAVLNode *fatherNode;

		leftDepth = LEFT_DEPTH(thisNode);
		rightDepth = RIGHT_DEPTH(thisNode);
		fatherNode = thisNode->father;
		if (leftDepth - rightDepth >= 2)
		{
			rTree = thisNode->right;
			lTree = thisNode->left;
			lrTree = lTree->right;
			llTree = lTree->left;
			if (LEFT_DEPTH(lTree) >= RIGHT_DEPTH(lTree))
			{
				///LL Type
				SET_LEFT(thisNode, lrTree);
				SET_RIGHT(lTree, thisNode);
				SET_FATHER(lTree, fatherNode, thisNode);
				EVAL_DEPTH(thisNode);
				EVAL_DEPTH(lTree);
				thisNode = fatherNode;
			}
			else
			{
				CAVLNode *lrrTree, *lrlTree;

				///LR Type
				lrrTree = lrTree->right;
				lrlTree = lrTree->left;
				SET_LEFT(thisNode, lrrTree);
				SET_RIGHT(lTree, lrlTree);
				SET_RIGHT(lrTree, thisNode);
				SET_LEFT(lrTree, lTree);
				SET_FATHER(lrTree, fatherNode, thisNode);
				EVAL_DEPTH(thisNode);
				EVAL_DEPTH(lTree);
				EVAL_DEPTH(lrTree);
				thisNode = fatherNode;
			}
		}
		else if (rightDepth - leftDepth >= 2)
		{
			lTree = thisNode->left;
			rTree = thisNode->right;
			rlTree = rTree->left;
			rrTree = rTree->right;
			if (RIGHT_DEPTH(rTree) >= LEFT_DEPTH(rTree))
			{
				///RR Type
				SET_RIGHT(thisNode, rlTree);
				SET_LEFT(rTree, thisNode);
				SET_FATHER(rTree, fatherNode, thisNode);
				EVAL_DEPTH(thisNode);
				EVAL_DEPTH(rTree);
				thisNode = fatherNode;
			}
			else
			{
				CAVLNode *rllTree, *rlrTree;

				///RL Type
				rllTree = rlTree->left;
				rlrTree = rlTree->right;
				SET_RIGHT(thisNode, rllTree);
				SET_LEFT(rTree, rlrTree);
				SET_LEFT(rlTree, thisNode);
				SET_RIGHT(rlTree, rTree);
				SET_FATHER(rlTree, fatherNode, thisNode);
				EVAL_DEPTH(thisNode);
				EVAL_DEPTH(rTree);
				EVAL_DEPTH(rlTree);
				thisNode = fatherNode;
			}
		}
		else
		{
			thisDepth = MAX(leftDepth, rightDepth) + 1;
			if (thisNode->depth == thisDepth)
				return;
			else
			{
				thisNode->depth = thisDepth;
				thisNode = fatherNode;
			}
		}
	}
}

CAVLNode *CAVLTree::getSmallest(void)
{
	CAVLNode *thisNode;

	if (getRoot() == NULL)
		return NULL;
	thisNode = getRoot();
	while (thisNode->left != NULL)
		thisNode = thisNode->left;
	return thisNode;
}

CAVLNode *CAVLTree::getLargest(void)
{
	CAVLNode *thisNode;

	if (getRoot() == NULL)
		return NULL;
	thisNode = getRoot();
	while (thisNode->right != NULL)
		thisNode = thisNode->right;
	return thisNode;
}

CAVLNode *CAVLTree::searchFirstGreatEqual(const void *pObject)
{
	CAVLNode *thisNode, *lastNode;

	thisNode = getRoot();
	lastNode = NULL;
	while (thisNode != NULL)
	{
		switch (compareFunc(thisNode->pObject, pObject))
		{
		case 1:
			lastNode = thisNode;
			thisNode = thisNode->left;
			break;
		case -1:
			thisNode = thisNode->right;
			break;
		case 0:
			lastNode = thisNode;
			thisNode = thisNode->left;
			break;
		default:
			EMERGENCY_EXIT("Invalid return value of compare function");
		}
	}
	return lastNode;
}

CAVLNode *CAVLTree::searchFirstGreat(const void *pObject)
{
	CAVLNode *thisNode, *lastNode;

	thisNode = getRoot();
	lastNode = NULL;
	while (thisNode != NULL)
	{
		switch (compareFunc(thisNode->pObject, pObject))
		{
		case 1:
			lastNode = thisNode;
			thisNode = thisNode->left;
			break;
		case -1:
			thisNode = thisNode->right;
			break;
		case 0:
			thisNode = thisNode->right;
			break;
		default:
			EMERGENCY_EXIT("Invalid return value of compare function");
		}
	}
	return lastNode;
}

CAVLNode *CAVLTree::searchLastLessEqual(const void *pObject)
{
	CAVLNode *thisNode, *lastNode;

	thisNode = getRoot();
	lastNode = NULL;
	while (thisNode != NULL)
	{
		switch (compareFunc(thisNode->pObject, pObject))
		{
		case 1:
			thisNode = thisNode->left;
			break;
		case -1:
			lastNode = thisNode;
			thisNode = thisNode->right;
			break;
		case 0:
			lastNode = thisNode;
			thisNode = thisNode->right;
			break;
		default:
			EMERGENCY_EXIT("Invalid return value of compare function");
		}
	}
	return lastNode;
}

CAVLNode *CAVLTree::searchLastLess(const void *pObject)
{
	CAVLNode *thisNode, *lastNode;

	thisNode = getRoot();
	lastNode = NULL;
	while (thisNode != NULL)
	{
		switch (compareFunc(thisNode->pObject, pObject))
		{
		case 1:
			thisNode = thisNode->left;
			break;
		case -1:
			lastNode = thisNode;
			thisNode = thisNode->right;
			break;
		case 0:
			thisNode = thisNode->left;
			break;
		default:
			EMERGENCY_EXIT("Invalid return value of compare function");
		}
	}
	return lastNode;
}

CAVLNode *CAVLTree::searchFirstEqual(const void *pObject)
{
	CAVLNode *thisNode, *lastNode;
	bool found = false;

	thisNode = getRoot();
	lastNode = NULL;
	while (thisNode != NULL)
	{
		switch (compareFunc(thisNode->pObject, pObject))
		{
		case 1:
			thisNode = thisNode->left;
			break;
		case -1:
			thisNode = thisNode->right;
			break;
		case 0:
			lastNode = thisNode;
			thisNode = thisNode->left;
			found = true;
			break;
		default:
			EMERGENCY_EXIT("Invalid return value of compare function");
		}
	}
	if (found)
		return lastNode;
	else
		return NULL;
}

CAVLNode *CAVLTree::searchLastEqual(const void *pObject)
{
	CAVLNode *thisNode, *lastNode;

	thisNode = getRoot();
	lastNode = NULL;
	while (thisNode != NULL)
	{
		switch (compareFunc(thisNode->pObject, pObject))
		{
		case 1:
			thisNode = thisNode->left;
			break;
		case -1:
			thisNode = thisNode->right;
			break;
		case 0:
			lastNode = thisNode;
			thisNode = thisNode->right;
			break;
		default:
			EMERGENCY_EXIT("Invalid return value of compare function");
		}
	}
	return lastNode;
}

CAVLNode *CAVLTree::getNextNode(CAVLNode *pNode)
{
	CAVLNode *thisNode, *fatherNode;

	if (pNode->right != NULL)
	{
		///right tree exist, find the leftmost node in the right tree
		thisNode = pNode->right;
		while (thisNode->left != NULL)
			thisNode = thisNode->left;
		return thisNode;
	}
	///find the recent ascend, whose left tree contain this node
	thisNode = pNode;
	fatherNode = thisNode->father;
	while (fatherNode != NULL)
	{
		if (fatherNode->left == thisNode)
			return fatherNode;
		thisNode = fatherNode;
		fatherNode = thisNode->father;
	}
	return NULL;
}

CAVLNode *CAVLTree::getPrevNode(CAVLNode *pNode)
{
	CAVLNode *thisNode, *fatherNode;

	if (pNode->left != NULL)
	{
		///left tree exist, find the rightmost node in the left tree
		thisNode = pNode->left;
		while (thisNode->right != NULL)
			thisNode = thisNode->right;
		return thisNode;
	}
	///find the recent ascend, whose right tree contain this node
	thisNode = pNode;
	fatherNode = thisNode->father;
	while (fatherNode != NULL)
	{
		if (fatherNode->right == thisNode)
			return fatherNode;
		thisNode = fatherNode;
		fatherNode = thisNode->father;
	}
	return NULL;
}

CAVLNode *CAVLTree::findObject(const void *pObject)
{
	CAVLNode *thisNode = searchFirstEqual(pObject);
	for (;;)
	{
		if (thisNode == NULL)
			return NULL;
		if (thisNode->pObject == pObject)
			return thisNode;
		if (compareFunc(thisNode->pObject, pObject) != 0)
			return NULL;
		thisNode = getNextNode(thisNode);
	}
}

/*
void CAVLTree::removeNode(CAVLNode *pNode)
{
	CAVLNode *thisNode,*fatherNode,*targetNode;
	int leftDepth,rightDepth;

	thisNode=pNode;
	fatherNode=thisNode->father;
	while (thisNode->depth!=1)
	{
		leftDepth=LEFT_DEPTH(thisNode);
		rightDepth=RIGHT_DEPTH(thisNode);
		if (leftDepth>rightDepth)
		{
			///find the rightmost in the left tree
			targetNode=thisNode->left;
			while (targetNode->right!=NULL)
				targetNode=targetNode->right;
			///move object
			thisNode->pObject=targetNode->pObject;
			thisNode=targetNode;
		}
		else
		{
			///find the leftmost in the right tree
			targetNode=thisNode->right;
			while (targetNode->left!=NULL)
				targetNode=targetNode->left;
			///move object
			thisNode->pObject=targetNode->pObject;
			thisNode=targetNode;
		}
		fatherNode=thisNode->father;
	}
	if (fatherNode!=NULL)
	{
		if (fatherNode->left==thisNode)
			fatherNode->left=NULL;
		else
			fatherNode->right=NULL;
	}
	else
		setRoot(NULL);
	if(thisNode != pNode)

	alterTree(fatherNode);
	pIndexMem->free(thisNode);
} */

void CAVLTree::removeNode(CAVLNode *pNode)
{
	CAVLNode *thisNode, *fatherNode, *targetNode;
	int leftDepth, rightDepth;

	thisNode = pNode;
	fatherNode = thisNode->father;
	if (thisNode->depth != 1)
	{
		leftDepth = LEFT_DEPTH(thisNode);
		rightDepth = RIGHT_DEPTH(thisNode);
		if (leftDepth > rightDepth)
		{
			//find the rightmost in the left tree
			targetNode = thisNode->left;
			while (targetNode->right != NULL)
				targetNode = targetNode->right;
		}
		else
		{
			//find the leftmost in the right tree
			targetNode = thisNode->right;
			while (targetNode->left != NULL)
				targetNode = targetNode->left;
		}

		//move object
		fatherNode = targetNode->father;
		if (thisNode->left == targetNode)
		{
			SET_RIGHT(targetNode, thisNode->right);
			fatherNode = targetNode;
		}
		else if (thisNode->right == targetNode)
		{
			SET_LEFT(targetNode, thisNode->left);
			fatherNode = targetNode;
		}
		else
		{
			if (fatherNode->left == targetNode)
			{
				SET_LEFT(fatherNode, targetNode->right);
			}
			else
			{
				SET_RIGHT(fatherNode, targetNode->left);
			}
			SET_RIGHT(targetNode, thisNode->right);
			SET_LEFT(targetNode, thisNode->left);
		}

		targetNode->depth = thisNode->depth;
		SET_FATHER(targetNode, thisNode->father, thisNode);
	}
	else
	{
		if (fatherNode != NULL)
		{
			if (fatherNode->left == thisNode)
				fatherNode->left = NULL;
			else
				fatherNode->right = NULL;
		}
		else
			setRoot(NULL);
	}
	alterTree(fatherNode);
	pIndexMem->free(pNode);
}

void CAVLTree::removeObject(const void *pObject)
{
	CAVLNode *pNode;
	pNode = findObject(pObject);
	if (pNode == NULL)
		return;
	removeNode(pNode);
}

void CAVLTree::removeAll(void)
{
	pIndexMem->freeAll();
	pIndexMem->alloc();
	setRoot(NULL);
}

CAVLNode *CAVLTree::updateNode(CAVLNode *pNode)
{
	const void *pObject;

	pObject = pNode->pObject;
	removeNode(pNode);
	return addObject(pObject);
}

CAVLNode *CAVLTree::updateObject(const void *pObject)
{
	CAVLNode *pNode;
	pNode = findObject(pObject);
	if (pNode == NULL)
		return NULL;
	return updateNode(pNode);
}

static int isNodeValid(CAVLNode *thisNode)
{
	int leftDepth, rightDepth;

	if (thisNode == NULL)
		return 1;
	if (thisNode->left != NULL)
		if (thisNode->left->father != thisNode)
			return 0;
	if (thisNode->right != NULL)
		if (thisNode->right->father != thisNode)
			return 0;
	leftDepth = LEFT_DEPTH(thisNode);
	rightDepth = RIGHT_DEPTH(thisNode);
	if (thisNode->depth != MAX(leftDepth, rightDepth) + 1)
		return 0;
	if (abs(leftDepth - rightDepth) > 1)
		return 0;
	if (!isNodeValid(thisNode->left))
		return 0;
	if (!isNodeValid(thisNode->right))
		return 0;
	return 1;
}

static CAVLNode *pLastNode;

static int checkOrder(CAVLNode *thisNode, TCompareFunc compareFunc)
{
	if (thisNode == NULL)
		return 1;
	if (!checkOrder(thisNode->left, compareFunc))
		return 0;
	if (pLastNode != NULL)
	{
		if (compareFunc(pLastNode->pObject, thisNode->pObject) > 0)
			return 0;
	}
	pLastNode = thisNode;
	return checkOrder(thisNode->right, compareFunc);
}

char *CAVLTree::isValid(int count)
{
	int nodeCount;
	CAVLNode *thisNode;
	///phase 1: check the pointer and depth
	if (!isNodeValid(getRoot()))
		return "Node invalid";
	///phase 2: check the order
	pLastNode = NULL;
	if (!checkOrder(getRoot(), compareFunc))
		return "Order invalid";
	///phase 3:another way to check order
	nodeCount = 0;
	pLastNode = NULL;
	thisNode = getSmallest();
	for (;;)
	{
		if (thisNode == NULL)
			break;
		nodeCount++;
		if (pLastNode != NULL)
		{
			if (compareFunc(pLastNode->pObject, thisNode->pObject) > 0)
				return "compare failed";
		}
		pLastNode = thisNode;
		thisNode = getNextNode(thisNode);
	}
	if ((count != -1) && (count != nodeCount))
		return "invalid node count";
	return NULL;
}

int CAVLTree::getReuseID(void)
{
	return pIndexMem->GetReuseID();
}
