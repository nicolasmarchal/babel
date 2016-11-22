
#include <iostream>
#include <cstring>
#include <fstream>
#include <string.h>

#if defined(WIN32) || defined(WIN64)
# define strcpy(x, y) strcpy_s(x, y)
#define _WINSOCK_DEPRECATED_NO_WARNINGS 1

# endif

#include "Command.hh"

Command::Command(Client * &sender, std::list<Client *> & clients, int code):
  _cli(sender), _clients(clients)
{
  _cmd[cmd::REGISTER] = &Command::registerUser;
  _cmd[cmd::LOGIN] = &Command::login;
  _cmd[cmd::LOGOUT] = &Command::logout;
  _cmd[cmd::GET_CONTACT_LIST] = &Command::getContactList;
  _cmd[cmd::ADD_CONTACT] = &Command::addContact;
  _cmd[cmd::DEL_CONTACT] = &Command::delContact;
  _cmd[cmd::REP_CONTACT] = &Command::repContact;
  _cmd[cmd::SEND_CALL] = &Command:: sendCall;
  _cmd[cmd::RESPONSE_CALL] = &Command::responseCall;
  _cmd[cmd::END_CALL] = &Command::endCall;
  _cmd[cmd::SEND_TEXT] = &Command::sendText;
  _cmd[cmd::GET_ALL_CONTACT] = &Command::getAllContact;
  _cmd[cmd::GET_CONTACT_ONLINE] = &Command::getContactOnline;
  _cmd[cmd::GET_CONV_HISTORY] = &Command::getConvHistory;
  _code = code;
}

Command::~Command()
{

}


void Command::execCmd()
{
  if (_cmd.find(_code) != _cmd.end())
   {
     void (Command::*tmp)();
     tmp = _cmd[_code];
     (this->*tmp)();
   }
  else
  {
    std::cout << "unknown instruction !" << std::endl;
    basicResponse(err::UNKOWN);
  }
}

void sendRequest(int cmd, void *data, unsigned int length, ISocket * sock)
{
  unsigned int glength = sizeof(t_hdr) + length;
  char *packet = new char[glength];
    {
      t_hdr com;
      com.magic = MAGIC_VALUE;
      com.cmd = cmd;
      std::memcpy(packet, &com, sizeof(t_hdr));
      if (data != nullptr)
        std::memcpy(&packet[sizeof(t_hdr)], data, length);
        sock->write((void *)packet, (int)glength);
    }
    delete[] packet;
}

void	Command::basicResponse(int error)
{
  sendRequest(_code, &error, sizeof(error), _cli->getSocket());
}

IFile *getFile(std::string name)
{
  #if defined(WIN32) || defined(WIN64)
    return new WindowsFile(name);
  #endif
  #if __linux
  return new UnixFile(name);
  #endif
  #ifdef __APPLE__
    return new UnixFile(name);
  #endif
}

void	Command::registerUser()
{
  t_usrpwd	data;
  bool      exist;

  _cli->getSocket()->read(&data, sizeof(t_usrpwd));
  std::cout << "Register: " <<  data.user << std::endl;
  if ((exist = SEARCH(std::string(data.user))) == true)
  {
    basicResponse(err::USER_EXISTS);
    return;
  }
  IFile *file = getFile(std::string(data.user));
  file->setPassword(std::string(data.pwd));
  delete file;
  basicResponse(OK);
}

void	Command::login()
{
  t_usrpwd data;
  bool exist;

  _cli->getSocket()->read(&data, sizeof(t_usrpwd));
  std::cout << "login: " << data.user << std::endl;
  if ((exist = SEARCH(std::string(data.user))) == false)
  {
    basicResponse(err::NO_USER);
    return;
  }
  IFile *file = getFile(std::string(data.user));
  if ((file->getPassword().compare(data.pwd)) != 0)
  {
    basicResponse(err::BAD_PASSWORD);
    return;
  }
  for (std::list<Client *>::iterator it = _clients.begin(); it != _clients.end(); it++)
  {
    if ((*it)->getUserName().compare(data.user) == 0)
    {
      basicResponse(err::NO_USER);
      return;
    }
  }
_cli->setUserName(std::string(data.user));
_cli->SetContactList(file->getUsers());
basicResponse(OK);
sendnewConnexion();
int i = 1;
std::list<std::string> requests = file->getRequests();
for (std::list<std::string>::iterator it = requests.begin(); it != requests.end(); it++)
  {
    t_usr data;
    memset(data.user, 0, 64);
    strcpy(data.user, (*it).c_str());
    sendRequest(rsp::ADD_CONTACT, &data, sizeof(data), _cli->getSocket());
    file->delRequest(i++);
  }
  delete file;
}

