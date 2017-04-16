#include "showwindow.h"
#include "tablewindow.h"
#include "plotwindow.h"
#include <math.h>

#include <QComboBox>
#include <QPaintEvent>
#include <QPainter>
#include <QPixmap>
#include <qDebug>
#include <QMenu>
#include <QAction>
#include <QFileDialog>

bool showwindow::isShow = false;

void picture::closeEvent(QCloseEvent*)
{
    showwindow::isShow = false;
}

void framewindow::setState(int s){
    state = s;
}

framewindow::framewindow(basewindow* p)
    :basewindow(p)
{
    imgIndex = 0;
    index = -1;
    SIZEH = new int();
    SIZEW = new int();
    *SIZEW = rect().width();
    *SIZEH = rect().height();
    path = "";
    showWindow = new showwindow(this,SIZEW,SIZEH);
    attrBox = new QComboBox(this);
    attrBox->addItem(QStringLiteral("重建结果"));
    attrBox->addItem(QStringLiteral("3D视图"));
    attrBox->addItem(QStringLiteral("景深扩展"));
    attrBox->addItem(QStringLiteral("颗粒探测"));

    menu = new QMenu();

    saveAction = new QAction(this);
    saveAction->setText(QStringLiteral("保存为图片"));
    sca = new scale(this);

    connect(saveAction,SIGNAL(triggered()),this,SLOT(save()));
    connect(this,SIGNAL(send(float)),sca,SLOT(updateScale(float)));
    connect(showWindow,SIGNAL(send(float)),sca,SLOT(updateScale(float)));
}

bool framewindow::isShowWhite()
{
    return sca->isWhite;
}

void framewindow::isShowWhite(bool is)
{
    sca->isShowWhite(is);
}

void framewindow::addItem(int index)
{

}

void framewindow::setFigIndex(int i)
{
    index = i;
    showWindow->loadImg(path + "/img_fuse" + QString::number(i) + ".bmp");
}

void framewindow::setDetectIndex(int i)
{
    index = i;
    showWindow->loadImg(path + "/img_binaryzation" + QString::number(i) + ".jpg");
}

QComboBox* framewindow::getBox()
{
    return attrBox;
}

void framewindow::save()
{
    QString filename = QFileDialog::getSaveFileName(this, tr("保存图片"),QDir::currentPath(), tr("Images (*.png);; Image(*.bmp);; Image(*.jpg);"));
    if(filename.isEmpty()) {
        return;
    }
    else {
        QPixmap pixmap;
        switch(state){
        case 0:
            if(*isInverse){
                pixmap.load(path + "/temp_plane_" + QString::number(index)+"i.jpg");
            }
            else {
                pixmap.load(path + "/temp_plane_" + QString::number(index)+".jpg");
            }
            break;
        case 1:
            if(*isInverse){
                pixmap.load(path + "/img_fuse" + QString::number(index)+"i.bmp");
            }
            else {
                pixmap.load(path + "/img_fuse_" + QString::number(index)+".bmp");
            }
            break;
        case 2:
            if(*isInverse){
                pixmap.load(path + "/img_binaryzation_" + QString::number(index)+"i.jpg");
            }
            else {
                pixmap.load(path + "/img_binaryzation_" + QString::number(index)+".jpg");
            }
            break;
        default:
            return;
        }
        QPainter paint(&pixmap);
        paint.setPen(QColor(255,255,255));
        if(showWindow->isCalculate){
            float ratio = showWindow->ratio;
            paint.drawLine(showWindow->beginx/ratio,showWindow->beginy/ratio,
                           showWindow->endx/ratio,showWindow->endy/ratio);
            float length = sqrt((showWindow->beginx-showWindow->endx)
                                *(showWindow->beginx-showWindow->endx)
                                +(showWindow->beginy-showWindow->endy)
                                *(showWindow->beginy-showWindow->endy));
            paint.setFont(QFont("Arial",20));
            paint.drawText((int)((showWindow->beginx/ratio+showWindow->endx/ratio)/2)-10,
                           (int)((showWindow->beginy/ratio+showWindow->endy/ratio)/2)-10,
                           QString::number(length*showWindow->pixelSize
                                           /showWindow->ratio)+"um");
        }
        pixmap.save(filename);
    }
}

