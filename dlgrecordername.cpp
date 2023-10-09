#include "dlgrecordername.h"
#include "ui_dlgrecordername.h"

DlgRecorderName::DlgRecorderName(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgRecorderName)
{
    ui->setupUi(this);
}

DlgRecorderName::~DlgRecorderName()
{
    delete ui;
}

void DlgRecorderName::on_pushButton_clicked()
{
    QString name = ui->lineEdit->text();
    emit getRecorderName(name);
    this->accept();
}
