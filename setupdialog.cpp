#include "setupdialog.h"
#include "addattrwindow.h"
#include "removeattrwindow.h"

#include <QLineEdit>
#include <QSpinBox>
#include <QPushButton>
#include <QCheckBox>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QComboBox>
#include <qDebug>
#include <QDataStream>
#include <QFile>
#include <QMessageBox>
#include <fstream>
#include <QUrl>
#include <QFileDialog>

bool setupdialog::isCreate = false;

void setupdialog::Add()
{
    addattrWindow = new addattrwindow();
    addattrWindow->show();
    connect(addattrWindow->okBtn,SIGNAL(clicked()),this,SLOT(addAttr()));
    connect(addattrWindow->cancelBtn,SIGNAL(clicked()),this,SLOT(cancelAddAttr()));

}

void setupdialog::closeEvent(QCloseEvent*)
{
    qDebug()<<"isClose";
    isCreate = false;
}

void setupdialog::Remove()
{

    removeattrWindow = new removeattrwindow(map,namelist);
    removeattrWindow->show();
    connect(removeattrWindow,SIGNAL(updateStyle()),this,SLOT(updateAttr()));
}

void setupdialog::updateAttr()
{
    disconnect(attrBox, SIGNAL(currentIndexChanged(int)), this, SLOT(setValue(int)));
    attrBox->clear();
    connect(attrBox, SIGNAL(currentIndexChanged(int)), this, SLOT(setValue(int)));
    for(int i=0;i<namelist->size();i++){
        attrBox->addItem((*namelist)[i],i);
    }
}

void setupdialog::updateAttr(QString str,premessage_t msg)
{
    namelist->push_back(str);
    (*map)[str] = msg;
    attrBox->addItem(str,namelist->size()-1);
}

setupdialog::setupdialog(QString p,QVector<QString>* n,map_t* m,double size,message_t* msg,QWidget* parent)
    :QWidget(parent)
{
    preattr = p;
    dpix = size;
    namelist = n;
    map = m;
    message = msg;

    isCreate = true;
    layout();
    setConnect();
}

void setupdialog::setConnect()
{
    connect(addAttrBtn,SIGNAL(clicked()),this,SLOT(Add()));
    connect(removeAttrBtn,SIGNAL(clicked()),this,SLOT(Remove()));
    connect(attrBox,SIGNAL(currentIndexChanged(int)),this,SLOT(setValue(int)));
    connect(chooseDir,SIGNAL(clicked()),this,SLOT(choose()));
}

void setupdialog::choose()
{
    QUrl url = QFileDialog::getExistingDirectoryUrl
            (this);
    QString path = url.toString().mid(8);
    if(path!="")fileDirText->setText(path);
}

setupdialog::~setupdialog()
{
//    disconnect(addAttrBtn,0,this,0);
//    disconnect(removeAttrBtn,0,this,0);

    qDebug()<<"~setupdialog";
    isCreate = false;
    delete line1;
    delete line2;
    delete line3;
    delete fileDirText;

    delete intervalText;
    delete minRadiusText;
    delete maxRadiusText;

    delete detection;
    delete chooseDir;
    delete addAttrBtn;
    delete removeAttrBtn;

    delete isInverse;

    delete fileDir;
    delete interval;
    delete minRadius;
    delete maxRadius;
    delete preAttr;

    for(int i=0;i<HMAX;i++){
        delete hlayout[i];
    }
    for(int i=0;i<VMAX;i++){
        delete vlayout[i];
    }
}

