#include "Packet.hh"


Packet::Packet()
{
  
}

Packet::~Packet()
{

}

void Packet::sendTo(ISocket *sock)
{
  // std::cerr << buffer.tellp() << std::endl;
  //std::cout << buffer.str() << std::endl;
  //write(1, buffer.str().c_str(), buffer.tellp());
  //  write(1, buffer.c_str(), buffer.size);
  sock->write(buffer.str().c_str(), buffer.tellp());
}

void Packet::addMsg(char *buf, int size)
{
  buffer.write(buf, size);
}

void Packet::addMagicNumber()
{
  int hdr = MAGIC_VALUE;

  void *test = &hdr;
  
  buffer.write((char *)test, sizeof(int));
}

void Packet::addCode(int code)
{
  void *code2 = &code;
  buffer.write((char *)code2, sizeof(int));
}

void Packet::addError(int error)
{
  void *code2 = &error;
  buffer.write((char *)code2, sizeof(int));
}

void Packet::clear()
{
  buffer.str("");
}
