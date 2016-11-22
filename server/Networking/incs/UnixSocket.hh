#ifndef UNIXSOCKET_HH_
# define UNIXSOCKET_HH_

#include "ISocket.hh"

class UnixSocket : public ISocket
{
 public:
   UnixSocket(int fd);
  UnixSocket();
  ~UnixSocket();
  virtual SOCKET getFd() const;
  virtual void setFd(SOCKET fd);
  virtual int write(void const *buf, int size);
  virtual int read(void *buf, int size);
  virtual void close();
};

#endif
