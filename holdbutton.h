#ifndef HOLDBUTTON_H
#define HOLDBUTTON_H

#include <QPushButton>

class imageButton :public QPushButton
{
    Q_OBJECT
public:
    imageButton();
    void set(QString text,QString path);
};

class holdButton : public QPushButton
{
    Q_OBJECT
public:
    bool isDown;
    holdButton();
    void set(QString text,QString path);
    void setClicked(bool);
protected:
    void mousePressEvent(QMouseEvent *event);
};

#endif // HOLDBUTTON_H
