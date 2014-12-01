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

    baseStageframe = new qglviewer::ManipulatedFrame();
    robotArmframe = new qglviewer::ManipulatedFrame();
    rotCenterStage = qglviewer::Vec();
    rotCenterFiber = qglviewer::Vec();
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
    rotCenterStage = qglviewer::Vec( 0.0, 0.0, 0.0);
//    rotCenterStageAxis = qglviewer::Vec( 0.1, 0.2, 0.3);

    qglviewer::Vec curr_pos = qglviewer::Vec();
    float curr_theta, curr_phi;
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

        ManipulatedFrame f = ManipulatedFrame();

        if ( e_Gcode->hasG())
        {
            s_Vec = e_Vec = curr_pos;


            switch (e_Gcode->g) {
            case 1:
                if(s_Gcode->hasX()) s_Vec.x = s_Gcode->x / shrinkRatio;
                if(s_Gcode->hasY()) s_Vec.y = s_Gcode->y / shrinkRatio;
                if(s_Gcode->hasZ()) s_Vec.z = s_Gcode->z / shrinkRatio;

                if(e_Gcode->hasX())
                {
                    e_Vec.x = e_Gcode->x / shrinkRatio;
                    curr_pos.x = e_Vec.x;
                }
                if(e_Gcode->hasY())
                {
                    e_Vec.y = e_Gcode->y / shrinkRatio;
                    curr_pos.y = e_Vec.y;
                }
                if(e_Gcode->hasZ())
                {
                    e_Vec.z = e_Gcode->z / shrinkRatio;
                    curr_pos.z = e_Vec.z;
                }

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

                drawArrow(s_Vec, e_Vec, lineRadius);

                break;
            case 68:

                switch (e_Gcode->g_plane) {
                case 17: // xy plane
                    f.translate(-rotCenterStage);
                    f.rotate(Quaternion(rotCenterStageAxis, e_Gcode->r));
                    f.translate(rotCenterStage);

//                    f->rotateAroundPoint(Quaternion(rotCenterStageAxis, e_Gcode->r), rotCenterStage);

                    curr_theta += e_Gcode->r;
                    e_Vec = endPointArc(curr_pos, rotCenterStage, curr_theta, EnumList::theta);
                    curr_pos = e_Vec;
                    break;

                case 18: // zx plane
                    break;
                case 19:
                    curr_phi = e_Gcode->r;
                    e_Vec = endPointArcPhi(curr_pos, rotCenterFiber, curr_phi, curr_theta, EnumList::phi);
//                    e_Vec = endPointArcPhi(curr_pos, rotCenterFiber, curr_phi, curr_theta, EnumList::phi);
                    curr_pos = e_Vec;

                    break;

                default:
                    break;
                }

                break;
            default:
                break;
            }

        }

        if (i == (_currBlockNumber-1))
                {
                    glColor3f(0,1,1);
                    baseStageframe->setPosition(curr_pos);
                    baseStageframe->setRotation(qglviewer::Quaternion(qglviewer::Vec(0,0,1),  curr_theta /180 * M_PI));
                    robotArmframe->setRotation(qglviewer::Quaternion(qglviewer::Vec(1,0,0),  curr_theta / 180 * M_PI + curr_phi / 180 * M_PI));
                }

    }

    drawArcPhi(0,0,0.1,0, M_PI, 50);

    // moving frame
    glPushMatrix();// transform robot arm
    glMultMatrixd(baseStageframe->matrix());
//    frame->rotate(curr_theta, 0,0,1);
    glColor3f(1.0,1.0,1.0);
    rotCenterFiber = qglviewer::Vec( 0.0, 0.2, 0.2);
    rotCenterFiberAxis = qglviewer::Vec( 0.3, 0.2, 0.2);
    rotFiberVec = new qglviewer::Vec(1,0,0);
    glBegin(GL_LINES);
    line(rotCenterFiber, rotCenterFiberAxis);
    glEnd();

//    glRotated(60, 0.0, 0.0, 1.0);

    drawAxis(0.4f);
    const float scale = 0.3f;
    glScalef(scale, scale, scale);

    glPushMatrix();// translated tip of Fiber
    glTranslated(0.2,0.2,0.2);
    glMultMatrixd(robotArmframe->matrix());
    drawAxis(0.4f);;
    cylinder(0.03, 0.4, 10);
    glPopMatrix();
    glPopMatrix();
//    drawAxis();

    // show Materix
