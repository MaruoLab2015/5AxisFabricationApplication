#ifndef STAGESETTINGDIALOG_H
#define STAGESETTINGDIALOG_H

#include <QDialog>

namespace Ui {
class StageSettingDialog;
}

class StageSettingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit StageSettingDialog(QWidget *parent = 0);
    ~StageSettingDialog();

private:
    Ui::StageSettingDialog *ui;
};

#endif // STAGESETTINGDIALOG_H
