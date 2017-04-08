

#ifndef OPENGLWINDOW_H
#define OPENGLWINDOW_H
#define GLUT_DISABLE_ATEXIT_HACK
#include <QGLWidget>
#include <QtOpenGLExtensions/qopenglextensions.h>
#include "gl/gl.h"
#include "gl/glu.h"
#include "gl/glut.h"
#include "position.h"
#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QVector>
#include <QMatrix4x4>


class QComboBox;

class OpenGLView: public QGLWidget
{
    Q_OBJECT
private:
    enum{BUFSIZE=100};
    int num;
    float ax;
    float ay;
    float mx;
    float my;
    float z;
    bool isDown;
    int number;
    int* pointnum;
    int* ipointnum;
    float whRatio;
    bool isInverse;
    position** pos;
    position** ipos;
    double minx;
    double maxx;
    double miny;
    double maxy;
    double minz;
    double maxz;
    int index;

    QOpenGLVertexArrayObject m_vao;
    QOpenGLBuffer m_logoVbo;
    QMatrix4x4 m_proj;
    QMatrix4x4 m_camera;
    QMatrix4x4 m_world;
    void drawScene();
    void reset(position** pos);
public:
    void setInverse(bool);
    void updateIndex(int);
    void setPos(position** _pos,position** _ipos,int _num,int* ,int*);
    QComboBox* attrBox;
    OpenGLView();
protected:
    void initializeGL();
    void resizeGL(int width,int height);
    void paintGL();
    void mousePressEvent(QMouseEvent *e)override;//鼠标单击事件
    void mouseMoveEvent(QMouseEvent *e)override;//鼠标移动事件
    void mouseReleaseEvent(QMouseEvent *e)override;//鼠标释放事件
    void wheelEvent(QWheelEvent *event)override;
};

#endif // OPENGLWINDOW_H

