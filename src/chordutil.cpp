#include "chordutil.h"
#include <QSettings>
#include <QFileInfo>
#include <QDir>
#include <QDebug>


QStringList ChordUtil::LastProjects()
{
    QStringList l;
    QSettings s;
    for ( int i=0; i<10; i++)
    {   QString val=s.value(QString("LastProjects/f%1").arg(i+1)).toString();
        if (val.isEmpty()) break;
        l <<val;
    }
   return l;
}

void ChordUtil::MemorizeProject(QString filename)
{
    if (filename.isEmpty()) return;
    QSettings s;
    s.beginGroup("LastProjects");
    QStringList olds;
    foreach ( QString l, s.allKeys())
      {
       if ( s.value(QString("%1").arg(l)).toString()==filename) return ;
       olds<<s.value(QString("%1").arg(l)).toString();
      }
    s.endGroup();
    s.setValue("LastProjects/f1",filename);
    int i=2;
    foreach ( QString old, olds)
    {
        s.setValue(QString("LastProjects/f%1").arg(i),old);
        if ( i==10) return;
        i++;
    }
}


QString  ChordUtil::getLastDirectory()
{
    QSettings s;
    return(s.value("LastOpenedDirectory").toString());
}

void ChordUtil::setLastDirectory(QString filename)
{
    if (filename.isEmpty()) return;
    QSettings s;
    QFileInfo fi(filename);
    QString  dir=fi.absoluteDir().absolutePath();
    s.setValue("LastOpenedDirectory",dir);
}

QString ChordUtil::toRomain(int i)
{
    if ( i == 1 ) return "I";
    if ( i == 2 ) return "II";
    if ( i == 3 ) return "III";
    if ( i == 4 ) return "IV";
    if ( i == 5 ) return "V";
    if ( i == 6 ) return "VI";
    if ( i == 7 ) return "VII";
    if ( i == 8 ) return "VIII";
    if ( i == 9 ) return "IX";
    if ( i == 10 ) return "X";
    if ( i == 11 ) return "XI";
    if ( i == 12 ) return "XII";
    if ( i == 13 ) return "XIII";
    if ( i == 14 ) return "XIV";
    if ( i == 15 ) return "XV";
    if ( i == 16 ) return "XVI";
    if ( i == 17 ) return "XVII";
    if ( i == 18 ) return "XVIII";
    if ( i == 19 ) return "XIX";
    if ( i == 20 ) return "XX";
    return "";
}


int ChordUtil::fromRomain (QString i)
{

    if ( i == "I" ) return 1;
    if ( i == "II" ) return 2;
    if ( i == "III" ) return 3;
    if ( i == "IV" ) return 4;
    if ( i == "V" ) return 5;
    if ( i == "VI" ) return 6;
    if ( i == "VII" ) return 7;
    if ( i == "VIII" ) return 8;
    if ( i == "IX" ) return 9;
    if ( i == "X" ) return 10;
    if ( i == "XI") return 11;
    if ( i == "XII" ) return 12;
    if ( i == "XIII" ) return 13;
    if ( i == "XIV" ) return 14;
    if ( i == "XV" ) return 15;
    if ( i == "XVI") return 16;
    if ( i == "XVII" ) return 17;
    if ( i == "XVIII" ) return 18;
    if ( i == "XIX" ) return 19;
    if ( i == "XX" ) return 20;
    return 0;
}

QStringList ChordUtil::Normalize(QStringList)
{
 return QStringList();
}

