#include "graphicswidget.h"

#include <QtOpenGL>
#include <GLUT/glut.h>

GraphicsWidget::GraphicsWidget(QWidget *parent) :
    QGLWidget(parent)
  ,xRot(0)
  ,yRot(0)
  ,zRot(0)
{
}

void GraphicsWidget::initializeGL()
{
    glClearColor(0.2, 0.2, 0.2, 1);
}

void GraphicsWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glRotatef(0.5, 1, 1, 1);
    glColor3f(1, 0.6, 0);
//    glutSolidTeapot(0.6);

    drawAxis(10);
    drawTile(10);
}

void GraphicsWidget::resizeGL(int width, int height)
{
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, (float)width/height, 0.01, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0,0,5, 0,0,0, 0,1,0);
}

// Rotation

void GraphicsWidget::qNormalizeAngle(int &angle)
{
    while (angle < 0)
        angle += 360 * 16;
    while (angle > 360)
        angle -= 360 * 16;
}

void GraphicsWidget::setXRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != xRot)
    {
        xRot = angle;
        updateGL();
    }
}

void GraphicsWidget::setYRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != yRot)
    {
        yRot = angle;
        updateGL();
    }
}

void GraphicsWidget::setZRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != zRot)
    {
        zRot = angle;
        updateGL();
    }
}

// mouse Action

void GraphicsWidget::mousePressEvent(QMouseEvent *event)
{
    lastPos = event->pos();
}

void GraphicsWidget::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->x() - lastPos.x();
    int dy = event->y() - lastPos.y();

    if (event->buttons()  & Qt::LeftButton)
    {
//        setXRotation(xRot + 4 * dy);
        setYRotation(yRot + 4 * dx);
    }

    lastPos = event->pos();

}

// draw model

void GraphicsWidget::drawAxis(int size)
{
    glLineWidth(1);
    glBegin(GL_LINE_LOOP);
    glColor3f(1, 0, 0);
    glVertex3f(0,0,0);
    glVertex3f(size, 0, 0);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glColor3f(0, 1, 0);
    glVertex3f(0,0,0);
    glVertex3f(0, size, 0);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glColor3f(0, 0, 1);
    glVertex3f(0,0,0);
    glVertex3f(0, 0, size);
    glEnd();
}

void GraphicsWidget::drawTile(float size)
{
    float aTilelength = 5.0;
    float edgeTotalLength = size * 2.0;

    float tileLength = edgeTotalLength / aTilelength;
    int tileCount = qFloor(edgeTotalLength / aTilelength);

    for (int i=0;i<tileCount;i++)
    {
        float x1 = i * tileLength - tileLength * tileCount / 2.0;
        float z1 = - edgeTotalLength + tileLength * tileCount / 2.0;
        float x2 = i * tileLength - tileLength * tileCount / 2.0;
        float z2 =   edgeTotalLength - tileLength * tileCount / 2.0;
        glLineWidth(1);
        glBegin(GL_LINE_LOOP);
        glColor3f(1, 1, 1);
        glVertex3f(x1, -0.01, z1);
        glVertex3f(x2, -0.01, z2);
        glEnd();
    }

    for (int i=0;i<tileCount;i++)
    {
        float z1 = i * tileLength - tileLength * tileCount / 2.0;
        float x1 = - edgeTotalLength + tileLength * tileCount / 2.0;
        float z2 = i * tileLength - tileLength * tileCount / 2.0;
        float x2 =   edgeTotalLength - tileLength * tileCount / 2.0;
        glLineWidth(1);
        glBegin(GL_LINE_LOOP);
        glColor3f(1, 1, 1);
        glVertex3f(x1, -0.01, z1);
        glVertex3f(x2, -0.01, z2);
        glEnd();
    }

}
