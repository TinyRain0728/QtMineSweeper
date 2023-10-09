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
    // qDebug() << "beginInit";
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
    // qDebug() << "timeInitStart";
    if(m_Timer != NULL)
        m_Timer->stop();
    m_TimeNum = 0;
    emit timeChange(m_TimeNum);

    if(m_ElapsedTimer != NULL)
        delete m_ElapsedTimer;
    m_ElapsedTimer = new QElapsedTimer();

    emit faceSmile();
    emit flagChange(m_NumOfMines);

    // qDebug() << "paramInitOver";

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
    // qDebug() << "blankInitOver";
}

// 获取光标处格子对象
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
    if(!item->isSweeped())
    {
        item->setSweeped(true);
        m_NumOfUnSweeped--;
    }

    // 第一次点击，开始计时
    if(m_Width * m_Height == m_NumOfUnSweeped + 1)
    {
        m_Timer->start();
        m_ElapsedTimer->start();
    }

    // 标记在该格子触雷，标记游戏结束
    if(item->isMine())
    {
        item->setStepMine(true);
        isGameover = true;
    }

    // 该格子周围没有雷，递归翻开周围非雷非标记格子
    else if(item->getNumOfMines() == 0)
    {
        int x = item->getCol();
        int y = item->getRow();
        for(int i = 0; i < 9; i++)
        {
            int nx = x + dx[i];
            int ny = y + dy[i];
            if(0 <= nx && nx < m_Width && 0 <= ny && ny < m_Height && i != 4)
            {
                BlockItem * nItem = m_BlockItems[ny][nx];
                if(!nItem->isMine() && !nItem->isFlagged() && !nItem->isSweeped())
                    sweepItem(nItem, isGameover);
            }
        }
    }
    item->updateItemStatus();
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

    // 更新所有雷和错误标记的格子状态
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
    // qDebug() << score;

    // 标记剩余雷并更新格子状态
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
        // 左键按下动作
        if(event->button() == Qt::LeftButton)
        {
            emit faceOoh();

            item->pressOrReleaseItem(); // 更新格子状态

            if(m_rPress)                // 标记于该格子处左右键都按下
            {
                m_lrPressItem = item;
                m_rPress = false;
            }
            else                        // 标记于该格子处左键按下
                m_lPressItem = item;
        }

        // 右键按下动作
        else if(event->button() == Qt::RightButton)
        {
            // 若左键已按下，标记于该格子处左右键都按下
            if(m_lPressItem == item)
            {
                m_lrPressItem = item;
                m_lPressItem = NULL;
            }
            // 标记右键按下
            else
                m_rPress = true;

            if(!item->isSweeped() && m_lrPressItem != item)
            {
                // 根据格子属性更新相关数据及格子状态
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

        // 左右键都被按下动作
        if(m_lrPressItem == item)
        {
            int x = item->getCol();
            int y = item->getRow();

            // 逐行遍历周围3*3格子时x、y值相对中心格子的偏移量
            int dx[9] = {-1, 0, 1, -1, 0, 1, -1, 0, 1};
            int dy[9] = {-1, -1, -1, 0, 0, 0, 1, 1, 1};

            // 递归更新周围格子状态
            for(int i = 0; i < 9; i++)
            {
                int nx = x + dx[i];
                int ny = y + dy[i];
                if(0 <= nx && nx < m_Width && 0 <= ny && ny < m_Height && i != 4)
                {
                    BlockItem * nItem = m_BlockItems[ny][nx];
                    nItem->pressOrReleaseItem();
                }
            }
        }
    }
    QGraphicsScene::mousePressEvent(event);
}

void BlockScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    BlockItem *item = getCurItem(event);

    // 左右键共同按下时鼠标移动
    if(m_lrPressItem != NULL)
    {
        int x = m_lrPressItem->getCol();
        int y = m_lrPressItem->getRow();

        // 递归更新周围格子状态
        for(int i = 0; i < 9; i++)
        {
            int nx = x + dx[i];
            int ny = y + dy[i];
            if(0 <= nx && nx < m_Width && 0 <= ny && ny < m_Height)
            {
                BlockItem * nItem = m_BlockItems[ny][nx];
                if(nItem->isPressed())
                    nItem->pressOrReleaseItem();
            }
        }
    }

    // 左键按下时鼠标移动
    if(m_lPressItem != NULL)
    {
        if(m_lPressItem->isPressed())
            m_lPressItem->pressOrReleaseItem();
    }

    // 光标未移出界则更新格子按下状态
    if(item != NULL)
    {
        if(m_lPressItem)
        {
            item->pressOrReleaseItem();
            m_lPressItem = item;
        }
        else if(m_lrPressItem)
        {
            int x = item->getCol();
            int y = item->getRow();

            // 递归更新周围格子状态
            for(int i = 0; i < 9; i++)
            {
                int nx = x + dx[i];
                int ny = y + dy[i];
                if(0 <= nx && nx < m_Width && 0 <= ny && ny < m_Height)
                {
                    BlockItem * nItem = m_BlockItems[ny][nx];
                    nItem->pressOrReleaseItem();
                }
            }

            m_lrPressItem = item;
        }
    }

    QGraphicsScene::mouseMoveEvent(event);
}

void BlockScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    BlockItem *item = getCurItem(event);
    if(item != NULL && !m_Over)
    {
        bool isGameover = false;

        //左右键同时按下状态释放按键
        if(m_lrPressItem == item)
        {
            int x = item->getCol();
            int y = item->getRow();

            // 处在翻开格子上的释放
            if(item->isSweeped())
            {
                int flagNum = 0;

                // 递归统计周围标记数
                for(int i = 0; i < 9; i++)
                {
                    int nx = x + dx[i];
                    int ny = y + dy[i];
                    if(0 <= nx && nx < m_Width && 0 <= ny && ny < m_Height && i != 4)
                    {
                        BlockItem * nItem = m_BlockItems[ny][nx];
                        if(nItem->isFlagged())
                            flagNum++;
                    }
                }

                // 周围标记数与雷数相同，触发周围翻开检测
                if(flagNum == item->getNumOfMines())
                {
                    for(int i = 0; i < 9; i++)
                    {
                        int nx = x + dx[i];
                        int ny = y + dy[i];
                        if(0 <= nx && nx < m_Width && 0 <= ny && ny < m_Height && i != 4)
                        {
                            BlockItem * nItem = m_BlockItems[ny][nx];
                            if(nItem->isPressed())
                                sweepItem(nItem, isGameover);
                        }
                    }
                }
            }

            // 不处在翻开格子上的释放或周围翻开检测失败，直接递归释放周围按下的格子
            for(int i = 0; i < 9; i++)
            {
                int nx = x + dx[i];
                int ny = y + dy[i];
                if(0 <= nx && nx < m_Width && 0 <= ny && ny < m_Height)
                {
                    BlockItem * nItem = m_BlockItems[ny][nx];
                    if(nItem->isPressed())
                        nItem->pressOrReleaseItem();
                }
            }

            // 标记不再左右键同时按下
            m_lrPressItem = NULL;
        }

        // 未双按时单独释放左键
        else if(event->button() == Qt::LeftButton && m_lPressItem == item)
        {
            // 翻开该格子
            if(item->isPressed())
                sweepItem(item, isGameover);

            // 标记不再按下左键
            m_lPressItem = NULL;
        }
        else if(event->button() == Qt::RightButton)
        {
            // 标记不再按下右键
            m_rPress = false;
        }

        // qDebug() << m_NumOfMines << " " << m_NumOfUnFlaggedMine << " " << m_NumOfUnSweeped;

        // 判断是否获胜或失败
        if(checkWin())
            win();
        else if(isGameover)
            gameover();
        else
            emit faceSmile();
    }
    else if(item == NULL)
    {
        m_lPressItem = NULL;
        m_lrPressItem = NULL;
        m_rPress = false;
        emit faceSmile();
    }
    QGraphicsScene::mouseReleaseEvent(event);
}
