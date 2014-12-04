#include "modeldrawer.h"

#include <gl.h>
#include <QtMath>
#include <QDebug>

ModelDrawer::ModelDrawer(QObject *parent) :
    QObject(parent)
{
}

void ModelDrawer::cylinder(float radius,float height,int sides)
{
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

void ModelDrawer::line(Vec v1, Vec v2)
{
    glBegin(GL_LINES);
    glVertex3d(v1.x, v1.y, v1.z);
    glVertex3d(v2.x, v2.y, v2.z);
    glEnd();
}

void ModelDrawer::arcTheta(Vec v, float r, float start_angle, float arc_angle, int num_segments)
{
    arcTheta(v.x, v.y, v.z, r, start_angle, arc_angle, num_segments);
}

void ModelDrawer::arcTheta(float cx, float cy, float cz, float r, float start_angle, float arc_angle, int num_segments)
{
    float theta = arc_angle / float(num_segments - 1);//theta is now calculated from the arc angle instead, the - 1 bit comes from the fact that the arc is open

    float tangetial_factor = tanf(theta);

    float radial_factor = cosf(theta);

    float x = r * cosf(start_angle);//we now start at the start angle
    float y = r * sinf(start_angle);

    glBegin(GL_LINE_STRIP);//since the arc is not a closed curve, this is a strip now
    for(int ii = 0; ii < num_segments; ii++)
    {
        glVertex3f(x , y, cz);

        float tx = -y;
        float ty = x;

        x += tx * tangetial_factor;
        y += ty * tangetial_factor;

        x *= radial_factor;
        y *= radial_factor;
    }
    glEnd();
}

void ModelDrawer::arcPhi(Vec v, float r, float start_angle, float arc_angle, int num_segments)
{
    arcPhi(v.x, v.y, v.z, r, start_angle, arc_angle, num_segments);
}

void ModelDrawer::arcPhi(float cx, float cy, float cz, float r, float start_angle, float arc_angle, int num_segments)
{
    float theta = arc_angle / float(num_segments - 1);//theta is now calculated from the arc angle instead, the - 1 bit comes from the fact that the arc is open

    float tangetial_factor = tanf(theta);

    float radial_factor = cosf(theta);

    float y = r * cosf(start_angle);//we now start at the start angle
    float z = r * sinf(start_angle);

    glBegin(GL_LINE_STRIP);//since the arc is not a closed curve, this is a strip now
    for(int ii = 0; ii < num_segments; ii++)
    {
        glVertex3f(0 , y, z);

        float tx = -z;
        float ty = y;

        y += tx * tangetial_factor;
        z += ty * tangetial_factor;

        y *= radial_factor;
        z *= radial_factor;
    }
    glEnd();
}
