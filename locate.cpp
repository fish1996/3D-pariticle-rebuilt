
#include "locate.h"
#include <qDebug>
location::location()
{
    //qDebug()<<"location";
}

location::~location(){

}

void location::set(Mat *p1xy0,Mat *p1area0,Mat *p1box0,Mat *ip1xy0,Mat *ip1area0,Mat *ip1box0)
{
    p1xy=p1xy0;
    p1area=p1area0;
    p1box=p1box0;
    ip1xy=ip1xy0;
    ip1area=ip1area0;
    ip1box=ip1box0;
}

//取矩阵极值
void location::extreme(Mat &imageout, double &maxv, double &minv)
{
    maxv=minv=0.0; //初始化
    minMaxIdx(imageout,&minv,&maxv);
}

Mat location::fft2(Mat src)
{
    Mat Fourier;
    Mat planes[]={Mat_<double>(src),Mat::zeros(src.size(),CV_64F)};
    merge(planes,2,Fourier);
    dft(Fourier,Fourier,0,0);

    return Fourier;
}

Mat location::ifft2(Mat src)
{
    Mat Fourier;
    idft(src,Fourier,0);
    vector<Mat> planes(2);
    split(Fourier,planes);
    int h=planes[0].rows;
    int w=planes[0].cols;
    planes[0]=planes[0]/(h*w);

    return planes[0];
}

//矩阵按照行合并C = [A;B];
Mat location::mergeRows(Mat A, Mat B)
{
    CV_Assert(A.cols == B.cols&&A.type() == B.type());
    int totalRows = A.rows + B.rows;

    Mat mergedDescriptors(totalRows, A.cols, A.type());
    Mat submat = mergedDescriptors.rowRange(0, A.rows);
    A.copyTo(submat);
    submat = mergedDescriptors.rowRange(A.rows, totalRows);
    B.copyTo(submat);

    return mergedDescriptors;
}

//矩阵按照列合并C = [A B];
Mat location::mergeCols(Mat A, Mat B)
{
    if(A.rows != B.rows&&A.type() != B.type()){
        cout<<"merge error!"<<endl;
        return A;
    }
    int totalCols = A.cols + B.cols;

    Mat mergedDescriptors( A.rows,totalCols, A.type());
    Mat submat = mergedDescriptors.colRange(0, A.cols);
    A.copyTo(submat);
    submat = mergedDescriptors.colRange(A.cols, totalCols);
    B.copyTo(submat);

    return mergedDescriptors;
}

//找到矩阵每行的最大值以及对应位置序号
void location::maxrow(Mat &imgsrc,Mat &imgmax,Mat &imgorder){
    double max;
    int maxorder;
    for(int i=0;i<imgsrc.rows;i++){
        max=imgsrc.at<double>(i,0);
        maxorder=0;
        for(int j=1;j<imgsrc.cols;j++){
            if(imgsrc.at<double>(i,j)>max){
                max=imgsrc.at<double>(i,j);
                maxorder=j;
            }
        }
        imgmax.at<double>(i,0)=max;
        imgorder.at<double>(i,0)=maxorder+1;
    }
}

Mat location::SQRTs(const Mat &a)
{
    Mat t=a.clone();
    double sq;
    for(int i=0;i<t.rows;i++){
        for(int j=0;j<t.cols;j++){
            sq=sqrt(t.at<double>(i,j)*4/PI);
            t.at<double>(i,j)=sq;
        }
    }
    return t;
}

//删除矩阵某一行
void location::DeleteRowofMat(Mat& object,vector<int> rownum){
    int num=0;
    for(int n=0;n<rownum.size();n++){
        num=rownum[n]-n;
        if(num<0||num>=object.rows){
            return;
        }
        else{
            if(num==object.rows-1){
                object.pop_back();
            }
            else{
                for(int i=num+1;i<object.rows;i++){
                    object.row(i-1)=object.row(i)+Scalar(0,0,0,0);
                }
                object.pop_back();
            }
        }
    }
}
//由于matlab求标准差使用的n-1，这里特殊说明
double location::getvariance(Mat object){
    Scalar mean;
    Scalar stddev;
    meanStdDev ( object, mean, stddev );
    double imean=mean.val[0]; //得到矩阵平均数

    int row=object.rows;
    int col=object.cols;
    double var=0;
    double dif=0;

    for(int i=0;i<row;i++){
        for(int j=0;j<col;j++){
            dif=(object.at<double>(i,j)-imean);
            var=var+dif*dif;
        }
    }
    var=var/(row*col-1);

    return var;
}