void	Command::logout()
{
  std::cout << "Logout: " << _cli->getUserName() << std::endl;
  if(_cli->isCalling())
    quitCall();
  sendDeconnexion();
  _cli->clear();
}

void Command::getConvHistory()
{
  t_usr data;
  t_conv conv;

  _cli->getSocket()->read(&data, sizeof(t_usr));
  IFile *file = getFile(_cli->getUserName());
  const std::vector<t_usrtxt> msgs = file->getMessage(data.user);
  unsigned int i = -1;
  while (++i < msgs.size())
  {
    memset(conv.user1, 0, sizeof(conv.user1));
    memset(conv.user2, 0, sizeof(conv.user2));
    memset(conv.txt, 0, sizeof(conv.txt));
    strcpy(conv.user1, data.user);
    strcpy(conv.user2, msgs[i].user);
    strcpy(conv.txt, msgs[i].txt);
    sendRequest(_code, &conv, sizeof(conv), _cli->getSocket());
  }
}

void	Command::getContactList()
{
  std::list<std::string>::iterator it;
  STRING name;
  std::cout << "getContact " << _cli->getUserName() << std::endl;
  for (it = _cli->getContacts().begin(); it != _cli->getContacts().end(); ++it)
    {
      memset(name, 0, 64);
      strcpy(name, (*it).c_str());
      sendRequest(_code , name, sizeof(name), _cli->getSocket());
    }
}

void	Command::addContact()
{
  t_usr data;

  _cli->getSocket()->read(&data, sizeof(t_usr));
  std::cout << "addContact " <<  _cli->getUserName() << std::endl;
  if (_cli->getUserName().compare(data.user) == 0)
  {
    basicResponse(err::ALREADY_IN_CONTACTS);
    return;
  }
  for (std::list<std::string>::iterator it = _cli->getContacts().begin(); it != _cli->getContacts().end(); ++it)
  {
    if ((*it).compare(data.user) == 0)
    {
      basicResponse(err::ALREADY_IN_CONTACTS);
      return;
    }
  }
  for (std::list<Client *>::iterator it = _clients.begin(); it != _clients.end(); ++it)
  {
    if ((*it)->getUserName().compare(data.user) == 0)
    {
      basicResponse(OK);
      sendAddContact(*it, data);
      return;
    }
  }

bool exist;
if ((exist = SEARCH(std::string(data.user))) == false)
{
  basicResponse(err::NO_USER);
  return;
}
IFile *file = getFile(std::string(data.user));
file->addRequest(_cli->getUserName().c_str());
basicResponse(OK);
delete file;
}

void	Command::delContact()
{
  t_usr data;

  std::cout << "delContact " <<  _cli->getUserName() << std::endl;
  _cli->getSocket()->read(&data, sizeof(t_usr));
  bool exist;
  if ((exist = SEARCH(std::string(data.user))) == false)
  {
    basicResponse(err::NO_USER);
    return;
  }
  _cli->getContacts().remove(std::string(data.user));
  IFile *file = getFile(data.user);
  file->delUser(_cli->getUserName());
  IFile *file2 = getFile(_cli->getUserName());
  file2->delUser(data.user);
  delete file;
  delete file2;
  basicResponse(OK);
}

void	Command::repContact()
{
  t_usrrep data;

  _cli->getSocket()->read(&data, sizeof(t_usrrep));
  std::cout << "repContact " <<  _cli->getUserName() << std::endl;
  if (data.rep == OK)
  {
    IFile *file1 = getFile(data.user);
    IFile *file2 = getFile(_cli->getUserName());
    file1->addUser(_cli->getUserName());
    file2->addUser(data.user);
    _cli->addContact(std::string(data.user));
    delete file1;
    delete file2;
  }
  for (std::list<Client *>::iterator it = _clients.begin(); it != _clients.end(); ++it)
  {
    if ((*it)->getUserName().compare(data.user) == 0)
    {
      if (data.rep == OK)
      {
        (*it)->addContact(std::string(_cli->getUserName()));
      }
      sendRepContact(*it, data);
      return;
    }
  }
}

