#ifndef TECHNOHANDSSETTINGWIDGET_H
#define TECHNOHANDSSETTINGWIDGET_H

#include <QWidget>

class StageWidget;

namespace Ui {
class TechnohandsSettingWidget;
}

class TechnohandsSettingWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TechnohandsSettingWidget(QWidget *parent = 0);
    ~TechnohandsSettingWidget();

    void read(const QJsonObject &json);
    void write(QJsonObject &json) const;

private:
    Ui::TechnohandsSettingWidget *ui;

private:
    StageWidget* createTabWithTitle(QString title);

    StageWidget *zAxisSuppliedWidget;
    StageWidget *thetaAxisSuppliedWidget;
    StageWidget *phiAxisWidget;
};

#endif // TECHNOHANDSSETTINGWIDGET_H
