#include "convertpanel.h"
#include "ui_convertpanel.h"

#include <QFileDialog>
#include <QDebug>
#include <QMessageBox>

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
    QString fabDir = QFileDialog::getExistingDirectory(this,
                                               tr("Open Directory"),
                                               QString(),
                                               options
                                               );
    QDir dir(fabDir);
    ui->endLayerLineEdit->setText(QString::number(dir.count() - 3));
    ui->modelDirLineEdit->setText(fabDir);
}

void ConvertPanel::on_openLayerFolderButton_clicked()
{
    QString layerPath = QFileDialog::getOpenFileName(this,
                                             tr("Open layer file"),
                                             QString(),
                                             tr("CSV Files (*.csv)"));
    ui->layerLineEdit->setText(layerPath);

}

void ConvertPanel::on_convertPushButton_clicked()
{
    QString gcodeText;
    startLayer = ui->startLayerLineEdit->text().toInt();
    endLayer = ui->endLayerLineEdit->text().toInt();

    // read model files
    for (int i=startLayer;i<=endLayer;i++)
    {

        if (ui->modelDirLineEdit->text().isEmpty())
        {
            QMessageBox msgBox(this);
            msgBox.setText(tr("Can't read model file"));
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.exec();
            return;
        }
        QString modelFilePath = QString("%1/model.%2.csv")
                .arg(ui->modelDirLineEdit->text()).arg(i, 3, 10, QChar('0'));
        QFile file(modelFilePath);
        if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QMessageBox::critical(this, tr("Error"), tr("Could not open file"));
            return;
        }
        QTextStream in(&file);
        while(!in.atEnd())
        {
            gcodeText.append(convertCSVToGCodeXY(in.readLine()));
        }

        file.close();

        gcodeText.append(convertCSVToGCodeZ(ui->layerPitchLineEdit->text().toFloat()));
    }

    emit sendGcodeText(gcodeText);

}

QString ConvertPanel::convertCSVToGCodeXY(QString line)
{
    line = line.trimmed();
    QStringList list = line.split(",");
    QString gc = "";
    if(list.count() == 0) return gc;

    QString e = "0";
    if (list[2] == QString("1")) e = "1";
    gc.append(QString("G1 X%1 Y%2 E%3\n").arg(list[0]).arg(list[1]).arg(e));
    return gc;
}

QString ConvertPanel::convertCSVToGCodeZ(float layerPitch)
{
    QString gc = QString("G1 Z%1\n").arg(QString::number(layerPitch));
    return gc;
}
