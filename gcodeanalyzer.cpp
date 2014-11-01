#include "gcodeanalyzer.h"
#include "qmath.h"

GCodeAnalyzer::GCodeAnalyzer(QObject *parent) :
    QObject(parent)
  ,e(EnumList::off)
  ,x(0),y(0),z(0)
  ,f(10)
  ,lastX(0), lastY(0), lastZ(0)
  ,xOffset(0), yOffset(0), zOffset(0), layerZ(0)
  ,hasXHome(false), hasYHome(false), hasZHome(false)
  ,layer(0), lastlayer(0)
  ,isG1Move(false)
  ,printingTime(0)
  ,relative(false)
{
}

void GCodeAnalyzer::start(bool fire)
{

    x = y = z = 0;
    xOffset = yOffset = zOffset = 0;
    lastX = lastY = lastZ = 0;
    hasXHome = hasYHome = hasZHome = false;
}

void GCodeAnalyzer::startJob()
{

    layer = 0;
    printingTime = 0;
    lastX = lastY = lastZ = 0;
    lastlayer = 0;
}

void GCodeAnalyzer::analyze(GCode code)
{

    if (code.hasG())
    {
        switch (code.g) {
        case 0:
            break;
        case 1://直線補間
            if (code.hasF()) f = code.f;
            if (code.hasE()) e = (EnumList::StageShutter)code.e;

            if (relative)
            {

                if (code.hasX()) x = code.x;
                if (code.hasY()) y = code.y;
                if (code.hasZ()) z = code.z;
            }
            else
            {
                if (code.hasX()) x = xOffset + code.x;
                if (code.hasY()) y = yOffset + code.y;
                if (code.hasZ()) z = zOffset + code.z;
            }
            float dx, dy, dz;
            dx = abs(x - lastX);
            dy = abs(y - lastY);
            dz = abs(z - lastZ);

            double time;
            time = 0;
            if (dx + dy + dz > 0.01)
                time = qSqrt( dx * dx + dy * dy + dz * dz ) / f;
            printingTime += time;

            lastX = x;
            lastY = y;
            lastZ = z;

            break;

        case 2://円弧補間(時計)
        case 3://円弧補間(反時計)
        case 90:// set absolute movement
            relative = false;
            break;
        case 91:// set incremental movement
            relative = true;
            break;
        case 92: //現在地をhome position
            if (code.hasX()) { xOffset = code.x; x=0;}
            if (code.hasY()) { yOffset = code.y; y=0;}
            if (code.hasZ()) { zOffset = code.z; z=0;}
            break;
        default:
            break;
        }
    }
}
