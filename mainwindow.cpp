#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "settingDialog/stagesettingdialog.h"
#include "convert/convertpanel.h"
#include "editor/editorpanel.h"
#include "printpanel/printpanel.h"

#include <QDebug>
#include <QLabel>
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>
#include <QTime>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
  ,xLabel(new QLabel(tr("x : ")))
  ,yLabel(new QLabel(tr("y : ")))
  ,shutterLabel(new QLabel("shutter:"))
  ,xStatusLabel(new QLabel(tr("Not connecting")))
  ,yStatusLabel(new QLabel(tr("Not connecting")))
  ,shutterStatusLabel(new QLabel("Not connecting"))

{
    ui->setupUi(this);

    //Load setting
    defaultSettings();
    loadStageSettings(MainWindow::Json);
}

MainWindow::~MainWindow()
{
    delete ui;
}

/* My function */

void MainWindow::setMenu()
{
    /* Menu bar */
    //File Menu
    QAction *openSTLAction = new QAction(tr("Open STL File"), this);
    QAction *openGCodeAction = new QAction(tr("Open GCode File"), this);
    connect(openSTLAction, SIGNAL(triggered()), this, SLOT(fileMenuSelected()));
    connect(openGCodeAction, SIGNAL(triggered()), this, SLOT(fileMenuSelected()));

    QMenu *fileMenu = this->menuBar()->addMenu(tr("File"));
    fileMenu->addAction(openSTLAction);
    fileMenu->addAction(openGCodeAction);

    //stage menu
    QAction *stageSettingAction = new QAction(tr("Stage Setting"), this);
    connect(stageSettingAction, SIGNAL(triggered()), this, SLOT(stageMenuSelected()));

    QMenu *stageMenu = this->menuBar()->addMenu(tr("Stage"));

    // set
    stageMenu->addAction(stageSettingAction);
}

/* Button Action */

void MainWindow::on_actionStageSetting_triggered()
{
    settingDialog->exec();
}

void MainWindow::on_actionCanOpenStage_triggered()
{
    // get serial communication status
    canOpenStageList = printTab->stageManager.canOpenStages();
    xStatusLabel->setText(canOpenStageList[EnumList::x]);
    yStatusLabel->setText(canOpenStageList[EnumList::y]);
    shutterStatusLabel->setText(canOpenStageList[EnumList::shutter]);    
}

/* received debug Message */

void MainWindow::outputMessage(QtMsgType type, const QString &msg)
{
    QString debugMsg = QTime::currentTime().toString("hh:mm:ss:zzz");

    switch (type) {
    case QtDebugMsg:
        debugMsg.append(QString(" [Debug] %1\n").arg(msg));
        break;
    case QtWarningMsg:
        debugMsg.append(QString(" [Warning] %1\n").arg(msg));

        break;
    case QtCriticalMsg:
        debugMsg.append(QString(" [Critical] %1\n").arg(msg));
        break;
    case QtFatalMsg:
        debugMsg.append(QString(" [Fatal] %1\n").arg(msg));
        abort();
    }

    ui->debugTextBrowser->append(debugMsg);
}

/* SLOTS */

void MainWindow::applySettings()
{
    loadStageSettings(MainWindow::Json);
}

/* Load and Save settings */

void MainWindow::defaultSettings()
{
    //Status bar
    ui->statusBar->addPermanentWidget(xLabel, 0);
    ui->statusBar->addPermanentWidget(xStatusLabel, 10);
    ui->statusBar->addPermanentWidget(yLabel, 0);
    ui->statusBar->addPermanentWidget(yStatusLabel, 10);
    ui->statusBar->addPermanentWidget(shutterLabel, 0);
    ui->statusBar->addPermanentWidget(shutterStatusLabel, 10);

    // create Dialog
    settingDialog = new StageSettingDialog(this);
    connect(settingDialog, SIGNAL(applySettings()), this, SLOT(applySettings()));

    //set Control Tabs
    convertTab = new ConvertPanel(this);
    ui->tabWidget->addTab(convertTab, QIcon(), tr("Convert"));
    editorTab = new EditorPanel(this);
    ui->tabWidget->addTab(editorTab, QIcon(), tr("G-Code"));

    printTab = new PrintPanel();
    ui->tabWidget->addTab(printTab, QIcon(), tr("Print Panel"));

    ui->tabWidget->setCurrentIndex(1);

    /* SIGNALS & SLOTS*/
    connect(convertTab, SIGNAL(sendGcodeText(QList<GCode*>)), editorTab, SLOT(receiveGcodeText(QList<GCode*>)));
    connect(editorTab, SIGNAL(sendGCodeListToGraphicArea(QList<GCode*>)), ui->graphicWidget, SLOT(drawLines(QList<GCode*>)));

}

void MainWindow::read(const QJsonObject &json)
{
    printTab->stageManager.loadStageSettings(json);
    settingDialog->read(json);
}

bool MainWindow::loadStageSettings(SaveFormat saveFormat)
{
    QFile loadFile(saveFormat == Json
                   ? QStringLiteral("save.json")
                   : QStringLiteral("save.dat"));
    if (!loadFile.open(QIODevice::ReadOnly))
    {
        qWarning("Couldn't open save file");
        qDebug("Couldn't open save file");
        return false;
    }else
    {
        qDebug("Succeeded in opening save file");
    }

    QByteArray saveData = loadFile.readAll();

    QJsonDocument loadDoc(saveFormat == Json
                            ? QJsonDocument::fromJson(saveData)
                            : QJsonDocument::fromBinaryData(saveData));
    read(loadDoc.object());

    return true;
}

