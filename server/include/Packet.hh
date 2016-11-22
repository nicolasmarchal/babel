#ifndef PACKET_HH_
# define PACKET_HH_

#include <iostream>
#include <sstream>
#include "protocol.hh"
#include "ISocket.hh"

class Packet
{
private:
  std::stringstream buffer;
  int size;

public:
  Packet();
  ~Packet();
  void sendTo(ISocket *);
  void addMsg(char *buf, int size);
  void addMagicNumber();
  void addCode(int code);
  void addError(int error);
  void clear();
};

#endif
