#include "blockscene.h"
#include "blockitem.h"
#include <QVector>
#include <QRandomGenerator>
#include <QGraphicsSceneMouseEvent>
#include <QTimer>
#include <QElapsedTimer>

BlockScene::BlockScene(QObject *parent)
    : QGraphicsScene{parent}
{
    this->setSize();
    this->InitBlockScene();
    m_Timer = new QTimer(this);
    m_Timer->stop();
    m_Timer->setTimerType(Qt::CoarseTimer);
    m_Timer->setInterval(1000);

    connect(m_Timer, &QTimer::timeout, this, &BlockScene::do_timeout);
}

void BlockScene::setSize(int type, int width, int height, int numOfMines)
{
    m_Type = type;
    m_Width = width;
    m_Height = height;
    m_NumOfMines = numOfMines;
}

// 初始化游戏区域视图
void BlockScene::InitBlockScene()
{
    qDebug() << "beginInit";
    for(auto items : m_BlockItems)
    {
        for(auto item : items)
            delete item;
        items.clear();
    }
    m_BlockItems.clear();
    m_MineList.clear();
    m_FlagList.clear();
    this->clear();

    m_BlockItems = QVector<QVector<BlockItem*>>(m_Height, QVector<BlockItem*>(m_Width));
    m_NumOfUnSweeped = m_Width * m_Height;
    m_NumOfUnFlaggedMine = m_NumOfMines;
    m_Over = false;

    // 初始化计时器
    qDebug() << "timeInitStart";
    if(m_Timer != NULL)
        m_Timer->stop();
    m_TimeNum = 0;
    emit timeChange(m_TimeNum);

    if(m_ElapsedTimer != NULL)
        delete m_ElapsedTimer;
    m_ElapsedTimer = new QElapsedTimer();

    emit faceSmile();
    emit flagChange(m_NumOfMines);

    qDebug() << "paramInitOver";

    // 初始化格子
    for(int i = 0; i < m_Height; i++)
    {
        for(int j = 0; j < m_Width; j++)
        {
            m_BlockItems[i][j] = new BlockItem(i, j);
            m_BlockItems[i][j]->setPos(j * 16, i * 16);
            this->addItem(m_BlockItems[i][j]);
        }
    }

    // 设置地雷位置
    for(int i = 0; i < m_NumOfMines;)
    {

        int x = QRandomGenerator::global()->bounded(m_Width);
        int y = QRandomGenerator::global()->bounded(m_Height);

        if(!m_BlockItems[y][x]->isMine())
        {
            m_BlockItems[y][x]->setMine(true);
            m_MineList.push_back(m_BlockItems[y][x]);
            i++;
        }
    }

    // 设置周围雷数
    int nx, ny, m_NumOfMines;
    int dx[8] = {-1, 0, 1, -1, 1, -1, 0, 1};
    int dy[8] = {-1, -1, -1, 0, 0, 1, 1, 1};
    for(int i = 0; i < m_Height; i++)
    {
        for(int j = 0; j < m_Width; j++)
        {
            m_NumOfMines = 0;
            for(int k = 0; k < 8; k++)
            {
                nx = j + dx[k];
                ny = i + dy[k];
                if(0 <= nx && nx < m_Width && 0 <= ny && ny < m_Height)
                    if(m_BlockItems[ny][nx]->isMine())
                        m_NumOfMines++;
            }
            m_BlockItems[i][j]->setNumOfMines(m_NumOfMines);
        }
    }
    qDebug() << "blankInitOver";
}

// 获取鼠标点击格子对象
BlockItem *BlockScene::getCurItem(QGraphicsSceneMouseEvent *event)
{
    int x = event->scenePos().x() / 16;
    int y = event->scenePos().y() / 16;
    if(0 <= x && x < m_Width && 0 <= y && y < m_Height)
        return m_BlockItems[y][x];
    else
        return NULL;

}

// 翻开格子
void BlockScene::sweepItem(BlockItem *item, bool &isGameover)
{
    if(item->isFlagged() || item->isSweeped())
        emit faceSmile();
    else
    {
        item->setSweeped(true);
        m_NumOfUnSweeped--;
        if(m_Width * m_Height == m_NumOfUnSweeped + 1)
        {
            m_Timer->start();
            m_ElapsedTimer->start();
        }
        if(item->isMine())
        {
            isGameover = true;
            item->setStepMine(true);
        }
        else
        {
            if(item->getNumOfMines() == 0)
            {
                int x = item->getCol();
                int y = item->getRow();
                int dx[8] = {-1, 0, 1, -1, 1, -1, 0, 1};
                int dy[8] = {-1, -1, -1, 0, 0, 1, 1, 1};
                for(int i = 0; i < 8; i++)
                {
                    int nx = x + dx[i];
                    int ny = y + dy[i];
                    if(0 <= nx && nx < m_Width && 0 <= ny && ny < m_Height)    //对周围递归
                    {
                        BlockItem * nItem = m_BlockItems[ny][nx];
                        if(!nItem->isMine())
                            sweepItem(nItem, isGameover);
                    }

                }
            }
            item->updateItemStatus();
        }
    }
}

