#ifndef BASEWINDOW_H
#define BASEWINDOW_H
#include <QWidget>
class scale;
class showwindow;
class QComboBox;
class basewindow : public QWidget
{
    Q_OBJECT
public:
    basewindow(QWidget* parent = 0);
    virtual QComboBox* getBox();
    virtual void setI(bool*);
    virtual void reloadImg();
    virtual void zoomIn();
    virtual void zoomOut();
    virtual void original();
    virtual void fullScreen();
    virtual void setNum(int n);
    virtual scale* getSca();
    virtual float getRatio();
    virtual void setPixelSize(float s);
    virtual showwindow* getshowwindow();
    virtual void setCalculate(bool);
    virtual bool getCalculate();
    virtual void setPath(QString);
    virtual void reset();
    virtual void addItem(int);
    virtual void setIndex(int);
    virtual QString getPath();
    virtual void setTotalNum(int);
};

#endif // BASEWINDOW_H
