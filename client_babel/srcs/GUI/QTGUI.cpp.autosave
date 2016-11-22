#include <QCoreApplication>
#include "Client.hh"
#include "QTGUI.hh"

QTEventManager::QTEventManager(int ac, char **av)
    : app(ac, av)
{
    app.setWindowIcon(QIcon(":/Images/icon.ico"));
}

QTEventManager::~QTEventManager()
{}

void
QTEventManager::processEvents()
{
  app.processEvents();
}

QTGUI::QTGUI(Client &c) :
   AGUI(c)
{
}

QTGUI::~QTGUI()
{
}

void QTGUI::init()
{
   Login * loginWindow = new  Login;
   Form * mainWindow = new Form;
   Init * initWindow = new Init;

   this->_initWindow = initWindow;
   this->_initWindow->setGui(this);
   this->_loginWindow = loginWindow;
   this->_loginWindow->setGui(this);
   this->_mainWindow = mainWindow;
   this->_mainWindow->setGui(this);
   this->_initWindow->show();
}

void QTGUI::onRegister(void * data)
{
    t_rsp * rsp = (t_rsp *)data;

    if (rsp->status == OK)
    {
       this->_loginWindow->successAtRegister();
    } else {
        this->_loginWindow->errorAtRegister(rsp->status);
    }
    delete rsp;
}

void QTGUI::onLogin(void * data)
{
    t_rsp * rsp = (t_rsp *)data;

    if (rsp->status == OK)
    {
        this->_loginWindow->hide();
        this->client.doGetContacts();
        this->client.doGetOnlineContacts();
        this->_mainWindow->displayUsername();
        this->showMainWindow();
    } else {
        this->_loginWindow->errorAtLogin(rsp->status);
    }
    delete rsp;
}

void QTGUI::onLogout(void * data)
{
  _mainWindow->hide();
  delete _mainWindow;
  delete _loginWindow;
  delete _initWindow;
  QApplication::quit();
  client.quit();
}

void QTGUI::onGetContactList(void * data)
{
    t_usr * usr  = (t_usr *)data;

    _mainWindow->pushContact(usr->user);
}

void QTGUI::onAddContact(void * data)
{
    t_rsp *rsp = (t_rsp *)data;

    if (rsp->status == OK)
    {
        _mainWindow->onAddSuccess();
    }
    else
    {
        _mainWindow->onAddError(rsp->status);
    }
    delete rsp;
}

void QTGUI::onDelContact(void * data)
{
    t_rsp *rsp = (t_rsp *)data;

    _mainWindow->onDelSuccess();

    delete rsp;
}

void QTGUI::onSendCall(void * data)
{
  t_rsp *rsp = (t_rsp *)data;
  std::string * response = new std::string;

	if (rsp->status == err::USER_OFFLINE)
    {
        *response = "User is currently offline";
        onError(response);

    }
    else if (rsp->status == err::USER_IN_CALL)
    {
        *response = "User is already in call";
        onError(response);
    }
    else
        _mainWindow->onTryCallSuccess();
	delete rsp;
}

void QTGUI::onSendText(void * data)
{
}

void QTGUI::onGetOnlineContact(void * data)
{
  t_usr * usr  = (t_usr *)data;

  _mainWindow->setContactOnline(usr->user);
}

void QTGUI::onAddContactRequest(void * data)
{
  t_usr *usr = (t_usr *)data;

  _mainWindow->contactRequest(usr->user);

  delete usr;
}

void QTGUI::onRepContactRequest(void * data)
{
  t_usrrep *ur = (t_usrrep *)data;

	if (ur->rep == OK)
  {
    _mainWindow->pushContact(ur->user);
  }
  delete ur;
}

void QTGUI::onRepCallRequest(void * data)
{
  t_usrrep *up = (t_usrrep *)data;

  if (up->rep == 0)
    {
      _mainWindow->acceptingCall(up->user);
    }
  else
    _mainWindow->denyingCall(up->user);
  delete up;
}

void QTGUI::onEndCallRequest(void * data)
{
  t_usr *usr = (t_usr *)data;

  _mainWindow->endCall();
  delete usr;
}

void QTGUI::onReceiveText(void * data)
{
  t_usrtxt *ut = (t_usrtxt *)data;

  _mainWindow->displayText(std::pair<std::string, std::string>(ut->user, ut->txt));
  delete ut;
}

void QTGUI::onSendCallRequest(void * data)
{
  t_usrsck 	*us = (t_usrsck *)data;
  std::string rep;

  _mainWindow->contactIsCalling(us->user, us);
}

void QTGUI::onNewConn(void * data)
{
  t_usr *usr = (t_usr *)data;

  _mainWindow->setContactOnline(usr->user);
	delete usr;
}

void QTGUI::onDeconn(void * data)
{
  t_usr * usr = (t_usr *)data;

  _mainWindow->setContactOffline(usr->user);
  delete usr;
}

void QTGUI::onGetConvHistory(void * data)
{
  t_conv * conv = (t_conv *)data;
  std::pair<std::string, std::string> text(conv->user2, conv->txt);

  _mainWindow->displayHistory(conv->user1, text);
}

void QTGUI::onGetAllContact(void  * data)
{
}

void QTGUI::onError(void * data)
{
  std::string *err = (std::string *)data;

  _mainWindow->onUnexpectedError(*err);
	delete err;
}

void QTGUI::showMainWindow()
{
    _mainWindow->show();
}
