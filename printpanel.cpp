#include "printpanel.h"
#include "ui_printpanel.h"

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
    emit sendLineEditText(ui->requestLineEdit->text());
}
