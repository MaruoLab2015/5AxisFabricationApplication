#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    // Not necessary??
    if (!w.saveStageSettings(MainWindow::Json))
        return 1;
    if (!w.saveStageSettings(MainWindow::Binary))
        return 1;
    if (!w.loadStageSettings(MainWindow::Json))
        return 1;
    if (!w.loadStageSettings(MainWindow::Binary))
        return 1;

    return a.exec();
}
