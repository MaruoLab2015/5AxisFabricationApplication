#include "stagesettingdialog.h"
#include "ui_stagesettingdialog.h"
#include "printpanel/stagewidget.h"
#include "settingDialog/sigmasetitngwidget.h"
#include "settingDialog/technohandssettingwidget.h"

#include <QSerialPortInfo>
#include <QDebug>
#include <QSerialPort>
#include <QMetaObject>
#include <QMetaEnum>
#include <QJsonObject>
#include <QStackedWidget>

const QString technoKey = "Techno";
const QString sigmaKey = "sigma";
const QString shutterKey = "shutter";

void setSerialComboBoxButton(QString *name, QComboBox *box);

StageSettingDialog::StageSettingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StageSettingDialog)
{
    ui->setupUi(this);

    sigmaWidget = new SigmaSetitngWidget;
    ui->stackedWidget->addWidget(sigmaWidget);

    technoWidget = new TechnohandsSettingWidget;
    ui->stackedWidget->addWidget(technoWidget);

    shutterWidget = new StageWidget;
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
    sigmaWidget->read(json[sigmaKey].toObject());
    technoWidget->read(json[technoKey].toObject());
    shutterWidget->read(json[shutterKey].toObject());
}


void StageSettingDialog::write(QJsonObject &json) const
{

    QJsonObject technoObject, sigmaObject, shutterObject;
    sigmaWidget->write(sigmaObject);
    technoWidget->write(technoObject);
    shutterWidget->write(shutterObject);

    json[sigmaKey] = sigmaObject;
    json[technoKey] = technoObject;
    json[shutterKey] = shutterObject;
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
