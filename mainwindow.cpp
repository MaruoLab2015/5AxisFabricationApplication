#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "stagesettingdialog.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
//    this->setMenu();
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
}

void MainWindow::setToolbar()
{
}

/* Button Action */

void MainWindow::on_actionStageSetting_triggered()
{
    qDebug() << "stage setting";
    StageSettingDialog *dialog = new StageSettingDialog(this);
    dialog->exec();
}

void MainWindow::on_actionOpenStage_triggered(bool checked)
{
    if (checked)
    {
        qDebug() << "check!";
    }else
    {
        qDebug() << "uncheck!";
    }
}
