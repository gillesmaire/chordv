#include "formeditor.h"
#include "ui_formeditor.h"

#include "dialogbar.h"
#include "language.h"
#include "chord.h"


#include <QInputDialog>
#include <QDebug>
#include <QHeaderView>
#include <QTextCursor>
#include <QScrollBar>
#include <dialoginputdialog.h>
#include <QSqlQuery>
#include <QSqlError>

FormEditor::FormEditor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormEditor)
{
    ui->setupUi(this);

    Init();
    connect(ui->toolButtonCB,SIGNAL(clicked(bool)),this,SLOT(InsertCB()));
    connect(ui->toolButtonCol,SIGNAL(clicked(bool)),this,SLOT(InsertCol()));
    connect(ui->toolButtonTempo,SIGNAL(clicked(bool)),this,SLOT(InsertTempo()));
    connect(ui->toolButtonTime,SIGNAL(clicked(bool)),this,SLOT(InsertTime()));
    connect(ui->toolButtonCS,SIGNAL(clicked(bool)),this,SLOT(InsertCS()));
    connect(ui->toolButtonCT,SIGNAL(clicked(bool)),this,SLOT(InsertCT()));
    connect(ui->toolButtonSOS,SIGNAL(clicked(bool)),this,SLOT(InsertSOS()));
    connect(ui->toolButtonEOS,SIGNAL(clicked(bool)),this,SLOT(InsertEOS()));
    connect(ui->toolButtonSOC,SIGNAL(clicked(bool)),this,SLOT(InsertSOC()));
    connect(ui->toolButtonEOC,SIGNAL(clicked(bool)),this,SLOT(InsertEOC()));
    connect(ui->toolButtonSOV,SIGNAL(clicked(bool)),this,SLOT(InsertSOV()));
    connect(ui->toolButtonEOV,SIGNAL(clicked(bool)),this,SLOT(InsertEOV()));
    connect(ui->toolButtonSOG,SIGNAL(clicked(bool)),this,SLOT(InsertSOG()));
    connect(ui->toolButtonEOG,SIGNAL(clicked(bool)),this,SLOT(InsertEOG()));
    connect(ui->toolButtonAlbum,SIGNAL(clicked(bool)),this,SLOT(InsertAlbum()));
    connect(ui->toolButtonComposer,SIGNAL(clicked(bool)),this,SLOT(InsertComposer()));
    connect(ui->toolButtonArtist,SIGNAL(clicked(bool)),this,SLOT(InsertArtist()));
    connect(ui->toolButtonCopyright,SIGNAL(clicked(bool)),this,SLOT(InsertCopyright()));
    connect(ui->toolButtonYear,SIGNAL(clicked(bool)),this,SLOT(InsertYear()));
    connect(ui->toolButtonKey,SIGNAL(clicked(bool)),this,SLOT(InsertKey()));
    connect(ui->toolButtonDuration,SIGNAL(clicked(bool)),this,SLOT(InsertDuration()));
    connect(ui->toolButtonLyricist,SIGNAL(clicked(bool)),this,SLOT(InsertLyricist()));
    connect(ui->toolButtonArranger,SIGNAL(clicked(bool)),this,SLOT(InsertArranger()));
    connect(ui->toolButtonST,SIGNAL(clicked(bool)),this,SLOT(InsertST()));
    connect(ui->toolButtonT,SIGNAL(clicked(bool)),this,SLOT(InsertT()));
    connect(ui->toolButtonVERSION,SIGNAL(clicked(bool)),this,SLOT(InsertVersion()));
    connect(ui->toolButtonPageBreak,SIGNAL(clicked(bool)),this,SLOT(InsertPageBreak()));
    connect(ui->toolButtonComment,SIGNAL(clicked(bool)),this,SLOT(InsertComment()));
    connect(ui->toolButtonStyle,SIGNAL(clicked(bool)),this,SLOT(InsertStyle()));
    connect(ui->toolButtonCommentItalic,SIGNAL(clicked(bool)),this,SLOT(InsertCommentItalic()));
    connect(ui->toolButtonCommentBox,SIGNAL(clicked(bool)),this,SLOT(InsertCommentBox()));
    connect(ui->toolButtonInsertNewSong,SIGNAL(clicked(bool)),this,SLOT(InsertNewSong()));
    connect(ui->checkBoxLongShort,SIGNAL(clicked(bool)),this,SLOT(ToogleLongShort()));
    connect(ui->textEdit,SIGNAL(textChanged()),this,SLOT(TocUpdate()));
    connect(ui->tableWidgetToc,SIGNAL(clicked(QModelIndex)),this,SLOT(TocToText(QModelIndex)));
    connect (m_dialognewsong,SIGNAL(InsertSong(QString,QString,bool,int,double,int,int, DialogNewSong::Insertion)),
             this,SLOT(InsertSong(QString,QString,bool,int,double,int,int,DialogNewSong::Insertion)));
    connect (this,SIGNAL(Copy()),ui->textEdit,SLOT(copy()));
    connect (this,SIGNAL(Cut()),ui->textEdit,SLOT(cut()));
    connect (this,SIGNAL(Paste()),ui->textEdit,SLOT(paste()));

}

