#include "processor.h"
#include "ui_formconfig.h"
#include "ui_mainwindow.h"
#include "constants.h"
#include "chord.h"
#include "chordutil.h"

#include <QRegularExpression>
#include <QDebug>
#include <QFileInfo>
#include <QtMath>
#include <QDir>
#include <QStringList>
#include <QSqlQuery>
#include <QMessageBox>

#include <QGraphicsColorizeEffect>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QPainter>

using namespace PoDoFo;



Processor::Processor(Ui::MainWindow *ui1, Ui::FormConfig *ui2)
{
 try{
    m_uiconfig=ui2;
    m_fatalerror=true;
    m_uimainwindow=ui1;
    m_projectpath=m_uimainwindow->statusBar->getLabelDirName()->text();
    m_documentAllocation=false;

    m_file=QString("%1/%2_%3.pdf")
            .arg(m_uimainwindow->statusBar->getLabelDirName()->text())
            .arg(m_uimainwindow->statusBar->getProjectName())
            .arg(m_uiconfig->lineEditOutFile->text());

    m_pageAllocation=true;
    m_line=static_cast<int>(m_uiconfig->spuPageHeight->getPdfU()- m_uiconfig->spuVerticalMargin->getPdfU());
    m_column=static_cast<int>(m_uiconfig->spuHorizontalMargin->getPdfU());
    if (m_file.isEmpty()) return;
    QFile file(m_file);
    file.open(QIODevice::ReadWrite);
    file.close();
    QFileInfo fi(m_file);
    if ( !fi.isWritable() )  return;

    m_document = new PdfStreamedDocument(m_file.toStdString().c_str());
    m_dimension = new PdfRect(PageSize(0,0,m_uiconfig->spuPageWidth->getPdfU(),m_uiconfig->spuPageHeight->getPdfU()));
    m_documentAllocation=true;
    m_compress=false;
    m_refrain=false;
    m_text=m_uimainwindow->textEditCho3File->document()->toPlainText();
    m_nbrealpages=0;
    m_mode="generic";
    m_covertitleexist=false;
    m_covertsubtitleexist=false;
    m_time="4/4";
    m_tempo=120;
    m_subtitle.clear();
    m_fatalerror=false;
    m_subtitleMax=ui2->comboBoxSubtitleNumber->currentIndex();
    m_forcecolumn=ui2->comboBoxColumnNumber->currentIndex();
    m_pausenumber = ui2->comboBoxVerticalPausePerPage->currentIndex()+1;
    m_copyoffset=static_cast<int>(ui2->spuPageHeight->getPdfU()*1.0/m_pausenumber);
 }
    catch(const PoDoFo::PdfError & e )
    {
        throw (e);
    }
}


