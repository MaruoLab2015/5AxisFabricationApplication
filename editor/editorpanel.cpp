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

void EditorPanel::receiveGcodeText(QList<GCode*> gcodeList)
{
    GCode *gcode;
    QString gcText;
    foreach(gcode, gcodeList)
    {
        gcText.append(gcode->origText);
    }

    ui->mainTextEdit->textCursor().setPosition(0);
    ui->mainTextEdit->setText(gcText);

    emit sendGCodeListToGraphicArea(gcodeList);
}

void EditorPanel::on_openGcodeButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), QString(),
            tr("Text Files (*.txt);;Gcode Files(*.gcode)"));

    if (!fileName.isEmpty())
    {
        QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly))
        {
            QMessageBox::critical(this, tr("Error"), tr("Could not open file"));
            return;
        }

        QTextStream in(&file);
        ui->mainTextEdit->setText(in.readAll());
        file.close();
    }

}

void EditorPanel::on_saveGcodeButton_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Files"), QString(),
                                                    tr("Text Files (*.txt);;Gcode Files(*.gcode)"));
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
