#include "language.h"
#include "chord.h"
#include "constants.h"
#include <QSettings>
#include <QFileInfo>
#include <QDir>
#include <QComboBox>
#include <QFile>
#include <QDebug>
#include <QMap>
#include <QStringList>

static QMap <QString, QString > Code2Language;
static QMap <QString, QString > Language2Code;


void Language::InitLanguage()
{
    for (auto fi : QDir(Constants::AppDataLocationRoot()).entryInfoList(QDir::Dirs|QDir::NoDotAndDotDot))
    {
        QString lang=fi.fileName();
        QFile  file(Constants::AppDataLocation(lang)+"/code.txt");
        file.open(QIODevice::ReadOnly|QIODevice::Text);
        QString code=file.readLine();
        file.close();
        Code2Language[code]=lang;
        Language2Code[lang]=code;
    }
}

QString Language::getTranslationQmFileName(QString lang)
{
    return QString("%1/lang.qm").arg(Constants::AppDataLocation(lang));
}


QString Language::getTranslationDocFileName(QString lang)
{
    return QString("%1/doc.html").arg(Constants::AppDataLocation(lang));
}

QString Language::getTranslationDemoFileName(QString lang)
{
    return QString("%1/demo.chop").arg(Constants::AppDataLocation(lang));
}

QString Language::getFlagName(QString lang)
{
    return QString("%1/flag.png").arg(Constants::AppDataLocation(lang));
}

void Language::setLanguageComboBox(QComboBox *ptr)
{
    ptr->clear();
    for (auto fi :QDir(Constants::AppDataLocationRoot()).entryInfoList(QDir::AllDirs|QDir::NoDotAndDotDot ))
    {
        QString lang=fi.fileName();
        ptr->addItem(QIcon(getFlagName(lang)),lang,Language2Code[lang]);

    }

}

QStringList Language::ListNotes(QString lang)
{
    if ( lang.isEmpty() ) lang="English";
    QString filename=QString("%1/chords.txt").arg(Constants::AppDataLocation(lang));
    QFile file(filename);
    file.open(QIODevice::ReadOnly|QIODevice::Text);
    QString chords=file.readLine();
    chords.chop(1);
    file.close();
    return chords.split(",");
}

QStringList Language::ListMinor(QString lang)
{
    if ( lang.isEmpty() ) lang="English";
    QString filename=QString("%1/chords.txt").arg(Constants::AppDataLocation(lang));
    QFile file(filename);
    file.open(QIODevice::ReadOnly|QIODevice::Text);
    QString minor=file.readLine();
    minor=file.readLine();
    minor.chop(1);
    file.close();
    return minor.split(",");
}

QString Language::getCodeLang(QString language)
{
    return Language2Code[language];
}

QString Language::getLanguage(QString codelang)
{
    return Code2Language[codelang];
}


QString Language::TranslateLineWithchord(QString line,QString fromlang, QString frommin, QString tolang, QString tomin)
{
   bool accolade=false;
   if (line.isEmpty()) return QString();
   QRegularExpression regchord("(\\[[^]]+\\])");
   QRegularExpressionMatch match;
   int pos=-1;
   while ( (pos=line.indexOf(regchord,pos+1,&match)) !=-1 )
     {
          accolade=true;
          QString newchord=match.captured(1).replace("[","").replace("]","");
          
          newchord=Chord::translate(newchord,Language::getCodeLang(fromlang),frommin,Language::getCodeLang(tolang),tomin);
          line.replace(match.captured(1),QString("{%1}").arg(newchord));
 
     }
     if ( accolade)
     {
          line.replace("{","[").replace("}","]");
     }
     return line;
}

QStringList Language::ListLanguage()
{
   return Language2Code.keys();
}

QStringList Language::ListCodeLang()
{

    return Code2Language.keys();
}



