#include "dialogconfiguration.h"
#include "ui_dialogconfiguration.h"

#include "settings.h"
#include "language.h"

#include <QDebug>
#include <QLineEdit>
#include <QTranslator>
#include <QFileDialog>
#include <QDir>
#include <QFileInfoList>
#include <QFileInfo>
#include <qregularexpression.h>

DialogConfiguration::DialogConfiguration(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogConfiguration)
{
    ui->setupUi(this);
    m_parent=parent;
    Language::setLanguageComboBox(ui->comboBoxInterfaceLanguage);
     QSettings s;
    ui->checkBoxGetJjazzLabShowDir->setChecked(s.value("ShowJazzLabDirectory").toBool());
    ui->checkBoxMMAExportShowDir->setChecked(s.value("ShowMMADirectory").toBool());
    ui->checkBoxMarkDownExportShowDir->setChecked(s.value("ShowMarkDownDirectory").toBool());
    ui->checkBoxAddStartTempo->setChecked(s.value("ClickAtBegin").toBool());
    connect(ui->pushButtonClose,SIGNAL(clicked(bool)),this,SLOT(close()));
    connect(ui->pushButtonSave,SIGNAL(clicked(bool)),this,SLOT(Save()));
    connect(ui->toolButtonPDFReaderName,SIGNAL(clicked(bool)),this,SLOT(SetPDFReader()));
    //connect(ui->checkBoxInternalPDFReader,SIGNAL(stateChanged(int)),this,SLOT(InternalPDFStatus(int)));
    connect(ui->pushButtonSaveFormat,SIGNAL(clicked(bool)),this,SLOT(SaveFormat()));
    connect(ui->pushButtonSaveDefaultFormat,SIGNAL(clicked(bool)),this,SLOT(SaveDefaultFormat()));
    connect(ui->pushButtonLoadFormat,SIGNAL(clicked(bool)),this,SLOT(LoadFormat()));
    connect(ui->toolButtonDestinationDirectoryMarkDown,SIGNAL(clicked(bool)),this,SLOT(setDestinationDirectoryMarkDown()));
    connect(ui->toolButtonDirectiyWithMusicFilesMArkDown,SIGNAL(clicked(bool)),this,SLOT(setDirectoryWithMusicMarkDown()));
    connect(ui->JazzLabToolButton,SIGNAL(clicked(bool)),this,SLOT(setDirectoryJjazzLib()));
    connect(ui->toolButtonMMA,SIGNAL(clicked(bool)),this,SLOT(setDirectoryMMA()));
    connect (ui->checkBoxGetJjazzLabShowDir,SIGNAL(clicked(bool)),this,SLOT(setJjazzLabShowDir()));
    connect (ui->checkBoxMMAExportShowDir,SIGNAL(clicked(bool)),this,SLOT(setMMAShowDir()));
    connect (ui->checkBoxMarkDownExportShowDir,SIGNAL(clicked(bool)),this,SLOT(setDirectoryMarkDown()));
    ui->checkBoxInternalPDFReader->setDisabled(true);
    Connect();
    InitSettings();
    ui->tabWidget->setCurrentIndex(0);
    ui->widgetMemory->setOffSetAsDefault();
    ui->lyricsConfig->setOffButtonSetAsDefault();
    ui->textConfig->setOffButtonSetAsDefault();
    ui->chordConfig->setOffButtonSetAsDefault();
   
    LoadDefaultFormat();
}



void DialogConfiguration::setJjazzLabShowDir()
{
    QSettings s;
    s.setValue("ShowJazzLabDirectory",!s.value("ShowJazzLabDirectory").toBool());
}

bool DialogConfiguration::getJjazzLabShowDir()
{
    QSettings s;
    return s.value("ShowJazzLabDirectory").toBool();
}


void DialogConfiguration::setDirectoryJjazzLib()
{
    QSettings s;
    QString dir= QFileDialog::getExistingDirectory(this,
                                                    tr("Choose the directory where JJazzLib"
                                                       "files are stored"),
                                                    s.value("DestinationDirectoryJazzFiles",QDir::tempPath()).toString());
    ui->lineEditJazzLab->setText(dir);
}


QString DialogConfiguration::getDirectoryJjazzLib()
{
    QSettings s;
    return s.value("DestinationDirectoryJazzFiles",QDir::tempPath()).toString();
}



void DialogConfiguration::setDirectoryMMA()
{
     QSettings s;
    QString dir= QFileDialog::getExistingDirectory(this,
                                                    tr("Choose the directory where MMA "
                                                       "files are stored"),
                                                    s.value("DestinationDirectoryMMAFiles",QDir::tempPath()).toString());
    ui->lineEditMMA->setText(dir);
}

QString DialogConfiguration::getDirectoryMMA()
{
   QSettings s;
   return s.value("DestinationDirectoryMMAFiles",QDir::tempPath()).toString();
}

QString DialogConfiguration::getDirectoryMarkDown()
{
   QSettings s;
   return s.value("DestinationDirectoryMarkdown",QDir::tempPath()).toString();
}


