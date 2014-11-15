#include "sigmasetitngwidget.h"
#include "ui_sigmasetitngwidget.h"

#include <QSerialPort>
#include <QSerialPortInfo>
#include <QMetaEnum>
#include <QMetaObject>
#include <QComboBox>

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
