#ifndef CUSTOMITEM_H
#define CUSTOMITEM_H

#include <QWidget>

namespace Ui {
class CustomItem;
}

class CustomItem : public QWidget
{
    Q_OBJECT

public:
    explicit CustomItem(QWidget *parent = 0);
    ~CustomItem();

public:
    void setName(const std::string & usernamae);

public:
    void setOffline();
    void setOnline();

private:
    Ui::CustomItem *ui;
};

#define ONLINE_STYLESHEET "background-color:rgb(0, 229, 19);\
border-radius: 5px;"

#define OFFLINE_STYLESHEET "background-color:rgb(255, 0, 0); \
border-radius: 5px;"

#endif // CUSTOMITEM_H
