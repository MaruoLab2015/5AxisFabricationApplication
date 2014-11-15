#include "technohandssettingwidget.h"
#include "ui_technohandssettingwidget.h"
#include "printpanel/stagewidget.h"

#include <QJsonObject>

const QString thetaSuppliedKey = "thetaSupply";
const QString zSuppliedKey = "zSupply";
const QString phiKey = "phiaxis";

TechnohandsSettingWidget::TechnohandsSettingWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TechnohandsSettingWidget)
{
    ui->setupUi(this);

    phiAxisWidget = createTabWithTitle(tr("Φ軸"));
    zAxisSuppliedWidget = createTabWithTitle(tr("Z軸(樹脂供給用)"));
    thetaAxisSuppliedWidget =  createTabWithTitle(tr("θ軸(樹脂供給用)"));

}

TechnohandsSettingWidget::~TechnohandsSettingWidget()
{
    delete ui;
}

StageWidget* TechnohandsSettingWidget::createTabWithTitle(QString title)
{
    StageWidget *aTabWidget = new StageWidget();
    ui->tabWidget->addTab(aTabWidget, QIcon(), title);

    return aTabWidget;
}

void TechnohandsSettingWidget::read(const QJsonObject &json)
{
    phiAxisWidget->read(json[phiKey].toObject());
    zAxisSuppliedWidget->read(json[zSuppliedKey].toObject());
    thetaAxisSuppliedWidget->read(json[thetaSuppliedKey].toObject());
}

void TechnohandsSettingWidget::write(QJsonObject &json) const
{
    QJsonObject phiObject, zAxisSuppliedObject, thetaAxisSuppliedObject;

    phiAxisWidget->write(phiObject);
    zAxisSuppliedWidget->write(zAxisSuppliedObject);
    thetaAxisSuppliedWidget->write(thetaAxisSuppliedObject);

    json[phiKey] = phiObject;
    json[zSuppliedKey] = zAxisSuppliedObject;
    json[thetaSuppliedKey] = thetaAxisSuppliedObject;
}



