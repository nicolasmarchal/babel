#pragma once

#ifndef AGUI_HH_
# define AGUI_HH_

# include <iostream>
# include <chrono>
# include "IGUI.hh"
# include "protocol.hh"

// forward declaration
class Client;

class IEventManager
{
public:
    virtual ~IEventManager();
    virtual void processEvents() = 0;
};

class AGUI : public IGUI
{
public:
	AGUI(Client &c);
	virtual ~AGUI();
	virtual void init() = 0;
	virtual void onRegister(void *) = 0;
	virtual void onLogin(void *) = 0;
	virtual void onLogout(void *) = 0;
	virtual void onGetContactList(void *) = 0;
	virtual void onAddContact(void *) = 0;
	virtual void onDelContact(void *) = 0;
	virtual void onSendCall(void *) = 0;
	virtual void onSendText(void *) = 0;
	virtual void onGetOnlineContact(void *) = 0;
	virtual void onAddContactRequest(void *) = 0;
	virtual void onRepContactRequest(void *) = 0;
	virtual void onRepCallRequest(void *) = 0;
	virtual void onEndCallRequest(void *) = 0;
	virtual void onReceiveText(void *) = 0;
	virtual void onSendCallRequest(void *) = 0;
	virtual void onNewConn(void *) = 0;
	virtual void onDeconn(void *) = 0;
	virtual void onError(void *) = 0;
	virtual void onGetAllContact(void *) = 0;
  virtual void onGetConvHistory(void *) = 0;

	void operator()();
	Client &client;
};

#endif /* !AGUI_HH_ */
