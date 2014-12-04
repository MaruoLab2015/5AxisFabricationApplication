#include "gcode.h"
#include "climits"

#include <QDebug>

GCode::GCode(QObject *parent) :
    QObject(parent)
  ,isComment(false)
  ,x(FLT_MAX),y(FLT_MAX),z(FLT_MAX)
  ,e(FLT_MAX),f(FLT_MAX), r(FLT_MAX)
  ,ii(FLT_MAX),jj(FLT_MAX), kk(FLT_MAX)
  ,v(FLT_MAX),s(FLT_MAX)
  ,t(FLT_MAX),p(FLT_MAX)
{
}

void GCode::parse(QString str)
{

    origText = str;
    orig = str.trimmed(); // delete pre and post spaces

    orig = orig.toUpper();// convert string to uppercase

    int l = orig.length();
    int mode = 0; // 0 = search code, 1= search value
    int p1 = 0;
    QString code = ";";

//    qDebug() << l;

    for (int i=0; i<l ; i++)
    {

        QString c = orig.at(i);
        if (mode == 0 && c >= QString("a") && c <= QString("z"))
        {
            c = c.toUpper();
            orig = orig.mid(0,i) + c + orig.mid(i+1);

        }
        if (mode == 0 && c >= QString("A") && c <= QString("Z"))
        {
            code = c;
            mode = 1;
            p1 = i+1;// value of code
            continue;

        }else if (mode == 1)
        {
            if (c == QString(" ") || c == QString('\t') || c == QString(";"))
            {

                addCode(code, orig.mid(p1, i - p1));
                mode = 0;
            }
        }
        if (c == QString(";")) break;

    }
    if (mode == 1)
    {
        addCode(code, orig.mid(p1, orig.length() -p1));
    }


//    qDebug() << orig.toUtf8().data();
}

void GCode::addCode(QString c, QString val)
{
    if ( val == QString("")) return;
    else if (val == QString(":")) return; //Sigma perform comamnd "G:"

    double d = val.toDouble();

         if ( c == QString("X")) x = (float)d;
    else if ( c == QString("Y")) y = (float)d;
    else if ( c == QString("Z")) z = (float)d;
    else if ( c == QString("E")) e = (float)d;
    else if ( c == QString("F")) f = (float)d;
    else if ( c == QString("R")) r = (float)d;
    else if ( c == QString("I")) ii = (float)d;
    else if ( c == QString("J")) jj = (float)d;
    else if ( c == QString("K")) kk = (float)d;
    else if ( c == QString("S")) s = (float)d;
    else if ( c == QString("V")) v = (float)d;
    else if ( c == QString("T")) t = (float)d;
    else if ( c == QString("P")) p = (float)d;
}























