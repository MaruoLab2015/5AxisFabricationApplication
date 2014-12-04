#include "editorpanel.h"
#include "ui_editorpanel.h"

#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>

EditorPanel::EditorPanel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EditorPanel),
    currentBlockNumber(0),
    isChangeText(false)
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
        openedFileName = QString(fileName);
    }
}

void EditorPanel::on_saveGcodeButton_clicked()
{
    if (!openedFileName.isEmpty())
    {
        QFile file(openedFileName);
        if (!file.open(QIODevice::WriteOnly))
        {

        }else{
            QTextStream stream(&file);
            stream << ui->mainTextEdit->toPlainText();
            stream.flush();
            file.close();
        }
        return;
    }

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
    isChangeText = false;

    return gcodeList;
}

void EditorPanel::on_mainTextEdit_cursorPositionChanged()
{
    if (isChangeText)
    {
        gcodeList.clear();

        QString tmpText = ui->mainTextEdit->toPlainText();
        QStringList list = tmpText.split("\n");

        for(int i=0;i<list.count();i++)
        {
            QString gString = list.at(i);
            GCode *g = new GCode();
            g->parse(gString);
            gcodeList.append(g);
        }

        emit sendGCodeListToGraphicArea(gcodeList);
    }
    isChangeText = false;

    if (currentBlockNumber == ui->mainTextEdit->textCursor().blockNumber())
        return;

    currentBlockNumber = ui->mainTextEdit->textCursor().blockNumber();

    emit changedCurrBlockNumber(currentBlockNumber);
}

void EditorPanel::on_mainTextEdit_textChanged()
{
    isChangeText = true;
}
