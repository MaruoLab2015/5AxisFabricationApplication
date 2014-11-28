#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "settingDialog/stagesettingdialog.h"
#include "convert/convertpanel.h"
#include "editor/editorpanel.h"
#include "printpanel/printpanel.h"
#include "gcodelistdialog.h"

#include <QDebug>
#include <QLabel>
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>
#include <QTime>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
  ,sigmaLabel(new QLabel(tr("Sigma : ")))
  ,technoPhiLabel(new QLabel(tr("Techno Φ: ")))
  ,technoZLabel(new QLabel(tr("tZ : ")))
  ,technoThetaLabel(new QLabel(tr("tθ")))
  ,shutterLabel(new QLabel("shutter:"))
  ,sigmaStatusLabel(new QLabel(tr("Not connecting")))
  ,technoStatusPhiLabel(new QLabel(tr("Notconnecting")))
  ,technoStatusZLabel(new QLabel(tr("Notconnecting")))
  ,technoStatusThetaLabel(new QLabel(tr("Notconnecting")))
  ,shutterStatusLabel(new QLabel("Not connecting"))

{
    ui->setupUi(this);
    // ui setting
    QWidget *spacerWidget = new QWidget(this);
    spacerWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    spacerWidget->setVisible(true);
    ui->mainToolBar->insertWidget(ui->actionStop, spacerWidget);

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
    loadStageSettings(Json);
    settingDialog->exec();
}

void MainWindow::on_actionCanOpenStage_triggered()
{
    // get serial communication status
    canOpenStageList = printTab->stageManager.canOpenStages();
    technoStatusZLabel->setText(canOpenStageList[EnumList::zSupply]);
    technoStatusThetaLabel->setText(canOpenStageList[EnumList::thetaSupply]);
    technoStatusPhiLabel->setText(canOpenStageList[EnumList::phi]);
    sigmaStatusLabel->setText(canOpenStageList[EnumList::sigma]);
//    yStatusLabel->setText(canOpenStageList[EnumList::y]);
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
    ui->statusBar->addPermanentWidget(sigmaLabel, 0);
    ui->statusBar->addPermanentWidget(sigmaStatusLabel, 10);
    ui->statusBar->addPermanentWidget(technoPhiLabel, 0);
    ui->statusBar->addPermanentWidget(technoStatusPhiLabel, 0);
    ui->statusBar->addPermanentWidget(technoZLabel, 0);
    ui->statusBar->addPermanentWidget(technoStatusZLabel, 0);
    ui->statusBar->addPermanentWidget(technoThetaLabel, 0);
    ui->statusBar->addPermanentWidget(technoStatusThetaLabel, 10);
    ui->statusBar->addPermanentWidget(shutterLabel, 0);
    ui->statusBar->addPermanentWidget(shutterStatusLabel, 0);

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

    ui->tabWidget->setCurrentIndex(2);

    /* SIGNALS & SLOTS*/
    connect(convertTab, SIGNAL(sendGcodeText(QString)), editorTab, SLOT(receiveGcodeText(QString)));
    connect(editorTab, SIGNAL(sendGCodeListToGraphicArea(QList<GCode*>)), ui->graphicWidget, SLOT(drawLines(QList<GCode*>)));
    connect(editorTab, SIGNAL(changedCurrBlockNumber(int)), ui->graphicWidget, SLOT(changedCurrBlockNumber(int)));

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
//        qWarning("Couldn't open save file");
        qDebug("Couldn't open save file");
        return false;
    }else
    {
//        qDebug("Succeeded in opening save file");
    }

    QByteArray saveData = loadFile.readAll();

    QJsonDocument loadDoc(saveFormat == Json
                            ? QJsonDocument::fromJson(saveData)
                            : QJsonDocument::fromBinaryData(saveData));
    read(loadDoc.object());

    return true;
}

void MainWindow::on_actionStop_triggered()
{
    printTab->stageManager.stopStages();
}

void MainWindow::on_actionGCode_triggered()
{
    GcodeListDialog *gDialog = new GcodeListDialog(this);
    gDialog->exec();
}

void MainWindow::on_actionOpenGCode_triggered()
{
    editorTab->on_openGcodeButton_clicked();
}
