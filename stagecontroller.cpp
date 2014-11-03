#include "stagecontroller.h"
#include "enumList.h"
#include "responseanalyzer.h"
#include "stage.h"
#include "shutter.h"

#include <QJsonObject>
#include <QDebug>

StageController::StageController(QObject *parent) :
    QObject(parent)
{
    xStage = new Stage(EnumList::x);
    connect(xStage, SIGNAL(sendDebugMessage(QString)), this, SLOT(receiveDebugMessage(QString)));

    yStage = new Stage(EnumList::y);
    connect(yStage, SIGNAL(sendDebugMessage(QString)), this, SLOT(receiveDebugMessage(QString)));

    shutter = new Shutter(this);
}

StageController::~StageController()
{
    delete xStage;
    delete yStage;
    delete shutter;
}

void StageController::loadStageSettings(const QJsonObject &json)
{
    xStage->read(json["xaxis"].toObject());
    yStage->read(json["yaxis"].toObject());
    shutter->read(json["shutter"].toObject());
}

QMap<int, QString> StageController::canOpenStages()
{
    QMap<int, QString> map;
    if (xStage->openSerialPort())
        map.insert(EnumList::x, xStage->portName);
    else
        map.insert(EnumList::x, QString("Not connecting"));

    if (yStage->openSerialPort())
        map.insert(EnumList::y, yStage->portName);
    else
        map.insert(EnumList::y, QString("Not connecting"));
    if (shutter->openSerialPort())
        map.insert(EnumList::shutter, shutter->portName);
    else
        map.insert(EnumList::shutter, QString("Not connecting"));

    return map;
}

void StageController::getStagePositions(EnumList::Axis axis)
{
    switch (axis) {
    case EnumList::x:

        x = xStage->getCurrentPosition();
        break;
    case EnumList::y:
        y = yStage->getCurrentPosition();
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
    case EnumList::y:
        yStage->moveHome();
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
    case EnumList::y:
        if (isAbsolute)
            yStage->moveAbsolute(value);
        else
            yStage->moveRelative(value);
        break;

    default:
        break;
    }
}

void StageController::pressTheShutter(bool isOpen)
{
    if (isOpen) shutter->open();
    else shutter->close();
}

/* SLOTS */

void StageController::receiveLineEditText(const QString s)
{
    QString request = s;
    xStage->sendCommandDirectly(request);
}

void StageController::receiveRequest(const QString s, EnumList::Axis axis)
{
    QString request = s;
    switch (axis) {
    case EnumList::x:
        xStage->sendCommandDirectly(request);
        break;
    case EnumList::y:
        yStage->sendCommandDirectly(request);
        break;
    case EnumList::z:

        break;
    case EnumList::theta:

        break;
    case EnumList::phi:

        break;
    default:
        break;
    }

}

void StageController::receiveDebugMessage(QString s)
{
    emit sendDebugMessage(s);
}
