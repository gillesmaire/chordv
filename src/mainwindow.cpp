#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "dialogabout.h"
#include "dialogconfiguration.h"
#include "dialogchorddefinition.h"
#include "dialogsysteminfo.h"
#include "dialogsearch.h"
#include "dialogreplace.h"
#include "dialogdocumentation.h"
#include "dialogtwolinestochordpro.h"
#include "dialogchangechordname.h"
#include "dialogtranspose.h"
#include "dialogprocessmemory.h"
#include "dialogchekallchorddefined.h"
#include "dialogcheckdatabase.h"
#include "dialogshowexported.h"
#include "dialogcorrection.h"

#include "lyricsconfig.h"
#include "formconfig.h"
#include "textconfig.h"
#include "chord.h"
#include "hunspell/hunspell.hxx"

#include "chordutil.h"
#include "processortext.h"
#include "processorlyrics.h"
#include "processorchord.h"
#include "settings.h"
#include "logmessages.h"
#include "language.h"
#include "pdfviewer.h"


#include <QDebug>
#include <QFileDialog>
#include <QSettings>
#include <QFileInfo>
#include <QTranslator>
#include <QProcess>
#include <QInputDialog>
#include <QShortcut>
#include "ui_formconfig.h"
#include <QMessageBox>
#include <QApplication>
#include <QFontMetrics>
#include <QModelIndex>
#include <QColor>
#include <QRegularExpression>
#include <QDirIterator>
#include <QTemporaryFile>
#include <QFile>
#include <QTextStream>
#include <QMap>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->statusBar->setFormEditor(ui->textEditCho3File);
    mDialChordDefinition= new DialogChordDefinition(this);
    QFont f;
    f.setFamily("Elektra");
    f.setPixelSize(25);
    ui->labelMode->setFont(f);
    ui->statusBar->makeStackedList();
    ui->statusBar->setSongNumberStatusbar(0);
    QSettings s;
    QList <int> size;
    size<<700<<100;
    m_demofile=false;
    m_listcolor<<QColor(Qt::darkCyan)<<QColor(Qt::darkGreen)<<QColor(Qt::darkRed)<<QColor(Qt::darkBlue)<<QColor(Qt::darkGray)<<QColor(Qt::darkMagenta);
    ShowStacked(0);
    ui->splitter->setSizes(size);
    ui->actionSelectEditor->setChecked(true);
    Language::setLanguageComboBox( ui->comboBoxChordLanguage);
    ui->comboBoxMinorNotation->addItems(Language::ListMinor(s.value("InterfaceLanguage").toString()));
    ui->stackedWidget->setCurrentIndex(0);
    m_lastmenu= new QMenu(tr("Last Projects"));
    ui->actionLast_Project=ui->menuFile->insertMenu(ui->actionSave_Current_as_Defaut,m_lastmenu);
    setMenuLastProject();
    connect(m_lastmenu,SIGNAL(triggered(QAction*)),this,SLOT(LastProjectOpen(QAction*)));
    InitProject();
    // Menus
    connect(ui->actionNew_Project,SIGNAL(triggered(bool)),this,SLOT(newProject(bool)));
    connect(ui->actionNew_Project_with_sample,SIGNAL(triggered(bool)),SLOT(newProjectWithSample()));
    connect(ui->actionOpen_Project,SIGNAL(triggered(bool)),this,SLOT(openProject(bool)));
    connect(ui->actionOpen_Song_File,SIGNAL(triggered(bool)),this,SLOT(openChoFile(bool)));
    connect(ui->actionExport_tho_cho3_file,SIGNAL(triggered(bool)),this,SLOT(ExportCho3File()));
    connect(ui->actionSave,SIGNAL(triggered(bool)),this,SLOT(Save()));
    connect(ui->actionSave_As,SIGNAL(triggered(bool)),this,SLOT(SaveAs()));
    connect(ui->actionClose,SIGNAL(triggered(bool)),this,SLOT(Close()));
    connect(ui->actionQuit,SIGNAL(triggered(bool)),this,SLOT(close()));
    connect(ui->actionPreferences,SIGNAL(triggered(bool)),this,SLOT(Configuration()));
    connect(ui->actionChordDefinition,SIGNAL(triggered(bool)),this,SLOT(ChordDefinition()));
    connect(ui->actionReset_Preferences_as_Origin,SIGNAL(triggered(bool)),this,SLOT(PreferencesAsOrigine()));
    connect(ui->actionSave_Current_as_Defaut,SIGNAL(triggered(bool)),this,SLOT(CurrentAsDefault()));
    connect(ui->actionSearch,SIGNAL(triggered(bool)),this,SLOT(Search()));
    connect (ui->actionZoomIn,SIGNAL(triggered(bool)),this,SLOT(ZoomIn()));
    connect (ui->actionZoomOut,SIGNAL(triggered(bool)),this,SLOT(ZoomOut()));
    connect(ui->actionReplace,SIGNAL(triggered(bool)),this,SLOT(Replace()));
    connect (ui->actionSystem_Info,SIGNAL(triggered(bool)),this,SLOT(SystemInfo()));
    connect(ui->actionAbout,SIGNAL(triggered()),this,SLOT(About()));
    connect(ui->action2Line2chordpro,SIGNAL(triggered(bool)),this,SLOT(TwoLines2Chordpro()));
    connect(ui->actionChangeChordLang,SIGNAL(triggered(bool)),this,SLOT(ChangeChordLang()));
    connect(ui->actionReplace_space_chord_space_by_Chord,SIGNAL(triggered(bool)),this,SLOT(actionReplace_space_chord_space_by_Chord()));
    connect(ui->actionTranspose_chords,SIGNAL(triggered(bool)),this,SLOT(TransposeChords()));
    connect (ui->actionSelectDefinition,SIGNAL(triggered(bool)),this,SLOT(ShowDefinition()));
    connect (ui->actionSelectMemory,SIGNAL(triggered(bool)),this,SLOT(ShowMemoryMode()));
    connect (ui->actionSelectText,SIGNAL(triggered(bool)),this,SLOT(ShowTextMode()));
    connect (ui->actionSelectLyrics,SIGNAL(triggered(bool)),this,SLOT(ShowLyricsMode()));
    connect (ui->actionSelectChord,SIGNAL(triggered(bool)),this,SLOT(ShowChordMode()));
    connect(ui->actionSelectEditor,SIGNAL(triggered(bool)),this,SLOT(ShowEditor()));
    connect (ui->actionBuildText,SIGNAL(triggered(bool)),this,SLOT(BuildTextPdf()));
    connect (ui->actionBuildLyrics,SIGNAL(triggered(bool)),this,SLOT(BuildLyricsPdf()));
    connect (ui->actionBuildChord,SIGNAL(triggered(bool)),this,SLOT(BuildChordPdf()));
    connect (ui->actionBuildMemory,SIGNAL(triggered(bool)),this,SLOT(BuildMemoryPdf()));
    connect (ui->actionViewText,SIGNAL(triggered(bool)),this,SLOT(ViewTextPdf()));
    connect (ui->actionViewLyrics,SIGNAL(triggered(bool)),this,SLOT(ViewLyricsPdf()));
    connect (ui->actionViewChord,SIGNAL(triggered(bool)),this,SLOT(ViewChordPdf()));
    connect (ui->actionViewMemory,SIGNAL(triggered(bool)),this,SLOT(ViewMemoryPdf()));
    connect (ui->actionViewLastbuilt,SIGNAL(triggered(bool)),this,SLOT(ViewLastBuildPdf()));
    connect (ui->actionBuildAndViewText,SIGNAL(triggered(bool)),this,SLOT(BuildAndViewText()));
    connect (ui->actionBuildAndViewLyrics,SIGNAL(triggered(bool)),this,SLOT(BuildAndViewLyrics()));
    connect (ui->actionBuildAndViewChord,SIGNAL(triggered(bool)),this,SLOT(BuildAndViewChord()));
    connect (ui->actionBuildAndViewMemory,SIGNAL(triggered(bool)),this,SLOT(BuildAndViewMemory()));
    connect (ui->actionViewLastbuilt,SIGNAL(triggered(bool)),this,SLOT(ViewLastBuildPdf()));
    connect (ui->actionDocumentation,SIGNAL(triggered(bool)),this,SLOT(Documentation()));
    connect (ui->actionAll_chord_used_are_defined,SIGNAL(triggered(bool)),this,SLOT(CheckAllChordUsedAreDefined()));
    connect (ui->actionDataBaseCheck,SIGNAL(triggered(bool)),this,SLOT(CheckDataBase()));
    connect (ui->actionSuppress_chord_on_selection,SIGNAL(triggered(bool)),this,SLOT(SuppressChordOnSelection()));
    connect (ui->actionSortBy_song_name,SIGNAL(triggered()),this,SLOT(SortBySongName()));
    connect (ui->actionSortBy_Author_and_song_name,SIGNAL(triggered()),this,SLOT(SortByAuthorAndSongName()));
    connect (ui->actionExport_the_selected_song_to_formated_format,SIGNAL(triggered()),this,SLOT(ShowSongInExportedFormat()));
    connect (ui->actionExport_to_formated_format,SIGNAL(triggered()),this,SLOT(ShowWholeSongInExportedFormat()));
    connect (ui->actionBuild_a_markdown_File,SIGNAL(triggered()),this,SLOT(ConvertInMarkDown()));
    connect (ui->actionBuild_JazzLab_files,SIGNAL(triggered()),this,SLOT(ConvertInJazzFiles()));
    //connect (ui->actionBuild_MMA_files,SIGNAL(triggered()),this,SLOT(ConvertInMMAFiles()));
    connect(ui->actionBuild_MMA_files, SIGNAL(triggered()),this,SLOT(ConvertInMMAFiles()));
    ///
    connect(ui->stackedWidget,SIGNAL(currentChanged(int)),this,SLOT(ShowStacked(int)));
    connect(qApp,SIGNAL(aboutToQuit()),this,SLOT(AskSaveOnQuit()));
    connect (ui->actionLoad_demo_file,SIGNAL(triggered(bool)),this,SLOT(LoadDemoFile()));
    /// textEditCho3File
    connect( ui->textEditCho3File,SIGNAL(Toc(QStringList)),this,SLOT(SetTocInMemoryMode(QStringList)));
    connect (ui->textEditCho3File,SIGNAL(SongSelected(QString)),this,SLOT(SelectSongInMemory(QString)));
  //  connect (ui->textEditCho3File,SIGNAL(SongSelected(QString)),this,SLOT(SelectSongInMemory(QString)));
  //  connect (ui->textEditCho3File,SIGNAL(SongSelected(QString)),this,SLOT(SelectSongInMemory(QString)));

    connect (ui->pushButtonLaunchMemory,SIGNAL(clicked(bool)),this,SLOT(LaunchMemory()));
    connect (ui->widgetMemory,SIGNAL(SynchroMode(bool)),this,SLOT(SetSynchroDisplay(bool)));

  //  connect (&m_protectStatusBar,SIGNAL(clicked(bool)),&m_selectedsongStatusBar,SLOT(setDisabled(bool)));
    connect (ui->tableWidgetToc,SIGNAL(clicked(QModelIndex)),this,SLOT(SetSelected(QModelIndex)));
    connect (ui->actionCopy,SIGNAL(triggered(bool)),ui->textEditCho3File,SIGNAL(Copy()));
    connect (ui->actionCut,SIGNAL(triggered(bool)),ui->textEditCho3File,SIGNAL(Cut()));
    connect (ui->actionPaste,SIGNAL(triggered(bool)),ui->textEditCho3File,SIGNAL(Paste()));
  //  connect (m_checkwin,SIGNAL(AskChord(QString,QString)),ui->textEditCho3File,SLOT(AskChord(QString,QString)));
    connect (mDialChordDefinition,SIGNAL(Error(QString)),this,SLOT(Log(QString)));
    connect(ui->actionSpelling_check,SIGNAL(triggered(bool)),this,SLOT(SpellControl()));

    ui->tableWidgetToc->setColumnCount(1);
    ui->tableWidgetToc->setColumnWidth(0,65532);
    ui->tableWidgetToc->setSelectionMode(QAbstractItemView::SingleSelection);
    QStringList list;
    list<<tr("Songs list");
    ui->tableWidgetToc->setHorizontalHeaderLabels(list);
    QString file=getFileInArg();
    if ( ! file.isEmpty() )
    {
        QFileInfo fi(file) ;
        if ( ! fi.exists())
        {
            qWarning()<<tr("%1 does'nt exit. Bye !") ;
            exit(1);
        }
        openProject(file);
    }
    else
        if ( ! s.value("LastOpenedFile","").toString().isEmpty() && s.value("OpenLastFileOnStart","").toBool() )
        {
            openProject(s.value("LastOpenedFile","").toString());
        }
    if ( testMode())
    {
        ProducePDF();
        exit(0);
    }
  //  Chord A("La7b","fr");
    ui->widgetMemory->SynchroMode(s.value("Memory/JackSynchro",false).toBool());

}


