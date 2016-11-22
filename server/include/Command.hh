#ifndef COMMAND_HH_
# define COMMAND_HH_

#include <map>
#include <list>
#include "protocol.hh"
#include "Client.hh"
#include "Packet.hh"
#include "IFile.hh"
#include "UnixFile.hh"
#include "WindowsFile.hh"

class Command
{
public:
  Command(Client * &, std::list<Client *> &, int code);
  ~Command();
  void execCmd();
  void	quitCall();

private:
void getIp(Client * & client, t_usrsck *sck);

  void	registerUser();
  void	login();
  void	logout();
  void	getContactList();
  void	addContact();
  void	delContact();
  void	repContact();
  void	sendCall();
  void	responseCall();
  void	endCall();
  void	sendText();
  void	getAllContact();
  void	getContactOnline();
  void	basicResponse(int response);
  void  getConvHistory();


  void sendAddContact(Client * &client, t_usr);
  void sendRepContact(Client * &client, t_usrrep);
  void sendSendCall(Client * &, t_usrsck data);
  void sendResponseCall(Client * &client, t_usrrep);
  void sendEndCall(Client *&client, t_usr data);
  void sendReceiveText(Client *&client, t_usrtxt data);
  void sendnewConnexion();
  void sendDeconnexion();


private:
  std::map<int, void (Command::*)()> _cmd;
  Client *_cli;
  std::list<Client *> _clients;
  int _code;
};


#endif
