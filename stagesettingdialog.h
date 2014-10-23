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

public:
    explicit StageSettingDialog(QWidget *parent = 0);

    ~StageSettingDialog();

private slots:
    void on_buttonBox_clicked(QAbstractButton *button);

private:
    Ui::StageSettingDialog *ui;

};

#endif // STAGESETTINGDIALOG_H
