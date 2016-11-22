#include <QScrollBar>
#include "singlemessage.h"
#include "form.h"
#include "messages.h"
#include "ui_messages.h"

Messages::Messages(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Messages)
{
    QScrollBar * bar = new QScrollBar;

    ui->setupUi(this);

    this->lastUserToTalk = "";

    bar->setStyleSheet(BAR_STYLESHEET);
    ui->listWidget->setVerticalScrollBar(bar);
    ui->listWidget->setSelectionMode(QAbstractItemView::NoSelection);
    ui->listWidget->setFocusPolicy(Qt::FocusPolicy::NoFocus);
    ui->listWidget->setSpacing(5);
}

Messages::~Messages()
{
    delete ui;
}

void Messages::pushText(const std::pair<std::string, std::string> & text)
{
  SingleMessage * message;
  QListWidgetItem * item;

  message = new SingleMessage;
  item = new QListWidgetItem;
  if (text.first != lastUserToTalk)
     message->setUsername(text.first);
  else
      message->upText();
  message->setMessage(text.second);
  item->setSizeHint(QSize(0, message->height()));
  ui->listWidget->addItem(item);
  ui->listWidget->setItemWidget(item, message);
  this->lastUserToTalk = text.first;
  ui->listWidget->verticalScrollBar()->setSliderPosition(ui->listWidget->verticalScrollBar()->maximum());

}
