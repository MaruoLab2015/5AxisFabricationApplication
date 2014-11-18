#include "technostage.h"
#include "model/responseanalyzer.h"

#include <QDebug>

TechnoStage::TechnoStage(QObject */*parent*/)
{
    couldOpenSerialPort = false;
}

/* Stage action */
QString TechnoStage::sendCommandDirectly(QString &cmd)
{
    if (!couldOpenSerialPort) return errorString;

    QString sendRequest, debugMessage;
    QString stx, etx; // stx = Start of TeXt, etx = End of TeXt
    stx = ">";
    etx = "\r";

    sendRequest.append(stx);
    sendRequest.append(cmd);
    sendRequest.append(etx);

    QByteArray requestData = sendRequest.toLocal8Bit();


    debugMessage = QString("<stage:%1> ").arg(axisString);
    qDebug() << debugMessage << sendRequest;
    serial->write(requestData);
    QByteArray responseData = serial->readAll();
    while (serial->waitForReadyRead(100))
    {
        responseData += serial->readAll();
    }
    QString response(responseData);

    qDebug() << debugMessage << response;

    res = new ResponseAnalyzer();
    res->parseTechnoHandsResponseText(response);

    return response;
}

void TechnoStage::moveAbsolute(float val)
{
    QString cmd = QString("ma %1").arg(val);

    if (sendCommandDirectly(cmd) == errorString) return;
}

void TechnoStage::moveRelative(float val)
{
    QString cmd = QString("mr %1").arg(val);

    if (sendCommandDirectly(cmd) == errorString) return;
}

void TechnoStage::moveHome()
{
     QString cmd = QString("home");

    if (sendCommandDirectly(cmd) == errorString) return;
}

void TechnoStage::stop()
{
    QString cmd = QString("stop");

    if (sendCommandDirectly(cmd) == errorString) return;
}

float TechnoStage::getCurrentPosition()
{
    QString cmd = QString("cp");

    if (sendCommandDirectly(cmd) == errorString) return 0;

    return res->currentPosition;
}
