#include "menu.h"
#include <QMenu>
#include <QMenuBar>
#include <qDebug>
#include <QAction>
#include <QHBoxLayout>

menu::menu(QWidget *parent)
    : QWidget(parent)
{
    layout();
}

void menu::layout()
{
    hlayout = new QHBoxLayout();
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
    saveAction->setText(QStringLiteral("&保存"));
    importAction->setText(QStringLiteral("&导入原图"));
    importBackgroundAction->setText(QStringLiteral("&导入背景"));
    clearAction->setText(QStringLiteral("&清除缓存"));

    fileMenu->addAction(saveAction);
    fileMenu->addAction(importAction);
    fileMenu->addAction(importBackgroundAction);
    editMenu->addAction(setAction);
    editMenu->addAction(clearAction);

  //  openAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_O));
    saveAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_S));
    setAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_K));
  //  exportAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_E));
    importAction->setShortcut(QKeySequence(Qt::CTRL + Qt:: Key_I));
    importBackgroundAction->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt:: Key_I));

    hlayout->addWidget(menuBar);
    hlayout->setMargin(0);

    setLayout(hlayout);
}
