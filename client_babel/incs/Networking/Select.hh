#pragma once

# include <iostream>
# include <vector>
#if defined(WIN32) || defined(WIN64)
	#include <WinSock2.h>
	#pragma comment(lib, "ws2_32.lib")
#else
	#include <unistd.h>
	#include <sys/select.h>
#endif
# include "ISocket.hh"

class Select
{
public:
  class SelectIterator
  {
  public:
    SelectIterator();
    ~SelectIterator();
    void addSocket(ISocket *);
    bool hasNext() const;
    ISocket *getNext();
    void reset();
  private:
    unsigned int idx;
    std::vector<ISocket *> sockets;
  };
  Select();
  ~Select();
  void addSocket(ISocket *);
  void removeSocket(ISocket *);
  SelectIterator operator()(unsigned int delay);
private:
  void setMaxFd();
	void sleep(unsigned int delay);
private:
  unsigned int    maxFd;
  fd_set readfds;
  std::vector<ISocket *> sockets;
};
