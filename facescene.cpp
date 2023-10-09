#include "facescene.h"
#include "faceitem.h"

FaceScene::FaceScene(QObject *parent)
    : QGraphicsScene{parent}
{
    m_FaceItem = new FaceItem(faceSmile);
    m_FaceItem->setPos(0, 0);
    this->addItem(m_FaceItem);
}

void FaceScene::do_FaceSmile()
{
    m_FaceItem->setFace(faceSmile);
}

void FaceScene::do_FaceOoh()
{
    m_FaceItem->setFace(faceOoh);
}

void FaceScene::do_FaceWin()
{
    m_FaceItem->setFace(faceWin);
}

void FaceScene::do_FaceDead()
{
    m_FaceItem->setFace(faceDead);
}

void FaceScene::do_FaceSmilePressed()
{
    m_FaceItem->setFace(faceSmilePressed);
}

void FaceScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    m_FaceItem->setFace(faceSmilePressed);
    QGraphicsScene::mousePressEvent(event);
}

void FaceScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    emit faceClicked();
    m_FaceItem->setFace(faceSmile);
    QGraphicsScene::mouseReleaseEvent(event);
}
