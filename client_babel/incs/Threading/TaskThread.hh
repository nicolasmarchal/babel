#pragma once

# include <thread>
# include <mutex>
# include <condition_variable>
# include <iostream>
# include "AudioEncapsuler.hh"

class ThreadPool;

class TaskThread
{
public:
	TaskThread(ThreadPool &pool);
	~TaskThread();
	void start();
	void operator()();
	void recordAudio();
	void playAudio();
	void recordVideo();
	void playVideo();
	void join();
private:
	struct ThreadSafeAudio
	{
		std::mutex mutex;
		AudioEncapsuler ae;
	};
	ThreadSafeAudio tsa;
	std::thread *thread;
	ThreadPool &pool;
};
