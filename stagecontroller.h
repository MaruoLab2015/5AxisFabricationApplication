#ifndef STAGECONTROLLER_H
#define STAGECONTROLLER_H

#include <QObject>
#include <QMap>

#include "enumList.h"
#include "stagethread.h"

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

//    void finishedThread();
//    void startedThread();

//    void showResponse(const QString &s);
//    void processError(const QString &s);
//    void processTimeout(const QString &s);

private:
//    StageThread *xStage;
    QMap<int, QString> stagePositionList;

};

#endif // STAGECONTROLLER_H
