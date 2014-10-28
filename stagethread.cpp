#include "stagethread.h"

#include <QJsonObject>
#include <QDebug>
#include <QTime>

StageThread::StageThread(QObject *parent) :
    QThread(parent)
{
}

StageThread::StageThread(QObject *parent, EnumList::Axis stageAxis):
    QThread(parent)
{
    this->axis = stageAxis;
}


void StageThread::read(const QJsonObject &json)
{
    portName = json["portName"].toString();
    waitTime = json["waitTime"].toInt();
    baudrate = json["baudrate"].toInt();
    parity   = (QSerialPort::Parity)json["parity"].toInt();
    stopbits = (QSerialPort::StopBits)json["stopbits"].toInt();
    company  = (EnumList::Company)json["company"].toInt();
}

void StageThread::setAxis(const EnumList::Axis selectedAxis)
{
    axis = selectedAxis;
}

void StageThread::openSerialCommunication()
{

    qDebug() << "axis : " << this->axis;
//    emit response(tr("Attempting to connect to %1").arg(portName));

//    serial.setPortName(portName);
//    serial.setBaudRate(this->baudrate);
//    serial.setStopBits(this->stopbits);
//    serial.setParity(this->parity);
//    serial.setDataBits(serial.Data8);

//    if (!serial.open(QIODevice::ReadWrite)){
//        emit error(tr("Can't open %1, error %2").arg(portName).arg(serial.errorString()));
//        return;
//    }else
//    {
//        emit response(tr("Connection opend"));
//    }

}

void StageThread::transaction(QString &request)
{

    QMutexLocker locker(&mutex);

    this->request = applyFormat(request);

    if (!isRunning())
    {
        start();
    }else
    {
        cond.wakeOne();
    }
}

QString StageThread::applyFormat(QString &request)
{
    QString stx, etx; // stx = Start of TeXt, etx = End of TeXt
    QString sendRequest;

    switch (this->company) {
    case EnumList::TechnoHands:
        stx = ">";
        etx = "\r";
        break;
    default:
        break;
    }

    sendRequest.append(stx);
    sendRequest.append(request);
    sendRequest.append(etx);

    return sendRequest;
}


void StageThread::run()
{

    mutex.lock();

    int currentWaitTimeout = waitTime;
    QString currentRequest = request;
    mutex.unlock();
    QSerialPort serial;

    while(!quit)
    {

        serial.setPortName(portName);
        serial.setBaudRate(this->baudrate);
        serial.setStopBits(this->stopbits);
        serial.setParity(this->parity);
        serial.setDataBits(serial.Data8);

        if (!serial.open(QIODevice::ReadWrite)){
            emit error(tr("Can't open %1, error %2").arg(portName).arg(serial.errorString()));
            return;
        }

        //write request
        QByteArray requestData = currentRequest.toLocal8Bit();

        emit response(tr("request : %1").arg(this->request));

        serial.write(requestData);
        if (serial.waitForBytesWritten(waitTime))
        {
            //read response
            if (serial.waitForReadyRead(currentWaitTimeout))
            {
                QByteArray responseData = serial.readAll();
                while (serial.waitForReadyRead(100))
                {
                    responseData += serial.readAll();
                }

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
        currentWaitTimeout = waitTime;
        currentRequest = request;
        mutex.unlock();
    }
}
