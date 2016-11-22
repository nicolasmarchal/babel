#ifndef DIALOGBASIC_H
#define DIALOGBASIC_H

#include <QDialog>
#include <QMovie>
#include "AElement.h"

class QTGUI;

namespace Ui {
class DialogBasic;
}

class DialogBasic : public QDialog, public AElement
{
    Q_OBJECT

public:
    explicit DialogBasic(QWidget *parent = 0);
    ~DialogBasic();

private slots:
    void on_addButton_clicked();

public:
    void onAddError(unsigned int);

private:
    Ui::DialogBasic *ui;
    QMovie * mov;
};

#endif // DIALOGBASIC_H
