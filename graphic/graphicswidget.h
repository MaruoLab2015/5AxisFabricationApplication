#ifndef GRAPHICSWIDGET_H
#define GRAPHICSWIDGET_H

#include <QGLWidget>

class GraphicsWidget : public QGLWidget
{
    Q_OBJECT
public:
    explicit GraphicsWidget(QWidget *parent = 0);

signals:

public slots:

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

private:
    void drawAxis(int size);
    void drawTile(float size);

    void setXRotation(int angle);
    void setYRotation(int angle);
    void setZRotation(int angle);
    void qNormalizeAngle(int &angle);

    int xRot;
    int yRot;
    int zRot;

    QPoint lastPos;
};

#endif // GRAPHICSWIDGET_H
