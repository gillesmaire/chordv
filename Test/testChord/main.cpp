#include <QCoreApplication>
#include <QFile>
#include <QSettings>
#include <QStringList>
#include <QDebug>
#include <QProcess>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QStringList arguments=a.arguments();
    if ( arguments.count()<2)
    {
        qWarning()<<QString("Usage %1 :  %1 %2").arg(a.applicationName()).arg("name1=value1 name2=value2 ... file.chop");
        exit(-1);
    }
    QString file=arguments.takeLast();
    arguments.pop_front();
    QString outindication=arguments.join("_").replace("/",":");
    QFile::remove("test.chop");
    QFile::copy(file,"test.chop");
    QSettings sf("test.chop",QSettings::IniFormat);
    foreach ( QString arg, arguments)
    {
        QStringList values=arg.split("=");
        QString name=values.first();
        QString value=values.last();
        sf.setValue(name,value);
    }
    if ( outindication.isEmpty()) outindication=file.replace("conf/","").replace(QRegExp("\\.chop$"),"");
    sf.setValue("Memory/OutFile","Memory_"+outindication);
    sf.setValue("Lyrics/OutFile","Lyrics_"+outindication);
    sf.setValue("Text/OutFile","Text_"+outindication);
    sf.setValue("Chord/OutFile","Chord_"+outindication);
    sf.sync();
    QStringList arg;
    arg<<"-t"<<"test.chop";
    QProcess ::execute("../bin/chordV",arg);
}
