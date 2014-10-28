#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "stagecontroller.h"
#include "stagesettingdialog.h"
#include "printpanel.h"

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
    void showDebugLog(const QString &s);
    void applySettings();

private slots:
    void on_actionStageSetting_triggered();
    void on_actionCanOpenStage_triggered();

private:
    Ui::MainWindow *ui;

    int transactionCount;

    StageController stageManeger;
    QLabel *xLabel, *yLabel;
    QLabel *xStatusLabel, *yStatusLabel;
    StageSettingDialog *settingDialog;
    PrintPanel *printTab;
    QMap<int, QString> canOpenStageList;

    void setMenu();
};

#endif // MAINWINDOW_H
