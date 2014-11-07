#ifndef STAGEWIDGET_H
#define STAGEWIDGET_H

#include <QWidget>
#include <QtWidgets>

class StageWidget : public QWidget
{
    Q_OBJECT
    Q_ENUMS( Company )
public:
    explicit StageWidget(QWidget *parent = 0);

    enum Company {
        TechnoHands = 0,
        SigmaStage = 1,
        Shutter = 2
    };

    QComboBox *portComboBox;
    QComboBox *companyComboBox;
    QComboBox *baudrateComboBox;
    QComboBox *stopbitsComboBox;
    QComboBox *parityComboBox;
    QSpinBox *waitTimeSpinBox;
    QDialogButtonBox *dialogButtonBox;

    void read(const QJsonObject &json);
    void write(QJsonObject &json) const;

signals:

public slots:
    void companyCurrentIndexChanged(int);

private:
    void initialLayout();
    void initialComboBoxContent();
    void initialCompanyComboBoxButton();
    void initialSignalsAndSlotsConnect();
};

#endif // STAGEWIDGET_H
