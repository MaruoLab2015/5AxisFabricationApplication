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

    QComboBox *portComboBox;
    QComboBox *baudrateComboBox;
    QComboBox *stopbitsComboBox;
    QComboBox *parityComboBox;

    void read(const QJsonObject &json);
    void write(QJsonObject &json) const;

signals:

public slots:
    void companyCurrentIndexChanged(int);

private:
    void initialLayout();
    void initialComboBoxContent();
};

#endif // STAGEWIDGET_H
