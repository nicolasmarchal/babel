#ifndef DIALOGERROR_H
#define DIALOGERROR_H

#include <QDialog>

namespace Ui {
class DialogError;
}

class DialogError : public QDialog
{
    Q_OBJECT

public:
    explicit DialogError(QWidget *parent = 0);
    ~DialogError();

private slots:
    void on_pushButton_clicked();

public:
    void setMessage(const std::string &);

private:
    Ui::DialogError *ui;
};

#endif // DIALOGERROR_H
