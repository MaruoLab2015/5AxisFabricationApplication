#include "giqglviewer.h"
#include "graphic/modeldrawer.h"

#include <QGLViewer/manipulatedCameraFrame.h>
#include <QDebug>
#include <QtMath>

GIQGLViewer::GIQGLViewer(QWidget *parent) :
    QGLViewer(parent),
    currentPosition(Vec()),
    currentXYZ(Vec()),
    currentTheta(0.0f),
    currentPhi(0.0f),
    modelScale(1),
    lineList(new Lines()),
    _currBlockNumber(INT_MAX)
{
}

void GIQGLViewer::init()
{
  restoreStateFromFile();
  glDisable(GL_LIGHTING);
  glPointSize(3.0);
  setGridIsDrawn();

  initCamera();
  initFiber();
}

void GIQGLViewer::initFiber()
{
    fiberTipframe = new ManipulatedFrame();
    robotArmframe = new qglviewer::ManipulatedFrame();

    // initial
    fromFiberCenterToTip = Vec(0.0f, -1.0f, -1.0f);
    fromOriginToFiberCenter = Vec(0.0f, 1.0f, 1.0f);
    fromFiberCenterToTip.operator /=(2);
    fromOriginToFiberCenter.operator /=(2);
}

void GIQGLViewer::initCamera()
{
    Camera *initCamera = camera();
    Vec cameraPos = Vec(2, 2, 2);
    initCamera->setPosition(cameraPos);
    initCamera->setUpVector(Vec(0,0,1));
    initCamera->lookAt(Vec());
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
    currentPosition = Vec();
    currentXYZ = Vec();
    currentTheta = 0.0f;
    currentPhi = 0.0f;
    Vec s_Vec, e_Vec;
    s_Vec = Vec();
    e_Vec = Vec();

    modelScale = lineList->maxSize();
    double shrinkRatio = lineList->maxSize(); // bounding box size
    float lineRadius = 1.0;
    int s = 0;

    for(int i=0;i<_gcodeList.count();i++)
    {
        GCode *e_Gcode = new GCode();
        e_Gcode = _gcodeList[i];

        if ( e_Gcode->hasNormalVec() )
        {
            s_Vec = e_Vec;

            // line color
            if(e_Gcode->hasS()) s = e_Gcode->s;
            if (s == 1)
            {
                glColor3f(0, 0, 1);
                lineRadius = 0.003;
            }
            else
            {
                glColor3f(1,0,1);
                lineRadius = 0.001;
            }

            // draw normal
            Vec n = Vec(e_Gcode->ii,e_Gcode->jj, e_Gcode->kk);
            n.normalize();
            Vec v = Vec(e_Gcode->x ,e_Gcode->y, e_Gcode->z);
            v.operator /=(shrinkRatio);

            Vec pt = fromFiberCenterToTip;
            Vec d = fromOriginToFiberCenter;

            // compute angle from normal
            currentTheta = atan2(n.y, n.x)/ M_PI * 180 + 90;
            currentPhi = atan2(
                               sqrt(n.x * n.x +
                                    n.y * n.y), n.z) / M_PI * 180;

            float ct, st, cp, sp;
            ct = cos(currentTheta / 180.0 * M_PI );
            st = sin(currentTheta / 180.0 * M_PI );
            cp = cos( currentPhi / 180.0 * M_PI);
            sp = sin( currentPhi / 180.0 * M_PI);

            // read value
            currentXYZ.x = (v.x - pt.x * ct - d.x * ct + pt.y * cp * st - pt.z * sp * st + d.y * st) * ct
                        +  (v.y - pt.x * ct - d.x * ct - pt.y * cp * ct + pt.z * sp * ct - d.y * ct) * st;

            currentXYZ.y = (v.x - pt.x * ct - d.x * ct + pt.y * cp * st - pt.z * sp * st + d.y * st) * -st
                         + (v.y - pt.x * st - d.x * st - pt.y * cp * ct + pt.z * sp * ct - d.y * ct) * ct;

            currentXYZ.z = v.z
                    -pt.y * sp
                    -pt.z * cp
                    +pt.z;

            if(e_Gcode->hasV())
                currentVelocity = e_Gcode->v;

            //applyScale
//TODO: applyFiberFlameScaleEffect

            e_Vec = computeEndPoint(currentXYZ, currentTheta, currentPhi);
            drawArrow(s_Vec, e_Vec, lineRadius);

        }
        else if ( e_Gcode->hasCode())
        {
            s_Vec = e_Vec;

            // line color
            if(e_Gcode->hasS()) s = e_Gcode->s;
            if (s == 1)
            {
                glColor3f(0, 0, 1);
                lineRadius = 0.003;
            }
            else
            {
                glColor3f(1,0,1);
                lineRadius = 0.001;
            }

            // read value
            if(e_Gcode->hasX())
                currentXYZ.x = e_Gcode->x / shrinkRatio;
            if(e_Gcode->hasY())
                currentXYZ.y = e_Gcode->y / shrinkRatio;
            if(e_Gcode->hasZ())
                currentXYZ.z = e_Gcode->z / shrinkRatio;

            if(e_Gcode->hasT())
                currentTheta = e_Gcode->t;
            if(e_Gcode->hasP())
                currentPhi = e_Gcode->p;
            if(e_Gcode->hasV())
                currentVelocity = e_Gcode->v;

            //applyScale
//TODO: applyFiberFlameScaleEffect

            e_Vec = computeEndPoint(currentXYZ, currentTheta, currentPhi);
            drawArrow(s_Vec, e_Vec, lineRadius);
        }

        glColor3f(1,1,1);

        if (i == (_currBlockNumber))
        {
            fiberTipframe->setPosition(e_Vec);
            fiberTipframe->setRotation(Quaternion(Vec(0,0,1),  currentTheta /180 * M_PI));
            fiberTipframe->rotate(Quaternion(Vec(1,0,0),   currentPhi / 180 * M_PI ));

            displayText(currentXYZ, currentTheta, currentPhi, e_Vec, currentVelocity,shrinkRatio);
        }
    }

    placeFiberFrame();

    // display default position info
    if (_currBlockNumber == INT_MAX)
        displayText(Vec(), 0,0, Vec(), 0, 1);
}

