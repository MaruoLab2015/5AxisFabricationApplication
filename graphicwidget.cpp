#include "graphicwidget.h"
#include "graphic/giqglviewer.h"
#include "model/gcode.h"

#include <QGridLayout>
#include <QDebug>

GraphicWidget::GraphicWidget(QWidget *parent) :
    QWidget(parent)
{
    viewer = new GIQGLViewer();
    QGridLayout* layout = new QGridLayout();
    layout->addWidget(viewer);

    setLayout(layout);
}

void GraphicWidget::drawLines(QList<GCode*> gcodeList)
{
    viewer->drawLines(gcodeList);
}

void GraphicWidget::changedCurrBlockNumber(int blockNumber)
{
    viewer->setCurrBlockNumber(blockNumber);
}

void GraphicWidget::receivedCurrentFabricationGCodeNumber(int lineNumber)
{
    viewer->setCurrBlockNumber(lineNumber);
}
