#ifndef MENU_H
#define MENU_H

#include<QWidget>
class QMenu;
class QMenuBar;
class QVBoxLayout;
class QToolBar;
class menu: public QWidget{
        Q_OBJECT
private:
    QMenuBar* menuBar;
    QToolBar* toolBar;
    QMenu* fileMenu;//文件菜单
    QMenu* editMenu;//设置菜单
    QVBoxLayout* vlayout;
    void layout();

public:
    menu(QWidget *parent = 0);
    QAction* importAction;
    QAction* startAction;
    QAction* changeAction1;
    QAction* changeAction2;
    QAction* importBackgroundAction;
    QAction* saveAction;
    QAction* setAction;
    QAction* clearAction;

};

#endif // MENU_H
