#include "QTGUI.hh"
#include "Client.hh"
#include "callitem.h"
#include "ui_callitem.h"

callItem::callItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::callItem)
{
    calling = new QMovie(":/Form/Images/Form/call.gif");
    ui->setupUi(this);
    ui->label_2->setPixmap(ui->label_2->pixmap()->scaledToWidth(ui->label_2->width(), Qt::SmoothTransformation));
    ui->label_4->setMovie(calling);
    ui->hangoutButton->hide();
    calling->setScaledSize(QSize(ui->label_4->width(), ui->label_4->height()));
    isStartedCall = false;
    ui->time->hide();
}

callItem::~callItem()
{
    delete ui;
}

void callItem::changeUser(const std::string & username)
{
    ui->label_3->setText(QString::fromStdString(username));
}

void callItem::on_pushButton_clicked()
{
    dialog = new Dialog;
    dialog->setGui(_gui);
    dialog->setUsername(ui->label_3->text().toStdString());
    dialog->show();
}

void callItem::destroyDialog()
{
    delete dialog;
}

void callItem::on_pushButton_2_clicked()
{
    _gui->client.doSendCall(ui->label_3->text().toStdString());
}

void callItem::onResponseCall()
{
    calling->start();
    ui->pushButton_2->hide();
    ui->hangoutButton->show();
}


void callItem::startingCall()
{
    isStartedCall = true;
    calling->stop();
    ui->label_4->hide();
    ui->pushButton_2->hide();
    ui->hangoutButton->show();
    ui->hangoutButton->setEnabled(true);
    ui->time->setText(QTime(0, 0).toString());
    startTime.restart();
}

void callItem::updateTime()
{
    ui->label->setText(QTime(0, 0).addMSecs(startTime.elapsed()).toString());
}

void callItem::on_hangoutButton_clicked()
{
    std::cout << isStartedCall << std::endl;
    if (isStartedCall)
    {
        std::cout << "End Call Send" << std::endl;
        _gui->client.doEndCall();
        isStartedCall = false;
    }
    calling->stop();
    ui->label_4->hide();
    ui->pushButton_2->show();
    ui->hangoutButton->hide();
}

void callItem::stopCall()
{
    on_hangoutButton_clicked();
}
