#ifndef STAGECONTROLLER_H
#define STAGECONTROLLER_H

#include <QObject>

#include "enumList.h"
#include "stagethread.h"

class StageController : public QObject
{
    Q_OBJECT
public:
    explicit StageController(QObject *parent = 0);
    ~StageController();

    void loadStageSettings(const QJsonObject &json);
    QMap<int, QString> canOpenStages();

signals:
    void sendDebugMessage(const QString &s);

public slots:
    void receiveLineEditText(const QString s);
    void receiveRequest(const QString s, EnumList::Axis axis);

    void showResponse(const QString &s);
    void processError(const QString &s);
    void processTimeout(const QString &s);

private:
    StageThread *xStage;
};

#endif // STAGECONTROLLER_H
