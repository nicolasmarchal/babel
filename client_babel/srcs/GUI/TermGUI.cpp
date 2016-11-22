#include "stdafx.h"
#include "TermGUI.hh"
#include "Client.hh"

TermEventManager::TermEventManager(int ac, char **av)
{
	(void)ac;
	(void)av;
}

TermEventManager::~TermEventManager()
{}

void
TermEventManager::processEvents()
{
}


TermGUI::TermGUI(Client &c)
	: AGUI(c), open(true)
{
	isRespondingContact = false;
	isRespondingSendCall = false;
	rt = new std::thread(&TermGUI::read, this);
}

TermGUI::~TermGUI()
{
	rt->join();
	delete rt;
}

void
TermGUI::read()
{
	bool exec;

	std::string cmd("");
	std::string cmds[12] = {
		"/register",
		"/quit",
		"/get_contacts",
		"/add_contact",
		"/login",
		"/logout",
		"/del_contact",
		"/send_call",
		"/send_text",
		"/get_contact_online",
		"/end_call",
		"/connect"
	};
	TermGUIFunc funcs[12] = {
		&TermGUI::doRegister,
		&TermGUI::doQuit,
		&TermGUI::doGetContacts,
		&TermGUI::doAddContact,
		&TermGUI::doLogin,
		&TermGUI::doLogout,
		&TermGUI::doDelContact,
		&TermGUI::doSendCall,
		&TermGUI::doSendText,
		&TermGUI::doGetOnlineContacts,
		&TermGUI::doEndCall,
		&TermGUI::doConnect
	};

	while (open)
	{
		exec = false;
		std::cin >> cmd;
		if (isRespondingContact)
			{
				client.doRepContact(((t_usr *)currData)->user, cmd == "N");
				isRespondingContact = false;
			}
		else if (isRespondingSendCall)
			{
				t_usrsck *us = (t_usrsck *)currData;

				client.doResponseCall(us, us->user, cmd == "N");
				isRespondingSendCall = false;
			}
		else
		{
			for (int i = 0; i < 12; i++)
			{
				if (cmds[i] == cmd)
				{
					exec = true;
					(this->*funcs[i])();
				}
			}
			if (!exec)
				std::cout << "-> Unkown command" << std::endl;
		}
	}
	std::cout << "-> Program quit" << std::endl;
	client.quit();
}

void
TermGUI::init()
{
	std::cout << "=== Welcome to the Babel ===" << std::endl;
}

void
TermGUI::doGetContacts()
{
	client.doGetContacts();
}

void
TermGUI::doGetOnlineContacts()
{
	client.doGetOnlineContacts();
}

void
TermGUI::doAddContact()
{
	std::string user;

	std::cin >> user;
	client.doAddContact(user);
}

void
TermGUI::doRegister()
{
	std::string user;
	std::string pwd;

	std::cin >> user >> pwd;
	client.doRegister(user, pwd);
}

void
TermGUI::doLogin()
{
	std::string user;
	std::string pwd;

	std::cin >> user >> pwd;
	client.doLogin(user, pwd);
}

void
TermGUI::doLogout()
{
	std::cout << "-> Logged out" << std::endl;
	client.doLogout();
}

void
TermGUI::doDelContact()
{
	std::string user;

	std::cin >> user;
	client.doDelContact(user);
}

void
TermGUI::doSendCall()
{
	std::string user;

	std::cin >> user;
	client.doSendCall(user);
}

void
TermGUI::doSendText()
{
	std::string text;
	std::string user;

	std::cin >> user >> text;
	client.doSendText(user, text);
}

void
TermGUI::doEndCall()
{
	std::cout << "-> End Call" << std::endl;
	client.doEndCall();
}

void
TermGUI::doQuit()
{
	open = false;
}

void
TermGUI::doConnect()
{
	std::string ip;
	unsigned int port;

	std::cin >> ip >> port;
	if (client.connect(ip, port) == 0)
		std::cout << "-> Connection failed" << std::endl;
	else
		std::cout << "-> Connection successful" << std::endl;
}

void
TermGUI::onRegister(void *data)
{
	t_rsp *rsp = (t_rsp *)data;

	if (rsp->status == OK)
	{
		std::cout << "-> Registered" << std::endl;
	}
	else
		std::cout << "-> User already exists" << std::endl;
	delete rsp;
}

