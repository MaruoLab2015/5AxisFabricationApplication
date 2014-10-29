#include "stagecontroller.h"
#include "enumList.h"

#include <QJsonObject>
#include <QDebug>

StageController::StageController(QObject *parent) :
    QObject(parent)
{
    xStage = new StageThread(this, EnumList::x);
    connect(xStage, SIGNAL(response(QString)), this, SLOT(showResponse(QString)));
    connect(xStage, SIGNAL(timeout(QString)), this, SLOT(processTimeout(QString)));
    connect(xStage, SIGNAL(error(QString)), this, SLOT(processError(QString)));

}

StageController::~StageController()
{
    delete xStage;
}

void StageController::loadStageSettings(const QJsonObject &json)
{
    xStage->read(json["xaxis"].toObject());
}

QMap<int, QString> StageController::canOpenStages()
{

    QString openPortName, canNotOpenString;
    openPortName = "Success Open  ";
    canNotOpenString = "Can't open ";

    QMap<int, QString> map;
    if (xStage->canOpenPort())
        map.insert(EnumList::x, openPortName.append(xStage->portName));
    else
        map.insert(EnumList::x, canNotOpenString.append(xStage->portName));

    return map;
}

void StageController::getStagePositions()
{
    stagePositionList.insert(EnumList::x, xStage->getCurrentPosition());
    qDebug() << stagePositionList[EnumList::x];
}

/* SLOTS */

void StageController::receiveLineEditText(const QString s)
{

    qDebug() << "receive line edit text : " << s;

    QString request = s;
    xStage->transaction(request);
}

void StageController::receiveRequest(const QString s, EnumList::Axis axis)
{

    qDebug() << "receive request : " << s << ", Axis : " << axis;
}

void StageController::showResponse(const QString &s)
{

    qDebug() << "response" << s;
    emit sendDebugMessage(s);
}

void StageController::processError(const QString &s)
{

    qDebug() << "response" << s;
    emit sendDebugMessage(s);
}

void StageController::processTimeout(const QString &s)
{

    qDebug() << "response" << s;
    emit sendDebugMessage(s);
}
