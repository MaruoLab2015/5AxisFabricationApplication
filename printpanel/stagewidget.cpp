#include "stagewidget.h"
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QMetaEnum>
#include <QMetaObject>

void initialSerialComboBoxButton(QString *name, QComboBox *box);

StageWidget::StageWidget(QWidget *parent) :
    QWidget(parent)
{

    initialLayout();
    initialComboBoxContent();
    initialSignalsAndSlotsConnect();
}

void StageWidget::initialSignalsAndSlotsConnect()
{
    connect(companyComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(companyCurrentIndexChanged(int)));
}

void StageWidget::initialLayout()
{

    QLabel *portLabel = new QLabel(tr("port"));
    QLabel *companyLabel = new QLabel(tr("company"));
    QLabel *baudrateLabel = new QLabel(tr("baudrate"));
    QLabel *stopbitsLabel = new QLabel(tr("stopbits"));
    QLabel *parityLabel = new QLabel(tr("parity"));
    QLabel *waitTimeLabel = new QLabel(tr("waitTime[ms] (1000~10000)"));
    dialogButtonBox = new QDialogButtonBox(QDialogButtonBox::Ok |
                                                             QDialogButtonBox::Cancel);

    portComboBox = new QComboBox(this);
    companyComboBox = new QComboBox(this);
    baudrateComboBox = new QComboBox(this);
    stopbitsComboBox = new QComboBox(this);
    parityComboBox = new QComboBox(this);
    waitTimeSpinBox = new QSpinBox(this);

    waitTimeSpinBox->setRange(1000, 10000);

    QGridLayout *layout = new QGridLayout;

    layout->addWidget(portLabel, 0, 0);
    layout->addWidget(portComboBox, 0, 1);
    layout->addWidget(companyLabel, 1, 0);
    layout->addWidget(companyComboBox, 1, 1);
    layout->addWidget(baudrateLabel, 2, 0);
    layout->addWidget(baudrateComboBox, 2, 1);
    layout->addWidget(stopbitsLabel, 3, 0);
    layout->addWidget(stopbitsComboBox, 3, 1);
    layout->addWidget(parityLabel, 4, 0);
    layout->addWidget(parityComboBox, 4, 1);
    layout->addWidget(waitTimeLabel, 5, 0);
    layout->addWidget(waitTimeSpinBox, 5, 1);
    layout->addWidget(dialogButtonBox, 6, 1);

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

    initialCompanyComboBoxButton();
}

void initialSerialComboBoxButton(QString *name, QComboBox *box)
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

void StageWidget::initialCompanyComboBoxButton()
{

    const QMetaObject *metaObj = this->metaObject();
    QMetaEnum enumType = metaObj->enumerator(metaObj->indexOfEnumerator("Company"));

    for (int i=0; i< enumType.keyCount(); ++i)
    {
        QString item = QString::fromUtf8(enumType.key(i));
        companyComboBox->addItem(item);
    }
}

/* SLOT */

const int technoHands = 0;
const int sigmaStage = 1;
const int shutter = 2;

void StageWidget::companyCurrentIndexChanged(int index)
{
    qDebug() << index;
    switch (index) {
    case technoHands:
        baudrateComboBox->setCurrentIndex(7);
        break;
    case sigmaStage:
        baudrateComboBox->setCurrentIndex(5);
        break;
    case shutter:
        baudrateComboBox->setCurrentIndex(3);
        break;
    default:
        return;
    }
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

    baudrateComboBox->setCurrentIndex(json["baudrateIndex"].toInt());
    stopbitsComboBox->setCurrentIndex(json["stopbitsIndex"].toInt());
    parityComboBox->setCurrentIndex(json["parityIndex"].toInt());
    waitTimeSpinBox->setValue(json["waitTime"].toInt());
    companyComboBox->setCurrentIndex(json["company"].toInt());
}

void StageWidget::write(QJsonObject &json) const
{
    const QSerialPort *serial = new QSerialPort;

    const QMetaObject *metaObj = serial->metaObject();
    QMetaEnum baudrateEnumType = metaObj->enumerator(metaObj->indexOfEnumerator("BaudRate"));
    QMetaEnum stopBitsEnumType = metaObj->enumerator(metaObj->indexOfEnumerator("StopBits"));
    QMetaEnum parityEnumType = metaObj->enumerator(metaObj->indexOfEnumerator("Parity"));

    json["portName"] = portComboBox->currentText();
    json["baudrateIndex"] = baudrateComboBox->currentIndex();
    json["stopbitsIndex"] = stopbitsComboBox->currentIndex();
    json["parityIndex"] = parityComboBox->currentIndex();
    json["baudrate"]      = baudrateEnumType.value(baudrateComboBox->currentIndex()) ;
    json["stopbits"]      = stopBitsEnumType.value(stopbitsComboBox->currentIndex());
    json["parity"]        = parityEnumType.value(parityComboBox->currentIndex());
    json["waitTime"] = waitTimeSpinBox->value();
    json["company"] = companyComboBox->currentIndex();
}
