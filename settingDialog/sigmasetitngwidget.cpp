#include "sigmasetitngwidget.h"
#include "ui_sigmasetitngwidget.h"

#include <QSerialPort>
#include <QSerialPortInfo>
#include <QMetaEnum>
#include <QMetaObject>
#include <QComboBox>
#include <QJsonObject>

SigmaSetitngWidget::SigmaSetitngWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SigmaSetitngWidget)
{
    ui->setupUi(this);

    initialComboBoxContent();
}

SigmaSetitngWidget::~SigmaSetitngWidget()
{
    delete ui;
}



void SigmaSetitngWidget::initialComboBoxContent()
{
    // sigmaAxis
    initSigmaAxisComboBox();

    // port
    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        ui->portNameComboBox->addItem(info.portName());
    }

    // other settings
    initialSerialComboBoxButton(new QString("BaudRate"), ui->baudrateComboBox);
    initialSerialComboBoxButton(new QString("Parity"), ui->parityComboBox);
    initialSerialComboBoxButton(new QString("StopBits"), ui->stopbitsComboBox);
}

void SigmaSetitngWidget::initialSerialComboBoxButton(QString *name, QComboBox *box)
{
    const QSerialPort *serial = new QSerialPort;

    const QMetaObject *metaObj = serial->metaObject();
    QMetaEnum enumType = metaObj->enumerator(metaObj->indexOfEnumerator(name->toLocal8Bit().constData()));
    for (int i=0; i< enumType.keyCount(); ++i)
    {
        QString item = QString::fromUtf8(enumType.key(i));
        box->addItem(item);
    }
}

void SigmaSetitngWidget::initSigmaAxisComboBox()
{
    const QMetaObject *metaObj = this->metaObject();
    QMetaEnum enumType = metaObj->enumerator(metaObj->indexOfEnumerator("SigmaAxis"));
    for (int i=0; i< enumType.keyCount(); ++i)
    {
        QString item = QString::fromUtf8(enumType.key(i));
        ui->axis1ComboBox->addItem(item);
        ui->axis2ComboBox->addItem(item);
        ui->axis3ComboBox->addItem(item);
        ui->axis4ComboBox->addItem(item);
    }

}

void SigmaSetitngWidget::read(const QJsonObject &json)
{
    QString portName = json["portName"].toString();
    for (int i=0;i < ui->portNameComboBox->count(); i++)
    {
        if ( portName == ui->portNameComboBox->itemText(i))
        {
            ui->portNameComboBox->setCurrentIndex(i);
            break;
        }
    }

    ui->baudrateComboBox->setCurrentIndex(json["baudrateIndex"].toInt());
    ui->stopbitsComboBox->setCurrentIndex(json["stopbitsIndex"].toInt());
    ui->parityComboBox->setCurrentIndex(json["parityIndex"].toInt());
    ui->axis1ComboBox->setCurrentIndex(json["axis1"].toInt());
    ui->axis2ComboBox->setCurrentIndex(json["axis2"].toInt());
    ui->axis3ComboBox->setCurrentIndex(json["axis3"].toInt());
    ui->axis4ComboBox->setCurrentIndex(json["axis4"].toInt());

}

void SigmaSetitngWidget::write(QJsonObject &json) const
{
    const QSerialPort *serial = new QSerialPort;

    const QMetaObject *metaObj = serial->metaObject();
    QMetaEnum baudrateEnumType = metaObj->enumerator(metaObj->indexOfEnumerator("BaudRate"));
    QMetaEnum stopBitsEnumType = metaObj->enumerator(metaObj->indexOfEnumerator("StopBits"));
    QMetaEnum parityEnumType = metaObj->enumerator(metaObj->indexOfEnumerator("Parity"));

    json["portName"] = ui->portNameComboBox->currentText();
    json["baudrateIndex"] = ui->baudrateComboBox->currentIndex();
    json["stopbitsIndex"] = ui->stopbitsComboBox->currentIndex();
    json["parityIndex"] = ui->parityComboBox->currentIndex();
    json["baudrate"]      = baudrateEnumType.value(ui->baudrateComboBox->currentIndex()) ;
    json["stopbits"]      = stopBitsEnumType.value(ui->stopbitsComboBox->currentIndex());
    json["parity"]        = parityEnumType.value(ui->parityComboBox->currentIndex());
    json["axis1"]		  = ui->axis1ComboBox->currentIndex();
    json["axis2"]		  = ui->axis2ComboBox->currentIndex();
    json["axis3"]		  = ui->axis3ComboBox->currentIndex();
    json["axis4"]		  = ui->axis4ComboBox->currentIndex();
}