void MainWindow::AskChord( QString chord,  QString title)
{
    ui->textEditCho3File->AskChord(chord,title);
    ui->statusBar->getCheckWin()->CheckChords();
}


void MainWindow::AskSaveOnQuit()
{
    if ( m_modified )
    {
        int i = QMessageBox::warning(this,tr("Buffer as changed"),tr("Do you want to save ? "),
                          QMessageBox::Yes,QMessageBox::No   );
        if ( i==QMessageBox::Yes)
            Save();

    }
}

void MainWindow::Close()
{
    AskSaveOnQuit();
    ui->textEditCho3File->clear();
    m_initialbuffer.clear();
    InitDefault();
    ClearInfoFile();

}

QString MainWindow::getFileInArg()
{
    QStringList list=qApp->arguments();
    if ( list.empty()) return ("");
    list.removeFirst();
    foreach ( QString arg,list )
    {
        if ( ! arg.startsWith("-")) return arg;
    }
    return  ("");
}

bool MainWindow::testMode()
{
    foreach ( QString arg, qApp->arguments() )
    {
        if (  arg== "-t" || arg =="--test" ) return true;
    }
    return false;
}

void MainWindow::PreferencesAsOrigine()
{
    QSettings s;
    QFile file(s.fileName());

    file.remove();
    InitDefault();
    // TODO restore global elem.

}

