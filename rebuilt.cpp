
#include "rebuilt.h"
#include <math.h>
#include<QDebug>
#include <QMessageBox>
#include <iostream>


reBuilt::reBuilt(string tempfilename1,vector<string>& filename,
                 double zmin1, double zmax1, double lamda1,
                 double pixelsize, double interval,progressbar* bar)
{
  //  qDebug()<<"com here";
    inum=0;
    mybar = bar;
    closeflag = false;
    name = filename;
    tempfilename=tempfilename1+"/temp_plane_";
    zmin=zmin1;
    zmax=zmax1;
    lamda=lamda1;
    pixelh=pixelv=pixelsize;
    precision=interval;
}

void reBuilt::start()
{
 //   int count = 0;
    for(int i = 0;i < name.size();i++){
        Mat image2;
        image2=imread(name[i],CV_LOAD_IMAGE_GRAYSCALE);
        emit(ok());
        image2.convertTo(image,CV_64F,1.0/255.0);
        imagereconstruct2_wavelet(tempfilename, image, zmin, zmax, lamda, pixelh, pixelv, precision);
    }
    vector<double>::iterator biggest = max_element(begin(graymax), end(graymax));
    double graybig=*biggest;
    auto smallest = min_element(begin(graymin), end(graymin));
    double graysmall=*smallest;
    char filenamei[1000];
    char filename[1000];


    for(int i=0;i<renum;i++){
        sprintf(filename,"%s%d%s",tempfilename.c_str(),i,".jpg");
        Mat planez=imread(filename,CV_LOAD_IMAGE_GRAYSCALE);
        planez.convertTo(planez,CV_64F,1.0/255.0);
        planez=planez*(graymax[i]-graymin[i])+graymin[i];
        planez=(graybig-planez)/(graybig-graysmall);

        normalize(planez,planez,0,255,NORM_MINMAX);
        sprintf(filenamei,"%s%d%s",tempfilename.c_str(),i,"i.jpg");
        imwrite(filenamei,planez);
    }

    emit(allfinish(1));
}

reBuilt::~reBuilt()
{

}

//求图像矩阵的最大最小值
void reBuilt::extreme(Mat &imageout, double &maxv, double &minv){
   // qDebug()<<"extreme";
    maxv=minv=0.0; //初始化
    minMaxIdx(imageout,&minv,&maxv);
}

//xgv -- 【输入】指定X输入范围 ygv -- 【输入】指定Y输入范围 X   -- 【输出】Mat Y   -- 【输出】Mat
void reBuilt::meshgrid(int x1, int x2, int y1, int y2, Mat &X, Mat &Y, double x, double y)
{
    vector<double> t_x, t_y;
    for(int i = x1; i <= x2; i++) t_x.push_back(i*x);
    for(int j = y1; j <= y2; j++) t_y.push_back(j*y);

    repeat(Mat(t_x).t(), t_y.size(), 1, X);
    repeat(Mat(t_y), 1, t_x.size(), Y);
}

Mat reBuilt::fft2(Mat src)
{
 //   qDebug()<<"fft2";
    Mat Fourier;
    Mat planes[]={Mat_<double>(src), Mat::zeros(src.size(),CV_64F)}; //定义一个planes数组
    merge(planes, 2, Fourier);
    dft(Fourier, Fourier,0,0);
    return Fourier;
}

Mat reBuilt::ifft2(Mat src)
{
  //  qDebug()<<"ifft2";
     Mat Fourier;
     idft(src, Fourier,0);        //或者写为：dft(src, Fourier, DFT_INVERSE);
     vector<Mat> planes(2);
     split(Fourier, planes);
     //magnitude(planes[0], planes[1], planes[0]); //将复数转化为幅值
     int h=planes[0].rows;
     int w=planes[0].cols;
     planes[0]=planes[0]/(h*w);
     //normalize(planes[0], planes[0], 0,1,NORM_L2);//归一化
     return planes[0];
}

Mat reBuilt::ifftshift(Mat src) //矩阵圆周运动，按负方向（向左和向上）做圆周位移
{
  //  qDebug()<<"ifftshift";
    Mat change=src.clone();
    Mat changerow=change.clone();

    int r=changerow.cols/2; //求得列数的一半
    for(int i=0;i<changerow.rows;i++){
        for(int j=0;j<changerow.cols;j++){
            int k=j-r;
            if(k<0){
                k=k+changerow.cols;
            }
            double t1=change.at<double>(i,j);
            changerow.at<double>(i,k)=t1;
        }
    }
    Mat changecol=changerow.clone();
    int c=changecol.rows/2; //求得行数的一半
    for(int i=0;i<changecol.cols;i++){
        for(int j=0;j<changecol.rows;j++){
            int k=j-c;
            if(k<0){
                k=k+changecol.rows;
            }
            double t2=changerow.at<double>(j,i);
            changecol.at<double>(k,i)=t2;
        }
    }
    return changecol;
}
/*
把不同z处的重建颗粒图像以jpg格式实时保存到硬盘temp目录下
%好处：避免重建平面过多或者处理高分辨率全息图时内存不足
%缺点：计算相对较慢
最后三个参数为求得结果*/
int round(float x)
{
    return (int)(x+0.5);
}