void Processor::run()
{

    try {

    QString TitleAsked;
    if (m_fatalerror)
       {
        emit Error(tr("File %1 is not writable. Save as origin file in a writable place before to make a pdf").arg(m_file));
        return;
       }
    QRegularExpression NewSongREX("^ *\\{(new_song|ns) *\\} *$",QRegularExpression::CaseInsensitiveOption);
    QRegularExpressionMatch NewSongREXMatch,CompressREXMatch,ColumnsREXMatch,ColumnBreakREXMatch,CoverTitleREXMatch,CoverSubTitleREXMatch,TitleREXMatch;
    QRegularExpressionMatch SocREXMatch,EocREXMatch,SorREXMatch,EorREXMatch,DefineRexMatch,ChordREXMatch,TempoREXMatch,TimeREXMatch,ComposerRexMatch,LyricistRexMatch,ArrangerRexMatch;
    QRegularExpressionMatch ArtistRexMatch,AlbumRexMatch,CopyrightRexMatch,YearRexMatch,VersionRexMatch,KeyRexMatch,DurationRexMatch,SubTitleRexMatch,StyleREXMatch;
    QRegularExpression CompressREX("^ *\\{compress\\} * *$",QRegularExpression::CaseInsensitiveOption);
    QRegularExpression ColumnsREX("^ *\\{(?:col|columns): *([^}]*) *\\}",QRegularExpression::CaseInsensitiveOption);
    QRegularExpression ColumnBreakREX("^ *\\{(colb|column_break)(:(text|lyrics))? *\\}",QRegularExpression::CaseInsensitiveOption);
    QRegularExpression NewPageREX("^ *\\{(np|new_page)(:(text|lyrics))? *\\}",QRegularExpression::CaseInsensitiveOption);
    QRegularExpression CoverTitleREX("^ *\\{*(?:cover_title|ct): *([^}]+)\\}",QRegularExpression::CaseInsensitiveOption);
    QRegularExpression CoverSubTitleREX("^ *\\{(?:cover_subtitle|cs): *([^}]+)\\}",QRegularExpression::CaseInsensitiveOption);
    QRegularExpression SubTitleREX("^ *\\{(?:subtitle|st): *([^}]+)\\}",QRegularExpression::CaseInsensitiveOption);
    QRegularExpression TitleREX("^ *\\{(?:title|t): *([^}]*)\\}",QRegularExpression::CaseInsensitiveOption);
    QRegularExpression SosREX("^ *\\{(?:sos|start_of_solo)\\}",QRegularExpression::CaseInsensitiveOption);
    QRegularExpression EosREX("^ *\\{(?:eos|end_of_solo)\\}",QRegularExpression::CaseInsensitiveOption);
    QRegularExpression SocREX("^ *\\{(?:soc|start_of_chorus)\\}",QRegularExpression::CaseInsensitiveOption);
    QRegularExpression EocREX("^ *\\{(?:eoc|end_of_chorus)\\}",QRegularExpression::CaseInsensitiveOption);
    QRegularExpression SovREX("^ *\\{(?:sov|start_of_verse)\\}",QRegularExpression::CaseInsensitiveOption);
    QRegularExpression EovREX("^ *\\{(?:eov|end_of_verse)\\}",QRegularExpression::CaseInsensitiveOption);
    QRegularExpression SogREX("^ *\\{(?:sog|start_of_grid)\\}",QRegularExpression::CaseInsensitiveOption);
    QRegularExpression EogREX("^ *\\{(?:eog|end_of_grid)\\}",QRegularExpression::CaseInsensitiveOption);
    QRegularExpression DefineRex("^\\{define: *([^ ]+) +base-fret +(\\d+) +frets +(\\d+|x) +(\\d+|x) +(\\d+|x) +(\\d+|x) +(\\d+|x) +(\\d+|x)([^}]*)\\}",QRegularExpression::CaseInsensitiveOption);
    QRegularExpression ChordREX("\\[[^]]+\\]",QRegularExpression::CaseInsensitiveOption);
    QRegularExpression TempoREX("^ *\\{(?:tempo): *([^}]*) *\\}",QRegularExpression::CaseInsensitiveOption);
    QRegularExpression TimeREX("^ *\\{(?:time): *([^}]*) *\\}",QRegularExpression::CaseInsensitiveOption);
    QRegularExpression ComposerRex("^ *\\{(?:composer): *([^}]*) *\\}",QRegularExpression::CaseInsensitiveOption);
    QRegularExpression LyricistRex("^ *\\{(?:lyricist): *([^}]*) *\\}",QRegularExpression::CaseInsensitiveOption);
    QRegularExpression ArrangerRex("^ *\\{(?:arranger): *([^}]*) *\\}",QRegularExpression::CaseInsensitiveOption);
    QRegularExpression ArtistRex("^ *\\{(?:artist): *([^}]*) *\\}",QRegularExpression::CaseInsensitiveOption);
    QRegularExpression AlbumRex("^ *\\{(?:album): *([^}]*) *\\}",QRegularExpression::CaseInsensitiveOption);
    QRegularExpression CopyrightRex("^ *\\{(?:copyright): *([^}]*) *\\}",QRegularExpression::CaseInsensitiveOption);
    QRegularExpression YearRex("^ *\\{(?:year): *([^}]*) *\\}",QRegularExpression::CaseInsensitiveOption);
    QRegularExpression KeyRex("^ *\\{(?:key): *([^}]*) *\\}",QRegularExpression::CaseInsensitiveOption);
    QRegularExpression StyleRex("^ *\\{(?:style): *([^}]*) *\\}",QRegularExpression::CaseInsensitiveOption);
    QRegularExpression DurationRex("^ *\\{(?:duration): *([^}]*) *\\}",QRegularExpression::CaseInsensitiveOption);
    QRegularExpression VersionRex("^ *\\{(?:version|v): *([^}]*) *\\}",QRegularExpression::CaseInsensitiveOption);
    setCoverMade(false);
    m_lineindex=0;
    foreach ( QString line, m_text.split(QRegularExpression("\n")) )
    {
        m_lineindex++;
        if ( line.contains(NewSongREX) )
        { m_newsong =true;
          m_initialvposition=m_uiconfig->spuHorizontalMargin->getPdfU();
          m_firstlyric=true;
        }
        else if ( line.contains(CompressREX) )
        {
            setCompress(true);
        }
        else if ( line.contains(ColumnsREX,&ColumnsREXMatch) )
        {
            int coln=1;
            if ( m_forcecolumn == 0 )
                {
                coln=ColumnsREXMatch.captured(1).toInt();
                setColNumber(coln);
                }
            else
                if (m_forcecolumn >=2 )
                {
                   coln=m_forcecolumn;
                   setColNumber(coln);
                }
            //m_pagecontinued =coln>1;
            m_pagecontinued=0;
        }
        else if ( line.contains(TempoREX,&TempoREXMatch))
        {
            setBpm(TempoREXMatch.captured(1).toDouble());
        }
        else if ( line.contains(TimeREX,&TimeREXMatch))
        {
            setRytm(TimeREXMatch.captured(1));
        }
        else if ( line.contains(StyleRex,&StyleREXMatch))
        {
            setRytm(TimeREXMatch.captured(1));
        }
        
        else if ( line.contains(ComposerRex,&ComposerRexMatch))
        {
            setComposer(ComposerRexMatch.captured(1));
        }
        else if ( line.contains(LyricistRex,&LyricistRexMatch))
        {
            setLyricist(LyricistRexMatch.captured(1));
        }
        else if ( line.contains(ArrangerRex,&ArrangerRexMatch))
        {
            setArranger(ArtistRexMatch.captured(1));
        }
        else if ( line.contains(ArtistRex,&ArtistRexMatch))
        {
            setArtist(ArtistRexMatch.captured(1));
        }
        else if ( line.contains(AlbumRex,&AlbumRexMatch))
        {
            setAlbum(AlbumRexMatch.captured(1));
        }
        else if ( line.contains(CopyrightRex,&CopyrightRexMatch))
        {
            setCopyright(CopyrightRexMatch.captured(1));
        }
        else if ( line.contains(YearRex,&YearRexMatch))
        {
            setYear(YearRexMatch.captured(1).toInt());
        }
        else if ( line.contains(KeyRex,&KeyRexMatch))
        {
            setKey(KeyRexMatch.captured(1));
        }
        else if ( line.contains(KeyRex,&StyleREXMatch))
        {
            setKey(StyleREXMatch.captured(1));
        }
        
        else if ( line.contains(DurationRex,&DurationRexMatch))
        {
            setDuration(DurationRexMatch.captured(1));
        }
        else if ( line.contains(ColumnBreakREX) )
        {
            setColBreak(line);
        }
        else if ( line.contains(NewPageREX) )
        {
            setColBreak(line);
        }
        else if ( line.contains(CoverTitleREX,&CoverTitleREXMatch) )
        {
            setCoverTitle(CoverTitleREXMatch.captured(1));
            m_newsong=true;
        }
        else if (line.contains(VersionRex,&VersionRexMatch) )
        {
            setVersion(VersionRexMatch.captured(1));
            m_newsong=true;
        }
        else if ( line.contains(CoverSubTitleREX,&CoverSubTitleREXMatch) )
        {
            setCoverSubtitle(CoverSubTitleREXMatch.captured(1));
            m_newsong=true;
        }
        else if ( line.contains(SubTitleREX,&SubTitleRexMatch) )
        {
        if ( m_uiconfig->comboBoxSubtitleNumber->currentIndex()!=0)  m_subtitle<<SubTitleRexMatch.captured(1);
        }
        else if (line.contains(ColumnBreakREX,&ColumnBreakREXMatch))
        {
            doColumnBreak(line);
        }
        else if (line.contains(DefineRex,&DefineRexMatch))
        {
            m_newsong=false;
            // define a chord
            QString chordname=DefineRexMatch.captured(1);
            QString basefret=DefineRexMatch.captured(2);
            QString fret=QString("%1 %2 %3 %4 %5 %6").arg(DefineRexMatch.captured(3))
                    .arg(DefineRexMatch.captured(4)).arg(DefineRexMatch.captured(5)).arg(DefineRexMatch.captured(6))
                    .arg(DefineRexMatch.captured(7)).arg(DefineRexMatch.captured(8));
            QSqlQuery select (QString("SELECT * FROM chords WHERE Name='%1' AND Approved=1 ").arg(chordname));
            if (select.next())
            {
                if ( basefret != "0" )
                {
                     QString newchordname=QString("%1(%2)").arg(chordname).arg(ChordUtil::toRomain(basefret.toInt()));
                     QSqlQuery select (QString("SELECT * FROM chords WHERE Name='%1'").arg(newchordname));
                     if ( ! select.next())
                     {
                         QSqlQuery query(QString("INSERT INTO chords (Name, Value, Approved) VALUES ('%1','%2 %3',0)").arg(newchordname).arg(basefret).arg(fret));

                     }
                }
            }
            else
            {
                QSqlQuery select (QString("SELECT * FROM chords WHERE Name='%1' AND Approved=0 ").arg(chordname));
                if (  ! select.next() )
                QSqlQuery query(QString("INSERT INTO chords (Name, Value, Approved) VALUES ('%1','%2 %3',0)").arg(chordname).arg(basefret).arg(fret));
            }
        }
        else if ( line.contains(TitleREX,&TitleREXMatch) )
        {
            m_newsong=true;
            m_verticalStartOfLyric=0;

            if ( m_covertitleexist && ! getCoverMade()  
                && m_uiconfig->checkBoxCover->isChecked())
            {
                 Cover(getCoverTitle(),getCoverSubtitle(),getVersion());
                 setCoverMade(true);
                 m_nbrealpages=0;
            }


            PrintPageSubtitle();
            displayChordsForSong();
            displayLyrics();
            m_nbrealpages++;
            displayPageTitle(TitleREXMatch.captured(1));
             m_verticalStartOfLyric=m_line;
             m_initialvposition=m_line;
             m_initiallyric=m_line;
              m_BufLyrics.clear();
              m_BufChords.clear();
              m_subtitle.clear();
              setCompress(false);
              m_tempo=120;
              m_time=QString("4/4");
              m_lyricist.clear();
              m_arranger.clear();
              m_album.clear();
              m_copyright.clear();
              m_duration.clear();
              m_year=0;
              m_composer.clear();
              m_artist.clear();
              m_firstline=false;
              m_firstlyric=0;
        }
        else if ( line.contains(SosREX) )
        {   
            includeSos();
        }
        else if ( line.contains(EosREX) )
        {
            includeEos();
            
        }
        else if ( line.contains(SocREX) )
        {
            setRefrain(true);
 
            m_newsong=false;
        }
        else if ( line.contains(EocREX) )
        {
            setRefrain(false);
            includeEoc();
            m_newsong=false;
        }
        else if ( line.contains(SovREX) )
        {
            m_newsong=false;
        }
        else if ( line.contains(EovREX) )
        {
            setRefrain(false);
            includeEoc();
            m_newsong=false;
        }
        else if ( line.contains(SogREX) )
        {
                qDebug()<<"Start of Grid not implemented";
        }
        else if ( line.contains(EogREX) )
        {
        }
        else if ( line.contains(ChordREX) )
        {
             m_BufLyrics<<line;
             memorizeChords(line);
             m_newsong=false;
        }
        else 
        {
            m_newsong=false;
            m_BufLyrics<<line;
        }

    }

PrintPageSubtitle();
doChords();

displayChordsForSong();
displayLyrics();
addToc();
save();
openExistingFile();
addFooter();
makePageNumber();
savemem();
emit PDFMade(m_file);
}
    catch(const PoDoFo::PdfError & e )
    {
        throw (e);
    }
}


void Processor::PrintPageSubtitle()
{
 int nbsubtitle= m_subtitleMax < m_subtitle.count()? m_subtitleMax:m_subtitle.count();
                for ( int t =0; t < nbsubtitle ; t++)
                displayPageSubtitle(m_subtitle.at(t));
                displayLyricistComposerYear();
                displayRytm();
                displayBpm();
}


Processor::~Processor()
{
    if ( m_documentAllocation )
    {
      delete m_document;
      delete m_dimension;
    }

    m_documentAllocation=false;
    m_pageAllocation=false;
}

void Processor::setCompress(bool compress)
{
    m_compress=compress;
}

void Processor::setColNumber(int colnumber)
{
    m_colnumber=colnumber;
    ////m_pagecontinued=m_colnumber >=2 ? true:false;
}

void Processor::setBpm(double bpm)
{
    m_tempo=bpm;
}

void Processor::setRytm( QString rythm)
{
    m_time=rythm;
}

void Processor::setStyle( QString style)
{
    m_style=style;
}


void Processor::setCoverTitle(QString covertitle)
{
    m_covertitle=covertitle;
    m_covertitleexist=true;
    includeInfo();
}


void Processor::includeInfo(QString author, QString title,QString subtitle,QString date)
{
   m_document->GetInfo()->SetCreator ( PdfString("Chord V") );
   m_document->GetInfo()->SetAuthor  ( PdfString(author.toStdString()) );
   m_document->GetInfo()->SetTitle   ( PdfString(title.toStdString()) );
   m_document->GetInfo()->SetSubject ( PdfString(subtitle.toStdString()) );
   m_document->GetInfo()->SetKeywords( PdfString(date.toStdString()));
}

