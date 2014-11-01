#ifndef GCODEANALYZER_H
#define GCODEANALYZER_H

#include <QObject>

#include "enumList.h"
#include "gcode.h"

class GCodeAnalyzer : public QObject
{
    Q_OBJECT
public:
    explicit GCodeAnalyzer(QObject *parent = 0);

    EnumList::StageShutter e;
    float x, y, z;
    float f;
    float lastX, lastY, lastZ;
    float xOffset, yOffset, zOffset, layerZ;
    bool hasXHome, hasYHome, hasZHome;
    int layer, lastlayer;
    bool isG1Move;
    double printingTime;
    bool relative;

    float realX(){ return x + xOffset;};
    float realY(){ return y + yOffset;};
    float realZ(){ return z + zOffset;};

    void start(bool fire);
    void startJob();
    void analyze(GCode code);

signals:
    void onPositionChange();

public slots:

};

#endif // GCODEANALYZER_H
