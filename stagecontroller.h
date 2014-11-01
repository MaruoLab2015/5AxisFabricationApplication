#ifndef STAGECONTROLLER_H
#define STAGECONTROLLER_H

#include <QObject>
#include <QMap>

#include "enumList.h"

class Stage;

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

    Stage *xStage;

    void loadStageSettings(const QJsonObject &json);
    void getStagePositions();

signals:
    void sendDebugMessage(const QString s);

public slots:
    void receiveLineEditText(const QString s);
    void receiveRequest(const QString s, EnumList::Axis axis);


private:
    QMap<int, QString> stagePositionList;

};

#endif // STAGECONTROLLER_H
