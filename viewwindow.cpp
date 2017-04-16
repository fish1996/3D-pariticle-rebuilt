#include "viewwindow.h"
#include <io.h>
#include <QWidget>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QPainter>
#include <QDebug>
#include <string>
#include <iostream>
#include <QMessageBox>
#include <fstream>
#include <QDir>

viewwindow::viewwindow(QWidget* parent)
    :QWidget(parent)
{
    SIZEH = 120;
    SIZEW = 150;
    setMouseTracking(true);
    index = -1;
    beginindex = -1;
    endindex = -1;
    count = 0;
    flag = false;
    showindex = INT_MAX;
    front = 0;
    isPress = false;
    isPressCtrl = false;
    isPressShift = false;
    isIn = false;
    layout();
}

QFileInfoList viewwindow::loadFile(QString path)
{


    QDir dir(path);
    QFileInfoList file_list =  dir.entryInfoList(QStringList() << "*.jpg" << "*.png" << "*.bmp" );
    QFileInfoList folder_list = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
    if(count >= 1) {

        return file_list;
    }

    //message.exec();
    for(int i = 0; i != folder_list.size(); i++) {
        QString name = folder_list.at(i).absoluteFilePath();
        count++;
        QFileInfoList child_file_list = loadFile(name);
        count--;
        file_list.append(child_file_list);
    }

    return file_list;
}

void viewwindow::clear()
{
    QMessageBox message(QMessageBox::NoIcon, "Qustion",
                        QStringLiteral("您是否要取消去背景？"), QMessageBox::Yes | QMessageBox::No, NULL);
    if(message.exec() == QMessageBox::Yes)
    {
        cancelBackground();
    }
}

void viewwindow::reset()
{
     std::set<int>::iterator it;
     for(it = chooseImg.begin();it!=chooseImg.end();it++) {
        //qDebug()<<"reset" << *it;
         isRmBackground[*it] = true;
         QString p = fileList[*it].filePath();
         fileList[*it] = QFileInfo(p.mid(0,p.size() - 4)+"r"+p.mid(p.size()-4));
     }
     update();
}

void viewwindow::setPath(QString p)
{
    isRmBackground.clear();
    count = 0;
    flag = false;
    path = p;
    fileList = loadFile(p);
    for(int i =0;i<fileList.size();i++) {
        isRmBackground.push_back(false);
    }
    update();
}

void viewwindow::mouseReleaseEvent(QMouseEvent*)
{
    isPress = false;
}

void viewwindow::mouseMoveEvent(QMouseEvent* event)
{
    int x = event->pos().x();
    if(x >= SIZEW){
        x = SIZEW-1;
    }
    if(x < 0){
        x = 0;
    }
    int newindex = x/(SIZEW/4) + front;

    if(!isPress && index != newindex) {
        index = newindex;
        update();
    }
    if(isPress && endindex != newindex) {
        endindex = newindex;
        update();
    }
}

void viewwindow::leaveEvent(QEvent* )
{
    isIn = false;
    index = -1;
    update();
}

void viewwindow::updateImg()
{
    isRmBackground.clear();
    count = 0;
    flag = false;
    fileList = loadFile(path);
    for(int i =0;i<fileList.size();i++) {
        isRmBackground.push_back(false);
    }



    update();
}

void viewwindow::updatePath(QString p)
{
    path = p;
}

void viewwindow::mousePressEvent(QMouseEvent* event)
{
    if(event->button() == Qt::LeftButton) {
     //   qDebug() << "leftbutton";
        int x = event->pos().x();
        if(isPressShift) {
            endindex = x/(SIZEW/4) + front;
            update();
            return;
        }
        else if(isPressCtrl) {
            indexVector.push_back(x/(SIZEW/4) + front);
            update();
            return;
        }
        isPress = true;
        indexVector.clear();

        beginindex = endindex = x/(SIZEW/4) + front;
        bool flag = false;
        //qDebug() <<"size = "<<filename.size();
        for(int i = 0;i<filename.size();i++) {
            if(beginindex>=0 && beginindex <fileList.size() &&
                    std::string(fileList.at(beginindex).filePath().toLocal8Bit())
                    == filename[i]) {
                flag = true;
        //        qDebug()<<"choose" <<i;
                emit(choose(i));
                break;
            }
        }
        if(filename.size()!=0 && !flag) {
       //     qDebug()<<"choose -1";
            emit(choose(-1));
        }

       // qDebug()<<"loadimg";
        if(beginindex < fileList.size()) {
            emit(loadImg(fileList[beginindex].filePath()));
        }

        index = -1;
        update();
    }
    else if(event->button() == Qt::RightButton) {
        beginindex = -1;
        endindex = -1;
        index = -1;
        indexVector.clear();
        update();
    }

}

void viewwindow::cancelRm()
{
    for(int i = 0;i < isRmBackground.size();i++) {
        if(isRmBackground[i]) {
            QString p = fileList[i].filePath();
            QFile file(p);
            file.remove();
            fileList[i] = QFileInfo(p.mid(0,p.size() - 5) + p.mid(p.size()-4));
        }
        isRmBackground[i] = false;
    }
    update();
}

