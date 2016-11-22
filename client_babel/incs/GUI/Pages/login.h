#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>
#include <map>

class Client;

class QTGUI;

namespace Ui {
class Login;
}

class Login : public QWidget
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = 0);
    ~Login();

private slots:
    void on_loginButton_clicked();
    void on_pushButton_pressed();
    void on_pushButton_released();
    void on_signOut_clicked();
    void on_signIn_clicked();
    void on_retype_password_textEdited(const QString &arg1);
    void on_password_2_textEdited(const QString &arg1);
    void on_username_2_textEdited(const QString &arg1);
    void on_registerButton_clicked();

private:
    Ui::Login *ui;
    QTGUI * _gui;

public:
    void errorAtLogin(unsigned int);
    void errorAtRegister(unsigned int);
    void successAtRegister();
    void closeEvent(QCloseEvent *);

public:
    void setGui(QTGUI *);
};

    /* ====== STYLESHEETS ==== */

#define SIGNIN_STYLESHEET "#signIn \
{ \
    background-color: transparent; \
    background: none; \
    border: none; \
    background-repeat: none; \
    color: rgba(255, 255, 255, 0.4); \
    font: 16pt 'Montserrat'; \
} \
\
#signIn:hover \
{ \
    color: rgba(255, 255, 255, 1); \
}"

#define SIGNOUT_STYLESHEET "#signOut \
{ \
    background-color: transparent; \
    background: none; \
    border: none; \
    background-repeat: none; \
    color: rgba(255, 255, 255, 1); \
    font: 16pt 'Montserrat'; \
} \
\
#signOut:hover \
{ \
    color: rgba(255, 255, 255, 1); \
}"

#define INVERT_SIGNIN_STYLESHEET "#signIn \
{ \
    background-color: transparent; \
    background: none; \
    border: none; \
    background-repeat: none; \
    color: rgba(255, 255, 255, 1); \
    font: 16pt 'Montserrat'; \
} \
\
#signIn:hover \
{ \
    color: rgba(255, 255, 255, 1); \
}"

#define INVERT_SIGNOUT_STYLESHEET "#signOut \
{ \
    background-color: transparent; \
    background: none; \
    border: none; \
    background-repeat: none; \
    color: rgba(255, 255, 255, 0.4); \
    font: 16pt 'Montserrat'; \
} \
\
#signOut:hover \
{ \
    color: rgba(255, 255, 255, 1); \
}"

    /* ====== END ====== */

#endif // LOGIN_H
