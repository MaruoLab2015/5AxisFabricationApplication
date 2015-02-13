#include "technostage.h"
#include "model/responseanalyzer.h"

#include <QDebug>

const float TechnoStage::kPositiveConversion = 18613.0f;
const float TechnoStage::kNegativeConversion = 18783.0f;

TechnoStage::TechnoStage(QObject */*parent*/)
{
    couldOpenSerialPort = false;
    isDebug = true;
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
//    qDebug() << debugMessage << sendRequest;
//    if (isDebug)
//        qDebug() << sendRequest;

    serial->write(requestData);
    QByteArray responseData = serial->readAll();
    while (serial->waitForReadyRead(100))
    {
        responseData += serial->readAll();
    }
    QString response(responseData);

//    if (isDebug)
//        qDebug() << debugMessage << response;

    res = new ResponseAnalyzer();
    res->parseTechnoHandsResponseText(response);

    return response;
}

void TechnoStage::moveAbsolute(float val)
{
    QString cmd;
    if (val > 0)
    {
        cmd = QString("3ma %1").arg(val / 90.0f * kPositiveConversion);
    }else
    {
        cmd = QString("3ma %1").arg(val / 90.0f * kNegativeConversion);
    }

    if (sendCommandDirectly(cmd) == errorString) return;
}

void TechnoStage::moveRelative(float val)
{
    QString cmd = QString("3mr %1").arg(val);

    if (sendCommandDirectly(cmd) == errorString) return;
}

void TechnoStage::moveHome()
{
     QString cmd = QString("3hm");

    if (sendCommandDirectly(cmd) == errorString) return;
}

void TechnoStage::stop()
{
    QString cmd = QString("3st");

    if (sendCommandDirectly(cmd) == errorString) return;
}

float TechnoStage::getCurrentPosition()
{
    QString cmd = QString("3cp");

    if (sendCommandDirectly(cmd) == errorString) return 0;

    return res->currentPosition;
}
