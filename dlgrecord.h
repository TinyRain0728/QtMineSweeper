#ifndef DLGRECORD_H
#define DLGRECORD_H

#include <QDialog>

namespace Ui {
class DlgRecord;
}

class DlgRecord : public QDialog
{
    Q_OBJECT

public:
    explicit DlgRecord(QWidget *parent = nullptr);
    ~DlgRecord();

    double getRecord(int type);
    QString getName(int type);
    void setRecord(int type, QString name, double score);

private:
    Ui::DlgRecord *ui;
};

#endif // DLGRECORD_H
