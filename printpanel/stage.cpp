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

// Stage action
QString Stage::sendCommandDirectly(QString &cmd)
{
    if (!couldOpenSerialPort) return errorString;

    QString sendRequest, debugMessage;
    QString stx, etx; // stx = Start of TeXt, etx = End of TeXt
    switch (this->company) {
    case EnumList::TechnoHands:
        stx = ">";
        etx = "\r";
        break;
    default:
        break;
    }

    sendRequest.append(stx);
    sendRequest.append(cmd);
    sendRequest.append(etx);

    QByteArray requestData = sendRequest.toLocal8Bit();


    debugMessage = QString("<stage:%1> ").arg(axisString);
    emit sendDebugMessage(debugMessage + sendRequest);
    serial->write(requestData);
    QByteArray responseData = serial->readAll();
    while (serial->waitForReadyRead(100))
    {
        responseData += serial->readAll();
    }
    QString response(responseData);

    emit sendDebugMessage(debugMessage + response);

    res = new ResponseAnalyzer();
    res->parseTechnoHandsResponseText(response);

    return response;
}

void Stage::moveAbsolute(float val)
{
    QString cmd = QString("ma %1").arg(val);

    if (sendCommandDirectly(cmd) == errorString) return;
}

void Stage::moveRelative(float val)
{
    QString cmd = QString("mr %1").arg(val);

    if (sendCommandDirectly(cmd) == errorString) return;
}

void Stage::moveHome()
{
     QString cmd = QString("home");

    if (sendCommandDirectly(cmd) == errorString) return;
}

void Stage::stop()
{
    QString cmd = QString("stop");

    if (sendCommandDirectly(cmd) == errorString) return;
}

float Stage::getCurrentPosition()
{
    QString cmd = QString("cp");

    if (sendCommandDirectly(cmd) == errorString) return 0;

    return res->currentPosition;
}

// Setting

void Stage::read(const QJsonObject &json)
{
    portName = json["portName"].toString();
    waitTime = json["waitTime"].toInt();
    baudrate = json["baudrate"].toInt();
    parity   = (QSerialPort::Parity)json["parity"].toInt();
    stopbits = (QSerialPort::StopBits)json["stopbits"].toInt();
    company  = (EnumList::Company)json["company"].toInt();
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
    serial->close();
}
