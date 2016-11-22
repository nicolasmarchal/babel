#include "Client.hh"


Client::Client(ISocket *sock)
{
  _sock = sock;
  _username = std::string("");
  _inCall = false;
}

Client::~Client()
{
  _sock->close();
}

ISocket *Client::getSocket()
{
  return _sock;
}

void Client::setSocket(ISocket *sock)
{
  _sock = sock;
}

void Client::SetContactList(std::list<std::string> contacts)
{
  _contacts = contacts;
}

void Client::addContact(std::string name)
{
  _contacts.push_back(name);
}

std::list<std::string> & Client::getContacts()
{
  return _contacts;
}

void Client::setUserName(std::string const &name)
{
  _username = name;
}

std::string Client::getUserName()
{
  return _username;
}

void Client::setIsCalling(bool calling)
{
  _inCall = calling;
}

bool Client::isCalling()
{
  return _inCall;
}

void Client::clear()
{
  _username = std::string("");
  _inCall = false;
  _contacts.clear();
  _usersInCall.clear();
}

void Client::clearCall()
{
  _usersInCall.clear();
}

std::vector<Client *> & Client::getUsersInCall()
{
  return _usersInCall;
}

void Client::addUserInCall(Client * &client)
{
  _usersInCall.push_back(client);
}

void Client::delUserInCall(Client * &client)
{

  auto it = std::find(_usersInCall.begin(), _usersInCall.end(), client);
  if(it != _usersInCall.end())
    _usersInCall.erase(it);

  //_usersInCall.erase(std::remove(_usersInCall.begin(), _usersInCall.end(), client), _usersInCall.end());
//  _usersInCall.remove(client);
}
