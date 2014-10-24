#include "stagesettingdialog.h"
#include "ui_stagesettingdialog.h"

#include <QSerialPortInfo>
#include <QDebug>
#include <QSerialPort>
#include <QMetaObject>
#include <QMetaEnum>

void setSerialComboBoxButton(QString *name, QComboBox *box);

StageSettingDialog::StageSettingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StageSettingDialog)
{

    ui->setupUi(this);

    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        ui->portComboBox->addItem(info.portName());
    }

    setSerialComboBoxButton(new QString("BaudRate"), ui->baudrateComboBox);
    setSerialComboBoxButton(new QString("Parity"), ui->parityComboBox);
    setSerialComboBoxButton(new QString("StopBits"), ui->stopbitComboBox);

    setCompanyComboBoxButton();


}

StageSettingDialog::~StageSettingDialog()
{
    delete ui;
}

void StageSettingDialog::on_buttonBox_clicked(QAbstractButton *button)
{
    if (button->text() == "OK")
    {
        setComboBoxSetting();
    }

    this->close();
}

void setSerialComboBoxButton(QString *name, QComboBox *box)
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

void StageSettingDialog::setCompanyComboBoxButton()
{

    const QMetaObject *metaObj = this->metaObject();
    QMetaEnum enumType = metaObj->enumerator(metaObj->indexOfEnumerator("Company"));

    for (int i=0; i< enumType.keyCount(); ++i)
    {
        QString item = QString::fromUtf8(enumType.key(i));
        ui->campanyComboBox->addItem(item);
    }
}

const int technoHands = 0;
const int sigmaStage = 1;
const int shutter = 2;

void StageSettingDialog::on_campanyComboBox_currentIndexChanged(int index)
{

    switch (index) {
    case technoHands:

        ui->baudrateComboBox->setCurrentIndex(7);
        break;

    case sigmaStage:
        ui->baudrateComboBox->setCurrentIndex(5);
        break;

    case shutter:

        ui->baudrateComboBox->setCurrentIndex(3);
        break;
    default:
        return;
    }
}

void StageSettingDialog::on_pushButton_clicked()
{
    setComboBoxSetting();
}

void StageSettingDialog::setComboBoxSetting()
{

    const QSerialPort *serial = new QSerialPort;

    const QMetaObject *metaObj = serial->metaObject();
    QMetaEnum baudrateEnumType = metaObj->enumerator(metaObj->indexOfEnumerator("BaudRate"));
    QMetaEnum stopBitsEnumType = metaObj->enumerator(metaObj->indexOfEnumerator("StopBits"));
    QMetaEnum parityEnumType = metaObj->enumerator(metaObj->indexOfEnumerator("Parity"));

    this->applySetting(
                ui->portComboBox->currentText(),
                baudrateEnumType.value(ui->baudrateComboBox->currentIndex()),
                (QSerialPort::StopBits)stopBitsEnumType.value(ui->stopbitComboBox->currentIndex()),
                (QSerialPort::Parity)parityEnumType.value(ui->parityComboBox->currentIndex()),
                ui->waitTimeSpinBox->value()
                );

}
