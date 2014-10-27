#include "masterthread.h"

#include <QSerialPort>

#include <QTime>
#include <QDebug>
#include <QJsonObject>

MasterThread::MasterThread(QObject *parent) :
    QThread(parent)
{

    xStage = new StageThread(this);
    connect(xStage, SIGNAL(response(QString)), this, SLOT(showResponse(QString)));
    connect(xStage, SIGNAL(error(QString)), this, SLOT(processError(QString)));
    connect(xStage, SIGNAL(timeout(QString)), this, SLOT(processTimeout(QString)));

}

MasterThread::~MasterThread()
{
    mutex.lock();
    quit = true;
    cond.wakeOne();
    mutex.unlock();
    wait();
}

void MasterThread::openStages()
{
    qDebug() << "open";
    xStage->openSerialCommunication();
}

void MasterThread::closeStages()
{
    qDebug() << "close";
    xStage->closeSerialCommunication();
}


void MasterThread::read(const QJsonObject &json)
{
    xStage->read(json["xaxis"].toObject());
}

void MasterThread::transaction(QString &request)
{
    QMutexLocker locker(&mutex);

    if (!isRunning())
    {
        start();
    }else
    {
        cond.wakeOne();
    }
}

void MasterThread::run()
{

}

/* SLOTS */
void MasterThread::receiveRequestText(QString request)
{
    qDebug() << "transaction";
    xStage->transaction(request);
    transaction(request);
}

void MasterThread::showResponse(const QString &s)
{

    sendDebugMessage(s, false);
//    ui->debugTextBrowser->append(tr("Traffic, transaction #%1:"
//                             "\n\r-request: %2"
//                             "\n\r-response: %2")
//                          .arg(++transactionCount).arg(s));
}

void MasterThread::processError(const QString &s)
{

    emit sendDebugMessage(s, true);
}

void MasterThread::processTimeout(const QString &s)
{

    emit sendDebugMessage(s, true);
}
