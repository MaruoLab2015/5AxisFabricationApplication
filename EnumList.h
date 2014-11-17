#ifndef ENUMLIST_H
#define ENUMLIST_H

#include <QObject>

const QString technoKey = "Techno";
const QString sigmaKey = "sigma";
const QString shutterKey = "shutter";
const QString portNameKey = "portName";
const QString baudrateKey = "baudrate";
const QString baudrateIndexKey = "baudrateIndex";
const QString stopbitsKey = "stopbits";
const QString stopbitsIndexKey = "stopbitsIndex";
const QString parityKey = "parity";
const QString parityIndexKey = "parityIndex";
const QString axis1Key = "axis1";
const QString axis2Key = "axis2";
const QString axis3Key = "axis3";
const QString axis4Key = "axis4";
const QString thetaSuppliedKey = "thetaSupply";
const QString zSuppliedKey = "zSupply";
const QString phiKey = "phiaxis";
const QString disableKey = "disable";


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

//    const QString sigmaKey = "sigma";
//    const QString shutterKey = "shutter";


    enum Axis{
        x,
        y,
        z,
        theta,
        phi,
        zSupply,
        thetaSupply,
        shutter
    };

    enum StageShutter{
        on = 1,
        off = 0
    };


};

#endif // ENUMLIST_H
