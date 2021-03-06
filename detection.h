//图片均保存为img_binaryzation0.jpg类似格式，一次生成一张图
#ifndef DETECTION_H
#define DETECTION_H
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
#include <QObject>
#include "engine.h"
using namespace std;
using namespace cv;


#define     NO_OBJECT       0
#define     MIN(x, y)       (((x) < (y)) ? (x) : (y))
#define     ELEM(img, r, c) (CV_IMAGE_ELEM(img, unsigned char, r, c))
#define     ONETWO(L, r, c, col) (L[(r) * (col) + c])

class detection : public QObject
{
    Q_OBJECT
public:
    detection();
    virtual ~detection();
    void adaptivethreshold(string tempfilename,double blocksize,double globalgraythreshhold,double globalgraythreshold2,int imgnum); //另外seg保存所有数据

private:
    int find( int set[], int x );
    int bwlabel(Mat &img, Mat &L, int n, int* labels); //得到图像矩阵的连通区域
    void dothreshold(Mat &inputimg,Mat &outputimg,double thresh, double maxval); //二值化，超过thresh，取maximal，反之为0
    void extreme(Mat &imageout, double &maxv, double &minv); //取矩阵极值
    void bwmorph(Mat &inputimg,Mat &outputimg); //去除图像亮度孤立点，即将被0包围的1均变为0
    void fillHole(const Mat srcBw, Mat &dstBw); //填空函数imfill，把0填为1
    mxArray* mat2mxarray(Mat imgsrc); //opencv的mat类型转化为matlab的mxarray类型
    Mat mxarray2mat(mxArray *data);

public:
    Mat *p1xy;
    Mat *p1area;
    Mat *p1box;
signals:
    void detectOk();
/*private:
    string tempfilename;
    Mat imgtotal;
    double blocksize;
    double globalgraythreshhold;
    double globalgraythreshold2;
    Mat L1;*/

};

#endif // DETECTION_H
