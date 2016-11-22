#include <QSize>
#include "Client.hh"
#include "QTGUI.hh"
#include "dialogbasic.h"
#include "ui_dialogbasic.h"


DialogBasic::DialogBasic(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogBasic),
    mov(new QMovie(":/Form/Images/Form/loading.gif"))
{
    ui->setupUi(this);
    ui->label_3->setMovie(mov);
    mov->setScaledSize(QSize(ui->label_3->width(), ui->label_3->height()));
    mov->stop();
    ui->label_3->hide();
}

DialogBasic::~DialogBasic()
{
    delete ui;
}

void DialogBasic::on_addButton_clicked()
{
    std::string _username = ui->username->text().toStdString();

    if (_username.size() == 0)
    {
        ui->label_2->setText("Username field is required");
        return;
    }
    ui->label_3->show();
    mov->start();
    _gui->client.doAddContact(_username);
}

void DialogBasic::onAddError(unsigned int error)
{
    mov->stop();
    ui->label_3->hide();
    switch (error) {
        case err::NO_USER:
            ui->label_2->setText("Username not found"); break;
        case err::ALREADY_IN_CONTACTS:
            ui->label_2->setText("User already in your contacts"); break;
        case err::CANT_ADD_YOURSELF:
            ui->label_2->setText("You cannot add yourself"); break;
    }
}

