#ifndef DIALOGCALL_H
#define DIALOGCALL_H

#include <QDialog>
#include "protocol.hh"
#include "AElement.h"
#include "callitem.h"

namespace Ui {
class DialogCall;
}

class DialogCall : public QDialog, public AElement
{
    Q_OBJECT

public:
    explicit DialogCall(QWidget *parent = 0);
    ~DialogCall();

public:
    void personify(const std::string &);

public:
    t_usrsck * socket;
    callItem * _callinfo;

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

private:
    std::string _userToRespond;


private:
    Ui::DialogCall *ui;
};

#endif // DIALOGCALL_H
