
#include "extend.h"
#include<QDebug>

extend::extend(int size1,int num1,string tempfilename1, double zmin1, double zmax1, double interval
               )
{
    size = size1;
    num = num1;
    tempfilename=tempfilename1;
    zmin=zmin1;
    zmax=zmax1;
    precision=interval;

}

void extend::start()
{
    if(!dwt22Initialize()){
        cout<<"Could not initialize!"<<endl;
        return;
    }
    if(!idwt22Initialize()){
        cout<<"Could not initialize!"<<endl;
        return;
    }
    for(int i = 0;i<num;i++) {
        qDebug()<<"i="<<i;
        image_extend(i,size, zmin, zmax, precision);
        iimage_extend(i,size, zmin, zmax, precision);
    }


    idwt22Terminate();
    dwt22Terminate();
    qDebug()<<"allfinish hererere";
    emit(extendAll());
}

extend::~extend()
{
}

Mat extend::POW2(const Mat& a)
{
    Mat t(a.size(),a.type(),Scalar(0));
    for (int j=0; j<a.rows; j++) {
        const double* ap=a.ptr<double>(j);
        double* tp=t.ptr<double>(j);
        for (int i=0; i<a.cols; i++)    tp[i]=ap[i]*ap[i];
    }
    return t;
}

Mat extend::SQRT(const Mat& b)
{
    Mat t(b.size(),b.type(),Scalar(0));
    for (int j=0; j<b.rows; j++) {
        const double* ap=b.ptr<double>(j);
        double* tp=t.ptr<double>(j);
        for (int i=0; i<b.cols; i++)    tp[i]=sqrt(ap[i]);
    }
    return t;
}

Mat extend::stdfilt(Mat_<double> const& I, InputArray kernel, int borderType)
{
    Mat G1,G2;
    Mat I2=POW2(I);
    Scalar n=sum(kernel);
    filter2D( I2, G2, CV_64F,kernel, Point(-1,-1),0,borderType);
    G2=G2/(n[0]-1.);
    filter2D( I,  G1, CV_64F,kernel, Point(-1,-1),0,borderType);
    G1=POW2(G1)/(n[0]*(n[0]-1.));
    return SQRT(max<double>(G2-G1,0.));//return SQRT<double>(MAMX<double>(G2-G1,0.));
}

mwArray extend::mat2mwarray(const Mat imgsrc)
{
    Mat imgsrcs=imgsrc.clone();
    int h,w,c;
    h=imgsrcs.rows;
    w=imgsrcs.cols;
    c=imgsrcs.channels();

    if(c==1){
        mwSize dims[2]={w,h};
        mwArray pMat(2,dims,mxDOUBLE_CLASS);

        double *x=(double*)imgsrcs.data;
        pMat.SetData(x,h*w);

        return pMat;
    }
    else if(c==3){
        mwSize dims[3]={w,h,c};
        mwArray pMat(3,dims,mxDOUBLE_CLASS);

        for(int i=0;i<h;i++){
            for(int j=0;j<w;j++){
                pMat(i+1,j+1,1)=(double)imgsrcs.at<Vec3b>(i,j)[2];
                pMat(i+1,j+1,2)=(double)imgsrcs.at<Vec3b>(i,j)[1];
                pMat(i+1,j+1,3)=(double)imgsrcs.at<Vec3b>(i,j)[0];
            }
        }
        return pMat;
    }
}

Mat extend::mwarray2mat(const mwArray data)
{
    mwArray dims=data.GetDimensions();

    int n=data.NumberOfDimensions();
    int M=dims(1);
    int N=dims(2);
    int h=M;
    int w=N;

    if(n==2){
        Mat img(h,w,CV_64F);
        for(int i=0;i<h;i++){
            for(int j=0;j<w;j++){
                double index=data(i+1,j+1);
                img.at<double>(i,j)=index;
                //img.row(i).col(j).data[0]=index;
            }
        }
        return img;
    }
    else if(n==3){
        int c=dims(3);
        Mat img(h,w,CV_64FC3);
        for(int i=0;i<h;i++){
            for(int j=0;j<w;j++){
                img.at<Vec3b>(i,j)[0]=(double)data.Get(3,i+1,j+1,3);
                img.at<Vec3b>(i,j)[1]=(double)data.Get(3,i+1,j+1,2);
                img.at<Vec3b>(i,j)[2]=(double)data.Get(3,i+1,j+1,1);
            }
        }
        return img;

    }
}

