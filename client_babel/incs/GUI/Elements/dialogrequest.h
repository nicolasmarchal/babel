#ifndef DIALOGREQUEST_H
#define DIALOGREQUEST_H

#include <QDialog>
#include "AElement.h"

namespace Ui {
class DialogRequest;
}

class DialogRequest : public QDialog, public AElement
{
    Q_OBJECT

public:
    explicit DialogRequest(QWidget *parent = 0);
    ~DialogRequest();

public:
    void personify(const std::string &);
    void closeEvent(QCloseEvent * event);

private:
    std::string userAdd;

private slots:
    void on_pushButton_2_clicked();
    void on_pushButton_clicked();

private:
    Ui::DialogRequest *ui;
};

#endif // DIALOGREQUEST_H
