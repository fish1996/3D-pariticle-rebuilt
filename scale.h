#ifndef SCALE_H
#define SCALE_H

#include <QWidget>
#include "attr.h"
#include "message.h"
class scale : public QWidget
{
    Q_OBJECT
private:
    float length;
    int cur_x;
    int cur_y;
    bool isShow;
    int id;
    double wavelength;
public:
    int* size;
    bool isWhite;
    void isShowWhite(bool is);
    void setWavelength(double w);
    void moveLeft();
    void moveRight();
    void moveFront();
    void moveBack();
    void Move(int x,int y);
    int getCurX();
    int getCurY();
    scale(QWidget* parent = 0);
protected:
    void paintEvent(QPaintEvent* event);
public slots:
    void updateScale(float l);
};

#endif // SCALE_H
