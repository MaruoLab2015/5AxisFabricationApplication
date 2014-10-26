#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "masterthread.h"
#include "stagesettingdialog.h"

class QSerialPort;
class QLabel;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    enum SaveFormat{
        Json, Binary
    };

    void defaultSettings();
    bool loadStageSettings(SaveFormat saveFormat);
    void read(const QJsonObject &json);

public slots:
    void showResponse(const QString &s);
    void processError(const QString &s);
    void processTimeout(const QString &s);

private slots:
    void on_actionStageSetting_triggered();
    void on_actionOpenStage_triggered(bool checked);

//    void applySettings(QString portName,
//                       int baudrate,
//                       QSerialPort::StopBits stopbits,
//                       QSerialPort::Parity parity,
//                       int waitTime);

private:
    Ui::MainWindow *ui;

    int transactionCount;
    MasterThread xAxisThread;
    QLabel *statusLabel;
    StageSettingDialog *settingDialog;

    void setMenu();
};

#endif // MAINWINDOW_H
