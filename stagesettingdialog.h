#ifndef STAGESETTINGDIALOG_H
#define STAGESETTINGDIALOG_H

#include <QDialog>
#include <QTableWidget>
#include <QDialogButtonBox>
#include <QSerialPort>

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

    void read(const QJsonObject &json);
    void write(QJsonObject &json) const;

Q_SIGNALS:
    void accepted(QString *string);
    void applySetting(QString portName,
                      int baudrate,
                      QSerialPort::StopBits stopbits,
                      QSerialPort::Parity parity,
                      int waitTime);


private slots:
    void on_buttonBox_clicked(QAbstractButton *button);
    void on_campanyComboBox_currentIndexChanged(int index);

    void on_pushButton_clicked();

private:
    Ui::StageSettingDialog *ui;
    void setCompanyComboBoxButton();
    void setComboBoxSetting();
};

#endif // STAGESETTINGDIALOG_H
