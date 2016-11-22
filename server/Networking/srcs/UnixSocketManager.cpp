#include "UnixSocketManager.hh"

UnixSocketManager::UnixSocketManager(int port) : ISocketManager()
{
  _backlog = 125;
  _port = port;
}

UnixSocketManager::UnixSocketManager(int port, int backlog) : ISocketManager()
{
  _port = port;
  _backlog = backlog;
}

UnixSocketManager::UnixSocketManager(int port, std::string const &ip)
  : ISocketManager()
{
  _ip = ip;
  _port = port;
}

UnixSocketManager::~UnixSocketManager()
{
}

void UnixSocketManager::createSocket(std::string protoName)
{
  struct protoent *pe;
  int type = protoName == "UDP" ? SOCK_DGRAM : SOCK_STREAM;

  if ((pe = getprotobyname(protoName.c_str())) == NULL)
    {
      std::cerr << "Error on getprotobyname" << std::endl;
      //THROW EXECPTION
      return;
    }
  _sock = new UnixSocket(socket(AF_INET, type, protoName == "UDP" ? 0 : pe->p_proto));
}

int UnixSocketManager::connectSocket()
{
	struct hostent *host;
	SOCKADDR_IN sin;

	if (!(host = gethostbyname(_ip.c_str())))
		return (-1);
	sin.sin_addr = *((struct in_addr *)host->h_addr);
	sin.sin_port = htons(_port);
	sin.sin_family = AF_INET;
	return (connect(_sock->getFd(), (struct sockaddr *)&sin, sizeof(struct sockaddr)));
}

SOCKADDR_IN *
UnixSocketManager::getSin() const
{
	struct hostent *host;
	SOCKADDR_IN *sin = new SOCKADDR_IN;

	if (!(host = gethostbyname(_ip.c_str())))
	  return (0);
	sin->sin_addr = *((struct in_addr *)host->h_addr);
	sin->sin_port = htons(_port);
	sin->sin_family = AF_INET;
	return (sin);
}

int  UnixSocketManager::bindSocket()
{
  SOCKADDR_IN s_in;

  s_in.sin_family = AF_INET;
  s_in.sin_port = htons(_port);
  s_in.sin_addr.s_addr = INADDR_ANY;
  if ((bind(_sock->getFd(), (const struct sockaddr *)&s_in, sizeof(s_in))) == -1)
    {
      std::cerr << "bind:" << strerror(errno) << std::endl;
      //TRHOW EXECPTION
      return (-1);
    }
  return (0);
}

int  UnixSocketManager::listenSocket()
{
  if ((listen(_sock->getFd(), _backlog)) == -1)
    {
      std::cerr << "listen:" << strerror(errno) << std::endl;
      //THROW EXECPTION
      return (-1);
    }
    return (0);
}

void UnixSocketManager::closeSocket()
{
  _sock->close();
}

ISocket *UnixSocketManager::getSocket() const
{
  return _sock;
}

int    UnixSocketManager::getPort() const
{
  return _port;
}

int    UnixSocketManager::getBacklog() const
{
  return _backlog;
}

void   UnixSocketManager::setPort(int port)
{
  _port = port;
}

void    UnixSocketManager::setBacklog(int backlog)
{
  _backlog = backlog;
}

void
UnixSocketManager::setSocket(ISocket *sock)
{
	_sock = sock;
}

ISocket *UnixSocketManager::acceptSocket()
{
    struct sockaddr_in  s_in;
    socklen_t           size;

    size = sizeof(s_in);
    return new UnixSocket(accept(_sock->getFd(), (struct sockaddr *)&s_in, &size));
}
