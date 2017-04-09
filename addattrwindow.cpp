#include "addattrwindow.h"
#include <QLabel>
#include <QLineEdit>
#include <QSpinBox>
#include <QPushButton>
#include <QCheckBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <fstream>
#include <QUrl>
#include <QFileDialog>
#include <qDebug>
#include <QMessageBox>

bool addattrwindow::isDigit(QString s)
{
    bool flag = false;
    for(int i = 0; i < s.size();i++) {
        if((s.at(i)<'0' || s.at(i)>'9')&& s.at(i)!='.') {
            return false;
        }
        if(s.at(i)=='.'){
            if(flag == false){
                flag = true;
            }
            else return false;
        }
    }
    return true;
}

addattrwindow::addattrwindow(QWidget* parent):
    QWidget(parent)
{
   // setAttribute(Qt::WA_DeleteOnClose);
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
    pathText->setDisabled(true);

    minRadiusText = new QSpinBox();
    maxRadiusText = new QSpinBox();
    plotnumText = new QSpinBox();

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

    vlayout[0]->addLayout(hlayout[0]);
    vlayout[0]->addLayout(hlayout[1]);
    vlayout[0]->addLayout(hlayout[2]);
    vlayout[0]->addLayout(hlayout[3]);
    vlayout[0]->addLayout(hlayout[4]);
    vlayout[0]->addLayout(hlayout[5]);
    vlayout[0]->addLayout(hlayout[6]);
    vlayout[0]->addLayout(hlayout[9]);
    vlayout[0]->addLayout(hlayout[8]);

  //  setWindowFlags(Qt::WindowStaysOnTopHint);
    setWindowModality(Qt::ApplicationModal);
    setWindowTitle(QStringLiteral("添加预设参数"));

    setLayout(vlayout[0]);

    connect(okBtn,SIGNAL(clicked()),this,SLOT(addAttr()));
    connect(cancelBtn,SIGNAL(clicked()),this,SLOT(cancelAddAttr()));
    connect(choosePathBtn,SIGNAL(clicked()),this,SLOT(choose()));
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
    msg.path = pathText->text();
    msg.plotnum = plotnumText->text();
    msg.minRadius = minRadiusText->text();
    msg.maxRadius = maxRadiusText->text();

    if(!isDigit(msg.zmin) || !isDigit(msg.zmax) || !isDigit(msg.detection1) ||
            !isDigit(msg.detection2) || !isDigit(msg.detection3) ||
            !isDigit(msg.dpix) || !isDigit(msg.lamda) || !isDigit(msg.interval)||
            !isDigit(msg.plotnum)) {
        QMessageBox::critical(NULL, "Error", QStringLiteral("输入格式错误"),
                              QMessageBox::Yes);
        return;
    }
    if(msg.zmin=="" || msg.zmax=="" ||msg.detection1=="" ||
            msg.detection2=="" || msg.detection3=="" ||
            msg.dpix=="" || msg.lamda=="" || msg.interval==""||
            msg.plotnum==""||msg.path=="") {
        QMessageBox::critical(NULL, "Error", QStringLiteral("请填写完整!"),
                              QMessageBox::Yes);
        return;
    }
    if(msg.zmin.toDouble()>=msg.zmax.toDouble()) {
        QMessageBox::critical(NULL, "Error", QStringLiteral("重建终点应该大于重建起点!"),
                              QMessageBox::Yes);
        return;
    }
    emit(updateAttr(nameText->text(),msg));
    std::ofstream out("data.txt",std::ios::app);
    out << nameText->text().toStdString() << " ";
    out << zminText->text().toStdString() << " ";
    out << zmaxText->text().toStdString() << " ";
    out << intervalText->text().toStdString() << " ";
    out << lamdaText->text().toStdString() << " ";
    out << dpixText->text().toStdString() << " ";
    out << d1Text->text().toStdString() << " ";
    out << d2Text->text().toStdString() << " ";
    out << d3Text->text().toStdString() << " ";
    out << pathText->text().toStdString() << " ";

    out << minRadiusText->text().toStdString() << " ";
    out << maxRadiusText->text().toStdString() << " ";
    out << plotnumText->text().toStdString() << " ";

    out<<"\n";
    out.close();

    std::ofstream nameout("name.txt",std::ios::app);
    nameout << nameText->text().toStdString() << " ";

    nameout.close();

    close();
}

void addattrwindow::choose()
{
    QUrl url = QFileDialog::getExistingDirectoryUrl
            (this);
    QString path = url.toString().mid(8);
    if(path!="")pathText->setText(path);

}
