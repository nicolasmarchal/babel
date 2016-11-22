#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include "AElement.h"

class QTGUI;

namespace Ui {
class Dialog;
}

class Dialog : public QDialog, public AElement
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

private:
    std::string _username;

public:
    void setUsername(const std::string &);
private slots:
    void on_pushButton_clicked();

private:
    Ui::Dialog *ui;
};

#endif // DIALOG_H
