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
  ,statusLabel(new QLabel(tr("Status: Not runnning.")))
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
//    qDebug() << "stage setting";

    settingDialog->exec();
}

/* Serial Communication */

void MainWindow::on_actionOpenStage_triggered(bool checked)
{
    QString request = "aiueo";
    if (checked)
    {

        statusLabel->setText(tr("Status: Running, connected to ports."));
        ui->debugTextBrowser->append(statusLabel->text());
        masterThread.openStages();
    }else
    {

        masterThread.closeStages();
    }
}

/* SLOTS */
void MainWindow::showDebugLog(const QString &s, bool isError)
{
    ui->debugTextBrowser->append(s);

    if (isError)
    {
        ui->actionOpenStage->setChecked(false);
        statusLabel->setText("Status: Not runnning.");
    }
}

void MainWindow::applySettings()
{
    loadStageSettings(MainWindow::Json);
}

/* Load and Save settings */

void MainWindow::defaultSettings()
{

    //Status bar
    ui->statusBar->addPermanentWidget(statusLabel, 100);

    /* SIGNALS & SLOTS*/
    connect(&masterThread, SIGNAL(sendDebugMessage(QString,bool)), this, SLOT(showDebugLog(QString,bool)));

    // create Dialog
    settingDialog = new StageSettingDialog(this);
    connect(settingDialog, SIGNAL(applySettings()), this, SLOT(applySettings()));

    //set Control Tabs
    printTab = new PrintPanel();
    connect(printTab, SIGNAL(sendLineEditText(QString)), &masterThread, SLOT(receiveRequestText(QString)));
//    connect(printTab, SIGNAL(sendLineEditText(QString), masterThread.xStage, SLOT(receiveRequestText(QString))));
    ui->tabWidget->addTab(printTab, QIcon(), tr("Print Panel"));
    ui->tabWidget->setCurrentIndex(3);

}

void MainWindow::read(const QJsonObject &json)
{

    masterThread.read(json);
    settingDialog->read(json);
}

bool MainWindow::loadStageSettings(SaveFormat saveFormat)
{
//    qDebug() << "loadStageSettings";

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
