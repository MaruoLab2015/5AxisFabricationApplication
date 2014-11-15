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

private:
    Ui::TechnohandsSettingWidget *ui;

private:
    StageWidget* createTabWithTitle(QString title);
};

#endif // TECHNOHANDSSETTINGWIDGET_H
