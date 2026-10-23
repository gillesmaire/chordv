#ifndef FORMEDITOR_H
#define FORMEDITOR_H

#include <QWidget>
#include <QTextCursor>
#include <QTextDocument>
#include <QToolButton>
#include <QModelIndex>
#include "dialognewsong.h"
#include "textedit.h"

namespace Ui {
class FormEditor;
}

///
/// \brief The FormEditor class manage
/// - some toolbuttons in a tabWidget
/// - the editor of song in a TextEdit (dérived QTextEdit)
/// - a list of all songs title in the document in QTableWidget
///
class FormEditor : public QWidget
{
    Q_OBJECT

public:
    enum Range { ChordUnderTheCurser, /// Action only on chord under the cursor
                 AllChordInLine, /// Action on all chord in the current line
                 AllChordInSong, /// Action on all chord in song
                 AllChordInFile /// Action on all chord in booklet
               };
    ///
    /// \brief FormEditor is usable aa a promotable widget
    /// \param parent
    ///
    explicit FormEditor(QWidget *parent = 0);
    ///
    /// \brief setChordLanguage set the m_chordlangage
    /// \param language
    ///
    void setChordLanguage(QString language,QString minor);
    ~FormEditor();
    ///
    /// \brief clear remove all the information in the Editor and in the list of all titles
    ///
    void clear();
    ///
    /// \brief append append text to Editor
    ///
    void append(QString);
    ///
    /// return the textCursor of the Editor
    ///
    QTextCursor textCursor();
    ///
    /// \brief setTextCursor set the TextCursor of the Editor
    /// \param textcursor
    ///
    void setTextCursor(QTextCursor textcursor);
    ///
    /// \brief setText set the text of the Editor
    /// \param text the QTextCursor
    ///
    void setText(QString text);
    ////
    /// \brief documen return the QTextDocument of Editor
    /// \return QTextDocument
    ///
    QTextDocument *document();
    ///
    /// \brief copy the Editor
    ///
    void SuppressChordInSelection();
    void copy();
    ///
    /// \brief insertPlainText insert text in QCursor of editor
    /// \param text QString  to insert
    /// \param addcariagereturn bool to force to add a carriage return
    ///
    void insertPlainText(QString text, bool addcariagereturn=true);
    ///
    /// \brief retranslate retranslate the UI
    ///
    void retranslate();
    ///
    /// \brief find find text with the flag
    /// \param text text to find
    /// \param flag of QTextDocument::FindFlags
    /// \return
    ///
    bool find(QString text, QTextDocument::FindFlags flag);
    void SortSongByName();
    void SortSontByAuthorAndName();
    QString getMemorizedText(){ return TextSontMemorized;}
    void setMemorizedTextSong(QString song) { TextSontMemorized=song;}
public slots:
    void zoomIn();
    void zoomOut();
    void AskChord(QString title, QString chord );
private:
    Ui::FormEditor *ui;
    void ReplaceLongShort(QString a, QString b);
    QString m_buffreplace;
    QString m_chordlanguage;
    QString m_minor;
    void resizeEvent(QResizeEvent *event);
    DialogNewSong *m_dialognewsong;
    void Init();
    ///
    /// \brief TransposeLineWithChord transpose all the chord in a line
    /// \param line line of song with 0 to n chords
    /// \param numberofchroma : number of chroma
    /// \return the Line transposed
    QString TransposeLineWithChord(QString line, int numberofchroma);
    QString TextSontMemorized;
    void MemorizeTextSelectedSong();
    QString getChord(TextEdit *edit);
private slots:
    void InsertTempo();
    void InsertTime();
    void InsertT();
    void ToogleLongShort();
    void InsertST();
    void InsertCol();
    void InsertCompress();
    void InsertCS();
    void InsertCT();
    void InsertEOS();
    void InsertSOS();
    void InsertSOC();
    void InsertEOC();
    void InsertCB();
    void GenericInsert(QToolButton *w, QString token, QString label1, QString label2, QString Table=QString());
    void InsertPageBreak();
    void InsertSOV();
    void InsertEOV();
    void InsertAlbum();
    void InsertComposer();
    void InsertArtist();
    void InsertCopyright();
    void InsertYear();
    void InsertKey();
    void InsertDuration();
    void InsertLyricist();
    void InsertArranger();
    void InsertCommentItalic();
    void InsertComment();
    void InsertStyle();
    void InsertSOG();
    void InsertEOG();
    void InsertCommentBox();
    void TocUpdate();
    void TocToText(QModelIndex);
    void InsertNewSong();

    ///
    /// \brief InsertSong Insert so from dialogNewSong
    /// \param title : title of song
    /// \param subtitle : subtitle of song
    /// \param compressed : compressed (true/false)
    /// \param column : number of column ( no output for default 1)
    /// \param time1 : time1 for time1/time2
    /// \param time2 : time2 for time1/time2
    /// \param tempp : beat per minute
    ///
    void InsertSong(QString title, QString subtitle, bool compressed, int column,
                    double tempo, int time1, int time2, DialogNewSong::Insertion i);
    void TransposeChord(int numberofchroma,  int range);
    void InsertVersion();
signals:
    void Toc(QStringList);
    void SongSelected(QString);
    void Copy();
    void Cut();
    void Paste();
};

#endif // FORMEDITOR_H
