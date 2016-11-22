#include "QTGUI.hh"
#include "Client.hh"
#include "login.h"
#include "ui_login.h"

Login::Login(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);
    ui->password->setEchoMode(QLineEdit::Password);
    ui->password_2->setEchoMode(QLineEdit::Password);
    ui->retype_password->setEchoMode(QLineEdit::Password);
    ui->underline2->hide();
    ui->usernameLabel->hide();
    ui->username_2->hide();
    ui->passwordLabel->hide();
    ui->password_2->hide();
    ui->retypeLabel->hide();
    ui->retype_password->hide();
    ui->registerButton->hide();
    ui->wrongPassword->hide();
    ui->wrongPassword_2->hide();
    ui->wrongUsername->hide();
    ui->validPassword->hide();
    ui->validPassword_2->hide();
    ui->validUsername->hide();
    ui->registerButton->setEnabled(false);
    ui->label->setPixmap(ui->label->pixmap()->scaledToWidth(ui->label->width(), Qt::SmoothTransformation));
    ui->wrongPassword->setPixmap(ui->wrongPassword->pixmap()->scaledToWidth(ui->wrongPassword->width(), Qt::SmoothTransformation));
    ui->wrongUsername->setPixmap(ui->wrongUsername->pixmap()->scaledToWidth(ui->wrongUsername->width(), Qt::SmoothTransformation));
    ui->wrongPassword_2->setPixmap(ui->wrongPassword_2->pixmap()->scaledToWidth(ui->wrongPassword_2->width(), Qt::SmoothTransformation));
    ui->validPassword->setPixmap(ui->validPassword->pixmap()->scaledToWidth(ui->validPassword->width(), Qt::SmoothTransformation));
    ui->validUsername->setPixmap(ui->validUsername->pixmap()->scaledToWidth(ui->validUsername->width(), Qt::SmoothTransformation));
    ui->validPassword_2->setPixmap(ui->validPassword_2->pixmap()->scaledToWidth(ui->validPassword_2->width(), Qt::SmoothTransformation));
    connect(ui->username, SIGNAL(returnPressed()), ui->loginButton, SIGNAL(clicked()));
    connect(ui->password, SIGNAL(returnPressed()), ui->loginButton, SIGNAL(clicked()));
}

Login::~Login()
{
    delete ui;
}

void Login::setGui(QTGUI * gui)
{
    _gui = gui;
}

/* ========== HANDLING WINDOW EVENTS ========== */

void Login::on_loginButton_clicked()
{
   std::string _username = this->ui->username->text().toStdString();
   std::string _password = this->ui->password->text().toStdString();
   bool error = false;
   if (_username.size() == 0)
   {
        ui->errorUsername->setText("This field is required");
        error = true;
   } else
        ui->errorUsername->setText("");
   if (_password.size() == 0)
   {
       ui->errorPassword->setText("This field is required");
       error = true;
   } else
       ui->errorPassword->setText("");
   if (error == true)
       return;
   _gui->username = _username;
   _gui->client.doLogin(_username, _password);
}

void Login::on_pushButton_pressed()
{
    ui->password->setEchoMode(QLineEdit::Normal);
}

void Login::on_pushButton_released()
{
    ui->password->setEchoMode(QLineEdit::Password);
}

void Login::on_signOut_clicked()
{
    ui->password->hide();
    ui->username->hide();
    ui->loginButton->hide();
    ui->pushButton->hide();
    ui->underline->hide();
    ui->errorPassword->hide();
    ui->errorUsername->hide();
    ui->signIn->setStyleSheet(SIGNIN_STYLESHEET);
    ui->signOut->setStyleSheet(SIGNOUT_STYLESHEET);
    ui->underline2->show();
    ui->usernameLabel->show();
    ui->username_2->show();
    ui->passwordLabel->show();
    ui->password_2->show();
    ui->retypeLabel->show();
    ui->retype_password->show();
    ui->registerButton->show();
}

