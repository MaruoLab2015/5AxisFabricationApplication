#include "stagecontroller.h"
#include "enumList.h"
#include "model/responseanalyzer.h"
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
    QJsonObject phiJson, zsJson, thetasJson, shutterJson;
    phiJson     = json[technoKey].toObject()[phiKey].toObject();
    zsJson      = json[technoKey].toObject()[zSuppliedKey].toObject();
    thetasJson  = json[technoKey].toObject()[thetaSuppliedKey].toObject();
    shutterJson = json[shutterKey].toObject();

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
    switch (axis) {
    case EnumList::x:
        break;
    case EnumList::y:
        break;
    case EnumList::phi:
        phi = phiStage->getCurrentPosition();
        break;
    default:
        break;
    }
}

void StageController::moveHome(EnumList::Axis axis)
{
    switch (axis) {
    case EnumList::x:
        break;
    case EnumList::y:
        break;
    case EnumList::phi:
        phiStage->moveHome();
        break;
    default:
        break;
    }
}

//void StageController::move(EnumList::Axis axis, float value, bool isAbsolute)
//{
//    switch (axis) {
//    case EnumList::x:
//        if (isAbsolute)
//            xStage->moveAbsolute(value);
//        else
//            xStage->moveRelative(value);
//        break;
//    case EnumList::y:
//        if (isAbsolute)
//            yStage->moveAbsolute(value);
//        else
//            yStage->moveRelative(value);
//        break;
//    default:
//        break;
//    }
//}

void StageController::pressTheShutter(bool isOpen)
{
    if (isOpen) shutter->open();
    else shutter->close();
}

void StageController::supplyAction()
{
    qDebug() << "Supply action coming soon...";
}

/* SLOTS */

void StageController::receiveRequest(const QString s, EnumList::Axis axis)
{
    QString request = s;
    switch (axis) {
    case EnumList::x:
        break;
    case EnumList::y:
        break;
    case EnumList::z:
        break;
    case EnumList::theta:
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
