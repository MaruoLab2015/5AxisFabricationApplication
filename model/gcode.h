#ifndef GCODE_H
#define GCODE_H

#include <QObject>

#include "cfloat"

class GCode : public QObject
{
    Q_OBJECT
public:
    explicit GCode(QObject *parent = 0);
    void parse(QString s);
    void addCode(QString c, QString val);

    bool hasCode();
    bool hasText();

    bool hasG(){ return  (g != USHRT_MAX);};
    bool hasX(){ return  (x != FLT_MAX);};
    bool hasY(){ return  (y != FLT_MAX);};
    bool hasZ(){ return  (z != FLT_MAX);};
    bool hasE(){ return  (e != FLT_MAX);};
    bool hasF(){ return  (f != FLT_MAX);};

    bool isComment;

    ushort g;
    float x, y, z, e, f;
    QString origText;

signals:

public slots:

private:
    QString orig;

};

#endif // GCODE_H
