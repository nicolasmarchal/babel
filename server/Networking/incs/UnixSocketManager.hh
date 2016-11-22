#ifndef UNIXSOCKETMANAGER_HH_
# define UNIXSOCKETMANAGER_HH_

#include "ISocketManager.hh"

#ifdef __linux
  #include <netdb.h>
  #include <sys/types.h>
  #include <sys/socket.h>
  #include <arpa/inet.h>
  #include <unistd.h>
#include "UnixSocket.hh"
#endif

#ifdef __APPLE__
#include "UnixSocket.hh"
#endif

class UnixSocketManager : public ISocketManager
{
public:
  UnixSocketManager(int port);
  UnixSocketManager(int port, int backlog);
  UnixSocketManager(int port, const std::string &ip);
  ~UnixSocketManager();

  void createSocket(std::string protoName);
  int  bindSocket();
  int  listenSocket();
  int  connectSocket();
  SOCKADDR_IN *getSin() const;
  void closeSocket();
  ISocket *getSocket() const;
  void		setSocket(ISocket *socket);
  int    getPort() const;
  int    getBacklog() const;
  void   setPort(int port);
  void    setBacklog(int backlog);
  ISocket  *acceptSocket();
};

#endif /* SOCKETMANAGER_HH_ */
