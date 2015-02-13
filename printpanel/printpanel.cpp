#include "printpanel.h"
#include "ui_printpanel.h"

#include <QDebug>

PrintPanel::PrintPanel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PrintPanel)
{
    ui->setupUi(this);
}

PrintPanel::~PrintPanel()
{
    delete ui;
}

void PrintPanel::on_sendRequestButton_clicked()
{
    GCode *gc = new GCode();
    gc->parse(ui->requestLineEdit->text());
//    QString str = QString("g : %1, x : %2, y : %3").arg(gc->g).arg(gc->x).arg(gc->y);


//    if( gc->hasCode())
//    {
//        if (gc->hasX()) stageManager.move(EnumList::x, gc->x, true);
//        if (gc->hasY()) stageManager.move(EnumList::y, gc->y, true);
//        if (gc->hasZ()) stageManager.move(EnumList::z, gc->z, true);
//        if (gc->hasT()) stageManager.move(EnumList::theta, gc->t, true);
//        if (gc->hasP()) stageManager.move(EnumList::phi, gc->p, true);

//        return;
//    }

    if (ui->sendAxisComboBox->currentIndex() != 0){
        stageManager.receiveRequest(ui->requestLineEdit->text(), (EnumList::Axis)(ui->sendAxisComboBox->currentIndex()-1));
    }
}

void PrintPanel::on_cpX_clicked()
{
    stageManager.getStagePositions(EnumList::x);
    setSigmaCurrentPositionLabel();
};

void PrintPanel::on_cpY_clicked()
{
    stageManager.getStagePositions(EnumList::y);
    setSigmaCurrentPositionLabel();
};

void PrintPanel::on_cpZ_clicked()
{
    stageManager.getStagePositions(EnumList::z);
    setSigmaCurrentPositionLabel();
}

void PrintPanel::on_cpTheta_clicked()
{
    stageManager.getStagePositions(EnumList::theta);
    setSigmaCurrentPositionLabel();
}

void PrintPanel::on_cpPhi_clicked()
{
    stageManager.getStagePositions(EnumList::phi);
    ui->positionPhi->setText(QString::number(stageManager.phi));
}

void PrintPanel::setSigmaCurrentPositionLabel()
{
    ui->positionX->setText(QString::number(stageManager.x));
    ui->positionY->setText(QString::number(stageManager.y));
    ui->positionZ->setText(QString::number(stageManager.z));
    ui->positionTheta->setText(QString::number(stageManager.theta));
}
