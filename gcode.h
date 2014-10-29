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
    bool hasX();
    bool hasY();
    bool hasZ();
    bool hasE();
    bool hasF();

    bool isComment;

    ushort g;
    float x, y, z, e, f;

signals:

public slots:

private:
    QString *text;
    QString orig;

};

#endif // GCODE_H
