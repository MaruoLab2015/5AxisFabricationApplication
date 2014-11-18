#ifndef SIGMASTAGE_H
#define SIGMASTAGE_H

#include <QObject>
#include "stage.h"

class SigmaStage : public Stage
{
    Q_OBJECT
public:
    explicit SigmaStage(QObject *parent = 0);

    QString sendCommandDirectly(QString &cmd);
    void moveAbsolute(float val);
    void moveRelative(float val);
    void moveHome();
    void stop();
    float getCurrentPosition();

signals:

public slots:

};

#endif // SIGMASTAGE_H
