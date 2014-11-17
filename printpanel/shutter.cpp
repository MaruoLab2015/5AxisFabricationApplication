#include "shutter.h"

#include <QJsonObject>
#include <QDebug>

Shutter::Shutter(QObject */*parent*/)
{
}

bool Shutter::openSerialPort()
{
    serial = new QSerialPort();

    serial->setPortName(portName);
    serial->setBaudRate(baudrate);
    serial->setParity(parity);
    serial->setStopBits(stopbits);
    serial->setDataBits(QSerialPort::Data8);

    couldOpenSerialPort = serial->open(QIODevice::ReadWrite);
    return couldOpenSerialPort;
}

void Shutter::read(const QJsonObject &json)
{
    portName = json["portName"].toString();
    waitTime = json["waitTime"].toInt();
    baudrate = json["baudrate"].toInt();
    parity   = (QSerialPort::Parity)json["parity"].toInt();
    stopbits = (QSerialPort::StopBits)json["stopbits"].toInt();
}

void Shutter::open()
{
    if (!couldOpenSerialPort) return;

    QString sendRequest = QString("SH 1,1\r\n");
    QByteArray requestData = sendRequest.toLocal8Bit();
    QString debugMessage = QString("<Shutter> open");
    qDebug() << debugMessage;
    serial->write(requestData);
}

void Shutter::close()
{
    if (!couldOpenSerialPort) return;

    QString sendRequest = QString("SH 1,0\r\n");
    QByteArray requestData = sendRequest.toLocal8Bit();
    QString debugMessage = QString("<Shutter> close");
    qDebug() << debugMessage;
    serial->write(requestData);

}
