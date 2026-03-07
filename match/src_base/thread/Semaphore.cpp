#include "Semaphore.h"

CSemaphore::CSemaphore(int nValue)
{
#ifdef WIN32
	m_sid = CreateSemaphore(NULL, nValue, 1000, NULL);
#elif defined(AIX) | defined(HP_UX)
	m_nValue = nValue;
	pthread_mutex_init(&m_mutex, NULL);
	pthread_cond_init(&m_cond,	NULL);
#else
	sem_init(&m_sid, 0, nValue);
#endif
}

CSemaphore::~CSemaphore()
{
#ifdef WIN32
	CloseHandle(m_sid);
#elif defined(AIX) | defined(HP_UX)
	pthread_cond_destroy(&m_cond);
	pthread_mutex_destroy(&m_mutex);
#else
	sem_destroy(&m_sid);
#endif
}

bool CSemaphore::Lock()
{
#ifdef WIN32
	return (WaitForSingleObject(m_sid, INFINITE)==WAIT_OBJECT_0);
#elif defined(AIX)|defined(HP_UX)	
	pthread_mutex_lock(&m_mutex);
	while (m_nValue <= 0)
	{
		pthread_cond_wait(&m_cond,&m_mutex);
	}
	if(m_nValue > 0)
	{
		m_nValue--;
	}
	pthread_mutex_unlock(&m_mutex);
	return true;
#else
	return (sem_wait(&m_sid) == 0);
#endif
}

bool CSemaphore::Lock(int timeout)
{
#ifdef WIN32
	return (WaitForSingleObject(m_sid, INFINITE)==WAIT_OBJECT_0);
#elif defined(AIX)|defined(HP_UX)	
	pthread_mutex_lock(&m_mutex);
	while (m_nValue <= 0)
	{
		struct timespec m_timeout;
		m_timeout.tv_sec=time(NULL)+timeout;
		m_timeout.tv_nsec=0;
		int ret = pthread_cond_timedwait(&m_cond,&m_mutex,&m_timeout);
		if(ret == ETIMEDOUT)
			m_nValue++;
	}
	if(m_nValue > 0)
	{
		m_nValue--;
	}
	pthread_mutex_unlock(&m_mutex);
	return true;
#else
	timespec m_timeout;
	if (clock_gettime(CLOCK_REALTIME, &m_timeout) == -1)
	{
		return	(sem_wait(&m_sid) == 0);
	}
	else
	{
		m_timeout.tv_sec+=timeout;
		return (sem_timedwait(&m_sid,&m_timeout) == 0);
	}
#endif
}
bool CSemaphore::UnLock(int *pPrevCount)
{
#ifdef WIN32
	return (ReleaseSemaphore(m_sid, 1, (long *)pPrevCount) == TRUE);
#elif defined(AIX) | defined(HP_UX)	
	pthread_mutex_lock(&m_mutex);
	m_nValue++;
	pthread_cond_signal(&m_cond);
	pthread_mutex_unlock(&m_mutex);
	if(pPrevCount != NULL)
	{
		*pPrevCount = m_nValue;
	}
	return true;
#else
	if(sem_post(&m_sid) == 0)
	{
		if(pPrevCount != NULL)
		{
			sem_getvalue(&m_sid, pPrevCount);
		}
		return true;
	}
	return false;
#endif
}

