#include "slider.h"
#include "button.h"
#include <QSlider>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QLabel>
#include <qDebug>

slider::slider(QWidget* parent)
    :QWidget(parent)
{
    count = 1;
    total = 100;
    slid = new QSlider();
    hlayout = new QHBoxLayout();
    hlayout1 = new QHBoxLayout();
    QHBoxLayout *hlayout2 = new QHBoxLayout();
    vlayout = new QVBoxLayout();
    nextbtn = new Button();
    lastbtn = new Button();
    label = new QLineEdit("1");
    label->setFixedWidth(30);
    label->setEnabled(false);
    location = new QLineEdit();
    location->setEnabled(false);

    text = new QLabel(QStringLiteral("第"));

    slid->setOrientation(Qt::Horizontal);
    QLabel* text1 = new QLabel(QStringLiteral("页"));
    QLabel* text2 = new QLabel(QStringLiteral("位置"));
    QIcon nextico,lastico;
    nextico.addFile(":/image/next.ico",QSize(40,40));
    lastico.addFile(":/image/previous.ico",QSize(40,40));
    nextbtn->SetButtonIcons(nextico,nextico,nextico);
    lastbtn->SetButtonIcons(lastico,lastico,lastico);

    nextbtn->setIconSize(QSize(30,30));
    lastbtn->setIconSize(QSize(30,30));
    nextbtn->setFixedSize(30,30);
    lastbtn->setFixedSize(30,30);

    hlayout->addWidget(lastbtn);
    hlayout->addWidget(nextbtn);

    hlayout1->addWidget(text);
    hlayout1->addWidget(label);
    hlayout1->addWidget(text1);

    hlayout2->addWidget(text2);
    hlayout2->addWidget(location);


    vlayout->addWidget(slid);
    vlayout->addLayout(hlayout);
    vlayout->addLayout(hlayout1);
    vlayout->addLayout(hlayout2);
    setLayout(vlayout);

    connect(nextbtn,SIGNAL(clicked()),this,SLOT(next()));
    connect(lastbtn,SIGNAL(clicked()),this,SLOT(last()));
    connect(slid,SIGNAL(valueChanged(int)),this,SLOT(setNum(int)));

    setDisable();
}

void slider::set(double _zmin,double _interval)
{
    zmin = 0.01*_zmin;
    interval = 0.001*_interval;
}

void slider::next()
{
    if(count>=total)return;
    count++;
    slid->setValue(count);
    label->setText(QString::number(count));
}

void slider::last()
{
    if(count<=1)return;
    count--;
    slid->setValue(count);
    label->setText(QString::number(count));
}

void slider::setNum(int c,int t)
{
    total = t;
    count = c;
    slid->setRange(1,total);
    setEnable();
}

void slider::setNum(int n)
{
    count = n;
    label->setText(QString::number(count));
    slid->setValue(count);
    location->setText(QString::number(zmin+0.1*(count-1)*interval)+"cm");
}

void slider::setDisable()
{
    slid->setDisabled(true);
    nextbtn->setDisabled(true);
    lastbtn->setDisabled(true);
}

void slider::setEnable()
{
    slid->setEnabled(true);
    nextbtn->setEnabled(true);
    lastbtn->setEnabled(true);
}


