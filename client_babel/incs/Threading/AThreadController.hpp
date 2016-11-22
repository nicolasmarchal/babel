#ifndef ATHREADCONTROLLER_HH_
# define ATHREADCONTROLLER_HH_

# include <thread>
# include <mutex>
# include <deque>
# include <iostream>
# include <condition_variable>

template<class Storage, class FuncType>
class AThreadController
{
public:
  AThreadController(){}
  ~AThreadController(){}
public:
  void push(FuncType func)
  {
    {
      std::unique_lock<std::mutex> lock(mutex);

      tasks.push_back(func);
    }
    cv.notify_one();
  }
  bool queueEmpty() const
  {
    return (tasks.empty());
  }

  FuncType &front()
  {
    return (tasks.front());
  }
  void     popFront()
  {
    tasks.pop_front();
  }

public:
  std::mutex mutex;
  std::condition_variable cv;
protected:
  Storage stored;
  std::deque<FuncType> tasks;
};

#endif /* !ATHREADCONTROLLER_HH_ */
