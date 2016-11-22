#include "dialogerror.h"
#include "ui_dialogerror.h"

DialogError::DialogError(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogError)
{
    ui->setupUi(this);
}

DialogError::~DialogError()
{
    delete ui;
}

void DialogError::setMessage(const std::string & error)
{
    ui->label->setText(QString::fromStdString(error));
}

void DialogError::on_pushButton_clicked()
{
    this->hide();
}
