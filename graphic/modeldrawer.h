#ifndef MODELDRAWER_H
#define MODELDRAWER_H

#include <QObject>
#include <QGLViewer/qglviewer.h>

using namespace qglviewer;

class ModelDrawer : public QObject
{
    Q_OBJECT
public:
    explicit ModelDrawer(QObject *parent = 0);

    static void cylinder(float radius,float height,int sides);
    static void line(Vec v1, Vec v2);
    static void arcTheta(float cx, float cy, float cz,float r, float start_angle, float arc_angle, int num_segments);
    static void arcTheta(Vec v, float r, float start_angle, float arc_angle, int num_segments);
    static void arcPhi(float cx, float cy, float cz,float r, float start_angle, float arc_angle, int num_segments);
    static void arcPhi(Vec v, float r, float start_angle, float arc_angle, int num_segments);

signals:

public slots:

};

#endif // MODELDRAWER_H
