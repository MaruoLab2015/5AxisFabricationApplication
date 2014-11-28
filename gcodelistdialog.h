#ifndef GCODELISTDIALOG_H
#define GCODELISTDIALOG_H

#include <QDialog>

namespace Ui {
class GcodeListDialog;
}

class GcodeListDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GcodeListDialog(QWidget *parent = 0);
    ~GcodeListDialog();

private:
    Ui::GcodeListDialog *ui;
};

#endif // GCODELISTDIALOG_H
