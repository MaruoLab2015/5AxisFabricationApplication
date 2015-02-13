#ifndef SIGMASTAGE_H
#define SIGMASTAGE_H

#include <QObject>
#include <QMap>

#include "stage.h"

class SigmaStage : public Stage
{
    Q_OBJECT
public:
    explicit SigmaStage(QObject *parent = 0);

    QString sendCommandDirectly(QString &cmd);
    void moveAbsoluteCommand(float val, EnumList::Axis);
    void moveAbsolute4Axis(float axis1Val, float axis2Val, float axis3Val, float axis4Val);
    void moveRelativeCommand(float val, EnumList::Axis);
    QString transformFloatToSigmaString(float v, bool isRotated);
    void sendSetScanSpeed();
    void moveHomeCommand(EnumList::Axis);
    void performCommand();
    void stop();
    void sendInitialSetting();
    QMap<EnumList::Axis, float> getCurrentPosition();

    void readAxis(const QJsonObject &json);

    QMap<int, int> axisMap;

    float currentTheta;
    bool isReady();

signals:

public slots:

};

#endif // SIGMASTAGE_H
