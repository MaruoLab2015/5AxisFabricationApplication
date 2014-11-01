#ifndef PRINTPANEL_H
#define PRINTPANEL_H

#include <QWidget>

#include "stagecontroller.h"
#include "enumList.h"
#include "gcode.h"

namespace Ui {
class PrintPanel;
}

class PrintPanel : public QWidget
{
    Q_OBJECT

public:
    explicit PrintPanel(QWidget *parent = 0);
    ~PrintPanel();

    StageController stageManager;

signals:
    void sendLineEditText(QString lineEditText);
    void sendRequestToStage(QString request, EnumList::Axis axis);

private slots:

    void on_sendRequestButton_clicked();

    void on_homeX_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::PrintPanel *ui;
};

#endif // PRINTPANEL_H
