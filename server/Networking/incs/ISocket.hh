#ifndef ISOCKET_HH_
# define ISOCKET_HH_

#ifdef __linux
	#include <netdb.h>
	#include <sys/types.h>
	#include <sys/socket.h>
	#include <arpa/inet.h>
	#include <unistd.h>
	typedef struct sockaddr_in SOCKADDR_IN;
	typedef int SOCKET;
	#define closesocket close
#endif

#if defined(WIN32) || defined(WIN64)
	#include <ws2tcpip.h>
	#include <WinSock2.h>
	#pragma comment(lib, "ws2_32.lib")
#endif
#ifdef __APPLE__
# include <sys/socket.h>
# include <netinet/in.h>
# include <netinet/ip.h>
# include <netdb.h>
# include <arpa/inet.h>
# include <unistd.h>
#define closesocket close
typedef struct sockaddr_in SOCKADDR_IN;
typedef int SOCKET;
#endif

class ISocket
{
protected:
  SOCKET _fd;
public:
	virtual ~ISocket();
  virtual SOCKET getFd() const = 0;
  virtual void setFd(SOCKET fd) = 0;
  virtual int write(void const *buf, int size) = 0;
  virtual int read(void *buf, int size) = 0;
  virtual void close() = 0;
};

#endif
