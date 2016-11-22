#ifndef SERVER_HH_
# define SERVER_HH_

#include <iostream>
#include <string>
#include <list>
#include "SocketManager.hh"
#include "ISocket.hh"
#include "Client.hh"
#include "Command.hh"

class Server
{
public:
  Server(int port);
  ~Server();
  void launch();
private:
  SocketManager _sm;
  std::list<Client *> _clients;
  fd_set  _sRead;
  fd_set  _sWrite;

  void execSelect();
  SOCKET init_fd();
  void readCommand(Client *cli, int header, int code);
  void newClient();
  void receiveContent();
  int getCmd(Client *&);
};

#endif /* SERVER_HH_ */
