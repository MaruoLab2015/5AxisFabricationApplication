#include "stagecontroller.h"
#include "enumList.h"
#include "model/responseanalyzer.h"
#include "sigmastage.h"
#include "technostage.h"
#include "stage.h"
#include "shutter.h"
#include "model/gcode.h"

#include <QJsonObject>
#include <QDebug>
#include <QEventLoop>
#include <QTimer>
#include <QtMath>
#include <QAudioOutput>
#include <QFile>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QFileDialog>
#include <QThread>

StageController::StageController(QObject *parent) :
    QObject(parent), isEmergencyStop(false)
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
        if (sigmaStage->openSerialPort()){
            map.insert(EnumList::sigma, sigmaStage->portName);
//            sigmaStage->sendInitialSetting();
        }
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
        {
            map.insert(EnumList::phi, phiStage->portName);
            phiStage->moveHome();
        }
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
    qDebug() << "stop the stages";
    isEmergencyStop = true;

    if (sigmaStage)
        sigmaStage->stop();

}

void StageController::startFabrication(QList<GCode *> &gcodeList)
{


    isEmergencyStop = false;

    if (gcodeList.count() == 0){

        qDebug() << "Can't read the fabrication file";
        return;
    }

    qDebug("start Fabrication");

    int percentage = 0;
    float cx, cy, cz, ct; // current position
    float cv; // current velocity
    cv = 10; // default scan speed is 10micro meter per second
    cx = cy = cz = ct = 0;

    if (sigmaStage)
        sigmaStage->sendSetScanSpeed();

    for (int i=0;i<gcodeList.count();i++)
    {
        GCode *gc = gcodeList.at(i);

        if (isEmergencyStop){ break; }


        // shutter
        if (shutter){
        if (gc->hasS()){
//            if (gcode->s == 0) qDebug() << "shutter open";
//            else               qDebug() << "shutter close";
//        }
            if (gc->s == 0) shutter->close();
            else               shutter->open();
        }
        }

        // move Stages (sigma)
//        if (gc->hasX()) cx = gc->x;
//        if (gc->hasY()) cy = gc->y;
//        if (gc->hasZ()) cz = gc->z;
//        if (gc->hasT()) ct = gc->t;

//        sigmaStage->moveAbsolute4Axis(cx, cy, -cz, ct);// zは動かす

        // move stage (Technohands)
//        if (gc->hasP())
//            qDebug() << "move phi:" << gc->p;
//            phiStage->moveAbsolute(gc->p);

//        QHash<QString, float> dic;
//        dic = currentFabricationLineNumber(i);
//        qDebug() << dic.value("phi");

        emit currentFabricationLineNumber(i);

        // 進捗状況
        int tmpP = qFloor((float)i / (float)gcodeList.count() * 100);
        if (percentage != tmpP)
        {
            percentage = tmpP;
            qDebug() << "now" << percentage << "%";
        }

        if (i == gcodeList.count() -1)
        {
            if (shutter) shutter->close();
            playFinishMusic();
        }

        QEventLoop loop;
        QTimer::singleShot(100, &loop, SLOT(quit()));
        loop.exec();

    }

}

void StageController::playFinishMusic()
{

    QMediaPlayer *player = new QMediaPlayer;
    player->setMedia(QUrl::fromLocalFile("/Users/genki/Desktop/RingingVillageShort.mp3"));
    player->setVolume(50);
    player->play();
}

/* SLOTS */

void StageController::receiveRequest(const QString s, EnumList::Axis axis)
{
    QString request = s;
    switch (axis) {
    case EnumList::x:
        sigmaStage->sendCommandDirectly(request);
        sigmaStage->performCommand();
        break;
    case EnumList::y:
        sigmaStage->sendCommandDirectly(request);
        sigmaStage->performCommand();
        break;
    case EnumList::z:
        sigmaStage->sendCommandDirectly(request);
        sigmaStage->performCommand();
        break;
    case EnumList::theta:
        sigmaStage->sendCommandDirectly(request);
        sigmaStage->performCommand();
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
//    qDebug() << s;
}

void StageController::receivedCurrentPosition(float x,float y,float z,float t,float p)
{

    if (sigmaStage)
    {
        sigmaStage->moveAbsolute4Axis(x, y, -z, p); //z down
         while (!sigmaStage->isReady()){
            if (isEmergencyStop) break;
         }
    }

    if (phiStage){
        phiStage->isDebug = false;
        phiStage->moveAbsolute(p);
    }
}
