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

    bool hasCode(){return (
                hasX() || hasY() || hasZ() ||
                hasE() || hasF() ||
                hasI() || hasJ() || hasK() ||
                hasV() || hasS() ||
                hasT() || hasP()
                    )
                ;};

    bool hasText();

    bool hasX(){ return  (x != FLT_MAX);};
    bool hasY(){ return  (y != FLT_MAX);};
    bool hasZ(){ return  (z != FLT_MAX);};
    bool hasE(){ return  (e != FLT_MAX);};
    bool hasF(){ return  (f != FLT_MAX);};
    bool hasI(){ return  (ii != FLT_MAX);};
    bool hasJ(){ return  (jj != FLT_MAX);};
    bool hasK(){ return  (kk != FLT_MAX);};
    bool hasV(){ return  (v != FLT_MAX);};
    bool hasS(){ return  (s != FLT_MAX);};
    bool hasT(){ return  (t != FLT_MAX);};
    bool hasP(){ return  (p != FLT_MAX);};

    bool isComment;

    float x, y, z, e, f, r;
    float ii, jj, kk, v, s;
    float t, p;
    QString origText;

signals:

public slots:

private:
    QString orig;

};

#endif // GCODE_H