void GIQGLViewer::placeFiberFrame()
{
    // fiber tip frame
    glPushMatrix();// transform robot arm
    glMultMatrixd(fiberTipframe->matrix());
    drawAxis(0.4);
    ModelDrawer::cylinder(0.03,0.4,10);
    glTranslatef(
                -fromFiberCenterToTip.x,
                -fromFiberCenterToTip.y,
                -fromFiberCenterToTip.z
                );
    drawPhiCircle();
    drawAxis(0.4f);
    glPopMatrix();
}

void GIQGLViewer::drawLines(QList<GCode*> gcodeList)
{
    _gcodeList = gcodeList;
    lineList = new Lines(gcodeList);
    lineList->computedBoudingBox();
    updateGL();
}

void GIQGLViewer::drawPhiCircle()
{
    int num_segments = 50;
    float r = fromFiberCenterToTip.norm();
    float theta = M_PI * 2 / float(num_segments - 1);//theta is now calculated from the arc angle instead, the - 1 bit comes from the fact that the arc is open
    float start_angle = 0;

    float tangetial_factor = tanf(theta);
    float radial_factor = cosf(theta);

    float x = r * cosf(start_angle);//we now start at the start angle
    float y = r * sinf(start_angle);

    glBegin(GL_LINE_STRIP);//since the arc is not a closed curve, this is a strip now
    for(int ii = 0; ii < num_segments; ii++)
    {
        glVertex3f(0, x , y);

        float tx = -y;
        float ty = x;

        x += tx * tangetial_factor;
        y += ty * tangetial_factor;

        x *= radial_factor;
        y *= radial_factor;
    }
    glEnd();
}

Vec GIQGLViewer::computeEndPoint(Vec moveXYZ, float theta, float phi)
{
    Vec e_Vec = Vec();

    glPushMatrix();
    glLoadIdentity();
    // ratated phi
    glRotatef(theta, 0,0,1);
    // traslated origin
    glTranslatef(
                moveXYZ.x,
                moveXYZ.y,
                moveXYZ.z
                );
    //translated xyz
    glTranslatef(
                fromOriginToFiberCenter.x,
                fromOriginToFiberCenter.y,
                fromOriginToFiberCenter.z
                );
    //rotated theta
    glRotatef(phi, 1,0,0);
    e_Vec = applyModelViewMatrix(fromFiberCenterToTip);
    glPopMatrix();

    return e_Vec;
}

Vec GIQGLViewer::applyModelViewMatrix(Vec v)
{
    GLfloat m[16];
    glGetFloatv(GL_MODELVIEW_MATRIX, m);
    float v_mat[4], tmp_v_mat[4];
    tmp_v_mat[0] = v.x;
    tmp_v_mat[1] = v.y;
    tmp_v_mat[2] = v.z;
    tmp_v_mat[3] = 1;

    int column = 4;

    for(int i = 0;i < column;i++) {
        register double f = 0.0;
        for(int j = 0;j < column;j++) {
            f += m[i + j * column] * tmp_v_mat[j];
        }
        v_mat[i] = f;
    }

    return Vec(v_mat[0], v_mat[1], v_mat[2]);
}

void GIQGLViewer::showMatrix()
{
    GLfloat m[16];
    glGetFloatv(GL_MODELVIEW_MATRIX, m);
    qDebug() << "m = ";
    qDebug() << m[0] << m[4] << m[8] << m[12];
    qDebug() << m[1] << m[5] << m[9] << m[13];
    qDebug() << m[2] << m[6] << m[10] << m[14];
    qDebug() << m[3] << m[7] << m[11] << m[15];
}

void GIQGLViewer::showVector(Vec v)
{
    qDebug() << "v = (" << v.x << ", " << v.y << ", " << v.z << ")";
}

void GIQGLViewer::displayText(Vec xyz, float theta, float phi, Vec realPos, float v, float scale)
{
    if( qAbs(realPos.x) < 0.00001) realPos.x = 0;
    if( qAbs(realPos.y) < 0.00001) realPos.y = 0;
    if( qAbs(realPos.z) < 0.00001) realPos.z = 0;

    drawText(20, 20, QString("X : %1").arg(xyz.x * scale));
    drawText(20, 40, QString("Y : %1").arg(xyz.y * scale));
    drawText(20, 60, QString("Z : %1").arg(xyz.z * scale));
    drawText(150, 20, QString("Theta    : %1").arg(theta));
    drawText(150, 40, QString("    Phi    : %1").arg(phi));
    drawText(150, 60, QString("velocity : %1").arg(v));
    drawText(300, 20, QString("realX : %1").arg(realPos.x * scale));
    drawText(300, 40, QString("realY : %1").arg(realPos.y * scale));
    drawText(300, 60, QString("realZ : %1").arg(realPos.z * scale));
}
