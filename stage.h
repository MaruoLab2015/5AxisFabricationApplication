#ifndef STAGE_H
#define STAGE_H

#include <QSerialPort>

#include "enumList.h"

class QString;
class QJsonObject;
class ResponseAnalyzer;

class Stage
{

public:
    Stage();

    // control Stage command
    QString sendCommandDirectly(QString &cmd);
    void moveAbsolute(float val);
    void moveRelative(float val);
    void moveHome();
    void stop();
    float getCurrentPosition();

    // setting
    void read(const QJsonObject &json);
    bool openSerialPort();
    bool closeSerialPort();

    // property
    QSerialPort *serial;
    QString portName;
    int waitTime;
    int baudrate;
    QSerialPort::Parity parity;
    QSerialPort::StopBits stopbits;

    EnumList::Company company;
    EnumList::Axis axis;

private:
    bool couldOpenSerialPort;
    const QString errorString;
    ResponseAnalyzer *res;

};

#endif // STAGE_H
