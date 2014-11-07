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
    void closeSerialPort();

    // property
    QSerialPort *serial;
    QString portName;
    int waitTime;
    int baudrate;
    QSerialPort::Parity parity;
    QSerialPort::StopBits stopbits;

    EnumList::Company company;
    EnumList::Axis axis;
    QString axisString;

signals:
    void sendDebugMessage(QString s);

private:
    bool couldOpenSerialPort;
    const QString errorString;
    ResponseAnalyzer *res;

};

#endif // STAGE_H