int Processor::calcColumn()
{
  return 1 ;
}

int Processor::calcLine()
{
 return 1 ;
}

void Processor::setCoverSubtitle(QString coversubtitle)
{
    m_covertsubtitleexist=true;
    m_coversubtitle=coversubtitle;
}

QString Processor::getVersion(){return m_version ;}

void Processor::setVersion(QString version)
{
    m_version=version;
}

void Processor::includeRefrain(QString line)
{
  m_BufLyrics<<line;
}


void Processor::includeSos()
{
 
}

void Processor::includeEos()
{
}


void Processor::includeSog()
{
    
}

void Processor::includeEog()
{
    
}


void Processor::includeSoc()
{
    m_BufLyrics<<"SOC"; 
}

void Processor::includeEoc()
{
 
}

void Processor::includeSolo( QString  s)
{
}

void Processor::displayPageTitle( QString title)
{
    if ( m_uiconfig->comboBoxDuplex->currentIndex()==2 && (m_nbrealpages+1)% 2 == 1)
    {
        newPage();
        m_nbrealpages++;
    }
    newPage();
    m_NormalPages<<m_page->GetContents();
    m_title=title.replace(QRegularExpression("^ +"),"");
    m_tocpages[m_title]=1;
    m_toc<<m_title;
    //if ( m_uiconfig->comboBoxColumnNumber->currentIndex() >=2)  m_colnumber=m_uiconfig->comboBoxColumnNumber->currentIndex(); else m_colnumber=1;
    m_colnumber=1;
    if ( m_uiconfig->toolButtonTitleFont->getBackgroundColor() != m_uiconfig->colorButtonPaperColor->getColor())
    {
      m_painter.SetColor(m_uiconfig->toolButtonTitleFont->getBackgroundColor().redF(),m_uiconfig->toolButtonTitleFont->getBackgroundColor().greenF(),m_uiconfig->toolButtonTitleFont->getBackgroundColor().blueF());
      m_painter.Rectangle(m_uiconfig->spuHorizontalMargin->getPdfU(),
                        m_uiconfig->spuPageHeight->getPdfU()-m_uiconfig->spuVerticalMargin->getPdfU()-m_uiconfig->toolButtonTitleFont->getFont().pointSize(),
                        m_uiconfig->spuPageWidth->getPdfU()-2*m_uiconfig->spuHorizontalMargin->getPdfU(),
                        m_uiconfig->toolButtonTitleFont->getFont().pointSize());
      m_painter.Fill();
    }
    if ( m_uiconfig->checkBoxTitleInUppercase->isChecked()) title=title.toUpper();
    m_line= static_cast<int>(m_uiconfig->spuPageHeight->getPdfU()- m_uiconfig->spuVerticalMargin->getPdfU()-m_uiconfig->toolButtonTitleFont->getFont().pointSize()/1.4);//*0.8;
    Text(m_document,title,m_uiconfig->spuPageWidth->getPdfU()/2,
               m_line,
               m_uiconfig->toolButtonTitleFont,center);
    m_firstline=true;
    m_line-=m_uiconfig->toolButtonTitleFont->font().pointSize();
    //m_line-=m_uiconfig->toolButtonSubtitleFont->getFont().pointSize()/1.8;
    m_initialvposition-=m_line;
    
   //// m_pagecontinued=m_colnumber >=2 ? true:false;
}

void Processor::displayPageSubtitle(QString subtitle)
{

    int subtitlenumber=m_subtitleMax <m_subtitle.count()? m_subtitleMax:m_subtitle.count();

    if ( !m_pageAllocation)
    {
       m_page = m_document->CreatePage(*m_dimension);
       m_painter.SetPage(m_page);
       m_pageAllocation=true;
    }
  if ( m_uiconfig->toolButtonSubtitleFont->getBackgroundColor() != m_uiconfig->colorButtonPaperColor->getColor())
  {
    m_painter.SetColor(m_uiconfig->toolButtonSubtitleFont->getBackgroundColor().redF(),
                       m_uiconfig->toolButtonSubtitleFont->getBackgroundColor().greenF(),
                       m_uiconfig->toolButtonSubtitleFont->getBackgroundColor().blueF());
    m_painter.Rectangle(m_uiconfig->spuHorizontalMargin->getPdfU(),
                        m_uiconfig->spuPageHeight->getPdfU()-m_uiconfig->spuVerticalMargin->getPdfU()-m_uiconfig->toolButtonTitleFont->getFont().pointSize()-subtitlenumber*m_uiconfig->toolButtonSubtitleFont->getFont().pointSize(),//*1.4-1,
                        m_uiconfig->spuPageWidth->getPdfU()-2*m_uiconfig->spuHorizontalMargin->getPdfU(),
                        m_uiconfig->toolButtonSubtitleFont->getFont().pointSize());
    m_painter.Fill();
  }
  m_line-=m_uiconfig->toolButtonSubtitleFont->getFont().pointSize();
  Text(m_document,subtitle,m_uiconfig->spuPageWidth->getPdfU()/2,
       m_line,
       m_uiconfig->toolButtonSubtitleFont,center);
  m_line-=m_uiconfig->toolButtonSubtitleFont->font().pointSize();
  m_initialvposition-=m_line;
  m_pagecontinued=0;

}


void Processor::setRefrain(bool socmode)
{
   if ( socmode ) m_BufLyrics<<tr("Chorus :");
   m_refrain=socmode;
  
}
void Processor::setSovMode(bool )
{ 
}



void Processor::newPage()
{
    if ( m_pageAllocation)
    {
        FinishPage(&m_painter);
    }
    m_page = m_document->CreatePage(*m_dimension);
    m_painter.SetPage(m_page);
    m_line= static_cast<int>(m_uiconfig->spuPageHeight->getPdfU()-m_uiconfig->spuVerticalMargin->getPdfU());
    if ( m_mode == "lyrics" && m_uiconfig->comboBoxChordInText->currentIndex()==FormConfig::DiagramInText)
    {
        m_line-=m_uiconfig->spuChordHorizontalSize->getPdfU();
    }
    m_column= static_cast<int>(m_uiconfig->spuHorizontalMargin->getPdfU());
    m_pageAllocation=true;
    m_pagecontinued++;
 }

void Processor::doColumnBreak(QString )
{   
        int currentcol=currentColumn();
        if ( m_colnumber >=2 &&  currentcol < m_colnumber  )
              {
                m_column = nextColumn( currentcol)  ;
                if ( m_pagecontinued  == 1 )
                       m_line=m_initiallyric;
                else
                        m_line= m_initialvposition;
                m_columnChangeAsked=true;
                
              }
            else if ( m_colnumber >=2 )
            {
                if (m_tocpages.count() != 0)
                    m_tocpages[m_title]++;
                newPage();
            }
  
}

void Processor::displayLyrics()
{
    QRegularExpression CommentRex("^ *\\{(?:comment|c): *([^}]*) *\\}",QRegularExpression::CaseInsensitiveOption);
    QRegularExpression CommentItalicRex("^ *\\{(?:comment-italic|ci): *([^}]*) *\\}",QRegularExpression::CaseInsensitiveOption);
    QRegularExpression CommentBoxRex("^ *\\{(?:comment-box|cb): *([^}]*) *\\}",QRegularExpression::CaseInsensitiveOption);
    QRegularExpressionMatch CommentRexMatch,CommentItalicRexMatch,CommentBoxRexMatch;

    BufLyricsNormailisation();
    if ( m_firstline )
    {
        m_firstline=false;
        m_line-=m_uiconfig->toolButtonNormalFont->getFont().pointSizeF()*3;
    }
    
    foreach (QString text,m_BufLyrics)
    {
        if ( ! text.isEmpty() )
        {
            if ( m_firstlyric ) {
                     m_firstlyric=false;
                     m_initiallyric=m_line;
             }
            text.replace(QRegularExpression("\\[[^]]+\\]"),"") ;
            if ( isColBreak(text))
                doColumnBreak(text);
            else if ( text.contains(CommentRex,&CommentRexMatch))
            {
                 Text(m_document,CommentRexMatch.captured(1),m_column,m_line,m_uiconfig->toolButtonCommentFont);
                  m_CurrentString=text;NextLine();
            }
            else if ( text.contains(CommentItalicRex,&CommentItalicRexMatch))
            {
                Text(m_document,CommentItalicRexMatch.captured(1),m_column,m_line,m_uiconfig->toolButtonCommentItalicFont);
                  m_CurrentString=text;NextLine();
            }
            else if ( text.contains(CommentBoxRex,&CommentBoxRexMatch))
            {
                TextInBox(m_document,CommentRexMatch.captured(1),m_column,m_line,m_uiconfig->toolButtonCommentBoxFont);
                  m_CurrentString=text;NextLine();
            }
            else
            {
                Text(m_document,text,m_column,m_line,m_uiconfig->toolButtonNormalFont);
                m_CurrentString=text;
                NextLine();
            }
        }
        else if (! m_firstlyric)
           {
              m_CurrentString=text;
              NextLine();
           }
    }
    m_firstlyric=true;
    m_initiallyric=0;
}


