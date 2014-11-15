#include "sigmasetitngwidget.h"
#include "ui_sigmasetitngwidget.h"

SigmaSetitngWidget::SigmaSetitngWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SigmaSetitngWidget)
{
    ui->setupUi(this);
}

SigmaSetitngWidget::~SigmaSetitngWidget()
{
    delete ui;
}