bool BlockScene::checkWin()
{
    return m_NumOfUnSweeped == m_NumOfUnFlaggedMine ? true : false;
}

void BlockScene::gameover()
{
    m_Over = true;
    emit faceDead();
    m_Timer->stop();
    for(auto item : m_MineList)
        item->updateItemStatus(true);
    for(auto item : m_FlagList)
        item->updateItemStatus(true);
}

void BlockScene::win()
{
    m_Over = true;
    emit faceWin();
    emit flagChange(0);
    m_Timer->stop();
    double score = m_ElapsedTimer->elapsed() / 10 / 100.0;
    emit scoreChange(m_Type, score);
    qDebug() << score;
    for(auto item : m_MineList)
    {
        if(!item->isFlagged())
        {
            item->setFlagged(true);
            item->updateItemStatus(true);
        }
    }
}

void BlockScene::do_timeout()
{
    m_TimeNum++;
    emit timeChange(m_TimeNum);
}

void BlockScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    BlockItem *item = getCurItem(event);
    if(item != NULL && !m_Over)
    {
        // 左右键同时按下动作
        if((event->buttons() & Qt::LeftButton) && (event->buttons() & Qt::RightButton))
        {
            emit faceOoh();
            m_LeftAndRightPressed = true;
            int x = item->getCol();
            int y = item->getRow();
            // 逐行遍历周围3*3格子时x、y值相对中心格子的偏移量
            int dx[9] = {-1, 0, 1, -1, 0, 1, -1, 0, 1};
            int dy[9] = {-1, -1, -1, 0, 0, 0, 1, 1, 1};
            for(int i = 0; i < 9; i++)
            {
                int nx = x + dx[i];
                int ny = y + dy[i];
                if(0 <= nx && nx < m_Width && 0 <= ny && ny < m_Height)    //对周围递归
                {
                    BlockItem * nItem = m_BlockItems[ny][nx];
                    if(!nItem->isSweeped() && !nItem->isFlagged())
                        nItem->setPixmapOpen0();
                }
            }
        }

        // 左键按下动作
        else if(event->button() == Qt::LeftButton)
        {
            emit faceOoh();
            if(!item->isSweeped())
                item->setPixmapOpen0();
        }

        // 右键按下动作
        else if(event->button() == Qt::RightButton)
        {
            if(!item->isSweeped())
            {
                if(!item->isFlagged())
                {
                    item->setFlagged(true);
                    m_FlagList.push_back(item);
                    emit flagChange(m_NumOfMines - m_FlagList.size());
                    if(item->isMine())
                    {
                        m_NumOfUnFlaggedMine--;
                        m_NumOfUnSweeped--;
                    }
                }
                else
                {
                    item->setFlagged(false);
                    m_FlagList.removeOne(item);
                    emit flagChange(m_NumOfMines - m_FlagList.size());
                    if(item->isMine())
                    {
                        m_NumOfUnFlaggedMine++;
                        m_NumOfUnSweeped++;
                    }
                }
                item->updateItemStatus();
            }
        }
    }
    QGraphicsScene::mousePressEvent(event);
}

void BlockScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    BlockItem *item = getCurItem(event);
    if(item != NULL && !m_Over)
    {
        if(event->button() == Qt::LeftButton)
        {
            bool isGameover = false;
            if(m_LeftAndRightPressed)
            {
                m_LeftAndRightPressed = false;
                m_LeftAndRightReleased = true;
                int x = item->getCol();
                int y = item->getRow();
                int dx[9] = {-1, 0, 1, -1, 0, 1, -1, 0, 1};
                int dy[9] = {-1, -1, -1, 0, 0, 0, 1, 1, 1};
                int flagNum = 0;
                for(int i = 0; i < 9; i++)
                {
                    int nx = x + dx[i];
                    int ny = y + dy[i];
                    if(0 <= nx && nx < m_Width && 0 <= ny && ny < m_Height)    //对周围递归
                    {
                        BlockItem * nItem = m_BlockItems[ny][nx];
                        if(i != 4 && nItem->isFlagged())
                            flagNum++;
                        nItem->updateItemStatus();
                    }
                }
                if(flagNum == item->getNumOfMines())
                {
                    for(int i = 0; i < 9; i++)
                    {
                        int nx = x + dx[i];
                        int ny = y + dy[i];
                        if(0 <= nx && nx < m_Width && 0 <= ny && ny < m_Height && i != 4)    //对周围递归
                        {
                            BlockItem * nItem = m_BlockItems[ny][nx];
                            if(!nItem->isSweeped() && !nItem->isFlagged())
                            {
                                if(!nItem->isMine())
                                    sweepItem(nItem, isGameover);
                                else if(nItem->isMine())
                                {
                                    nItem->setStepMine(true);
                                    isGameover = true;
                                }
                            }
                        }
                    }
                }
            }
            else
                sweepItem(item, isGameover);

            if(checkWin())
                win();
            else if(isGameover)
                gameover();
            else
                emit faceSmile();
        }
        else if(event->button() == Qt::RightButton && m_LeftAndRightReleased)
            m_LeftAndRightReleased = false;
    }
    QGraphicsScene::mouseReleaseEvent(event);
}
