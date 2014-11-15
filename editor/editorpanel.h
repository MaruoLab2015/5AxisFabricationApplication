#ifndef EDITORPANEL_H
#define EDITORPANEL_H

#include <QWidget>

#include "model/gcode.h"

namespace Ui {
class EditorPanel;
}

class EditorPanel : public QWidget
{
    Q_OBJECT

public:
    explicit EditorPanel(QWidget *parent = 0);
    ~EditorPanel();

    QList<GCode> gcodeList;

signals:
    void sendGCodeListToGraphicArea(QList<GCode *> gcodeList);

public slots:
    void receiveGcodeText(QList<GCode*> gcodeList);

private slots:
    void on_openGcodeButton_clicked();
    void on_saveGcodeButton_clicked();

private:
    Ui::EditorPanel *ui;
};

#endif // EDITORPANEL_H
