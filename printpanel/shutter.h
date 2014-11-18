#ifndef SHUTTER_H
#define SHUTTER_H

#include <QObject>
#include <QSerialPort>

#include "enumList.h"
#include "stage.h"

class Shutter : public Stage
{
    Q_OBJECT
public:
    explicit Shutter(QObject *parent = 0);

    void read(const QJsonObject &json);
    bool openSerialPort();

    void open();
    void close();
};

#endif // SHUTTER_H
