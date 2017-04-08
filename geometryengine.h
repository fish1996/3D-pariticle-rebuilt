
#ifndef GEOMETRYENGINE_H
#define GEOMETRYENGINE_H

#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QVector>

class GeometryEngine : protected QOpenGLFunctions
{
public:
    GeometryEngine();
    virtual ~GeometryEngine();

    void drawCubeGeometry(QOpenGLShaderProgram *program);

private:
    GLfloat radians(GLfloat r);
    const int angleSpan = 9;
    void initCubeGeometry();
    GLfloat* sphere(GLfloat radius);
    void normalize(QVector3D& vec);

    QOpenGLBuffer arrayBuf;
    QOpenGLBuffer indexBuf;
    QOpenGLBuffer lineBuf;
};

#endif // GEOMETRYENGINE_H

