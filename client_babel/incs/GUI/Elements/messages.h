#ifndef MESSAGES_H
#define MESSAGES_H

#include <QWidget>
#include <vector>

namespace Ui {
class Messages;
}

class Messages : public QWidget
{
    Q_OBJECT

public:
    explicit Messages(QWidget *parent = 0);
    ~Messages();

public:
  void pushText(const std::pair<std::string, std::string> &);

private:
  std::vector<std::pair<std::string, std::string>> _messages;
  std::string lastUserToTalk;

private:
    Ui::Messages *ui;
};

#endif // MESSAGES_H
