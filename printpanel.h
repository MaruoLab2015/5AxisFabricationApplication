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

    void on_homeX_clicked(){stageManager.moveHome(EnumList::x);};
    void on_cpX_clicked(){stageManager.getStagePositions(EnumList::x);};

    void on_moveN100X_clicked(){stageManager.move(EnumList::x, -100.f, false);};
    void on_moveN10X_clicked(){stageManager.move(EnumList::x, -10.f, false);};;
    void on_moveN1X_clicked(){stageManager.move(EnumList::x, -1.f, false);};;
    void on_moveP1X_clicked(){stageManager.move(EnumList::x, 1.f, false);};;
    void on_moveP10X_clicked(){stageManager.move(EnumList::x, 10.f, false);};;
    void on_moveP100X_clicked(){stageManager.move(EnumList::x, 100.f, false);};;

private:
    Ui::PrintPanel *ui;
};

#endif // PRINTPANEL_H
