#include "stagecontroller.h"
#include "enumList.h"
#include "model/responseanalyzer.h"
#include "sigmastage.h"
#include "technostage.h"
#include "stage.h"
#include "shutter.h"

#include <QJsonObject>
#include <QDebug>

StageController::StageController(QObject *parent) :
    QObject(parent)
{
}

StageController::~StageController()
{
    delete phiStage;
    delete zSupplyStage;
    delete thetaSupplyStage;
    delete shutter;
}

void StageController::loadStageSettings(const QJsonObject &json)
{
    QJsonObject sigmaJson,phiJson, zsJson, thetasJson, shutterJson;
    sigmaJson   = json[sigmaKey].toObject();
    phiJson     = json[technoKey].toObject()[phiKey].toObject();
    zsJson      = json[technoKey].toObject()[zSuppliedKey].toObject();
    thetasJson  = json[technoKey].toObject()[thetaSuppliedKey].toObject();
    shutterJson = json[shutterKey].toObject();

    if ( sigmaJson[axis1Key].toInt() != 0 |
         sigmaJson[axis2Key].toInt() != 0 |
         sigmaJson[axis3Key].toInt() != 0 |
         sigmaJson[axis4Key].toInt() != 0
        )
    {
        sigmaStage = new SigmaStage();
        sigmaStage->readAxis(sigmaJson);
//        sigmaStage->read(sigmaJson);
    }else
        sigmaStage = NULL;

    if (!(Qt::CheckState)phiJson[disableKey].toInt())
    {
        phiStage = new TechnoStage();
        phiStage->read(phiJson);
    }else
        phiStage = NULL;

    if (!(Qt::CheckState)zsJson[disableKey].toInt())
    {
        zSupplyStage = new TechnoStage();
        zSupplyStage->read(zsJson);
    }else
        zSupplyStage = NULL;

    if (!(Qt::CheckState)thetasJson[disableKey].toInt())
    {
        thetaSupplyStage = new TechnoStage();
        thetaSupplyStage->read(thetasJson);
    }else
        thetaSupplyStage = NULL;

    if (!(Qt::CheckState)shutterJson[disableKey].toInt())
    {
        shutter = new Shutter();
        shutter->read(shutterJson);
    }else
        shutter = NULL;
}

QMap<int, QString> StageController::canOpenStages()
{
    QMap<int, QString> map;
    if (sigmaStage)
    {
        if (sigmaStage->openSerialPort())
            map.insert(EnumList::sigma, sigmaStage->portName);
        else
            map.insert(EnumList::sigma, QString("Not connecting"));
    }else
        map.insert(EnumList::sigma, QString("Not connecting"));

    if (zSupplyStage)
    {
        if (zSupplyStage->openSerialPort())
            map.insert(EnumList::zSupply, zSupplyStage->portName);
        else
            map.insert(EnumList::zSupply, QString("Not connecting"));
    }else
        map.insert(EnumList::zSupply, QString("Not connecting"));

    if (thetaSupplyStage)
    {
        if (thetaSupplyStage->openSerialPort())
            map.insert(EnumList::thetaSupply, thetaSupplyStage->portName);
        else
            map.insert(EnumList::thetaSupply, QString("Not connecting"));
    }else
        map.insert(EnumList::thetaSupply, QString("Not connecting"));

    if (phiStage)
    {
        if (phiStage->openSerialPort())
            map.insert(EnumList::phi, phiStage->portName);
        else
            map.insert(EnumList::phi, QString("Not connecting"));
    }else
        map.insert(EnumList::phi, QString("Not connecting"));

    if (shutter)
    {
        if (shutter->openSerialPort())
            map.insert(EnumList::shutter, shutter->portName);
        else
            map.insert(EnumList::shutter, QString("Not connecting"));
    }else
        map.insert(EnumList::shutter, QString("Not connecting"));

    return map;
}

void StageController::getStagePositions(EnumList::Axis axis)
{
    if ( axis == EnumList::phi)
    {
        phi = phiStage->getCurrentPosition();
    }
    else
    {
        sigmaPositionMap = sigmaStage->getCurrentPosition();
        x = sigmaPositionMap[EnumList::x];
        y = sigmaPositionMap[EnumList::y];
        z = sigmaPositionMap[EnumList::z];
        theta = sigmaPositionMap[EnumList::theta];
    }
}

void StageController::moveHome(EnumList::Axis axis)
{
    switch (axis) {
    case EnumList::x:
        sigmaStage->moveHomeCommand(axis);
        sigmaStage->performCommand();
        break;
    case EnumList::y:
        sigmaStage->moveHomeCommand(axis);
        sigmaStage->performCommand();
        break;
    case EnumList::z:
        sigmaStage->moveHomeCommand(axis);
        sigmaStage->performCommand();
        break;
    case EnumList::theta:
        sigmaStage->moveHomeCommand(axis);
        sigmaStage->performCommand();
        break;
    case EnumList::phi:
        phiStage->moveHome();
        break;
    default:
        break;
    }
}

void StageController::move(EnumList::Axis axis, float value, bool isAbsolute)
{
    if (axis == EnumList::phi)
    {
        if (isAbsolute)
            phiStage->moveAbsolute(value);
        else
            phiStage->moveRelative(value);
    }else
    {
        if (isAbsolute)
        {
            qDebug() << "move Absolute";
            sigmaStage->moveAbsoluteCommand(value, axis);
        }
        else
        {
            qDebug() << "move Relative";
            sigmaStage->moveRelativeCommand(value, axis);
        }
        sigmaStage->performCommand();
    }
}

void StageController::pressTheShutter(bool isOpen)
{
    if (isOpen) shutter->open();
    else shutter->close();
}

void StageController::supplyAction()
{
    qDebug() << "Supply action coming soon...";
}

void StageController::stopStages()
{
    if (sigmaStage)
        sigmaStage->stop();
}

/* SLOTS */

void StageController::receiveRequest(const QString s, EnumList::Axis axis)
{
    QString request = s;
    switch (axis) {
    case EnumList::x:
        sigmaStage->sendCommandDirectly(request);
        break;
    case EnumList::y:
        sigmaStage->sendCommandDirectly(request);
        break;
    case EnumList::z:
        sigmaStage->sendCommandDirectly(request);
        break;
    case EnumList::theta:
        sigmaStage->sendCommandDirectly(request);
        break;
    case EnumList::phi:
        phiStage->sendCommandDirectly(request);
        break;
    case EnumList::zSupply:
        zSupplyStage->sendCommandDirectly(request);
        break;
    case EnumList::thetaSupply:
        thetaSupplyStage->sendCommandDirectly(request);
        break;
    default:
        break;
    }

}

void StageController::receiveDebugMessage(QString s)
{
    qDebug() << s;
}
