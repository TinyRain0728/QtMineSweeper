#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
class QGraphicsScene;
class BlockScene;
class NumScene;
class FaceScene;
class DlgRecord;
class DlgRecorderName;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void setBeginner();
    void setIntermediate();
    void setExpert();

private slots:
    void on_actSetBeginner_triggered();

    void on_actSetIntermediate_triggered();

    void on_actSetExpert_triggered();

    void on_actRestart_triggered();

    void on_actGetRecords_triggered();

    void do_scoreChange(int type, double score);

    void do_getRecorderName(QString name);

private:
    Ui::MainWindow *ui;

    DlgRecord *dlgRecord;   // 查看历史记录窗口
    DlgRecorderName *dlgRecorderName;   // 登录新记录姓名窗口

    BlockScene *m_BlockScene;   // 游戏区域视图
    NumScene *m_NumMineScene;   // 剩余雷数计数视图
    NumScene *m_NumTimeScene;   // 计时视图
    FaceScene *m_FaceScene;     // 笑脸视图
    QString m_RecorderName;

    enum{typeBeg, typeMid, typeExp};
};
#endif // MAINWINDOW_H