int location::round(float x)
{
    return (int)(x+0.5);
}

//image 目前图片,boxcoef输入的参数,planesumnumber第一步所用到的总张数
//p1xyzd矩阵参数可以用于绘制三维图，共四列，分别是X,Y,Z,S(这里给的是直径）
void location::p_location(string tempfilename,double boxcoef,int planesumnuber,int* secnum,int imgnum,double* sectionmin,double* sectionmax){
    //qDebug()<<"p_location";
    p1xyzd=new Mat[imgnum]();
    diameterfre=new double *[imgnum];
    for(int i=0;i<imgnum;i++){
        diameterfre[i]=new double[*secnum]();
    }
    diametermin=new double [imgnum];
    diametermax=new double [imgnum];

    pos=new struct position*[imgnum]();
    pointnum=new int[imgnum];

    meandiameter=new double [imgnum];

    _secnum=secnum;
    _sectionmax=sectionmax;
    _sectionmin=sectionmin;

    for(int imgn=0;imgn<imgnum;imgn++){
        //qDebug()<<"imgn = "<<imgn;
        int row=p1xy[imgn].rows;
        pointnum[imgn]=row;

        pos[imgn]=new struct position[row]();
        p1xy[imgn].convertTo(p1xy[imgn],CV_64F);
        p1area[imgn].convertTo(p1area[imgn],CV_64F);
        p1box[imgn].convertTo(p1box[imgn],CV_64F);

        char img_fname[1000];
        sprintf(img_fname,"%s%s%d%s",tempfilename.c_str(),"\\img_fuse",imgn,".bmp");//需要根据实际改
        Mat img=imread(img_fname,CV_LOAD_IMAGE_GRAYSCALE);
        Size size=img.size();
        int ny=size.height;
        int nx=size.width;
        img.release();

        Size size1=p1xy[imgn].size(); //当前界面上的图
        int N1=size1.height;
        int M1=size1.width;
        int p1num=N1;

        char imgname[1000];
        string forefile="\\temp_plane_";
        Mat img1temp;
        Mat tenv1(p1num,planesumnuber,CV_64F,Scalar(0));

        for(int nz=1;nz<=planesumnuber;nz++){
            sprintf(imgname,"%s%s%d%s",tempfilename.c_str(),forefile.c_str(),nz-1+imgn*planesumnuber,".jpg"); //图像从1开始命名//!!!!!!
            img1temp = imread(imgname,CV_LOAD_IMAGE_GRAYSCALE);
            img1temp.convertTo(img1temp,CV_64F);

            for(int i=1;i<=p1num;i++){
                int ymin = round(p1box[imgn].at<double>(i-1,1)-boxcoef*(p1box[imgn].at<double>(i-1,3)));
                int ymax = round(p1box[imgn].at<double>(i-1,1)+(1+boxcoef)*(p1box[imgn].at<double>(i-1,3)));
                int xmin = round(p1box[imgn].at<double>(i-1,0)-boxcoef*(p1box[imgn].at<double>(i-1,2)));
                int xmax = round(p1box[imgn].at<double>(i-1,0)+(1+boxcoef)*(p1box[imgn].at<double>(i-1,2)));
                ymin = 1>=ymin?1:ymin;
                xmin = 1>=xmin?1:xmin;
                ymax = ymax<=ny?ymax:ny;
                xmax = xmax<=nx?xmax:nx;

                Mat_<double> roitemp = img1temp(Range(ymin-1,ymax),Range(xmin-1,xmax));
                Mat_<double> temp=roitemp.clone();

                Mat Sx = (Mat_<double>(3, 3) << -1, -2, -1, 0, 0, 0, 1, 2, 1);
                Mat Sxt=Sx.t();
                Mat Gx,Gy;
                filter2D(temp, Gx, -1, Sx, Point(-1, -1), 0, BORDER_REPLICATE); //复制边界，sobel算子
                filter2D(temp, Gy, -1, Sxt, Point(-1, -1), 0, BORDER_REPLICATE); //复制边界，sobel算子逆

                Mat G;
                Mat Gx1,Gy1;
                pow(Gx,2,Gx1);
                pow(Gy,2,Gy1);
                G=Gx1+Gy1;

                double var = getvariance(G);

                tenv1.at<double>(i-1,nz-1)=var;
                Gx.release();
                Gy.release();

            }
             emit(locateOk());

        }
        Mat maxz1(tenv1.rows,1,CV_64FC1);
        Mat p1xyztenv(tenv1.rows,1,CV_64FC1);

        maxrow(tenv1,maxz1,p1xyztenv);

        Mat p1=mergeCols(p1xy[imgn],p1xyztenv);
        Mat p2area=SQRTs(p1area[imgn]);
        p1xyzd[imgn]=mergeCols(p1,p2area);

        for(int r=0;r<row;r++){
            pos[imgn][r].X=p1xyzd[imgn].at<double>(r,0);
            pos[imgn][r].Y=p1xyzd[imgn].at<double>(r,1);
            pos[imgn][r].Z=p1xyzd[imgn].at<double>(r,2);
            pos[imgn][r].D=p1xyzd[imgn].at<double>(r,3);
        }

        double max,min;
        int secponum; //表示区间内的粒子总数
        flag=false; //表示区间最大值不大于实际
        extreme(p2area,max,min);

        if(max>*sectionmax){
            max=*sectionmax;
            flag=true;
        }
        if(min<*sectionmin){
            min=*sectionmin;
        }

        diametermin[imgn]=min;
        diametermax[imgn]=max;
        meandiameter[imgn]=0;

        int* frenum=new int[*secnum];
        memset(frenum,0,sizeof(frenum));
        for(int i=0;i<*secnum;i++){
            frenum[i] = 0;
        }
        secponum=0;
        double secdis=(max-min)/(*secnum);

        if(flag){
            secdis=(max-min)/(*secnum-1);
        }

        for(int ni=0;ni<p2area.rows;ni++){
            if(p2area.at<double>(ni,0)>=min){
                double factdis=p2area.at<double>(ni,0)-min;
                int factfre=floor(factdis/secdis);
                if(factfre>*secnum-1)factfre=*secnum-1;
                frenum[factfre]++;
                secponum++;
                meandiameter[imgn]+=p2area.at<double>(ni,0);
            }
        }

        double *fre=new double[*secnum];
        memset(fre,0,*secnum*sizeof(double));
        for(int i=0;i<*secnum;i++){
            fre[i]=frenum[i]*1.0/(secponum);
            diameterfre[imgn][i]=fre[i];
        }
        meandiameter[imgn]/=secponum;

        img1temp.release();
        tenv1.release();
        maxz1.release();
        p1xyztenv.release();
        p1.release();
        p2area.release();
        delete[] frenum;
        delete[] fre;

    }

    //做反色图相应处理
    ip1xyzd=new Mat[imgnum]();
    idiameterfre=new double *[imgnum];
    for(int i=0;i<imgnum;i++){
        idiameterfre[i]=new double[*secnum]();
    }
    idiametermin=new double [imgnum];
    idiametermax=new double [imgnum];

    ipos=new struct position*[imgnum]();
    ipointnum=new int[imgnum];

    imeandiameter=new double [imgnum];

    for(int imgn=0;imgn<imgnum;imgn++){
        //qDebug()<<"imgn = "<<imgn;
        int row=ip1xy[imgn].rows;
        ipointnum[imgn]=row;

        ipos[imgn]=new struct position[row]();
        ip1xy[imgn].convertTo(ip1xy[imgn],CV_64F);
        ip1area[imgn].convertTo(ip1area[imgn],CV_64F);
        ip1box[imgn].convertTo(ip1box[imgn],CV_64F);

        char img_fname[1000];
        sprintf(img_fname,"%s%s%d%s",tempfilename.c_str(),"\\img_fuse",imgn,"i.bmp");//需要根据实际改
        Mat img=imread(img_fname,CV_LOAD_IMAGE_GRAYSCALE);
        Size size=img.size();
        int ny=size.height;
        int nx=size.width;
        img.release();

        Size size1=ip1xy[imgn].size(); //当前界面上的图
        int N1=size1.height;
        int M1=size1.width;
        int p1num=N1;

        char imgname[1000];
        string forefile="\\temp_plane_";
        Mat img1temp;
        Mat tenv1(p1num,planesumnuber,CV_64F,Scalar(0));

        for(int nz=1;nz<=planesumnuber;nz++){
            sprintf(imgname,"%s%s%d%s",tempfilename.c_str(),forefile.c_str(),nz-1+imgn*planesumnuber,"i.jpg"); //图像从1开始命名//!!!!!!
            img1temp = imread(imgname,CV_LOAD_IMAGE_GRAYSCALE);
            img1temp.convertTo(img1temp,CV_64F);

            for(int i=1;i<=p1num;i++){
                int ymin = round(ip1box[imgn].at<double>(i-1,1)-boxcoef*(ip1box[imgn].at<double>(i-1,3)));
                int ymax = round(ip1box[imgn].at<double>(i-1,1)+(1+boxcoef)*(ip1box[imgn].at<double>(i-1,3)));
                int xmin = round(ip1box[imgn].at<double>(i-1,0)-boxcoef*(ip1box[imgn].at<double>(i-1,2)));
                int xmax = round(ip1box[imgn].at<double>(i-1,0)+(1+boxcoef)*(ip1box[imgn].at<double>(i-1,2)));
                ymin = 1>=ymin?1:ymin;
                xmin = 1>=xmin?1:xmin;
                ymax = ymax<=ny?ymax:ny;
                xmax = xmax<=nx?xmax:nx;

                Mat_<double> roitemp = img1temp(Range(ymin-1,ymax),Range(xmin-1,xmax));
                Mat_<double> temp=roitemp.clone();

                Mat Sx = (Mat_<double>(3, 3) << -1, -2, -1, 0, 0, 0, 1, 2, 1);
                Mat Sxt=Sx.t();
                Mat Gx,Gy;
                filter2D(temp, Gx, -1, Sx, Point(-1, -1), 0, BORDER_REPLICATE); //复制边界，sobel算子
                filter2D(temp, Gy, -1, Sxt, Point(-1, -1), 0, BORDER_REPLICATE); //复制边界，sobel算子逆

                Mat G;
                Mat Gx1,Gy1;
                pow(Gx,2,Gx1);
                pow(Gy,2,Gy1);
                G=Gx1+Gy1;

                double var = getvariance(G);

                tenv1.at<double>(i-1,nz-1)=var;
                Gx.release();
                Gy.release();

            }

             emit(locateOk());

        }

        Mat maxz1(tenv1.rows,1,CV_64FC1);
        Mat p1xyztenv(tenv1.rows,1,CV_64FC1);

        maxrow(tenv1,maxz1,p1xyztenv);

        Mat p1=mergeCols(ip1xy[imgn],p1xyztenv);
        Mat p2area=SQRTs(ip1area[imgn]);
        ip1xyzd[imgn]=mergeCols(p1,p2area);

        for(int r=0;r<row;r++){
            ipos[imgn][r].X=ip1xyzd[imgn].at<double>(r,0);
            ipos[imgn][r].Y=ip1xyzd[imgn].at<double>(r,1);
            ipos[imgn][r].Z=ip1xyzd[imgn].at<double>(r,2);
            ipos[imgn][r].D=ip1xyzd[imgn].at<double>(r,3);
        }

        double max,min;
        extreme(p2area,max,min);
        iflag=false;
        int secponum;

        if(max>*sectionmax){
            max=*sectionmax;
            iflag=true;
        }
        if(min<*sectionmin){
            min=*sectionmin;
        }


        idiametermin[imgn]=min;
        idiametermax[imgn]=max;
        imeandiameter[imgn]=0;
        secponum=0;

        int* frenum=new int[*secnum];
        memset(frenum,0,sizeof(frenum));
        for(int i=0;i<*secnum;i++){
            frenum[i] = 0;
        }
        double secdis=(max-min)/(*secnum);
        if(iflag){
            secdis=(max-min)/(*secnum-1);
        }
        for(int ni=0;ni<p2area.rows;ni++){
            if(p2area.at<double>(ni,0)>=min){
                double factdis=p2area.at<double>(ni,0)-min;
                int factfre=floor(factdis/secdis);
                if(factfre>*secnum-1)factfre=*secnum-1;
                frenum[factfre]++;
                secponum++;
                imeandiameter[imgn]+=p2area.at<double>(ni,0);
            }
        }

        double *fre=new double[*secnum];
        memset(fre,0,*secnum*sizeof(double));
        for(int i=0;i<*secnum;i++){
            fre[i]=frenum[i]*1.0/(secponum);
            idiameterfre[imgn][i]=fre[i];
        }
        imeandiameter[imgn]/=secponum;

        img1temp.release();
        tenv1.release();
        maxz1.release();
        p1xyztenv.release();
        p1.release();
        p2area.release();
        delete[] frenum;
        delete[] fre;

    }

    emit(locateAll());
    emit(locateAll(pos,ipos,imgnum,planesumnuber,diameterfre,idiameterfre,
            diametermin,idiametermin,diametermax,idiametermax,
                   pointnum,ipointnum,flag,iflag,meandiameter,imeandiameter));
}
