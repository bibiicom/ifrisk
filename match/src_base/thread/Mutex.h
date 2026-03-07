#ifndef __MUTEX_H_
#define __MUTEX_H_
#ifdef WIN32
#include <windows.h>
#else
#include <pthread.h>
#endif

class CMutex
{
public:
	CMutex()
	{
#ifdef  WIN32
		m_lock = CreateMutex(NULL, FALSE, NULL);
#else
		pthread_mutexattr_t mattr;
		pthread_mutexattr_init(&mattr);
		pthread_mutexattr_settype(&mattr, PTHREAD_MUTEX_RECURSIVE);
		pthread_mutex_init(&m_lock, &mattr);
		pthread_mutexattr_destroy(&mattr);
#endif
	}

	~CMutex()
	{
#ifdef  WIN32
		CloseHandle(m_lock);
#else
		pthread_mutex_destroy(&m_lock);
#endif
	}


	void Lock()
	{
#ifdef  WIN32
		WaitForSingleObject(m_lock, INFINITE);
#else
		pthread_mutex_lock(&m_lock);
#endif
	}

	bool TryLock()
	{
#ifdef  WIN32
		return (WaitForSingleObject(m_lock, 0) == WAIT_OBJECT_0);
#else
		return (pthread_mutex_trylock(&m_lock) == 0);
#endif
	}

	void UnLock()
	{
#ifdef  WIN32
		ReleaseMutex(m_lock);
#else
		pthread_mutex_unlock(&m_lock);
#endif
	}
private:
#ifdef WIN32
	HANDLE  m_lock;
#else
	pthread_mutex_t m_lock;
#endif
};

class CRWLock
{
public:
	CRWLock()
	{
#ifdef  WIN32
		m_lock = CreateMutex(NULL, FALSE, NULL);
#else
		//pthread_rwattr_t mattr;
		//pthread_rwattr_init(&mattr);
		//pthread_mutexattr_settype(&mattr, PTHREAD_MUTEX_RECURSIVE);
		//pthread_mutex_init(&m_lock, &mattr);
		//pthread_mutexattr_destroy(&mattr);
		pthread_rwlock_init(&m_lock, NULL);
#endif
	}

	~CRWLock()
	{
#ifdef  WIN32
		CloseHandle(m_lock);
#else
		pthread_rwlock_destroy(&m_lock);
#endif
	}


	void ReadLock()
	{
#ifdef  WIN32
		WaitForSingleObject(m_lock, INFINITE);
#else
		pthread_rwlock_rdlock(&m_lock);
#endif
	}

	void WriteLock()
	{
#ifdef  WIN32
		WaitForSingleObject(m_lock, INFINITE);
#else
		pthread_rwlock_wrlock(&m_lock);
#endif
	}

	bool TryReadLock()
	{
#ifdef  WIN32
		return (WaitForSingleObject(m_lock, 0) == WAIT_OBJECT_0);
#else
		return (pthread_rwlock_tryrdlock(&m_lock) == 0);
#endif
	}

	bool TryWriteLock()
	{
#ifdef  WIN32
		return (WaitForSingleObject(m_lock, 0) == WAIT_OBJECT_0);
#else
		return (pthread_rwlock_trywrlock(&m_lock) == 0);
#endif
	}

	void UnLock()
	{
#ifdef  WIN32
		ReleaseMutex(m_lock);
#else
		pthread_rwlock_unlock(&m_lock);
#endif
	}
private:
#ifdef WIN32
	HANDLE  m_lock;
#else
	pthread_rwlock_t m_lock;
#endif
};


class   CSpinLock {
public:
	CSpinLock()
	{
#ifdef  WIN32
		m_lock = CreateMutex(NULL, FALSE, NULL);
#else
		pthread_spin_init(&m_lock, PTHREAD_PROCESS_SHARED);
#endif
	}

	~CSpinLock()
	{
#ifdef  WIN32
		CloseHandle(m_lock);
#else
		pthread_spin_destroy(&m_lock);
#endif

	}

	void Lock()
	{
#ifdef  WIN32
		WaitForSingleObject(m_lock, INFINITE);
#else
		pthread_spin_lock(&m_lock);
#endif
	}

	bool TryLock()
	{
#ifdef  WIN32
		return (WaitForSingleObject(m_lock, 0) == WAIT_OBJECT_0);
#else
		return (pthread_spin_trylock(&m_lock) == 0);
#endif
	}

	void UnLock()
	{
#ifdef  WIN32
		ReleaseMutex(m_lock);
#else
		pthread_spin_unlock(&m_lock);
#endif
	}
private:
#ifdef WIN32
	HANDLE  m_lock;
#else
	pthread_spinlock_t m_lock;
#endif
};

/// Locks/Unlocks a mutex using RAII.
class CLocker
{
public:
	CLocker(CMutex& mutex)
		: m_mutex(mutex)
	{
		m_mutex.Lock();
	}

	~CLocker()
	{
		m_mutex.UnLock();
	}
private:
	CMutex& m_mutex;
};

#endif