void framewindow::setPath(QString p)
{
  //  qDebug()<<"frame path="<<path;
    path = p;
}

void framewindow::zoomIn()
{
    showWindow->zoomIn();
}

void framewindow::zoomOut()
{
    showWindow->zoomOut();
}

void framewindow::original()
{
    showWindow->original();
}

void framewindow::fullScreen()
{
    showWindow->fullScreen();
}

scale* framewindow::getSca()
{
    return sca;
}

void framewindow::setCalculate(bool is)
{
    showWindow->isCalculate = is;
}

void framewindow::reset()
{
    showWindow->reset();
}

bool framewindow::getCalculate()
{
    if(showWindow->beginx!=0 && showWindow->beginy !=0 &&
            showWindow->endx !=0 && showWindow->endy!=0){
        showWindow->update();
    }
    return showWindow->isCalculate;
}

void framewindow::setIndex(int i)
{
    if(index == -1) {
        showWindow->path == "";
        showWindow->update();
        return;
    }
    imgIndex = i;
    setNum(1);
}

void framewindow::setNum(int n)
{
    index = n - 1;
    showWindow->loadImg(path + "/temp_plane_" + QString::number(imgIndex*total + n - 1)+".jpg");
}

void framewindow::paintEvent(QPaintEvent* e)
{
    QPainter paint(this);
    *SIZEW = rect().width();
    *SIZEH = rect().height();
  //  qDebug()<<"framewindow"<<*SIZEW<<*SIZEH;
  //  paint.setPen(QColor(100,100,100));
  //  paint.setBrush(QColor(222,222,222));
  //  paint.drawRect(rect());
    //qDebug()<<"paint parent";


    paint.setPen(Qt::NoPen);
    int margin = 0;
    const int size = 10;
    for(int i=0;i<rect().width()/size;i++){
        for(int j=0;j<rect().height()/size;j++){
            paint.setBrush(QColor(222,222,222));
            paint.drawRect(size*i+margin, size*j+margin, size/2, size/2);
            paint.drawRect(size*i + size/2+margin, size*j+size/2+margin, size/2, size/2);
            paint.setBrush(QColor(255,255,255));
            paint.drawRect(size*i+size/2+margin, size*j+margin, size/2, size/2);
            paint.drawRect(size*i+margin, size*j+size/2+margin, size/2, size/2);
        }
    }
}

showwindow* framewindow::getshowwindow()
{
    return showWindow;
}

void framewindow::contextMenuEvent(QContextMenuEvent* event)
{
    if(index == -1) {
        return;
    }
    menu->clear();
    menu->addAction(saveAction);

    menu->exec(QCursor::pos());
    event->accept();
}

void framewindow::setPixelSize(double p)
{
    showWindow->setPixelSize(p);
}

void framewindow::setTotalNum(int t)
{
    total = t;
}

bool framewindow::isDrawWhite()
{
    return showWindow->isDrawWhite();
}

void framewindow::isDrawWhite(bool is)
{
    showWindow->isDrawWhite(is);
}

void framewindow::setI(bool* is)
{
    isInverse = is;
    showWindow->setI(is);
}

void framewindow::reloadImg()
{
    showWindow->reloadImg();
}

QString framewindow::getPath()
{
    return showWindow->path;
}

