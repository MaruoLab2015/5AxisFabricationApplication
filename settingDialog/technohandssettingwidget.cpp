#include "technohandssettingwidget.h"
#include "ui_technohandssettingwidget.h"
#include "printpanel/stagewidget.h"

TechnohandsSettingWidget::TechnohandsSettingWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TechnohandsSettingWidget)
{
    ui->setupUi(this);

    createTabWithTitle(tr("Φ軸"));
    createTabWithTitle(tr("Z軸(樹脂供給用)"));
    createTabWithTitle(tr("θ軸(樹脂供給用)"));

}

TechnohandsSettingWidget::~TechnohandsSettingWidget()
{
    delete ui;
}

StageWidget* TechnohandsSettingWidget::createTabWithTitle(QString title)
{
    StageWidget *aTabWidget = new StageWidget();
    ui->tabWidget->addTab(aTabWidget, QIcon(), title);
//    connect(aTabWidget->dialogButtonBox, SIGNAL(clicked(QAbstractButton*)), this, SLOT(onButtonBoxClicked(QAbstractButton*)));

    return aTabWidget;

}


