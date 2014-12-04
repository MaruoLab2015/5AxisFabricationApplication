#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QTextBrowser>

#include "printpanel/stagecontroller.h"

class ConvertPanel;
class EditorPanel;
class PrintPanel;
class StageSettingDialog;
class QSerialPort;
class QLabel;
class LogBrowser;

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

    void outputMessage( QtMsgType type, const QString &msg);

public slots:
    void applySettings();

private slots:
    void on_actionStageSetting_triggered();
    void on_actionGCode_triggered();
    void on_actionCanOpenStage_triggered();
    void on_actionStop_triggered();

    void on_actionOpenGCode_triggered();

    void on_actionSave_GCode_triggered();

private:
    Ui::MainWindow *ui;

    int transactionCount;

    QLabel *sigmaLabel,
    *technoPhiLabel, *technoZLabel,*technoThetaLabel,
    *shutterLabel;
    QLabel *sigmaStatusLabel,
    *technoStatusPhiLabel, *technoStatusZLabel,*technoStatusThetaLabel,
    *shutterStatusLabel;
    StageSettingDialog *settingDialog;
    ConvertPanel *convertTab;
    EditorPanel *editorTab;
    PrintPanel *printTab;
    QMap<int, QString> canOpenStageList;

    void setMenu();
};

#endif // MAINWINDOW_H
