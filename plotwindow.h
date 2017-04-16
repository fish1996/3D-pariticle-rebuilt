#ifndef PLOTWINDOW_H
#define PLOTWINDOW_H
#include <QWidget>
#include <QVector>
#include "attr.h"
#include "position.h"
#include "basewindow.h"
class QMenu;
class QAction;

class plotwindow : public basewindow
{
    Q_OBJECT
private:
    int SIZEW;
    int SIZEH;
    int XMIN;
    int XMAX;
    int YMIN;
    int YMAX;
    int x;
    int num;

    void layout();
    QVector<QColor> colorVector;
    class Rect{
    public:
        int beginx;
        int beginy;
        int sizex;
        int sizey;
        Rect();
        Rect(int bx,int by,int sx,int sy);
        bool isOn(int x,int y);
    };

    bool isFirstDraw;
    int currentRect;
    QMenu* menu;
    QPixmap* pixmap;
    QAction* saveAsImageAction;
    QAction* saveAsExcelAction;
    Rect* rectVector;
    QVector<int> radius;
    QString** table;
    int row;
    int col;
    int imgnum;
    int index;
    int* pointnum;
    double* min;
    double* max;
    double** diameterfre;
    double* diametermin;
    double* diametermax;
    position** pos;
    int* ipointnum;
    double** idiameterfre;
    double* idiametermin;
    double* idiametermax;
    position** ipos;
    bool isInverse;
public:
    void setRadius(double* _min,double* _max);
    void setInverse(bool);
    void saveImg(QString name,int,bool);
    void saveExcel(QString name,int,bool);
    void setIndex(int);
    bool isDraw;
    void setPlotNum(int n);
    void setAttr(double** diameterfre,double** idiameterfre,double* diametermin,double* idiametermin,
                 double* diametermax,double* idiametermax,int,position**,position**,int*,int*);
    void loadTable(int row,int col);
    plotwindow(basewindow* parent = 0);
protected:
    void paintEvent(QPaintEvent* event);
    void mousePressEvent(QMouseEvent *event);
    void contextMenuEvent(QContextMenuEvent* event);
public slots:
    void saveAsImage();
    void saveAsExcel();
};

#endif // PLOTWINDOW_H
