#pragma once

#include <string>
#include "NetworkError.hh"
#include "UnixSocket.hh"

class ClientSocket : public UnixSocket
{
public:
	ClientSocket(int port, std::string const &ip, std::string const &proto);
	~ClientSocket();
	// int write(void *buf, int size);
	// int read(void *buf, int size);
	// void close();
};
