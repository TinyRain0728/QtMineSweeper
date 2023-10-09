#include "dlgrecord.h"
#include "ui_dlgrecord.h"

DlgRecord::DlgRecord(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgRecord)
{
    ui->setupUi(this);
}

DlgRecord::~DlgRecord()
{
    delete ui;
}

double DlgRecord::getRecord(int type)
{
    switch (type) {
    case 0:
        return ui->labBeginnerScore->text().toDouble();
    case 1:
        return ui->labIntermediateScore->text().toDouble();
    case 2:
        return ui->labExpertScore->text().toDouble();
    }
    return -1;
}

QString DlgRecord::getName(int type)
{
    switch (type) {
    case 0:
        return ui->labBeginnerName->text();
    case 1:
        return ui->labIntermediateName->text();
    case 2:
        return ui->labExpertName->text();
    }
    return "";
}

void DlgRecord::setRecord(int type, QString name, double score)
{
    switch (type) {
    case 0:{
        ui->labBeginnerName->setText(name);
        ui->labBeginnerScore->setText(QString::asprintf("%.2f", score));
        break;}
    case 1:{
        ui->labIntermediateName->setText(name);
        ui->labIntermediateScore->setText(QString::asprintf("%.2f", score));
        break;}
    case 2:{
        ui->labExpertName->setText(name);
        ui->labExpertScore->setText(QString::asprintf("%.2f", score));
        break;}
    default:
        qDebug() << "记录更新错误！";
        break;
    }
}

