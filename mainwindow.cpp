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
  ,transactionCount(0)
  ,statusLabel(new QLabel(tr("Status: Not runnning.")))
{
    ui->setupUi(this);

    //Status bar
    ui->statusBar->addPermanentWidget(statusLabel, 100);

    /* SIGNALS & SLOTS*/
    connect(&xAxisThread, SIGNAL(response(QString)), this, SLOT(showResponse(QString)));
    connect(&xAxisThread, SIGNAL(error(QString)), this, SLOT(processError(QString)));
    connect(&xAxisThread, SIGNAL(timeout(QString)), this, SLOT(processTimeout(QString)));

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

    stageMenu->addAction(stageSettingAction);
    xAxisThread.setAxis(xAxisThread.XAxis);
}

/* Button Action */

void MainWindow::on_actionStageSetting_triggered()
{
//    qDebug() << "stage setting";

    settingDialog->exec();
}

void MainWindow::applySettings(QString portName, int baudrate, QSerialPort::StopBits stopbits, QSerialPort::Parity parity, int waitTime)
{

    xAxisThread.setSerialSettings(portName, waitTime, baudrate, parity, stopbits);
    saveStageSettings(MainWindow::Json);
}

/* Serial Communication */

void MainWindow::on_actionOpenStage_triggered(bool checked)
{
    QString request = "aiueo";
    if (checked)
    {
//        qDebug() << "check!";
        statusLabel->setText(tr("Status: Running, connected to port %1.")
                             .arg(xAxisThread.portName));
        ui->debugTextBrowser->append(statusLabel->text());
        xAxisThread.transaction(request);
    }else
    {
//        qDebug() << "uncheck!";
    }
}


void MainWindow::showResponse(const QString &s)
{

    ui->debugTextBrowser->append(tr("Traffic, transaction #%1:"
//                             "\n\r-request: %2"
                             "\n\r-response: %2")
                          .arg(++transactionCount).arg(s));
}

void MainWindow::processError(const QString &s)
{

    statusLabel->setText(tr("Status: Not runnning, %1").arg(s));
    ui->debugTextBrowser->append(statusLabel->text());
    ui->actionOpenStage->setChecked(false);
}

void MainWindow::processTimeout(const QString &s)
{
    statusLabel->setText(tr("Status: Running, %1.").arg(s));
    ui->debugTextBrowser->append(statusLabel->text());
    ui->actionOpenStage->setChecked(false);

}

/* Load and Save settings */

void MainWindow::defaultSettings()
{

    QSerialPort serial;
    xAxisThread.setSerialSettings(QString("COM1"),
                                  1000,
                                  115200,
                                  serial.NoParity,
                                  serial.OneStop
                                  );

    // create Dialog
    settingDialog = new StageSettingDialog(this);
    connect(settingDialog, SIGNAL(applySetting(QString,int,QSerialPort::StopBits,QSerialPort::Parity,int)),
            this, SLOT(applySettings(QString,int,QSerialPort::StopBits,QSerialPort::Parity,int)));

}

void MainWindow::read(const QJsonObject &json)
{

    xAxisThread.read(json["xaxis"].toObject());
    settingDialog->read(json["xaxisDialog"].toObject());
}

void MainWindow::write(QJsonObject &json) const
{

    QJsonObject xAxisObject;
    QJsonObject xAxisDialogObject;
    xAxisThread.write(xAxisObject);
    settingDialog->write(xAxisDialogObject);
    json["xaxis"] = xAxisObject;
    json["xaxisDialog"] = xAxisDialogObject;
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

bool MainWindow::saveStageSettings(SaveFormat saveFormat) const
{

    QFile saveFile(saveFormat == Json
                   ? QStringLiteral("save.json")
                   : QStringLiteral("save.dat"));

    if (!saveFile.open(QIODevice::WriteOnly))
    {
        qWarning("Couldn't open save File");
        return false;
    }

    QJsonObject settingObject;
    write(settingObject);

    QJsonDocument saveDoc(settingObject);
    saveFile.write(saveFormat == Json
                   ? saveDoc.toJson()
                   : saveDoc.toBinaryData());

    return true;
}
