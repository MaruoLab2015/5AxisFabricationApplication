#include "graphicwidget.h"
#include "graphic/graphicswidget.h"

#include <QGridLayout>
#include <QDebug>

GraphicWidget::GraphicWidget(QWidget *parent) :
    QWidget(parent)
{

    qDebug() << "layout";
    Renderer *renderer = new Renderer();

    Scene *scene = new Scene();

    QGridLayout* layout = new QGridLayout();

//    layout->addWidget(new GraphicsWidget(),1,0);

    layout->addWidget(renderer);

    setLayout(layout);
}
