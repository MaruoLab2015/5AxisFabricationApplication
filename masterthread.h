#ifndef MASTERTHREAD_H
#define MASTERTHREAD_H

#include <QThread>
#include <QWaitCondition>
#include <QSerialPort>

class MasterThread : public QThread
{
    Q_OBJECT
public:
    explicit MasterThread(QObject *parent = 0);
    ~MasterThread();

    enum Axis
    {
        XAxis,
        YAxis,
        ZAxis
    };

    MasterThread::Axis axis;
    void setAxis(const MasterThread::Axis &axis);

    void transaction(QString &request);
    void setSerialSettings(const QString &portName,
                           int waitTimeout,
                           int baudrate,
                           QSerialPort::Parity parity,
                           QSerialPort::StopBits stopbits);
    void run();

    void read(const QJsonObject &json);
    void write(QJsonObject &json) const;

    QString portName;

signals:
    void response(const QString &s);
    void error(const QString &s);
    void timeout(const QString &s);

public slots:

private:

    int waitTime;
    int baudrate;
    QSerialPort::Parity parity;
    QSerialPort::StopBits stopbits;
    QString request;
    QMutex mutex;
    QWaitCondition cond;
    bool quit;

};

#endif // MASTERTHREAD_H
