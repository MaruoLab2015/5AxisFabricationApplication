#include "stagesettingdialog.h"
#include "ui_stagesettingdialog.h"

StageSettingDialog::StageSettingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StageSettingDialog)
{
    ui->setupUi(this);
}

StageSettingDialog::~StageSettingDialog()
{
    delete ui;
}
