#include "stdafx.h"
#include "ThreadPool.hh"

ThreadPool::ThreadPool()
	: over(false)
{
	TaskThread *t;

	for (int i = 0; i < THREAD_POOL_LENGTH; i++)
	{
		t = new TaskThread(*this);
		stored.push_back(t);
		t->start();
	}
}

ThreadPool::~ThreadPool()
{
	over = true;
	cv.notify_all();
	for (unsigned int i = 0; i < stored.size(); ++i)
	{
		stored[i]->join();
		delete stored[i];
	}
}

bool
ThreadPool::isOver() const
{
	return (over);
}

void
ThreadPool::setOver(bool nover)
{
	over = nover;
}
