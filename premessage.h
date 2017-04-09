#ifndef PREMESSAGE_H
#define PREMESSAGE_H

#include <QString>
#include <QMap>

struct premessage_t{
    QString zmin;
    QString zmax;
    QString interval;
    QString lamda;
    QString Mag;
    QString dpix;
    QString detection1;
    QString detection2;
    QString detection3;
    QString location;
    QString path;
    QString minRadius;
    QString maxRadius;
    QString plotnum;
 //   bool isInverse;
};

typedef QMap<QString,premessage_t> map_t;

#endif // PREMESSAGE_H
