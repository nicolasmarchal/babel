#ifndef WINSOCKET_HH_
# define WINSOCKET_HH_

#include <iostream>
#include "ISocket.hh"

class WinSocket : public ISocket
{
 public:
  WinSocket(SOCKET fd);
  ~WinSocket();
  virtual SOCKET getFd() const;
  virtual void setFd(SOCKET fd);
  virtual int write(void const *buf, int size);
  virtual int read(void *buf, int size);
  virtual void close();
};

#endif
