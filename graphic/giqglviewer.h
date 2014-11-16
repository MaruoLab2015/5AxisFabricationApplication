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

protected:
    //    virtual void fastDraw();
    virtual void draw();
    virtual void init();
    virtual void animate();

private:
    void initCamera();

    QList<GCode*> _gcodeList;
    Lines *lineList;
    int _currBlockNumber;
};

#endif // GIQGLVIEWER_H
