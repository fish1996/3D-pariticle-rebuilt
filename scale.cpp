#include "scale.h"
#include <QPainter>
#include <qDebug>

static int cnt = 0;
scale::scale(QWidget* parent):
    QWidget(parent)
{
    id = cnt++;
    qDebug()<<"new scale";
    length = 0;
    //setFixedSize(SIZEW,SIZEH);
    move(200,10);
    update();
}

void scale::updateScale(float l)
{

    length = l;
    update();
}

void scale::setWavelength(double w)
{
    wavelength = w;
}

void scale::paintEvent(QPaintEvent* )
{

    QPainter painter(this);

    painter.setPen(QColor(0,0,0));
    painter.setBrush(QColor(0,0,0));
    painter.drawLine(0,0,100,0);

    if(length!=0){
       // qDebug()<<"id="<<id<<"updateScale"<<length;
        painter.drawText(rect(),QString::number(100.0*wavelength/length)+QString("um"));
    }
}