bool DialogConfiguration::getMMAShowDir()
{
    QSettings s;
    return s.value("ShowMMADirectory").toBool();
}

void DialogConfiguration::setMMAShowDir()
{
 QSettings s;
  s.setValue("ShowMMADirectory",!s.value("ShowMMADirectory").toBool());
}



void DialogConfiguration::setDirectoryWithMusicMarkDown()
{
    QSettings s;
    QString dir= QFileDialog::getExistingDirectory(this,
                                                    tr("Choose the directory where music"
                                                       "files are stored"),
                                                    s.value("Destination",QDir::tempPath()).toString());
    ui->lineEditDirectoryWithMusicFilesMarkdown->setText(dir);
}

void DialogConfiguration::setMarkDownShowDir() 
{

QSettings s;
  s.setValue("ShowMarkDownDirectory",!s.value("ShowMarkDonwDirectory").toBool());

}

bool DialogConfiguration::getMarkDownShowDir() { 
QSettings s; 
return s.value("ShowMarkDownDirectory").toBool();

}

void DialogConfiguration::setTranslator(QTranslator *translator)
{
    m_translator=translator;
}

void DialogConfiguration::SaveDefaultFormat()
{
    QSettings s;
    QString defaultfile=ConfigPath()+"/default.fec";
    QSettings fs(defaultfile,QSettings::IniFormat);
    for ( auto children :   ui->scrollArea->findChildren<QLineEdit*>())
    {
     QLineEdit *le=qobject_cast<QLineEdit*>(children);
     QString variable=le->objectName();
     variable.remove(QRegularExpression("^lineEdit"));
     fs.setValue(variable,le->text());
     }
}

void DialogConfiguration::LoadDefaultFormat()
{
    QSettings s;
    QString defaultfile=ConfigPath()+"/default.fec";
    if ( ! QFile(defaultfile).exists())
    {
       QSettings fs(defaultfile,QSettings::IniFormat);
       fs.setValue("cover_title","<H1>$cover_title</H1>");
       fs.setValue("cover_subtitle","<H2>$cover_subtitle:</H2>");
       fs.setValue("title","<H3>$title</H3>");
       fs.setValue("subtitle","<H4>$subtitle</H1>");
       fs.sync();
    }
   QSettings fs(defaultfile,QSettings::IniFormat);
   for ( auto children :   ui->scrollArea->findChildren<QLineEdit*>())
    {
     QLineEdit *le=qobject_cast<QLineEdit*>(children);
     QString variable=le->objectName();
     variable.remove(QRegularExpression("^lineEdit"));
     le->setText(fs.value(variable).toString());
     }

}

void DialogConfiguration::LoadFormat()
{
    QSettings s;
    QString filter=tr("Format Export Chord (*.fec)");
    QString fname=QFileDialog::getOpenFileName(this,tr("Choose file to get format"),s.value("LastOpenedDirectory",
                                      QDir::homePath()).toString(),   filter, &filter);
    QSettings fs(fname,QSettings::IniFormat);
   for ( auto children :   ui->scrollArea->findChildren<QLineEdit*>())
    {
     QLineEdit *le=qobject_cast<QLineEdit*>(children);
     QString variable=le->objectName();
     variable.remove(QRegularExpression("^lineEdit"));
     le->setText(fs.value(variable).toString());
     }

}

void DialogConfiguration::SaveFormat()
{

    QSettings s;
    QString filter=tr("Format Export Chord (*.fec)");

    QString fname=QFileDialog::getSaveFileName(this,tr("Choose file to save format"),s.value("LastOpenedDirectory",
                                      QDir::homePath()).toString(),filter,&filter   );
    if ( ! fname.endsWith(".fec")) fname=fname+".fec";
    QSettings fs(fname,QSettings::IniFormat);
    for ( auto children :   ui->scrollArea->findChildren<QLineEdit*>())
    {
     QLineEdit *le=qobject_cast<QLineEdit*>(children);
     QString variable=le->objectName();
     variable.remove(QRegularExpression("^lineEdit"));
     fs.setValue(variable,le->text());
     }
}


void DialogConfiguration::SetPDFReader()
{
    QString pdfreader=QFileDialog::getOpenFileName(this,tr("Choose PDF Reader program"),"/");
    ui->lineEditPDFReaderName->setText(pdfreader);
    emit PdfReaderChanged();
}

void DialogConfiguration::Connect()
{
    connect(ui->comboBoxInterfaceLanguage,SIGNAL(currentTextChanged(QString)),this,SLOT(SelectLanguage(QString)));

}

DialogConfiguration::~DialogConfiguration()
{
    delete ui;
}

void DialogConfiguration::SelectLanguage(QString  lang)
{
    Settings s;
    s.setValue("InterfaceLanguage",lang);
    QTranslator tr;
    QString filename=Language::getTranslationQmFileName(lang);
    bool ok = tr.load(filename);
    if ( !ok ) qDebug()<<filename<<" not found";
    if ( lang != "English" )
       {
          qApp->installTranslator(&tr);
          m_translator=&tr;
       }
    else
       {
          qApp->removeTranslator(m_translator);
          m_translator=nullptr;
       }
    Retranslate(lang);
    emit LanguageChanged(lang);
}


