#include "Client.hh"
#include "QTGUI.hh"
#include "init.h"
#include "ui_init.h"

Init::Init(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Init)
{
    ui->setupUi(this);
    ui->label->setPixmap(ui->label->pixmap()->scaledToWidth(ui->label->width(), Qt::SmoothTransformation));
    ui->username->setFocus();
    connect(ui->username, SIGNAL(returnPressed()), ui->registerButton, SIGNAL(clicked()));
    connect(ui->password, SIGNAL(returnPressed()), ui->registerButton, SIGNAL(clicked()));
    this->isConnected = false;
}

Init::~Init()
{
    delete ui;
}

void Init::on_registerButton_clicked()
{
    std::string ip = ui->username->text().toStdString();
    std::string port = ui->password->text().toStdString();
    bool error = false;

    if (ip.size() == 0)
    {
        ui->errorUsername->setText("This field is required");
        error = true;
    }

    if (port.size() == 0)
    {
        ui->errorUsername_2->setText("This field is required");
        error = true;
    }
    if (error)
        return;
    try {
        if (_gui->client.connect(ip, atoi(port.c_str())) == 1)
        {
            this->hide();
            _gui->_loginWindow->show();
        } else {
                ui->errorUsername_3->setText("Connection failed");
        }

    } catch (NetworkError * e)
    {
        ui->errorUsername_3->setText(QString::fromStdString(e->what()));
        std::cout << e->what() << std::endl;
        delete e;
        return;
    }
}

void Init::setGui(QTGUI * gui)
{
    _gui = gui;
}

void Init::closeEvent(QCloseEvent *event)
{
    _gui->onLogout(NULL);
    QWidget::closeEvent(event);
}
