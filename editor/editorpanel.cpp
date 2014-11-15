#include "editorpanel.h"
#include "ui_editorpanel.h"

#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>

EditorPanel::EditorPanel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EditorPanel)
{
    ui->setupUi(this);
}

EditorPanel::~EditorPanel()
{
    delete ui;
}

void EditorPanel::receiveGcodeText(QString fileName)
{
    QList<GCode*> gcodeList = createGcodeFromPlainText(fileName);
    emit sendGCodeListToGraphicArea(gcodeList);
}

void EditorPanel::on_openGcodeButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(
                this,
                tr("Open File"),
                QString(),
                QString(tr("Gcode Files(*.gcode)")));

    if (!fileName.isEmpty())
    {
        QList<GCode*> gcodeList = createGcodeFromPlainText(fileName);
        emit sendGCodeListToGraphicArea(gcodeList);
    }
}

void EditorPanel::on_saveGcodeButton_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(
                this,
                tr("Save Files"),
                QString(),
                QString(tr("Gcode Files(*.gcode)")));
    if (!fileName.isEmpty())
    {
        QFile file(fileName);
        if (!file.open(QIODevice::WriteOnly))
        {

        }else{
            QTextStream stream(&file);
            stream << ui->mainTextEdit->toPlainText();
            stream.flush();
            file.close();
        }
    }
}

QList<GCode*> EditorPanel::createGcodeFromPlainText(QString fileName)
{
    QList<GCode*> gcodeList;
    QString gcodeText;

    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::critical(this, tr("Error"), tr("Could not open file"));
        return gcodeList;
    }
    QTextStream in(&file);
    while(!in.atEnd())
    {
        QString gString = in.readLine();
        gcodeText.append(gString);
        gcodeText.append("\n");
        GCode *g = new GCode();
        g->parse(gString);
        gcodeList.append(g);
    }
    file.close();

    ui->mainTextEdit->setText(gcodeText);

    return gcodeList;
}
