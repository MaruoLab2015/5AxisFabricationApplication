#include "stagesettingdialog.h"
#include "ui_stagesettingdialog.h"
#include "stagewidget.h"

#include <QSerialPortInfo>
#include <QDebug>
#include <QSerialPort>
#include <QMetaObject>
#include <QMetaEnum>
#include <QJsonObject>

const QString xKey = "xaxis";
const QString yKey = "yaxis";
const QString zKey = "zaxis";
const QString thetaKey = "thetaaxis";
const QString phiKey = "phiaxis";
const QString shutterKey = "shutter";

void setSerialComboBoxButton(QString *name, QComboBox *box);

StageSettingDialog::StageSettingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StageSettingDialog)
{

    ui->setupUi(this);

    initialTabs();
}

StageSettingDialog::~StageSettingDialog()
{
    delete ui;
}

void StageSettingDialog::initialTabs()
{

    xAxisWidget = createTabWithTitle(tr("X軸"));
    yAxisWidget = createTabWithTitle(tr("Y軸"));
    zAxisWidget = createTabWithTitle(tr("Z軸"));
    thetaAxisWidget = createTabWithTitle(tr("θ軸"));
    phiAxisWidget = createTabWithTitle(tr("Φ軸"));
    shutterWidget = createTabWithTitle(tr("シャッター"));
}

StageWidget* StageSettingDialog::createTabWithTitle(QString title)
{

    StageWidget *aTabWidget = new StageWidget();
    ui->tabWidget->addTab(aTabWidget, QIcon(), title);
    connect(aTabWidget->dialogButtonBox, SIGNAL(clicked(QAbstractButton*)), this, SLOT(onButtonBoxClicked(QAbstractButton*)));
    return aTabWidget;
}

void StageSettingDialog::onButtonBoxClicked(QAbstractButton *button)
{
    if (button->text() == "OK")
    {
        saveStageSettingss(StageSettingDialog::Json);
    }

    this->close();
}

bool StageSettingDialog::saveStageSettingss(SaveFormat saveFormat) const
{

    QFile saveFile(saveFormat == Json
                   ? QStringLiteral("save.json")
                   : QStringLiteral("save.dat"));

    if (!saveFile.open(QIODevice::WriteOnly))
    {
        qWarning("Couldn't open save File");
        return false;
    }

    QJsonObject settingObject;
    write(settingObject);

    QJsonDocument saveDoc(settingObject);
    saveFile.write(saveFormat == Json
                   ? saveDoc.toJson()
                   : saveDoc.toBinaryData());

    return true;
}


void StageSettingDialog::read(const QJsonObject &json)
{
    xAxisWidget->read(json[xKey].toObject());
    yAxisWidget->read(json[yKey].toObject());
    zAxisWidget->read(json[zKey].toObject());
    thetaAxisWidget->read(json[thetaKey].toObject());
    phiAxisWidget->read(json[phiKey].toObject());
    shutterWidget->read(json[shutterKey].toObject());
}


void StageSettingDialog::write(QJsonObject &json) const
{

    QJsonObject xAxisObject, yAxisObject, zAxisObject,
            thetaAxisObject, phiAxisObject, shutterObject;
    xAxisWidget->write(xAxisObject);
    yAxisWidget->write(yAxisObject);
    zAxisWidget->write(zAxisObject);
    thetaAxisWidget->write(thetaAxisObject);
    phiAxisWidget->write(phiAxisObject);
    shutterWidget->write(shutterObject);

    json[xKey] = xAxisObject;
    json[yKey] = yAxisObject;
    json[zKey] = zAxisObject;
    json[thetaKey] = thetaAxisObject;
    json[phiKey] = phiAxisObject;
    json[shutterKey] = shutterObject;
}
