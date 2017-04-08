#ifndef MENU_H
#define MENU_H

#include<QWidget>
class QMenu;
class QMenuBar;
class QHBoxLayout;

class menu: public QWidget{
        Q_OBJECT
private:
    QMenuBar* menuBar;
    QMenu* fileMenu;//文件菜单
    QMenu* editMenu;//设置菜单
    QHBoxLayout* hlayout;
    void layout();

public:
    menu(QWidget *parent = 0);
    QAction* importAction;
    QAction* importBackgroundAction;
    QAction* saveAction;
    QAction* setAction;
    QAction* clearAction;

};

#endif // MENU_H
