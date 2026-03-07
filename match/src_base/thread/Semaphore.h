#ifndef _SEMAPHORE_H__
#define _SEMAPHORE_H__

#if _MSC_VER > 1000
#pragma once
#endif

#include "platform.h"
#ifdef WIN32
#include <windows.h>
#elif defined(AIX) 
#include <pthread.h>
#elif defined(HP_UX)
#include <pthread.h>
#include <sys/semaphore.h>
#else
#include <semaphore.h>
#endif

class  CSemaphore
{
public:
	CSemaphore(int nValue = 1);
	~CSemaphore();
	bool Lock(int timeout);
	bool Lock();
	bool UnLock(int *pPrevCount = NULL);
private:
#ifdef WIN32
	HANDLE m_sid;
#elif defined(AIX)
	pthread_mutex_t m_mutex;
	pthread_cond_t m_cond;
	int m_value;
#elif defined(HP_UX)
	pthread_mutex_t m_mutex;
	pthread_cond_t m_cond;
	int m_nValue;		
#else
	sem_t m_sid;
#endif
};

#endif
