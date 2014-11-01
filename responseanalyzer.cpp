#include "responseanalyzer.h"
#include <QDebug>

ResponseAnalyzer::ResponseAnalyzer(QObject *parent) :
    QObject(parent)
  ,currentPosition(DBL_MAX)
{
}

void ResponseAnalyzer::parseTechnoHandsResponseText(QString response)
{

    orig = response.trimmed();
    orig = response;
    orig = orig.toLower();

    qDebug() << orig;

    int l = orig.length();
    qDebug() << l;
    int mode = 0; // 0 = search code, 1 = search value
    int c1, p1, p2;
    c1 = p1 = p2 = 0;
    QString code;

    for (int i=0; i<l ; i++)
    {
        QString c = orig.at(i);
        if ( c == QString("<"))
        {
            continue;
        }
        if(mode == 0 && c >= QString("a") && c <= QString("z"))
        {
//            qDebug() << "code" << i;
            c1 = i;
            mode = 1;
            continue;
        }
        else if( mode == 1 && c == QString(" "))
        {
//            mode = 1;
//            qDebug() << "space" << i;

            code = orig.mid(c1, i-1);
            p1 = i+1;
        }
        if ( c == QString("\r"))
        {
//            qDebug() << "cr" << i;

            p2 = i;
            mode = 0;
        }


    }

    addParameter(code, orig.mid(p1, p2-p1));
//    qDebug() << "code : " << code << ", param :" << orig.mid(p1, p2 - p1);
}

void ResponseAnalyzer::addParameter(QString c, QString val)
{
    if ( val == QString("")) return;

    double d = val.toDouble();

    if (c == QString("cp")) this->currentPosition = (double)d;

    qDebug() << this->currentPosition;
}
