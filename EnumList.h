#ifndef ENUMLIST_H
#define ENUMLIST_H

#include <QObject>

class EnumList : public QObject
{

    Q_ENUMS( SaveFormat Company StageShutter )

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

    enum StageShutter{
        on = 1,
        off = 0
    };


};

#endif // ENUMLIST_H
