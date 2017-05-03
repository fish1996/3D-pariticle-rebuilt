#include "removeattrwindow.h"
#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <fstream>
#include <qDebug>

removeattrwindow::removeattrwindow(map_t* m,QVector<QString>* n,QWidget* parent)
    :QWidget(parent)
{
    map = m;
    namelist = n;
    layout();
}

void removeattrwindow::layout()
{

    okBtn = new QPushButton(QStringLiteral("删除"));
    cancelBtn = new QPushButton(QStringLiteral("退出"));
    tips = new QLabel(QStringLiteral("请选择需删除的预设项目："));
    attrBox = new QComboBox();
    for(int i=0;i<HMAX;i++){
        hlayout[i] = new QHBoxLayout();
    }
    for(int i=0;i<VMAX;i++){
        vlayout[i] = new QVBoxLayout();
    }
    for(int i=0;i<namelist->size();i++){
        attrBox->addItem((*namelist)[i],i);
    }

    hlayout[0]->addStretch();
    hlayout[0]->addWidget(okBtn);
    hlayout[0]->addWidget(cancelBtn);

    vlayout[0]->addWidget(tips);
    vlayout[0]->addWidget(attrBox);
    vlayout[0]->addLayout(hlayout[0]);

  //  setWindowFlags(Qt::WindowStaysOnTopHint);
    setWindowModality(Qt::ApplicationModal);
    setWindowTitle(QStringLiteral("删除预设参数"));

    setLayout(vlayout[0]);
    okBtn->setEnabled(true);
    cancelBtn->setEnabled(true);
    attrBox->setEnabled(true);

    connect(okBtn,SIGNAL(clicked()),this,SLOT(removeAttr()));
    connect(cancelBtn,SIGNAL(clicked()),this,SLOT(cancelRemoveAttr()));
}


void removeattrwindow::cancelRemoveAttr()
{
    close();
}

void removeattrwindow::removeAttr()
{
    //qDebug()<<"removeAttr";
    QString name = attrBox->currentText();

    for(int i=0;i<namelist->size();i++){
        if((*namelist)[i] == name){
            namelist->remove(i);
            break;
        }
    }
    std::ofstream nameout("name.txt",std::ios::trunc);
    for(int i=0;i<namelist->size();i++){
        nameout << (*namelist)[i].toStdString() << " ";
    }

    nameout.close();
    QMap<QString,premessage_t>::iterator it;
    it = map->find(name);
    if(it!=map->end()){
        //qDebug()<<"erase";
        map->erase(it);
    }

    std::ofstream out("data.txt",std::ios::trunc);

    for(int i=0;i<namelist->size();i++){
        out << (*namelist)[i].toStdString() << " "
            << (*map)[(*namelist)[i]].zmin.toStdString() << " "
            << (*map)[(*namelist)[i]].zmax.toStdString() << " "
            << (*map)[(*namelist)[i]].interval.toStdString() << " "
            << (*map)[(*namelist)[i]].lamda.toStdString() << " "
           // << (*map)[(*namelist)[i]].Mag.toStdString() << " "
            << (*map)[(*namelist)[i]].dpix.toStdString() << " "
            << (*map)[(*namelist)[i]].detection1.toStdString() << " "
            << (*map)[(*namelist)[i]].detection2.toStdString() << " "
            << (*map)[(*namelist)[i]].detection3.toStdString() << " "
           // << (*map)[(*namelist)[i]].location.toStdString() << " "
            << (*map)[(*namelist)[i]].path.toStdString() << " "
            << (*map)[(*namelist)[i]].minRadius.toStdString() << " "
            << (*map)[(*namelist)[i]].maxRadius.toStdString() << " "
            << (*map)[(*namelist)[i]].plotnum.toStdString() << " ";

        out << "\n";
    }

    emit(updateStyle());
    close();
}
