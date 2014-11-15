#include "stagesettingdialog.h"
#include "ui_stagesettingdialog.h"
#include "printpanel/stagewidget.h"
#include "settingDialog/sigmasetitngwidget.h"
#include "settingDialog/technohandssettingwidget.h"
#include "settingDialog/shuttersettingwidget.h"

#include <QSerialPortInfo>
#include <QDebug>
#include <QSerialPort>
#include <QMetaObject>
#include <QMetaEnum>
#include <QJsonObject>
#include <QStackedWidget>

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

    SigmaSetitngWidget *sigmaWidget = new SigmaSetitngWidget;
    ui->stackedWidget->addWidget(sigmaWidget);

    TechnohandsSettingWidget *technoWidget = new TechnohandsSettingWidget;
    ui->stackedWidget->addWidget(technoWidget);

    ShutterSettingWidget *shutterWidget = new ShutterSettingWidget;
    ui->stackedWidget->addWidget(shutterWidget);

    connect(ui->settingComboBox, SIGNAL(currentIndexChanged(int)),
            ui->stackedWidget, SLOT(setCurrentIndex(int)));

}


StageSettingDialog::~StageSettingDialog()
{
    delete ui;
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
//    xAxisWidget->read(json[xKey].toObject());
//    yAxisWidget->read(json[yKey].toObject());
//    zAxisWidget->read(json[zKey].toObject());
//    thetaAxisWidget->read(json[thetaKey].toObject());
//    phiAxisWidget->read(json[phiKey].toObject());
//    shutterWidget->read(json[shutterKey].toObject());
}


void StageSettingDialog::write(QJsonObject &json) const
{

//    QJsonObject xAxisObject, yAxisObject, zAxisObject,
//            thetaAxisObject, phiAxisObject, shutterObject;
//    xAxisWidget->write(xAxisObject);
//    yAxisWidget->write(yAxisObject);
//    zAxisWidget->write(zAxisObject);
//    thetaAxisWidget->write(thetaAxisObject);
//    phiAxisWidget->write(phiAxisObject);
//    shutterWidget->write(shutterObject);

//    json[xKey] = xAxisObject;
//    json[yKey] = yAxisObject;
//    json[zKey] = zAxisObject;
//    json[thetaKey] = thetaAxisObject;
//    json[phiKey] = phiAxisObject;
//    json[shutterKey] = shutterObject;
}

void StageSettingDialog::on_buttonBox_clicked(QAbstractButton *button)
{
    if (button->text() == "OK")
    {
        saveStageSettingss(StageSettingDialog::Json);
        emit applySettings();
    }

    this->close();

}
