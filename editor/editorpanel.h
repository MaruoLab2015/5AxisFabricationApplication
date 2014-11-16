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

    QList<GCode*> gcodeList;

    QList<GCode *> createGcodeFromPlainText(QString text);

signals:
    void sendGCodeListToGraphicArea(QList<GCode *> gcodeList);
    void changedCurrBlockNumber(int blockNumber);

public slots:
    void receiveGcodeText(QString gcodeText);

private slots:
    void on_openGcodeButton_clicked();
    void on_saveGcodeButton_clicked();

    void on_mainTextEdit_cursorPositionChanged();
    void on_mainTextEdit_textChanged();

private:
    Ui::EditorPanel *ui;
    int currentBlockNumber;
    bool isChangeText;
};

#endif // EDITORPANEL_H
