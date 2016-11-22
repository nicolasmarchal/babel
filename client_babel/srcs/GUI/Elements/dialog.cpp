#include "QTGUI.hh"
#include "Client.hh"
#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::setUsername(const std::string & username)
{
    _username = username;
}

void Dialog::on_pushButton_clicked()
{
    _gui->client.doDelContact(_username);
}
