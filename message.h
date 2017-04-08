#ifndef MESSAGE_H
#define MESSAGE_H

struct message_t{
    double detection1;
    double detection2;
    double detection3;
    double minRadius;
    double maxRadius;
    QString path;
    int interval;
    bool isInverse;
    void set(double d1,double d2,double d3,double minr,double maxr,
             QString p,int in,bool is);
};

#endif // MESSAGE_H
