#ifndef GIQGLVIEWER_H
#define GIQGLVIEWER_H

#include "QGLViewer/qglviewer.h"

class GIQGLViewer : public QGLViewer
{
    Q_OBJECT
public:
    explicit GIQGLViewer(QWidget *parent = 0);

protected:
    virtual void draw();
    virtual void init();
    virtual void animate();

private:
    void initCamera();
signals:

public slots:

};

#endif // GIQGLVIEWER_H
