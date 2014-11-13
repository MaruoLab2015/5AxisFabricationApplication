#ifndef CONVERTPANEL_H
#define CONVERTPANEL_H

#include <QWidget>

class GCode;

namespace Ui {
class ConvertPanel;
}

class ConvertPanel : public QWidget
{
    Q_OBJECT

public:
    explicit ConvertPanel(QWidget *parent = 0);
    ~ConvertPanel();

signals:
    void sendGcodeText(QList<GCode*> gcodeList);

private slots:
    void on_openFolderPathButton_clicked();
    void on_openLayerFolderButton_clicked();
    void on_convertPushButton_clicked();

    QString convertCSVToGCodeXY(QString line);
    QString convertCSVToGCodeZ(float layerPitch);

private:
    Ui::ConvertPanel *ui;

    int startLayer, endLayer;
};

#endif // CONVERTPANEL_H
