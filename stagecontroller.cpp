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
    connect(xStage, SIGNAL(sendDebugMessage(QString)), this, SLOT(receiveDebugMessage(QString)));
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
    QMap<int, QString> map;
    if (xStage->openSerialPort())
        map.insert(EnumList::x, xStage->portName);
    else
        map.insert(EnumList::x, QString("Not connecting"));

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

//    qDebug() << "receive line edit text : " << s;

    QString request = s;
    xStage->sendCommandDirectly(request);
}

void StageController::receiveRequest(const QString s, EnumList::Axis axis)
{

//    qDebug() << "receive request : " << s << ", Axis : " << axis;
}

void StageController::receiveDebugMessage(QString s)
{
    emit sendDebugMessage(s);
}
