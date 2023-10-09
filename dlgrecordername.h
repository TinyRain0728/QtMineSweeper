#ifndef DLGRECORDERNAME_H
#define DLGRECORDERNAME_H

#include <QDialog>

namespace Ui {
class DlgRecorderName;
}

class DlgRecorderName : public QDialog
{
    Q_OBJECT

public:
    explicit DlgRecorderName(QWidget *parent = nullptr);
    ~DlgRecorderName();

signals:
    void getRecorderName(QString name);

private slots:
    void on_pushButton_clicked();

private:
    Ui::DlgRecorderName *ui;
};

#endif // DLGRECORDERNAME_H
