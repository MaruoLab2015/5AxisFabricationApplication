#include "sigmastage.h"
#include "model/responseanalyzer.h"

#include <QDebug>
#include <QJsonObject>
#include <QtMath>

SigmaStage::SigmaStage(QObject */*parent*/)
{
    couldOpenSerialPort = false;
}

QString SigmaStage::sendCommandDirectly(QString &cmd)
{
    if(!couldOpenSerialPort) return errorString;

    QString sendRequest, debugMessage;
    QString etx; // etx = End of TeXt or Delimeter
    etx = "\r\n";

    sendRequest.append(cmd);
    sendRequest.append(etx);

    QByteArray requestData = sendRequest.toLocal8Bit();

    debugMessage = QString("<stage:%1> ").arg(axisString);
    qDebug() << debugMessage << sendRequest.trimmed();
    serial->write(requestData);
    QByteArray responseData = serial->readAll();
    while (serial->waitForReadyRead(50))
    {
        responseData += serial->readAll();
    }
    QString response(responseData);

    qDebug() << debugMessage << response.trimmed();

    res = new ResponseAnalyzer();
    res->parseSigmaResponseText(response);

    return response;

}

void SigmaStage::moveAbsoluteCommand(float val, EnumList::Axis axis)
{
    QString cmd;
    if (axis == EnumList::theta)
    {
        cmd = QString("A:%1+P%2")
                .arg(axisMap[axis])
                .arg(qRound(val * CONVERSION_R));
    }else
    {
        cmd = QString("A:%1+P%2")
                .arg(axisMap[axis])
                .arg(qRound(val * CONVERSION));
    }
    if (sendCommandDirectly(cmd) == errorString) return;

}

void SigmaStage::moveRelativeCommand(float val, EnumList::Axis axis)
{
    QString cmd;
    if (axis == EnumList::theta)
    {
        if (val >= 0)
            cmd = QString("M:%1+P%2")
                    .arg(axisMap[axis])
                    .arg(qRound(val * CONVERSION_R));
        else
            cmd = QString("M:%1-P%2")
                    .arg(axisMap[axis])
                    .arg(qAbs(qRound(val * CONVERSION_R)));

    }else
    {
        if (val >= 0)
            cmd = QString("M:%1+P%2")
                    .arg(axisMap[axis])
                    .arg(qRound(val * CONVERSION));
        else
            cmd = QString("M:%1-P%2")
                    .arg(axisMap[axis])
                    .arg(qAbs(qRound(val * CONVERSION)));
    }

    if (sendCommandDirectly(cmd) == errorString) return;
}

void SigmaStage::moveHomeCommand(EnumList::Axis axis)
{
    QString cmd = QString("A:%1+P0").arg(axisMap[axis]);
    if (sendCommandDirectly(cmd) == errorString) return;
}

void SigmaStage::stop()
{
    QString cmd = QString("L:W");
    if (sendCommandDirectly(cmd) == errorString) return;
}

QMap<EnumList::Axis, float> SigmaStage::getCurrentPosition()
{
    QString cmd = "Q:";
    QMap<EnumList::Axis, float> a;

    if (sendCommandDirectly(cmd) == errorString) return a;

    return res->sigmaCurrPosMap;
}

void SigmaStage::performCommand()
{
    QString cmd = QString("G:");
    if (sendCommandDirectly(cmd) == errorString) return;
}

void SigmaStage::readAxis(const QJsonObject &json)
{
    read(json);
    QList<int> axisList;
    axisList.clear();
    axisList.append(json[axis1Key].toInt());
    axisList.append(json[axis2Key].toInt());
    axisList.append(json[axis3Key].toInt());
    axisList.append(json[axis4Key].toInt());

    // "contians(1)" 1 is combobox index
    // "indexOf(1) + 1 " is adjust command number
    // ex axis1 move home
    // "A:1+P0" QString(A:%1+P0).arg(axisMap(EnumList::x))
    if (axisList.contains(1))
        axisMap.insert(EnumList::x, axisList.indexOf(1)+1);
    if (axisList.contains(2))
        axisMap.insert(EnumList::y, axisList.indexOf(2)+1);
    if (axisList.contains(3))
        axisMap.insert(EnumList::z, axisList.indexOf(3)+1);
    if (axisList.contains(4))
        axisMap.insert(EnumList::theta, axisList.indexOf(4)+1);

}
