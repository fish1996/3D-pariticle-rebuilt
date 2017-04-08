#include "savewindow.h"
#include <QPushButton>
#include <QComboBox>
#include <QCheckBox>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>

savewindow::savewindow(QWidget* parent)
    :QWidget(parent)
{
    for(int i = 0;i < HMAX;i++) {
        hlayout[i] = new QHBoxLayout();
    }
    for(int i = 0;i < VMAX;i++) {
        vlayout[i] = new QVBoxLayout();
    }
    label[0].setText(QStringLiteral("文件名"));
    label[1].setText(QStringLiteral("保存路径"));
    label[2].setText(QStringLiteral("类型"));
    okBtn = new QPushButton(QStringLiteral("保存"));
    cancelBtn = new QPushButton(QStringLiteral("取消"));
    chooseBtn = new QPushButton(".");
    pathLine = new QLineEdit();

   // prefix = new QCombo
}


