#ifndef BLOCKSCENE_H
#define BLOCKSCENE_H

#include <QGraphicsScene>
class BlockItem;
class QTimer;
class QElapsedTimer;

class BlockScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit BlockScene(QObject *parent = nullptr);
    void setSize(int type = 0, int width = 9, int height = 9, int numOfMines = 10);
    void InitBlockScene();
    BlockItem* getCurItem(QGraphicsSceneMouseEvent *event);
    void sweepItem(BlockItem* item, bool &gameover);
    bool checkWin();
    void gameover();
    void win();

signals:
    void faceSmile();
    void faceOoh();
    void faceWin();
    void faceDead();
    void faceSmilePressed();

    void flagChange(int restMineNum);
    void timeChange(int timeNum);
    void scoreChange(int i, double score);

private slots:
    void do_timeout();

private:
    // 游戏难度相关
    int m_Type;
    int m_Width;
    int m_Height;
    int m_NumOfMines;

    // 用于检查是否获胜
    int m_NumOfUnSweeped;
    int m_NumOfUnFlaggedMine;

    // 记录游戏开始结束状态
    bool m_Over = false;

    // 用于记录鼠标活动
    bool m_rPress = false;
    BlockItem* m_lPressItem = NULL;
    BlockItem* m_lrPressItem = NULL;

    // 用于显示时间及获胜后记录分数
    QTimer *m_Timer = NULL;     // 统计时间给时间显示视图
    QElapsedTimer *m_ElapsedTimer = NULL;   // 统计总时间，记为分数
    int m_TimeNum;

    // 格子数组
    QVector<QVector<BlockItem*>> m_BlockItems;

    // 用于在失败后更新对应格子
    QList<BlockItem*> m_MineList;
    QList<BlockItem*> m_FlagList;

    // 逐行遍历周围3*3格子时x、y值相对中心格子的偏移量
    int dx[9] = {-1, 0, 1, -1, 0, 1, -1, 0, 1};
    int dy[9] = {-1, -1, -1, 0, 0, 0, 1, 1, 1};

    // QGraphicsScene interface
protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

    // QGraphicsScene interface
};

#endif // BLOCKSCENE_H
