#include "toolwindow.h"
#include <QIcon>
#include <QRadioButton>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QButtonGroup>
#include <QPushButton>
#include <QPainter>

toolwindow::toolwindow(QWidget* window):
    QWidget(window)
{
    layout();
}

void toolwindow::paintEvent(QPaintEvent *)
{
    QPainter paint(this);
    paint.setPen(QColor(255,255,255));
    paint.setBrush(QColor(255,255,255));
    paint.drawRect(rect());
}

void toolwindow::set(double zmin,double interval)
{
    slid->set(zmin,interval);
}

void toolwindow::layout()
{
    //setStyleSheet("background-color:rgb(200,200,200);");

    setFixedWidth(150);

    vlayout = new QVBoxLayout();
    glayout = new QGridLayout();

    slid = new slider();

    inverseBtn = new holdButton();
    scaleBtn = new holdButton();
    calculateBtn = new holdButton();

    inverseBtn->set(QStringLiteral("反色"),":/image/inverse.ico");
    scaleBtn->set(QStringLiteral("比例尺"),":/image/rule.ico");
    calculateBtn->set(QStringLiteral("测量"),":/image/cal.ico");

    scaleBtn->setClicked(true);

    zoomInBtn = new Button();
    zoomOutBtn = new Button();
    originBtn = new Button();
    fullScreenBtn = new Button();

    QIcon zoom_in,zoom_out,zoom_origin,zoom_fit;
    zoom_in.addFile(":/image/zoom_in.ico",QSize(30,30));
    zoom_out.addFile(":/image/zoom_out.ico",QSize(30,30));
    zoom_origin.addFile(":/image/zoom_original.ico",QSize(30,30));
    zoom_fit.addFile(":/image/zoom_fit.ico",QSize(30,30));

    zoomInBtn->SetButtonIcons(zoom_in,zoom_in,zoom_in);
    zoomOutBtn->SetButtonIcons(zoom_out,zoom_out,zoom_out);
    originBtn->SetButtonIcons(zoom_origin,zoom_origin,zoom_origin);
    fullScreenBtn->SetButtonIcons(zoom_fit,zoom_fit,zoom_fit);

    zoomInBtn->setIconSize(QSize(20,20));
    zoomOutBtn->setIconSize(QSize(20,20));
    originBtn->setIconSize(QSize(20,20));
    fullScreenBtn->setIconSize(QSize(20,20));

    glayout->addWidget(zoomInBtn,1,1);
    glayout->addWidget(zoomOutBtn,1,2);
    glayout->addWidget(originBtn,2,1);
    glayout->addWidget(fullScreenBtn,2,2);

    vlayout->addLayout(glayout);
    vlayout->addSpacing(20);
    vlayout->addWidget(inverseBtn);
    vlayout->addWidget(scaleBtn);
    vlayout->addWidget(calculateBtn);
    vlayout->addSpacing(20);
    vlayout->addWidget(slid);
    vlayout->addStretch();

    setLayout(vlayout);
}