void FormEditor::setChordLanguage(QString language, QString minor)
{
    m_chordlanguage=language;
    m_minor=minor;
    ui->textEdit->setChordLanguage(language,minor);
}


void FormEditor::Init()
{
    ui->splitterHorizontal->setStretchFactor(0,3);
    ui->splitterHorizontal->setStretchFactor(1,1);
    ui->tableWidgetToc->setColumnCount(1);
    QStringList list;
    list<<tr("Songs list");
    ui->tableWidgetToc->setHorizontalHeaderLabels(list);
    ui->tableWidgetToc->horizontalHeader()->setStretchLastSection(true);
    m_dialognewsong= new DialogNewSong(this);
}


void FormEditor::clear()
{
    ui->textEdit->clear();
}

void FormEditor::append(QString value)
{
    ui->textEdit->append(value );
}

QTextCursor FormEditor::textCursor()
{
    return ui->textEdit->textCursor();
}

void FormEditor::setTextCursor (QTextCursor textcursor)
{
    ui->textEdit->setTextCursor(textcursor);
}

void FormEditor::setText(QString text)

{
    ui->textEdit->setText(text);
    ui->checkBoxLongShort->setChecked(ui->textEdit->document()->toPlainText().contains("{title:",Qt::CaseInsensitive));
}

QTextDocument *FormEditor::document()
{
    return ui->textEdit->document();
}

void FormEditor::SuppressChordInSelection()
{
    ui->textEdit->SuppressChordInSelectedText();
}

void FormEditor::copy()
{
    ui->textEdit->copy();
}


void FormEditor::insertPlainText(QString text, bool addcarriagereturn)
{
    ui->textEdit->insertPlainText(text,addcarriagereturn);
}

void FormEditor::retranslate()
{
    ui->retranslateUi(this);
}

void FormEditor::zoomIn()
{
    ui->textEdit->zoomIn();
}

void FormEditor::zoomOut()
{
    ui->textEdit->zoomOut();
}

bool FormEditor::find(QString text, QTextDocument::FindFlags flag)
{
    return ui->textEdit->find( text,flag);
}

