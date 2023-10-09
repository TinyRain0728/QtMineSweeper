#include "blockitem.h"

BlockItem::BlockItem(int row, int col)
{
    this->row = row;
    this->col = col;
    QPixmap pixItem = QPixmap(":/images/images/blank.jpg");
    pixItem = pixItem.scaled(QSize(16, 16), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    this->setPixmap(pixItem);
}

// 根据格子属性更新显示图片
void BlockItem::updateItemStatus(bool isOver)
{
    QPixmap pixItem;
    if(!isOver && !this->isSweeped())
    {
        if(this->isFlagged())
            pixItem = QPixmap(":/images/images/bombFlagged.jpg");
        else if(this->isPressed())
            pixItem = QPixmap(":/images/images/open0.jpg");
        else
            pixItem = QPixmap(":/images/images/blank.jpg");
    }
    else
    {
        if(isOver)
        {
            if(this->isFlagged() && !this->isMine())
                pixItem = QPixmap(":/images/images/bombWrong.jpg");
            else if(this->isFlagged())
                pixItem = QPixmap(":/images/images/bombFlagged.jpg");
            else if(this->isStepMine())
                pixItem = QPixmap(":/images/images/bombDeath.jpg");
            else if(!this->isFlagged())
                pixItem = QPixmap(":/images/images/bombRevealed.jpg");
        }
        else
        {
            switch (this->numOfMines) {
            case 0:
                pixItem = QPixmap(":/images/images/open0.jpg");break;
            case 1:
                pixItem = QPixmap(":/images/images/open1.jpg");break;
            case 2:
                pixItem = QPixmap(":/images/images/open2.jpg");break;
            case 3:
                pixItem = QPixmap(":/images/images/open3.jpg");break;
            case 4:
                pixItem = QPixmap(":/images/images/open4.jpg");break;
            case 5:
                pixItem = QPixmap(":/images/images/open5.jpg");break;
            case 6:
                pixItem = QPixmap(":/images/images/open6.jpg");break;
            case 7:
                pixItem = QPixmap(":/images/images/open7.jpg");break;
            case 8:
                pixItem = QPixmap(":/images/images/open8.jpg");break;
            }
        }
    }

    pixItem = pixItem.scaled(QSize(16, 16), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    this->setPixmap(pixItem);
}

// 按下或释放格子，更新格子状态
void BlockItem::pressOrReleaseItem()
{
    if(!isSweeped() && !isFlagged())
    {
        setPressed(!isPressed());
        updateItemStatus();
    }
}

int BlockItem::getRow() const
{
    return row;
}

int BlockItem::getCol() const
{
    return col;
}

bool BlockItem::isSweeped() const
{
    return sweeped;
}

void BlockItem::setSweeped(bool newSweeped)
{
    sweeped = newSweeped;
}

bool BlockItem::isMine() const
{
    return mine;
}

void BlockItem::setMine(bool newMine)
{
    mine = newMine;
}

bool BlockItem::isFlagged() const
{
    return flagged;
}

void BlockItem::setFlagged(bool newFlagged)
{
    flagged = newFlagged;
}

bool BlockItem::isPressed() const
{
    return pressed;
}

void BlockItem::setPressed(bool newPressed)
{
    pressed = newPressed;
}

int BlockItem::getNumOfMines() const
{
    return numOfMines;
}

void BlockItem::setNumOfMines(int newNumOfMines)
{
    numOfMines = newNumOfMines;
}

bool BlockItem::isStepMine() const
{
    return stepMine;
}

void BlockItem::setStepMine(bool newStepMine)
{
    stepMine = newStepMine;
}
