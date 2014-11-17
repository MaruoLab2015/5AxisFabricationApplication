#include "stage.h"
#include "model/responseanalyzer.h"

#include <QJsonObject>
#include <QDebug>

Stage::Stage()
{
}

Stage::Stage(EnumList::Axis stageAxis):
    couldOpenSerialPort(false)
  ,errorString(QString("Not connected"))

{
    this->axis = stageAxis;
    switch (axis) {
    case EnumList::x:
        axisString = QString("X");
        break;
    case EnumList::y:
        axisString = QString("Y");
        break;
    default:
        break;
    }
}

// Setting

void Stage::read(const QJsonObject &json)
{
    portName = json["portName"].toString();
    waitTime = json["waitTime"].toInt();
    baudrate = json["baudrate"].toInt();
    parity   = (QSerialPort::Parity)json["parity"].toInt();
    stopbits = (QSerialPort::StopBits)json["stopbits"].toInt();
}

bool Stage::openSerialPort()
{
    serial = new QSerialPort();

    serial->setPortName(portName);
    serial->setBaudRate(baudrate);
    serial->setParity(parity);
    serial->setStopBits(stopbits);
    serial->setDataBits(QSerialPort::Data8);

    couldOpenSerialPort = serial->open(QIODevice::ReadWrite);
    if (couldOpenSerialPort)
        qDebug() << QString("Open %1").arg(portName);
    else
        qDebug() << QString("Couldn't open %1").arg(portName);

    return couldOpenSerialPort;
}

void Stage::closeSerialPort()
{
    couldOpenSerialPort = false;
    serial->close();
}