void extend::dwt2(Mat &i, Mat &ll, Mat &hh, Mat &dd, Mat &vv)
{
    mwArray p=mat2mwarray(i);
    Mat a=mwarray2mat(p);

    mwArray llp,hhp,vvp,ddp;
    dwt22(4,llp,hhp,vvp,ddp,p);

    ll=mwarray2mat(llp);
    hh=mwarray2mat(hhp);
    vv=mwarray2mat(vvp);
    dd=mwarray2mat(ddp);
}

void extend::idwt2(Mat &idst,Mat &llf,Mat &hhf,Mat &ddf,Mat &vvf)
{
    mwArray lf=mat2mwarray(llf);
    mwArray hf=mat2mwarray(hhf);
    mwArray vf=mat2mwarray(vvf);
    mwArray df=mat2mwarray(ddf);

    mwArray img_f;
    idwt22(1,img_f,lf,hf,vf,df);
    idst=mwarray2mat(img_f);
}

int extend::round(float x)
{
    return (int)(x+0.5);
}

void extend::accurate4(Mat& img)
{
    for(int i=0;i<img.rows;i++){
        for(int j=0;j<img.cols;j++){
            img.at<double>(i,j)=round(img.at<double>(i,j)*10000)/10000.f;
        }
    }
}

void extend::image_extend(int num,int size, double zmin, double zmax, double precision)
{
   // qDebug()<<"here";
    int planesumnumber;
    int plane_ind;

    planesumnumber=0;
    for(double n=zmin;n<zmax+precision;n=n+precision){
        planesumnumber++;
    }

    char tempfilename_mat[1000];
    string forefile="\\temp_plane_";
    char newimg[1000];
    Mat temp_mat;
    Mat Image;

    Mat_<double> llf,hhf,vvf,ddf,var_dd,var_ll,map_dd,map_ll,mean_llf;

    for(plane_ind=(num)*size;plane_ind<(num+1)*size;plane_ind++){
        sprintf(tempfilename_mat,"%s%s%d%s",tempfilename.c_str(),forefile.c_str(),plane_ind,".jpg"); //图像从1开始命名，假设文件名无／,不确定输入是否文件夹
        qDebug()<<tempfilename_mat;
        temp_mat=imread(tempfilename_mat,CV_LOAD_IMAGE_GRAYSCALE);
        temp_mat.convertTo(Image,CV_64F);
        Mat NHOOD(15, 15, CV_64FC1, Scalar(1));
      //  qDebug()<<tempfilename_mat;
        Mat_<double> ll,hh,dd,vv;

        dwt2(Image,ll,hh,vv,dd);
     //   qDebug()<<tempfilename_mat;
        Mat Sx = (Mat_<double>(3, 3) << -1, -2, -1, 0, 0, 0, 1, 2, 1);
        Mat Sxt=Sx.t();
        Mat hh1,ddh1,vv1,ddv1;
        filter2D(hh, hh1, -1, Sxt, Point(-1, -1), 0, BORDER_REPLICATE); //复制边界，sobel算子逆
        filter2D(dd, ddh1, -1, Sxt, Point(-1, -1), 0, BORDER_REPLICATE); //复制边界，sobel算子逆
        filter2D(vv, vv1, -1, Sx, Point(-1, -1), 0, BORDER_REPLICATE); //复制边界，sobel算子
        filter2D(dd, ddv1, -1, Sx, Point(-1, -1), 0, BORDER_REPLICATE); //复制边界，sobel算子
        Mat Gx_dd,Gy_dd;
        Gx_dd=hh1+ddh1;
        Gy_dd=vv1+ddv1;
        Mat G_dd;
        Mat Gx_dd1,Gy_dd1;
        pow(Gx_dd,2,Gx_dd1);
        pow(Gy_dd,2,Gy_dd1);
        G_dd=Gx_dd1+Gy_dd1;

        Mat vartemp_dd=stdfilt(G_dd,NHOOD,BORDER_REFLECT);//处理边界镜像，BORDER_REFLECT或BORDER_REFLECT_101

        Mat ll1,ll2;
        filter2D(ll, ll1, -1, Sxt, Point(-1, -1), 0, BORDER_REPLICATE); //复制边界，sobel算子逆
        filter2D(ll, ll2, -1, Sx, Point(-1, -1), 0, BORDER_REPLICATE); //复制边界，sobel算子逆
        Mat Gx_ll=ll1+ll1;
        Mat Gy_ll=ll2+ll2;
        Mat G_ll;
        Mat Gx_ll1,Gy_ll1;
        pow(Gx_ll,2,Gx_ll1);
        pow(Gy_ll,2,Gy_ll1);
        G_ll=Gx_ll1+Gy_ll1;

        Mat vartemp_ll=stdfilt(G_ll,NHOOD,BORDER_REFLECT);//处理边界镜像，BORDER_REFLECT或BORDER_REFLECT_101

        if(plane_ind%size==0){
            llf=ll.clone();
            hhf=hh.clone();
            vvf=vv.clone();
            ddf=dd.clone();
            var_dd=vartemp_dd.clone();
            var_ll=vartemp_ll.clone();
            Size size=ll.size();
            int d1=size.height;
            int d2=size.width;

            map_ll=Mat::ones(d1, d2, CV_64F);
            map_dd= map_ll.clone();

            mean_llf= ll.clone();
        }
        else{
            for(int x=0;x<vartemp_dd.rows;x++){
                for(int y=0;y<vartemp_dd.cols;y++){
                    if(vartemp_dd.at<double>(x,y)>var_dd.at<double>(x,y)){
                        var_dd.at<double>(x,y)=vartemp_dd.at<double>(x,y);
                        hhf.at<double>(x,y)=hh.at<double>(x,y);
                        vvf.at<double>(x,y)=vv.at<double>(x,y);
                        ddf.at<double>(x,y)=dd.at<double>(x,y);
                        map_dd.at<double>(x,y)=plane_ind+1;
                    }
                }
            }
            for(int x1=0;x1<vartemp_ll.rows;x1++){
                for(int y1=0;y1<vartemp_ll.cols;y1++){
                    if(vartemp_ll.at<double>(x1,y1)>var_ll.at<double>(x1,y1)){
                        var_ll.at<double>(x1,y1)=vartemp_ll.at<double>(x1,y1);
                        llf.at<double>(x1,y1)=ll.at<double>(x1,y1);
                        map_ll.at<double>(x1,y1)=plane_ind+1;
                    }
                }
            }

            mean_llf=mean_llf+ll;
        }

        emit(extendOk());

    }

    accurate4(llf);
    accurate4(hhf);
    accurate4(vvf);
    accurate4(ddf);

    Mat img_f;
    idwt2(img_f,llf,hhf,vvf,ddf);

    Size size=Image.size();
    int ny=size.height;
    int nx=size.width;

    img_f=img_f(Range(0,ny),Range(0,nx));

    Mat img_fc=img_f.clone();
    normalize(img_fc, img_fc, 0.0,255.0,NORM_MINMAX);//归一化

    //imshow("figure",img_fc); //视具体情况定
    sprintf(newimg,"%s%s%d%s",tempfilename.c_str(),"\\img_fuse",num,".bmp");
    imwrite(newimg,img_fc);

    Mat map_ddc=map_dd.clone();
    normalize(map_ddc, map_ddc, 0,1,NORM_MINMAX);//归一化
    normalize(map_ddc, map_ddc, 0,63,NORM_MINMAX);//归一化
    map_dd.convertTo(map_ddc,CV_8U);//转为索引图
    sprintf(newimg,"%s%s%d%s",tempfilename.c_str(),"\\map_highfrequency",num,".tiff");
    imwrite(newimg,map_ddc);

    Mat map_llc=map_ll.clone();
    normalize(map_llc, map_llc, 0,1,NORM_MINMAX);//归一化
    normalize(map_llc, map_llc, 0,63,NORM_MINMAX);//归一化
    map_ll.convertTo(map_llc,CV_8U);
    sprintf(newimg,"%s%s%d%s",tempfilename.c_str(),"\\map_lowfrequency",num,".tiff");
    imwrite(newimg,map_llc);

    temp_mat.release();
    Image.release();
    llf.release();
    hhf.release();
    vvf.release();
    ddf.release();
    var_dd.release();
    var_ll.release();
    map_dd.release();
    mean_llf.release();
}



