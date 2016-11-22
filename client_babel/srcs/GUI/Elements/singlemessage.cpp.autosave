#include <iostream>
#include <QFontMetrics>
#include "singlemessage.h"
#include "ui_singlemessage.h"

SingleMessage::SingleMessage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SingleMessage)
{
    ui->setupUi(this);
}

SingleMessage::~SingleMessage()
{
    delete ui;
}

void SingleMessage::setUsername(const std::string & username)
{
    ui->label->setText(QString::fromStdString(username));
}

void SingleMessage::setMessage(const std::string & message)
{
    ui->label_2->setWordWrap(true);
    ui->label_2->setText(QString::fromStdString(message));
    ui->label_2->adjustSize();
    ui->label_2->setFixedHeight(ui->label_2->height() + 5);
    this->adjustSize();
}

void SingleMessage::upText()
{
    int y = ui->label->geometry().y();
    ui->label_2->setGeometry(ui->label_2->geometry().x(), y, ui->label_2->width(), ui->label_2->height());
}