void Command::getIp(Client * & client, t_usrsck *sck)
{
  struct sockaddr_in addr;
  socklen_t addr_size = sizeof(struct sockaddr_in);
  getpeername(client->getSocket()->getFd(), (struct sockaddr *)&addr, &addr_size);
  memset(sck->ip, 0, 64);
  strcpy(sck->ip, inet_ntoa(addr.sin_addr));
}

void	Command::sendCall()
{
  t_usrrep data;
  std::list<Client *>::iterator it;

  std::cout << "sendCall " <<  _cli->getUserName() << std::endl;
  _cli->getSocket()->read(&data, sizeof(t_usrrep));
  for (it = _clients.begin(); it != _clients.end() ; ++it)
    {
      if ((*it)->getUserName().compare(data.user) == 0)
	     {
         if ((*it)->isCalling())
         {
           basicResponse(err::USER_IN_CALL);
           return;
         }
         t_usrsck data2;
         strcpy(data2.user, data.user);
         data2.port = data.rep;
         getIp(_cli, &data2);
         basicResponse(OK);
         sendSendCall(*it, data2);
         std::cout  << "A EXEC SENDSENDCALL" << std::endl;
         return;
       }
     }
    basicResponse(err::USER_OFFLINE);
}

void	Command::responseCall()
{
   t_usrrep data;
   std::list<Client *>::iterator it;

  std::cout << "responseCall " << _cli->getUserName() << std::endl;
  _cli->getSocket()->read(&data, sizeof(t_usrrep));
  for (it = _clients.begin(); it != _clients.end(); ++it)
    {
      if ((*it)->getUserName().compare(data.user) == 0)
      {
        if (data.rep == OK)
        {
          //cpy contacts
          _cli->setIsCalling(true);
          (*it)->setIsCalling(true);
          _cli->addUserInCall(*it);
          (*it)->addUserInCall(_cli);
        }
        sendResponseCall(*it, data);
	    }
   }
}

void	Command::quitCall()
{
  t_usr  data;

  std::cout << "endCall " <<  _cli->getUserName() << std::endl;
  std::vector<Client *> v = _cli->getUsersInCall();
  for (unsigned int i = 0; i < v.size() ; i++)
  {
    v[i]->delUserInCall(_cli);
    if (v[i]->getUsersInCall().size() == 0)
      v[i]->setIsCalling(false);
    sendEndCall(v[i], data);
  }
  _cli->clearCall();
  _cli->setIsCalling(false);
}


void	Command::endCall()
{
  t_usr  data;

  _cli->getSocket()->read(&data, sizeof(t_usr));
  std::cout << "endCall " <<  _cli->getUserName() << std::endl;

  std::vector<Client *> v = _cli->getUsersInCall();
  for (unsigned int i = 0; i < v.size() ; i++)
  {
    std::cout << "list: " << v[i]->getUserName() << std::endl;
    v[i]->delUserInCall(_cli);
    if (v[i]->getUsersInCall().size() == 0)
      v[i]->setIsCalling(false);
    sendEndCall(v[i], data);
  }
  _cli->clearCall();
  _cli->setIsCalling(false);
}

void	Command::sendText()
{
  t_usrtxt data;
  std::cout << "sendText " <<  _cli->getUserName() << std::endl;
  _cli->getSocket()->read(&data, sizeof(t_usrtxt));
  for (std::list<Client *>::iterator it = _clients.begin(); it != _clients.end(); ++it)
    {
      if ((*it)->getUserName().compare(data.user) == 0)
      {
        basicResponse(OK);
        IFile *file1 = getFile(_cli->getUserName());
        file1->addMessage(_cli->getUserName(), data.user, data.txt);
        IFile *file = getFile(std::string(data.user));
        file->addMessage(_cli->getUserName(), data.user, data.txt);
        delete file;
        delete file1;
        sendReceiveText(*it, data);
        return;
      }
    }

  bool exist;
  if ((exist = SEARCH(std::string(data.user))) == false)
  {
    basicResponse(err::NO_USER);
    return;
  }
  IFile *file1 = getFile(_cli->getUserName());
  file1->addMessage(_cli->getUserName(), data.user, data.txt);
  IFile *file = getFile(std::string(data.user));
  file->addMessage(_cli->getUserName(), data.user, data.txt);
  basicResponse(OK);
  delete file;
  delete file1;
}

