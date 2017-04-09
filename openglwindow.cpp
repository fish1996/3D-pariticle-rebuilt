
#include "openglwindow.h"
#include <QWheelEvent>
#include <qDebug>
#include <QComboBox>
OpenGLView::OpenGLView()
{
    pos = nullptr;
    ipos = nullptr;
    number = 0;
    pointnum = 0;
    ipointnum = 0;
    index = 0;
    isInverse = true;

    attrBox = new QComboBox(this);
    attrBox->addItem(QStringLiteral("重建结果"),0);
    attrBox->addItem(QStringLiteral("3D视图"),1);
    attrBox->addItem(QStringLiteral("景深扩展"),2);
    attrBox->addItem(QStringLiteral("颗粒探测"),3);
    attrBox->setCurrentIndex(1);qDebug()<<"OpenGLView";

}


void OpenGLView::setPos(position** _pos,position** _ipos,int _num,int * _size,int* _isize)
{
    if(pos!=nullptr) {
        for(int i=0;i<number;i++){
            delete[] pos[i];
        }
        delete[] pointnum;
        delete[] pos;
    }
    if(ipos!=nullptr) {
        for(int i=0;i<number;i++){
            delete[] ipos[i];
        }
        delete[] ipointnum;
        delete[] ipos;
    }
    ipos = _ipos;
    pos = _pos;
    number = _num;
    pointnum = _size;
    ipointnum = _isize;
    reset(pos,pointnum);qDebug()<<"setPos";
    reset(ipos,ipointnum);qDebug()<<"setPos";
}

void OpenGLView::setInverse(bool is)
{
    isInverse = is;
    update();
}

void OpenGLView::reset(position** pos,int* pointnum)
{
    for(int i = 0;i<number;i++){

        minx = 10000;
        miny = 10000;
        minz = 10000;
        maxx = -10000;
        maxy = -10000;
        maxz = -10000;
        for(int j = 0;j<pointnum[i];j++) {

            if(pos[i][j].X < minx) {
                minx = pos[i][j].X;
            }
            if(pos[i][j].Y < miny) {
                miny = pos[i][j].Y;
            }
            if(pos[i][j].Z < minz) {
                minz = pos[i][j].Z;
            }
            if(pos[i][j].X > maxx) {
                maxx = pos[i][j].X;
            }
            if(pos[i][j].Y > maxy) {
                maxy = pos[i][j].Y;
            }
            if(pos[i][j].Z > maxz) {
                maxz = pos[i][j].Z;
            }
        }

        double dx = maxx - minx;
        double dy = maxy - miny;
        double dz = maxz - minz;

        for(int j = 0;j<pointnum[i];j++) {
                pos[i][j].X = 6*(pos[i][j].X - minx)/dx - 3;
                pos[i][j].Y = 6*(pos[i][j].Y - miny)/dy - 3;
                pos[i][j].Z = 6*(pos[i][j].Z - minz)/dz - 3;
        }
    }
}

void OpenGLView::updateIndex(int _index)
{
    if(_index>=0 && _index <number) {
        index = _index;
    }
    update();
}

void OpenGLView::initializeGL()
{
    qDebug()<<"initialize";
    glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
    //initializeOpenGLFunctions();

    glClearColor(0,0,0,1);
    ax = ay = mx = my = 0;
    z = 0;
    isDown = false;
}

void OpenGLView::resizeGL(int width, int height)
{

    if (height == 0) {
        height = 1;   //让高度为1（避免出现分母为0的现象）
    }
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);//设置矩阵模式为投影
    glLoadIdentity();   //初始化矩阵为单位矩阵
    whRatio = (GLfloat)width / (GLfloat)height;  //设置显示比例
    gluPerspective(45.0f, whRatio,0.1f,100.0f);

    glMatrixMode(GL_MODELVIEW);  //设置矩阵模式为模型
}

