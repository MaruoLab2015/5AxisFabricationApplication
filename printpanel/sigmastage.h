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
    void moveRelativeCommand(float val, EnumList::Axis);
    void moveHomeCommand(EnumList::Axis);
    void performCommand();
    void stop();
    QMap<EnumList::Axis, float> getCurrentPosition();

    void readAxis(const QJsonObject &json);

    QMap<int, int> axisMap;

signals:

public slots:

};

#endif // SIGMASTAGE_H
