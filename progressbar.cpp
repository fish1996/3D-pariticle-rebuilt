#include "progressbar.h"
#include <qDebug>
#include <QPainter>

progressbar::progressbar(QString n,QWidget* parent):
    QWidget(parent)
{
    name = n;
    count = 0;
    layout();
}

void progressbar::display()
{
    show();
}

void progressbar::layout()
{
   // setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint);
    setFixedSize(SIZEW,SIZEH);
    setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint|Qt::WindowStaysOnTopHint);
    setWindowModality(Qt::ApplicationModal);
    setWindowTitle(QStringLiteral("正在执行")+name+QStringLiteral("..."));
}

void progressbar::closeEvent(QCloseEvent *)
{
    emit(isClose());
}

void progressbar::paintEvent(QPaintEvent* )
{
    QPainter painter(this);

    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(QColor(0,0,0));
    painter.drawText(150,30,
                     QStringLiteral("第")
                     + QString::number(count)
                     + QStringLiteral("张，共")
                     + QString::number(total)
                     + QStringLiteral("张"));
    painter.setPen(QColor(0,0,0));
    painter.setBrush(Qt::NoBrush);
    painter.drawRoundedRect(QRect(30,80,SIZEW-60,SIZEH-160),5,5);

    painter.setBrush(QColor(0,0,0));
    if(total==0)return;
    float size = 1.0*(SIZEW - 60)/total;
    painter.drawRoundedRect(QRect(30,80,size*count,SIZEH-160),5,5);
    if(total==count){

       // emit(allfinish(1));
        close();
    }
    //qDebug()<<"finish"<<count;
}

void progressbar::changeState()
{

    count++;
    update();
}
