#include "menu.h"
#include <QMenu>
#include <QMenuBar>
#include <qDebug>
#include <QAction>
#include <QToolBar>
#include <QVBoxLayout>

menu::menu(QWidget *parent)
    : QWidget(parent)
{
    layout();
}

void menu::layout()
{
    vlayout = new QVBoxLayout();
    menuBar = new QMenuBar();

    fileMenu = new QMenu(QStringLiteral("文件"));
    editMenu = new QMenu(QStringLiteral("设置"));

    menuBar->addMenu(fileMenu);
    menuBar->addMenu(editMenu);

    setAction = new QAction(this);
    saveAction = new QAction(this);
    importAction = new QAction(this);
    clearAction = new QAction(this);
    importBackgroundAction = new QAction(this);

    setAction->setText(QStringLiteral("&设置"));
   // saveAction->setText(QStringLiteral("&保存"));
    importAction->setText(QStringLiteral("&导入原图"));
    importBackgroundAction->setText(QStringLiteral("&导入背景"));
   // clearAction->setText(QStringLiteral("&清除缓存"));

    saveAction = fileMenu->addAction(QIcon(":/image/save.ico"),QStringLiteral("&保存"));
    fileMenu->addAction(importAction);
    fileMenu->addAction(importBackgroundAction);
    editMenu->addAction(setAction);
    clearAction = editMenu->addAction(QIcon(":/image/editclear.ico"),QStringLiteral("&清除缓存"));

    saveAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_S));
    setAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_K));
    importAction->setShortcut(QKeySequence(Qt::CTRL + Qt:: Key_I));
    importBackgroundAction->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt:: Key_I));

    toolBar = new QToolBar(tr("&文件"));

    toolBar->addSeparator();
    toolBar->addAction(saveAction);
    toolBar->addAction(clearAction);

    toolBar->setStyleSheet("background-color:rgb(235,235,235);");


    vlayout->addWidget(menuBar);
    vlayout->addWidget(toolBar);
    vlayout->setSpacing(0);
    vlayout->setMargin(0);

    setLayout(vlayout);
}