bool Processor::isColBreak(QString line)
{
    return ( line.contains(QRegularExpression("^ *\\{(column_break|colb)",QRegularExpression::CaseInsensitiveOption)));
}

void Processor::NextLine(int num )
{
   double verticalspace=m_uiconfig->comboBoxVerticalSpacing->currentData().toDouble();
    qreal fontSize=m_uiconfig->toolButtonNormalFont->getFont().pointSizeF();
// test if on bottom on page
   if (  m_line - 2*fontSize*verticalspace-2 * num >=
         m_copyoffset *( m_pausenumber -1)+ m_uiconfig->spuVerticalMargin->getPdfU() )
    {
       // we are not on the page bottom we can decrease m_line
        m_line=m_line-fontSize*verticalspace-num;
    }
    else 
    {
    int currentcol=currentColumn() ;
    if ( m_colnumber >= 2 &&  currentcol < m_colnumber )
   {
       // we are at the bottom of the page and we go on the next page of song
        m_column = nextColumn(currentcol) ;
        
       if ( m_pagecontinued > 0 )
       {
           m_line= static_cast<int>(m_uiconfig->spuPageHeight->getPdfU()-m_uiconfig->spuVerticalMargin->getPdfU());
           m_firstlyric=true;
       }
       else
       {
           m_line=m_initiallyric;
           m_firstlyric=true;

       }
   }
// force next page
    else
    {
        if (m_tocpages.count() != 0)
        {
            m_tocpages[m_title]++;
        }
        newPage();
    }
    }
}
 


int Processor::currentColumn()
{
  int c=1;
  while (c * m_uiconfig->spuPageWidth->getPdfU()/m_colnumber < m_column ) c++;
  return c;
}

int Processor::nextColumn( int colnumber)
{
    return ( static_cast<int>( m_uiconfig->spuPageWidth->getPdfU()*(colnumber)/m_colnumber +m_uiconfig->spuHorizontalMargin->getPdfU()/4));
}

void Processor::Cover(QString title, QString subtitle, QString version)

{
   try{
        AllocatePage();

        PdfFont *pfont=m_document->CreateFont(m_uiconfig->toolButtonCoverFont->getFont().family().toLatin1());
        QString image=QDir(m_projectpath+"/"+m_uiconfig->toolButtonCoverImage->getImage()).canonicalPath();
        QColor backgroundcolor=QColor(m_uiconfig->toolButtonCoverFont->getBackgroundColor());
        m_painter.SetColor(backgroundcolor.redF(),backgroundcolor.greenF(),backgroundcolor.blueF());
        m_painter.Rectangle(0,0,m_uiconfig->spuPageWidth->getPdfU(),m_uiconfig->spuPageHeight->getPdfU());
        m_painter.Fill(true);
        if (! image.isEmpty())
          {
            QPixmap pix(image);
            if (! pix.isNull())
            {
                double maxh=m_uiconfig->spuPageHeight->getPdfU()/m_pausenumber;
                double maxw=m_uiconfig->spuPageWidth->getPdfU()*0.75;
                double imageh=pix.size().height();
                double imagew=pix.size().width();
                double scale1=maxh/imageh;
                double scale2=maxw/imagew;
                double scale=scale1>scale2?scale2:scale1;
                int x0=static_cast<int>((m_uiconfig->spuPageWidth->getPdfU()-maxw)/2.0);

                PdfImage *pdfi = new PdfImage(m_document);
                pdfi->SetImageChromaKeyMask(0,0,0);
                pdfi->LoadFromFile(image.toLatin1());
                for ( int pause=0; pause<m_pausenumber;pause++)
                  m_painter.DrawImage(x0,
                   m_uiconfig->spuPageHeight->getPdfU()-m_uiconfig->spuVerticalMargin->getPdfU()-m_copyoffset*(pause+1)+ImagePosition(imageh*scale)
                                      ,pdfi,scale,scale);
            }
          }

          double posx=m_uiconfig->spuPageWidth->getPdfU()/2;
          double posy=TitlePosition();
          double x2=Text(m_document,title,posx,m_uiconfig->spuPageHeight->getPdfU()-posy,m_uiconfig->toolButtonCoverFont,center);
          pfont->SetFontSize(static_cast<float>(m_uiconfig->toolButtonCoverFont->getFont().pointSize()*0.5));
          pfont->SetUnderlined(m_uiconfig->toolButtonCoverFont->getFont().underline());
          pfont->SetStrikeOut(m_uiconfig->toolButtonCoverFont->getFont().strikeOut());
          double widthtext=pfont->GetFontMetrics()->StringWidth(subtitle.toLatin1());
          Text(m_document,subtitle,x2-widthtext/2,
                 m_uiconfig->spuPageHeight->getPdfU()-posy-m_uiconfig->toolButtonCoverFont->getFont().pointSize()/m_pausenumber,
                 m_uiconfig->toolButtonCoverFont,right,0.5);
          Text(m_document,version,20,m_uiconfig->spuPageHeight->getPdfU()-m_copyoffset+10,m_uiconfig->toolButtonCoverFont,center,0.2);
          if ( m_uiconfig->comboBoxDuplex->currentIndex()>0)
              newPage();
     }
    catch (const  PoDoFo::PdfError& e)
    {
        throw e;
    }
}


void Processor::AllocatePage()
{
    try{
    m_pageAllocation=true;
    m_page= m_document->CreatePage(*m_dimension);
    m_painter.SetPage(m_page);
   }
    catch (const PoDoFo::PdfError & e)
    {
        throw e;
    }
}

void Processor::doChords()
{

}

void Processor::displayChordsForSong()
{
}

void Processor::setPageBreak()
{

}

void Processor::setColBreak(QString line)
{
       m_BufLyrics<<line;

}

void Processor::save()
{
    if ( m_pageAllocation)
    {
        m_painter.FinishPage();
    }
    if ( m_documentAllocation)
    {
       m_document->Close();
    }
}

void Processor::savemem()
{
    try {
       //int i= m_mdocument->GetPageCount();
       QFile  file(m_file);
       file.remove();
       m_mdocument->Write(m_file.toStdString().c_str());
    }
    catch (const PoDoFo::PdfError &e)
    {
        throw(e);
    }
}
void Processor::openExistingFile()
{
   try {
    m_mdocument = new PdfMemDocument(m_file.toStdString().c_str());
    }
    catch (const PoDoFo::PdfError &e)
    {
        throw(e);
    }
}

void Processor::addFooter()
{

}

int Processor::TocColSize()
{
    if ( m_uiconfig->comboBoxTocColumnNumber->currentIndex() == 0 ) return  static_cast<int>(m_uiconfig->spuPageWidth->getPdfU()-2*m_uiconfig->spuHorizontalMargin->getPdfU())  ;
    else if ( m_uiconfig->comboBoxTocColumnNumber->currentIndex() == 1 )  return  static_cast<int>(m_uiconfig->spuPageWidth->getPdfU()/2-m_uiconfig->spuHorizontalMargin->getPdfU()) ;
    else return static_cast<int>( m_uiconfig->spuPageWidth->getPdfU()/3-m_uiconfig->spuHorizontalMargin->getPdfU()) ;
}

void Processor::addLinkInToc()
{

}


int Processor::NbPagesInToc(int nbpages)
{
    bool ok ;
    // we multiply the number of page by number of pause per page
    nbpages*=(m_uiconfig->comboBoxVerticalPausePerPage->currentIndex()+1);
    double verticalspacing=m_uiconfig->comboBoxTocVerticalSpacing->currentData().toDouble(&ok);
    if ( !ok ) verticalspacing=1.2;
    double heightneed=nbpages*(verticalspacing+1)
            *m_uiconfig->toolButtonTocFont->getFont().pointSizeF()/
            m_uiconfig->comboBoxTocColumnNumber->currentText().toInt();
    double nbpagesneed=heightneed/(m_uiconfig->spuPageHeight->getPdfU() - m_uiconfig->toolButtonTitleFont->getFont().pointSizeF()*2.4
                        - 2*m_uiconfig->spuVerticalMargin->getPdfU() );
    int nb= static_cast<int>(nbpagesneed+1);
    // if the toc has many column we have to divide the number of page of Toc
    nb=nb/(m_uiconfig->comboBoxTocColumnNumber->currentIndex()+1);
    if (nb % 2 == 1 && m_uiconfig->comboBoxDuplex->currentIndex()>0) nb++;
     
    return nb;
}

int Processor::FirstPageNumber()
{
    if ( m_covermade )
        if ( m_uiconfig->comboBoxDuplex->currentIndex()>0)
            return 3;
        else
            return 2;
    else
    return 1;
}


