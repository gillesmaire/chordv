#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "pdfviewer.h"
#include "chordconfig.h"
#include "dialogchekallchorddefined.h"
#include "dialogchorddefinition.h"
#include <QTranslator>
#include <QToolButton>
#include <QProcess>
#include <QMainWindow>
#include <QMenu>
#include <QLabel>
#include <QLineEdit>
#include <QCheckBox>
#include <QModelIndex>
#include <QPlainTextEdit>

namespace Ui {
class MainWindow;
}

/// \mainpage General documentation for ChordV developpers
/// \section Introduction Introduction
///
/// ChordV allow to edit song booklet with lyrics and text of song, or only chords
/// output, or only Lyrics output
/// All feature are accessible from the MainWindow.
/// All the MainWindow features are accessible from Menu
/// There are many mode :
/// - Edition mode where users can enter text, chords and lyrics
/// - Lyrics mode where users can define the Lyrics mode booklets
/// - Text mode where users can define the Text booklets
/// - Text and Chors mode where users can define the options for the Text and chord booklets
/// In preferences dialog option you can design the general booklet definition and change
/// the behaviour for each booklet from the behaviour defined in option.
///
/// \subsection Convention Convention
///
/// - **language** : is always the name of the language in the language. For example Français or English
/// - **codelang** : is always te code lang for a language. For example fr or en
///
/// \section Edition Edition Mode
///
/// \section Configuration Configuration Mode
///
///



///
/// \brief The MainWindow class
///
class MainWindow : public QMainWindow
{
    Q_OBJECT

    ///
    /// Brief : Main window generated in first by Qt completed by needed tools
    ///
    ///
public:
    ///
    /// \brief MainWindow classical constructor for the main Window
    /// \param parent  the parent window
    ///
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    ///
    /// \brief setTranstator allow to keep the translator applied to
    /// mainwindow to be able to remove it in case of retranslation
    /// \param tr  : translator name
    ///
    void setTranstator(QTranslator *tr) { m_translator= tr;}
private:
    Ui::MainWindow *ui;
    //QString m_currentprojectname;
    //QString m_currentprojectfile;
    //QString m_currentprojectdir;
    QMenu *m_lastmenu;
    QTranslator *m_translator;
    void setMenuLastProject();
    void InitProject();
    ///
    /// \brief openchoFile : open a chord file
    /// \param filename : the name of file to open
    ///
    void openchoFile(QString filename);
    ///
    /// \brief SaveFile : save the current project in a file name
    /// \param filename
    ///
    void SaveFile(QString filename);
    ///
    /// \brief getFileInArg allows  to retrieve the name of file to open if chordV is called
    /// with a filename argument
    /// \return return the first file in arguments of main, no starting with -
    ///
    QString  getFileInArg();
    ///
    /// \brief testMode test if chordV is called with -t or --test argument
    /// \return true if there is -t or --test argument false else
    ///
    bool testMode();
    QString getRelativeFilename(QString chofilename);
    void ActualizeProject(QString file);
    QString m_pdffilename;
    bool m_found;
    void setChecked(int i);
    QString getPdfFileName(QString  text);
    QList <QColor> m_listcolor;
    QString m_initialbuffer;
    bool m_modified;
    void InitDefault();
    void ProducePDF();
    bool m_demofile;


    ///
    /// \brief DisplayInfoFile display info files in status bar
    /// \param filename filename
    ///
    void DisplayInfoFile(QString filename);
    ///
    /// \brief RemoveInfoFile clear InfoFile in status bar
    ///
    void ClearInfoFile();
    void SetMode(int mode);

    DialogChordDefinition  *mDialChordDefinition;
    QList<QString> extractBracketWords(const QString &line);
    QString MakeChordsList(const QStringList &chords);
public slots:
    ///
    /// \brief ChangeLanguage retranslate all the child window and tha main window
    ///
    void ChangeLanguage(QString );

private slots:
    void newProject(bool);
    void openProject(bool);
    void Log(QString message);
    void Save();
    void SaveAs();
    void LastProjectOpen(QAction *action);
    void openProject(QString filename);
    void About();
    void Configuration();
    void openChoFile(QString filename);
    void openChoFile(bool);
    void PreferencesAsOrigine();
    void CurrentAsDefault();
    void Info(QString info);
    void Close();
    void PDFReaderChanged();
    void ConversionDone(QString filename);
    void ChordDefinition();
    void SystemInfo();
    void Search();
    void Replace();
    void Search(QString text, bool casesensitive, bool wordonly);
    void SearchBack(QString text, bool casesensitive, bool wordonly);
    void Replace(QString textfrom, QString textto, bool all, bool casesensitive, bool wordonly, bool back);
    void ShowDefinition();
    void ShowLyricsMode();
    void ShowMemoryMode();
    void ShowChordMode();
    void ShowTextMode();

    void ShowEditor();
    void BuildTextPdf();
    void BuildLyricsPdf();
    void BuildChordPdf();
    void BuildMemoryPdf();
    void BuildJjazzLab();
    void ViewLastBuildPdf();
    void ViewTextPdf();
    void ViewLyricsPdf();
    void ViewChordPdf();
    void ViewMemoryPdf();

    void ExportCho3File();
    void ShowStacked(int i);
    void BuildAndViewText();
    void BuildAndViewMemory();
    void BuildAndViewChord();
    void BuildAndViewLyrics();
    void ZoomIn();
    void ZoomOut();
    void Documentation();
    void TwoLines2Chordpro();
    void ChangeChordLang();
    void AskSaveOnQuit();
    void TransposeChords();
    void LoadDemoFile();
    void SetTocInMemoryMode(QStringList toc);
    void LaunchMemory();
    void SelectSongInMemory(QString song);
    void DisplayError(QString message);
    void SetSelected(QModelIndex index);
    void CheckAllChordUsedAreDefined();
    void CheckDataBase();
    void AskChord(QString chord, QString title);
    void actionReplace_space_chord_space_by_Chord();
    void SuppressChordOnSelection();
    void SortBySongName();
    void SortByAuthorAndSongName();
    void newProjectWithSample();
    void ShowSongInExportedFormat();
    void ShowWholeSongInExportedFormat();
    void SpellControl();
    void Display(QString str);
    void ConvertInMarkDown();
    void ConvertInJazzFiles();
   
public slots:
    void SetSynchroDisplay(bool val);
    void ConvertInMMAFiles();
};

#endif // MAINWINDOW_H
