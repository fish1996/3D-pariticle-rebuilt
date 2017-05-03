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
   // move(200,40);
    setFixedWidth(400);
    update();
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
    QPainter painter(this);
    if(!isWhite) {
        painter.setPen(QColor(0,0,0));
        painter.setBrush(QColor(0,0,0));
    }
    else {
        painter.setPen(QColor(255,255,255));
        painter.setPen(QColor(255,255,255));
    }
    painter.drawLine(0,0,*size,0);

    if(length!=0){
        painter.drawText(rect(),QString::number((*size)*wavelength/length)+QString("um"));
    }
}