void
TermGUI::onLogin(void *data)
{
	t_rsp *rsp = (t_rsp *)data;

	if (rsp->status == OK)
	{
		std::cout << "-> Logged" << std::endl;
	}
	else
		std::cout << "-> Not Logged" << std::endl;
	delete rsp;
}

void
TermGUI::onLogout(void *data)
{
	(void)data;
	std::cout << "-> Logged out" << std::endl;
}

void
TermGUI::onGetContactList(void *data)
{
	t_usr *usr = (t_usr *)data;

	std::cout << "-> " << usr->user << std::endl;
	delete usr;
}

void
TermGUI::onGetOnlineContact(void *data)
{
	t_usr *usr = (t_usr *)data;

	std::cout << "-> connected: " << usr->user << std::endl;
	delete usr;
}

void
TermGUI::onAddContact(void *data)
{
	(void)data;
}

void
TermGUI::onDelContact(void *data)
{
	t_rsp *rsp = (t_rsp *)data;

	if (rsp->status == err::NO_USER)
		std::cout << "-> No user" << std::endl;
	else
		std::cout << "-> User deleted" << std::endl;
	delete rsp;
}

void
TermGUI::onSendCall(void *data)
{
  t_rsp *rsp = (t_rsp *)data;

	if (rsp->status == err::USER_OFFLINE)
		std::cout << "-> User offline" << std::endl;
	else if (rsp->status == err::USER_IN_CALL)
		std::cout << "-> User already in call" << std::endl;
	else
		std::cout << "-> User asked" << std::endl;
	delete rsp;
}

void
TermGUI::onSendText(void *data)
{
	t_rsp *rsp = (t_rsp *)data;

	if (rsp->status == err::NO_USER)
		std::cout << "-> No user" << std::endl;
	else
		std::cout << "-> Send call successful" << std::endl;
	delete rsp;
}

void
TermGUI::onAddContactRequest(void *data)
{
	t_usr *usr = (t_usr *)data;
	std::string rep;

	std::cout << "-> User " << usr->user << " wants to add you, accept ? (Y | N)" << std::endl;
	isRespondingContact = true;
	currData = (void *)usr;
	// delete usr;
}

void
TermGUI::onRepContactRequest(void *data)
{
	t_usrrep *ur = (t_usrrep *)data;

	if (ur->rep == 0)
		std::cout << "-> user " << ur->user << " accepts you" << std::endl;
	else
		std::cout << "-> user " << ur->user << " doesn't want you" << std::endl;
	delete ur;
}

void
TermGUI::onRepCallRequest(void *data)
{
	t_usrrep *up = (t_usrrep *)data;

	if (up->rep == 0)
		std::cout << "-> Call start with user " << up->user << std::endl;
	else
		std::cout << "-> user " << up->user << " doesn't respond" << std::endl;
	delete up;
}

void
TermGUI::onEndCallRequest(void *data)
{
	t_usr *usr = (t_usr *)data;

	std::cout << "-> Call ended with user " << usr->user << std::endl;
	delete usr;
}

void
TermGUI::onReceiveText(void *data)
{
	t_usrtxt *ut = (t_usrtxt *)data;

	std::cout << "-> User " << ut->user << " says: " << ut->txt << std::endl;
	delete ut;
}

void
TermGUI::onSendCallRequest(void *data)
{
	t_usrsck 	*us = (t_usrsck *)data;
	std::string rep;

	std::cout << "-> User " << us->user << " wants to speak with you, accept ? (Y | N)" << std::endl;
	isRespondingSendCall = true;
	currData = (void *)us;
	// delete us;
}

void
TermGUI::onNewConn(void *data)
{
	t_usr *usr = (t_usr *)data;

	std::cout << "-> User " << usr->user << " connected" << std::endl;
	delete usr;
}

void
TermGUI::onDeconn(void *data)
{
	t_usr *usr = (t_usr *)data;

	std::cout << "-> User " << usr->user << " disconnected" << std::endl;
	delete usr;
}

void
TermGUI::onError(void *data)
{
	std::string *err = (std::string *)data;

	std::cout << "-> ERROR: " << *err << std::endl;
	delete err;
}

void
TermGUI::onGetAllContact(void *data)
{
	t_usr *usr = (t_usr *)data;

	std::cout << "-> " << usr->user << std::endl;
	delete usr;
}

void
TermGUI::onGetConvHistory(void *data)
{
	(void)data;
	std::cout << "Je recois l'histo" << std::endl;
}
