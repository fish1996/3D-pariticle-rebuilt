#ifndef IMGINVERSE_H
#define IMGINVERSE_H
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/contrib/contrib.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include <QDebug>
#include <cmath>
#include <cvaux.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <QObject>
using namespace std;
using namespace cv;

class imginverse
{
public:
    imginverse();
    virtual ~imginverse();
    void doimginverse(string tempfilename);
};

#endif // IMGINVERSE_H
