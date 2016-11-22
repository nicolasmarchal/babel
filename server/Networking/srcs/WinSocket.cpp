#include "WinSocket.hh"

WinSocket::WinSocket(SOCKET fd)
{
  _fd = fd;
}


WinSocket::~WinSocket()
{
}

SOCKET WinSocket::getFd() const
{
  return _fd;
}

void WinSocket::setFd(SOCKET fd)
{
	_fd = fd;
}

int WinSocket::write(void const *buf, int size)
{
  return (send(_fd, (const char *)buf, size, 0));
}

int WinSocket::read(void *buf, int size)
{
  return (recv(_fd, (char *)buf, size, 0));
}

void WinSocket::close()
{
  ::closesocket(_fd);
}
