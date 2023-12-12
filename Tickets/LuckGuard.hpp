#pragma once

#include <pthread.h>

class Mutex
{
public:
    Mutex(pthread_mutex_t *lock)
        : _lock(lock)
    {
    }
    void Lock()
    {
        pthread_mutex_lock(_lock);
    }
    void UnLock()
    {
        pthread_mutex_unlock(_lock);
    }
    ~Mutex()
    {
    }

private:
    pthread_mutex_t *_lock;
};

class LockGuard
{
public:
    LockGuard(pthread_mutex_t *lock)
        : mutex(lock)
    {
        mutex.Lock();
    }
    ~LockGuard()
    {
        mutex.UnLock();
    }

private:
    Mutex mutex;
};