void framewindow::wheelEvent(QWheelEvent *event)
{
    if(showWindow->isCalculate)return;
    if(path=="")return;
   // qDebug()<<"WheelEvent";
    int &w = showWindow->w;
    int &h = showWindow->h;
    int numDegrees = event->delta()/8;
    int numSteps=numDegrees/15;
    int prew = w;
    int preh = h;
    if(w > 100 && h > 100 && numSteps<0 || numSteps>0 && w<*SIZEW*10 && h<*SIZEH*10){
        w=w*(1+numSteps*0.15);
    }
    h = 1.0*w/showWindow->original_w * showWindow->original_h;
    int x,y;
    x = showWindow->frameGeometry().topLeft().x();
    y = showWindow->frameGeometry().topLeft().y();

    showWindow->move(x+(prew-w)/2,y+(preh-h)/2);
    showWindow->ratio = 1.0*w/showWindow->width;
    showWindow->beginx *= 1.0*w/prew;
    showWindow->beginy *= 1.0*h/preh;
    showWindow->endx *= 1.0*w/prew;
    showWindow->endy *= 1.0*h/preh;
    showWindow->ishold = true;
    showWindow->resize(w,h);

    emit(send(showWindow->ratio));
}

showwindow::showwindow(basewindow* p,int * w,int* h)
    :basewindow(p),SIZEW(w),SIZEH(h)
{
    parent = p;
    layout();
}

void showwindow::reset()
{
    beginx = endx = -1;
    beginy = endy = -1;
}

float showwindow::getRatio()
{
    return ratio;
}

void showwindow::layout()
{
    beginx = beginy = endx = endy;
    isCalculate = false;
    ishold = true;
    drawWhite = true;
    path = "";
}

void showwindow::loadImg(QString p)
{
    //qDebug() << "loadimg " << p;
    path = p;
    if(*isInverse){
        //qDebug()<<"yes";
        pix.load(p.mid(0,p.size()-4) + "i"+p.mid(p.size()-4,4));
    }
    else pix.load(p);
    w = pix.width();
    h = pix.height();
    if(w==0 && h==0){
        path = "";
    }

    width = w;
    height = h;
    float r = 1.0 * w/h;
    if(1.0*w/h > 1.0* (*SIZEW)/(*SIZEH)){
        w = *SIZEW;
        h = 1.0*w/r;
    }
    else{
        h = *SIZEH;
        w = 1.0*h*r;
    }
    original_w = w;
    original_h = h;

    ratio = 1.0*w/width;

    move(0,0);
    resize(w,h);

    update();
    emit(send(ratio));
}

void showwindow::isDrawWhite(bool is)
{
    drawWhite = is;
    update();
}

void showwindow::setI(bool* is)
{
    isInverse = is;
}

bool showwindow::isDrawWhite()
{
    return drawWhite;
}

void showwindow::paintEvent(QPaintEvent* )
{
    //qDebug()<<"paint children";
    if(path=="")return;
    QPainter paint(this);
    if(drawWhite)
        paint.setPen(QColor(255,255,255));
    else
        paint.setPen(QColor(0,0,0));
    float length;
    static float len;
   // bool flag = false;
    if(path!="")paint.drawPixmap(0,0,w,h,pix);
    if(isCalculate){
        paint.drawLine(beginx,beginy,endx,endy);
        length = sqrt((beginx-endx)*(beginx-endx) + (beginy-endy)*(beginy-endy));
        if(ishold) {

            paint.drawText((int)((beginx+endx)/2)-10,
                   (int)((beginy+endy)/2)-10,
                   QString::number(len)+"um");
            ishold = false;

        }
        else {
            length = sqrt((beginx-endx)*(beginx-endx) + (beginy-endy)*(beginy-endy));
            len = length*pixelSize/ratio;
            if(length) {
                paint.drawText((int)((beginx+endx)/2)-10,
                       (int)((beginy+endy)/2)-10,
                       QString::number(len)+"um");
            }

        }
    }


}

showtabwindow::showtabwindow(QTabWidget* parent)
    :QTabWidget(parent)
{
    layout();
}

