#ifndef CALLITEM_H
#define CALLITEM_H

#include <QWidget>
#include <QMovie>
#include <QTime>
#include <QTimer>
#include "AElement.h"
#include "dialog.h"

class QTGUI;

namespace Ui {
class callItem;
}

class callItem : public QWidget, public AElement
{
    Q_OBJECT

public:
    explicit callItem(QWidget *parent = 0);
    ~callItem();

private:
    Ui::callItem *ui;
    Dialog * dialog;
    QMovie * calling;
    QTime startTime;
    QTimer t;

public:
    void changeUser(const std::string &);
    void destroyDialog();
    void startingCall();
    void updateTime();
    void stopCall();
    void onResponseCall();

private:
    bool isStartedCall;
private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_hangoutButton_clicked();
};

#endif // CALLITEM_H
