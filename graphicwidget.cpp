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
    qDebug() << "recerived gcode";

    viewer->drawLines(gcodeList);

}
