#include "addattrwindow.h"
#include <QLabel>
#include <QLineEdit>
#include <QSpinBox>
#include <QPushButton>
#include <QCheckBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <fstream>
#include <qDebug>

addattrwindow::addattrwindow(QWidget* parent):
    QWidget(parent)
{
    setAttribute(Qt::WA_DeleteOnClose);
    layout();
}

void addattrwindow::layout()
{
    for(int i = 0;i < HMAX; i++){
        hlayout[i] = new QHBoxLayout();
    }
    for(int i = 0;i < VMAX; i++){
        vlayout[i] = new QVBoxLayout();
    }
    name = new QLabel(QStringLiteral("预设参数名字:"));
    zmin = new QLabel(QStringLiteral("重建起点:"));
    zmax = new QLabel(QStringLiteral("重建终点:"));
    interval = new QLabel(QStringLiteral("平面间距:"));
    lamda = new QLabel(QStringLiteral("重建波长:"));
    Mag = new QLabel(QStringLiteral("放大倍数:"));
    dpix  = new QLabel(QStringLiteral("像素尺寸:"));
    detection  = new QLabel(QStringLiteral("自适应阈值参数:"));
    path = new QLabel(QStringLiteral("用户文件夹:"));
    minRadius = new QLabel(QStringLiteral("最小粒径:"));
    maxRadius = new QLabel(QStringLiteral("最大粒径:"));
    plotnum = new QLabel(QStringLiteral("区间个数:"));

    nameText = new QLineEdit();
    zminText = new QLineEdit();
    zmaxText = new QLineEdit();
    intervalText = new QLineEdit();
    lamdaText = new QLineEdit();
    MagText = new QLineEdit();
    dpixText = new QLineEdit();
    d1Text = new QLineEdit();
    d2Text = new QLineEdit();
    d3Text = new QLineEdit();
    pathText = new QLineEdit();

    minRadiusText = new QSpinBox();
    maxRadiusText = new QSpinBox();
    plotnumText = new QSpinBox();

    isInverse = new QCheckBox(QStringLiteral("反色"));

    okBtn = new QPushButton(QStringLiteral("确定"));
    cancelBtn = new QPushButton(QStringLiteral("取消"));
    choosePathBtn = new QPushButton(QStringLiteral("."));

    hlayout[0]->addWidget(name);
    hlayout[0]->addWidget(nameText);

    hlayout[1]->addWidget(zmin);
    hlayout[1]->addWidget(zminText);
    hlayout[1]->addWidget(zmax);
    hlayout[1]->addWidget(zmaxText);

    hlayout[2]->addWidget(interval);
    hlayout[2]->addWidget(intervalText);
    hlayout[2]->addWidget(lamda);
    hlayout[2]->addWidget(lamdaText);

    hlayout[3]->addWidget(Mag);
    hlayout[3]->addWidget(MagText);
    hlayout[3]->addWidget(dpix);
    hlayout[3]->addWidget(dpixText);

    hlayout[4]->addWidget(detection);
    hlayout[4]->addWidget(d1Text);
    hlayout[4]->addWidget(d2Text);
    hlayout[4]->addWidget(d3Text);

    hlayout[5]->addWidget(path);
    hlayout[5]->addWidget(pathText);
    hlayout[5]->addWidget(choosePathBtn);

    hlayout[6]->addWidget(minRadius);
    hlayout[6]->addWidget(minRadiusText);
    hlayout[6]->addWidget(maxRadius);
    hlayout[6]->addWidget(maxRadiusText);

    hlayout[8]->addWidget(okBtn);
    hlayout[8]->addWidget(cancelBtn);

    hlayout[9]->addWidget(plotnum);
    hlayout[9]->addWidget(plotnumText);
    hlayout[9]->addWidget(isInverse);

    vlayout[0]->addLayout(hlayout[0]);
    vlayout[0]->addLayout(hlayout[1]);
    vlayout[0]->addLayout(hlayout[2]);
    vlayout[0]->addLayout(hlayout[3]);
    vlayout[0]->addLayout(hlayout[4]);
    vlayout[0]->addLayout(hlayout[5]);
    vlayout[0]->addLayout(hlayout[6]);
    vlayout[0]->addLayout(hlayout[9]);
    vlayout[0]->addLayout(hlayout[8]);

    setWindowFlags(Qt::WindowStaysOnTopHint);
    setWindowModality(Qt::ApplicationModal);
    setWindowTitle(QStringLiteral("添加预设参数"));

    setLayout(vlayout[0]);

    connect(okBtn,SIGNAL(clicked()),this,SLOT(addAttr()));
    connect(cancelBtn,SIGNAL(clicked()),this,SLOT(cancelAddAttr()));
}

void addattrwindow::cancelAddAttr()
{
    close();
}

bool addattrwindow::isValid(const premessage_t& msg)
{
    return true;
}

void addattrwindow::addAttr()
{
    premessage_t msg;
    msg.zmin = zminText->text();
    msg.zmax = zmaxText->text();
    msg.detection1 = d1Text->text();
    msg.detection2 = d2Text->text();
    msg.detection3 = d3Text->text();
    msg.dpix = dpixText->text();
    msg.lamda = lamdaText->text();
    msg.interval = intervalText->text();
    msg.Mag = MagText->text();
    msg.location = locationText->text();
    msg.path = pathText->text();
    msg.plotnum = plotnumText->text();
    msg.minRadius = minRadiusText->text();
    msg.maxRadius = maxRadiusText->text();
    msg.isInverse = isInverse->isChecked();

    emit(updateAttr(nameText->text(),msg));
    std::ofstream out("data.txt",std::ios::app);

    out << nameText->text().toStdString() << " ";
    out << zminText->text().toStdString() << " ";
    out << zmaxText->text().toStdString() << " ";
    out << intervalText->text().toStdString() << " ";
    out << lamdaText->text().toStdString() << " ";
    out << MagText->text().toStdString() << " ";
    out << dpixText->text().toStdString() << " ";
    out << d1Text->text().toStdString() << " ";
    out << d2Text->text().toStdString() << " ";
    out << d3Text->text().toStdString() << " ";
    out << locationText->text().toStdString() <<" ";
    out << pathText->text().toStdString() << " ";

    out << minRadiusText->text().toStdString() << " ";
    out << maxRadiusText->text().toStdString() << " ";
    out << plotnumText->text().toStdString() << " ";

    if(isInverse->isChecked()){
        out << "1 ";
    }
    else out << "0 ";


    out<<"\n";
    out.close();

    std::ofstream nameout("name.txt",std::ios::app);
    nameout << nameText->text().toStdString() << " ";

    /*QMessageBox message(QMessageBox::Question, "Message",
                        QStringLiteral("添加预设参数\"")
                        + addattrWindow->nameText->text()
                        + QStringLiteral("\"成功！"),
                        QMessageBox::Yes,addattrWindow);
    message.setWindowFlags(Qt::WindowStaysOnTopHint);
    message.exec();*/
    nameout.close();

    close();
}

