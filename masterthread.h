#ifndef MASTERTHREAD_H
#define MASTERTHREAD_H

#include <QThread>
#include <QWaitCondition>
#include <QSerialPort>

#include "stagethread.h"

class MasterThread : public QThread
{
    Q_OBJECT
public:
    explicit MasterThread(QObject *parent = 0);
    ~MasterThread();

    void transaction(QString &request);
    void run();

    void read(const QJsonObject &json);

    void openStages();
    void closeStages();

signals:
    void sendDebugMessage(const QString&s, bool isError);

public slots:
    void showResponse(const QString &s);
    void processError(const QString &s);
    void processTimeout(const QString &s);

public slots:
    void receiveRequestText(QString request);

private:

    QString request;
    QMutex mutex;
    QWaitCondition cond;
    bool quit;
    StageThread *xStage, *yStage, *zStage;
    StageThread *thetaStage, *phiStage;
    StageThread *shutterStage;

};

#endif // MASTERTHREAD_H
