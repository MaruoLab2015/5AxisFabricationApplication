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

    double shrinkRatio = lineList->maxSize(); // bounding box size
    float lineRadius = 1.0;

    for(int i=0;i<_gcodeList.count();i++)
    {
        GCode *e_Gcode = new GCode();
        e_Gcode = _gcodeList[i];

        int e = 0;

        if ( e_Gcode->hasG())
        {
            s_Vec = e_Vec;

            switch (e_Gcode->g) {
            case 1:
                if(e_Gcode->hasX())
                {
                    e_Vec.x = e_Gcode->x / shrinkRatio;
                    currentXYZ.x = e_Vec.x;
                }
                if(e_Gcode->hasY())
                {
                    e_Vec.y = e_Gcode->y / shrinkRatio;
                    currentXYZ.y = e_Vec.y;
                }
                if(e_Gcode->hasZ())
                {
                    e_Vec.z = e_Gcode->z / shrinkRatio;
                    currentXYZ.z = e_Vec.z;
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

                break;
            case 68:
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

                switch (e_Gcode->g_plane) {
                case 17: // xy plane

//                    drawArcTheta(e_Vec, e_Gcode->r);
                    currentTheta = e_Gcode->r;
                    break;

                case 18: // zx plane
                    break;
                case 19:
//                    drawArcPhi(e_Vec, e_Gcode->r);
                    currentPhi = e_Gcode->r;
                    break;

                default:
                    break;
                }
                break;
            default:
                break;
            }

            e_Vec = computeEndPoint(currentXYZ, currentTheta, currentPhi);
            drawArrow(s_Vec, e_Vec, lineRadius);
        }

        glColor3f(1,1,1);

        if (i == (_currBlockNumber))
        {
            fiberTipframe->setPosition(e_Vec);
            fiberTipframe->setRotation(Quaternion(Vec(0,0,1),  currentTheta /180 * M_PI));
            fiberTipframe->rotate(Quaternion(Vec(1,0,0),   currentPhi / 180 * M_PI ));

            displayText(currentXYZ, currentTheta, currentPhi, e_Vec, shrinkRatio);
        }

    }

    // fiber tip frame
    glPushMatrix();// transform robot arm
    glMultMatrixd(fiberTipframe->matrix());
    ModelDrawer::cylinder(0.03,0.4,10);
    glPopMatrix();

    // display default position info
    if (_currBlockNumber == INT_MAX)
        displayText(Vec(), 0,0, Vec(), 1);
}

void GIQGLViewer::drawLines(QList<GCode*> gcodeList)
{
    _gcodeList = gcodeList;
    lineList = new Lines(gcodeList);
    lineList->computedBoudingBox();
    updateGL();
}

//void GIQGLViewer::drawArcTheta(Vec s_point, float angle)
//{
//    Vec v;
//    angle = angle / 180 * M_PI;

//    v = Vec(s_point.x, s_point.y, 0);

//    float num_segments = 20;
//    float start_angle = atan2f(v.y, v.x);
//    float arc_angle = angle;
//    float r = v.norm();

//    ModelDrawer::arcTheta(s_point, r, start_angle, arc_angle, num_segments);
//}


//void GIQGLViewer::drawArcPhi(Vec s_point, float angle)
//{
//    Vec v;
//    v = Vec(0 , s_point.y, s_point.z);

//    float num_segments = 20;
//    float start_angle = -3.0/4.0 * M_PI;//atan(fromFiberCenterToTip.y / fromFiberCenterToTip.x);//currentPhi / 180 * M_PI;
//    float arc_angle = angle / 180 * M_PI;
//    float r = fromFiberCenterToTip.norm();

//    glPushMatrix();

//    glRotatef(currentTheta, 0,0,1);
//    glTranslatef(
//                currentXYZ.x,
//                currentXYZ.y,
//                currentXYZ.z
//                );
//    glTranslatef(
//                fromOriginToFiberCenter.x,
//                fromOriginToFiberCenter.y,
//                fromOriginToFiberCenter.z
//                );
//    glRotatef(currentPhi, 1,0,0);

//    ModelDrawer::arcPhi(s_point, r, start_angle, arc_angle, num_segments);

//    glPopMatrix();
//}

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

void GIQGLViewer::displayText(Vec xyz, float theta, float phi, Vec realPos, float scale)
{
    drawText(20, 20, QString("X : %1").arg(xyz.x * scale));
    drawText(20, 40, QString("Y : %1").arg(xyz.y * scale));
    drawText(20, 60, QString("Z : %1").arg(xyz.z * scale));
    drawText(20, 80, QString("Theta : %1").arg(theta));
    drawText(20, 100, QString("Phi : %1").arg(phi));
    drawText(100, 20, QString("realX : %1").arg(realPos.x * scale));
    drawText(100, 40, QString("realY : %1").arg(realPos.y * scale));
    drawText(100, 60, QString("realZ : %1").arg(realPos.z * scale));

}
