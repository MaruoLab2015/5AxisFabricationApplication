#ifndef STAGE_H
#define STAGE_H

#include <QSerialPort>
#include <QObject>

#include "enumList.h"

class QString;
class QJsonObject;
class ResponseAnalyzer;

class Stage : public QObject
{
    Q_OBJECT

public:
    Stage();
    Stage(EnumList::Axis stageAxis);

    // setting
    void read(const QJsonObject &json);
    bool openSerialPort();
    void closeSerialPort();

    // property
    QSerialPort *serial;
    QString portName;
    int waitTime;
    int baudrate;
    QSerialPort::Parity parity;
    QSerialPort::StopBits stopbits;

    EnumList::Axis axis;
    QString axisString;

    ResponseAnalyzer *res;

protected:
    bool couldOpenSerialPort;
    const QString errorString;
};

#endif // STAGE_H
