#include "faceitem.h"

FaceItem::FaceItem(int type)
{
    this->setFace(type);
}

void FaceItem::setFace(int type)
{
    switch (type) {
    case 0:
        m_PixFace = QPixmap(":/images/images/faceSmile.jpg");   break;
    case 1:
        m_PixFace = QPixmap(":/images/images/faceOoh.jpg");   break;
    case 2:
        m_PixFace = QPixmap(":/images/images/faceWin.jpg");   break;
    case 3:
        m_PixFace = QPixmap(":/images/images/faceDead.jpg");   break;
    case 4:
        m_PixFace = QPixmap(":/images/images/faceSmilePressed.jpg");   break;
    }
    m_PixFace = m_PixFace.scaled(QSize(26, 26), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    this->setPixmap(m_PixFace);
}
