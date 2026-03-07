// HashMap.h: interface for the CHashMap class.
//////////////////////////////////////////////////////////////////////

#ifndef _HASHMAP_H__
#define _HASHMAP_H__

#if _MSC_VER > 1000
#pragma once
#endif

#include "platform.h"

// Note: assumes long is at least 32 bits.
enum { __stl_num_primes = 28 };

static const unsigned long __stl_prime_list[__stl_num_primes] =
{
	53ul,         97ul,         193ul,       389ul,       769ul,
	1543ul,       3079ul,       6151ul,      12289ul,     24593ul,
	49157ul,      98317ul,      196613ul,    393241ul,    786433ul,
	1572869ul,    3145739ul,    6291469ul,   12582917ul,  25165843ul,
	50331653ul,   100663319ul,  201326611ul, 402653189ul, 805306457ul, 
	1610612741ul, 3221225473ul, 4294967291ul
};

class HashInt
{
public:
	int operator()(const int key) const
	{
		return key;
	}; 
};

class HashString
{ 
public:
	int operator()(const char *str) const
	{
		unsigned long ret=0;
		
		if ((str == NULL) || (*str == '\0'))
		{
			return(ret);
		}

		long n = 0x100;
		while (*str)
		{
			unsigned long v = n|(*str);
			n += 0x100;
			int r = (int)((v>>2)^v)&0x0f;
			ret = (ret<<r)|(ret>>(32-r));
			ret &= 0xFFFFFFFFL;
			ret ^= v*v;
			str++;
		}
		return ((ret>>16)^ret);
	}
};

template <class _Key, class _Tp>
class CHashNode
{
public:
	_Key  _M_Key;
	_Tp   _M_Value;
	CHashNode *_M_Next;
};

template <class _Key, class _Tp>
class CHasMapIterator
{
public:
	typedef CHashNode<_Key, _Tp> _node_type;
	typedef CHasMapIterator<_Key, _Tp> _Self;		/**<本身类型定义*/
	typedef _Tp& reference;						/**<引用类型定义*/
	typedef _Tp* pointer;						/**<指针类型定义*/

	/**构造函数
	*/
	CHasMapIterator(_node_type **_Bucket, int _Size){
		_M_Bucket = _Bucket;
		_M_Size = _Size;
		_M_CurrBucket = 0;
		for(; _M_CurrBucket<_Size; _M_CurrBucket++){
			_M_Node = _M_Bucket[_M_CurrBucket];
			if( _M_Node != NULL){
				break;
			}
		}
	}
	CHasMapIterator(){
		_M_Bucket = NULL;
		_M_Node = NULL;
		_M_CurrBucket = _M_Size;
	}

	/**获取当前记录
	* @return 当前记录
	*/
	reference operator*() const { return _M_Node->_M_Value; };

	/**获取当前键值
	* @return 当前键值
	*/
	_Key & GetKey()	{ return _M_Node->_M_Key; };

	/**移动到下一个记录
	*/
	_Self& operator++() { 
		if(_M_Node->_M_Next != NULL){
			_M_Node = _M_Node->_M_Next;
		}
		else{
			if(!IsEnd())_M_CurrBucket++;
			for(; _M_CurrBucket<_M_Size; _M_CurrBucket++){
				_M_Node = _M_Bucket[_M_CurrBucket];
				if( _M_Node != NULL){
					break;
				}
			}
		}
		return *this;
	}

	/**移动到下一个记录
	*/
	_Self operator++(int) { 
		_Self __tmp = *this;
		++*this;
		return __tmp;
	}
	
	/**是否还有更多的记录
	* @return true 没有更多的记录\n
	* @return false 还有更多的记录
	*/
	inline bool IsEnd(){
		return _M_CurrBucket == _M_Size;
	}
private:
	_node_type ** _M_Bucket;
	int _M_CurrBucket;
	_node_type * _M_Node;
	int _M_Size;
};

template <class _Key, class _Tp, class _HashFcn>
class CHashMap  
{
public:
	typedef CHasMapIterator<_Key, _Tp> iterator;
	typedef CHashNode<_Key, _Tp> _node_type;

	CHashMap(int _Size = 53){
		_M_Bucket = new _node_type*[_Size];
		_M_Size = _Size;
		Clear();
	};
	virtual ~CHashMap(){
		delete[] _M_Bucket;
	};

	int GetPos(const _Key &key)
	{
		return ((unsigned int)_M_hash((_Key)key)) % _M_Size;
	}
	
	void Insert(const _Key &key, const _Tp &value)
	{
		int pos = GetPos(key);
		_node_type *pNode = Alloc();
		pNode->_M_Key = key;
		pNode->_M_Value = value;
		pNode->_M_Next = _M_Bucket[pos];
		_M_Bucket[pos] = pNode;
		_M_Count++;
	}

	_Tp *Find(const _Key &key)
	{
		int pos = GetPos(key);
		_node_type *pNode = _M_Bucket[pos];
		while(pNode != NULL){
			if(pNode->_M_Key == key){
				return &pNode->_M_Value;
			}
			pNode = pNode->_M_Next;
		}
		return NULL;
	}

	void Erase(const _Key &key)
	{
		int pos = GetPos(key);
		_node_type *pNode = _M_Bucket[pos];
		_node_type *pPrev = NULL;
		while(pNode != NULL){
			if(pNode->_M_Key == key){
				if(pPrev == NULL){
					_M_Bucket[pos] = pNode->_M_Next;
				}
				else{
					pPrev->_M_Next = pNode->_M_Next;
				}
				pNode->_M_Next = _M_Free;
				_M_Free = pNode;
				_M_Count--;
				return;
			}
			pPrev = pNode;
			pNode = pNode->_M_Next;
		}
	}

	iterator Begin()
	{
		return iterator(_M_Bucket, _M_Size);
	}

	_node_type *Alloc()
	{
		_node_type *pNewNode = NULL;
		if(_M_Free != NULL){
			pNewNode = _M_Free;
			_M_Free = _M_Free->_M_Next;
		}
		else{
			_node_type node;
			_M_Link.push_back(node);
			pNewNode = &_M_Link.back();
		}
		return pNewNode;
	}

	int GetCount()
	{
		return _M_Count;
	}

	void Clear()
	{
		memset(_M_Bucket, 0, sizeof(_node_type*)*_M_Size);
		_M_Link.clear();
		_M_Free = NULL;
		_M_Count = 0;
	}
private:
	_HashFcn _M_hash;
	_node_type ** _M_Bucket;
	deque<_node_type> _M_Link;
	_node_type * _M_Free;
	int _M_Size;
	int _M_Count;
};

#endif
