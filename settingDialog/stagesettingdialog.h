#ifndef STAGESETTINGDIALOG_H
#define STAGESETTINGDIALOG_H

#include <QDialog>
#include <QTableWidget>
#include <QDialogButtonBox>
#include <QSerialPort>

class StageWidget;
class SigmaSetitngWidget;
class TechnohandsSettingWidget;
class QComboBox;

namespace Ui {
class StageSettingDialog;
}

class StageSettingDialog : public QDialog
{
    Q_OBJECT
    Q_ENUMS( Company )

public:
    explicit StageSettingDialog(QWidget *parent = 0);

    ~StageSettingDialog();

    enum Company {
        TechnoHands = 0,
        SigmaStage = 1,
        Shutter = 2
    };

    enum SaveFormat{
        Json, Binary
    };

    void read(const QJsonObject &json);
    void write(QJsonObject &json) const;

    bool saveStageSettingss(SaveFormat saveFormat) const;

signals:
    void applySettings();

private slots:
    void on_buttonBox_clicked(QAbstractButton *button);

private:
    Ui::StageSettingDialog *ui;

    SigmaSetitngWidget *sigmaWidget;
    TechnohandsSettingWidget *technoWidget;
    StageWidget *shutterWidget;
};

#endif // STAGESETTINGDIALOG_H
