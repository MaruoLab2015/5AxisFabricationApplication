#ifndef GCODE_H
#define GCODE_H

#include <QObject>

class GCode : public QObject
{
    Q_OBJECT
public:
    explicit GCode(QObject *parent = 0);

signals:

public slots:

};

#endif // GCODE_H