void MainWindow::InitDefault()
{
    ui->widgetChordMode->InitDefault("Chord");
    ui->widgetLyricsMode->InitDefault("Lyrics");
    ui->widgetTextMode->InitDefault("Text");
}

void MainWindow::CurrentAsDefault()
{
    QSettings s;
    SaveFile(s.fileName());
}

void MainWindow::openChoFile(bool)
{
   QString file=QFileDialog::getOpenFileName(this,tr("Open text file"),ChordUtil::getLastDirectory(),tr("cho3 file(*.cho3)"));
   ChordUtil::setLastDirectory(file);
   openChoFile(file);

}

void MainWindow::setMenuLastProject()
{
    m_lastmenu->clear();
    foreach ( QString l, ChordUtil::LastProjects())
    {
        QAction *a= new QAction(l,this);
        m_lastmenu->addAction(a);
    }
}

void MainWindow::LastProjectOpen(QAction *action )
{
    openProject(action->text());
}


void MainWindow::Log(QString message)
{
    ui->log->Error(message);
}

void MainWindow::Info(QString info)
{
    ui->log->Info(info);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::newProject( bool)
{
 AskSaveOnQuit();
 InitProject();
}

void MainWindow::newProjectWithSample()
{
    newProject(true);
    QString Song=QString("{title: %1}\n"
    "{columns: 2}\n\n"
    "La[D] la la la [A7]la la la la\n"
    "La[Dx2] la la la [A7]la la la la\n"
    "La[D:2] la[A7-:2] la la [D]la la la la\n"
    "La(V)[D] la la la [A7(V))]la la la la\n\n"
    "{start_of_solo}\n"
    "[D:2]          [A:2]         [D:2]             [A7]\n"
    "{end_of_solo}\n\n"
    "La la la la la la\n"
    "la la la la la la\n").arg(tr("La la song"));
    ui->textEditCho3File->append(Song);
}


void MainWindow::InitProject()
{
    Settings s;
    m_modified=false;
    ui->statusBar->getLabelDirName()->clear();
    ui->statusBar->getLabelFileName()->clear();
    m_initialbuffer.clear();
    ui->textEditCho3File->setChordLanguage(s.value("ChordLang","en").toString(),s.value("ChordMinor","-").toString());
    ui->lineEditCreatorName->setText(s.value("Creator","").toString());
    ui->lineEditWatermark->setText(s.value("Watermark","").toString());
    QString file=getFileInArg();
    ui->statusBar->setFileNameStatusBar(file,false);
    ui->labelNameDirProject->setText(ui->statusBar->getLabelDirName()->text());
    ui->labelNameProjectName->setText(ui->statusBar->getLabelFileName()->text());
    openChoFile(ui->statusBar->getLabelDirName()->text()+"/"+ui->statusBar->getLabelFileName()->text());
}



void MainWindow::openChoFile( QString filename)
{
    QSettings s;
    QFile file(filename);

    ui->textEditCho3File->clear();
    if ( filename.isEmpty()) return;
    if ( filename == ui->statusBar->getLabelDirName()->text()+"/") return;
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) ui->log->Error(QString(tr("Cannot open file : %1").arg(filename)));
    else
    {
        QTextStream in(&file);
        ui->textEditCho3File->append(in.readAll());
        QTextCursor textCursor = ui->textEditCho3File->textCursor();
        textCursor.movePosition(QTextCursor::Start, QTextCursor::MoveAnchor);
        ui->textEditCho3File->setTextCursor(textCursor);
        DisplayInfoFile(filename);
    }
}


