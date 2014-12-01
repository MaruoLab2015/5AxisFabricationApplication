#ifndef GIQGLVIEWER_H
#define GIQGLVIEWER_H

#include <QGLViewer/qglviewer.h>
#include "model/gcode.h"
#include "model/lines.h"
#include "EnumList.h"

using namespace qglviewer;

class GIQGLViewer : public QGLViewer
{
    Q_OBJECT
public:
    explicit GIQGLViewer(QWidget *parent = 0);

    void drawLines(QList<GCode*> lines);
//    void changeCurrBlockNumber(int blockNumber);

    void setCurrBlockNumber(int currBlockNumber);
    void cylinder(float radius,float height,int sides);
    void line(qglviewer::Vec v1, qglviewer::Vec v2);
    void drawFiberFlame();
//    void drawArcTheta(float cx, float cy, float r, float start_angle, float arc_angle, int num_segments);
    void drawArcPhi(float cx, float cy, float r, float start_angle, float arc_angle, int num_segments);
    Vec endPointArc(Vec s_point, Vec rotationCenter, float angle, EnumList::Axis axis);
    Vec endPointArcPhi(Vec s_point, Vec rotationCenter, float angle, float theta, EnumList::Axis axis);
    void drawArcTheta(Vec s_point, Vec rotationCenter, float angle);


protected:
//    virtual void fastDraw();
    virtual void draw();
    virtual void init();
    virtual void animate();

private:
    void initCamera();
    void initFiber();

    ManipulatedFrame* baseStageframe;
    ManipulatedFrame* robotArmframe;
    Vec *a;
    qglviewer::Vec rotCenterStage;
    qglviewer::Vec rotCenterStageAxis;
    qglviewer::Vec *rotStageVec;
    qglviewer::Vec rotCenterFiber;
    qglviewer::Vec rotCenterFiberAxis;
    qglviewer::Vec *rotFiberVec;

    QList<GCode*> _gcodeList;
    Lines *lineList;
    int _currBlockNumber;
};

#endif // GIQGLVIEWER_H