void	Command::getAllContact()
{
  std::cout << "getAllContact " <<  _cli->getUserName() << std::endl;

  std::list<std::string> contacts = GETUSER;
  for (std::list<std::string>::iterator it = contacts.begin(); it != contacts.end(); ++it)
  {
        STRING name;
        memset(name, 0, 64);
        strcpy(name, (*it).c_str());
        sendRequest(_code, name, sizeof(name), _cli->getSocket());
   }
}

void	Command::getContactOnline()
{
  STRING name;

  std::cout << "getContactOnline" << std::endl;
  for (std::list<std::string>::iterator it = _cli->getContacts().begin(); it != _cli->getContacts().end(); ++it)
    {
      for (std::list<Client *>::iterator it2 = _clients.begin(); it2 != _clients.end(); ++it2)
      {
        if (((*it).compare((*it2)->getUserName())) == 0)
        {
          memset(name, 0, 64);
          strcpy(name, (*it).c_str());
          sendRequest(_code , name, sizeof(name), _cli->getSocket());
        }
      }
    }
}

void Command::sendAddContact(Client * &client, t_usr data)
{
  memset(data.user, 0, 64);
  strcpy(data.user, _cli->getUserName().c_str());
  sendRequest(_code + 100, &data, sizeof(data), client->getSocket());
}

void Command::sendRepContact(Client * &client, t_usrrep data)
{
  memset(data.user, 0, 64);
  strcpy(data.user, _cli->getUserName().c_str());
  sendRequest(_code + 100, &data, sizeof(data), client->getSocket());
}

void Command::sendSendCall(Client * &client, t_usrsck data)
{
    memset(data.user, 0, 64);
    strcpy(data.user, _cli->getUserName().c_str());
    sendRequest(_code + 100, &data, sizeof(data), client->getSocket());
}

void Command::sendResponseCall(Client * &client, t_usrrep data)
{
  memset(data.user, 0, 64);
  strcpy(data.user, _cli->getUserName().c_str());
  sendRequest(_code + 100, &data, sizeof(data), client->getSocket());
}

void Command::sendEndCall(Client *&client, t_usr data)
{
  std::cout << "SEND END CALL" << std::endl;
    memset(data.user, 0, 64);
    strcpy(data.user, _cli->getUserName().c_str());
    std::cout << "to : " << client->getUserName() << " data: " << data.user << std::endl;
    sendRequest(_code + 100, &data, sizeof(data), client->getSocket());
}

void Command::sendReceiveText(Client *&client, t_usrtxt data)
{
  memset(data.user, 0, 64);
  strcpy(data.user, _cli->getUserName().c_str());
  sendRequest(_code + 100, &data, sizeof(data), client->getSocket());
}

void Command::sendnewConnexion()
{
  for (std::list<std::string>::iterator it = _cli->getContacts().begin(); it != _cli->getContacts().end(); ++it)
  {
    for (std::list<Client *>::iterator it2 = _clients.begin(); it2 != _clients.end(); ++it2)
      {
        if ((*it).compare((*it2)->getUserName()) == 0)
          {
            STRING name;
            memset(name, 0, 64);
            strcpy(name, _cli->getUserName().c_str());
            sendRequest(rsp::NEW_CONN , name, sizeof(name), (*it2)->getSocket());
          }
      }
  }
}

void Command::sendDeconnexion()
{
  for (std::list<std::string>::iterator it = _cli->getContacts().begin(); it != _cli->getContacts().end(); ++it)
  {
    for (std::list<Client *>::iterator it2 = _clients.begin(); it2 != _clients.end(); ++it2)
      {
        if ((*it).compare((*it2)->getUserName()) == 0)
          {
              STRING name;
              memset(name, 0, 64);
              strcpy(name, _cli->getUserName().c_str());
              sendRequest(rsp::DECONN , name, sizeof(name), (*it2)->getSocket());
          }
      }
  }
}
