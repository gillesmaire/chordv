#include "mainwindow.h"
#include <QApplication>
#include <QSettings>
#include <QDebug>
#include <QTranslator>
#include <QSqlDatabase>
#include <QFileInfo>
#include <QMessageBox>
#include <QDir>
#include <version.h>
#include <language.h>
#include <QMap>
#include <QFontDatabase>
#include <QStandardPaths>
#include <QSqlQuery>
#include <QSqlError>


///
/// \brief main the main program for ChordV
/// \param argc
/// \param argv
/// \return
///
int main(int argc, char *argv[])
{


    QApplication a(argc, argv);
    a.setOrganizationName("ChordV");
    a.setApplicationName("ChordV");
    a.setApplicationVersion(QString("%1").arg(VERSIONWORK));
    QSettings::setDefaultFormat(QSettings::IniFormat);
    Language::InitLanguage();
    // Installation of DataBase include in ressource and chordV-fr file
    // in .config directory
    QSettings s;

    QString filename= s.fileName();
    QFileInfo fi(filename);

    QString databasefile=fi.absolutePath()+"/Chord.db";
    QString newbasefile=fi.absolutePath()+"/ChordNew.db";

    QDir dir(fi.absolutePath());
    if ( ! dir.exists() ) dir.mkpath(fi.absolutePath());
    QFileInfo fidb(databasefile);
    if  (! fidb.exists())
        QFile::copy(":/Base/Bd/Chord.db",databasefile);
    else
        QFile::copy(":/Base/Bd/Chord.db",newbasefile);
    if( ! fidb.isWritable() )
    {
        QFile file(databasefile);
        file.setPermissions(QFileDevice::ReadOwner|QFileDevice::WriteOwner|QFileDevice::ReadGroup| QFileDevice::WriteGroup);
    }
    databasefile=fi.absolutePath()+"/Chord.db";



    QTranslator *tr=nullptr;

    QString lang=s.value("InterfaceLanguage").toString();
    if ( lang != "English" )
    {
        tr =new QTranslator;
        QString qmfile=Language::getTranslationQmFileName(lang);
        bool ok =tr->load(qmfile);
        if ( ! ok) qDebug()<<qmfile<<" not found";
        a.installTranslator(tr);
    }
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");

    QFileInfo fi2(databasefile);
    if ( ! fi2.exists() || fi2.size()==0)
    {
        QMessageBox msgBox;
        msgBox.setText(QObject::tr("Database %1 not found. You will not be able to design chords !").arg(databasefile));
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.exec();
    }
    else
    {
        db.setDatabaseName(databasefile);
        db.open();
        if (  !db.tables().contains("Dico"))
        {
             QSqlQuery q("CREATE TABLE Dico (mot text non null unique);");
             q.exec();
           //  qDebug()<<"error"<<db.lastError().text();
        }
    }
    QFontDatabase::addApplicationFont(":/Font/ELEKTRA_.ttf");
    MainWindow w;
    bool hide=false;
    foreach ( QString arg, qApp->arguments() )
    {
        if (  arg== "-t" || arg =="--test" ) hide =true;
    }
    w.setTranstator(tr);
    if ( hide ) w.hide();
    else w.show();
    return a.exec();
}
