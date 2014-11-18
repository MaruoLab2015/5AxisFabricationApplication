#include "stagewidget.h"
#include "EnumList.h"

#include <QSerialPort>
#include <QSerialPortInfo>
#include <QMetaEnum>
#include <QMetaObject>


StageWidget::StageWidget(QWidget *parent) :
    QWidget(parent)
{
    initialLayout();
    initialComboBoxContent();
}

void StageWidget::initialLayout()
{
    QLabel *portLabel = new QLabel(tr("portName"));
    QLabel *baudrateLabel = new QLabel(tr("baudrate"));
    QLabel *stopbitsLabel = new QLabel(tr("stopbits"));
    QLabel *parityLabel = new QLabel(tr("parity"));
    QLabel *enableLabel = new QLabel(tr("接続しない"));

    portComboBox = new QComboBox(this);
    baudrateComboBox = new QComboBox(this);
    stopbitsComboBox = new QComboBox(this);
    parityComboBox = new QComboBox(this);
    isEnableCheckBox = new QCheckBox(this);
    connect(isEnableCheckBox, SIGNAL(clicked(bool)), this, SLOT(enableCheckBoxChanged(bool)));

    QGridLayout *layout = new QGridLayout;

    layout->addWidget(portLabel, 0, 0);
    layout->addWidget(portComboBox, 0, 1);
    layout->addWidget(baudrateLabel, 1, 0);
    layout->addWidget(baudrateComboBox, 1, 1);
    layout->addWidget(stopbitsLabel, 2, 0);
    layout->addWidget(stopbitsComboBox, 2, 1);
    layout->addWidget(parityLabel, 3, 0);
    layout->addWidget(parityComboBox, 3, 1);
    layout->addWidget(enableLabel, 4,0);
    layout->addWidget(isEnableCheckBox, 4, 1);

    this->setLayout(layout);
}

void StageWidget::initialComboBoxContent()
{
    // port
    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        portComboBox->addItem(info.portName());
    }

    // other settings
    initialSerialComboBoxButton(new QString("BaudRate"), baudrateComboBox);
    initialSerialComboBoxButton(new QString("Parity"), parityComboBox);
    initialSerialComboBoxButton(new QString("StopBits"), stopbitsComboBox);
}

void StageWidget::initialSerialComboBoxButton(QString *name, QComboBox *box)
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

/* SLOT */

void StageWidget::enableCheckBoxChanged(bool isEnable)
{
    portComboBox->setDisabled(isEnable);
    baudrateComboBox->setDisabled(isEnable);
    stopbitsComboBox->setDisabled(isEnable);
    parityComboBox->setDisabled(isEnable);
}

/* save and load */

void StageWidget::read(const QJsonObject &json)
{
    QString portName = json["portName"].toString();
    for (int i=0;i < portComboBox->count(); i++)
    {
        if ( portName == portComboBox->itemText(i))
        {
            portComboBox->setCurrentIndex(i);
            break;
        }
    }

    baudrateComboBox->setCurrentIndex(json[baudrateIndexKey].toInt());
    stopbitsComboBox->setCurrentIndex(json[stopbitsIndexKey].toInt());
    parityComboBox->setCurrentIndex(json[parityIndexKey].toInt());
    isEnableCheckBox->setCheckState((Qt::CheckState)json[disableKey].toInt());
    isEnableCheckBox->clicked(isEnableCheckBox->checkState());
}

void StageWidget::write(QJsonObject &json) const
{
    const QSerialPort *serial = new QSerialPort;

    const QMetaObject *metaObj = serial->metaObject();
    QMetaEnum baudrateEnumType = metaObj->enumerator(metaObj->indexOfEnumerator("BaudRate"));
    QMetaEnum stopBitsEnumType = metaObj->enumerator(metaObj->indexOfEnumerator("StopBits"));
    QMetaEnum parityEnumType = metaObj->enumerator(metaObj->indexOfEnumerator("Parity"));

    json[portNameKey] = portComboBox->currentText();
    json[baudrateIndexKey] = baudrateComboBox->currentIndex();
    json[stopbitsIndexKey] = stopbitsComboBox->currentIndex();
    json[parityIndexKey] = parityComboBox->currentIndex();
    json[baudrateKey]      = baudrateEnumType.value(baudrateComboBox->currentIndex()) ;
    json[stopbitsKey]      = stopBitsEnumType.value(stopbitsComboBox->currentIndex());
    json[parityKey]        = parityEnumType.value(parityComboBox->currentIndex());
    json[disableKey]       = isEnableCheckBox->checkState();
}
