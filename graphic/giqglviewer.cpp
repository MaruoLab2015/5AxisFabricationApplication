#include "giqglviewer.h"
#include "QGLViewer/vec.h"

#include <QDebug>

GIQGLViewer::GIQGLViewer(QWidget *parent) :
    QGLViewer(parent)
{
}

void GIQGLViewer::init()
{
//    qDebug() << "init";
  restoreStateFromFile();
  glDisable(GL_LIGHTING);
  glPointSize(3.0);
  setGridIsDrawn();

  initCamera();
//  startAnimation();
}

void GIQGLViewer::initCamera()
{
    qglviewer::Camera *initCamera = camera();
    qglviewer::Vec cameraPos = qglviewer::Vec(2, 2, 2);
    initCamera->setPosition(cameraPos);
    initCamera->setUpVector(qglviewer::Vec(0,0,1));
    initCamera->lookAt(qglviewer::Vec());
    initCamera->setZNearCoefficient(0.000001);
    initCamera->setZClippingCoefficient(10000);
    setCamera(initCamera);
}

//void GIQGLViewer::fastDraw()
//{

//}

void GIQGLViewer::draw()
{
    float curr_z, curr_y, curr_x;
    curr_x = curr_y = curr_z = 0.0f;
    for(int i=0;i<_gcodeList.count() - 1;i++)
    {
        GCode *s_Gcode;
        GCode *e_Gcode;
        s_Gcode = _gcodeList[i];
        e_Gcode = _gcodeList[i+1];

        qglviewer::Vec s_Vec, e_Vec;
        s_Vec = e_Vec = qglviewer::Vec();

        int e = 0;

        if(s_Gcode->hasX()) s_Vec.x = s_Gcode->x / 100000.0;
        else s_Vec.x = curr_x;
        if(s_Gcode->hasY()) s_Vec.y = s_Gcode->y / 100000.0;
        else s_Vec.y = curr_y;
        if(s_Gcode->hasZ()) s_Vec.z = s_Gcode->z;
        else s_Vec.z = curr_z;

        if(e_Gcode->hasX())
        {
            e_Vec.x = e_Gcode->x / 100000.0;
            curr_x = e_Vec.x;
        }
        else e_Vec.x = curr_x;
        if(e_Gcode->hasY())
        {
            e_Vec.y = e_Gcode->y / 100000.0;
            curr_y = e_Vec.y;
        }
        else e_Vec.y = curr_y;
        if(e_Gcode->hasZ())
        {
            e_Vec.z = e_Gcode->z;
            curr_z = e_Vec.z;
        }
        else e_Vec.z = curr_z;

        if(e_Gcode->hasE()) e = e_Gcode->e;

        if (e == 1)
        {
            glColor3f(1, 0, 0);
            glLineWidth(5.0);
        }
        else
        {
            glColor3f(1,0,1);
            glLineWidth(1.0);
        }

        glBegin(GL_LINES);
        glVertex3fv(s_Vec);
        glVertex3fv(e_Vec);
        glEnd();
    }

    glColor3f(1.0,1.0,1.0);
    drawAxis();
}

void GIQGLViewer::animate()
{
//    qDebug() << "animate";
}

void GIQGLViewer::drawLines(QList<GCode*> gcodeList)
{
    _gcodeList = gcodeList;
    updateGL();
}
