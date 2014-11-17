#ifndef PRINTPANEL_H
#define PRINTPANEL_H

#include <QWidget>

#include "stagecontroller.h"
#include "enumList.h"
#include "model/gcode.h"

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

//    void on_homeX_clicked(){stageManager.moveHome(EnumList::x);};
//    void on_homeY_clicked(){stageManager.moveHome(EnumList::y);};
    void on_cpX_clicked();//{stageManager.getStagePositions(EnumList::x);};
    void on_cpY_clicked();
//    void on_moveN100X_clicked(){stageManager.move(EnumList::x, -100.f, false);};
//    void on_moveN100Y_clicked(){stageManager.move(EnumList::y, -100.f, false);};
//    void on_moveN10X_clicked(){stageManager.move(EnumList::x, -10.f, false);};
//    void on_moveN10Y_clicked(){stageManager.move(EnumList::y, -10.f, false);};
//    void on_moveN1X_clicked(){stageManager.move(EnumList::x, -1.f, false);};
//    void on_moveN1Y_clicked(){stageManager.move(EnumList::y, -1.f, false);};
//    void on_moveP1X_clicked(){stageManager.move(EnumList::x, 1.f, false);};
//    void on_moveP1Y_clicked(){stageManager.move(EnumList::y, 1.f, false);};
//    void on_moveP10X_clicked(){stageManager.move(EnumList::x, 10.f, false);};
//    void on_moveP10Y_clicked(){stageManager.move(EnumList::y, 10.f, false);};
//    void on_moveP100X_clicked(){stageManager.move(EnumList::x, 100.f, false);};
//    void on_moveP100Y_clicked(){stageManager.move(EnumList::y, 100.f, false);};

    void on_shutterOpenButton_clicked(){stageManager.pressTheShutter(true);};
    void on_shutterCloseButton_clicked(){stageManager.pressTheShutter(false);};

    void on_supplyButton_clicked(){stageManager.supplyAction();}
    void on_homePhi_clicked(){stageManager.moveHome(EnumList::phi);}

    void on_cpPhi_clicked();

private:
    Ui::PrintPanel *ui;
};

#endif // PRINTPANEL_H