void MainWindow::DisplayInfoFile( QString filename)
{
    ui->statusBar->setFileNameStatusBar(filename,m_modified);
}

void MainWindow::ClearInfoFile()
{
  ui->statusBar->getLabelFileName()->clear();
  ui->statusBar->getLabelDirName()->clear();
}

void MainWindow::openProject(QString filename)
{
    QSettings s;
    ui->log->clear();
    ui->statusBar->setFileNameStatusBar(filename,false);
    if (! m_demofile)
    s.setValue("LastOpenedFile",filename);
    QDir::setCurrent(ui->statusBar->getLabelDirName()->text());
    ui->labelNameProjectName->setText(ui->statusBar->getProjectName());
    ui->labelNameDirProject->setText(ui->statusBar->getLabelDirName()->text());
    QSettings p(filename,QSettings::IniFormat);
    ui->lineEditCreatorName->setText(p.value("Creator").toString());
    ui->comboBoxChordLanguage->setCurrentIndex(p.value("ChordLang").toInt());
    ui->comboBoxMinorNotation->setCurrentText(p.value("ChordMinor").toString());

    ui->lineEditWatermark->setText(p.value("Watermark").toString());
    ui->widgetChordMode->SetConfigFromFile(filename);
    ui->widgetChordMode->InitDefault("Chord");
    ui->widgetChordMode->setProjectPath(ui->statusBar->getLabelDirName()->text());
    ui->widgetLyricsMode->SetConfigFromFile(filename);
    ui->widgetLyricsMode->InitDefault("Lyrics");
    ui->widgetLyricsMode->setProjectPath(ui->statusBar->getLabelDirName()->text());
    ui->widgetTextMode->SetConfigFromFile(filename);
    ui->widgetTextMode->InitDefault("Text");
    ui->widgetTextMode->setProjectPath(ui->statusBar->getLabelDirName()->text());
    ui->widgetMemory->Init(filename);
    m_initialbuffer=p.value("Content").toString();
    ui->textEditCho3File->setText(m_initialbuffer);
    //ui->checkBoxLongShort->setChecked(ui->textEditCho3File->document()->toPlainText().contains("{covertitle:",Qt::CaseInsensitive));
    DisplayInfoFile(filename);
}

void MainWindow::openProject ( bool)

{
   QSettings s;
   QString filename=QFileDialog::getOpenFileName(this,tr("Open conf file"),ChordUtil::getLastDirectory(),"*.chop");
   openProject(filename);
   ChordUtil::setLastDirectory(filename);
   ChordUtil::MemorizeProject(filename);
   setMenuLastProject();
}

void MainWindow::ZoomIn()
{
    ui->textEditCho3File->zoomIn();
}

void MainWindow::ZoomOut()
{
    ui->textEditCho3File->zoomOut();
}
void MainWindow::SaveFile(QString filename)
{
    ui->log->clear();
    if ( ! filename.endsWith(".chop") && !filename.endsWith(".conf")) filename+=".chop";
    ui->statusBar->setFileNameStatusBar(filename,false);
    QSettings sf(filename,QSettings::IniFormat);
    sf.clear();
    sf.setValue("Creator",ui->lineEditCreatorName->text());
    sf.setValue("ChordLang",ui->comboBoxChordLanguage->currentIndex());
    sf.setValue("ChordMinor",ui->comboBoxMinorNotation->currentText());
    sf.setValue("Content",ui->textEditCho3File->document()->toPlainText());
    sf.sync();
    ui->widgetChordMode->Save(filename,"Chord");
    ui->widgetLyricsMode->Save(filename,"Lyrics");
    ui->widgetTextMode->Save(filename,"Text");
    ui->widgetMemory->Save(filename);
    m_initialbuffer=ui->textEditCho3File->document()->toPlainText();
    m_modified=false;
    ui->statusBar->setFileNameStatusBar(filename,m_modified);
    m_demofile=false;
}

