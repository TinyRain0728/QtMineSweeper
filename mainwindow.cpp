#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "blockscene.h"
#include "numscene.h"
#include "facescene.h"
#include "dlgrecord.h"
#include "dlgrecordername.h"
#include <QFile>
#include <QFileInfo>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    // 初始化游戏窗口及游戏、计时、计数、笑脸视图（默认初级难度）
    this->setBeginner();

    m_BlockScene = new BlockScene(this);
    ui->graphicsView->setScene(m_BlockScene);
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    m_NumMineScene = new NumScene(this, 10);
    ui->graphicsView_mines->setScene(m_NumMineScene);

    m_NumTimeScene = new NumScene(this, 0);
    ui->graphicsView_time->setScene(m_NumTimeScene);

    m_FaceScene = new FaceScene(this);
    ui->graphicsView_face->setScene(m_FaceScene);

    // 初始化记录窗口和记录文件
    dlgRecord = new DlgRecord(this);
    QString curPath = QCoreApplication::applicationDirPath();
    QFile aFileScore(curPath + "/score.json");
    QFileInfo aFileInfo(aFileScore);
    if(!aFileInfo.isFile())
    {
        if(aFileScore.open(QIODevice::WriteOnly|QIODevice::Text))
        {
            qDebug() << "creating aFile, size: " << aFileScore.size();
            QJsonArray scoreArr;
            for(int i = 0; i < 3; i++)
            {
                QJsonObject scoreObj;
                scoreObj.insert("name", "NO_ONE");
                scoreObj.insert("score", 999.99);
                scoreArr.append(scoreObj);
            }
            QJsonDocument doc(scoreArr);
            aFileScore.write(doc.toJson());
            qDebug() << "created aFile";
            aFileScore.close();
        }
    }
    if(aFileScore.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        QJsonArray scoreArr = QJsonDocument::fromJson(aFileScore.readAll()).array();
        for(int i = 0; i < 3; i++)
        {
            QJsonObject scoreObj = scoreArr[i].toObject();
            dlgRecord->setRecord(i, scoreObj["name"].toString(), scoreObj["score"].toDouble());
        }

        aFileScore.close();
        qDebug() << "read aFile";
    }
    qDebug() << "readFileOk";

    connect(m_BlockScene, &BlockScene::faceSmile, m_FaceScene, &FaceScene::do_FaceSmile);
    connect(m_BlockScene, &BlockScene::faceOoh, m_FaceScene, &FaceScene::do_FaceOoh);
    connect(m_BlockScene, &BlockScene::faceWin, m_FaceScene, &FaceScene::do_FaceWin);
    connect(m_BlockScene, &BlockScene::faceDead, m_FaceScene, &FaceScene::do_FaceDead);
    connect(m_BlockScene, &BlockScene::faceSmilePressed, m_FaceScene, &FaceScene::do_FaceSmilePressed);

    connect(m_BlockScene, &BlockScene::flagChange, m_NumMineScene, &NumScene::on_numChanged);
    connect(m_BlockScene, &BlockScene::timeChange, m_NumTimeScene, &NumScene::on_numChanged);

    connect(m_BlockScene, &BlockScene::scoreChange, this, &MainWindow::do_scoreChange);

    connect(m_FaceScene, &FaceScene::faceClicked, this, &MainWindow::on_actRestart_triggered);

    qDebug() << "connectOk";
}

MainWindow::~MainWindow()
{
    // 保存记录文件
    QString curPath = QCoreApplication::applicationDirPath();
    QFile aFileScore(curPath + "/score.json");
    if(aFileScore.open(QIODevice::WriteOnly|QIODevice::Truncate|QIODevice::Text))
    {
        QJsonArray scoreArr;
        for(int i = 0; i < 3; i++)
        {
            QJsonObject scoreObj;
            scoreObj.insert("name", dlgRecord->getName(i));
            scoreObj.insert("score",dlgRecord->getRecord(i));
            scoreArr.append(scoreObj);
        }
        QJsonDocument doc(scoreArr);
        aFileScore.write(doc.toJson());
        aFileScore.close();
    }

    delete ui;
}

// 初始化初级游戏视图
void MainWindow::setBeginner()
{
    this->setFixedSize(162, 226);

    QPixmap background(":/images/images/backgroundBeginner.jpg");
    background = background.scaled(QSize(162, 227), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    QPalette plet = this->palette();
    plet.setBrush(QPalette::Window, background);
    this->setPalette(plet);

    ui->graphicsView_mines->setGeometry(14, 13, 39, 23);
    ui->graphicsView_face->setGeometry(67, 12, 26, 26);
    ui->graphicsView_time->setGeometry(107, 13, 39, 23);
    ui->graphicsView->setGeometry(8, 51, 16 * 9, 16 * 9);
}

// 初始化中级游戏视图
void MainWindow::setIntermediate()
{
    this->setFixedSize(274, 338);

    QPixmap background(":/images/images/backgroundIntermediate.jpg");
    background = background.scaled(QSize(274, 338), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    QPalette plet = this->palette();
    plet.setBrush(QPalette::Window, background);
    this->setPalette(plet);

    ui->graphicsView_mines->setGeometry(14, 13, 39, 23);
    ui->graphicsView_face->setGeometry(123, 11, 26, 26);
    ui->graphicsView_time->setGeometry(219, 13, 39, 23);
    ui->graphicsView->setGeometry(8, 50, 16 * 16, 16 * 16);
}

// 初始化高级游戏视图
void MainWindow::setExpert()
{
    this->setFixedSize(498, 338);

    QPixmap background(":/images/images/backgroundExpert.jpg");
    background = background.scaled(QSize(498, 338), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    QPalette plet = this->palette();
    plet.setBrush(QPalette::Window, background);
    this->setPalette(plet);

    ui->graphicsView_mines->setGeometry(14, 13, 39, 23);
    ui->graphicsView_face->setGeometry(235, 11, 26, 26);
    ui->graphicsView_time->setGeometry(443, 13, 39, 23);
    ui->graphicsView->setGeometry(8, 50, 16 * 30, 16 * 16);
}

void MainWindow::on_actSetBeginner_triggered()
{
    this->setBeginner();
    m_BlockScene->setSize(typeBeg, 9, 9, 10);
    m_BlockScene->InitBlockScene();
}


void MainWindow::on_actSetIntermediate_triggered()
{
    this->setIntermediate();
    m_BlockScene->setSize(typeMid, 16, 16, 40);
    m_BlockScene->InitBlockScene();
}


void MainWindow::on_actSetExpert_triggered()
{
    this->setExpert();
    m_BlockScene->setSize(typeExp, 30, 16, 99);
    m_BlockScene->InitBlockScene();
}


void MainWindow::on_actRestart_triggered()
{
    m_BlockScene->InitBlockScene();
}


void MainWindow::on_actGetRecords_triggered()
{
    dlgRecord->show();
}

void MainWindow::do_scoreChange(int type, double newScore)
{
    double score = dlgRecord->getRecord(type);
    if(score > newScore)
    {
        dlgRecorderName = new DlgRecorderName(this);
        connect(dlgRecorderName, &DlgRecorderName::getRecorderName, this, &MainWindow::do_getRecorderName);
        dlgRecorderName->exec();
        dlgRecord->setRecord(type, m_RecorderName, newScore);
    }
}

void MainWindow::do_getRecorderName(QString name)
{
    m_RecorderName = name;
}

