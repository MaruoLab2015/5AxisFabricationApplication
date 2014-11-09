#ifndef RENDERER_H
#define RENDERER_H

#include <QGLWidget>

class Renderer : public QGLWidget
{
    Q_OBJECT
public:
    explicit Renderer(QWidget *parent = 0);

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);

signals:

public slots:

};

#endif // RENDERER_H