void FormEditor::SortSongByName()
{
   QStringList head;
   QMap< QString,QStringList > doc;
   QString key="___" ;
   for ( auto line : ui->textEdit->document()->toPlainText().split("\n") )
   {
       QRegularExpression reg;
       if ( ui->checkBoxLongShort->isChecked() )
           reg=QRegularExpression("^\\{ *title: *([^}]+)\\}$");
       else
           reg=QRegularExpression("^\\{ *t: *([^}]+)\\}$");
      if (line.contains(reg)) key=line;
      else doc[key]<<line;
   }
   ui->textEdit->clear();
   ui->textEdit->append(doc["___"].join("\n"));
   doc.remove("___");
   QStringList sortedList =doc.keys();
   sortedList.sort(Qt::CaseInsensitive);
   for ( auto key : sortedList)
   {
      // if ( doc[key].at(0)!="{new_Song}")  ui->textEdit->append("{new_Song}\n");
       ui->textEdit->append(key);
       ui->textEdit->append(doc[key].join("\n"));
   }
   if ( ui->textEdit->document()->toPlainText().split("\n").last()== "{new_song}" )
   {
               QTextCursor cursor = ui->textEdit->textCursor();
               cursor.movePosition(QTextCursor::End);
               cursor.select(QTextCursor::LineUnderCursor);
               cursor.removeSelectedText();
               cursor.deletePreviousChar(); // Added to trim the newline char when removing last line
     }
}
void FormEditor::SortSontByAuthorAndName()
{
    QStringList head;
    QMap< QString,QStringList > doc;
    QString key="___" ;
    for ( auto line : ui->textEdit->document()->toPlainText().split("\n") )
    {
        QRegularExpression reg1,reg2;
        if ( ui->checkBoxLongShort->isChecked() )
            reg1=QRegularExpression("^\\{ *title: *([^}]+)\\}$");
        else
            reg1=QRegularExpression("^\\{ *st: *([^}]+)\\}$");
        if ( ui->checkBoxLongShort->isChecked() )
            reg2=QRegularExpression("^\\{ *subtitle: *([^}]+)\\}$");
        else
            reg2=QRegularExpression("^\\{ *t: *([^}]+)\\}$");
       if (line.contains(reg1)) key=line;
       else if ( line.contains(reg2) ) key=line+"_"+key;
       else doc[key]<<line;
    }
    ui->textEdit->clear();
    ui->textEdit->append(doc["___"].join("\n"));
    doc.remove("___");
    QStringList sortedList =doc.keys();
    sortedList.sort(Qt::CaseInsensitive);
    for ( auto key : sortedList)
    {

       /// if ( doc[key].at(0)!="{new_Song}")  ui->textEdit->append("{new_Song}\n");
       QString k=key;
       QStringList l=k.split("_");
        if ( l.length()>1) ui->textEdit->append(l.at(1));
        if ( l.length()>0)ui->textEdit->append(l.at(0));
        ui->textEdit->append(doc[key].join("\n"));
    }
//    if ( ui->textEdit->document()->toPlainText().split("\n").last()== "{new_Song}" )
//    {
//                QTextCursor cursor = ui->textEdit->textCursor();
//                cursor.movePosition(QTextCursor::End);
//                cursor.select(QTextCursor::LineUnderCursor);
//                cursor.removeSelectedText();
//                cursor.deletePreviousChar(); // Added to trim the newline char when removing last line
//      }
}

FormEditor::~FormEditor()
{
    delete ui;
}
void FormEditor::GenericInsert(QToolButton*w,QString token,QString label1, QString label2 , QString table)
{
    QString selected=ui->textEdit->textCursor().selectedText();
    if (! selected.isEmpty())
    {
        ui->textEdit->textCursor().removeSelectedText();
        ui->textEdit->insertPlainText(QString(token).arg(selected));
    }
    else
    {
        QString val;
        DialogInputDialog d(w);
        d.setValue(label1,label2,table);
        if ( d.exec() == QDialog::Accepted)
        {
            val=d.value();
            if ( !val.isEmpty() )
            {
               ui->textEdit->insertPlainText(QString(token).arg(val));

            }
        }
    }

}


void FormEditor::InsertVersion()
{
    GenericInsert(ui->toolButtonT,ui->checkBoxLongShort->isChecked()?QString("{version:%1}"):QString("{v:%1}"),QObject::tr("Enter version"),QObject::tr("Version"));
}



