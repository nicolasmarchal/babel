#include <iostream>
#include <QTGUI.hh>
#include "Client.hh"
#include "dialogrequest.h"
#include "ui_dialogrequest.h"

DialogRequest::DialogRequest(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogRequest)
{
    ui->setupUi(this);
}

DialogRequest::~DialogRequest()
{
    delete ui;
}

void DialogRequest::personify(const std::string & user)
{
    std::string message = std::string("The user ") + user + std::string("\nwants to add you to his contacts, do you accept ?");

    ui->label->setText(QString::fromStdString(message));

    this->userAdd = user;
}

void DialogRequest::on_pushButton_2_clicked()
{
    _gui->client.doRepContact(this->userAdd, false);
    _gui->_mainWindow->pushContact(this->userAdd);
    this->hide();
}

void DialogRequest::on_pushButton_clicked()
{
    _gui->client.doRepContact(this->userAdd, true);
    this->hide();
}

void DialogRequest::closeEvent(QCloseEvent * event)
{
    _gui->client.doRepContact(this->userAdd, true);
    QDialog::closeEvent(event);
}
