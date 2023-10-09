#ifndef FACEITEM_H
#define FACEITEM_H

#include <QGraphicsPixmapItem>

class FaceItem : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    explicit FaceItem(int type = 0);
    void setFace(int type);

signals:

private:
    QPixmap m_PixFace;
};

#endif // FACEITEM_H