void viewwindow::paintEvent(QPaintEvent* e)
{
  //  qDebug() << "isPaint";
    SIZEW = e->rect().width();
    SIZEH = e->rect().height();
    chooseImg.clear();
    int sizew = (SIZEW/4) - 20;
    int sizeh = SIZEH - 20;
    QPainter paint(this);
    paint.setPen(Qt::NoPen);
    paint.setBrush(QColor(80,80,80));

    paint.drawRect(QRect(0,0,SIZEW,SIZEH));

    paint.setBrush(QColor(20,20,20));
    if(index != -1 && index < fileList.size()) {
        paint.drawRect(QRect((index-front)*(SIZEW/4),0,SIZEW/4,SIZEH));
    }

    if(beginindex >= fileList.size() && endindex >= fileList.size());
    else if(beginindex <= endindex) {
        if(endindex >= fileList.size()){
            endindex = fileList.size() - 1;
        }
        for(int i = beginindex;i <= endindex;i++) {
            if(i>=0)chooseImg.insert(i);
        }
        paint.drawRect(QRect((beginindex-front)*(SIZEW/4),0,
                             (SIZEW/4)*((endindex-front) - (beginindex-front) + 1),SIZEH));
    }
    else {
        for(int i = endindex;i <= beginindex;i++) {
            if(i>=0)chooseImg.insert(i);
        }
        paint.drawRect(QRect((endindex-front)*(SIZEW/4),0,
                            (SIZEW/4)*((beginindex-front) - (endindex-front) + 1),SIZEH));
    }
    for(int i=0;i<indexVector.size();i++) {
        if(indexVector[i]>=0)chooseImg.insert(indexVector[i]);
        paint.drawRect(QRect((indexVector[i]-front)*(SIZEW/4),0,SIZEH,SIZEH));
    }
    paint.setBrush(Qt::NoBrush);
    int SIZE = fileList.size() < front + 4 ? fileList.size() : (front + 4);

    for(int i = front;i < SIZE;i++) {
        QPixmap image;
        QString p = fileList.at(i).filePath();

        image.load(p);


        int w = image.width();
        int h = image.height();
        if(1.0*w /h  > 1.0*sizew/sizeh) {
            h = 1.0f*h/w*sizew;
            w = sizew;
        }
        else {
            w = 1.0*w/h*sizeh;
            h = sizeh;
        }
        image = image.scaled(QSize(w,h));
        paint.drawPixmap(10+(SIZEW/4)*(i-front),0,w,h,image);

        paint.setPen(QColor(255,255,255));
        paint.drawText(10+(SIZEW/4)*(i-front),sizeh,fileList.at(i).fileName());
    }
}

void viewwindow::setBackground(QString path)
{
    std::set<int>::iterator it;
    for(it = chooseImg.begin();it != chooseImg.end();it++) {
        QString name = fileList.at(*it).filePath();
    }
    update();
}

std::vector<std::string> viewwindow::getFilename()
{
    filename.clear();
    std::set<int>::iterator it;

    for(it = chooseImg.begin();it != chooseImg.end();it++) {
        QString name = fileList.at(*it).filePath();
        filename.push_back(std::string(name.toLocal8Bit()));
    }
    return filename;
}

void viewwindow::chooseAll()
{
    beginindex = 0;
    endindex = fileList.size() - 1;
    update();
}

void viewwindow::clearImg()
{
    filename.clear();
}

void viewwindow::cancelBackground()
{
    cancelRm();
    update();
}

bool viewwindow::isEmpty()
{
    return chooseImg.empty();
}

void viewwindow::layout()
{
    setMinimumSize(120,150);

}

void viewwindow::release()
{
    isPressCtrl = false;
    isPressShift = false;
}

void viewwindow::pressCtrl()
{
    isPressCtrl = true;
}

void viewwindow::pressShift()
{
    isPressShift = true;
}

void viewwindow::enterEvent(QEvent*)
{
    isIn = true;
}

void viewwindow::setLastImg()
{
    if(beginindex < 0 || endindex < 0)return;
    indexVector.clear();
    if(beginindex <= endindex && beginindex >= 1 ||
            beginindex > endindex && endindex >=1) {
        beginindex--;
        endindex = beginindex;
    }
    if(beginindex < front) {
        front = beginindex - 3;
    }
    emit(loadImg(fileList[beginindex].filePath()));
    bool flag = false;
    for(int i = 0;i < filename.size();i++) {
        if(beginindex>=0 &&
                std::string(fileList.at(beginindex).filePath().toLocal8Bit())
                == filename[i]) {
            flag = true;
            emit(choose(i));
            break;
        }
    }
    if(filename.size()!=0 && !flag) {
        emit(choose(-1));
    }
    update();
}

void viewwindow::setNextImg()
{
    if(beginindex < 0 || endindex < 0) return;
    indexVector.clear();
    if(beginindex <= endindex && endindex < fileList.size() - 1 ||
           beginindex > endindex && beginindex < fileList.size() - 1) {
        endindex++;
        beginindex = endindex;
    }
    if(beginindex >= front + 4) {
        front = beginindex;
    }
    emit(loadImg(fileList[beginindex].filePath()));
    bool flag = false;
    for(int i = 0;i<filename.size();i++) {
        if(beginindex>=0 &&
                std::string(fileList.at(beginindex).filePath().toLocal8Bit())
                == filename[i]) {
            flag = true;
            emit(choose(i));
            break;
        }
    }
    if(filename.size()!=0 && !flag) {

        emit(choose(-1));
    }
    update();
}

void viewwindow::setNextPage()
{
    if(front + 4 < fileList.size()){
        front = front + 4;
    }
    update();
}

void viewwindow::setLastPage()
{
    if(front - 4 >=0){
        front = front - 4;
    }

    update();
}
