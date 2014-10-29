#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "stagesettingdialog.h"

#include <QDebug>
#include <QLabel>
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
  ,xLabel(new QLabel(tr("x : ")))
  ,yLabel(new QLabel(tr("y : ")))
  ,xStatusLabel(new QLabel(tr("Not connecting")))
  ,yStatusLabel(new QLabel(tr("Not connecting")))
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
    this->showDebugLog(xStatusLabel->text());

//    stageManeger.getStagePositions();
}

/* SLOTS */
void MainWindow::showDebugLog(const QString &s)
{
    qDebug() << "debug";
    ui->debugTextBrowser->append(s);
}

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

    // create Dialog
    settingDialog = new StageSettingDialog(this);
    connect(settingDialog, SIGNAL(applySettings()), this, SLOT(applySettings()));

    //set Control Tabs
    printTab = new PrintPanel();
    ui->tabWidget->addTab(printTab, QIcon(), tr("Print Panel"));
    ui->tabWidget->setCurrentIndex(3);

    /* SIGNALS & SLOTS*/
//    connect(printTab->stageManager, SIGNAL(sendDebugMessage(QString)), this, SLOT(showDebugLog(QString)));

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
        return false;
    }

    QByteArray saveData = loadFile.readAll();

    QJsonDocument loadDoc(saveFormat == Json
                            ? QJsonDocument::fromJson(saveData)
                            : QJsonDocument::fromBinaryData(saveData));
    read(loadDoc.object());

    return true;
}

