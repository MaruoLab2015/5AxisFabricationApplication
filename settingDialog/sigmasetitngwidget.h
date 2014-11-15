#ifndef SIGMASETITNGWIDGET_H
#define SIGMASETITNGWIDGET_H

#include <QWidget>

class QComboBox;

namespace Ui {
class SigmaSetitngWidget;
}

class SigmaSetitngWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SigmaSetitngWidget(QWidget *parent = 0);
    ~SigmaSetitngWidget();

private:
    Ui::SigmaSetitngWidget *ui;

    void initialComboBoxContent();
    void initialSerialComboBoxButton(QString *name, QComboBox *box);
};

#endif // SIGMASETITNGWIDGET_H
