#include "stdafx.h"
#include "TaskThread.hh"
#include "ThreadPool.hh"

TaskThread::TaskThread(ThreadPool &npool)
	: thread(nullptr), pool(npool)
{
}

TaskThread::~TaskThread()
{
	if (thread)
		delete thread;
}

void
TaskThread::start()
{
	thread = new std::thread(std::ref(*this));
}

void
TaskThread::recordAudio()
{
	std::cout << "Je record de l'audio" << std::endl;
}

void
TaskThread::playAudio()
{
	// std::cout << "Je play de l'audio" << std::endl;
	// audio.setPlayBuffer(buffer, size);
}

void
TaskThread::recordVideo()
{
	std::cout << "Je record de la video" << std::endl;
}

void
TaskThread::playVideo()
{
	std::cout << "Je play de la video" << std::endl;
}

void
TaskThread::operator()()
{
	 TaskThreadPtr taskPtr;

	while (true)
	{
		{
			std::unique_lock<std::mutex> lock(pool.mutex);

			while (!pool.isOver() && pool.queueEmpty())
			{
				pool.cv.wait(lock);
			}
			if (pool.isOver())
			{
				break;
			}
			taskPtr = pool.front();
			pool.popFront();
		}
		(this->*taskPtr)();
	}
}

void
TaskThread::join()
{
	thread->join();
}
