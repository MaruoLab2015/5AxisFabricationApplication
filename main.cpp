#include "mainwindow.h"
#include <QApplication>
#include <QPointer>
#include <QDebug>

QPointer<MainWindow> window;
//MainWindow w;

void myMessageOutput(QtMsgType type,
                     const QMessageLogContext &context,
                     const QString &msg)
{
    if(window)
       window->outputMessage(type, msg);
}


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    window = &w;

    w.show();

    qInstallMessageHandler(myMessageOutput);
    qDebug() << "Running Application";

    return a.exec();
}
