﻿#ifndef STAGECONTROLLER_H
#define STAGECONTROLLER_H

#include <QObject>
#include <QMap>
#include <QHash>
#include <QString>

#include "enumList.h"

class SigmaStage;
class TechnoStage;
class Stage;
class Shutter;
class GCode;

class StageController : public QObject
{
    Q_OBJECT

public:
    explicit StageController(QObject *parent = 0);
    ~StageController();

    QMap<int, QString> canOpenStages();

    EnumList::StageShutter e;
    float x, y, z;
    float theta, phi;
    float f;
    bool isEmergencyStop;

    SigmaStage *sigmaStage;
    TechnoStage *phiStage, *zSupplyStage, *thetaSupplyStage;
    Shutter *shutter;

    void loadStageSettings(const QJsonObject &json);
    void getStagePositions(EnumList::Axis axis);
    void moveHome(EnumList::Axis axis);
    void move(EnumList::Axis axis, float value,bool isAbsolute);
    void supplyAction();
    void stopStages();
    void startFabrication(QList<GCode*> &gcodeList);

    void pressTheShutter(bool isOpen);

    void receiveLineEditText(const QString s);
    void receiveRequest(const QString s, EnumList::Axis axis);

    void playFinishMusic();

public slots:
    void receiveDebugMessage(QString s);
    void receivedCurrentPosition(float x, float y, float z, float t, float p);

signals:
    void currentFabricationLineNumber(int lineNum);

private:
    QMap<EnumList::Axis, float> sigmaPositionMap;

};

#endif // STAGECONTROLLER_H