int Processor::NbPageCover()
{
    if ( m_covermade )
    if ( m_uiconfig->comboBoxDuplex->currentIndex()>0)
            return 2;
        else
            return 1;
    else
        return 0;

}

void Processor::memorizeChords(QString line)
{
    QRegularExpression chordEx("\\[([^]]*)\\]");
    QRegularExpressionMatch chordExMatch;
    while ( line.indexOf(chordEx,0,&chordExMatch)!=-1)
    {
        m_BufChords<<chordExMatch.captured(1);
        line.replace(QString("[%1]").arg(chordExMatch.captured(1)),"");
    }
}

void Processor::addTocAtBegining()
{   int pageintoc=1;
    try {
    if ( !m_pageAllocation)
    {
       m_page = m_document->CreatePage(*m_dimension);
       m_painter.SetPage(m_page);
       m_pageAllocation=true;
    }
    pageintoc++;
    PdfPage *toc;
    int pagenumber=0;
    int position =FirstPageNumber();
    
    m_positiontoc=position-1;
    m_line= static_cast<int>(m_uiconfig->spuPageHeight->getPdfU()
                            -m_uiconfig->toolButtonTitleFont->getFont().pointSizeF() 
                            - m_uiconfig->spuVerticalMargin->getPdfU());

    toc=m_document->InsertPage(*m_dimension,position-1);

    pagenumber++;
    m_TocPages<<toc->GetContents();
    m_painter.SetPage(toc);
    bool ok;
    double verticalspacing=m_uiconfig->comboBoxTocVerticalSpacing->currentData().toDouble(&ok);
    if ( !ok ) verticalspacing=1.2;
    Text(m_document,QObject::tr("Table of content"),m_uiconfig->spuPageWidth->getPdfU()/2,m_line,m_uiconfig->toolButtonTocFont,center,2);
    m_line-=m_uiconfig->toolButtonTitleFont->getFont().pointSizeF()*2.4 ;
    int lineinit=m_line;
    int colinit= static_cast<int>(m_uiconfig->spuHorizontalMargin->getPdfU());
    int currentcol=0;
    int titlenumber=0;
    int nbpagesintoc=1;
    nbpagesintoc=NbPagesInToc(m_nbrealpages);
    foreach ( QString title, m_toc)
        {
         // rect is the zone clickable
         int page=pagenumber+nbpagesintoc-1;
         //modif
         PdfRect rect=LineToc(title,
                             TocColSize()-m_uiconfig->spuHorizontalMargin->getPdfU(),
                             colinit,
                             m_line,
                             m_uiconfig->toolButtonTocFont,page+1);
         PdfAnnotation *a=toc->CreateAnnotation(ePdfAnnotation_Link,rect);
         //a->SetContents(tr("Go to page %1").arg(pagenumber+nbpagesintoc-2).toStdString().c_str());
         // manque a->setContents
         PdfDestination dest(m_document->GetPage(page-nbpagesintoc+position - (m_covermade?0:1)));
         a->SetDestination(dest);
         a->SetFlags( ePdfAnnotationFlags_Hidden);
         pagenumber+=m_tocpages[title];
         // Can we add a new line in toc  without creating new page ?
         if ( m_line - 
         
             m_uiconfig->toolButtonTocFont->getFont().pointSizeF()*verticalspacing 
             - 2* m_uiconfig->toolButtonPageNumberFont->getFont().pointSizeF()
              >
             m_copyoffset*(m_pausenumber-1) + m_uiconfig->spuVerticalMargin->getPdfU())
             // We can add a new line in toc 
             m_line-=m_uiconfig->toolButtonTocFont->getFont().pointSizeF()*verticalspacing;
         // Can we add a new column ?    
         else if ( currentcol + 1 <= m_uiconfig->comboBoxTocColumnNumber->currentIndex() )
         // We add a column
         {
             currentcol++;
             m_line=lineinit;
             colinit=colinit+TocColSize();
         }
         else
         // We need to add a page in Toc 
         {
             m_line=lineinit;
             currentcol=0;
             colinit= static_cast<int>(m_uiconfig->spuHorizontalMargin->getPdfU());
             m_tocpages[m_title]++;
             if ( m_pageAllocation)  FinishPage(&m_painter);
             toc=m_document->InsertPage(*m_dimension,position);
             position++;
             m_painter.SetPage(toc);
             m_column= static_cast<int>(m_uiconfig->spuHorizontalMargin->getPdfU());
             m_TocPages<<toc->GetContents();
             pageintoc++;
         }
         titlenumber++;
    }
    
    if (pageintoc % 2 == 1 && m_uiconfig->comboBoxDuplex->currentIndex()>0)
     {//   newPage();
        FinishPage(&m_painter);
    }
    else 
    {
      if ( m_pageAllocation)  FinishPage(&m_painter);
      toc=m_document->InsertPage(*m_dimension,position);
      m_painter.SetPage(toc);
    }
    }
     catch (const PoDoFo::PdfError &e)
    {
    throw(e);
   }
   
   

}

void Processor::addTocAtEnd()
{
    try{
    if ( !m_pageAllocation)
    {
       m_page = m_document->CreatePage(*m_dimension);
       m_painter.SetPage(m_page);
       m_pageAllocation=true;
    }
    PdfPage *toc;
    int pagenumber=1;
    m_positiontoc=m_document->GetPageCount();
    m_line= static_cast<int>(m_uiconfig->spuPageHeight->getPdfU()- 
                             m_uiconfig->toolButtonSubtitleFont->getFont().pointSizeF() - 
                             m_uiconfig->spuVerticalMargin->getPdfU());
    toc=m_document->CreatePage(*m_dimension);
    pagenumber++;
    m_TocPages<<toc->GetContents();
    m_painter.SetPage(toc);
    bool ok;
    double verticalspacing=m_uiconfig->comboBoxTocVerticalSpacing->currentData().toDouble(&ok);
    if ( !ok ) verticalspacing=1.2;
    Text(m_document,QObject::tr("Table of content"),m_uiconfig->spuPageWidth->getPdfU()/2,m_line,m_uiconfig->toolButtonTocFont,center,2);
    m_line-=m_uiconfig->toolButtonTitleFont->getFont().pointSizeF()*2.4 ;
    int lineinit=m_line;
    int colinit= static_cast<int>(m_uiconfig->spuHorizontalMargin->getPdfU());
    int currentcol=0;
    int titlenumber=0;
//    int nbpagesintoc=1;
//    nbpagesintoc=NbPagesInToc(m_nbrealpages);
    foreach ( QString title, m_toc)
        {
         // rect is the zone clickable
         PdfRect rect=LineToc(title,
                             TocColSize()-m_uiconfig->spuHorizontalMargin->getPdfU(),
                             colinit,
                             m_line,
                             m_uiconfig->toolButtonTocFont,pagenumber-1);
         PdfAnnotation *a=toc->CreateAnnotation(ePdfAnnotation_Link,rect);
     //    a->SetContents(tr("Go to page %1").arg(pagenumber+FirstPageNumber()).toStdString().c_str());
         a->SetContents(QString("Go to page 11").toStdString().c_str());

         PdfDestination dest(m_document->GetPage(pagenumber-1+NbPageCover()));
         a->SetDestination(dest);
         a->SetFlags( ePdfAnnotationFlags_Hidden);
         pagenumber+=m_tocpages[title];
         if ( m_line - 
             m_uiconfig->toolButtonTocFont->getFont().pointSizeF()*verticalspacing
             - 2* m_uiconfig->toolButtonPageNumberFont->getFont().pointSizeF()
              >
             m_copyoffset*(m_pausenumber-1) + m_uiconfig->spuVerticalMargin->getPdfU())
                 m_line-=m_uiconfig->toolButtonTocFont->getFont().pointSizeF()*verticalspacing;
         else if ( m_uiconfig->comboBoxTocColumnNumber->currentIndex() >= currentcol +1  )
         
         
         {
             currentcol++;
             m_line=lineinit;
             colinit=colinit+TocColSize();
         }
         else
         {
             m_line=lineinit;
             currentcol=0;
             colinit= static_cast<int>(m_uiconfig->spuHorizontalMargin->getPdfU());
             m_tocpages[m_title]++;
             if ( m_pageAllocation)  FinishPage(&m_painter);
             toc=m_document->CreatePage(*m_dimension);
             m_painter.SetPage(toc);
             m_column= static_cast<int>(m_uiconfig->spuHorizontalMargin->getPdfU());
             m_TocPages<<toc->GetContents();
         }
         titlenumber++;
    }
    FinishPage(&m_painter);
    }
     catch (const PoDoFo::PdfError &e)
   {
    throw(e);
   }

}

void Processor::addToc()
{
    if ( m_uiconfig->comboBoxTocPosition->currentIndex()==0)  FinishPage(&m_painter);
    else if ( m_uiconfig->comboBoxTocPosition->currentIndex()==1) addTocAtBegining();
    else addTocAtEnd();

}

