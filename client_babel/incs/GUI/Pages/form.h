#ifndef FORM_H
#define FORM_H

#include <QWidget>
#include <QListWidgetItem>
#include <QVBoxLayout>
#include "protocol.hh"
#include "customitem.h"
#include "dialogbasic.h"
#include "callitem.h"
#include "dialogrequest.h"
#include "messages.h"
#include "dialogcall.h"
#include "dialogerror.h"

class QTGUI;

namespace Ui {
class Form;
}

class Form : public QWidget
{
    Q_OBJECT

public:
    explicit Form(QWidget *parent = 0);
    ~Form();

private slots:
    void on_listWidget_itemClicked(QListWidgetItem *item);
    void on_addContactButton_clicked();
    void on_sendButton_clicked();
    void on_pushButton_clicked();

private:
    std::vector<std::string> _contacts;
    int _currentIndex;
    std::string _username;
    std::map<std::string, CustomItem * > _items;
    std::map<std::string, Messages *> _messages;
    std::map<std::string, bool> _convLoad;
    DialogBasic * dialog;
    callItem * _callInfo;
    DialogRequest * _requestAdd;
    DialogError * _err;

    QVBoxLayout * layout;
    Messages * _current;
    std::string _usernameOnFocus;
    DialogCall  * _requestCall;
    QTime startTime;
    QTimer t;

public:
    void setContactOnline(const std::string &);
    void setContactOffline(const std::string &);
    void setGui(QTGUI *);
    void onAddSuccess();
    void onAddError(unsigned int);
    void onDelSuccess();
    void refreshingContacts();
    void deletingContacts();
    void pushContact(const std::string &);
    void contactRequest(const std::string &);
    void displayUsername();
    void displayText(const std::pair<std::string, std::string> &);
    void displayHistory(const std::string &, const std::pair<std::string, std::string> &);
    void closeEvent(QCloseEvent *);
    void contactIsCalling(const std::string &, t_usrsck *);
    void onUnexpectedError(const std::string &);
    void denyingCall(const std::string &);
    void acceptingCall(const std::string &);
    void onTryCallSuccess();
    void endCall();

private:
    Ui::Form * ui;
    QTGUI * _gui;
};

#define BAR_STYLESHEET "QScrollBar\
{\
    background: none\
}\
\
QScrollBar:handle\
{\
    background-color: rgba(0,0,0, 0.2);\
    border-radius: 7px;\
}\
\
QScrollBar:vertical {\
    border: none;\
}\
QScrollBar::add-line:vertical {\
    height: 0px;\
}\
QScrollBar::sub-line:vertical {\
    height: 0px;\
}\
\
QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {\
background: none;\
}"

#endif // FORM_H
