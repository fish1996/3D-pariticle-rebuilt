﻿#ifndef SETUPWINDOW_H
#define SETUPWINDOW_H
#include <QTabWidget>
#include <QDockWidget>
#include <QWidget>
#include "attr.h"
#include "Button.h"
#include "camera.h"
class QPushButton;
class QLabel;
class QLineEdit;
class QSpinBox;
class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QGroupBox;
class QComboBox;

class imgAttr_t{
private:
    int size;
    int sizeW;
    int sizeH;
    QString createdate;
    QString revisedate;
    bool hasBackground;

public:
    void setSize(int s);
    void setWidth(int w);
    void setHeight(int h);
    void setCreateDate(QString d);
    void setReviseDate(QString d);
    void setBackground(bool b);
    int getSize()const;
    int getWidth()const;
    int getHeight()const;
    bool isBackground()const;
    QString getCreateDate()const;
    QString getReviseDate()const;
};

class tabwindow : public QTabWidget
{
    Q_OBJECT
private:
    void layout();
    void cameralayout();
    void filelayout();
    void setConnect();
    enum{
        VMAX = 4,
        HMAX = 7
    };
    imgAttr_t imgAttr;
    Button* playBtn;
    Button* multiPlayBtn;
    QPushButton* choosePathBtn;

    QGroupBox* cameraGroupBox;
    QGroupBox* fileGroupBox;

    QHBoxLayout* hlayout[HMAX];
    QVBoxLayout* vlayout[VMAX];

    QLabel* prefix;
    QLabel* fps;
    QLabel* exposureTime;
    QLabel* cameraPicture;
    QLabel* chooseCamera;
    QLabel* size;

    QLineEdit* pathText;

    QSpinBox* fpsBox;
    QSpinBox* timeBox;

    QWidget* cameraWindow;
    QWidget* fileWindow;

public:
    Button* stopBtn;
    camera* came;
    QComboBox* sizeText;
    QComboBox* prefixText;
    QComboBox* chooseCameraText;
    QString path;
    QString status;
    QLabel* filePicture;
    QLabel* imgMessage;
    void updateTitle();
    tabwindow(QTabWidget* parent = 0);
private slots:
    void setPath();
    void setSize(QString);
    void updateIndex(int in);
};

class setupwindow : public QWidget
{
    Q_OBJECT
private:
    float SIZEW;
    float SIZEH;
    bool isOpen;
    bool isHide;
    enum{
        VMAX = 5,
        HMAX = 5,
    };

    QLabel* zmin;
    QLabel* zmax;
    QLabel* interval;
    QLabel* lamda;
    QLabel* Mag;
    QLabel* dpix;
    QLabel* cm[2];
    QLabel* mm;
    QLabel* um;
    QLabel* nm;

    QVBoxLayout* vlayout[VMAX];
    QHBoxLayout* hlayout[HMAX];
    QGridLayout* glayout;
    QGridLayout* glayout1;
    QDockWidget* dock;
    QWidget* window;

    QString Path;

    void layout();

public:
    QPushButton* oneKeyBtn;
    QPushButton* runBtn;
    QPushButton* detectionBtn;
    QPushButton* locationBtn;
    QPushButton* figplotBtn;
    QLineEdit* zminText;
    QLineEdit* zmaxText;
    QLineEdit* intervalText;
    QLineEdit* lamdaText;
    QLineEdit* MagText;
    QLineEdit* dpixText;
    tabwindow* tabWindow;
    void setPath(QString);
    QString getPath();

    void loadImg(QString path);
    setupwindow(QWidget* parent = 0);
    void paintEvent(QPaintEvent*);
    void closeCamera();
private slots:
    void play();

    void indexChanged(int index);
public slots:
    void findCamera();
};

#endif // SETUPWINDOW_H
