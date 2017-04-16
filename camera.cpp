
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
    saveImage();
    flag = true;
    emit(changePath(path));
}

void camera::saveImage()
{
    pixmap->save(path + "1." + prefix);
}

void camera::setPath(QString p)
{
    path = p;
}

void camera::multishot()
{
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
    multiflag = false;
    fps = 1;
    time = 20;
    prefix = "jpg";
    timer = new QTimer();
    pixmap = nullptr;
    count = 1;
    count1 = 0;
    total = 2;
    connect(timer,SIGNAL(timeout()),this,SLOT(changeState()));
 //   open();

}

void camera::changeState()
{
    if(capture.isOpened()) {
        if(multiflag){
            QString str = path + QString::number(count1) + "." + prefix;
            count1++;
            capture >> image;
            if(image.empty()) {
                timer->stop();
            }
            imwrite(str.toStdString(),image);
            int index1[] = {0,1,2,3,4,5,6,7,
                            image.rows-1,image.rows-2,image.rows-3,image.rows-4,
                           image.rows-5,image.rows-6,image.rows-7,image.rows-8};
            int index2[] = {0,1,2,3,4,5,6,7,
                            image.cols-1,image.cols-2,image.cols-3,image.cols-4,
                           image.cols-5,image.cols-6,image.cols-7,image.cols-8};
            for(int i=0;i<image.rows;i++){
                for(int j=0;j<16;j++){
                    image.at<Vec3b>(i,index2[j])[0] = 30;
                    image.at<Vec3b>(i,index2[j])[1] = 30;
                    image.at<Vec3b>(i,index2[j])[2] = 230;
                }
            }
            for(int i=0;i<image.cols;i++){
                for(int j=0;j<16;j++){
                    image.at<Vec3b>(index1[j],i)[0] = 30;
                    image.at<Vec3b>(index1[j],i)[1] = 30;
                    image.at<Vec3b>(index1[j],i)[2] = 230;
                }
            }

            setImage(cvMat2QImage(image));
            update();
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
}

void camera::close()
{
    capture.release();
    timer->stop();
}

void camera::open()
{
    capture.open(0);
 //   fps = capture.get(CV_CAP_PROP_FPS);
    timer->start(300);
}

void camera::setImage(QImage img)
{
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
