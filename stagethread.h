#ifndef STAGETHREAD_H
#define STAGETHREAD_H

#include <QThread>
#include <QWaitCondition>
#include <QSerialPort>

#include "enumList.h"

class StageThread : public QThread
{
    Q_OBJECT
    Q_ENUMS( Axis )

public:
    explicit StageThread(QObject *parent = 0);
    StageThread(QObject *parent, EnumList::Axis stageAxis);
    void transaction(QString &request);
    void run();

    void read(const QJsonObject &json);

    void setAxis(const EnumList::Axis);

    void openSerialCommunication();

signals:
    void response(const QString &s);
    void error(const QString &s);
    void timeout(const QString &s);

public slots:

public:
    int waitTime;
    int baudrate;
    QSerialPort::Parity parity;
    QSerialPort::StopBits stopbits;
    QString request;
    QMutex mutex;
    QWaitCondition cond;
    bool quit;
    QString portName;
    EnumList::Company company;

private:
    EnumList::Axis axis;

    QString applyFormat(QString &request);
};

#endif // STAGETHREAD_H
