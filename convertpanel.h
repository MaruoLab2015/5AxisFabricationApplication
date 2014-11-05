#ifndef CONVERTPANEL_H
#define CONVERTPANEL_H

#include <QWidget>

namespace Ui {
class ConvertPanel;
}

class ConvertPanel : public QWidget
{
    Q_OBJECT

public:
    explicit ConvertPanel(QWidget *parent = 0);
    ~ConvertPanel();

    QString fabDir, layerPath;

private slots:
    void on_openFolderPathButton_clicked();
    void on_openLayerFolderButton_clicked();

private:
    Ui::ConvertPanel *ui;
};

#endif // CONVERTPANEL_H
