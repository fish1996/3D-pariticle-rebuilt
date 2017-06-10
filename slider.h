#ifndef SLIDER_H
#define SLIDER_H

#include <QWidget>
#include <QLabel>
class QSlider;
class Button;
class QHBoxLayout;
class QVBoxLayout;
class QLineEdit;

class slider : public QWidget
{
    Q_OBJECT
private:

    QHBoxLayout* hlayout;
    QHBoxLayout* hlayout1;
    QVBoxLayout* vlayout;

    QLineEdit* label;

    QLabel* text;
    QLineEdit* location;
    int total;
    int count;
    double interval;
    double zmin;
public:
    Button* lastbtn;
    Button* nextbtn;
    QSlider* slid;
    slider(QWidget* parent = 0);
    void setNum(int,int);
    void setDisable();
    void setEnable();
    void set(double zmin,double interval);
private slots:
    void next();
    void last();
public slots:
    void setNum(int);
};

#endif // SLIDER_H
