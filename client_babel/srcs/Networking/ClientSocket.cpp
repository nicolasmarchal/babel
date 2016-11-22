#include "ClientSocket.hh"

ClientSocket::ClientSocket(int port, std::string const &ip, std::string const &protocol)
	: UnixSocket()
{
  struct hostent        *host;
  struct sockaddr_in    sin;

  if (!(host = gethostbyname(ip.c_str())))
    throw new NetworkError("Failed to get the host name");
  if ((_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    throw new NetworkError("Failed to create the socket");
  sin.sin_addr = *((struct in_addr *)host->h_addr);
  sin.sin_port = htons(port);
  sin.sin_family = AF_INET;
  if (connect(_fd, (struct sockaddr *)&sin, sizeof(struct sockaddr)) == -1)
    throw new NetworkError("Failed to connect to the client");
}

ClientSocket::~ClientSocket()
{
	::closesocket(_fd);
}