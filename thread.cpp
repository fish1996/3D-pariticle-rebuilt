#include "thread.h"
#include <QPushButton>
#include <QLineEdit>
#include <qDebug>
#include <string>
#include <QMessageBox>
#include <math.h>

threadA::threadA() {}



void threadA::set(double _min,double _max,QString _name,int _size,
         double _interval,int _imgnum,progressbar* _bar,mainwindow* _window)
{
    zmin = _min;
    zmax = _max;
    name = _name;
    size = _size;
    interval = _interval * 0.001;
    imgnum = _imgnum;
    bar = _bar;
    window = _window;
}

void threadA::run()
{
    ext = new extend(size,imgnum,name.toStdString(),zmin,zmax,interval);
    connect(ext,SIGNAL(extendOk()),bar,SLOT(changeState()));
    connect(ext,SIGNAL(extendAll()),window->showWindow,SLOT(initPlot()));
    connect(ext,SIGNAL(extendAll()),this,SLOT(release()));
    ext->start();

}

void threadA::release()
{
   // window->toolWindow->slid->setNum(0,1);
    delete ext;
    emit(Next());
    emit(Next(Extend));
}

void threadB::release()
{
    emit(set(detect->p1xy,detect->p1area,detect->p1box,detect->ip1xy,detect->ip1area,detect->ip1box));
    delete detect;
    emit(Next(Detect));
    emit(Next());

}

void threadB::set(mainwindow* _window,progressbar* _bar,int num)
{

    window = _window;
    blocks = window->msg.detection1;
    gray1 = window->msg.detection2;
    gray2 = window->msg.detection3;
    imgnum = num;
    bar = _bar;
    tempfilename1 = window->msg.path.toStdString();
}

void threadB::run()
{
    detect = new detection();
    connect(detect,SIGNAL(detectOk()),bar,SLOT(changeState()));
    connect(detect,SIGNAL(detectAll()),window->showWindow,SLOT(initDetect()));
    connect(detect,SIGNAL(detectAll()),this,SLOT(release()));

    detect->adaptivethreshold(tempfilename1,blocks,gray1,gray2,imgnum);
}

Mat* threadB::getP1xy()
{
    return detect->p1xy;
}

Mat* threadB::getP1Area()
{
    return detect->p1area;
}

Mat* threadB::getP1Box()
{
    return detect->p1box;
}

void threadC::set(mainwindow* _window,int size,progressbar* _bar)
{
    bar = _bar;
    window = _window;
    tempfilename1=window->msg.path.toStdString();
    boxcoef=1;
    planesumnuber=window->total;
    secnum=window->msg.interval;
    imgnum=size;
}

void threadC::get(Mat* _p1xy,Mat* _p1area,Mat* _p1box,Mat* _ip1xy,Mat* _ip1area,Mat* _ip1box)
{
    p1xy = _p1xy;
    p1area = _p1area;
    p1box = _p1box;
    ip1xy = _ip1xy;
    ip1area = _ip1area;
    ip1box = _ip1box;
}

void threadC::release()
{
    delete Location;
    qDebug()<<"location release";
    emit(Next());
    emit(Next(Locate));
}

void threadC::run(){

    Location = new location();
    qDebug()<<"run herererere";
    connect(Location,SIGNAL(locateOk()),bar,SLOT(changeState()));
    connect(Location,SIGNAL(locateAll(position**,int,int,double**,double*,double*,int*)),
            window->showWindow,SLOT(initLocate(position**,int,int,double**,double*,double*,int*)));
    connect(Location,SIGNAL(locateAll()),this,SLOT(release()));
    Location->set(p1xy,p1area,p1box,ip1xy,ip1area,ip1box);
    Location->p_location(tempfilename1,boxcoef,planesumnuber,secnum,imgnum);
}

thread::thread()
{
    window = new mainwindow();
    window->show();

    state = Null;

    thA = new threadA();
    thB = new threadB();
    thC = new threadC();

  //  window->setupWindow->runBtn->setEnabled(false);
    window->setupWindow->figplotBtn->setEnabled(false);
    window->setupWindow->detectionBtn->setEnabled(false);
    window->setupWindow->locationBtn->setEnabled(false);

    connect(window->setupWindow->runBtn,SIGNAL(clicked()),this,SLOT(handle()));
    connect(window->setupWindow->figplotBtn,SIGNAL(clicked()),this,SLOT(figplot()));
    connect(window->setupWindow->detectionBtn,SIGNAL(clicked()),this,SLOT(detect()));
    connect(window->setupWindow->locationBtn,SIGNAL(clicked()),this,SLOT(locate()));
    connect(window->setupWindow->oneKeyBtn,SIGNAL(clicked()),this,SLOT(oneKey()));
    connect(this,SIGNAL(Next(State)),this,SLOT(changeState(State)));
    connect(thA,SIGNAL(Next(State)),this,SLOT(changeState(State)));
    connect(thB,SIGNAL(Next(State)),this,SLOT(changeState(State)));
    connect(thC,SIGNAL(Next(State)),this,SLOT(changeState(State)));

    connect(thB,SIGNAL(set(Mat*,Mat*,Mat*,Mat*,Mat*,Mat*)),thC,SLOT(get(Mat*,Mat*,Mat*,Mat*,Mat*,Mat*)));
    connect(this,SIGNAL(send(int)),this,SLOT(errorMsg(int)));
}

void thread:: oneKey()
{
    handle();
    window->showWindow->isOneKey = true;
    connect(this,SIGNAL(Next()),this,SLOT(figplot()));
    connect(thA,SIGNAL(Next()),this,SLOT(detect()));
    connect(thB,SIGNAL(Next()),this,SLOT(locate()));
    connect(thC,SIGNAL(Next()),this,SLOT(complete()));
}

