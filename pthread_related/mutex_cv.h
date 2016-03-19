/*
 * copy from leveldb/port
 */

#ifndef MUTEX_CV_H
#define MUTEX_CV_H

#include <pthread.h>

class Mutex {
    public:
        Mutex();
        ~Mutex();
        void Lock();
        void Unlock();

    private:
        friend class CondVar;
        pthread_mutex_t m_mutex;

        // No copy
        Mutex(const Mutex&);
        Mutex& operator=(const Mutex&);
};

class CondVar {
    public:
        explicit CondVar(Mutex* mu);
        ~CondVar();
        void Wait();
        void Signal();
        void SignalAll();
    private:
        pthread_cond_t m_cv;
        Mutex* m_mu;

        CondVar(const CondVar&);
        CondVar& operator=(const CondVar&);
};


/*
 * Usage:
 *
 * void MyClass::MyMethod() {
 *      MutexLock l(&mu_);       // mu_ is an instance variable
 *      //     ... some complex code, possibly with multiple return paths ...
 * }
 */
class MutexLock {
    public:
        explicit MutexLock(Mutex *mu)
            : m_mu(mu) {
                this->m_mu->Lock();
            }
        ~MutexLock() {
            this->m_mu->Unlock();
        }

    private:
        Mutex *const m_mu;

        MutexLock(const MutexLock&);
        MutexLock& operator=(const MutexLock&);
};

#endif
