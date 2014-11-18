#include "sigmastage.h"

#include <QDebug>
SigmaStage::SigmaStage(QObject */*parent*/)
{
    couldOpenSerialPort = false;
}

QString SigmaStage::sendCommandDirectly(QString &cmd)
{
    if(!couldOpenSerialPort) return errorString;

    QString sendRequest, debugMessage;
    QString stx, etx; // stx = Start of TeXt, etx = End of TeXt
    stx = ">";
    etx = "\r\n";

//    sendRequest.append(stx);
    qDebug() << "send";
    sendRequest.append(cmd);
    sendRequest.append(etx);

    QByteArray requestData = sendRequest.toLocal8Bit();


    debugMessage = QString("<stage:%1> ").arg(axisString);
    qDebug() << debugMessage << sendRequest.trimmed();
    serial->write(requestData);
    QByteArray responseData = serial->readAll();
    while (serial->waitForReadyRead(100))
    {
        responseData += serial->readAll();
    }
    QString response(responseData);

    qDebug() << debugMessage << response.trimmed();

//    res = new ResponseAnalyzer();
//    res->parseTechnoHandsResponseText(response);

    return response;

}

void SigmaStage::moveAbsolute(float val)
{

}

void SigmaStage::moveRelative(float val)
{

}

void SigmaStage::moveHome()
{

}

void SigmaStage::stop()
{

}

float SigmaStage::getCurrentPosition()
{

}
