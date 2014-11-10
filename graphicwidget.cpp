#include "graphicwidget.h"
#include "graphic/giqglviewer.h"

#include <QGridLayout>
#include <QDebug>

GraphicWidget::GraphicWidget(QWidget *parent) :
    QWidget(parent)
{

    GIQGLViewer *viewer = new GIQGLViewer();
    QGridLayout* layout = new QGridLayout();
    layout->addWidget(viewer);

    setLayout(layout);
}
