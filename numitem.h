#ifndef NUMITEM_H
#define NUMITEM_H

#include <QGraphicsPixmapItem>

class NumItem : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    explicit NumItem(int num);

    void setNum(int num);

signals:

};

#endif // NUMITEM_H
