#ifndef STAGECONTROLLER_H
#define STAGECONTROLLER_H

#include <QObject>
#include <QMap>

#include "enumList.h"

class Stage;
class Shutter;

class StageController : public QObject
{
    Q_OBJECT
public:
    explicit StageController(QObject *parent = 0);
    ~StageController();

    QMap<int, QString> canOpenStages();

    EnumList::StageShutter e;
    float x, y, z;
    float f;

    Stage *xStage, *yStage;
    Shutter *shutter;

    void loadStageSettings(const QJsonObject &json);
    void getStagePositions(EnumList::Axis axis);
    void moveHome(EnumList::Axis axis);
    void move(EnumList::Axis axis, float value,bool isAbsolute);

    void pressTheShutter(bool isOpen);

    void receiveLineEditText(const QString s);
    void receiveRequest(const QString s, EnumList::Axis axis);

public slots:
    void receiveDebugMessage(QString s);

signals:
    void sendDebugMessage(QString s);

private:
    QMap<int, QString> stagePositionList;

};

#endif // STAGECONTROLLER_H