void FormEditor::InsertT()
{
    GenericInsert(ui->toolButtonT,ui->checkBoxLongShort->isChecked()?QString("{title:%1}"):QString("{t:%1}"),QObject::tr("Enter title"),QObject::tr("Title"));
}

void FormEditor::InsertST()
{
    GenericInsert(ui->toolButtonST,ui->checkBoxLongShort->isChecked()?QString("{subtitle:%1}"):QString("{st:%1}"),QObject::tr("Enter sub title"),QObject::tr("Sub Title"));
}

void FormEditor::InsertAlbum()
{
    GenericInsert(ui->toolButtonAlbum,QString("{album:%1}"),QObject::tr("Enter album"),QObject::tr("Album"));
}

void FormEditor::InsertArtist()
{
    GenericInsert(ui->toolButtonArtist,QString("{artist:%1}"),QObject::tr("Enter artist"),QObject::tr("Artist"),QString("Artist"));
}

void FormEditor::InsertComposer()
{
    GenericInsert(ui->toolButtonComposer,QString("{composer:%1}"),QObject::tr("Enter composer"),QObject::tr("Composer"),QString("Composer"));
}

void FormEditor::InsertLyricist()
{
    GenericInsert(ui->toolButtonLyricist,QString("{lyricist:%1}"),QObject::tr("Enter lyricist"),QObject::tr("Lyricist"),QString("Lyricist"));
}
void FormEditor::InsertArranger()
{
    GenericInsert(ui->toolButtonArranger,QString("{arranger:%1}"),QObject::tr("Enter arranger"),QObject::tr("Arranger"),QString("Arranger"));
}



void FormEditor::InsertCopyright()
{
    GenericInsert(ui->toolButtonCopyright,QString("{copyright:%1}"),QObject::tr("Enter copyright"),QObject::tr("Copyright"));
}

void FormEditor::InsertYear()
{
    GenericInsert(ui->toolButtonYear,QString("{year:%1}"),QObject::tr("Enter year"),QObject::tr("Year"));
}

void FormEditor::InsertKey()
{
    GenericInsert(ui->toolButtonKey,QString("{key:%1}"),QObject::tr("Enter key"),QObject::tr("Key"));
}

void FormEditor::InsertDuration()
{
    GenericInsert(ui->toolButtonDuration,QString("{duration:%1}"),QObject::tr("Enter duration"),QObject::tr("Duration"));
}

void FormEditor::InsertComment()
{
    GenericInsert(ui->toolButtonComment,ui->checkBoxLongShort->isChecked()?QString("{comment:%1}"):QString("{c:%1}"),QObject::tr("Enter comment"),QObject::tr("Comment"));
}

void FormEditor::InsertStyle()
{
    GenericInsert(ui->toolButtonStyle,ui->checkBoxLongShort->isChecked()?QString("{style:%1}"):QString("{s:%1}"),QObject::tr("Enter Style"),QObject::tr("Style"));
}

void FormEditor::InsertCommentItalic()
{
    GenericInsert(ui->toolButtonCommentItalic,ui->checkBoxLongShort->isChecked()?QString("{comment_Italic:%1}"):QString("{ci:%1}"),QObject::tr("Enter comment"),QObject::tr("Comment italic"));
}

void FormEditor::InsertCommentBox()
{
    GenericInsert(ui->toolButtonCommentBox,ui->checkBoxLongShort->isChecked()?QString("{comment_box:%1}"):QString("{ci:%1}"),QObject::tr("Enter comment"),QObject::tr("Comment box"));
}


void FormEditor::InsertCol()
{
    QString token=ui->checkBoxLongShort->isChecked()?QString("{columns:%1}"):QString("{col:%1}");
    int i=QInputDialog::getInt(ui->toolButtonTempo,tr("Enter columns number"),tr("Column number"),1,1,3);
    ui->textEdit->insertPlainText(QString(token).arg(QString("%1").arg(i)));
}



