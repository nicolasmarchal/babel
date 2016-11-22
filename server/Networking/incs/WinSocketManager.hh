#ifndef WINSOCKETMANAGER_HH_
# define WINSOCKETMANAGER_HH_

#include "ISocketManager.hh"

#if defined(WIN32) || defined(WIN64)
  #include <WinSock2.h>
  #pragma comment(lib, "ws2_32.lib")
  #include "WinSocket.hh"
#endif

class WinSocketManager : public ISocketManager
{
public:
  static unsigned int count;
  WinSocketManager(int port);
  WinSocketManager(int port, int backlog);
  WinSocketManager(int port, const std::string &ip);
  ~WinSocketManager();

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
  void    startWSA();
  void    cleanWSA();
};

#endif /* SOCKETMANAGER_HH_ */