void showwindow::setPixelSize(float s){
    pixelSize = s;
}

void showtabwindow::addItem(int index)
{
    frameWindow->addItem(index);
}

QString showtabwindow::getPath()
{
    return frameWindow->getPath();
}

void showtabwindow::setTotalNum(int total)
{
    frameWindow->setTotalNum(total);
}

void showtabwindow::setPlotNum(int n)
{
    countwindow->setPlotNum(n);
}

void showtabwindow::layout()
{
    prefix = "";
    isOneKey = false;
    count = 0;
    Index = 0;
    isCalculate = false;
    frameWindow = new framewindow();
    countwindow = new plotwindow();
    figWindow = new framewindow();
    detectWindow = new framewindow();
    openglWindow = new OpenGLView();

    frameWindow->setState(0);
    figWindow->setState(1);
    detectWindow->setState(2);


    addTab(frameWindow,QStringLiteral("视图"));
    addTab(countwindow,QStringLiteral("统计"));

    connect(frameWindow->attrBox,SIGNAL(currentIndexChanged(int)),
            this,SLOT(indexChanged(int)));
    connect(openglWindow->attrBox,SIGNAL(currentIndexChanged(int)),
            this,SLOT(indexChanged(int)));
    connect(figWindow->attrBox,SIGNAL(currentIndexChanged(int)),
            this,SLOT(indexChanged(int)));
    connect(detectWindow->attrBox,SIGNAL(currentIndexChanged(int)),
            this,SLOT(indexChanged(int)));
}

void showtabwindow::setCalculate(bool is)
{
    frameWindow->setCalculate(is);
    figWindow->setCalculate(is);
    detectWindow->setCalculate(is);
}

bool showtabwindow::getCalculate(){

    if(Index == 0) {
        return frameWindow->getCalculate();
    }
    else if(Index == 2){
        return figWindow->getCalculate();
    }
    else if(Index==3){
        return detectWindow->getCalculate();
    }

}

void showtabwindow::indexChanged(int index)
{
    Index = index;
    if(index == 0) {
        removeTab(0);
        removeTab(1);
        addTab(frameWindow,QStringLiteral("视图"));
        addTab(countwindow,QStringLiteral("统计"));
        frameWindow->attrBox->setCurrentIndex(0);
    }
    else if(index == 1) {
        removeTab(0);
        removeTab(1);
        addTab(openglWindow,QStringLiteral("视图"));
        addTab(countwindow,QStringLiteral("统计"));
        openglWindow->attrBox->setCurrentIndex(1);

    }
    else if(index == 2){
        removeTab(0);
        removeTab(1);
        addTab(figWindow,QStringLiteral("视图"));
        addTab(countwindow,QStringLiteral("统计"));
        figWindow->attrBox->setCurrentIndex(2);
    }
    else if(index == 3) {
        removeTab(0);
        removeTab(1);
        addTab(detectWindow,QStringLiteral("视图"));
        addTab(countwindow,QStringLiteral("统计"));
        detectWindow->attrBox->setCurrentIndex(3);
    }
}


void showtabwindow::setPixelSize(double p)
{
    frameWindow->setPixelSize(p);
    figWindow->setPixelSize(p);
    detectWindow->setPixelSize(p);
}

void showtabwindow::reset()
{
    frameWindow->reset();
}

void showtabwindow::setPath(QString p)
{
    frameWindow->setPath(p);
    figWindow->setPath(p);
    detectWindow->setPath(p);
}

void showwindow::reloadImg()
{
    loadImg(path);
}

void showwindow::mouseMoveEvent(QMouseEvent *event)
{
    if(path=="")return;
    if (event->buttons()==Qt::LeftButton) {
        int x = event->pos().x();
        int y = event->pos().y();
        if(isCalculate){
            endx = x;
            endy = y;
        }
        else {
            move(event->globalPos()-point);
            event->accept();
        }
        update();
    }
}

