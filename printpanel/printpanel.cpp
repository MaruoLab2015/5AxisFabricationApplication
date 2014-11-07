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
    QString str = QString("g : %1, x : %2, y : %3").arg(gc->g).arg(gc->x).arg(gc->y);


    if( gc->hasG())
    {
        switch (gc->g) {
        case 1:
            if (gc->hasX()) stageManager.move(EnumList::x, gc->x, false);
            if (gc->hasY()) stageManager.move(EnumList::y, gc->y, false);
            break;
        default:
            break;
        }
        return;
    }

    if (ui->sendAxisComboBox->currentIndex() != 0)
        stageManager.receiveRequest(ui->requestLineEdit->text(), (EnumList::Axis)(ui->sendAxisComboBox->currentIndex()-1));
}

void PrintPanel::on_cpX_clicked()
{
    stageManager.getStagePositions(EnumList::x);
    ui->positionX->setText(QString::number(stageManager.x));
};

void PrintPanel::on_cpY_clicked()
{
    stageManager.getStagePositions(EnumList::y);
    ui->positionY->setText(QString::number(stageManager.y));
};
