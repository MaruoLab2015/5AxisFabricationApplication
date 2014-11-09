#include "renderer.h"

#include <QDebug>
#include <QtOpenGL>
#include <GLUT/glut.h>

Renderer::Renderer(QWidget *parent) :
    QGLWidget(parent)
{
}


void Renderer::initializeGL()
{
    glClearColor(0.2, 0.2, 0.2, 1);
}

void Renderer::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glRotatef(0.5, 1, 1, 1);
    glColor3f(1, 0.6, 0);
}

void Renderer::resizeGL(int width, int height)
{
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, (float)width/height, 0.01, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0,0,5, 0,0,0, 0,1,0);
}
