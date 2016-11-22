#ifndef ISOCKETMANAGER_HH_
# define ISOCKETMANAGER_HH_

#include <iostream>
#include <string>
#include <errno.h>
#include <string.h>
#include "ISocket.hh"

#ifdef __linux
typedef struct sockaddr_in SOCKADDR_IN;
#endif

class ISocketManager
{
public:
  ~ISocketManager() {};

  virtual void createSocket(std::string protoName) = 0;
  virtual int  bindSocket() = 0;
  virtual int  listenSocket() = 0;
  virtual int  connectSocket() = 0;
  virtual SOCKADDR_IN *getSin() const = 0;
  virtual void closeSocket() = 0;
  virtual ISocket *getSocket() const = 0;
  virtual void		setSocket(ISocket *socket) = 0;
  virtual int    getPort() const = 0;
  virtual int    getBacklog() const = 0;
  virtual void   setPort(int port) = 0;
  virtual void    setBacklog(int backlog) = 0;
  virtual ISocket  *acceptSocket() = 0;
protected:
	std::string _ip;
  int     _port;
  int     _backlog;
  ISocket  *_sock;
};

#endif /* SOCKETMANAGER_HH_ */