void setupdialog::layout()
{
   // setAttribute(Qt::WA_DeleteOnClose,true);
    line1 = new QLineEdit();
    line2 = new QLineEdit();
    line3 = new QLineEdit();
    fileDirText = new QLineEdit();

    intervalText = new QSpinBox();
    minRadiusText = new QSpinBox();
    maxRadiusText = new QSpinBox();

    attrBox = new QComboBox();
  //  attrBox->addItem("",namelist->size());
    for(int i=0;i<namelist->size();i++){
        attrBox->addItem((*namelist)[i],i);
    }

    chooseDir = new QPushButton(QStringLiteral("."));
    addAttrBtn = new QPushButton(QStringLiteral("+"));
    removeAttrBtn = new QPushButton(QStringLiteral("-"));
    okBtn = new QPushButton(QStringLiteral(" 确定 "));
    cancelBtn = new QPushButton(QStringLiteral(" 取消 "));

    isInverse = new QCheckBox(QStringLiteral("反色"));

    detection = new QLabel(QStringLiteral("自适应阈值参数"));
    fileDir = new QLabel(QStringLiteral("用户文件夹"));
    interval = new QLabel(QStringLiteral("区间数"));
    minRadius = new QLabel(QStringLiteral("最小粒径"));
    maxRadius = new QLabel(QStringLiteral("最大粒径"));
    preAttr = new QLabel(QStringLiteral("预设参数"));


    for(int i=0;i<HMAX;i++){
        hlayout[i] = new QHBoxLayout();
    }
    for(int i=0;i<VMAX;i++){
        vlayout[i] = new QVBoxLayout();
    }

    if(preattr=="")setValue(0);
    else {
        attrBox->setCurrentText(preattr);
        setValue(attrBox->currentIndex());
    }
    hlayout[0]->addWidget(detection);
    hlayout[0]->addWidget(line1);
    hlayout[0]->addWidget(line2);
    hlayout[0]->addWidget(line3);

    hlayout[1]->addWidget(fileDir);
    hlayout[1]->addWidget(fileDirText);
    hlayout[1]->addWidget(chooseDir);

    hlayout[2]->addWidget(preAttr);
    hlayout[2]->addWidget(attrBox);
    hlayout[2]->addWidget(addAttrBtn);
    hlayout[2]->addWidget(removeAttrBtn);

    hlayout[3]->addWidget(minRadius);
    hlayout[3]->addWidget(minRadiusText);
    hlayout[3]->addWidget(maxRadius);
    hlayout[3]->addWidget(maxRadiusText);

    hlayout[4]->addStretch();
    hlayout[4]->addWidget(okBtn);
    hlayout[4]->addWidget(cancelBtn);

    hlayout[5]->addWidget(isInverse);
    hlayout[5]->addWidget(interval);
    hlayout[5]->addWidget(intervalText);

    vlayout[0]->addLayout(hlayout[0]);
    vlayout[0]->addLayout(hlayout[1]);
    vlayout[0]->addLayout(hlayout[2]);
    vlayout[0]->addLayout(hlayout[3]);
    vlayout[0]->addLayout(hlayout[5]);
    vlayout[0]->addLayout(hlayout[4]);

    setWindowFlags(Qt::WindowStaysOnTopHint);
  //  setWindowModality(Qt::ApplicationModal);
    setWindowTitle(QStringLiteral("设置"));

    setLayout(vlayout[0]);
    qDebug()<<"ok";
}

void setupdialog::setValue(int i)
{/*
    if(i==namelist->size()) {
        line1->setText(QString::number(message->detection1));
        line2->setText(QString::number(message->detection2));
        line3->setText(QString::number(message->detection3));

        maxRadiusText->setRange(0,100000);
        fileDirText->setText(message->path);
        minRadiusText->setValue(message->minRadius);
        maxRadiusText->setValue(message->maxRadius);
        intervalText->setValue(message->interval);
        isInverse->setChecked(true);

        return;
    }*/
    QString name = (*namelist)[i];

    line1->setText((*map)[name].detection1);
    line2->setText((*map)[name].detection2);
    line3->setText((*map)[name].detection3);

    maxRadiusText->setRange(0,100000);
    fileDirText->setText((*map)[name].path);
    minRadiusText->setValue((*map)[name].minRadius.toInt());
    maxRadiusText->setValue((*map)[name].maxRadius.toInt());
    intervalText->setValue((*map)[name].plotnum.toInt());

    if((*map)[name].isInverse==true){
        isInverse->setChecked(true);
    }
}