void showwindow::mousePressEvent(QMouseEvent *event)
{
    if(path=="")return;
    if(event->button() == Qt::LeftButton) {
        int x = event->pos().x();
        int y = event->pos().y();

        beginx = x;
        beginy = y;

        if (event->button()==Qt::LeftButton) {
            point=event->globalPos()-frameGeometry().topLeft();
            event->accept();
        }
    }
}

void showwindow::zoomIn()
{
    if(isCalculate)return;
    if(path == "")return;
    int prew = w;
    int preh = h;
    if(w<(*SIZEW)*10 && h<(*SIZEH)*10){
        w=w*1.05;
        h=h*1.05;
    }
    int x,y;
    x = frameGeometry().topLeft().x();
    y = frameGeometry().topLeft().y();

    move(x+(prew-w)/2,y+(preh-h)/2);
    ratio = 1.0*w/width;
    beginx *= 1.0*w/prew;
    beginy *= 1.0*h/preh;
    endx *= 1.0*w/prew;
    endy *= 1.0*h/preh;
    ishold = true;
    resize(w,h);

    emit(send(ratio));
}

void showwindow::zoomOut()
{
    if(isCalculate)return;
    if(path == "")return;
    int prew = w;
    int preh = h;
    if(w>100 && h>100){
        w=w*0.95;
        h=h*0.95;
    }
    int x,y;
    x = frameGeometry().topLeft().x();
    y = frameGeometry().topLeft().y();

    move(x+(prew-w)/2,y+(preh-h)/2);
    ratio = 1.0*w/width;
    beginx *= 1.0*w/prew;
    beginy *= 1.0*h/preh;
    endx *= 1.0*w/prew;
    endy *= 1.0*h/preh;
    ishold = true;
    resize(w,h);

    emit(send(ratio));
}

void showwindow::original()
{
    //parent->update();
    qDebug()<<"original"<<*SIZEW<<*SIZEH;
    if(isCalculate)return;
    int prew = w;
    int preh = h;
    float r = 1.0 * original_w/original_h;
    if(1.0*original_w/original_h > 1.0* (*SIZEW)/(*SIZEH)){
        w = *SIZEW;
        h = 1.0*w/r;
    }
    else{
        h = *SIZEH;
        w = 1.0*h*r;
    }
    move(0,0);
    ishold = true;
    resize(w,h);
    beginx *= 1.0*w/prew;
    beginy *= 1.0*h/preh;
    endx *= 1.0*w/prew;
    endy *= 1.0*h/preh;
    ratio = 1.0f*w/width;
    emit(send(ratio));
}

void showwindow::fullScreen()
{
    if(isShow)return;
    isShow = true;
    pic = new picture();
    pic->setPixmap(pix);
    pic->show();
}

void showtabwindow::zoomIn()
{

    if(Index==0){
        frameWindow->zoomIn();
    }
    else if(Index==2){
        figWindow->zoomIn();
    }
    else if(Index == 3){
        detectWindow->zoomIn();
    }
}

void showtabwindow::initPlot()
{
    indexChanged(2);
    figWindow->setFigIndex(0);

}


void showtabwindow::zoomOut()
{

    if(Index==0){
        frameWindow->zoomOut();
    }
    else if(Index==2){
        figWindow->zoomOut();
    }
    else if(Index==3){
        detectWindow->zoomOut();
    }
}

void showtabwindow::original()
{

    if(Index==0){
        frameWindow->original();
    }
    else if(Index==2){
        figWindow->original();
    }
    else if(Index == 3){
        detectWindow->original();
    }
}

void showtabwindow::fullScreen()
{

    if(Index==0){
        frameWindow->fullScreen();
    }
    else if(Index==2){
        figWindow->fullScreen();
    }
    else if(Index==3){
        detectWindow->fullScreen();
    }
}

