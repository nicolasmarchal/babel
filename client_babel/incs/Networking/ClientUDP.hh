#pragma once

#include <cstring>
#include <iostream>
#include "ISocket.hh"
#include "protocol.hh"

class ClientUDP : public ISocket
{
public:
  ClientUDP(ISocket * = nullptr, bool is = false);
  ~ClientUDP();
  SOCKET getFd() const;
  void setFd(SOCKET fd);
  int write(void const *buff, int size);
  int read(void *, int size);
  void close();
  void setSin(SOCKADDR_IN *sin);
  void setSocket(ISocket *sock);
private:
  SOCKADDR_IN *from;
  ISocket *socket;
  bool isServer;
};
