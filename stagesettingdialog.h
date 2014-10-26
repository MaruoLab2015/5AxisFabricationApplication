#ifndef STAGESETTINGDIALOG_H
#define STAGESETTINGDIALOG_H

#include <QDialog>
#include <QTableWidget>
#include <QDialogButtonBox>
#include <QSerialPort>

class StageWidget;
class QComboBox;

namespace Ui {
class StageSettingDialog;
}

class StageSettingDialog : public QDialog
{
    Q_OBJECT
    Q_ENUMS( Company )

public:
    explicit StageSettingDialog(QWidget *parent = 0);

    ~StageSettingDialog();

    enum Company {
        TechnoHands = 0,
        SigmaStage = 1,
        Shutter = 2
    };

    enum SaveFormat{
        Json, Binary
    };

    void read(const QJsonObject &json);
    void write(QJsonObject &json) const;

    bool saveStageSettingss(SaveFormat saveFormat) const;

Q_SIGNALS:
//    void applySetting(QString portName,
//                      int baudrate,
//                      QSerialPort::StopBits stopbits,
//                      QSerialPort::Parity parity,
//                      int waitTime);


private slots:
    void onButtonBoxClicked(QAbstractButton *button);

private:
    Ui::StageSettingDialog *ui;

    StageWidget *xAxisWidget;
    StageWidget *yAxisWidget;
    StageWidget *zAxisWidget;
    StageWidget *thetaAxisWidget;
    StageWidget *phiAxisWidget;
    StageWidget *shutterWidget;

    void initialTabs();
    StageWidget* createTabWithTitle(QString);
};

#endif // STAGESETTINGDIALOG_H
