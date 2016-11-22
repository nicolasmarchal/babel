#pragma once

# include <string>
# include <vector>
# include "ISocket.hh"

class SocketList
{
public:
  struct WrappedSocket
  {
    ISocket *socket;
    std::string id;
  };
  SocketList();
  ~SocketList();
  void addSocket(ISocket *, std::string const &id);
  ISocket *getById(std::string const &id);
private:
  std::vector<struct WrappedSocket> sockets;
};
