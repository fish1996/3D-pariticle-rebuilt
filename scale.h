#ifndef SCALE_H
#define SCALE_H

#include <QWidget>
#include "attr.h"

class scale : public QWidget
{
    Q_OBJECT
private:
    float length;
    bool isShow;
    int id;
    double wavelength;
  //  float SIZEW = 100*rebuilt::scale;
 //   float SIZEH = 50*rebuilt::scale;
public:
    bool isWhite;
    void isShowWhite(bool is);
    void setWavelength(double w);
    scale(QWidget* parent = 0);
protected:
    void paintEvent(QPaintEvent* event);
public slots:
    void updateScale(float l);
};

#endif // SCALE_H
