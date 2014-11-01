#include "stagecontroller.h"
#include "enumList.h"
#include "responseanalyzer.h"
#include "stage.h"

#include <QJsonObject>
#include <QDebug>

StageController::StageController(QObject *parent) :
    QObject(parent)
{
//    xStage = new StageThread(this, EnumList::x);

    xStage = new Stage();
}

StageController::~StageController()
{
    delete xStage;
}

void StageController::loadStageSettings(const QJsonObject &json)
{
//    xStage->read(json["xaxis"].toObject());
    xStage->read(json["xaxis"].toObject());
}

QMap<int, QString> StageController::canOpenStages()
{

    if (xStage->openSerialPort())
        qDebug() << "success open xstage";
    else
        qDebug() << "fail to open";

    QMap<int, QString> map;
//    if (xStage->canOpenPort())
//        map.insert(EnumList::x, openPortName.append(xStage->portName));
//    else
//        map.insert(EnumList::x, canNotOpenString.append(xStage->portName));

    return map;
}

void StageController::getStagePositions()
{
    xStage->getCurrentPosition();
}

/* SLOTS */

void StageController::receiveLineEditText(const QString s)
{


    qDebug() << "receive line edit text : " << s;

    QString request = s;
    xStage->sendCommandDirectly(request);
}

void StageController::receiveRequest(const QString s, EnumList::Axis axis)
{

    qDebug() << "receive request : " << s << ", Axis : " << axis;
}
