#include "graphicwidget.h"

#include <QGridLayout>
#include <QDebug>

GraphicWidget::GraphicWidget(QWidget *parent) :
    QWidget(parent)
{

    qDebug() << "layout";
    Renderer *renderer = new Renderer();

    Scene *scene = new Scene();

    QGridLayout* layout = new QGridLayout();
    layout->addWidget(renderer);

    setLayout(layout);
}