void Processor::makePageNumber()
{
    try{
    bool arrow = false;
    Constants::PageNumber pagetype= Constants::getPageNumber(m_uiconfig->comboBoxPageNumber->currentIndex());
    if (pagetype == Constants::No )  return ;

    int firstpage=FirstPageNumber();
    int totalpage=m_mdocument->GetPageCount()-firstpage+1;
    if (totalpage<=0) return;
    int nbpage;
//    if ( m_uiconfig->comboBoxDuplex->currentIndex()>0)
//        nbpage=-1;
//    else nbpage=1;
    nbpage=1;
    // make pattern
    for ( int p=firstpage-1; p<=totalpage+1 ; p++)
    {
        if ( nbpage > totalpage ) break;
        PdfPage *pdfp = m_mdocument->GetPage(p);
        m_painter.SetPage(pdfp);
        QString pattern;
        if ( m_uiconfig->comboBoxPageNumberStyle->currentIndex()==Constants::Number)
            pattern="%1";
        else if ( m_uiconfig->comboBoxPageNumberStyle->currentIndex()==Constants::NumberAndTiret )
            pattern="- %1 -";
        else if ( m_uiconfig->comboBoxPageNumberStyle->currentIndex()==Constants::NumberDivideByPageNUmber )
           {
            QString t=QString("%1").arg(totalpage);
            pattern=QString("%1/")+t;
           }
        else if ( m_uiconfig->comboBoxPageNumberStyle->currentIndex()==Constants::NumberAndArrows )
        {
            pattern=QString("%1");
            arrow=true;
        }
        else pattern=QString("%1");
        QString page=QString(pattern).arg(nbpage);
        double  widthtext=m_uiconfig->toolButtonPageNumberFont->getFont().pointSizeF()*page.length();
        int x=0;
        int y=static_cast<int>(m_uiconfig->spuHorizontalMargin->getPdfU()-m_uiconfig->spuPageNumberPosition->getPdfU());
//        if (  m_uiconfig->spuHorizontalMargin->getPdfU() < m_uiconfig->toolButtonPageNumberFont->getFont().pointSizeF() )
//            y= static_cast<int>(m_uiconfig->toolButtonPageNumberFont->getFont().pointSizeF()*3);
//        else
//          y =   static_cast<int>(m_uiconfig->spuHorizontalMargin->getPdfU()-m_uiconfig->toolButtonPageNumberFont->getFont().pointSizeF()) ;
        if ( pagetype == Constants::Center)
            x = static_cast<int>( m_uiconfig->spuPageWidth->getPdfU()/2.0);
        else if ( m_uiconfig->comboBoxPageNumberStyle->currentIndex()==Constants::Outside )
        {  if ( m_uiconfig->comboBoxDuplex->currentIndex() ==Constants::Simplex)
              x = static_cast<int>(m_uiconfig->spuPageWidth->getPdfU()-m_uiconfig->spuHorizontalMargin->getPdfU());
           else if ( nbpage %2 == 0 )
               x = static_cast<int>(m_uiconfig->spuPageWidth->getPdfU()-m_uiconfig->spuHorizontalMargin->getPdfU());
           else
               x =  static_cast<int>(m_uiconfig->spuHorizontalMargin->getPdfU());
        }
        if ( m_uiconfig->toolButtonPageNumberFont->getBackgroundColor() != m_uiconfig->colorButtonPaperColor->getColor() )
        {
            m_painter.SetColor(m_uiconfig->toolButtonPageNumberFont->getBackgroundColor().redF(),m_uiconfig->toolButtonPageNumberFont->getBackgroundColor().greenF(),m_uiconfig->toolButtonPageNumberFont->getBackgroundColor().blueF());
            m_painter.Rectangle(m_uiconfig->spuHorizontalMargin->getPdfU(),
                              y-m_uiconfig->toolButtonTocFont->getFont().pointSize()/3.0,
                              m_uiconfig->spuPageWidth->getPdfU()-2*m_uiconfig->spuHorizontalMargin->getPdfU(),
                              m_uiconfig->toolButtonTocFont->getFont().pointSize()*1.3);
            m_painter.Fill();
        }
        if  ( pagetype == Constants::Center )
        {

            if ( arrow )
            {
              for ( int pause=0; pause<m_pausenumber;pause++)
                 Text(m_mdocument,page,x,pause*m_copyoffset+m_uiconfig->spuPageNumberPosition->getPdfU(),m_uiconfig->toolButtonPageNumberFont,left);
              if (nbpage > 1)
              {
                m_painter.SetColor(m_uiconfig->toolButtonPageNumberFont->getTextColor().redF(),m_uiconfig->toolButtonPageNumberFont->getTextColor().greenF(),m_uiconfig->toolButtonPageNumberFont->getTextColor().blueF());
                m_painter.MoveTo(x-m_uiconfig->toolButtonTocFont->getFont().pointSize(),y+0.4*m_uiconfig->toolButtonTocFont->getFont().pointSize());
                m_painter.LineTo(x-m_uiconfig->toolButtonTocFont->getFont().pointSize()/2,y+0.8*m_uiconfig->toolButtonTocFont->getFont().pointSize());
                m_painter.LineTo(x-m_uiconfig->toolButtonTocFont->getFont().pointSize()/2,y);
                m_painter.ClosePath();
                m_painter.Fill();
                PdfRect rect1(x-1.5*m_uiconfig->toolButtonTocFont->getFont().pointSize(),y,m_uiconfig->toolButtonTocFont->getFont().pointSize(),m_uiconfig->toolButtonPageNumberFont->getFont().pointSizeF());
                PdfAnnotation *a1=pdfp->CreateAnnotation(ePdfAnnotation_Link,rect1);
                a1->SetContents(tr("Go to last page").toStdString().c_str());
                PdfDestination dest1(m_document->GetPage(nbpage-1));
                a1->SetDestination(dest1);
                a1->SetFlags( ePdfAnnotationFlags_Hidden);
              }
              if (nbpage < totalpage)
              {
                m_painter.SetColor(m_uiconfig->toolButtonPageNumberFont->getTextColor().redF(),m_uiconfig->toolButtonPageNumberFont->getTextColor().greenF(),m_uiconfig->toolButtonPageNumberFont->getTextColor().blueF());
                m_painter.MoveTo(x+widthtext+m_uiconfig->toolButtonTocFont->getFont().pointSize()/2,y+0.4*m_uiconfig->toolButtonTocFont->getFont().pointSize());
                m_painter.LineTo(x+widthtext,y+0.8*m_uiconfig->toolButtonTocFont->getFont().pointSize());
                m_painter.LineTo(x+widthtext,y);
                m_painter.ClosePath();
                m_painter.Fill();
                PdfRect rect2(x+widthtext,y,m_uiconfig->toolButtonTocFont->getFont().pointSize(),m_uiconfig->toolButtonPageNumberFont->getFont().pointSizeF());
                PdfAnnotation *a2=pdfp->CreateAnnotation(ePdfAnnotation_Link,rect2);
                a2->SetContents(tr("Go to next page").toStdString().c_str());
                PdfDestination dest2(m_document->GetPage(nbpage+1));
                a2->SetDestination(dest2);
                a2->SetFlags( ePdfAnnotationFlags_Hidden);
              }
            }
            else
                for ( int pause=0; pause<m_pausenumber;pause++)
                Text(m_mdocument,page,x,pause*m_copyoffset+m_uiconfig->spuPageNumberPosition->getPdfU(),m_uiconfig->toolButtonPageNumberFont,center);
        }
        else
            for ( int pause=0; pause<m_pausenumber;pause++)
            Text(m_mdocument,page,x,pause*m_copyoffset+m_uiconfig->spuPageNumberPosition->getPdfU(), m_uiconfig->toolButtonPageNumberFont,left);
        nbpage++;
    if (m_uiconfig->comboBoxTocPosition->currentIndex() != Constants::No)
    {
            PdfRect rect(x-m_uiconfig->toolButtonPageNumberFont->getFont().pointSizeF()/2,y,1.5*widthtext,m_uiconfig->toolButtonPageNumberFont->getFont().pointSizeF());
            PdfAnnotation *a=pdfp->CreateAnnotation(ePdfAnnotation_Link,rect);
            a->SetContents(tr("Go to table of content").toStdString().c_str());
            PdfDestination dest(m_document->GetPage(m_positiontoc));
            a->SetDestination(dest);
            a->SetFlags( ePdfAnnotationFlags_Hidden);
     }
        FinishPage(&m_painter);
    }
    }
     catch (const PoDoFo::PdfError &e)
   {
    throw(e);
   }
}

PdfRect Processor::PageSize( double left, double bottom, double width, double height )
{
 try {
    PdfRect size(left,bottom,width,height);
    return size;
  }
  catch (const PoDoFo::PdfError &e)
   {
     throw(e);
    }
}

void Processor::FollowingLine()
{

}

QString Processor::Category()
{
    return QString("TextBook");
}


double Processor::mm(int value)
{
    return (value*72/25.4);
}


