#include "masterthread.h"

#include <QSerialPort>

#include <QTime>
#include <QDebug>

MasterThread::MasterThread(QObject *parent) :
    QThread(parent)
{
}

MasterThread::~MasterThread()
{
    mutex.lock();
    quit = true;
    cond.wakeOne();
    mutex.unlock();
    wait();
}

void MasterThread::transaction(const QString &portName, int waitTimeout, const QString &request)
{
    QMutexLocker locker(&mutex);
    this->portName = portName;
    this->waitTimeout = waitTimeout;
    this->request = request;
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

//    qDebug() << "run";

    bool currentPortNameChanged = false;

    mutex.lock();
    QString currentPortName;
    if (currentPortName != portName)
    {
        currentPortName = portName;
        currentPortNameChanged = true;
    }

    int currentWaitTimeout = waitTimeout;
    QString currentRequest = request;
    mutex.unlock();

    QSerialPort serial;

    while(!quit)
    {

        if (currentPortNameChanged)
        {
            serial.close();
            serial.setPortName(currentPortName);
            serial.setBaudRate(serial.Baud115200);
            serial.setStopBits(serial.OneStop);
            serial.setParity(serial.NoParity);
            serial.setDataBits(serial.Data8);

            if (!serial.open(QIODevice::ReadWrite)){
                emit error(tr("Can't open %1, errorcode %2").arg(portName).arg(serial.error()));
                serial.close();//add
                return;
            }
        }


        //write request
        QByteArray requestData = currentRequest.toLocal8Bit();
        serial.write(requestData);
        if (serial.waitForBytesWritten(waitTimeout))
        {
            //read response
            if (serial.waitForReadyRead(currentWaitTimeout))
            {
                QByteArray responseData = serial.readAll();
                while (serial.waitForReadyRead(10))
                {
                    responseData += serial.readAll();
                }

                qDebug() << responseData;
                QString response(responseData);
                emit this->response(response);

            }else{

                emit timeout(tr("Wait read response timeout %1").arg(QTime::currentTime().toString()));
                serial.close();//add
            }
        }else{

            emit timeout(tr("Wait write request timeout 1").arg(QTime::currentTime().toString()));
            serial.close();//add
        }

        mutex.lock();
        cond.wait(&mutex);
        if (currentPortName != portName)
        {
            currentPortName = portName;
            currentPortNameChanged = true;
        }else{

            currentPortNameChanged = false;
        }
        currentWaitTimeout = waitTimeout;
        currentRequest = request;
        mutex.unlock();
    }
}