QString MainWindow::getRelativeFilename( QString chofilename )
{
    QDir dir (ui->statusBar->getLabelDirName()->text());
    QString d=dir.relativeFilePath(chofilename);
    return d;
}

void MainWindow::Save()
{
    ui->log->clear();
    if ( ui->statusBar->getLabelFileName()->text().isEmpty() )
    {
        SaveAs();
        return;
    }
    QFileInfo fi(ui->statusBar->getCompleteFileName());
    if ( !  fi.isWritable())
    {
        SaveAs();
        return ;
    }
    SaveFile(ui->statusBar->getCompleteFileName());
}


void MainWindow::SaveAs()
{
    QString currentprojectfile=QFileDialog::getSaveFileName(this,tr("Save project as"),ChordUtil::getLastDirectory(),tr("(*.chop)"));
    if (currentprojectfile.isEmpty() )
    {
       QMessageBox::warning(this,tr("File is not saved"),tr("You have set an empty file name, the file is not saved !"));
       return;
    }
    if ( !currentprojectfile.endsWith(".chop")) currentprojectfile+=".chop";
    SaveFile(currentprojectfile);

}

void MainWindow::ActualizeProject( QString )
{

}

void MainWindow::ProducePDF()
{
//  if (ui->checkBoxTextMode->isChecked())
//  {
     BuildTextPdf();
//  }
//  if (ui->checkBoxLyricsMode->isChecked())
//  {
     BuildLyricsPdf();
//  }
//  if ( ui->checkBoxChordMode->isChecked())
//  {
      BuildChordPdf();
//  }
//  if ( ui->checkBoxMemoryMode->isChecked())
//  {
      BuildMemoryPdf();
//  }
      BuildJjazzLab();
}





void MainWindow::BuildTextPdf()
{
    try{
    ProcessorText *p;
    p= new ProcessorText(ui,ui->widgetTextMode->getUi());
    connect(p,SIGNAL(PDFMade(QString)),this, SLOT(ConversionDone(QString)));
    connect(p,SIGNAL(Error(QString)),this,SLOT(DisplayError(QString)));
    p->run() ;
    p->deleteLater();
    }
    catch(const PoDoFo::PdfError &e) {
        fprintf(stderr,"Error %i occured!\n",e.GetError());
        e.PrintErrorMsg();
    }

}

void MainWindow::BuildLyricsPdf()
{
    try {
    ProcessorLyrics *p;
    p= new ProcessorLyrics(ui,ui->widgetLyricsMode->getUi());
    connect(p,SIGNAL(PDFMade(QString)),this, SLOT(ConversionDone(QString)));
    connect(p,SIGNAL(Error(QString)),this,SLOT(DisplayError(QString)));
    p->run() ;
    p->deleteLater();
    }
    catch(const PoDoFo::PdfError &e) {
        fprintf(stderr,"Error %i occured!\n",e.GetError());
        e.PrintErrorMsg();
    }

}

void MainWindow::BuildChordPdf()
{
    try {

    ProcessorChord *p;
    p = new ProcessorChord(ui,ui->widgetChordMode->getUi());
    connect(p,SIGNAL(PDFMade(QString)),this, SLOT(ConversionDone(QString)));
    connect(p,SIGNAL(Error(QString)),this,SLOT(DisplayError(QString)));
    p->run();
    p->deleteLater();
    }
    catch(const PoDoFo::PdfError &e) {
        fprintf(stderr,"Error %i occured!\n",e.GetError());
        e.PrintErrorMsg();
    }
}

void MainWindow::BuildMemoryPdf()
{

}

void MainWindow::BuildJjazzLab()
{
    qDebug()<<"JJazzLab";
}   


QString MainWindow::getPdfFileName(QString text )
{
   return  ui->statusBar->getCompleteFileName().replace(QRegularExpression("\\.chop"),QString("_%1.pdf").arg(text));
}

void MainWindow::ViewTextPdf()
{   
    PdfViewer viewer(getPdfFileName(ui->widgetTextMode->getUi()->lineEditOutFile->text()),this);
    if ( ! viewer.getStatus())  Log(viewer.getStatusError());
    else Info(viewer.getStatusInfo());
}


void MainWindow::ViewLyricsPdf()
{
    PdfViewer viewer(getPdfFileName(ui->widgetLyricsMode->getUi()->lineEditOutFile->text()),this);
    if ( ! viewer.getStatus())  Log(viewer.getStatusError());
    else Info(viewer.getStatusInfo());
}

void MainWindow::ViewChordPdf()
{
    PdfViewer viewer(getPdfFileName(ui->widgetChordMode->getUi()->lineEditOutFile->text()),this);
    if ( ! viewer.getStatus())  Log(viewer.getStatusError());
    else Info(viewer.getStatusInfo());
}

void MainWindow::ViewMemoryPdf()
{

}

void MainWindow::ViewLastBuildPdf()
{
    PdfViewer viewer(m_pdffilename,this);
    if ( ! viewer.getStatus())  Log(viewer.getStatusError());
    else Info(viewer.getStatusInfo());
}


void MainWindow::BuildAndViewText()
{
   BuildTextPdf();
   ViewTextPdf();
}

void MainWindow::BuildAndViewLyrics()
{
   BuildLyricsPdf();
   ViewLyricsPdf();
}

void MainWindow::BuildAndViewChord()
{
   BuildChordPdf();
   ViewChordPdf();
}

