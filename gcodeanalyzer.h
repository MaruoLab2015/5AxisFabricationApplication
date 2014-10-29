#ifndef GCODEANALYZER_H
#define GCODEANALYZER_H

#include <QObject>

class GCodeAnalyzer : public QObject
{
    Q_OBJECT
public:
    explicit GCodeAnalyzer(QObject *parent = 0);

signals:

public slots:

};

#endif // GCODEANALYZER_H
