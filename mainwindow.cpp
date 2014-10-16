#include "mainwindow.h"
#include "ui_mainwindow.h"
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

/* SLOT */
void MainWindow::fileMenuSelected()
{
    QAction *action = qobject_cast<QAction *>(sender());
    if(action)
    {
        qDebug() << "File Menu: " << action->text();
    }
}

void MainWindow::stageMenuSelected()
{

    QAction *action = qobject_cast<QAction *>(sender());
    if(action)
    {
        qDebug() << "Stage Menu: " << action->text();
    }
}


void MainWindow::on_actionStageSetting_triggered()
{
    qDebug() << "stage setting";
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
