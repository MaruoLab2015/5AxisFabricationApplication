#include "stagecontroller.h"
#include "enumList.h"
#include "responseanalyzer.h"
#include "stage.h"

#include <QJsonObject>
#include <QDebug>

StageController::StageController(QObject *parent) :
    QObject(parent)
{
    xStage = new Stage(EnumList::x);
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

void StageController::getStagePositions(EnumList::Axis axis)
{
    switch (axis) {
    case EnumList::x:

        xStage->getCurrentPosition();
        break;
    default:
        break;
    }
}

void StageController::moveHome(EnumList::Axis axis)
{
    switch (axis) {
    case EnumList::x:

        xStage->moveHome();
        break;
    default:
        break;
    }

}

void StageController::move(EnumList::Axis axis, float value, bool isAbsolute)
{
    switch (axis) {
    case EnumList::x:
        if (isAbsolute)
            xStage->moveAbsolute(value);
        else
            xStage->moveRelative(value);
        break;
    default:
        break;
    }
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
