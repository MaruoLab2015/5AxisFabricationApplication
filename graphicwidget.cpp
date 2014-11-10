#include "graphicwidget.h"

#include <QGridLayout>
#include <QDebug>

GraphicWidget::GraphicWidget(QWidget *parent) :
    QWidget(parent)
{

    QGridLayout* layout = new QGridLayout();
//    layout->addWidget(renderer);

    setLayout(layout);
}
