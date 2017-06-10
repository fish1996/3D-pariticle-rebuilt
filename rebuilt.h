
#ifndef REBUILT_H
#define REBUILT_H

#include "progressbar.h"
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
using namespace std;
using namespace cv;

class reBuilt :public QObject
{
    Q_OBJECT
public:
    bool closeflag;
    void start();
    const char* errmsg;
    progressbar* mybar;
    vector<int> errorIndex;
    vector<string> name;
    int inum;
    reBuilt(string tempfilename1,vector<string>& name,
            double zmin1, double zmax1, double lamda1,
            double pixelsize, double interval,progressbar* bar);
    virtual ~reBuilt();

    void imagereconstruct2_wavelet(string tempfilename, Mat &image, double zmin, double zmax, double lamda, double pixelh, double pixelv, double precision);
    //vector<Mat> reimg;

private:
    vector<double> graymax;
    vector<double> graymin;
    int renum;
    //float M_PI;
    static void extreme(Mat &imageout, double &maxv, double &minv);
    static void meshgrid(int x1, int x2, int y1, int y2, Mat &X, Mat &Y, double x, double y);
    Mat fft2(Mat src);
    Mat ifft2(Mat src);
    Mat ifftshift(Mat src);
    string tempfilename;
    Mat image;
    double zmin;
    double zmax;
    double lamda;
    double pixelh;
    double pixelv;
    double precision;

signals:
    void finish();
    void ok();
    void allfinish(int);
    void err();
};


#endif // REBUILT_H
