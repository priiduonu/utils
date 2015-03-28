/**
 * Copyright (c) 2013, Martin Hejnfelt (martin@hejnfelt.com)
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */


#include "Thread.h"
#include <cstdio>

Thread::Thread(bool deleteOnExit) :
	m_tid((pthread_t)-1),
	m_deleteOnExit(deleteOnExit)
{
	pthread_attr_init(&m_attr);
};

Thread::~Thread() {
	if(m_tid != (pthread_t)-1) {
		if(isRunning()) {
			stop();
		}
	}
	pthread_attr_destroy(&m_attr);
};

void Thread::start() {
	if(m_tid == (pthread_t)-1) {
		pthread_create(&m_tid,&m_attr,Thread::_thread,(void*)this);
	}
};

void Thread::stop() {
	pthread_cancel(m_tid);
	if(m_tid != (pthread_t)-1) pthread_join(m_tid,NULL);
};

void Thread::cleanup(void* ptr)
{
	Thread* t = static_cast<Thread*>(ptr);
	pthread_join(t->m_tid,NULL);
	t->doCleanup();
	t->m_tid = (pthread_t)-1;
	if(t->m_deleteOnExit) {
		delete t;
	}
	return;
}

void* Thread::_thread(void*ptr) {
	pthread_cleanup_push(Thread::cleanup,ptr);
	Thread* self = static_cast<Thread*>(ptr);
	self->thread();
	pthread_exit(0);
	pthread_cleanup_pop(1);
};

