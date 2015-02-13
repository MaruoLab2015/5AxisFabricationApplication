#ifndef GRAPHICWIDGET_H
#define GRAPHICWIDGET_H

#include <QWidget>

class GCode;
class GIQGLViewer;

class GraphicWidget : public QWidget
{
    Q_OBJECT
public:
    explicit GraphicWidget(QWidget *parent = 0);

    GIQGLViewer *viewer;

    void computeCurrPos();
signals:

public slots:
    void drawLines(QList<GCode*> lines);
    void changedCurrBlockNumber(int blockNumber);
    void receivedCurrentFabricationGCodeNumber(int lineNumber);

};

#endif // GRAPHICWIDGET_H
