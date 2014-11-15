#ifndef LINES_H
#define LINES_H

#include "gcode.h"
#include <QGLViewer/vec.h>

class Lines
{
public:
    Lines();
    Lines(QList<GCode*> gcodeList);

    QList<qglviewer::Vec*> vertices;
    QList<bool> shutterFlagList;

    void computedBoudingBox();
    qglviewer::Vec size, center, min, max;

    double maxSize() {return std::max(size.z, std::max(size.x, size.y));}
};

#endif // LINES_H