void showtabwindow::setI(bool* is)
{
    frameWindow->setI(is);
    figWindow->setI(is);
    detectWindow->setI(is);
}

void showtabwindow::setInverse(bool is)
{
    countwindow->setInverse(is);
    openglWindow->setInverse(is);
}

void showtabwindow::setNum(int n)
{
    num = n;

    if(Index==0){
        frameWindow->setNum(n);
    }
}

void showtabwindow::initDetect()
{
    indexChanged(3);
    detectWindow->setDetectIndex(0);
}

void showtabwindow::setPrefix(QString p)
{
    prefix = p;
}

void showtabwindow::initLocate(position** pos,position** ipos,int num,int size,
                                                  double**diameterfre,double** idiameterfre,
                                                  double*diametermin,double*idiametermin,
                                                  double*diametermax,double*idiametermax,
                                                  int* pointnum,int * ipointnum)
{
    openglWindow->setPos(pos,ipos,num,pointnum,ipointnum);
    countwindow->isDraw = true;
    countwindow->setAttr(diameterfre,idiameterfre,diametermin,idiametermin,
                         diametermax,idiametermax,num,pos,ipos,
                         pointnum,ipointnum);
    if(isOneKey){
        isOneKey = false;
        for(int i = 0;i < num;i++) {
            save(i,"data/user/"+prefix+"location" + QString::number(i)+".xls","data/user/"+prefix+"location"+QString::number(i)+".jpg",false);
            save(i,"data/user/"+prefix+"ilocation" + QString::number(i)+".xls","data/user/"+prefix+"ilocation"+QString::number(i)+".jpg",true);
        }
    }
     indexChanged(1);
}

void showtabwindow::setIndex(int index)
{
    if(currentIndex()==1){
        countwindow->setIndex(index);
        return;
    }
    if(Index == 0) {
        frameWindow->setIndex(index);
    }
    else if(Index == 1){
        openglWindow->updateIndex(index);
    }
    else if(Index == 2) {
        figWindow->setIndex(index);
        figWindow->setFigIndex(index);
    }
    else if(Index == 3){
        detectWindow->setIndex(index);
        detectWindow->setDetectIndex(index);
    }
}

void showtabwindow::paintEvent(QPaintEvent *)
{
    QPainter paint(this);
    paint.setPen(QColor(255,255,255));
    paint.setBrush(QColor(255,255,255));
    paint.drawRect(rect());
}

scale* showtabwindow::getSca(){
    if(Index == 0) {
        return frameWindow->getSca();
    }
    else if(Index == 2) {
        return figWindow->getSca();
    }
    else if(Index == 3){
        return detectWindow->getSca();
    }
    return nullptr;
}

void showtabwindow::initIndex(int index)
{
    indexChanged(0);
    frameWindow->setIndex(index - 1);
    figWindow->setIndex(index - 1);
    detectWindow->setIndex(index - 1);
}

void showtabwindow::reloadImg(){
    frameWindow->reloadImg();
    figWindow->reloadImg();
    detectWindow->reloadImg();
}

void showtabwindow::setWavelength(double w)
{
    frameWindow->getSca()->setWavelength(w);
    figWindow->getSca()->setWavelength(w);
    detectWindow->getSca()->setWavelength(w);
}

void showtabwindow::isShowWhite(bool is)
{
    frameWindow->isShowWhite(is);
}

bool showtabwindow::isShowWhite()
{
    return frameWindow->isShowWhite();
}

void showtabwindow::save(int index,QString name1,QString name2,bool is){
    countwindow->saveExcel(name1,index,is);
    countwindow->saveImg(name2,index,is);
}

void showtabwindow::isDrawWhite(bool is)
{
    frameWindow->isDrawWhite(is);
}

bool showtabwindow::isDrawWhite()
{
    return frameWindow->isDrawWhite();
}

void showtabwindow::setRadius(double* min,double* max)
{
    countwindow->setRadius(min,max);
}