void MainWindow::BuildAndViewMemory()
{
   BuildMemoryPdf();
   ViewMemoryPdf();
}

void MainWindow::ConversionDone( QString filename)
{
    m_pdffilename=filename;
    Info(tr("Conversion done for : %1").arg(filename));
}


void MainWindow::DisplayError(QString message)
{
    QMessageBox::warning(this,tr("Problem !"),message);
}

void MainWindow::About()
{
  DialogAbout *d = new DialogAbout(this) ;
  d->exec();
}


void MainWindow::PDFReaderChanged()
{
    QSettings s;
   // ui->pushButtonMakeAndShowPDF->setEnabled( ! ui->textEditCho3File->toPlainText().isNull() && ! s.value("PDFReader").isNull());
}

void MainWindow::Configuration()
{
    DialogConfiguration * dial = new DialogConfiguration(this);
    dial->setTranslator(m_translator);
    connect ( dial,SIGNAL(LanguageChanged(QString)),this,SLOT(ChangeLanguage(QString)));
    connect ( dial,SIGNAL(PdfReaderChanged()),this,SLOT(PDFReaderChanged()));
    dial->exec();
    delete dial;
}


void MainWindow::ChordDefinition()
{

    mDialChordDefinition->SetChordLanguage(ui->comboBoxChordLanguage->currentData().toString(),ui->comboBoxMinorNotation->currentText());
    mDialChordDefinition->show();

}

void MainWindow::ChangeLanguage(QString )
{
    ui->retranslateUi(this);
    ui->widgetChordMode->Retranslate();
    ui->widgetLyricsMode->Retranslate();
    ui->widgetTextMode->Retranslate();
    ui->textEditCho3File->retranslate();
    ui->widgetMemory->Retranslate();
    ui->statusBar->Retranslate();
    ui->labelMode->setText(ui->statusBar->getLabelactivestacked()->text());
}



void MainWindow::ExportCho3File()
{
    QString cho3file=ui->statusBar->getCompleteFileName();
    cho3file.replace(QRegularExpression("\\.chop$"),".cho3");
   QFile file( cho3file);
   if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
   {
       ui->log->Error(tr("Cannot write file : %1").arg(cho3file));
       return;
   }
    QTextStream out(&file);
    out << ui->textEditCho3File->document()->toPlainText();
    file.close();
    ui->log->Info(tr("%1 file exported").arg(cho3file));
}



void MainWindow::SystemInfo()
{
    DialogSystemInfo dial(this);
    dial.exec();
}


void MainWindow::Search()
{
  ui->textEditCho3File->copy();
  DialogSearch * dial = new DialogSearch(this);
  connect (dial,SIGNAL(Search(QString,bool,bool)),this,SLOT(Search(QString,bool,bool)));
  connect (dial,SIGNAL(SearchBack(QString,bool,bool)),this,SLOT(SearchBack(QString,bool,bool)));
  dial->show();
}


void MainWindow::Replace()
{
 ui->textEditCho3File->copy();
 DialogReplace *dial = new DialogReplace(this);
 connect (dial,SIGNAL(Search(QString,bool,bool)),this,SLOT(Search(QString,bool,bool)));
 connect (dial,SIGNAL(SearchBack(QString,bool,bool)),this,SLOT(SearchBack(QString,bool,bool)));
 connect (dial,SIGNAL(Replace(QString,QString,bool,bool,bool,bool)),this,SLOT(Replace(QString,QString,bool,bool,bool,bool)));
 dial->show();
}


void MainWindow::Search(QString text, bool casesensitive, bool wordonly )
{
    ui->stackedWidget->setCurrentIndex(0);
    QFlags<QTextDocument::FindFlag> flag;
    if (casesensitive ) flag =QTextDocument::FindCaseSensitively;
    if (wordonly) flag|=QTextDocument::FindWholeWords;
    m_found=ui->textEditCho3File->find(text,flag);
}

void MainWindow::SearchBack(QString text, bool casesensitive, bool wordonly )
{
    ui->stackedWidget->setCurrentIndex(0);
    QFlags<QTextDocument::FindFlag> flag= QTextDocument::FindBackward;
    if (casesensitive ) flag |=QTextDocument::FindCaseSensitively;
    if (wordonly) flag|=QTextDocument::FindWholeWords;
    m_found=ui->textEditCho3File->find(text,flag);
}

void MainWindow::Replace ( QString textfrom, QString textto,bool all, bool casesensitive, bool wordonly, bool back)
{
    if (!m_found ) return;
    ui->stackedWidget->setCurrentIndex(0);
    if ( all )
    {
        while ( m_found )
        {
         ui->textEditCho3File->textCursor().clearSelection();
         ui->textEditCho3File->insertPlainText(textto,false);
         if ( back ) SearchBack(textfrom,casesensitive,wordonly);
         else Search(textfrom,casesensitive,wordonly);
        }
    }
    else
    {
        ui->textEditCho3File->textCursor().clearSelection();
        ui->textEditCho3File->insertPlainText(textto,false);
        if ( back ) SearchBack(textfrom,casesensitive,wordonly);
        else Search(textfrom,casesensitive,wordonly);
    }
}

void MainWindow::ShowEditor()
{
    ui->stackedWidget->setCurrentIndex(0);
    setChecked(0);
}

void MainWindow::ShowDefinition()
{
    ui->stackedWidget->setCurrentIndex(1);
    setChecked(1);
}

void MainWindow::ShowLyricsMode()
{
    ui->stackedWidget->setCurrentIndex(2);
    setChecked(2);

}

