#include "numitem.h"

NumItem::NumItem(int num)
{
    this->setNum(num);
}

void NumItem::setNum(int num)
{
    QPixmap pixNum;
    switch (num) {
    case 0:
        pixNum = QPixmap(":/images/images/num0.jpg");   break;
    case 1:
        pixNum = QPixmap(":/images/images/num1.jpg");   break;
    case 2:
        pixNum = QPixmap(":/images/images/num2.jpg");   break;
    case 3:
        pixNum = QPixmap(":/images/images/num3.jpg");   break;
    case 4:
        pixNum = QPixmap(":/images/images/num4.jpg");   break;
    case 5:
        pixNum = QPixmap(":/images/images/num5.jpg");   break;
    case 6:
        pixNum = QPixmap(":/images/images/num6.jpg");   break;
    case 7:
        pixNum = QPixmap(":/images/images/num7.jpg");   break;
    case 8:
        pixNum = QPixmap(":/images/images/num8.jpg");   break;
    case 9:
        pixNum = QPixmap(":/images/images/num9.jpg");   break;
    case -1:
        pixNum = QPixmap(":/images/images/num-.jpg");   break;
    default:
        break;
    }
    pixNum = pixNum.scaled(QSize(13, 23), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    this->setPixmap(pixNum);
}