void FormEditor::ReplaceLongShort(  QString a, QString b)
{
    if ( ui->checkBoxLongShort->isChecked())
                 m_buffreplace.replace(a,b,Qt::CaseInsensitive);
             else
                 m_buffreplace.replace(b,a,Qt::CaseInsensitive);
}

void FormEditor::resizeEvent(QResizeEvent *)
{
  ui->tableWidgetToc->setColumnWidth(0,ui->tableWidgetToc->width());
}


void FormEditor::InsertCompress()
    {
        ui->textEdit->insertPlainText("{compress}");

    }



    void FormEditor::InsertCT()
    {
        GenericInsert(ui->toolButtonCT,ui->checkBoxLongShort->isChecked()?QString("{cover_title:%1}"):QString("{ct:%1}"),QObject::tr("Enter cover title"),QObject::tr("Cover title"));
    }

    void FormEditor::InsertEOS()
    {
        ui->textEdit->insertPlainText(ui->checkBoxLongShort->isChecked()?QString("{end_of_solo}"):QString("{eos}"));
    }


    void FormEditor::InsertSOS()
    {
        ui->textEdit->insertPlainText(ui->checkBoxLongShort->isChecked()?QString("{start_of_solo}"):QString("{sos}"));
    }

    void FormEditor::InsertPageBreak()
    {
        ui->textEdit->insertPlainText(ui->checkBoxLongShort->isChecked()?QString("{new_page}"):QString("{np}"));
    }


    void FormEditor::InsertSOC()
    {
        ui->textEdit->insertPlainText(ui->checkBoxLongShort->isChecked()?QString("{start_of_chorus}"):QString("{sor}"));
    }

    void FormEditor::InsertEOC()
    {
        ui->textEdit->insertPlainText(ui->checkBoxLongShort->isChecked()?QString("{end_of_chorus}"):QString("{eor}"));
    }

    void FormEditor::InsertSOV()
    {
        ui->textEdit->insertPlainText(ui->checkBoxLongShort->isChecked()?QString("{start_of_verse}"):QString("{sov}"));
    }


    void FormEditor::InsertEOV()
    {
        ui->textEdit->insertPlainText(ui->checkBoxLongShort->isChecked()?QString("{end_of_verse}"):QString("{eov}"));
    }

    void FormEditor::InsertSOG()
    {
        ui->textEdit->insertPlainText(QString("{start_of_grid}"));
    }


    void FormEditor::InsertEOG()
    {
        ui->textEdit->insertPlainText(QString("{end_of_grid}"));
    }

    void FormEditor::InsertCB()
    {
       ui->textEdit->insertPlainText(ui->checkBoxLongShort->isChecked()?QString("{column_break}"):QString("{colb}"));
    }

    void FormEditor::InsertTempo()
    {
        double i=QInputDialog::getDouble(ui->toolButtonTempo,tr("Enter tempo"),tr("Beats per minute"),120,20,250);
        ui->textEdit->insertPlainText(QString("{tempo:%1}").arg(QString("%1").arg(i)));
    }

    void FormEditor::InsertTime()
    {
        DialogBar time(ui->toolButtonTime);
        time.exec();
        if ( !time.canceled())
        {
            ui->textEdit->insertPlainText(QString("{time:%1}").arg(time.value()));
            ui->textEdit->insertPlainText(QString(QChar(0x2028)));
        }
    }

    void FormEditor::InsertCS()
    {
        GenericInsert(ui->toolButtonCS,ui->checkBoxLongShort->isChecked()?QString("{cover_subtitle:%1}"):QString("{cs:%1}"),QObject::tr("Enter  cover subtitle"),QObject::tr("Cover Subtitle"));
    }



    void FormEditor::ToogleLongShort()
    {
        m_buffreplace=ui->textEdit->toPlainText();
        ReplaceLongShort("{t:","{title:");
        ReplaceLongShort("{st:","{subtitle:" );
        ReplaceLongShort("{ct:","{cover_title:");
        ReplaceLongShort("{cs:","{cover_subtitle:");
        ReplaceLongShort("{col:","{columns:");
        ReplaceLongShort("{colb}","{column_break}");
        ReplaceLongShort("{ns}","{new_song}");
        ReplaceLongShort("{sos}","{start_of_solo}");
        ReplaceLongShort("{eos}","{end_of_solo}");
        ReplaceLongShort("{soc}","{start_of_chorus}");
        ReplaceLongShort("{eoc}","{end_of_chorus}");
        ReplaceLongShort("{sov}","{start_of_verse}");
        ReplaceLongShort("{eov}","{end_of_verse}");
        ReplaceLongShort("{sog}","{start_of_grid}");
        ReplaceLongShort("{eog}","{end_of_grid}");
        ReplaceLongShort("{np}","{new_page}");
        ReplaceLongShort("{c:","{comment:");
        ReplaceLongShort("{ci:","{comment_italic:");
        ReplaceLongShort("{ci:","{comment_italic:");
        ReplaceLongShort("{cb:","{comment_box:");
        ReplaceLongShort("{v:","{version:");
        ReplaceLongShort("{s:","{style:");
        
        
        
        ui->textEdit->setText(m_buffreplace);
    }

    void FormEditor::TocUpdate()
    {
        ui->tableWidgetToc->clear();
        ui->tableWidgetToc->setRowCount(0);
        QStringList text=ui->textEdit->toPlainText().split('\n');
        QRegularExpression Title("^\\{(title|t):([^}]+)\\}$",QRegularExpression::CaseInsensitiveOption);
        QRegularExpressionMatch titlematch;
        QStringList toc;
        for ( auto t : text)
        {
            if ( t.contains(Title,&titlematch))
            {
                ui->tableWidgetToc->setRowCount(ui->tableWidgetToc->rowCount()+1);
                ui->tableWidgetToc->setItem(ui->tableWidgetToc->rowCount()-1,0,
                                                  new QTableWidgetItem(titlematch.captured(2).trimmed()));
                toc<<titlematch.captured(2);
            }
        }
        emit Toc(toc);
    }

    void FormEditor::TocToText(QModelIndex index)
    {
        QString text1=index.data().toString();
        text1.remove(QRegularExpression("^ "));
        text1.remove(QRegularExpression(" $"));
        QRegularExpression text;
        if ( ui->checkBoxLongShort->isChecked() )
            text=QRegularExpression(QString("\\{title: *%1\\}").arg(text1));
        else
            text=QRegularExpression(QString("\\{t: *%1\\}").arg(text1));
        ui->textEdit->moveCursor(QTextCursor::End);
        ui->textEdit->find(text,QTextDocument::FindBackward);
        MemorizeTextSelectedSong();
        emit SongSelected(text1);
    }

    void FormEditor::MemorizeTextSelectedSong()
    {
        TextSontMemorized.clear();
        int from =textCursor().position()-textCursor().selectedText().length();
        QRegularExpression nextTitle;
        int correction;
        if ( ui->checkBoxLongShort->isChecked() )
        {
                       nextTitle=QRegularExpression(QString("^ *\\{title:"));
                       correction=7;
        }
        else
        {
                       nextTitle=QRegularExpression(QString("^ *\\{t:"));
                       correction=3;
        }

        int to=(ui->textEdit->find(nextTitle))? textCursor().position(): ui->textEdit->toPlainText().size();
        QString buffer=TextSontMemorized=ui->textEdit->toPlainText();
        TextSontMemorized=buffer.mid(from,to-from-correction);
        ui->textEdit->find(nextTitle,QTextDocument::FindBackward);


    }

    void FormEditor::AskChord( QString chord, QString title)
    {
        QRegularExpression text;
        title.remove(QRegularExpression("^ "));
        title.remove(QRegularExpression(" $"));
        if ( ui->checkBoxLongShort->isChecked() )
            text=QRegularExpression(QString("\\{title: *%1\\}").arg(title));
        else
            text=QRegularExpression(QString("\\{t: *%1\\}").arg(title));
        ui->textEdit->moveCursor(QTextCursor::End);
        ui->textEdit->find(text,QTextDocument::FindBackward);
        Chord ch(chord);
        QString s=QString("[%1").arg(ch.ToLocale(chord));
        ui->textEdit->find(s);
        emit SongSelected(title);
    }



    void FormEditor::InsertNewSong()
    {
        m_dialognewsong->show();
    }


    void FormEditor::InsertSong(QString title,QString subtitle,bool compressed,int column, double tempo,
                                int time1, int time2, DialogNewSong::Insertion insertmode)
    {
        QRegularExpression text;
        QRegularExpressionMatch textmatch;
        if ( ui->checkBoxLongShort->isChecked() )
            text=QRegularExpression("^\\{ *title: *([^}]+)\\}$");
        else
            text=QRegularExpression("^\\{ *t: *([^}]+)\\}$");

        QString t;
        if (insertmode != DialogNewSong::UnderCursor)
        {
        ui->textEdit->moveCursor(QTextCursor::Start);
        int line=0;
        bool found=false;


        for ( QString t : ui->textEdit->toPlainText().split("\n"))
        {
           if ( t.contains(text) )
           {
               QString tt=textmatch.captured(1).toLower();
               tt.remove(QRegularExpression("^ +"));
               tt.replace("à","a");tt.replace("ä","a");tt.replace("â","a");
               tt.replace("é","e");tt.replace("è","e");tt.replace("ë","e");tt.replace("ê","e");
               tt.replace("ï","i"); tt.replace("î","i");
               tt.replace("ô","o");tt.replace("ö","o");
               tt.replace("û","u");tt.replace("ü","u");tt.replace("ù","u");
               tt.replace("ç","c");
               if ( tt > title.toLower())
                 {
                   found =true ;
                   break;
                 }
           }
           line++;
        }
        QTextCursor cursor;
        if ( found )
         {
          cursor= QTextCursor(ui->textEdit->document()->findBlockByLineNumber(line-1));
          ui->textEdit->setTextCursor(cursor);
         }
        else
           ui->textEdit->moveCursor(QTextCursor::End);




        cursor.movePosition(QTextCursor::StartOfLine);
        cursor.movePosition(QTextCursor::Up,QTextCursor::MoveAnchor,2);
        }
        if ( ui->checkBoxLongShort->isChecked() )
            t=QString("{title:%1}").arg(title);
        else
            t=QString("{t:%2}").arg(title);
        if ( ui->checkBoxLongShort->isChecked() )
            ui->textEdit->textCursor().insertText("{new_song}\n");
        else
            ui->textEdit->textCursor().insertText("{ns}\n");
        ui->textEdit->textCursor().insertText(t+"\n");
        if ( !subtitle.isEmpty())
        {

            if ( ui->checkBoxLongShort->isChecked() )
                t=QString("{subtitle:%1}").arg(subtitle);
            else
                t=QString("{st:%2}").arg(subtitle);
            ui->textEdit->insertPlainText(t);
        }
        if ( compressed)
        {
            t="{compress}\n";
            ui->textEdit->textCursor().insertText(t);
        }
        if ( column > 1)
        {
            t=QString("{columns:%1}\n").arg(column);
            ui->textEdit->insertPlainText(t);
        }
        t =QString("{time:%1/%2}").arg(time1).arg(time2);
        ui->textEdit->insertPlainText(t);
        t =QString("{tempo:%1}\n").arg(tempo);
        ui->textEdit->insertPlainText(t);
        ui->textEdit->insertPlainText("\n\n");

    }


