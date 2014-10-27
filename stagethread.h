#ifndef STAGETHREAD_H
#define STAGETHREAD_H

#include <QThread>
#include <QWaitCondition>
#include <QSerialPort>

class StageThread : public QThread
{
    Q_OBJECT
    Q_ENUMS( Axis )

public:
    explicit StageThread(QObject *parent = 0);

    enum Axis{
        x,
        y,
        z,
        theta,
        phi,
        shutter
    };

    void transaction(QString &request);
//    void setSerialSettings(const QString &portName,
//                           int waitTimeout,
//                           int baudrate,
//                           QSerialPort::Parity parity,
//                           QSerialPort::StopBits stopbits);
    void run();

    void read(const QJsonObject &json);

    void setAxis(const StageThread::Axis);

    void openSerialCommunication();
    void closeSerialCommunication();

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
    QSerialPort serial;
};

#endif // STAGETHREAD_H
