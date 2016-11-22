#ifndef INIT_H
#define INIT_H

#include <QWidget>
#include "Client.hh"

class QTGUI;

namespace Ui {
class Init;
}

class Init : public QWidget
{
    Q_OBJECT

public:
    explicit Init(QWidget *parent = 0);
    ~Init();
private slots:
    void on_registerButton_clicked();

private:
    bool isConnected;

public:
    void setGui(QTGUI *);
    bool getIsConnected() const;
    void closeEvent(QCloseEvent * );

private:
    Ui::Init *ui;
    QTGUI * _gui;
};

#endif // INIT_H
