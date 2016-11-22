#include "Server.hh"

Server::Server(int port) :
_sm(port, 128)
{

}

Server::~Server()
{
  for (std::list<Client *>::iterator it = _clients.begin(); it != _clients.end(); ++it)
    delete *it;
}

SOCKET Server::init_fd()
{
  SOCKET nfds;

  FD_ZERO(&_sRead);
  FD_ZERO(&_sWrite);
  FD_SET(_sm.getSocket()->getFd(), &_sRead);
  FD_SET(_sm.getSocket()->getFd(), &_sWrite);
  nfds = _sm.getSocket()->getFd();
  for (std::list<Client *>::iterator it = _clients.begin(); it != _clients.end(); ++it)
    {
      FD_SET((*it)->getSocket()->getFd(), &_sRead);
      FD_SET((*it)->getSocket()->getFd(), &_sWrite);
      if (nfds < (*it)->getSocket()->getFd())
	nfds = (*it)->getSocket()->getFd();
    }
  return nfds;
}

void Server::readCommand(Client *cli, int header, int code)
{
  Command cmd(cli, _clients, code);

  std::cout << "header: "<< header << "code: " << code << std::endl;
  if (header != MAGIC_VALUE)
    {
      std::cout << "bad Header !" << std::endl;
      return;
    }
  cmd.execCmd();
}


void Server::newClient()
{
  _clients.push_back(new Client(_sm.acceptSocket()));
}

int	Server::getCmd(Client * &cli)
{
  t_hdr hdr;

  if ((cli->getSocket()->read(&hdr, sizeof(t_hdr))) <= 0)
    {
    //  Command cmd(cli, _clients, cmd::END_CALL);
      //cmd.quitCall();
      readCommand(cli, MAGIC_VALUE, cmd::LOGOUT);
      delete cli;
      return -1;
    }
  readCommand(cli, hdr.magic, hdr.cmd);
  return 0;
}

void  Server::receiveContent()

{
  std::list<Client *>::iterator it;
  for (it = _clients.begin(); it != _clients.end();)
  {
    if (FD_ISSET((*it)->getSocket()->getFd(), &_sRead))
      {
	       if ((getCmd(*it)) != 0)
          _clients.erase(it++);
         else
          ++it;
      }
    else
      ++it;
  }
}

void Server::execSelect()
{
  struct  timeval tv;

  while (1)
    {
      tv.tv_sec = 300;
      tv.tv_usec = 0;
      SOCKET nfds = init_fd();
      if (select(nfds + 1, &_sRead, &_sWrite, NULL, &tv) < 0)
	     {
	        std::cerr << "select" << strerror(errno) << std::endl;
	         return;
	        }
      if (FD_ISSET(_sm.getSocket()->getFd(), &_sRead))
	     {
	        std::cout << "NewClient connect " << std::endl;
	        newClient();
	      }
      receiveContent();
    }
}

void Server::launch()
{
  _sm.createSocket("TCP");

  if (_sm.bindSocket() == -1 || _sm.listenSocket() == -1)
    {
      _sm.closeSocket();
      //ERROR TROW
    }
  execSelect();
}
