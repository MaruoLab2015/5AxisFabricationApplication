#include "editorpanel.h"
#include "ui_editorpanel.h"

#include <QDebug>

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

    ui->mainTextEdit->setText(gcText);

    emit sendGCodeListToGraphicArea(gcodeList);
}
