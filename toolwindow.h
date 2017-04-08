#ifndef TOOLWINDOW_H
#define TOOLWINDOW_H
#include <QWidget>
#include "button.h"
#include "holdbutton.h"
#include "attr.h"
#include "slider.h"
class QRadioButton;
class QButtonGroup;
class QVBoxLayout;
class QPushButton;
class QGridLayout;
class toolwindow : public QWidget
{
    Q_OBJECT
private:
    float SIZEW;
    float SIZEH;

    QVBoxLayout* vlayout;
    QGridLayout* glayout;
    void layout();
public:
    holdButton* inverseBtn;
    holdButton* scaleBtn;
    holdButton* calculateBtn;
    Button* zoomInBtn;
    Button* zoomOutBtn;
    Button* originBtn;
    Button* fullScreenBtn;
    slider* slid;
    toolwindow(QWidget* = 0);
    void paintEvent(QPaintEvent *);
    void set(double zmin,double interval);
};

#endif // TOOLWINDOW_H
