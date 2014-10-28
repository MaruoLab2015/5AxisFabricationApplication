#include "stagecontroller.h"
#include "enumList.h"

#include <QJsonObject>
#include <QDebug>

StageController::StageController(QObject *parent) :
    QObject(parent)
{
    xStage = new StageThread(this, EnumList::x);
    connect(xStage, SIGNAL(response(QString)), this, SLOT(showResponse(QString)));
    connect(xStage, SIGNAL(timeout(QString)), this, SLOT(processTimeout(QString)));
    connect(xStage, SIGNAL(error(QString)), this, SLOT(processError(QString)));
}

StageController::~StageController()
{
    delete xStage;
}

void StageController::loadStageSettings(const QJsonObject &json)
{
    xStage->read(json["xaxis"].toObject());
}

/* SLOTS */

void StageController::receiveLineEditText(const QString s)
{
    qDebug() << "receive line edit text : " << s;

    QString request = s;
    xStage->transaction(request);
}

void StageController::receiveRequest(const QString s, EnumList::Axis axis)
{
    qDebug() << "receive request : " << s << ", Axis : " << axis;

}

void StageController::showResponse(const QString &s)
{
    qDebug() << "response" << s;
    emit sendDebugMessage(s);

}

void StageController::processError(const QString &s)
{
    emit sendDebugMessage(s);

}

void StageController::processTimeout(const QString &s)
{
    emit sendDebugMessage(s);

}
