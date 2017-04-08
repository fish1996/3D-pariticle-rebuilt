#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "menu.h"
#include "showwindow.h"
#include "setupwindow.h"
#include "viewwindow.h"
#include "toolwindow.h"
#include "initwindow.h"
#include "setupdialog.h"
#include "premessage.h"
#include "toolline.h"
#include "catalog.h"
#include "attr.h"
//#include "engine.h"
#include "message.h"

#include <QDir>
#include <QWidget>
#include <QVector>

class QVBoxLayout;
class QHBoxLayout;

class mainwindow : public QWidget
{
    Q_OBJECT
private:
    enum{
        HMAX = 2,
        VMAX = 2,
        INF = 100000,
        CTRL = 16777249,
        SHIFT = 16777248,
    };

    QString preAttr;
    QString currentpath;
    QDir dir;

    menu* menuWindow;
    bool* isInverse;

    QString backgroundname;
    QString currentPath;

    map_t map;

    QVector<QString>namelist;

    QHBoxLayout* hlayout[HMAX];
    QVBoxLayout* vlayout[VMAX];
    QImage image;
    QString cpath;

    initwindow* initWindow;
    setupdialog* setupDialog;
    catalog* cata;
    toolLine* toolline;
    void layout();
    void setConnect();
    void loadAttr();
    bool copyFile(QString sourceDir ,QString toDir, bool coverFileIfExist);
    /*

public:
    Engine* ep;
*/
public:
    QString name;
    message_t msg;
    int total;
    int totalnum;
    int count;
    int imgnum;
    int index;
    toolwindow* toolWindow;
    setupwindow* setupWindow;
    showtabwindow* showWindow;
    viewwindow* viewWindow;
    mainwindow(QWidget *parent = 0);

private slots:
    void updatePath();
    void zoomIn();
    void zoomOut();
    void original();
    void fullScreen();
    void setIndex(int);
    void setup();
    void initOk();
    void initCancel();
    void openInitWindow();
    void importImg();
    void importBackgroundImg();
    void setupOk();
    void setupCancel();
    void updateAttr(QString name,premessage_t msg);
    void nextImg();
    void lastImg();
    void setInverse();
    void Scale();
    void calculate();
    void setLastImg();
    void setNextImg();
    void setLastPage();
    void setNextPage();
    void chooseCatalog();
    void setPath(QString);
    void updatePath(QString);
    void chooseBackground();
    void clearBuffer();
    void saveImg();
public:
    void saveAll(QString,bool);

protected:
    void closeEvent(QCloseEvent*);
    void keyReleaseEvent(QKeyEvent* );
    void keyPressEvent(QKeyEvent*);
};

#endif // MAINWINDOW_H
