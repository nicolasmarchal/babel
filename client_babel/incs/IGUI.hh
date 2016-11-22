#pragma once

#ifndef IGUI_HH_
# define IGUI_HH_

# include "protocol.hh"

class IGUI
{
public:
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
	virtual void onGetAllContact(void *) = 0;
	virtual void onError(void *) = 0;
	virtual void onGetConvHistory(void *) = 0;
};

#endif /* !IGUI_HH_ */
