#ifndef STAGESETTINGDIALOG_H
#define STAGESETTINGDIALOG_H

#include <QDialog>
#include <QTableWidget>
#include <QDialogButtonBox>

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
        TechnoHands,
        SigmaStage,
        Shutter
    };

private slots:
    void on_buttonBox_clicked(QAbstractButton *button);

private:
    Ui::StageSettingDialog *ui;
    void setCompanyComboBoxButton();

};

#endif // STAGESETTINGDIALOG_H
