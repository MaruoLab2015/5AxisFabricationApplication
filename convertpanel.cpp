#include "convertpanel.h"
#include "ui_convertpanel.h"

#include <QFileDialog>
#include <QDebug>

ConvertPanel::ConvertPanel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ConvertPanel)
{
    ui->setupUi(this);
}

ConvertPanel::~ConvertPanel()
{
    delete ui;
}

void ConvertPanel::on_openFolderPathButton_clicked()
{
    QFileDialog::Options options = QFileDialog::DontResolveSymlinks | QFileDialog::ShowDirsOnly;
    fabDir = QFileDialog::getExistingDirectory(this,
                                                       tr("Open Directory"),
                                                       QString(),
                                                       options
                                                       );
    ui->fabFolderLineEdit->setText(fabDir);
}

void ConvertPanel::on_openLayerFolderButton_clicked()
{
    layerPath = QFileDialog::getOpenFileName(this,
                                                     tr("Open layer file"),
                                                     QString(),
                                                     tr("CSV Files (*.csv)"));
    ui->layerLineEdit->setText(layerPath);

}
