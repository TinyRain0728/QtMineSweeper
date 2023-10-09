#include "numscene.h"
#include "numitem.h"

NumScene::NumScene(QObject *parent, int num)
    : QGraphicsScene{parent}
{
    if(num < 0 || num > 999)
    {
//        qDebug() << "NumScene数值溢出";
        return;
    }

    this->m_Num = num;

    int hun = m_Num / 100;
    int ten = m_Num % 100 / 10;
    int one = m_Num % 10;

    m_Hun = new NumItem(hun);
    m_Ten = new NumItem(ten);
    m_One = new NumItem(one);

    this->addItem(m_Hun);
    this->addItem(m_Ten);
    this->addItem(m_One);

    m_Hun->setPos(0 * 13, 0);
    m_Ten->setPos(1 * 13, 0);
    m_One->setPos(2 * 13, 0);

}

void NumScene::on_numChanged(int newNum)
{
    if(newNum < -99 || newNum > 999)
    {
//        qDebug() << "NumScene数值溢出";
        return;
    }

    int hun = m_Num < 0 ? -1 : m_Num / 100;
    int ten = abs(m_Num) % 100 / 10;
    int one = abs(m_Num) % 10;

    int newHun = newNum < 0 ? -1 : newNum / 100;
    int newTen = abs(newNum) % 100 / 10;
    int newOne = abs(newNum) % 10;

    m_Num = newNum;

    if(hun != newHun)
        m_Hun->setNum(newHun);
    if(ten != newTen)
        m_Ten->setNum(newTen);
    if(one != newOne)
        m_One->setNum(newOne);
}
