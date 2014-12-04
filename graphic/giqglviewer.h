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

    void drawPhiCircle();
    Vec applyModelViewMatrix(Vec v);

protected:
//    virtual void fastDraw();
    virtual void draw();
    virtual void init();

private:
    void initCamera();
    void initFiber();

    void showMatrix();
    void showVector(Vec v);
    void displayText(Vec xyz, float theta, float phi, Vec realPos, float v, float scale);

    ManipulatedFrame* fiberTipframe;
    ManipulatedFrame* robotArmframe;

    Vec fromFiberCenterToTip;
    Vec fromOriginToFiberCenter;
    Vec currentPosition;
    Vec currentXYZ;
    float currentTheta, currentPhi;
    float currentVelocity;
    Vec computeEndPoint(Vec moveXYZ, float theta, float phi);
    float modelScale;

    QList<GCode*> _gcodeList;
    Lines *lineList;
    int _currBlockNumber;
};

#endif // GIQGLVIEWER_H
