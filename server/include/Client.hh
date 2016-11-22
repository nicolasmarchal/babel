#ifndef CLIENT_HH_
# define CLIENT_HH_

#include <string>
#include <list>
#include <vector>
#include <algorithm>
#include "SocketManager.hh"

class Client
{
public:
  Client(ISocket *fd);
  ~Client();
  ISocket *getSocket();
  void setSocket(ISocket *sock);
  std::string getUserName();
  void setUserName(std::string const & name);
  std::list<std::string> &getContacts();
  void clear();
  void addContact(std::string name);
  void setIsCalling(bool calling);
  bool isCalling();
  void SetContactList(std::list<std::string> contacts);
  void clearCall();
  std::vector<Client *> & getUsersInCall();
  void addUserInCall(Client * &client);
  void delUserInCall(Client * &client);



private:
  ISocket *_sock;
  std::string _username;
  bool  _inCall;
  std::vector<Client *> _usersInCall;
  std::list<std::string> _contacts;
};



#endif
