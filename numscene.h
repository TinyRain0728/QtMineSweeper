#ifndef NUMSCENE_H
#define NUMSCENE_H

#include <QGraphicsScene>
class NumItem;

class NumScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit NumScene(QObject *parent = nullptr, int num = 0);

public slots:
    void on_numChanged(int newNum);

signals:

private:
    int m_Num;
    NumItem *m_Hun;
    NumItem *m_Ten;
    NumItem *m_One;

};

#endif // NUMSCENE_H
