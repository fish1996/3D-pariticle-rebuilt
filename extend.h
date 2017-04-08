
#ifndef EXTEND_H
#define EXTEND_H
#include <QObject>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/contrib/contrib.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include <cmath>
#include <cvaux.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include "mclmcr.h"
#include "dwt22.h"
#include "idwt22.h"
#include "matrix.h"
#include "mclcppclass.h"

using namespace std;
using namespace cv;

class extend : public QObject
{
   Q_OBJECT
public:
    void start();
    extend(int size1,int num1,string tempfilename1, double zmin1, double zmax1, double interval);
    virtual ~extend();
    void image_extend(int num,int size,double zmin, double zmax, double precision);
    void iimage_extend(int num,int size,double zmin, double zmax, double precision);

private:
    int size;
    int num;
    Mat POW2(const Mat& a);
    Mat SQRT(const Mat& b);
    Mat stdfilt(Mat_<double> const& I, InputArray kernel, int borderType);
    void dwt2(Mat &i,Mat &ll,Mat &hh,Mat &dd,Mat &vv); //调用matlab dll实现sym4小波变换
    void idwt2(Mat &idst,Mat &llf,Mat &hhf,Mat &ddf,Mat &vvf); //调用matlab dll实现sym4小波逆向变换
    mwArray mat2mwarray(const Mat imgsrc); //opencv的mat类型转化为matlab的mwarray类型
    Mat mwarray2mat(const mwArray data); //matlab的mwarray类型转化为opencv的mat类型
    void accurate4(Mat& img); //为配合matlab保留四位小数
    int round(float x);

private:
    string tempfilename;
    double zmin;
    double zmax;
    double precision;

signals:
    void extendOk();
    void extendAll();

};

#endif // EXTEND_H

