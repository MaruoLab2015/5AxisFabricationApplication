#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "masterthread.h"
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
    void showDebugLog(const QString &s, bool isError);
    void applySettings();

private slots:
    void on_actionStageSetting_triggered();
    void on_actionOpenStage_triggered(bool checked);

private:
    Ui::MainWindow *ui;

    int transactionCount;
    MasterThread masterThread;
    QLabel *statusLabel;
    StageSettingDialog *settingDialog;
    PrintPanel *printTab;

    void setMenu();
};

#endif // MAINWINDOW_H
