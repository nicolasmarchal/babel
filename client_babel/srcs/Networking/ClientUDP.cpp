#include "ClientUDP.hh"

ClientUDP::ClientUDP(ISocket *sck, bool is)
  : from(nullptr), socket(sck), isServer(is)
{

}

ClientUDP::~ClientUDP()
{
  delete socket;
  if (from)
    delete from;
}

SOCKET
ClientUDP::getFd() const
{
  return (socket->getFd());
}

void
ClientUDP::setFd(SOCKET fd)
{
  socket->setFd(fd);
}

int
ClientUDP::write(void const *buff, int size)
{
	int fromsize = sizeof(*from);

	return (sendto(socket->getFd(), (const char *)buff, size, 0, (struct sockaddr *)from, (socklen_t)fromsize));
}

int
ClientUDP::read(void *buff, int size)
{
  SOCKADDR_IN *nfrom = new SOCKADDR_IN;
	int fromsize = sizeof(*from);
  int ret;

	ret = recvfrom(socket->getFd(), (char *)buff, size, 0, (struct sockaddr *)nfrom, (socklen_t *)&fromsize);
  from = nfrom;
  return (ret);
}

void
ClientUDP::close()
{
  socket->close();
}

void
ClientUDP::setSin(SOCKADDR_IN *sin)
{
  from = sin;
}

void
ClientUDP::setSocket(ISocket *sck)
{
  socket = sck;
}