//做反色图的结果
void extend::iimage_extend(int num,int size, double zmin, double zmax, double precision)
{
   // qDebug()<<"here";
    int planesumnumber;
    int plane_ind;

    planesumnumber=0;
    for(double n=zmin;n<zmax+precision;n=n+precision){
        planesumnumber++;
    }

    char tempfilename_mat[1000];
    string forefile="\\temp_plane_";
    char newimg[1000];
    Mat temp_mat;
    Mat Image;

    Mat_<double> llf,hhf,vvf,ddf,var_dd,var_ll,map_dd,map_ll,mean_llf;

    for(plane_ind=(num)*size;plane_ind<(num+1)*size;plane_ind++){
        sprintf(tempfilename_mat,"%s%s%d%s",tempfilename.c_str(),forefile.c_str(),plane_ind,"i.jpg"); //图像从1开始命名，假设文件名无／,不确定输入是否文件夹
        qDebug()<<tempfilename_mat;
        temp_mat=imread(tempfilename_mat,CV_LOAD_IMAGE_GRAYSCALE);
        temp_mat.convertTo(Image,CV_64F);
        Mat NHOOD(15, 15, CV_64FC1, Scalar(1));
      //  qDebug()<<tempfilename_mat;
        Mat_<double> ll,hh,dd,vv;

        dwt2(Image,ll,hh,vv,dd);
     //   qDebug()<<tempfilename_mat;
        Mat Sx = (Mat_<double>(3, 3) << -1, -2, -1, 0, 0, 0, 1, 2, 1);
        Mat Sxt=Sx.t();
        Mat hh1,ddh1,vv1,ddv1;
        filter2D(hh, hh1, -1, Sxt, Point(-1, -1), 0, BORDER_REPLICATE); //复制边界，sobel算子逆
        filter2D(dd, ddh1, -1, Sxt, Point(-1, -1), 0, BORDER_REPLICATE); //复制边界，sobel算子逆
        filter2D(vv, vv1, -1, Sx, Point(-1, -1), 0, BORDER_REPLICATE); //复制边界，sobel算子
        filter2D(dd, ddv1, -1, Sx, Point(-1, -1), 0, BORDER_REPLICATE); //复制边界，sobel算子
        Mat Gx_dd,Gy_dd;
        Gx_dd=hh1+ddh1;
        Gy_dd=vv1+ddv1;
        Mat G_dd;
        Mat Gx_dd1,Gy_dd1;
        pow(Gx_dd,2,Gx_dd1);
        pow(Gy_dd,2,Gy_dd1);
        G_dd=Gx_dd1+Gy_dd1;

        Mat vartemp_dd=stdfilt(G_dd,NHOOD,BORDER_REFLECT);//处理边界镜像，BORDER_REFLECT或BORDER_REFLECT_101

        Mat ll1,ll2;
        filter2D(ll, ll1, -1, Sxt, Point(-1, -1), 0, BORDER_REPLICATE); //复制边界，sobel算子逆
        filter2D(ll, ll2, -1, Sx, Point(-1, -1), 0, BORDER_REPLICATE); //复制边界，sobel算子逆
        Mat Gx_ll=ll1+ll1;
        Mat Gy_ll=ll2+ll2;
        Mat G_ll;
        Mat Gx_ll1,Gy_ll1;
        pow(Gx_ll,2,Gx_ll1);
        pow(Gy_ll,2,Gy_ll1);
        G_ll=Gx_ll1+Gy_ll1;

        Mat vartemp_ll=stdfilt(G_ll,NHOOD,BORDER_REFLECT);//处理边界镜像，BORDER_REFLECT或BORDER_REFLECT_101

        if(plane_ind%size==0){
            llf=ll.clone();
            hhf=hh.clone();
            vvf=vv.clone();
            ddf=dd.clone();
            var_dd=vartemp_dd.clone();
            var_ll=vartemp_ll.clone();
            Size size=ll.size();
            int d1=size.height;
            int d2=size.width;

            map_ll=Mat::ones(d1, d2, CV_64F);
            map_dd= map_ll.clone();

            mean_llf= ll.clone();
        }
        else{
            for(int x=0;x<vartemp_dd.rows;x++){
                for(int y=0;y<vartemp_dd.cols;y++){
                    if(vartemp_dd.at<double>(x,y)>var_dd.at<double>(x,y)){
                        var_dd.at<double>(x,y)=vartemp_dd.at<double>(x,y);
                        hhf.at<double>(x,y)=hh.at<double>(x,y);
                        vvf.at<double>(x,y)=vv.at<double>(x,y);
                        ddf.at<double>(x,y)=dd.at<double>(x,y);
                        map_dd.at<double>(x,y)=plane_ind+1;
                    }
                }
            }
            for(int x1=0;x1<vartemp_ll.rows;x1++){
                for(int y1=0;y1<vartemp_ll.cols;y1++){
                    if(vartemp_ll.at<double>(x1,y1)>var_ll.at<double>(x1,y1)){
                        var_ll.at<double>(x1,y1)=vartemp_ll.at<double>(x1,y1);
                        llf.at<double>(x1,y1)=ll.at<double>(x1,y1);
                        map_ll.at<double>(x1,y1)=plane_ind+1;
                    }
                }
            }

            mean_llf=mean_llf+ll;
        }

        emit(extendOk());

    }

    accurate4(llf);
    accurate4(hhf);
    accurate4(vvf);
    accurate4(ddf);

    Mat img_f;
    idwt2(img_f,llf,hhf,vvf,ddf);

    Size size=Image.size();
    int ny=size.height;
    int nx=size.width;

    img_f=img_f(Range(0,ny),Range(0,nx));

    Mat img_fc=img_f.clone();
    normalize(img_fc, img_fc, 0.0,255.0,NORM_MINMAX);//归一化

    //imshow("figure",img_fc); //视具体情况定
    sprintf(newimg,"%s%s%d%s",tempfilename.c_str(),"\\img_fuse",num,"i.bmp");
    imwrite(newimg,img_fc);

    Mat map_ddc=map_dd.clone();
    normalize(map_ddc, map_ddc, 0,1,NORM_MINMAX);//归一化
    normalize(map_ddc, map_ddc, 0,63,NORM_MINMAX);//归一化
    map_dd.convertTo(map_ddc,CV_8U);//转为索引图
    sprintf(newimg,"%s%s%d%s",tempfilename.c_str(),"\\map_highfrequency",num,"i.tiff");
    imwrite(newimg,map_ddc);

    Mat map_llc=map_ll.clone();
    normalize(map_llc, map_llc, 0,1,NORM_MINMAX);//归一化
    normalize(map_llc, map_llc, 0,63,NORM_MINMAX);//归一化
    map_ll.convertTo(map_llc,CV_8U);
    sprintf(newimg,"%s%s%d%s",tempfilename.c_str(),"\\map_lowfrequency",num,"i.tiff");
    imwrite(newimg,map_llc);

    temp_mat.release();
    Image.release();
    llf.release();
    hhf.release();
    vvf.release();
    ddf.release();
    var_dd.release();
    var_ll.release();
    map_dd.release();
    mean_llf.release();
}
