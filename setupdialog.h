﻿#ifndef SETUPDIALOG_H
#define SETUPDIALOG_H
#include "premessage.h"
#include "message.h"
#include <QWidget>

class QPushButton;
class QLineEdit;
class QCheckBox;
class QLabel;
class QSpinBox;
class QHBoxLayout;
class QVBoxLayout;
class QComboBox;
class addattrwindow;
class removeattrwindow;

class setupdialog : public QWidget
{
    Q_OBJECT
private:
    enum{
        VMAX = 1,
        HMAX = 7,
    };
    int sizex;
    int sizey;

    QPushButton* chooseDir;
    QPushButton* addAttrBtn;
    QPushButton* removeAttrBtn;

    QLabel* detection;
    QLabel* fileDir;
    QLabel* interval;
    QLabel* minRadius;
    QLabel* maxRadius;
    QLabel* scaleLength;
    QLabel* place;
    QLabel* place_x;
    QLabel* place_y;
    QLabel* um;
    message_t* message;

    QLabel* preAttr;

    double dpix;
    QHBoxLayout* hlayout[HMAX];
    QVBoxLayout* vlayout[VMAX];

    addattrwindow* addattrWindow;
    removeattrwindow* removeattrWindow;
    map_t* map;
    void layout();

    void setConnect();
public:

    bool isChange;
    static bool isCreate;
    QString preattr;
    QComboBox* attrBox;

    QPushButton* okBtn;
    QPushButton* cancelBtn;
    QString attrName;
    QLineEdit* line1;
    QLineEdit* line2;
    QLineEdit* line3;
    QLineEdit* fileDirText;

    QSpinBox* intervalText;
    QSpinBox* scaleLengthText;
    QSpinBox* minRadiusText;
    QSpinBox* maxRadiusText;
    QSpinBox* placeXText;
    QSpinBox* placeYText;
    QVector<QString>* namelist;
    setupdialog(int sizex,int sizey,QString preattr,QVector<QString>* namelist,map_t* map,double size,message_t* msg,QWidget* parent = 0);
    ~setupdialog();
protected:
    void closeEvent(QCloseEvent* event);
private slots:
    void Add();
    void Remove();
    void setValue(int i);
    void choose();
public slots:
    void updateAttr();
    void updateAttr(QString,premessage_t);
};

#endif // SETUPDIALOG_H
