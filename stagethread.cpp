#include "stagethread.h"

#include <QJsonObject>
#include <QDebug>
#include <QTime>

StageThread::StageThread(QObject *parent) :
    QThread(parent)
{
}

void StageThread::read(const QJsonObject &json)
{
    portName = json["portName"].toString();
    waitTime = json["waitTime"].toInt();
    baudrate = json["baudrate"].toInt();
    parity   = (QSerialPort::Parity)json["parity"].toInt();
    stopbits = (QSerialPort::StopBits)json["stopbits"].toInt();
}

void StageThread::openSerialCommunication()
{

    serial.setPortName(portName);
    serial.setBaudRate(this->baudrate);
    serial.setStopBits(this->stopbits);
    serial.setParity(this->parity);
    serial.setDataBits(serial.Data8);

    if (!serial.open(QIODevice::ReadWrite)){
        emit error(tr("Can't open %1, errorcode %2").arg(portName).arg(serial.error()));
        return;
    }
}

void StageThread::closeSerialCommunication()
{
    serial.close();
}

void StageThread::transaction(QString &request)
{
//    qDebug() << "stage transaction";

    QMutexLocker locker(&mutex);

    if (!isRunning())
    {
        start();
    }else
    {
        cond.wakeOne();
    }
}


void StageThread::run()
{

    bool currentPortNameChanged = false;

    mutex.lock();
    QString currentPortName;
    if (currentPortName != portName)
    {
        currentPortName = portName;
        currentPortNameChanged = true;
    }

    int currentWaitTimeout = waitTime;
    QString currentRequest = request;
    mutex.unlock();

    while(!quit)
    {

        //write request
        QByteArray requestData = currentRequest.toLocal8Bit();
        qDebug() << "write: " << request;
        serial.write(requestData);
        if (serial.waitForBytesWritten(waitTime))
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
            }
        }else{

            emit timeout(tr("Wait write request timeout %1").arg(QTime::currentTime().toString()));
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
        currentWaitTimeout = waitTime;
        currentRequest = request;
        mutex.unlock();
    }
}