double Processor::TitlePosition()
{
    double factor;
    if ( m_uiconfig->comboBoxTitlePosition->currentIndex()==0) factor=1/4.0;
    else if ( m_uiconfig->comboBoxTitlePosition->currentIndex()==1) factor=1/2.0;
    else factor=3/4.0;
   // return m_uiconfig->spuPageHeight->getPdfU()*factor;
    return m_copyoffset*factor;
}

double Processor::ImagePosition(int size)
{
    double factor;
    if ( m_uiconfig->comboBoxTitlePosition->currentIndex()==0) factor=1/4.0;
    else if ( m_uiconfig->comboBoxTitlePosition->currentIndex()==1) factor=1/4.0;
    else factor=3/4.0;
    return factor*(m_copyoffset-size);
}

double Processor::TextInBox(PdfDocument *doc, QString text, double x, double y, FontButton *fb ,Align align, double scale)
{
    try{
    if ( !m_pageAllocation)
    {
       m_page = m_document->CreatePage(*m_dimension);
       m_painter.SetPage(m_page);
       m_pageAllocation=true;
    }
    PdfFont *pfont=doc->CreateFont(fb->getFont().family().toLatin1());
    PdfString str(text.toLatin1());
    double fontsize=fb->getFont().pointSize()*scale;
    pfont->SetFontSize( static_cast<float>(fontsize));
    pfont->SetUnderlined(fb->getFont().underline());
    pfont->SetStrikeOut(fb->getFont().strikeOut());
    double widthtext=pfont->GetFontMetrics()->StringWidth(text.toLatin1())*scale;
    double heighttext= pfont->GetFontSize()*1.1;
    m_painter.SetColor(fb->getBackgroundColor().redF(),fb->getBackgroundColor().greenF(),fb->getBackgroundColor().blueF());
    double vs=m_uiconfig->comboBoxVerticalSpacing->currentData().toDouble();
    m_painter.Rectangle(x-widthtext*0.1,y-heighttext*0.2,widthtext*vs,heighttext*vs);
    m_painter.Fill();
    return Text(doc,text,x,y,fb,align,scale);
    }
     catch (const PoDoFo::PdfError &e)
   {
    throw(e);
   }

}


double  Processor::Text( PdfDocument *doc, QString text, double x, double y, FontButton *fb ,Align align, double scale)
{
    try{
    if ( !m_pageAllocation)
    {
       m_page = m_document->CreatePage(*m_dimension);
       m_painter.SetPage(m_page);
       m_pageAllocation=true;
    }
    double end=0;
    PdfFont *pfont=doc->CreateFont(fb->getFont().family().toLatin1());
    PdfString str(text.toLatin1());
    double fontsize=fb->getFont().pointSize()*scale;
    pfont->SetFontSize(static_cast<int>(fontsize));
    pfont->SetUnderlined(fb->getFont().underline());
    pfont->SetStrikeOut(fb->getFont().strikeOut());
    double widthtext=pfont->GetFontMetrics()->StringWidth(str);
    while ( widthtext >m_uiconfig->spuPageWidth->getPdfU() )
    {
        fontsize=fontsize*0.9;
        pfont->SetFontSize(static_cast<int>(fontsize));
        widthtext=pfont->GetFontMetrics()->StringWidth(str);
    }
    m_painter.SetFont(pfont);
    m_painter.SetColor(fb->getTextColor().redF(),fb->getTextColor().greenF(),fb->getTextColor().blueF());
    m_painter.Fill(true);
    if (! text.isEmpty())
    {
        if ( align == right )  x-=widthtext*scale;
        else if ( align == center ) x-=widthtext*scale/2;
        else if (align == left ) {}
        for ( int copy   = 0; copy < m_pausenumber ; copy ++ )
       {
        int h=static_cast<int>(y-copy*m_copyoffset);
        m_painter.DrawText(x,h,str);
       }
       end=x+widthtext;
    }
    return end;
    }
     catch (const PoDoFo::PdfError &e)
   {
    throw(e);
    }
}




double Processor::TextRotated(PoDoFo::PdfDocument *doc, QString text, double x, double y, FontButton *fb)
{
    if ( !m_pageAllocation)
    {
       m_page = m_document->CreatePage(*m_dimension);
       m_painter.SetPage(m_page);
       m_pageAllocation=true;
    }
    
    double end=0;
    PdfFont *pfont=doc->CreateFont(fb->getFont().family().toLatin1());
    PdfString str(text.toLatin1());
    double fontsize=fb->getFont().pointSize();
    pfont->SetFontSize(static_cast<int>(fontsize));
    pfont->SetUnderlined(fb->getFont().underline());
    pfont->SetStrikeOut(fb->getFont().strikeOut());
   double widthtext=pfont->GetFontMetrics()->StringWidth(str);
    while ( widthtext >m_uiconfig->spuPageWidth->getPdfU() )
    {
        fontsize=fontsize*0.9;
        pfont->SetFontSize(static_cast<int>(fontsize));
        widthtext=pfont->GetFontMetrics()->StringWidth(str);
    }
    m_painter.SetFont(pfont);
    m_painter.SetColor(fb->getTextColor().redF(),fb->getTextColor().greenF(),fb->getTextColor().blueF());
    m_painter.Fill(true);
    m_painter.Save();
    m_painter.SetTransformationMatrix(1,0,0,1,x,y);
    m_painter.SetTransformationMatrix(0,1,-1,0,0,0);
    m_painter.DrawText(0,0,str);
    m_painter.Restore();
       
    return end;
}


PdfRect Processor::LineToc(QString text, double width, double x, double y, FontButton *fb, int pagenumber)
{
    try {
    if ( !m_pageAllocation)
    {
       m_page = m_document->CreatePage(*m_dimension);
       m_painter.SetPage(m_page);
       m_pageAllocation=true;
    }
    QRegularExpression space("^ +");
    text.replace(space,"").append(" ");
    PdfFont *pfont=m_document->CreateFont(fb->getFont().family().toLatin1());
    pfont->SetFontSize(fb->getFont().pointSize());
    pfont->SetUnderlined(fb->getFont().underline());
    pfont->SetStrikeOut(fb->getFont().strikeOut());
    m_painter.SetFont(pfont);
    QString point(" ");
    bool odd=true;
    while ( pfont->GetFontMetrics()->StringWidth(QString("%1%2    ").arg(text,point).toLatin1())> width)
    {
        text.chop(1);
    }
    while ( pfont->GetFontMetrics()->StringWidth(QString("%1%2    ").arg(text,point).toLatin1())<= width)
    {
        odd=odd?false:true;
        QString c;
        if ( m_uiconfig->comboBoxTocSpaceCharacter->currentIndex()==0 ) c=".";
        else if ( m_uiconfig->comboBoxTocSpaceCharacter->currentIndex()==1 ) c="-";
        else c=" ";
        if ( odd ) point+=c;
        else point+=" ";
    }
    text=QString("%1%2    ").arg(text,point);
    m_painter.SetColor(fb->getTextColor().redF(),fb->getTextColor().greenF(),fb->getTextColor().blueF());
    m_painter.Fill(true);
    for ( int pause=0; pause < m_pausenumber ; pause ++ )
    {
       int h= static_cast<int>(y-pause*m_copyoffset);
        m_painter.DrawText(x,h,PdfString(text.toLatin1()));
    }
    PdfRect rect(x,y,x+width+fb->getFont().pointSizeF(),fb->getFont().pointSizeF());
    Text(m_document,QString("%1").arg(pagenumber)
         ,x+width+fb->getFont().pointSize(),
         y,fb,right);
    return rect;
    }
     catch (const PoDoFo::PdfError &e)
   {
    throw(e);
   }
}


void Processor::FinishPage(PdfPainter *painter)
{
    try{
    QString watermark=m_uimainwindow->lineEditWatermark->text();
    watermark=QString("          %1          ").arg(watermark);
    if ( ! watermark.isEmpty() )
    {
        painter->Save();

        PdfFont *pfont=m_document->CreateFont(m_uiconfig->toolButtonTitleFont->getFont().family().toLatin1());

        PdfExtGState state(m_document);
        state.SetFillOpacity(static_cast<float>(0.4));
        painter->SetExtGState(&state);
        PdfString str(watermark.toLatin1());
        double lenght=sqrt(qPow(m_uiconfig->spuPageHeight->getPdfU(),2)+qPow(m_uiconfig->spuPageWidth->getPdfU(),2));
        while (pfont->GetFontMetrics()->StringWidth(str) < lenght )
        {
            pfont->SetFontSize(pfont->GetFontSize()+1);
        }
        painter->SetFont(pfont);
        painter->SetColor(0.90,0.90,0.90);
        painter->Fill(true);
        double angle=qAtan(m_uiconfig->spuPageHeight->getPdfU()/m_uiconfig->spuPageWidth->getPdfU());
        double sinA=sin(angle);
        double cosA=cos(angle);
        painter->SetTransformationMatrix(cosA,sinA,-sinA,cosA,0,0);
        painter->DrawText(0,0,PdfString(watermark.toLatin1().toStdString()));
        painter->Restore();
    }
    if ( m_pausenumber > 1)
        for ( int pause =1; pause <m_pausenumber ; pause ++)
        {
            painter->Save();
            painter->SetColor(0.2,0.20,0.20);
            painter->SetStrokeWidth(0.001);
            painter->DrawLine(0,pause*m_copyoffset,20,pause*m_copyoffset );
            painter->DrawLine(m_uiconfig->spuPageWidth->getPdfU()-20,pause*m_copyoffset,m_uiconfig->spuPageWidth->getPdfU(),pause*m_copyoffset );
            painter->Restore();
        }

    painter->FinishPage();
    }
     catch (const PoDoFo::PdfError &e)
   {
    throw(e);
   }
}



