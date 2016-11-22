#pragma once

#ifndef TERMGUI_HH_
# define TERMGUI_HH_

# include <cstring>
# include <thread>
# include "stdafx.h"
# include "AGUI.hh"

class TermEventManager : public IEventManager
{
public:
    TermEventManager(int ac, char **av);
    ~TermEventManager();
    void processEvents();
};

class TermGUI : public AGUI
{
public:
	TermGUI(Client &c);
	~TermGUI();
	void init();

	void onRegister(void *);
	void onLogin(void *);
	void onLogout(void *);
	void onGetContactList(void *);
	void onAddContact(void *);
	void onDelContact(void *);
	void onSendCall(void *);
	void onSendText(void *);
	void onGetOnlineContact(void *);
	void onAddContactRequest(void *);
	void onRepContactRequest(void *);
	void onRepCallRequest(void *);
	void onEndCallRequest(void *);
	void onReceiveText(void *);
	void onSendCallRequest(void *);
	void onNewConn(void *);
	void onDeconn(void *);
	void onError(void *);
	void onGetAllContact(void *);
  void onGetConvHistory(void *);
private:
	void read();

	void doRegister();
	void doLogin();
	void doLogout();
	void doGetContacts();
	void doGetOnlineContacts();
	void doAddContact();
	void doDelContact();
	void doSendCall();
	void doEndCall();
	void doSendText();
	void doQuit();
	void doConnect();

	std::thread *rt;
	bool open;
  void *currData;
  bool isRespondingContact;
  bool isRespondingSendCall;
};

typedef void (TermGUI::*TermGUIFunc)(void);

#endif /* !TERMGUI_HH_ */
