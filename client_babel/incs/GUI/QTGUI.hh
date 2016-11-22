#ifndef QTGUI_H
#define QTGUI_H
#include <QApplication>
#include <map>
#include <iostream>
#include "AGUI.hh"
#include "stdafx.h"
#include "form.h"
#include "login.h"
#include "init.h"

class QTEventManager : public IEventManager
{
public:
    QTEventManager(int ac, char **av);
    ~QTEventManager();
    void processEvents();
private:
    QApplication app;
};

class QTGUI : public AGUI
{
public:
    QTGUI(Client &client);
    ~QTGUI();

public:
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

public:
    Login * _loginWindow;
    Form * _mainWindow;
    QApplication * _app;
    Init * _initWindow;

public:
    void showMainWindow();

public:
    std::string username;
};

#endif // QTGUI_H
