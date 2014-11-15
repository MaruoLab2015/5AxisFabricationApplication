#ifndef STAGEWIDGET_H
#define STAGEWIDGET_H

#include <QWidget>
#include <QtWidgets>

class StageWidget : public QWidget
{
    Q_OBJECT
public:
    explicit StageWidget(QWidget *parent = 0);

    QComboBox *portComboBox;
    QComboBox *baudrateComboBox;
    QComboBox *stopbitsComboBox;
    QComboBox *parityComboBox;
    QCheckBox *isEnableCheckBox;

    void read(const QJsonObject &json);
    void write(QJsonObject &json) const;

signals:

public slots:
    void enableCheckBoxChanged(bool);

private:
    void initialLayout();
    void initialComboBoxContent();
    void initialSerialComboBoxButton(QString *name, QComboBox *box);
};

#endif // STAGEWIDGET_H
