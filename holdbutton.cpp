#include "holdbutton.h"
#include <QPixmap>
#include <qDebug>

imageButton::imageButton() { }

void imageButton::set(QString text,QString path)
{
    QPixmap pixmap(path);
    setIcon(pixmap);
    setText(text);
    setFlat(true);
}

holdButton::holdButton()
{
    isDown = false;
}

void holdButton::setClicked(bool is)
{
    isDown = is;
    if(isDown){
        setStyleSheet("border-radius:0.2em;"
                      "color:rgb(255,255,255);"
                      "border:none;"
                      "background-color:rgb(100,100,100);");
    }
    else{
        setStyleSheet("border-radius:0.2em;"
                      "color:rgb(20,20,20);"
                      "border:none;"
                      "background-color:rgb(220,220,220);");
    }
}
//qvariant

void holdButton::mousePressEvent(QMouseEvent*)
{
    if(isDown==false){
        isDown = true;
        setStyleSheet("border-radius:0.2em;"
                      "color:rgb(255,255,255);"
                      "border:none;"
                      "background-color:rgb(100,100,100);");
    }
    else{
        isDown = false;
        setStyleSheet("border-radius:0.2em;"
                      "color:rgb(20,20,20);"
                      "border:none;"
                      "background-color:rgb(220,220,220);");
    }
    emit(clicked());
}

void holdButton::set(QString text,QString path)
{
    QPixmap pixmap(path);
    setStyleSheet("border-radius:0.2em;"
                  "color:rgb(20,20,20);"
                  "border:none;"
                  "background-color:rgb(220,220,220);");
    setIcon(pixmap);
    setIconSize(pixmap.size());
    setFixedSize(130, 30);
    setText(text);
   // setFlat(true);
}
