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
        qDebug() << "ok";
    }else if (button->text() == "Cancel")
    {
        qDebug() << "cancel";
    }

    this->close();
}

void setSerialComboBoxButton(QString *name, QComboBox *box)
{

    const QSerialPort *serial = new QSerialPort;
    qDebug() << name->toLocal8Bit().constData();

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
    qDebug() << enumType.keyCount();

    for (int i=0; i< enumType.keyCount(); ++i)
    {
        QString item = QString::fromUtf8(enumType.key(i));
        ui->campanyComboBox->addItem(item);
    }
}
