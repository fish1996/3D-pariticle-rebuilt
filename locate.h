//实际用到的diameterfre,diametermin，diametermax,pos
//调用方式diameterfre[0][0]前一个0表示一次性实现的图像张数例如3张，后一个0表示第1个区间的频率
//直接diametermin[0],diametermax[0],第一张图对应的最大最小值横坐标
//pos[0][0].X,第一张图对应的第一个球的X值，另有Y,Z,D,D表示的应该是球的直径，且每张图对应的球个数不同
#ifndef LOCATION_H
#define LOCATION_H
#include "position.h"
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/contrib/contrib.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include <cmath>
#include <cvaux.h>
#include <cstring>
#include <iostream>
#include <fstream>
#include <sstream>
#include <QObject>
using namespace std;
using namespace cv;
#define PI 3.1415926

class location : public QObject
{
    Q_OBJECT
public:
    location();
    virtual ~location();
    void set(Mat *p1xy0,Mat *p1area0,Mat *p1box0,Mat *ip1xy0,Mat *ip1area0,Mat *ip1box0);
    void p_location(string tempfilename,double boxcoef,int planesumnuber,int *secnum,int imgnum,double *sectionmin,double *sectionmax);
    void re_location();

private:
    int round(float x);
    void extreme(Mat &imageout, double &maxv, double &minv);
    Mat fft2(Mat src);
    Mat ifft2(Mat src);
    Mat mergeRows(Mat A, Mat B);
    Mat mergeCols(Mat A, Mat B);
    void maxrow(Mat &imgsrc,Mat &imgmax,Mat &imgorder);
    Mat SQRTs(const Mat &a);
    void DeleteRowofMat(Mat& object,vector<int> rownum);
    double getvariance(Mat object);

public:
    Mat *p1xy;
    Mat *p1area;
    Mat *p1box;
    Mat *p1xyzd;
    Mat *ip1xy;
    Mat *ip1area;
    Mat *ip1box;
    Mat *ip1xyzd;
    double **diameterfre;
    double *diametermin;
    double *diametermax;
    double **idiameterfre;
    double *idiametermin;
    double *idiametermax;
    struct position **ipos;
    struct position **pos;
    int *pointnum;
    int *ipointnum;
    bool flag;
    bool iflag;
    double *meandiameter;
    double *imeandiameter;
    int *_secnum;
    double *_sectionmin;
    double *_sectionmax;
    int imgnumber;

signals:
    void locateOk();
    void locateAll();
    void locateAll(position**,position**,int,int,double**,double**,double*,double*,double*,double*,int*,int*,bool,bool,double*,double*);
/*private:
    string tempfilename;
    double boxcoef;
    int planesumnuber;*/
};

#endif // LOCATION_H
