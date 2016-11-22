#include "WinSocketManager.hh"

unsigned int WinSocketManager::count = 0;

WinSocketManager::WinSocketManager(int port) : ISocketManager()
{
  _port = port();
  _backlog = 125;
  startWSA();
}

WinSocketManager::WinSocketManager(int port, int backlog) : ISocketManager()
_port(port), _backlog(backlog)
{
  _port = port;
  _backlog = backlog;
	startWSA();
}

WinSocketManager::WinSocketManager(int port, std::string const &ip) : ISocketManager()
{
  _port = port;
  _ip = ip;
  startWSA();
}

WinSocketManager::~WinSocketManager()
{
	cleanWSA();
}

void
WinSocketManager::startWSA()
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
WinSocketManager::cleanWSA()
{
	#if defined(WIN32) || defined(WIN64)
		--count;
		if (count == 0)
			WSACleanup();
	#endif
}

void WinSocketManager::createSocket(std::string protoName)
{
  struct protoent *pe;
	int type = protoName == "UDP" ? SOCK_DGRAM : SOCK_STREAM;

	if ((pe = getprotobyname(protoName.c_str())) == NULL)
    {
      std::cerr << "Error on getprotobyname" << std::endl;
      //THROW EXECPTION
      return;
    }
	_sock = new WinSocket(socket(AF_INET, type, protoName == "UDP" ? 0 : pe->p_proto));
}

int WinSocketManager::connectSocket()
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
WinSocketManager::getSin() const
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

int  WinSocketManager::bindSocket()
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

int  WinSocketManager::listenSocket()
{
  if ((listen(_sock->getFd(), _backlog)) == -1)
    {
      std::cerr << "listen:" << strerror(errno) << std::endl;
      //THROW EXECPTION
      return (-1);
    }
    return (0);
}

void WinSocketManager::closeSocket()
{
  _sock->close();
}

ISocket *WinSocketManager::getSocket() const
{
  return _sock;
}

int    WinSocketManager::getPort() const
{
  return _port;
}

int    WinSocketManager::getBacklog() const
{
  return _backlog;
}

void   WinSocketManager::setPort(int port)
{
  _port = port;
}

void    WinSocketManager::setBacklog(int backlog)
{
  _backlog = backlog;
}

void
WinSocketManager::setSocket(ISocket *sock)
{
	_sock = sock;
}

ISocket *WinSocketManager::acceptSocket()
{
    struct sockaddr_in  s_in;
    socklen_t           size;

    size = sizeof(s_in);
    return new WinSocket(accept(_sock->getFd(), (struct sockaddr *)&s_in, &size));
}
