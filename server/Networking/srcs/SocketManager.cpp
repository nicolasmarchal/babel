#include "SocketManager.hh"

unsigned int SocketManager::count = 0;

SocketManager::SocketManager(int port) :
_port(port), _backlog(125)
{
	startWSA();
}

SocketManager::SocketManager(int port, int backlog) :
_port(port), _backlog(backlog)
{
	startWSA();
}

SocketManager::SocketManager(int port, std::string const &ip)
  : _ip(ip), _port(port)
{
  startWSA();
}

SocketManager::~SocketManager()
{
	cleanWSA();
}

void
SocketManager::startWSA()
{
	#if defined(WIN32) || defined(WIN64)
		if (count == 0)
		{
			WSADATA WSAData;
			WSAStartup(MAKEWORD(2, 0), &WSAData);
		}
		++count;
	#endif
}

void
SocketManager::cleanWSA()
{
	#if defined(WIN32) || defined(WIN64)
		--count;
		if (count == 0)
			WSACleanup();
	#endif
}

void SocketManager::createSocket(std::string protoName)
{
  struct protoent *pe;
	int type = protoName == "UDP" ? SOCK_DGRAM : SOCK_STREAM;

	if ((pe = getprotobyname(protoName.c_str())) == NULL)
    {
      std::cerr << "Error on getprotobyname" << std::endl;
      //THROW EXECPTION
      return;
    }
#ifdef __linux
  _sock = new UnixSocket(socket(AF_INET, type, protoName == "UDP" ? 0 : pe->p_proto));
#elif __APPLE__
  _sock = new UnixSocket(socket(AF_INET, type, protoName == "UDP" ? 0 : pe->p_proto));
#else
	_sock = new WinSocket(socket(AF_INET, type, protoName == "UDP" ? 0 : pe->p_proto));
#endif
}

int SocketManager::connectSocket()
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
SocketManager::getSin() const
{
	struct hostent *host;
	SOCKADDR_IN *sin = new SOCKADDR_IN;

	if (!(host = gethostbyname(_ip.c_str())))
		return (nullptr);
	sin->sin_addr = *((struct in_addr *)host->h_addr);
	sin->sin_port = htons(_port);
	sin->sin_family = AF_INET;
	return (sin);
}

int  SocketManager::bindSocket()
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

int  SocketManager::listenSocket()
{
  if ((listen(_sock->getFd(), _backlog)) == -1)
    {
      std::cerr << "listen:" << strerror(errno) << std::endl;
      //THROW EXECPTION
      return (-1);
    }
    return (0);
}

void SocketManager::closeSocket()
{
  _sock->close();
}

ISocket *SocketManager::getSocket() const
{
  return _sock;
}

int    SocketManager::getPort() const
{
  return _port;
}

int    SocketManager::getBacklog() const
{
  return _backlog;
}

void   SocketManager::setPort(int port)
{
  _port = port;
}

void    SocketManager::setBacklog(int backlog)
{
  _backlog = backlog;
}

void
SocketManager::setSocket(ISocket *sock)
{
	_sock = sock;
}

ISocket *SocketManager::acceptSocket()
{
    struct sockaddr_in  s_in;
    socklen_t           size;

    size = sizeof(s_in);
#ifdef __linux
    return new UnixSocket(accept(_sock->getFd(), (struct sockaddr *)&s_in, &size));
#elif __APPLE__
    return new UnixSocket(accept(_sock->getFd(), (struct sockaddr *)&s_in, &size));
#else
		return new WinSocket(accept(_sock->getFd(), (struct sockaddr *)&s_in, &size));
#endif
}
