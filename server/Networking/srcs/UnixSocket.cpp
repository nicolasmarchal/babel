#include "UnixSocket.hh"

UnixSocket::UnixSocket(int fd)
{
  _fd = fd;
}

UnixSocket::UnixSocket()
{
}

UnixSocket::~UnixSocket()
{
}

SOCKET UnixSocket::getFd() const
{
  return _fd;
}
void UnixSocket::setFd(SOCKET fd)
{
  _fd = fd;
}

int UnixSocket::write(void const *buf, int size)
{

  return (send(_fd, (const char *)buf, size, 0));
}

int UnixSocket::read(void *buf, int size)
{
  return (recv(_fd, (char *)buf, size, 0));
}

void UnixSocket::close()
{
  ::closesocket(_fd);
}
