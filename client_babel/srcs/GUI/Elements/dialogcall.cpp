#include "QTGUI.hh"
#include "Client.hh"
#include "dialogcall.h"
#include "ui_dialogcall.h"

DialogCall::DialogCall(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogCall)
{
    ui->setupUi(this);
}

DialogCall::~DialogCall()
{
    delete ui;
}

void DialogCall::personify(const std::string & user)
{
    std::string message = std::string("The user ") + user + std::string("\nwants to start a call, do you accept ?");

    ui->label->setText(QString::fromStdString(message));

    this->_userToRespond = user;
}

void DialogCall::on_pushButton_2_clicked()
{
    _gui->client.doResponseCall(socket, _userToRespond, false);
    _callinfo->startingCall();
    this->hide();
}

void DialogCall::on_pushButton_clicked()
{
    _gui->client.doResponseCall(socket, _userToRespond, true);
    this->hide();
}