void MainWindow::ShowMemoryMode()
{
    ui->stackedWidget->setCurrentIndex(5);
    setChecked(5);
}

void MainWindow::ShowChordMode()
{
    ui->stackedWidget->setCurrentIndex(3);
    setChecked(3);
}

void MainWindow::ShowTextMode()
{
    ui->stackedWidget->setCurrentIndex(4);
    setChecked(4);
}

void MainWindow::setChecked(int i)
{
    ui->actionSelectChord->setChecked(i==3);
    ui->actionSelectDefinition->setChecked(i==1);
    ui->actionSelectEditor->setChecked(i==0);
    ui->actionSelectLyrics->setChecked(i==2);
    ui->actionSelectMemory->setChecked(i==5);
    ui->actionSelectText->setChecked(i==4);
}


void MainWindow::ShowStacked( int i )
{
    ui->statusBar->makeStackedList();
    ui->statusBar->setActiveStacked(i);
    SetMode(i);
}

void MainWindow::Documentation()
{
    DialogDocumentation *dial= new DialogDocumentation(this);
    dial->show();

}

void MainWindow::TwoLines2Chordpro()
{
  DialogTwoLinesToChordPro dial(this);
  dial.exec();
}

///
/// \brief MainWindow::ChangeChordLang change the chord lang for the current Edited file
///
void MainWindow::ChangeChordLang()
{
 DialogChangeChordName dial(this);
 dial.setFromLang(ui->comboBoxChordLanguage->currentIndex());
 dial.setFromMin(ui->comboBoxMinorNotation->currentIndex());
 dial.setToLang(ui->comboBoxChordLanguage->currentIndex());
 dial.setToMin(ui->comboBoxMinorNotation->currentIndex());
 dial.exec();
 QStringList out;
 if (dial.OkDialog())
 {
   QStringList lines=ui->textEditCho3File->document()->toPlainText().split("\n");
   foreach (QString line , lines)
      {
       if (! line.isEmpty())
       line=Language::TranslateLineWithchord(line,dial.getFromLang(),dial.getFromMin(),dial.getToLang(),dial.getToMin());
       out<<line;
      }
   }
 ui->textEditCho3File->document()->clear();
 ui->textEditCho3File->document()->setPlainText(out.join("\n"));
 ui->comboBoxChordLanguage->setCurrentIndex(dial.getToLangIndex());
 ui->comboBoxMinorNotation->setCurrentIndex(dial.getToMinIndex());
}


void MainWindow::TransposeChords()
{
   DialogTranspose dial(this);
   ui->textEditCho3File->setChordLanguage(ui->comboBoxChordLanguage->currentData().toString(),
                                          ui->comboBoxMinorNotation->currentText());
   connect(&dial,SIGNAL(AskTranspose(int,int)),ui->textEditCho3File,SLOT(TransposeChord(int,int)));
   dial.exec();
}

void MainWindow::LoadDemoFile()
{
    QSettings s;
    QString language=s.value("InterfaceLanguage").toString();
    if (language.isEmpty()) language="English";

    QString filename=Language::getTranslationDemoFileName(language);
    openProject(filename);
    m_demofile=true;
}

void MainWindow::SetTocInMemoryMode(QStringList toc)
{
    ui->tableWidgetToc->clear();
    ui->tableWidgetToc->setRowCount(toc.count());
    int i=0;
    for ( auto elem : toc)
    {
        ui->tableWidgetToc->setItem(i,0, new QTableWidgetItem(elem.trimmed()));
        i++;
    }
    ui->statusBar->setSongNumberStatusbar(i);
    m_modified = m_initialbuffer != ui->textEditCho3File->document()->toPlainText();
    ui->statusBar->setModified(m_modified);

}


void MainWindow::LaunchMemory()
{

    if ( ui->statusBar->getSelectedSongStatusBar()->text().isEmpty()
         &&  ui->tableWidgetToc->selectedItems().count()==0)
    {
        QMessageBox::warning(this,tr("No title selected"),tr("You must select a title"));
        return;
    }
    QString title;
    if ( !  ui->statusBar->getSelectedSongStatusBar()->text().isEmpty())
      title=ui->statusBar->getSelectedSongStatusBar()->text();
    else
      title = ui->tableWidgetToc->selectedItems().at(0)->text();
    this->hide();
    DialogProcessMemory memory(this,
                                                          ui->textEditCho3File->document()->toPlainText(),
                                                          ui->widgetMemory->getPosition(),
                                                          title,
                                                          ui->widgetMemory->getShowRythm(),
                                                          ui->widgetMemory->getClick(),
                                                          ui->widgetMemory->getVolume(),
                                                          ui->widgetMemory->getAccentuedFirst(),
                                                          ui->widgetMemory->getFont(),
                                                          ui->widgetMemory->getTextColor(),
                                                          ui->widgetMemory->getBackgroundColor(),
                                                          ui->widgetMemory->getFullscreenMode(),
                                                          ui->widgetMemory->getShowTwoLines(),
                                                          ui->widgetMemory->getAdvance(),
                                                          ui->widgetMemory->getDelay(),
                                                          ui->widgetMemory->getJackSynchro(),
                                                          ui->widgetMemory->getTimeBeforeUnit()
                                                          );

    memory.exec();
    ui->widgetMemory->setJackSynchro(memory.getJackConnected());
    this->show();
}


