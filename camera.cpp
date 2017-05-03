
#include "camera.h"

#include <QMessageBox>
#include <QLabel>
#include <qDebug>
#include <QHBoxLayout>
#include <QTimer>
#include <QPainter>
#include <QWaitCondition>


#define nullptr 0
void test();
using namespace cv;


void camera::updatePrefix(QString str)
{
    prefix = str;
}

void camera::shot()
{
    if(!isOpen)return;
    saveImage();
    flag = true;
    emit(changePath(path));
}

void camera::saveImage()
{
    pixmap->save(path + "photo." + prefix);
}

void camera::setPath(QString p)
{
    path = p;
}

void camera::multishot()
{
    if(!isOpen)return;
    multiflag = true;
    timer->stop();

    capture.set(CV_CAP_PROP_EXPOSURE,time);
    capture.set(CV_CAP_PROP_FPS,fps);

    //20fps = 每秒20次，1次1/20秒，即1/20*1000 = 50ms
    timer->start(1000/fps);

}

void camera::stopshot()
{
    count1 = 0;
    multiflag = false;
    timer->stop();
    timer->start(300);
}

void camera::setTime(int num)
{
    time = num;
}

camera::camera(QWidget* parent) :QWidget(parent)
{
    flag = false;
    isOpen = false;
    multiflag = false;
    fps = 1;
    time = 20;
    prefix = "jpg";
    index = 0;
    timer = new QTimer();
    pixmap = nullptr;
    count = 1;
    count1 = 0;
    total = 2;
    updateDeviceNum();
    connect(timer,SIGNAL(timeout()),this,SLOT(changeState()));
}

void camera::updateDeviceNum()
{
    index = 0;
    deviceNum = 0;
    while(1) {
        if(!capture.open(deviceNum)) {
            break;
        }
        deviceNum++;
        capture.release();
    }
}

void camera::changeState()
{
    if(isOpen) {
        Mat image;
        if(multiflag){
            QString str = path + QString::number(count1) + "." + prefix;
            count1++;
            capture >> image;
            if(image.empty() || !image.data) {
                timer->stop();
                return;
            }

            setImage(cvMat2QImage(image));
            update();
            pixmap->save(str);
        }
        else if(flag){

            Mat tmp(image.rows, image.cols, CV_8UC3);
            for(int i=0;i<image.rows;i++){
                for(int j=0;j<image.cols;j++){
                    tmp.at<Vec3b>(i,j)[2] = (uchar)(1.0*count/total*image.at<Vec3b>(i,j)[2]);
                    tmp.at<Vec3b>(i,j)[1] = (uchar)(1.0*count/total*image.at<Vec3b>(i,j)[1]);
                    tmp.at<Vec3b>(i,j)[0] = (uchar)(1.0*count/total*image.at<Vec3b>(i,j)[0]);
                }
            }
            count++;
            if(count==total){
                count = 1;
                flag = false;
            }
            setImage(cvMat2QImage(tmp));
            update();
        }
        else {
            capture >> image;
            if(image.empty()) {
                timer->stop();
            }
            setImage(cvMat2QImage(image));
            update();
        }
    }
}

void camera::setFPS(int num)
{
    fps = num;
    capture.set(CV_CAP_PROP_FPS,num);
}

void camera::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    if(pixmap!=nullptr) {
        painter.drawPixmap(rect(),*pixmap);
    }
    if(multiflag) {
        painter.setPen(QColor(240,40,40));
        painter.setBrush(QColor(240,40,40));

        painter.drawEllipse(10,10,10,10);
    }
}

void camera::close()
{
   // capture.
    if(!isOpen)return;
    isOpen = false;
    pixmap = nullptr;
    capture.release();
    timer->stop();
}

bool camera::open()
{
    isOpen = true;

 //   fps = capture.get(CV_CAP_PROP_FPS);
    timer->start(300);
    return capture.open(index);
}

void camera::setImage(QImage img)
{
    if(pixmap){
        delete pixmap;
    }
    pixmap = new QPixmap(QPixmap::fromImage(img));
}

QImage camera::cvMat2QImage(const cv::Mat& mat)
{
    if(mat.type() == CV_8UC1) {
        QImage image(mat.cols, mat.rows, QImage::Format_Indexed8);

        image.setColorCount(256);
        for(int i = 0; i < 256; i++) {
            image.setColor(i, qRgb(i, i, i));
        }

        uchar *pSrc = mat.data;
        for(int row = 0; row < mat.rows; row ++) {
            uchar *pDest = image.scanLine(row);
            memcpy(pDest, pSrc, mat.cols);
            pSrc += mat.step;
        }
        return image;
    }
    else if(mat.type() == CV_8UC3) {
        const uchar *pSrc = (const uchar*)mat.data;
        QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
        return image.rgbSwapped();
    }
    else if(mat.type() == CV_8UC4) {
        const uchar *pSrc = (const uchar*)mat.data;
        QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_ARGB32);
        return image.copy();
    }
    else {
        return QImage();
    }
}