void reBuilt::imagereconstruct2_wavelet(string tempfilename, Mat &image, double zmin, double zmax, double lamda, double pixelh, double pixelv, double precision)//不知道后面那个参数干嘛用, int &all_light)
{
    //qDebug()<<"come here";

    float M_PI = 3.14159f;
    normalize(image, image, 0.0,255.0,NORM_MINMAX);//归一化
    Size size=image.size();
    int Iy=size.height;
    int Ix=size.width;
    int imagescale=Iy<Ix?Iy:Ix;
    double pixelsize=(pixelh+pixelv)/2;
    Mat X,Y;
    int x1=(-round(Ix/2));
    int x2=(Ix-1-round(Ix/2));
    int y1=(-round(Iy/2));
    int y2=(Iy-1-round(Iy/2));
    meshgrid(x1, x2, y1, y2, X, Y, pixelh, pixelv);

    Mat XY2;
    Mat X1,Y1;
    pow(X,2,X1);
    pow(Y,2,Y1);
    XY2=X1+Y1;
    Mat fftimage=fft2(image);

    X.release();
    Y.release();
    X1.release();
    Y1.release();

    //int flag=0;
    //vector<Mat> planeimage;
    char filename[1000];
    /*
    if ((zmax-zmin)/(precision) <50) {
        for(int n=0;n<round((zmax-zmin)/precision);n++){
            Mat a=Mat::zeros(Iy,Ix,CV_64F);
            planeimage.push_back(a);
         }
         flag=1;
    }*/
    for (double z = zmin;z <zmax+precision*0.1;z = z + precision) {
        double sigma1=M_PI/(4*log(100.0))*(lamda*z/(pixelsize*pixelsize)); //check sampling
        double sigma2=0.6822*(imagescale*imagescale)*(pixelsize*pixelsize)/(4*lamda*z);  //check size
        double sigma=sigma1<sigma2?sigma1:sigma2;
        double M=sin(atan(sigma))/sqrt(1+sigma*sigma);
        double a=lamda*z/M_PI;

        Mat ondeletteN;
        Mat temp;
        Mat temp1=XY2.clone();
        for(int x=0;x<temp1.rows;x++){
            for(int y=0;y<temp1.cols;y++){
                temp1.at<double>(x,y)=sin(temp1.at<double>(x,y)/a);
            }
        } //计算矩阵元素的sin值
        exp(-XY2/(a*sigma),temp);
        multiply(1/a*(temp1-M),temp,ondeletteN,1,-1);


        Mat wt=fftimage.clone();
        Mat temp2=fft2(ondeletteN);
        //multiply(fftimage,temp2,wt,1,-1);
        for(int i=0;i<fftimage.rows;i++){
            for(int j=0;j<fftimage.cols;j++){
                wt.at<double>(i,2*j)=fftimage.at<double>(i,2*j)*temp2.at<double>(i,2*j)-fftimage.at<double>(i,2*j+1)*temp2.at<double>(i,2*j+1);
                wt.at<double>(i,2*j+1)=fftimage.at<double>(i,2*j)*temp2.at<double>(i,2*j+1)+fftimage.at<double>(i,2*j+1)*temp2.at<double>(i,2*j);
            }
        }

        Mat wt1;
        wt1=ifftshift(ifft2(wt));
        //Mat temp22=ifft2(wt);
        //wt1=ifftshift(ifft2(wt,'nonsymmetric'));

        //Mat planez=(real(wt1));
        Mat planez=wt1.clone();

        //all_light = all_light + planez;

        double max;
        double min;
        extreme(planez,max,min);

        graymax.push_back(max);
        graymin.push_back(min);

        planez=(planez-graymin[inum])/(graymax[inum]-graymin[inum]);
        Mat planetemp=planez.clone();

        normalize(planetemp, planetemp, 0.0,255.0,NORM_MINMAX);//归一化
        //cvtColor(planetemp,planetemp, CV_BGR2GRAY); //灰度化

        sprintf(filename,"%s%d%s",tempfilename.c_str(),inum,".jpg"); //图像从0开始命名，假设文件名无／,不确定输入是否文件夹

        imwrite(filename,planetemp);
        //reimg.push_back(planez);

        inum++;
        renum=inum;

        if(closeflag == true) {
            return;
        }
        emit(finish());
    }

    //qDebug()<<renum;

}
