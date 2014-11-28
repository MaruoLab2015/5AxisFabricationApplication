#include "giqglviewer.h"
#include "QGLViewer/vec.h"
#include <QGLViewer/manipulatedCameraFrame.h>

#include <QDebug>
#include <QtMath>

GIQGLViewer::GIQGLViewer(QWidget *parent) :
    QGLViewer(parent),
    _currBlockNumber(INT_MAX)
{
}

void GIQGLViewer::init()
{
  restoreStateFromFile();
  glDisable(GL_LIGHTING);
  glPointSize(3.0);
  setGridIsDrawn();
//  setAxisIsDrawn();

  initCamera();
  initFiber();

//  startAnimation();
}

void GIQGLViewer::initFiber()
{

    frame = new qglviewer::ManipulatedFrame();
    rotCenterStage = new qglviewer::Vec();
    rotCenterFiber = new qglviewer::Vec();
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

void GIQGLViewer::setCurrBlockNumber(int currBlockNumber)
{
    _currBlockNumber = currBlockNumber;
    updateGL();
}


//void GIQGLViewer::fastDraw()
//{

//}

void GIQGLViewer::draw()
{
    float curr_z, curr_y, curr_x;
    float curr_theta, curr_phi;
    curr_x = curr_y = curr_z = 0.0f;
    curr_theta, curr_phi = 0.0f;
    double shrinkRatio = lineList->maxSize(); // bounding box sizE
    float lineRadius = 1.0;


    for(int i=0;i<_gcodeList.count() - 1;i++)
    {
        GCode *s_Gcode = new GCode();
        GCode *e_Gcode = new GCode();
        s_Gcode = _gcodeList[i];
        e_Gcode = _gcodeList[i+1];

        qglviewer::Vec s_Vec, e_Vec;
        s_Vec = qglviewer::Vec();
        e_Vec = qglviewer::Vec();

        int e = 0;

        if ( e_Gcode->hasG())
        {

            switch (e_Gcode->g) {
            case 1:
                if(s_Gcode->hasX()) s_Vec.x = s_Gcode->x / shrinkRatio;
                else s_Vec.x = curr_x;
                if(s_Gcode->hasY()) s_Vec.y = s_Gcode->y / shrinkRatio;
                else s_Vec.y = curr_y;
                if(s_Gcode->hasZ()) s_Vec.z = s_Gcode->z / shrinkRatio;
                else s_Vec.z = curr_z;

                if(e_Gcode->hasX())
                {
                    e_Vec.x = e_Gcode->x / shrinkRatio;
                    curr_x = e_Vec.x;
                }
                else e_Vec.x = curr_x;
                if(e_Gcode->hasY())
                {
                    e_Vec.y = e_Gcode->y / shrinkRatio;
                    curr_y = e_Vec.y;
                }
                else e_Vec.y = curr_y;
                if(e_Gcode->hasZ())
                {
                    e_Vec.z = e_Gcode->z / shrinkRatio;
                    curr_z = e_Vec.z;
                }
                else e_Vec.z = curr_z;

                if(e_Gcode->hasE()) e = e_Gcode->e;

                if (e == 1)
                {
                    glColor3f(0, 0, 1);
                    lineRadius = 0.003;
                }
                else
                {
                    glColor3f(1,0,1);
                    lineRadius = 0.001;
                }
                break;
            case 68:
                break;
            default:
                break;
            }

        }


        if (i == (_currBlockNumber-1))
        {
            glColor3f(0,1,1);
            frame->setPosition(e_Vec);
        }

        drawArrow(s_Vec, e_Vec, lineRadius);

    }

    //theta axis
    rotCenterStage = new qglviewer::Vec( 0.1, 0.2, 0.0);
    rotCenterStageAxis = new qglviewer::Vec( 0.1, 0.2, 0.3);
    rotStageVec = new qglviewer::Vec(0,0,1);
    glBegin(GL_LINES);
    line(rotCenterStage, rotCenterStageAxis);
    glEnd();


    drawArcPhi(0,0,1,0, M_PI, 50);
//    frame->rotate(30.0, 0.0, 0.0, 1.0);

    // moving frame
    glMultMatrixd(frame->matrix());
    glColor3f(1.0,1.0,1.0);
    rotCenterFiber = new qglviewer::Vec( 0.0, 0.2, 0.2);
    rotCenterFiberAxis = new qglviewer::Vec( 0.3, 0.2, 0.2);
    rotFiberVec = new qglviewer::Vec(1,0,0);
    glBegin(GL_LINES);
    line(rotCenterFiber, rotCenterFiberAxis);
    glEnd();

//    glRotated(30.0, 0.0, 0.0, 1.0);

    drawAxis(0.4f);
    const float scale = 0.3f;
    glScalef(scale, scale, scale);

    cylinder(0.03, 0.4, 10);
//    drawAxis();
}

void GIQGLViewer::animate()
{
//    qDebug() << "animate";
}

void GIQGLViewer::drawLines(QList<GCode*> gcodeList)
{
    _gcodeList = gcodeList;
    lineList = new Lines(gcodeList);
    lineList->computedBoudingBox();
    updateGL();
}

void GIQGLViewer::cylinder(float radius,float height,int sides)
{
//    double pi = 3.1415;
    //上面
    glNormal3d(0.0, 1.0, 0.0);
    glBegin(GL_POLYGON);
    for(double i = 0; i < sides; i++) {
        double t = M_PI*2/sides * (double)i;
        glVertex3d(radius * cos(t), radius * sin(t), height);
    }
    glEnd();
    //側面
    glBegin(GL_QUAD_STRIP);
    for(double i=0;i<=sides;i=i+1){
        double t = i*2*M_PI/sides;
        glNormal3f((GLfloat)cos(t),0.0,(GLfloat)sin(t));
        glVertex3f((GLfloat)(radius*cos(t)),(GLfloat)(radius*sin(t)), 0);
        glVertex3f((GLfloat)(radius*cos(t)),(GLfloat)(radius*sin(t)), height);
    }
    glEnd();
    //下面
    glNormal3d(0.0, -1.0, 0.0);
    glBegin(GL_POLYGON);
    for(double i = sides; i >= 0; --i) {
        double t = M_PI*2/sides * (double)i;
        glVertex3d(radius * cos(t), radius * sin(t), 0);
    }
    glEnd();
}

void GIQGLViewer::line(qglviewer::Vec *v1, qglviewer::Vec *v2)
{
    glVertex3d(v1->x, v1->y, v1->z);
    glVertex3d(v2->x, v2->y, v2->z);
}

void GIQGLViewer::drawFiberFlame()
{

}

void GIQGLViewer::drawArcTheta(float cx, float cy, float r, float start_angle, float arc_angle, int num_segments)
{
    float theta = arc_angle / float(num_segments - 1);//theta is now calculated from the arc angle instead, the - 1 bit comes from the fact that the arc is open

    float tangetial_factor = tanf(theta);

    float radial_factor = cosf(theta);


    float x = r * cosf(start_angle);//we now start at the start angle
    float y = r * sinf(start_angle);

    glBegin(GL_LINE_STRIP);//since the arc is not a closed curve, this is a strip now
    for(int ii = 0; ii < num_segments; ii++)
    {
        glVertex2f(x + cx, y + cy);

        float tx = -y;
        float ty = x;

        x += tx * tangetial_factor;
        y += ty * tangetial_factor;

        x *= radial_factor;
        y *= radial_factor;
    }
    glEnd();
}

void GIQGLViewer::drawArcPhi(float cx, float cy, float r, float start_angle, float arc_angle, int num_segments)
{
    float theta = arc_angle / float(num_segments - 1);//theta is now calculated from the arc angle instead, the - 1 bit comes from the fact that the arc is open

    float tangetial_factor = tanf(theta);

    float radial_factor = cosf(theta);


    float x = r * cosf(start_angle);//we now start at the start angle
    float y = r * sinf(start_angle);

    glBegin(GL_LINE_STRIP);//since the arc is not a closed curve, this is a strip now
    for(int ii = 0; ii < num_segments; ii++)
    {
        glVertex3f(0, x + cx, y + cy);

        float tx = -y;
        float ty = x;

        x += tx * tangetial_factor;
        y += ty * tangetial_factor;

        x *= radial_factor;
        y *= radial_factor;
    }
    glEnd();
}