void Processor::displayChord(QString ch,int &line, int column,int size, QString lang)
{
    try {
    class Chord chord(ch,lang);
    int steph=size/8;
    int stepv=size/10 ;

//   1 2 3 4 5 6 7 8
//1        0
//2      ===========
//3       o
//4      -----------
//5
//6      -----------
//7
//8      -----------
//9
//10     -----------



    int sizecircle=steph/3;
    int sizenonote=steph/4;
    int sizefont=steph;

    if ( !m_pageAllocation)
    {
       m_page = m_document->CreatePage(*m_dimension);
       m_painter.SetPage(m_page);
       m_pageAllocation=true;
    }
    PdfFont *pfont=m_document->CreateFont(m_uiconfig->toolButtonChordFont->getFont().family().toLatin1());
    pfont->SetFontSize(sizefont);
    pfont->SetUnderlined(m_uiconfig->toolButtonChordFont->getFont().underline());
    pfont->SetStrikeOut(m_uiconfig->toolButtonChordFont->getFont().strikeOut());
    m_painter.SetFont(pfont);
    QString fret=chord.fret();
    // print fret number
    if (  fret!= "0") m_painter.DrawText(column+steph,line-stepv*3,PdfString(chord.fret().toLatin1()));
    // print chord name
    m_painter.DrawTextAligned(column+size/2,line-stepv,chord.nameLocale().length(),PdfString(chord.nameLocale().toLatin1()), EPdfAlignment::ePdfAlignment_Center);
    // print first fret
    m_painter.SetStrokeWidth(2);
    m_painter.SetLineCapStyle(ePdfLineCapStyle_Round);
    m_painter.DrawLine(column+2*steph,line-2*stepv+1,column+7*steph,line-2*stepv+1);
    m_painter.SetStrokeWidth(0.5);
    // print 4 fret
    for ( int i=0; i<=3;i++) m_painter.DrawLine(column+2*steph,line-(3+i)*stepv,column+7*steph,line-(3+i)*stepv);
    for (int i=1;i<7;i++)   m_painter.DrawLine(column+(1+i)*steph,line-2*stepv,column+(1+i)*steph,line-7*stepv);

    int string=1;
    foreach ( QString s, chord.toStrings())
    {
        if ( s == "x")
        {
            m_painter.DrawLine(column+(1+string)*steph-sizenonote,line-(stepv+s.toInt()*stepv)-stepv/2+2+sizenonote,column+(1+string)*steph+sizenonote,line-(stepv+s.toInt()*stepv)-stepv/2+2-sizenonote);
            m_painter.DrawLine(column+(1+string)*steph-sizenonote,line-(stepv+s.toInt()*stepv)-stepv/2+2-sizenonote,column+(1+string)*steph+sizenonote,line-(stepv+s.toInt()*stepv)-stepv/2+2+sizenonote);

        }
        else if ( s == "0")
        {
            m_painter.Circle(column+(1+string)*steph,line-(stepv+s.toInt()*stepv)-stepv/2+2,sizecircle*0.6);
            m_painter.SetStrokeWidth(0.5);
            m_painter.Stroke();
        }
        else
        {
            m_painter.Circle(column+(1+string)*steph,line-(stepv+s.toInt()*stepv)-stepv/2,sizecircle);
            m_painter.Fill(true);
        }
        string++;
    }
    column+=size;
    // PB NOT COL COMPLIENT
    if ( column +size > m_uiconfig->spuPageWidth->getPdfU() - m_uiconfig->spuHorizontalMargin->getPdfU())
    {
        column = static_cast<int>(m_uiconfig->spuHorizontalMargin->getPdfU());
        // TEST IF ENOUGH PLACE
        line=line-size;
    }
    }
     catch (const PoDoFo::PdfError &e)
   {
    throw(e);
   }
}

void Processor::displayLyricistComposerYear()
{
    QString comment ;
    if ( !m_lyricist .isEmpty()) comment +=tr("Lyricist: %1 ").arg(m_lyricist);
    if ( !m_composer .isEmpty()) comment +=tr("Composer: %1 ").arg(m_composer);
    if ( !m_artist .isEmpty()) comment +=tr("Artist: %1 ").arg(m_artist);
    if ( m_year!=0) comment +=tr("(%1) ").arg(m_year);
    if ( !m_pageAllocation)
    {
       m_page = m_document->CreatePage(*m_dimension);
       m_painter.SetPage(m_page);
       m_pageAllocation=true;
    }
  if ( m_uiconfig->toolButtonCommentFont->getBackgroundColor() != m_uiconfig->colorButtonPaperColor->getColor())
  {
    m_painter.SetColor(m_uiconfig->toolButtonCommentFont->getBackgroundColor().redF(),m_uiconfig->toolButtonCommentFont->getBackgroundColor().greenF(),m_uiconfig->toolButtonCommentFont->getBackgroundColor().blueF());
    m_painter.Rectangle(m_uiconfig->spuHorizontalMargin->getPdfU(),
                        m_uiconfig->spuPageHeight->getPdfU()-m_uiconfig->spuVerticalMargin->getPdfU()-m_uiconfig->toolButtonCommentFont->getFont().pointSize()*m_uiconfig->toolButtonCommentFont->getFont().pointSize(),//*1.4-1,
                        m_uiconfig->spuPageWidth->getPdfU()-2*m_uiconfig->spuHorizontalMargin->getPdfU(),
                        m_uiconfig->toolButtonCommentFont->getFont().pointSize());
    m_painter.Fill();
  }
  m_line-=m_uiconfig->toolButtonCommentFont->getFont().pointSize();

  Text(m_document,comment,m_uiconfig->spuPageWidth->getPdfU()/2,
       m_line,
       m_uiconfig->toolButtonCommentFont,center);
  m_line-=m_uiconfig->toolButtonCommentFont->font().pointSize()*2;
}

void Processor::displayRytm()
{

}


void Processor::displayBpm()
{

}


void Processor::BufLyricsNormailisation()
{
//    int i=0;
//    int max=m_BufLyrics.count();
//    QStringList out;
//    foreach ( QString text , m_BufLyrics)
//    {

//        //if ( text.contains(QRegExp("^ +$"))) text.replace(QRegExp(" +"),"");
//        if ( text.isEmpty() && i!=max-1 && m_BufLyrics.at(i+1).isEmpty())
//         ;
//        else
//            out<<text;
//        i++;
//    }
//    m_BufLyrics=out;
//    if (!m_BufLyrics.isEmpty())
//    while ( m_BufLyrics.last().isEmpty() && m_BufLyrics.count()!=1) m_BufLyrics.takeLast();
}


void Processor::addCol(int &col, int num)
{
  if ( m_colnumber == 1  && col + num < m_uiconfig->spuPageWidth->getPdfU()-m_uiconfig->spuHorizontalMargin->getPdfU())
  {
      col+=num;
  }
  else if ( m_colnumber == 1 )
  {
     NextLine(num);
     col=m_column;
  }
  else if ( m_colnumber > 1 &&  col+num < nextColumn(currentColumn()) )
  {
      col+=num;
  }
  else
  {
     NextLine(num);
     col=m_column;
  }
}


void Processor::setComposer( QString composer)
{
    m_composer=composer;
}

void Processor::setLyricist(QString lyricist)
{
    m_lyricist=lyricist;
}

void Processor::setArranger(QString arranger)
{
    m_arranger=arranger;
}

void Processor::setArtist(QString artist)
{
    m_artist=artist;
}
void Processor::setAlbum( QString album)
{
    m_album=album;
}
void Processor::setCopyright(QString copyright)
{
    m_copyright=copyright;
}

void Processor::setYear(int year)
{
    m_year=year;
}

void Processor::setKey(QString key)
{
    m_key=key;
}

void Processor::setDuration( QString duration)
{
    m_duration=duration;
}


void Processor::ColorizePixmap(QPixmap *src, QColor color)
{
    QGraphicsColorizeEffect colorize;
    colorize.setColor(color);
    if(src->isNull()) return ;
    QGraphicsScene scene;
    QGraphicsPixmapItem item(*src);
    item.setGraphicsEffect(&colorize);
    scene.addItem(&item);
    src->fill(Qt::transparent);
    QPainter ptr(src);
    scene.render (&ptr, QRectF(), QRectF( 0,0, src->width(), src->height()) );
}
