#ifndef BLOCKITEM_H
#define BLOCKITEM_H

#include <QGraphicsPixmapItem>

// 扫雷格子对象
class BlockItem : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    explicit BlockItem(int row, int col);

    int getRow() const;
    int getCol() const;
    bool isSweeped() const;
    void setSweeped(bool newSweeped);
    bool isMine() const;
    void setMine(bool newMine);
    bool isFlagged() const;
    void setFlagged(bool newFlagged);
    bool isStepMine() const;
    void setStepMine(bool newStepMine);
    int getNumOfMines() const;
    void setNumOfMines(int newNumOfMines);

    void updateItemStatus(bool isOver = false);
    void setPixmapOpen0();

signals:

private:
    int row;
    int col;
    bool sweeped = false;
    bool mine = false;
    bool flagged = false;
    bool stepMine = false;
    int numOfMines = 0;

};

#endif // BLOCKITEM_H
