#ifndef GIQGLVIEWER_H
#define GIQGLVIEWER_H

#include <QGLViewer/qglviewer.h>
#include "model/gcode.h"
#include "model/lines.h"

class GIQGLViewer : public QGLViewer
{
    Q_OBJECT
public:
    explicit GIQGLViewer(QWidget *parent = 0);

    void drawLines(QList<GCode*> lines);
//    void changeCurrBlockNumber(int blockNumber);

    void setCurrBlockNumber(int currBlockNumber);
    void cylinder(float radius,float height,int sides);
    void line(qglviewer::Vec *v1, qglviewer::Vec *v2);
    void drawFiberFlame();
    void drawArcTheta(float cx, float cy, float r, float start_angle, float arc_angle, int num_segments);
    void drawArcPhi(float cx, float cy, float r, float start_angle, float arc_angle, int num_segments);

protected:
//    virtual void fastDraw();
    virtual void draw();
    virtual void init();
    virtual void animate();

private:
    void initCamera();
    void initFiber();

    qglviewer::ManipulatedFrame* frame;
    qglviewer::Vec *rotCenterStage;
    qglviewer::Vec *rotCenterStageAxis;
    qglviewer::Vec *rotStageVec;
    qglviewer::Vec *rotCenterFiber;
    qglviewer::Vec *rotCenterFiberAxis;
    qglviewer::Vec *rotFiberVec;

    QList<GCode*> _gcodeList;
    Lines *lineList;
    int _currBlockNumber;
};

#endif // GIQGLVIEWER_H
