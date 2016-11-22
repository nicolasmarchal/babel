#include <iostream>
#include <QScrollBar>
#include <QVBoxLayout>
#include "Client.hh"
#include "QTGUI.hh"
#include "customitem.h"
#include "form.h"
#include "ui_form.h"



Form::Form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form)
{
    ui->setupUi(this);
    QScrollBar * bar = new QScrollBar;
    QVBoxLayout * vbox= new QVBoxLayout;

    _err = new DialogError;
    _callInfo = new callItem;
    _requestAdd = new DialogRequest;
    _requestCall = new DialogCall;
    _requestCall->_callinfo = _callInfo;
    layout = new QVBoxLayout;

    vbox->setSpacing(0);
    vbox->setMargin(0);
    vbox->addWidget(_callInfo);

    layout->setMargin(0);
    layout->setSpacing(0);
    ui->conv_box->setLayout(layout);
    ui->call_info->setLayout(vbox);
    ui->listWidget->setVerticalScrollBar(bar);
    ui->listWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->listWidget->setFocusPolicy(Qt::FocusPolicy::NoFocus);
    ui->label_2->setPixmap(ui->label_2->pixmap()->scaledToWidth(ui->label_2->width(), Qt::SmoothTransformation));
    ui->label_3->setPixmap(ui->label_3->pixmap()->scaledToWidth(ui->label_3->width(), Qt::SmoothTransformation));
    ui->noContact->hide();

    connect(ui->textField, SIGNAL(returnPressed()), ui->sendButton, SIGNAL(clicked()));

    bar->setStyleSheet(BAR_STYLESHEET);

    refreshingContacts();
}

Form::~Form()
{

    delete ui;
}

void Form::deletingContacts()
{
    for (std::map<std::string, CustomItem *>::iterator it = _items.begin() ; it != _items.end() ; ++it)
    {
       delete (it->second);
    }

    for (std::map<std::string, Messages *>::iterator it = _messages.begin() ; it != _messages.end() ; ++it)
    {
      delete (it->second);
    }

   _convLoad.clear();
   _messages.clear();
   _items.clear();
   _contacts.clear();
   ui->listWidget->clear();
}

void Form::refreshingContacts()
{
    CustomItem *myitem;
    QListWidgetItem *item;
    std::vector<std::string>::iterator it;

    if (_contacts.empty())
    {
        ui->noContact->setText("No contacts yet");
        ui->noContact->show();
        ui->call_info->hide();
        _current = NULL;
        ui->textField->hide();
        ui->sendButton->hide();
        return;
    } else {
        ui->noContact->hide();
        ui->call_info->show();
        _callInfo->show();
        ui->sendButton->show();
        ui->textField->show();
    }

    for (it = _contacts.begin() ; it != _contacts.end() ; ++it) {

        if (_items.find(*it) != _items.end())
            continue;
        myitem = new CustomItem(NULL);
        myitem->setName(*it);
        item = new QListWidgetItem();
        item->setSizeHint(QSize(0, 65));
        item->setWhatsThis(QString::fromStdString(*it));
        ui->listWidget->addItem(item);
        ui->listWidget->setItemWidget(item, myitem);
        _messages[*it] = new Messages;
        _items[*it] = myitem;
        _items[*it]->setOffline();
    }

    ui->listWidget->item(0)->setSelected(true);
    on_listWidget_itemClicked(ui->listWidget->item(0));
}

void Form::on_listWidget_itemClicked(QListWidgetItem *item)
{
    _usernameOnFocus = item->whatsThis().toStdString();
    _callInfo->changeUser(item->whatsThis().toStdString());
    if (_convLoad.find(item->whatsThis().toStdString()) == _convLoad.end())
    {
        _convLoad[item->whatsThis().toStdString()] = true;
        _gui->client.doGetConvHistory(item->whatsThis().toStdString());
    }
    if (_current != NULL)
    {
        _current->hide();
        layout->removeWidget(_current);
    }
    layout->addWidget(_messages[item->whatsThis().toStdString()]);
    _current = _messages[item->whatsThis().toStdString()];
    _current->show();
    layout->update();
}

void Form::setContactOnline(const std::string & contact)
{
    if (_items.find(contact) != _items.end())
    {
        _items[contact]->setOnline();
        return;
    }
    std::cout << "Contact " << contact << " not found in list" << std::endl;
}

void Form::setContactOffline(const std::string & contact)
{
    if (_items.find(contact) != _items.end())
    {
        _items[contact]->setOffline();
        return;
    }
    std::cout << "Contact " << contact << " not found in list" << std::endl;
}

void Form::on_addContactButton_clicked()
{
    dialog = new DialogBasic;
    dialog->setGui(_gui);
    dialog->show();
}

void Form::setGui(QTGUI * gui)
{
    _gui = gui;
    _callInfo->setGui(gui);
}

void Form::onDelSuccess()
{
    _callInfo->destroyDialog();
    deletingContacts();
    _gui->client.doGetContacts();
    _gui->client.doGetOnlineContacts();
    if (_contacts.empty())
    {
        _current = NULL;
        _callInfo->hide();
    }
}

void Form::onAddSuccess()
{
    delete dialog;
}

void Form::onAddError(unsigned int error)
{
    dialog->onAddError(error);
}

void Form::pushContact(const std::string & contact)
{
    _contacts.push_back(contact);
    refreshingContacts();
    _gui->client.doGetOnlineContacts();
}

void Form::displayUsername()
{
    ui->label_4->setText(QString::fromStdString(_gui->username));
}

void Form::contactRequest(const std::string & contact)
{
    _requestAdd->setGui(_gui);
    _requestAdd->personify(contact);
    _requestAdd->show();
}

void Form::displayText(const std::pair<std::string, std::string> & text)
{
    if (_messages.find(text.first) != _messages.end())
        _messages[text.first]->pushText(text);
}

void Form::displayHistory(const std::string & user, const std::pair<std::string, std::string> &text)
{
    _messages[user]->pushText(text);
}

void Form::on_sendButton_clicked()
{
    std::string text = ui->textField->text().toStdString();
    if (text.size() == 0)
        return;
    std::pair<std::string, std::string> myPair(ui->label_4->text().toStdString(), text);

    ui->textField->setText("");
    _messages[this->_usernameOnFocus]->pushText(myPair);
    _gui->client.doSendText(this->_usernameOnFocus, text);
}

void Form::on_pushButton_clicked()
{
    _gui->client.doLogout();
}

void Form::closeEvent(QCloseEvent *event)
{
    _gui->client.doLogout();
    QWidget::closeEvent(event);
}

void Form::contactIsCalling(const std::string & contact, t_usrsck * socket)
{
  _requestCall->setGui(_gui);
  _requestCall->personify(contact);
  _requestCall->socket = socket;
  _requestCall->show();
}

void Form::onUnexpectedError(const std::string & error)
{
    _err->setMessage(error);
    _err->show();
}

void Form::acceptingCall(const std::string & user)
{
    _callInfo->startingCall();
}

void Form::denyingCall(const std::string & user)
{
  _callInfo->stopCall();
  onUnexpectedError(std::string("The user ") + user + std::string(" denied your call !"));
}

void Form::endCall()
{
   _callInfo->stopCall();
}

void Form::onTryCallSuccess()
{
    _callInfo->onResponseCall();
}