void thread::complete()
{
   // window->saveAll("data/user");
    disconnect(this,SIGNAL(Next()),this,SLOT(figplot()));
    disconnect(thA,SIGNAL(Next()),this,SLOT(detect()));
    disconnect(thB,SIGNAL(Next()),this,SLOT(locate()));
    disconnect(thC,SIGNAL(Next()),this,SLOT(complete()));
    window->saveAll("data/user",true);
}

void thread::locate()
{
    bar = new progressbar(QStringLiteral("颗粒定位"));
    bar->total = size*imgnum;
    bar->count = 0;
    bar->display();
    thC->set(window,imgnum,bar);
    thC->start();
}

void thread::detect()
{
    bar = new progressbar(QStringLiteral("颗粒探测"));
    bar->total = 2*imgnum;
    bar->display();
    thB->set(window,bar,imgnum);
    thB->start();
}

void thread::figplot()
{
    bar = new progressbar(QStringLiteral("景深扩展"));
    bar->total = (2*size*imgnum);
    bar->display();
    thA->set(zmin1,zmax1,window->msg.path,size,
             window->setupWindow->intervalText->text().toDouble(),imgnum,bar,window);
    thA->start();
}

void thread::errorMsg(int e)
{
    switch(e){
    case NOPICTURE:
        QMessageBox::critical(NULL, "Error", QStringLiteral("请先导入图片"),
                              QMessageBox::Yes);
        break;
    case NOPATH:
        QMessageBox::critical(NULL, "Error", QStringLiteral("请先设置当前路径"),
                              QMessageBox::Yes);
        break;
    case OPENCVERR:
        QMessageBox::critical(NULL, "OpenCV Error", QStringLiteral("图片的颜色通道数目错误\n"),
                              QMessageBox::Yes);
        break;
    case NOCOMPLETE:
        QMessageBox::critical(NULL, "Error", QStringLiteral("请填写完整的参数！"),
                              QMessageBox::Yes);
        break;
    }
}

int thread::round(float x)
{
    return (int)(x+0.5);
}

void thread::handle()
{
    if((window->msg).path.isEmpty()) {
        emit(send(NOPATH));
        return;
    }
    else{
        tempfilename = (window->msg).path.toStdString();
    }

    if((window->name).isEmpty() && window->viewWindow->isEmpty()) {
        emit(send(NOPICTURE));
        return;
    }

    if(window->viewWindow->isEmpty()) {
        filename.push_back((window->name.toStdString()));
    }
    else {
        filename = window->viewWindow->getFilename();
    }

    if(window->setupWindow->zminText->text() == "" ||
       window->setupWindow->zmaxText->text() == "" ||
       window->setupWindow->lamdaText->text() == "" ||
       window->setupWindow->dpixText->text() == "" ||
       window->setupWindow->intervalText->text() == "") {
        emit(send(NOCOMPLETE));
        return;
    }
    zmin1=window->setupWindow->zminText->text().toDouble();
    zmin1=zmin1*pow(10.0,-2);
    zmax1=window->setupWindow->zmaxText->text().toDouble();
    zmax1=zmax1*pow(10.0,-2);
    lamda1=window->setupWindow->lamdaText->text().toDouble();
    lamda1=lamda1*pow(10.0,-9);
    pixelsize=window->setupWindow->dpixText->text().toDouble();
    pixelsize=pixelsize*pow(10.0,-6);
    interval=window->setupWindow->intervalText->text().toDouble();
    interval=interval*pow(10.0,-3);

    bar = new progressbar(QStringLiteral("全息重建"));

    window->count = bar->count = 0;
    imgnum = filename.size();
    bar->total = ((int)((((zmax1-zmin1)/interval))+1))*filename.size();
    size = window->totalnum = window->total= (((zmax1-zmin1)/interval)) + 1;
    window->imgnum = filename.size();
    window->showWindow->setTotalNum(((zmax1-zmin1)/interval)+1);

    window->toolWindow->slid->setNum(0,(zmax1-zmin1)/interval + 1);
    window->showWindow
            ->setWavelength(window->setupWindow->dpixText->text().toDouble());
    window->showWindow->setPixelSize(window->setupWindow->dpixText->text().toDouble());

    this->start();
}

void thread::release()
{
    delete reb;
    emit(Next());
    emit(Next(Rebuilt));
}

void thread::changeState(State s)
{
    state = s;
    switch(state){
    case Null:
        break;
    case Rebuilt:
        window->setupWindow->figplotBtn->setEnabled(true);
        window->setupWindow->locationBtn->setEnabled(false);
        window->setupWindow->detectionBtn->setEnabled(false);
        break;
    case Extend:
        window->setupWindow->detectionBtn->setEnabled(true);
        break;
    case Detect:
        window->setupWindow->locationBtn->setEnabled(true);
        break;
    case Locate:
        window->setupWindow->runBtn->setEnabled(true);
        break;
    }
}

void thread::change()
{
    bar->changeState();
}

void thread::barClose()
{
    reb->closeflag = true;
}

void thread::run()
{
    reb = new reBuilt(tempfilename, filename, zmin1, zmax1, lamda1, pixelsize, interval,bar); //调用rebuilt类*
    connect(reb,SIGNAL(finish()),this,SLOT(change()));
    connect(reb,SIGNAL(ok()),bar,SLOT(display()));
    connect(bar,SIGNAL(isClose()),this,SLOT(barClose()));
    connect(reb,SIGNAL(allfinish(int)),window->showWindow,SLOT(setNum(int)));
    connect(reb,SIGNAL(allfinish(int)),window->showWindow,SLOT(initIndex(int)));
    connect(reb,SIGNAL(allfinish(int)),this,SLOT(release()));
    reb->start();
}

