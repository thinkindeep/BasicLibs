#include "mutex_cv.h"

#include <cstdlib>
#include <stdio.h>
#include <string.h>

static void PthreadCall(const char *label, int res) {
    if (0 != res) {
        fprintf(stderr, "pthread %s: %s\n", label, strerror(res));
        abort();
    }
}

Mutex::Mutex() {
    PthreadCall("init mutex", pthread_mutex_init(&m_mutex, NULL));
}

Mutex::~Mutex() {
    PthreadCall("destroy mutex", pthread_mutex_destroy(&m_mutex));
}

void Mutex::Lock() {
    PthreadCall("lock", pthread_mutex_lock(&m_mutex));
}

void Mutex::Unlock() {
    PthreadCall("unlock", pthread_mutex_unlock(&m_mutex));
}

CondVar::CondVar(Mutex *mu)
    : m_mu(mu) {
    PthreadCall("init cv", pthread_cond_init(&m_cv, NULL));
}

CondVar::~CondVar() {
    PthreadCall("destroy cv", pthread_cond_destroy(&m_cv));
}

void CondVar::Wait() {
    PthreadCall("wait", pthread_cond_wait(&m_cv, &m_mu->m_mutex));
}

void CondVar::Signal() {
    PthreadCall("signal", pthread_cond_signal(&m_cv));
}

void CondVar::SignalAll() {
    PthreadCall("broadcast", pthread_cond_broadcast(&m_cv));
}
