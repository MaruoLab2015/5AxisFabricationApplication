#ifndef RESPONSEANALYZER_H
#define RESPONSEANALYZER_H

#include <QObject>
#include <QMap>

#include "float.h"
#include "EnumList.h"

class ResponseAnalyzer : public QObject
{
    Q_OBJECT
public:
    explicit ResponseAnalyzer(QObject *parent = 0);

    void parseTechnoHandsResponseText(QString);
    void parseSigmaResponseText(QString);

    bool hasPosition(){return currentPosition != DBL_MAX;};

    double currentPosition;
    float x,y,z,theta;
    QMap<EnumList::Axis, float> sigmaCurrPosMap;

signals:

public slots:

private:
    QString orig;
    void addParameter(QString c, QString val);
};

#endif // RESPONSEANALYZER_H