//    GLfloat m[16];
//    gl_model
//    glGetFloatv(GL_MODELVIEW_MATRIX, m);
//    qDebug() << m[0] << m[1] << m[2];
//    qDebug() << m[5] << m[6] << m[7];
//    qDebug() << m[9] << m[10] << m[11];

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

void GIQGLViewer::line(qglviewer::Vec v1, qglviewer::Vec v2)
{
    glVertex3d(v1.x, v1.y, v1.z);
    glVertex3d(v2.x, v2.y, v2.z);
}

void GIQGLViewer::drawFiberFlame()
{

}

Vec GIQGLViewer::endPointArc(Vec s_point, Vec rotationCenter, float angle, EnumList::Axis axis)
{

    drawArcTheta(s_point, rotationCenter, angle);

    Vec v, tmpv;

    angle = angle / 180 * M_PI;

    v = s_point.operator -=( rotationCenter ); // translated center to origin
    tmpv = v;

    v.x = tmpv.x * cosf(angle) - tmpv.y * sinf(angle);
    v.y = tmpv.x * sinf(angle) + tmpv.y * cosf(angle);
//    v.z = v.z;

    v = v.operator +=( rotationCenter);

    return v;
}

Vec GIQGLViewer::endPointArcPhi(Vec s_point, Vec rotationCenter, float angle, float theta, EnumList::Axis axis)
{

//    drawArcTheta(s_point, rotationCenter, angle);

    Vec v, tmpv;

    angle = angle / 180 * M_PI;

    // translated - stage center
    v = s_point.operator -=( rotCenterStage );

    tmpv = v;
    // rotation - theta
    v.x = tmpv.x * cosf(-theta) - tmpv.y * sinf(-theta);
    v.y = tmpv.x * sinf(-theta) + tmpv.y * cosf(-theta);

    // translated - rotacion center
    tmpv = v.operator -=( rotationCenter ); // translated center to origin

//    ManipulatedFrame *f = new ManipulatedFrame();

    // rotation + phi
    v.y = tmpv.y * cosf(angle) - tmpv.z * sinf(angle);
    v.z = tmpv.y * sinf(angle) + tmpv.z * cosf(angle);

    tmpv = v.operator +=( rotationCenter);

    v.x = tmpv.x * cosf(theta) - tmpv.y * sinf(theta);
    v.y = tmpv.x * sinf(theta) + tmpv.y * cosf(theta);

    v = v.operator +=( rotCenterStage );

    return v;
}


void GIQGLViewer::drawArcTheta(Vec s_point, Vec rotationCenter, float angle)

{
    Vec v, tmpv;
    Vec lastV;
    angle = angle / 180 * M_PI;

    v = s_point.operator -=( rotationCenter );

    float num_segments = 20;
    float start_angle = atan(v.y / v.x);
    float arc_angle = angle;
    float r = v.norm();

    float theta = arc_angle / float(num_segments - 1);//theta is now calculated from the arc angle instead, the - 1 bit comes from the fact that the arc is open

    float tangetial_factor = tanf(theta);

    float radial_factor = cosf(theta);


    float x = r * cosf(start_angle);//we now start at the start angle
    float y = r * sinf(start_angle);

    glPushMatrix();
//    glTranslated(rotationCenter.x, rotationCenter.y, rotationCenter.z);
    glBegin(GL_LINE_STRIP);//since the arc is not a closed curve, this is a strip now
//    glLineWidth(0.3);
    for(int ii = 0; ii < num_segments-1; ii++)
    {
        lastV = Vec(x,y,v.z);
        glVertex3f(x , y, v.z);

        float tx = -y;
        float ty = x;

        x += tx * tangetial_factor;
        y += ty * tangetial_factor;

        x *= radial_factor;
        y *= radial_factor;
    }
    glEnd();
//	lineRadius = 0.003;

    drawArrow(lastV, Vec(x,y,s_point.z), 0.003);
    glPopMatrix();

}

void GIQGLViewer::drawArcPhi(float cx, float cy, float r, float start_angle, float arc_angle, int num_segments)
{
    float theta = arc_angle / float(num_segments - 1);//theta is now calculated from the arc angle instead, the - 1 bit comes from the fact that the arc is open

    float tangetial_factor = tanf(theta);

    float radial_factor = cosf(theta);


    float x = r * cosf(start_angle);//we now start at the start angle
    float y = r * sinf(start_angle);

    glPushMatrix();
    glTranslated(rotCenterFiber.x,
                 rotCenterFiber.y,
                 rotCenterFiber.z);
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
    glPopMatrix();
}
