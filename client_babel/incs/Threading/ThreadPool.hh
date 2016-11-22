#pragma once

# include <thread>
# include <deque>
# include <vector>

# include "AThreadController.hpp"
# include "TaskThread.hh"

# define THREAD_POOL_LENGTH 6

typedef void (TaskThread::*TaskThreadPtr)(void);

class ThreadPool : public AThreadController<std::vector<TaskThread *>, TaskThreadPtr>
{
public:
	ThreadPool();
	~ThreadPool();
	bool isOver() const;
	void setOver(bool);
private:
	bool over;
};
