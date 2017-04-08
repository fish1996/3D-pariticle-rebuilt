/*
#include "mainwidget.h"
#include <QMouseEvent>
#include <math.h>

MainWidget::MainWidget(QWidget *parent) :
    QOpenGLWidget(parent),
    geometries(0),
    angularSpeed(20)
{
    setMouseTracking(true);
    attrBox = new QComboBox(this);
    attrBox->addItem("重建结果");
    attrBox->addItem("3D视图");
    attrBox->setCurrentIndex(1);
    isDown = false;
    z = -5.0f;
    ay = 45;
}

MainWidget::~MainWidget()
{
    makeCurrent();
    delete geometries;
    doneCurrent();
}

void MainWidget::mousePressEvent(QMouseEvent *e)
{
    isDown = true;
    float x = e->x();
    float y = e->y();
    mx = x;
    my = y;
}

void MainWidget::mouseMoveEvent(QMouseEvent *e)
{
    if(isDown){
        float x = e->x();
        float y = e->y();
        ax += (y-my)/10.0f;
        ay += (x-mx)/10.0f;
        mx = x;
        my = y;
        update();
    }
}

void MainWidget::mouseReleaseEvent(QMouseEvent *)
{
    isDown = false;
}

void MainWidget::timerEvent(QTimerEvent *)
{

}

void MainWidget::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor(0, 0, 0, 1);
    initShaders();
    glEnable(GL_DEPTH_TEST);
   // glEnable(GL_CULL_FACE);
    geometries = new GeometryEngine;
    timer.start(12, this);
}

void MainWidget::initShaders()
{
    if (!program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/image/vshader.glsl"))
        close();
  //  if (!program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/image/vline.glsl"))
 //       close();
    if (!program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/image/fshader.glsl"))
        close();
    if (!program.link())
        close();
    if (!program.bind())
        close();
}

void MainWidget::resizeGL(int w, int h)
{
    qreal aspect = qreal(w) / qreal(h ? h : 1);
    const qreal zNear = 0.1, zFar = 100.0, fov = 45.0;
    projection.setToIdentity();
    projection.perspective(fov, aspect, zNear, zFar);
}

void MainWidget::wheelEvent(QWheelEvent *event)
{
    int numDegrees = event->delta()/8;
    int numSteps=numDegrees/15;
    z += numSteps;
    update();
}

QQuaternion MainWidget::quater(float x,float y,float z)
{
    return QQuaternion(cos(x/2)*cos(y/2)*cos(z/2)+
                       sin(x/2)*sin(y/2)*sin(z/2),
                       sin(x/2)*cos(y/2)*cos(z/2)-
                       cos(x/2)*sin(y/2)*sin(z/2),
                       cos(x/2)*sin(y/2)*cos(z/2)+
                       sin(x/2)*cos(y/2)*sin(z/2),
                       cos(x/2)*cos(y/2)*sin(z/2)-
                       sin(x/2)*sin(y/2)*cos(z/2));
}

void MainWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    QMatrix4x4 matrix;
    matrix.translate(0.0, 0.0, z);
    matrix.rotate(quater(-ax,ay,0));


    program.setUniformValue("mvp_matrix", projection * matrix);
    geometries->drawCubeGeometry(&program);
}
*/
