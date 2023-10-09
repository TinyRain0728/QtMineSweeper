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
    int m_Type;
    int m_Width;
    int m_Height;
    int m_NumOfMines;
    int m_NumOfUnSweeped;
    int m_NumOfUnFlaggedMine;
    bool m_LeftAndRightPressed = false;
    bool m_LeftAndRightReleased = false;
    bool m_Over = false;

    QTimer *m_Timer = NULL;
    QElapsedTimer *m_ElapsedTimer = NULL;
    int m_TimeNum;

    QVector<QVector<BlockItem*>> m_BlockItems;
    QList<BlockItem*> m_MineList;
    QList<BlockItem*> m_FlagList;


    // QGraphicsScene interface
protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
};

#endif // BLOCKSCENE_H
