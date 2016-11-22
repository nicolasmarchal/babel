#ifndef SOCKETMANAGER_HH_
# define SOCKETMANAGER_HH_

#include <iostream>
#include <string>
#include <errno.h>
#include <string.h>
#include "ISocket.hh"

#ifdef __linux
  #include <netdb.h>
  #include <sys/types.h>
  #include <sys/socket.h>
  #include <arpa/inet.h>
  #include <unistd.h>
#include "UnixSocket.hh"
typedef struct sockaddr_in SOCKADDR_IN;
#endif

#if defined(WIN32) || defined(WIN64)
  #include <WinSock2.h>
  #pragma comment(lib, "ws2_32.lib")
  #include "WinSocket.hh"
#endif
#ifdef __APPLE__
  #include "UnixSocket.hh"
#endif

class SocketManager
{
public:
	static unsigned int count;
  SocketManager(int port);
	SocketManager(int port, int backlog);
	SocketManager(int port, const std::string &ip);
  ~SocketManager();

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
private:
	std::string _ip;
  int     _port;
  int     _backlog;
  ISocket  *_sock;
private:
  void    startWSA();
  void    cleanWSA();
};

#endif /* SOCKETMANAGER_HH_ */