void OpenGLView::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  //清除颜色缓存和深度缓存
    glLoadIdentity(); //初始化矩阵为单位矩阵

    gluLookAt(0,0,z+8,0,0,z,
        0, 1, 0);               // 场景（0，0，0）的视点中心 (0,5,50)，Y轴向上

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        //设置多边形绘制模式：正反面，填充

    glEnable(GL_DEPTH_TEST);//开启深度测试
    glEnable(GL_LIGHTING);  //开启光照模式

    GLfloat light_pos[] = {5.0,5.0,5.0,1}; //定义环境光位置
    GLfloat white[] = { 1.0f, 1.0f, 1.0f, 1.0f }; //定义白色

    glLightfv(GL_LIGHT0, GL_POSITION, light_pos); //设置第0号光源的光照位置
    glLightfv(GL_LIGHT0, GL_SPECULAR, white); //设置镜面反射光照颜色
    glLightfv(GL_LIGHT0, GL_DIFFUSE, white);                     //设置漫射光成分
    glLightfv(GL_LIGHT0, GL_AMBIENT, white);   //设置第0号光源多次反射后的光照颜色（环境光颜色）

    glEnable(GL_LIGHT0); //开启第0号光源
    glEnable(GL_BLEND);

    glRotatef(ax,1.0f,0.0f,0.0f);
    glRotatef(ay,0.0f,1.0f,0.0f);
    drawScene();    //绘制场景
}

void OpenGLView::drawScene()
{
//qDebug()<<"DRAW SCENE";
    GLfloat mat_ambient1[] = { 0.192250, 0.192250, 0.192250, 1.000000};
    GLfloat mat_diffuse1[] = { 0.507540, 0.507540, 0.507540, 1.000000};
    GLfloat mat_specular1[] = { 0.508273, 0.508273, 0.508273, 1.000000 };
    GLfloat mat_shininess1[] = { 51.200001}; //材质RGBA镜面指数，数值在0～128范围内


    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient1);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse1);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular1);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess1);
    glPushMatrix();
    glTranslatef(0,0,-3);
    glRotatef(180,0,1,0);
    glRecti(-3,-3,3,3);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-3,0,0);
    glRotatef(270,0,1,0);
    glRecti(-3,-3,3,3);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0,-3,0);
    glRotatef(90,1,0,0);
    glRecti(-3,-3,3,3);
    glPopMatrix();


    GLfloat mat_ambient[] = { 0.247250, 0.199500, 0.074500, 1.000000,};
    GLfloat mat_diffuse[] = { 0.751640, 0.606480, 0.226480, 1.000000,};
    GLfloat mat_specular[] = {0.628281, 0.555802, 0.366065, 1.000000, };
    GLfloat mat_shininess[] = {51.200001 };

    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    if(!isInverse) {
        if(pointnum == nullptr)return;
        for(int j = 0;j<pointnum[index];j++) {
            glPushMatrix();
            glTranslatef(pos[index][j].Y,pos[index][j].Z,-pos[index][j].X);
            glutSolidSphere(pos[index][j].D/100,16,16);
            glPopMatrix();
        }
    }
    else {
        if(ipointnum == nullptr)return;
        for(int j = 0;j<ipointnum[index];j++) {
            glPushMatrix();
            glTranslatef(ipos[index][j].Y,ipos[index][j].Z,-ipos[index][j].X);
            glutSolidSphere(ipos[index][j].D/100,16,16);
            glPopMatrix();
        }
    }
}

void OpenGLView::mousePressEvent(QMouseEvent *e)
{
    isDown = true;
    float x = e->x();
    float y = e->y();
    mx = x;
    my = y;

    update();
}

void OpenGLView::mouseMoveEvent(QMouseEvent *e)
{
    if(isDown){
        float x = e->x();
        float y = e->y();
        ax += (y-my)/5.0f;
        ay += (x-mx)/5.0f;
        mx = x;
        my = y;
        update();
    }
}

void OpenGLView::mouseReleaseEvent(QMouseEvent *e)
{
    isDown = false;
}

void OpenGLView::wheelEvent(QWheelEvent *event)
{
    int numDegrees = event->delta()/8;
    int numSteps=numDegrees/15;
    z -= numSteps;
    update();
}

