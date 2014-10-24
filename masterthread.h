#ifndef MASTERTHREAD_H
#define MASTERTHREAD_H

#include <QThread>
#include <QWaitCondition>

class MasterThread : public QThread
{
    Q_OBJECT
public:
    explicit MasterThread(QObject *parent = 0);
    ~MasterThread();
    void transaction(const QString &portName, int waitTimeout, const QString &request);
    void run();

signals:
    void response(const QString &s);
    void error(const QString &s);
    void timeout(const QString &s);

public slots:

private:
    QString portName;
    QString request;
    int waitTimeout;
    QMutex mutex;
    QWaitCondition cond;
    bool quit;

};

#endif // MASTERTHREAD_H
