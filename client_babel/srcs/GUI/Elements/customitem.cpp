#include <QString>
#include "customitem.h"
#include "ui_customitem.h"

CustomItem::CustomItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CustomItem)
{
    ui->setupUi(this);
}

CustomItem::~CustomItem()
{
    delete ui;
}

void CustomItem::setName(const std::string & username)
{
    QString str = QString::fromStdString(username);
    ui->label->setText(str);
}

void CustomItem::setOffline()
{
    ui->label_2->setStyleSheet(OFFLINE_STYLESHEET);
}

void CustomItem::setOnline()
{
    ui->label_2->setStyleSheet(ONLINE_STYLESHEET);
}
