#include "convertpanel.h"
#include "ui_convertpanel.h"
#include "model/gcode.h"

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
    QString fabDir = QFileDialog::getExistingDirectory(this,
                                               tr("Open Directory")
                                               );
    QDir dir(fabDir);

    for (uint i=0;i<dir.count();i++)
    {
        QString modelName = QString("model.%2.csv")
                .arg(i, 3, 10, QChar('0'));
        if (dir.exists(modelName))
            continue;
        else
        {
            ui->endLayerLineEdit->setText(QString::number(i-1));
            break;
        }
    }

    ui->modelDirLineEdit->setText(fabDir);
    ui->covertNameLineEdit->setText(dir.dirName());
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
    QList<GCode*> gcodeList;
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
            QString gString = convertCSVToGCodeXY(in.readLine());
            gcodeText.append(gString);
            GCode *g = new GCode();
            g->parse(gString);
            gcodeList.append(g);
        }

        file.close();

        QString gString = convertCSVToGCodeZ(ui->layerPitchLineEdit->text().toFloat() * (i + 1) );
        gcodeText.append(gString);
        GCode *g = new GCode();
        g->parse(gString);
        gcodeList.append(g);
    }

    // create gcode File
    QString fileName = ui->covertNameLineEdit->text();
    fileName += ".gcode";
    if (!fileName.isEmpty())
    {
        QFile file(fileName);
        if (!file.open(QIODevice::WriteOnly))
        {
            qDebug() << "Fail to create GCode File";

        }else{
            QTextStream stream(&file);
            stream << gcodeText.toUtf8().data();
            stream.flush();
            file.close();
            qDebug() << "Succeeded converting to Gcode.";
        }
    }

    emit sendGcodeText(fileName);
}

QString ConvertPanel::convertCSVToGCodeXY(QString line)
{
    line = line.trimmed();
    QStringList list = line.split(",");
    QString gc = "";

    // Only "x,y,e"
    if(list.count() != 3) return gc;

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
