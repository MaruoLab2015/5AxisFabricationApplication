#ifndef TECHNOSTAGE_H
#define TECHNOSTAGE_H

#include <QObject>

#include "stage.h"

class TechnoStage : public Stage
{
    Q_OBJECT
public:
    explicit TechnoStage(QObject *parent = 0);

    QString sendCommandDirectly(QString &cmd);
    void moveAbsolute(float val);
    void moveRelative(float val);
    void moveHome();
    void stop();
    float getCurrentPosition();

    bool isDebug;

private:
    static const float kPositiveConversion;
    static const float kNegativeConversion;
};

#endif // TECHNOSTAGE_H
