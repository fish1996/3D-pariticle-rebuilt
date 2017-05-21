#include "scale.h"
#include <QPainter>
#include <qDebug>

static int cnt = 0;
scale::scale(QWidget* parent):
    QWidget(parent)
{
    size = new int();
    *size = 100;
    isWhite = false;
    id = cnt++;
    length = 0;
    setFixedWidth(400);
    update();
}

void scale::Move(int x,int y)
{
    cur_x = x;
    cur_y = y;
    move(x,y);
}

void scale::updateScale(float l)
{
    length = l;
    update();
}

void scale::isShowWhite(bool is)
{
    isWhite = is;
    update();
}

void scale::setWavelength(double w)
{
    wavelength = w;
}

void scale::paintEvent(QPaintEvent* e)
{
    if(length ==0)return;
    QPainter painter(this);
    if(!isWhite) {
        painter.setPen(QColor(0,0,0));
        painter.setBrush(QColor(0,0,0));
    }
    else {
        painter.setPen(QColor(255,255,255));
        painter.setPen(QColor(255,255,255));
    }

    float len = (*size) * length/(wavelength);
    painter.drawLine(0,0,len,0);

    if(length!=0){
        painter.drawText(rect(),QString::number((*size))+QString("um"));
    }
}

void scale::moveLeft()
{
    cur_x -= 5;
    move(cur_x,cur_y);
}

void scale::moveRight()
{
    cur_x += 5;
    move(cur_x,cur_y);
}
void scale::moveFront()
{
    cur_y -= 5;
    move(cur_x,cur_y);
}
void scale::moveBack()
{
    cur_y += 5;
    move(cur_x,cur_y);
}

int scale::getCurX()
{
    return cur_x;
}

int scale::getCurY()
{
    return cur_y;
}

