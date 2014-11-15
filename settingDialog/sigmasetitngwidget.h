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
    Q_ENUMS( SigmaAxis )

public:

    enum SigmaAxis {
        None,
        x,
        y,
        z,
        theta
    };

    explicit SigmaSetitngWidget(QWidget *parent = 0);
    ~SigmaSetitngWidget();

    void read(const QJsonObject &json);
    void write(QJsonObject &json) const;

private:
    Ui::SigmaSetitngWidget *ui;

    void initialComboBoxContent();
    void initialSerialComboBoxButton(QString *name, QComboBox *box);
    void initSigmaAxisComboBox();
};

#endif // SIGMASETITNGWIDGET_H
