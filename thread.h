#ifndef THREAD_H
#define THREAD_H

#include "mainwindow.h"
#include "rebuilt.h"
#include "extend.h"
#include "progressbar.h"
#include "locate.h"
#include "detect.h"
#include <QThread>
#include <vector>
#include <string>

typedef enum{
    Null,
    Rebuilt,
    Extend,
    Detect,
    Locate,
}State;

class threadA:public QThread
{
    Q_OBJECT
private:
    double zmin;
    double zmax;
    double interval;
    QString name;
    extend* ext;
    progressbar* bar;
    mainwindow* window;
    //Engine* ep;
    int size;
protected:
    void run()override;
public:
    void set(QString _name,double _zmin, double _zmax,
             double _interval,int size,progressbar*,mainwindow* _window);
    threadA();
private slots:
    void release();
signals:
    void Next();
    void Next(State);
};

class threadB : public QThread
{
    Q_OBJECT
private:
    detection* detect;
    progressbar* bar;
    double blocks;
    double gray1;
    double gray2;
    mainwindow* window;
    int imgnum;
    string tempfilename1;
    //Engine* ep;
private slots:
    void release();
public:
    Mat* getP1xy();
    Mat* getP1Area();
    Mat* getP1Box();
    void set(mainwindow* window,progressbar* bar,int num);
protected:
    void run();
signals:
    void set(Mat*,Mat*,Mat*,Mat*,Mat*,Mat*);
    void Next();
    void Next(State);
};


class threadC : public QThread
{
    Q_OBJECT
private:
    mainwindow* window;
    location* Location;
    progressbar* bar;
    Mat* p1xy;
    Mat* p1area;
    Mat* p1box;
    Mat* ip1xy;
    Mat* ip1area;
    Mat* ip1box;
    string tempfilename1;
    double boxcoef;
    int planesumnuber;
    int secnum;
    int imgnum;
public:
     void set(mainwindow*,int,progressbar*);
public slots:
    void get(Mat* _p1xy,Mat* _p1area,Mat* _p1box,Mat* _ip1xy,Mat* _ip1area,Mat* _ip1box);
protected:
    void run()override;
private slots:
    void release();
signals:
    void Next();
    void Next(State);

};

class thread:public QThread
{
    Q_OBJECT
private:
  //  Engine* ep;
    int round(float x);
    enum{NOPICTURE,NOPATH,NOCOMPLETE,OPENCVERR};
    State state;
    reBuilt* reb;
    mainwindow* window;
    threadA* thA;
    threadB* thB;
    threadC* thC;
    progressbar* bar;
    double zmin1;
    double zmax1;
    double lamda1;
    double pixelsize;
    double interval;
    std::string tempfilename;
    std::vector<std::string> filename;
    //Mat image1;

public:
    thread();
protected:
    void run();
private slots:
    void oneKey();
    void detect();
    void handle();
    void release();
    void change();
    void errorMsg(int);
    void barClose();
    void figplot();
    void locate();
    void changeState(State);
    void complete();
signals:
    void Next();
    void Next(State);
    void send(int);
};


#endif // THREAD_H
