#include "sigmastage.h"
#include "model/responseanalyzer.h"

#include <QDebug>
#include <QJsonObject>
#include <QtMath>

SigmaStage::SigmaStage(QObject */*parent*/)
{
    couldOpenSerialPort = false;
    currentTheta = 0;
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
    qDebug() << val;
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

void SigmaStage::moveAbsolute4Axis(float axis1Val, float axis2Val, float axis3Val, float axis4Val)
{
    QString cmd, a1, a2, a3, a4;
    a1 = transformFloatToSigmaString(axis1Val, false);
    a2 = transformFloatToSigmaString(axis2Val, false);
    a3 = transformFloatToSigmaString(axis3Val, false);
    a4 = QString("+P0");
    if (axis4Val != 0){
        a4 = QString("+P%1").arg(qRound(currentTheta + 5 * CONVERSION_R));//transformFloatToSigmaString(axis4Val, true);
        currentTheta += 5 * CONVERSION_R;
    }

    cmd = QString("A:W%1%2%3%4").arg(a1).arg(a2).arg(a3).arg(a4);
    sendCommandDirectly(cmd);
    performCommand();
    qDebug() << cmd;
}

QString SigmaStage::transformFloatToSigmaString(float v, bool isRotated)
{
    QString str;

    if (v >= 0)
    {
        if (isRotated)
            str = QString("+P%1").arg(qRound(v * CONVERSION_R));
        else
            str = QString("+P%1").arg(qRound(v * CONVERSION));

        return str;
    }else
    {
        if (isRotated)
            str = QString("-P%1").arg(qRound(qAbs(v * CONVERSION_R)));
        else
            str = QString("-P%1").arg(qRound(qAbs(v * CONVERSION)));

        return str;
    }
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

void SigmaStage::sendSetScanSpeed()
{
    QString cmd, vx, vy, vz, vt;

    vx = QString("S%1F%2R1").arg(35 * CONVERSION -1).arg(35 * CONVERSION);
    vy = QString("S%1F%2R1").arg(35 * CONVERSION -1).arg(35 * CONVERSION);
    vz = QString("S%1F%2R1").arg(35 * CONVERSION -1).arg(35 * CONVERSION);
    vt = QString("S%1F%2R1").arg(10 * CONVERSION_R -1).arg(10 * CONVERSION_R);

    cmd = QString("D:WS4374F4375R1S4374F4375R1S4374F4375R1S39999F40000R1");
//    cmd = QString("D:W%1%2%3%4").arg(vx).arg(vy).arg(vz).arg(vt);
    sendCommandDirectly(cmd);
//    performCommand();
//    qDebug() << cmd;
}

void SigmaStage::sendInitialSetting()
{
    int xDivNum, yDivNum, zDivNum, thetaDivNum;
    xDivNum = 250;
    yDivNum = 250;
    zDivNum = 250;
    thetaDivNum = 20;
    QString cmdx, cmdy, cmdz, cmdt;
    cmdx = QString("S:1%1").arg(xDivNum);
    cmdy = QString("S:1%1").arg(yDivNum);
    cmdz = QString("S:1%1").arg(zDivNum);
    cmdt = QString("S:1%1").arg(thetaDivNum);

    sendCommandDirectly(cmdx);
    sendCommandDirectly(cmdy);
    sendCommandDirectly(cmdz);
    sendCommandDirectly(cmdt);
//    sendCommandDirectly(QString("S:2%1").arg(yDivNum));
//    sendCommandDirectly(QString("S:3%1").arg(zDivNum));
//    sendCommandDirectly(QString("S:4%1").arg(thetaDivNum));
}

bool SigmaStage::isReady()
{
    QString cmd, r;
    cmd = QString("!:");
    r = sendCommandDirectly(cmd);
    qDebug() << r.trimmed();
    if (r.trimmed() == "R"){
        qDebug() << "true";
        return true;
    }
    else {
        qDebug() << "false";
        return false;
    }
}
