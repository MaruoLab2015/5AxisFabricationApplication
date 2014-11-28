#include "gcodelistdialog.h"
#include "ui_gcodelistdialog.h"

#include <QStandardItemModel>
#include <QDebug>
#include <QFile>
#include <QDir>

GcodeListDialog::GcodeListDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GcodeListDialog)
{
    ui->setupUi(this);

    this->setWindowTitle(tr("Gコード一覧"));

    // read CSV file
    QFile csvFile(":/gcode/gcodeList.csv");
    if( csvFile.open(QIODevice::ReadOnly))
    {
        qDebug() << "open gcodeList.csv";
        QString w_data;
        w_data = csvFile.readAll();
        QStringList line = w_data.split('\r');
        QStringList element = line[0].split(',');
        qDebug() << element.count();

        QStandardItemModel *model = new QStandardItemModel(line.count(), element.count(), this);

        model->setHorizontalHeaderItem(0, new QStandardItem(tr("Gコード")));
        model->setHorizontalHeaderItem(1, new QStandardItem(tr("説明")));
        model->setHorizontalHeaderItem(2, new QStandardItem(tr("例")));

        for (int i=0;i<line.count();i++)
        {
            for (int j=0;j<element.count();j++)
            {
                QStringList e = line[i].split(",");
                model->setItem(i,j,new QStandardItem(e[j]));
            }
        }

        ui->gcodeTableView->setModel(model);
        ui->gcodeTableView->resizeRowsToContents();
        ui->gcodeTableView->resizeColumnsToContents();


    }else
        qDebug() << "couldn't open gcodeList.csv";

    }

GcodeListDialog::~GcodeListDialog()
{
    delete ui;
}
