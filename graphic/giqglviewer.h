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

protected:
//    virtual void fastDraw();
    virtual void draw();
    virtual void init();
    virtual void animate();

private:
    void initCamera();

    QList<GCode*> _gcodeList;
    Lines *lineList;
signals:

public slots:
    void drawLines(QList<GCode*> lines);
};

#endif // GIQGLVIEWER_H
