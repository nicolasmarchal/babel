#ifndef SINGLEMESSAGE_H
#define SINGLEMESSAGE_H

#include <QWidget>

namespace Ui {
class SingleMessage;
}

class SingleMessage : public QWidget
{
    Q_OBJECT

public:
    explicit SingleMessage(QWidget *parent = 0);
    ~SingleMessage();

public:
    void setUsername(const std::string &);
    void setMessage(const std::string &);
    void upText();

private:
    Ui::SingleMessage *ui;
};

#endif // SINGLEMESSAGE_H
