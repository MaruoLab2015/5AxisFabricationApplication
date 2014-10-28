#ifndef ENUMLIST_H
#define ENUMLIST_H

#include <QObject>

class EnumList : public QObject
{

    Q_ENUMS( SaveFormat Company )

public:
    enum SaveFormat{
        Json, Binary
    };

    enum Company {
        TechnoHands = 0,
        SigmaStage = 1,
        Shutter = 2
    };

    enum Axis{
        x,
        y,
        z,
        theta,
        phi,
        shutter
    };


};

#endif // ENUMLIST_H