void MainWindow::SetSynchroDisplay(bool val)
{
    if (val)
        ui->labelESCAPEPAUSE->setText(tr("< ESCAPE >  to abort monitoring window"));
    else
        ui->labelESCAPEPAUSE->setText(tr("< ESCAPE >  to abort monitoring window\n< PAUSE> to pause unpause"));
}


void MainWindow::SelectSongInMemory(QString song)
{
   ui->tableWidgetToc->selectionModel()->clearSelection();
     QList<QTableWidgetItem*> list=ui->tableWidgetToc->findItems(song,Qt::MatchExactly);
     if ( !list.empty() )
     {
         list.at(0)->setSelected(true);
     }
     ui->statusBar->setSelectedSongStatusBar(song);
}

void MainWindow::SetSelected(QModelIndex index)
{

    ui->statusBar->setSelectedSongStatusBar(index.data().toString());
}


void MainWindow::SetMode ( int mode)
{
    ui->statusBar->setActiveStacked(mode);
    ui->labelMode->setText(ui->statusBar->getLabelactivestacked()->text());
    if (mode != 6 )ui->labelMode->setStyleSheet(QString("color:%1").arg(m_listcolor.at(mode).name()));
}



void MainWindow::CheckAllChordUsedAreDefined()
{
      ui->statusBar->getCheckWin()->show();
      ui->statusBar->getCheckWin()->CheckChords();
}

void MainWindow::CheckDataBase()
{
   DialogCheckDataBase d(this);
   d.exec();
}


void MainWindow::actionReplace_space_chord_space_by_Chord()
{
    QStringList out;
    QStringList lines=ui->textEditCho3File->document()->toPlainText().split("\n");
      for (auto line : lines)
         {
          QStringList words=line.split(QRegularExpression("[ \t]"));
          QStringList l;
          for ( auto  word : words)
          {
              if (Chord::isChord(word))
              {
                  word=QString("[%1]").arg(word);
              }
              l<<word;
          }
          QString cl=l.join(" ");
          cl.replace(" [","[");
          cl.replace("] ","]");
          out <<cl;
         }
    ui->textEditCho3File->document()->clear();
    ui->textEditCho3File->document()->setPlainText(out.join("\n"));
}

void MainWindow::SuppressChordOnSelection()
{
    ui->textEditCho3File->SuppressChordInSelection();
}


void MainWindow::SortBySongName()
{
    ui->textEditCho3File->SortSongByName();
}

void MainWindow::SortByAuthorAndSongName()
{
   ui->textEditCho3File->SortSontByAuthorAndName();
}

void MainWindow::ShowSongInExportedFormat()
{
 DialogShowExported d(this);
 d.setWindowTitle(tr("Selected song formated"));
 d.setText(ui->textEditCho3File->getMemorizedText());
  d.exec();
}

void MainWindow::ShowWholeSongInExportedFormat()
{
 DialogShowExported d(this);
 d.setWindowTitle(tr("Whole song formated"));
 ui->textEditCho3File->setMemorizedTextSong(ui->textEditCho3File->document()->toPlainText());
 d.setText(ui->textEditCho3File->getMemorizedText());
 d.exec();
}

void MainWindow::SpellControl()
{
     ui->stackedWidget->setCurrentIndex(0);
     QString lang;
     QSettings s;
     lang=s.value("InterfaceLanguage").toString();
     QString lang2=lang.mid(0,2).toUpper();
     QString lang1=lang.mid(0,2).toLower();
     QString hunspell1=QString("/usr/share/hunspell/%1_%2.aff").arg(lang1).arg(lang2);
     QString hunspell2=QString("/usr/share/hunspell/%1_%2.dic").arg(lang1).arg(lang2);

#ifdef __linux__
     Hunspell *hsp = new Hunspell(hunspell1.toLatin1(),hunspell2.toLatin1());
#endif
     DialogCorrection d(this);
     connect(&d,SIGNAL(Search(QString)),this,SLOT(Display(QString)));
     QString text=ui->textEditCho3File->document()->toPlainText().replace(QRegularExpression("\\[[^]]+\\]"),"").replace(QRegularExpression("\\{[^}]+\\}"),"");
     QStringList list=text.split(QRegularExpression("[^a-zA-ZÀÄÂàäâçÇEÉÈÔÖéèëêïîöôÜÛüûù]"));
     QStringList phrases=text.split("\n");
     phrases.removeAll("");
     phrases.removeAll(" ");
     QStringList bads;
     bool find=false;
     foreach (  QString word  , list)
     {
         if ( ! word.isEmpty())
         {
             if ( ! hsp->spell(word.toStdString()))
             {
               if ( ! DialogCorrection::AuthorizedWord(word) )
                  {
                  if ( !bads.contains(word) ) bads<<word;

                  foreach ( QString ww , phrases)
                     {
                        QString exp=QString("([,;.'- ]+|^)%1([,;.'- ]+|$)").arg(word);
                        if ( ww.contains(QRegularExpression(exp)) )
                            {
                               d.setSentence(ww);
                               find=true;
                               break;
                            }

                     }
                    if (find) break;
                  }
             }
          }
     }
     d.setBadWord(bads);

     d.exec();
}

void MainWindow::Display(QString str)
{
    ui->stackedWidget->setCurrentIndex(0);
    QFlags<QTextDocument::FindFlag> flag=QTextDocument::FindCaseSensitively|QTextDocument::FindWholeWords;
    ui->textEditCho3File->find(str,flag);
}


#include "MarkDownConvertion.cpp"
#include "JazzLabConvertion.cpp"

#include "MMAConvertion.cpp"
