#ifndef SIGMASETITNGWIDGET_H
#define SIGMASETITNGWIDGET_H

#include <QWidget>

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
};

#endif // SIGMASETITNGWIDGET_H