void Login::on_signIn_clicked()
{
    ui->password->show();
    ui->username->show();
    ui->loginButton->show();
    ui->pushButton->show();
    ui->underline->show();
    ui->errorPassword->show();
    ui->errorUsername->show();
    ui->signIn->setStyleSheet(INVERT_SIGNIN_STYLESHEET);
    ui->signOut->setStyleSheet(INVERT_SIGNOUT_STYLESHEET);
    ui->underline2->hide();
    ui->usernameLabel->hide();
    ui->username_2->hide();
    ui->passwordLabel->hide();
    ui->password_2->hide();
    ui->retypeLabel->hide();
    ui->retype_password->hide();
    ui->registerButton->hide();
    ui->wrongPassword->hide();
    ui->wrongPassword_2->hide();
    ui->wrongUsername->hide();
    ui->validPassword->hide();
    ui->validPassword_2->hide();
    ui->validUsername->hide();
}

void Login::on_retype_password_textEdited(const QString &arg1)
{
    if (ui->password_2->text().toStdString().compare(arg1.toStdString()) == 0)
    {
        ui->wrongPassword_2->hide();
        ui->validPassword_2->show();
        if (ui->validUsername->isVisible() && ui->validPassword->isVisible() && ui->validPassword_2->isVisible())
        {
            ui->registerButton->setEnabled(true);
            ui->registerButton->setCursor(Qt::PointingHandCursor);
        }
    } else {
        if (arg1.size() == 0)
            ui->wrongPassword_2->setToolTip("Password cannot be null");
        else
            ui->wrongPassword_2->setToolTip("Passwords are different");
        ui->validPassword_2->hide();
        ui->wrongPassword_2->show();
    }
}

void Login::on_password_2_textEdited(const QString &arg1)
{
    if (arg1.size() >= 8)
    {
        ui->wrongPassword->hide();
        ui->validPassword->show();
        if (ui->validUsername->isVisible() && ui->validPassword->isVisible() && ui->validPassword_2->isVisible())
        {
            ui->registerButton->setEnabled(true);
            ui->registerButton->setCursor(Qt::PointingHandCursor);
        }
    } else {
        if (arg1.size() == 0)\
            ui->wrongPassword->setToolTip("Password cannot be null");
        else
            ui->wrongPassword->setToolTip(">= 8 characters");
        this->on_retype_password_textEdited(ui->retype_password->text());
        ui->validPassword->hide();
        ui->wrongPassword->show();
    }
}

void Login::on_username_2_textEdited(const QString &arg1)
{
    if (arg1.size() >= 5)
    {
        ui->wrongUsername->hide();
        ui->validUsername->show();
        if (ui->validUsername->isVisible() && ui->validPassword->isVisible() && ui->validPassword_2->isVisible())
        {
            ui->registerButton->setEnabled(true);
            ui->registerButton->setCursor(Qt::PointingHandCursor);
        }
    } else {
        if (arg1.size() == 0)
            ui->wrongUsername->setToolTip("Username cannot be null");
        else
            ui->wrongUsername->setToolTip(">= 5 characters");
        ui->validUsername->hide();
        ui->wrongUsername->show();
    }
}

void Login::on_registerButton_clicked()
{
       std::string _username = ui->username_2->text().toStdString();
       std::string _password = ui->password_2->text().toStdString();

       _gui->client.doRegister(_username, _password);
 }

/* ========= END ========== */


/* ======== ERROR AND SUCCESS EVENTS ========== */

void Login::errorAtRegister(unsigned int error)
{
    if (error == err::USER_EXISTS)
    {
       std::cout << "Error at register" << std::endl;
    }
}

void Login::successAtRegister()
{
    //POPUP OK
    on_signIn_clicked();
}

void Login::errorAtLogin(unsigned int error)
{
    if (error == err::BAD_PASSWORD)
    {
        ui->errorPassword->setText("Bad password");
        return;
    }

    if (error == err::NO_USER)
    {
        ui->errorUsername->setText("Username doesn't exist");
        return;
    }
}

void Login::closeEvent(QCloseEvent *event)
{
    _gui->onLogout(NULL);
    QWidget::closeEvent(event);
}

/*  ======== END  ========== */