QString  FormEditor::getChord( TextEdit * edit)
{
    const int max=10;
    QTextCursor cursor(edit->textCursor());
    cursor.movePosition(QTextCursor::PreviousCharacter,QTextCursor::KeepAnchor,1);
    int pos=max;
    while ( pos != 0  && ! cursor.selectedText().startsWith("["))  {
      cursor.movePosition(QTextCursor::PreviousCharacter,QTextCursor::KeepAnchor,1);
      pos--;
    }
    if (pos == 0 ) return QString(); 
    cursor.movePosition(QTextCursor::NextCharacter,QTextCursor::KeepAnchor,1);
    cursor.clearSelection();
    edit->setTextCursor(cursor);
    pos=max;
    while ( pos != 0  && ! cursor.selectedText().endsWith("]")) 
    {
      cursor.movePosition(QTextCursor::NextCharacter,QTextCursor::KeepAnchor,1);
      pos--;
    }
    if (pos == 0 ) return QString();
    cursor.movePosition(QTextCursor::PreviousCharacter,QTextCursor::KeepAnchor,1);
    edit->setTextCursor(cursor);
    QString ch=cursor.selectedText();
    cursor.removeSelectedText();
    return(ch);
}


void FormEditor::TransposeChord(int numberofchroma, int range)
{
    if ( range == ChordUnderTheCurser )
         {
            QTextCursor cursor = textCursor();
            QString chord=getChord(ui->textEdit);
            if ( ! chord.isEmpty() )
            {
                QString chordalone, fret, rhythm;
                Chord::SplitChord(chord,chordalone,fret,rhythm);
                QString transposedword=Chord::transposeWithLanguage(chordalone,m_chordlanguage,m_minor,numberofchroma)
                                       +rhythm;
                cursor.insertText(transposedword);
            }
         }
    else if (range == AllChordInLine )
         {
           QTextCursor cursor = textCursor();
           cursor.select(QTextCursor::LineUnderCursor);
           QString line=cursor.selectedText();
           cursor.removeSelectedText();
           line=TransposeLineWithChord(line,numberofchroma);
           cursor.insertText(line);
         }
    else if ( range == AllChordInSong)
         {
            QTextCursor cursor = document()->find(QRegularExpression("^\\{(t|title):"),textCursor(),QTextDocument::FindBackward);
            cursor.movePosition(QTextCursor::NextBlock);
            QString line;
            do
            {
              cursor.movePosition(QTextCursor::NextBlock,QTextCursor::KeepAnchor);
              line=cursor.selectedText();
              cursor.removeSelectedText();
              line=TransposeLineWithChord(line,numberofchroma);
              cursor.insertText(line);
            }
            while ( ! cursor.atEnd() && ! line.contains(QRegularExpression("^\\{(t|title):")));
          }
    else if ( range == AllChordInFile )
         {
             QStringList lines=document()->toPlainText().split("\n");
             document()->clear();
             foreach (QString line , lines)
                {
                 line=TransposeLineWithChord(line,numberofchroma);
                 insertPlainText(line);
                }
         }
}

QString FormEditor::TransposeLineWithChord( QString line, int numberofchroma)
{
    QRegularExpression regchord("(\\[[^]]+\\])");
    QRegularExpressionMatch regchordmatch;;
    while ( line.indexOf(regchord,0,&regchordmatch)!=-1)
      {
           QString newchord=regchordmatch.captured(1).replace("[","").replace("]","");
           QString chordalone, fret, rhythm;
           Chord::SplitChord(newchord,chordalone,fret,rhythm);
           QString transposedchord=Chord::transposeWithLanguage(chordalone,m_chordlanguage,m_minor,numberofchroma)
                                    +rhythm;
           line.replace("["+newchord+"]",QString("&lt;%1&gt;").arg(transposedchord));
      }
      line.replace("&lt;","[").replace("&gt;","]");
      return line;
}
