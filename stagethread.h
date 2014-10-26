#ifndef STAGETHREAD_H
#define STAGETHREAD_H

#include <QThread>

class stageThread : public QThread
{
    Q_OBJECT
public:
    explicit stageThread(QObject *parent = 0);

signals:

public slots:

};

#endif // STAGETHREAD_H
