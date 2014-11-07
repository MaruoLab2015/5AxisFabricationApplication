#ifndef RESPONSEANALYZER_H
#define RESPONSEANALYZER_H

#include <QObject>

#include "float.h"

class ResponseAnalyzer : public QObject
{
    Q_OBJECT
public:
    explicit ResponseAnalyzer(QObject *parent = 0);

    void parseTechnoHandsResponseText(QString);

    bool hasPosition(){return currentPosition != DBL_MAX;};

    double currentPosition;


signals:

public slots:

private:
    QString orig;
    void addParameter(QString c, QString val);
};

#endif // RESPONSEANALYZER_H
