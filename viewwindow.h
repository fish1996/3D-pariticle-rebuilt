#ifndef VIEWWINDOW_H
#define VIEWWINDOW_H
#include <QVector>
#include <set>
#include <vector>
#include <string>
#include <QScrollArea>
#include <QFileInfoList>
#include "attr.h"
class QPaintEvent;


class viewwindow : public QWidget
{
    Q_OBJECT
private:
    float SIZEH;
    float SIZEW;
    void layout();
    QString path;
    QString oldpath;
    QVector<QString>name;
    QVector<std::string> files;
    std::set<int> chooseImg;
    int index;
    int beginindex;
    int endindex;
    int showindex;
    bool isPress;
    bool isPressCtrl;
    bool isPressShift;
    bool flag;
    int front;
    int count;
    QVector<int>indexVector;
    std::vector<std::string> filename;

    QFileInfoList loadFile(QString);
    QFileInfoList fileList;
public:
    bool isIn;
    bool isEmpty();
    void reset();

    void setBackground(QString path);
    viewwindow(QWidget* parent = 0);
    std::vector<std::string> getFilename();
    std::vector<bool> isRmBackground;

    void setLastImg();
    void setNextImg();
    void setLastPage();
    void setNextPage();
    void clearImg();

protected:
    void paintEvent(QPaintEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void leaveEvent(QEvent* event);
    void enterEvent(QEvent* event);
public:
    void cancelRm();
    void pressCtrl();
    void pressShift();
    void release();
    void updatePath(QString);
public slots:
    void setPath(QString p);
    void updateImg();
    void clear();
    void cancelBackground();
    void chooseAll();
signals:
    void loadImg(QString);
    void choose(int);
};

#endif // VIEWWINDOW_H
