#include "holdbutton.h"
#include <QPixmap>
#include <qDebug>

imageButton::imageButton() { }

void imageButton::set(QString text,QString path)
{
   // QPixmap pixmap(path);
    //setIcon(pixmap);
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
        setStyleSheet( "background-color: qconicalgradient(cx:0.5, cy:0.522909, angle:179.9, stop:0.494318 rgba(181, 225, 250, 255), stop:0.5 rgba(222, 242, 251, 255));"
                "border-radius:5px;"
                "border: 1px solid #3C80B1;"  );
    }
    else{
        setStyleSheet("background-color: qconicalgradient(cx:0.5, cy:0.522909, angle:179.9, stop:0.494318 rgba(214, 214, 214, 255), stop:0.5 rgba(236, 236, 236, 255));"
                "border: 1px solid rgb(124, 124, 124);"
                "border-radius:5px;");
    }
}
//qvariant

void holdButton::mousePressEvent(QMouseEvent*)
{
    if(isDown==false){
        isDown = true;
        setStyleSheet( "background-color: qconicalgradient(cx:0.5, cy:0.522909, angle:179.9, stop:0.494318 rgba(181, 225, 250, 255), stop:0.5 rgba(222, 242, 251, 255));"
                       "border-radius:5px;"
                       "border: 1px solid #3C80B1;" );
    }
    else{
        isDown = false;
        setStyleSheet("background-color: qconicalgradient(cx:0.5, cy:0.522909, angle:179.9, stop:0.494318 rgba(214, 214, 214, 255), stop:0.5 rgba(236, 236, 236, 255));"
                      "border: 1px solid rgb(124, 124, 124);"
                      "border-radius:5px;");
    }
    emit(clicked());
}

void holdButton::set(QString text,QString path)
{
    QPixmap pixmap(path);
    setStyleSheet("background-color: qconicalgradient(cx:0.5, cy:0.522909, angle:179.9, stop:0.494318 rgba(214, 214, 214, 255), stop:0.5 rgba(236, 236, 236, 255));"
                  "border: 1px solid rgb(124, 124, 124);"
                  "border-radius:5px;");
   // setIcon(pixmap);
   // setIconSize(QSize(20,20));
    setFixedSize(130, 40);
    setText(text);
   // setFlat(true);
}
