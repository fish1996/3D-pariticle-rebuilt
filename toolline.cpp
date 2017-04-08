#include "toolline.h"

toolLine::toolLine(QGroupBox* parent)
    :QGroupBox(parent)
{
    layout();
}

void toolLine::layout()
{
    setFixedHeight(50);
    setStyleSheet("background-color:white;");
    lastBtn = new imageButton();
    nextBtn = new imageButton();
    lastpairBtn = new imageButton();
    nextpairBtn = new imageButton();
    chooseBackgroundBtn = new imageButton();
    chooseCatalogBtn = new imageButton();
    allchooseBtn = new imageButton();
    clearBtn = new imageButton();

    hlayout = new QHBoxLayout();

    lastBtn->set(QStringLiteral("上一个"),":/image/circlelast.ico");
    nextBtn->set(QStringLiteral("下一个"),":/image/circlenext.ico");
    lastpairBtn->set(QStringLiteral("上一对"),":image/lastpair.ico");
    nextpairBtn->set(QStringLiteral("下一对"),":/image/nextpair.ico");
    chooseCatalogBtn->set(QStringLiteral("目录"),":/image/drop.ico");
    chooseBackgroundBtn->set(QStringLiteral("背景"),":/image/brush.ico");
    clearBtn->set(QStringLiteral("清空"),":/image/clear.ico");
    allchooseBtn->set(QStringLiteral("全选"),":/image/arrow.ico");

    lastBtn->setFixedHeight(30);
    lastBtn->setIconSize(QSize(25,25));
    nextBtn->setFixedHeight(30);
    nextBtn->setIconSize(QSize(25,25));
    lastpairBtn->setFixedHeight(30);
    lastpairBtn->setIconSize(QSize(25,25));
    nextpairBtn->setFixedHeight(30);
    nextpairBtn->setIconSize(QSize(25,25));
    chooseCatalogBtn->setFixedHeight(30);
    chooseCatalogBtn->setIconSize(QSize(25,25));
    chooseBackgroundBtn->setFixedHeight(30);
    chooseBackgroundBtn->setIconSize(QSize(25,25));
    clearBtn->setFixedHeight(30);
    clearBtn->setIconSize(QSize(25,25));
    allchooseBtn->setFixedHeight(30);
    allchooseBtn->setIconSize(QSize(25,25));

    hlayout->addWidget(chooseCatalogBtn);
    hlayout->addWidget(lastBtn);
    hlayout->addWidget(nextBtn);
    hlayout->addWidget(lastpairBtn);
    hlayout->addWidget(nextpairBtn);
    hlayout->addWidget(chooseBackgroundBtn);
    hlayout->addWidget(clearBtn);
    hlayout->addWidget(allchooseBtn);

    setLayout(hlayout);
}
