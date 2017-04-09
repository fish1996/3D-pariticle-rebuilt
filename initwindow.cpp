#include "initwindow.h"
#include <QLabel>
#include <QPushButton>
#include <QGridLayout>
#include <QComboBox>
#include <QLineEdit>
#include <QSpinBox>
#include <qDebug>

initwindow::initwindow(QVector<QString>* n,map_t* m, QWidget* parent)
    :QWidget(parent)
{
    map = m;
    namelist = n;
    setAttribute(Qt::WA_DeleteOnClose);
    layout();
}

void initwindow::layout()
{
    qDebug()<<"layout";
    for(int i=0;i<HMAX;i++){
        hlayout[i] = new QHBoxLayout();
    }

    for(int i=0;i<VMAX;i++){
        vlayout[i] = new QVBoxLayout();
    }

    image = new QLabel();
    image->setPixmap(QPixmap(":/image/setting.ico"));
    preSet = new QLabel(QStringLiteral("预设参数"));
    pixelSize = new QLabel(QStringLiteral("像素尺寸"));
    wavelength = new QLabel(QStringLiteral("激光波长"));
    startpoint = new QLabel(QStringLiteral("重建起点"));
    endpoint = new QLabel(QStringLiteral("重建终点"));
    plane = new QLabel(QStringLiteral("平面间距"));

    okBtn = new QPushButton(QStringLiteral(" 确定 "));
    cancelBtn = new QPushButton(QStringLiteral(" 取消 "));
    plusBtn = new QPushButton(QStringLiteral("+"));
    minusBtn = new QPushButton(QStringLiteral("-"));

    pixelBox = new QLineEdit();
    waveBox = new QLineEdit();
    startBox = new QLineEdit();
    endBox = new QLineEdit();
    planeBox = new QLineEdit();

    combobox = new QComboBox();
    for(int i=0;i<namelist->size();i++){
        combobox->addItem((*namelist)[i],i);
    }

    setValue(0);

    hlayout[0]->addWidget(preSet);
    hlayout[0]->addWidget(combobox);
    hlayout[0]->addWidget(plusBtn);
    hlayout[0]->addWidget(minusBtn);

    hlayout[1]->addWidget(pixelSize);
    hlayout[1]->addWidget(pixelBox);

    hlayout[2]->addWidget(wavelength);
    hlayout[2]->addWidget(waveBox);

    hlayout[3]->addWidget(startpoint);
    hlayout[3]->addWidget(startBox);

    hlayout[4]->addWidget(endpoint);
    hlayout[4]->addWidget(endBox);

    hlayout[5]->addWidget(plane);
    hlayout[5]->addWidget(planeBox);

    hlayout[6]->addStretch();
    hlayout[6]->addWidget(okBtn);
    hlayout[6]->addWidget(cancelBtn);

    vlayout[0]->addLayout(hlayout[0]);
    vlayout[0]->addLayout(hlayout[1]);
    vlayout[0]->addLayout(hlayout[2]);
    vlayout[0]->addLayout(hlayout[3]);
    vlayout[0]->addLayout(hlayout[4]);
    vlayout[0]->addLayout(hlayout[5]);
    vlayout[0]->addLayout(hlayout[6]);

    hlayout[7]->addWidget(image);
    hlayout[7]->addLayout(vlayout[0]);

    setLayout(hlayout[7]);

    //setWindowFlags(Qt::WindowStaysOnTopHint);
    setWindowModality(Qt::ApplicationModal);
    setWindowTitle(QStringLiteral("设置"));

    connect(combobox,SIGNAL(currentIndexChanged(int)),this,SLOT(setValue(int)));
    connect(plusBtn,SIGNAL(clicked()),this,SLOT(plus()));
    connect(minusBtn,SIGNAL(clicked()),this,SLOT(minus()));
    qDebug()<<"finish";
}

void initwindow::plus()
{
    addattrWindow = new addattrwindow();
    connect(addattrWindow,SIGNAL(updateAttr(QString,premessage_t)),
            this,SLOT(updateAttr(QString,premessage_t)));
    addattrWindow->show();
}

void initwindow::minus()
{
    removeattrWindow = new removeattrwindow(map,namelist);
    connect(removeattrWindow,SIGNAL(updateStyle()),this,SLOT(updateStyle()));
    removeattrWindow->show();
}

void initwindow::setValue(int i)
{
    QString name = (*namelist)[i];
    waveBox->setText((*map)[name].lamda);
    pixelBox->setText((*map)[name].dpix);
    startBox->setText((*map)[name].zmin);
    endBox->setText((*map)[name].zmax);
    planeBox->setText((*map)[name].Mag);
}

void initwindow::updateStyle()
{
    disconnect(combobox, SIGNAL(currentIndexChanged(int)), this, SLOT(setValue(int)));
    combobox->clear();
    connect(combobox, SIGNAL(currentIndexChanged(int)), this, SLOT(setValue(int)));
    for(int i=0;i<namelist->size();i++){
        combobox->addItem((*namelist)[i],i);
    }
}

void initwindow::updateAttr(QString str,premessage_t msg)
{
    namelist->push_back(str);
    (*map)[str] = msg;
    combobox->addItem(str,namelist->size()-1);
}
