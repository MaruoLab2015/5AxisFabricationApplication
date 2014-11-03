#ifndef SHUTTER_H
#define SHUTTER_H

#include <QObject>
#include <QSerialPort>

#include "enumList.h"

class Shutter : public QObject
{
    Q_OBJECT
public:
    explicit Shutter(QObject *parent = 0);

    void read(const QJsonObject &json);
    bool openSerialPort();

    void open();
    void close();

    QSerialPort *serial;
    QString portName;
    int waitTime;
    int baudrate;
    QSerialPort::Parity parity;
    QSerialPort::StopBits stopbits;

    EnumList::Company company;

signals:
    void sendDebugMessage(QString);

public slots:

private:
    bool couldOpenSerialPort;
};

#endif // SHUTTER_H
