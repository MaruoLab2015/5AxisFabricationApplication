#include "printpanel.h"
#include "ui_printpanel.h"

#include <QDebug>

PrintPanel::PrintPanel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PrintPanel)
{
    ui->setupUi(this);
    connect(this, SIGNAL(sendLineEditText(QString)), &stageManager, SLOT(receiveLineEditText(QString)));
    connect(this, SIGNAL(sendRequestToStage(QString,EnumList::Axis)), &stageManager, SLOT(receiveRequest(QString,EnumList::Axis)));
}

PrintPanel::~PrintPanel()
{
    delete ui;
}

void PrintPanel::on_sendRequestButton_clicked()
{
    GCode *gc = new GCode();
    gc->parse(ui->requestLineEdit->text());
    QString str = QString("g : %1, x : %2, y : %3").arg(gc->g).arg(gc->x).arg(gc->y);


    qDebug() << gc->hasG();//str.toUtf8().data() ;
    emit sendLineEditText(ui->requestLineEdit->text());
}

//void PrintPanel::on_moveN100X_clicked()
//{

//}

//void PrintPanel::on_moveN10X_clicked()
//{

//}

//void PrintPanel::on_moveN1X_clicked()
//{

//}

//void PrintPanel::on_moveP1X_clicked()
//{

//}

//void PrintPanel::on_moveP10X_clicked()
//{

//}

//void PrintPanel::on_moveP100X_clicked()
//{

//}
