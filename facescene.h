#ifndef FACESCENE_H
#define FACESCENE_H

#include <QGraphicsScene>
class FaceItem;

class FaceScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit FaceScene(QObject *parent = nullptr);

signals:
    void faceClicked();     //connect restart

public slots:
    void do_FaceSmile();
    void do_FaceOoh();
    void do_FaceWin();
    void do_FaceDead();
    void do_FaceSmilePressed();

private:
    enum{faceSmile, faceOoh, faceWin, faceDead, faceSmilePressed};
    FaceItem *m_FaceItem;

    // QGraphicsScene interface
protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
};

#endif // FACESCENE_H
