/*#include "imginverse.h"

imginverse::imginverse()
{

}

imginverse::~imginverse()
{
}

void imginverse::doimginverse(string tempfilename)
{
    extern vector<double> graymax;
    extern vector<double> graymin;
    extern int renum; //重建数量
    qDebug()<<"inverse";

    vector<double>::iterator biggest = max_element(begin(graymax), end(graymax));
    double graybig=*biggest;
    auto smallest = min_element(begin(graymin), end(graymin));
    double graysmall=*smallest;

    char filename[1000];
    string forefile="\\temp_plane_";

    for(int i=0;i<renum;i++){
        sprintf(filename,"%s%s%d%s",tempfilename.c_str(),forefile.c_str(),i,".jpg");
        qDebug()<<filename;
        Mat planez=imread(filename,CV_LOAD_IMAGE_GRAYSCALE);
        planez.convertTo(planez,CV_64F,1.0/255.0);
        planez=planez*(graymax[i]-graymin[i])+graymin[i];
        planez=(graybig-planez)/(graybig-graysmall);

        normalize(planez,planez,0,255,NORM_MINMAX);
        imwrite(filename,planez);
    }
}
*/