void DialogConfiguration::Retranslate(QString)
{
    ui->comboBoxInterfaceLanguage->disconnect();
    ui->retranslateUi(this);
    ui->widgetMemory->Retranslate();
    ui->chordConfig->Retranslate();
    ui->lyricsConfig->Retranslate();
    ui->textConfig->Retranslate();
    //ui->comboBoxInterfaceLanguage->setCurrent;
    connect(ui->comboBoxInterfaceLanguage,SIGNAL(currentTextChanged(QString)),this,SLOT(SelectLanguage(QString)));

}

void DialogConfiguration::InitSettings()
{
    Settings s;
    ui->comboBoxInterfaceLanguage->setCurrentText(s.value("InterfaceLanguage","en").toString());
    ui->comboBoxUnit->setCurrentText(s.value("Unit","mm").toString());
    ui->lineEditArtistName->setText(s.value("ArtistName","").toString());
    ui->lineEditLyricistName->setText(s.value("LyricistName","").toString());
    ui->lineEditArrangerName->setText(s.value("ArrangerName","").toString());
    ui->lineEditComposerName->setText(s.value("ComposerName","").toString());
    ui->lineEditPDFReaderName->setText(s.value("PDFReader","").toString());
    ui->checkBoxOpenLastFileOnStart->setChecked(s.value("OpenLastFileOnStart",Qt::Checked).toBool());
    ui->checkBoxSpitInSeparateFilesMarkDown->setChecked(s.value("SplitInSeparateFile",Qt::Checked).toBool());
    ui->lineEditDestinationDirectoryMarkDown->setText(s.value("DestinationDirectoryMarkDown",QDir::tempPath()).toString());
    ui->lineEditJazzLab->setText(s.value("DestinationDirectoryJazzFiles",QDir::tempPath()).toString());
    ui->lineEditMMA->setText(s.value("DestinationDirectoryMMAFiles",QDir::tempPath()).toString());
    ui->lineEditDirectoryWithMusicFilesMarkdown->setText(s.value("DirectoryWithMusicFilesMarkDown","").toString());
    ui->toolButtonChordTextColor->setColor(s.value("ChordTextColor","#55007f").toString());
    ui->toolButtonTagsTextColor->setColor(s.value("TagsTextColor","#005000").toString());
    
    //ui->checkBoxInternalPDFReader->setChecked(s.value("InternalPDFReader",Qt::Checked).toInt());
    //InternalPDFStatus(s.value("InternalPDFReader",Qt::Checked).toInt());
}

QString DialogConfiguration::ConfigPath()
{
    QSettings s;
    QFileInfo fi(s.fileName());
    return fi.absolutePath();
}


void DialogConfiguration::Save()
{
   QSettings s;
   s.setValue("InterfaceLanguage",ui->comboBoxInterfaceLanguage->currentText());
   s.setValue("Unit",ui->comboBoxUnit->currentText());
   s.setValue("ArtistName",ui->lineEditArtistName->text());
   s.setValue("LyricistName",ui->lineEditLyricistName->text());
   s.setValue("ComposerName",ui->lineEditComposerName->text());
   s.setValue("ArrangerName",ui->lineEditArrangerName->text());
   s.setValue("PDFReader",ui->lineEditPDFReaderName->text());
   s.setValue("OpenLastFileOnStart",ui->checkBoxOpenLastFileOnStart->isChecked());
   // s.setValue("InternalPDFReader",ui->checkBoxInternalPDFReader->checkState());
   s.setValue("DestinationDirectoryJazzFiles",ui->lineEditJazzLab->text());
   s.setValue("DestinationDirectoryMarkDown",ui->lineEditDestinationDirectoryMarkDown->text());
   s.setValue("DestinationDirectoryMMAFiles",ui->lineEditMMA->text());
   s.setValue("DirectoryWithMusicFilesMarkDown",ui->lineEditDirectoryWithMusicFilesMarkdown->text());
   s.setValue("SplitInSeparateFile",ui->checkBoxSpitInSeparateFilesMarkDown->isChecked());
   s.setValue("ClickAtBegin",ui->checkBoxAddStartTempo->isChecked());
   //s.setValue("")
   s.setValue("ChordTextColor",ui->toolButtonChordTextColor->getColor().name());
   s.setValue("TagsTextColor",ui->toolButtonTagsTextColor->getColor().name());
   ui->chordConfig->Save(s.fileName(),"Chord");
   ui->textConfig->Save(s.fileName(),"Text");
   ui->lyricsConfig->Save(s.fileName(),"Lyrics");
   ui->widgetMemory->Save();

   close();
}

void DialogConfiguration::InternalPDFStatus(int status)
{
  ui->lineEditPDFReaderName->setEnabled(status!=Qt::Checked);
  ui->toolButtonPDFReaderName->setEnabled(status!=Qt::Checked);
  ui->labelPDFReaderName->setEnabled(status!=Qt::Checked);
}


