#include "editorpanel.h"
#include "ui_editorpanel.h"

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
