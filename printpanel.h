#ifndef PRINTPANEL_H
#define PRINTPANEL_H

#include <QWidget>

namespace Ui {
class PrintPanel;
}

class PrintPanel : public QWidget
{
    Q_OBJECT

public:
    explicit PrintPanel(QWidget *parent = 0);
    ~PrintPanel();

signals:
    void sendLineEditText(QString);
private slots:

    void on_sendRequestButton_clicked();

private:
    Ui::PrintPanel *ui;
};

#endif // PRINTPANEL_H
