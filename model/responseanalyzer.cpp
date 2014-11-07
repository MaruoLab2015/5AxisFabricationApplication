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
//    qDebug() << l;
    int mode = 0; // 0 = search response, 1 = search code, 2 = search value
    int c1, p1, p2;
    int j; // number of code caracters
    c1 = p1 = p2 = 0;
    QString code;

    for (int i=0; i<l ; i++)
    {
        j++;
        QString c = orig.at(i);
        if ( c == QString("<"))
        {
            mode = 1;
            continue;
        }
        if(mode == 1 && c >= QString("a") && c <= QString("z"))
        {
            c1 = i;
            j = 0;
            mode = 2;
            continue;
        }
        else if( mode == 2 && c == QString(" "))
        {
            code = orig.mid(c1, j);
            p1 = i+1;
        }
        if ( mode == 2 && c == QString("\r"))
        {
            p2 = i;
            mode = 0;

            addParameter(code, orig.mid(p1, p2-p1));
        }
    }
}

void ResponseAnalyzer::addParameter(QString c, QString val)
{
    if ( val == QString("")) return;

    double d = val.toDouble();

    if (c == QString("cp")) this->currentPosition = (double)d;

    qDebug() << "code : " << c << ", param :" << val;
}
