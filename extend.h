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


using namespace std;
using namespace cv;

#pragma once
enum ConvolutionType {
    /* Return the full convolution, including border */
    CONVOLUTION_FULL,

    /* Return only the part that corresponds to the original image */
    CONVOLUTION_SAME,

    /* Return only the submatrix containing elements that were not influenced by the border */
    CONVOLUTION_VALID
};

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
    /*void dwt2(Mat &i,Mat &ll,Mat &hh,Mat &dd,Mat &vv); //调用matlab dll实现sym4小波变换
    void idwt2(Mat &idst,Mat &llf,Mat &hhf,Mat &ddf,Mat &vvf); //调用matlab dll实现sym4小波逆向变换
    mwArray mat2mwarray(const Mat imgsrc); //opencv的mat类型转化为matlab的mwarray类型
    Mat mwarray2mat(const mwArray data); //matlab的mwarray类型转化为opencv的mat类型 */
    void dwt(Mat &i,Mat &a,Mat &h,Mat &v,Mat &d);//sym4离散小波变化
    Mat convdown(Mat x,Mat F,int first[],int last[],int sizeEXT);
    Mat conv2(Mat img, Mat ikernel, ConvolutionType type);//full,same,valid三类型的matlab conv2函数
    Mat wextendrow(int sizeEXT,Mat x);
    Mat wextendcol(int sizeEXT,Mat x);
    void idwt(Mat &i,Mat &a,Mat &h,Mat &v,Mat &d); //离散小波逆变换
    Mat upsconv(Mat &z,Mat f1,Mat f2,int sx[]);//逆卷积
    Mat wkeep2(Mat x,int siz[]);
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
