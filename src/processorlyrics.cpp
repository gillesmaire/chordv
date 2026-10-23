#include "processorlyrics.h"
#include "ui_formconfig.h"
#include "ui_mainwindow.h"
#include "chord.h"

#include <QDebug>
#include <QStringList>



using namespace PoDoFo;

ProcessorLyrics::ProcessorLyrics(Ui::MainWindow *ui1, Ui::FormConfig *ui2):Processor(ui1,ui2)
{
    m_mode="lyrics";
}


QStringList ProcessorLyrics::ListSolo(QString line)
{
  line.replace(" ","");
  int chordCount=line.count('[');
  QStringList  result;
  if ( ! line.endsWith("]")) return result;
  QRegularExpression chorusReg("(^\\[[^]]+])(.*)");
  QRegularExpressionMatch chorusMatch;
  while  ( line.indexOf(chorusReg,0,&chorusMatch)!=-1)
   {
        result<<chorusMatch.captured(1);
        line=chorusMatch.captured(2);
    }
  if ( result.count() != chordCount ) return QStringList();
  else return (result);
  }

void ProcessorLyrics::displayChordsForSong()
{
  if ( m_uiconfig->comboBoxChordInText->currentIndex()==FormConfig::ChordAtBeginingAndChordNameInText)
  {
    // chords are displayed in first with diagram and just chord name in text
    NextLine();
    NextLine();
    QStringList chords=m_BufChords;
    QStringList chordswithoutrythm=Chord::removeDupplicateWhithoutRytm(chords,m_uiconfig->comboBoxChordLang->currentData().toString());
    int linesave=m_line;
    foreach ( QString chord, chordswithoutrythm)
        for ( int copy =0 ; copy < m_pausenumber ; copy ++)
        {
           int line=m_line-copy*m_copyoffset;
           displayChord(chord,line,m_column,static_cast<int>(m_uiconfig->spuChordHorizontalSize->getPdfU()),m_uiconfig->comboBoxChordLang->currentData().toString());
           if (copy == 0 ) linesave=line;
        }
    m_column=static_cast<int>(m_uiconfig->spuHorizontalMargin->getPdfU());
    m_line=linesave-static_cast<int>(m_uiconfig->spuChordHorizontalSize->getPdfU());
 }
}

void ProcessorLyrics::displayLyrics()
{
    QRegularExpression CommentRex("^ *\\{(?:comment|c): *([^}]*) *\\}",QRegularExpression::CaseInsensitiveOption);
    QRegularExpressionMatch CommentRexMatch,CommentItalicRexMatch,CommentBoxRexMatch,chordexpMatch;
    QRegularExpression CommentItalicRex("^ *\\{(?:comment-italic|ci): *([^}]*) *\\}",QRegularExpression::CaseInsensitiveOption);
    QRegularExpression CommentBoxRex("^ *\\{(?:comment-box|cb): *([^}]*) *\\}",QRegularExpression::CaseInsensitiveOption);
    
   
    try{
    
    BufLyricsNormailisation();
    if ( m_firstline )
    {
        m_firstline=false;
        m_line-=m_uiconfig->toolButtonNormalFont->getFont().pointSizeF()*3;

    }
    int lastnum=0;
    foreach (QString text,m_BufLyrics)
    {
        int num=0;
        int col=m_column;
        PdfFont *pfont=m_document->CreateFont(m_uiconfig->toolButtonNormalFont->getFont().family().toLatin1());
        if ( ! text.isEmpty() )
        {
         if ( m_firstlyric ) {
                    m_firstlyric=false;
                    m_initiallyric=m_line;
            }
          if ( isColBreak(text)) doColumnBreak(text);
          else if ( text.contains(CommentRex,&CommentRexMatch))
          {
              Text(m_document,CommentRexMatch.captured(1),m_column,m_line,m_uiconfig->toolButtonCommentFont);
                m_CurrentString=text;
                NextLine();
                
          }
          else if ( text.contains(CommentItalicRex,&CommentItalicRexMatch))
          {
              Text(m_document,CommentItalicRexMatch.captured(1),m_column,m_line,m_uiconfig->toolButtonCommentItalicFont);
                m_CurrentString=text;NextLine();
          }
          else if ( text.contains(CommentBoxRex,&CommentBoxRexMatch))
          {
              TextInBox(m_document,CommentBoxRexMatch.captured(1),m_column,m_line,m_uiconfig->toolButtonCommentBoxFont);
               m_CurrentString=text;
               NextLine();
          }
          else if (text.contains("STARTSOLO"))
          {
           Text(m_document,tr("Solo :"),m_column,m_line,m_uiconfig->toolButtonNormalFont);
            NextLine();
            NextLine();
          }          
          else if (text.contains("ENDSOLO"))
          {
           
          }
          else 
          {
          QRegularExpression chordexp("([^[]*)\\[([^]]*)\\]([^[]*)(.*)");
          while  ( text.indexOf(chordexp,0,&chordexpMatch)!=-1)
          {

            ////class Chord ch(chordexpMatch.captured(2),m_uimainwindow->comboBoxChordLanguage->currentData().toString());
            ////QString t1=chordexpMatch.captured(1);
            class Chord ch(chordexpMatch.captured(2),m_uimainwindow->comboBoxChordLanguage->currentData().toString());
            QString t1=chordexpMatch.captured(1);
            QString t2=chordexpMatch.captured(3);
            QString reste=chordexpMatch.captured(4);
           
            int lenghtt1=static_cast<int>(pfont->GetFontMetrics()->StringWidth(PdfString (QString("%1").arg(t1).toLatin1())));
            int lenghtt2=static_cast<int>(pfont->GetFontMetrics()->StringWidth(PdfString (QString("%1").arg(t2).toLatin1())));
            //we print the lyrics between two chords
            Text(m_document,t1,col,m_line,m_uiconfig->toolButtonNormalFont);
            col+=lenghtt1;
            QString c1=m_uiconfig->comboBoxChordLang->currentData().toString()=="en"?ch.nameEnglish():ch.nameLocale();
            int lenghtc1;
            // the lenght of chord 
            if (  m_uiconfig->comboBoxChordInText->currentIndex()!= FormConfig::DiagramInText) 
                {
                   //c1 is the chord translated ie Lam in french
                lenghtc1=static_cast<int>(pfont->GetFontMetrics()->StringWidth(PdfString (QString("%1").arg(c1).toLatin1())));
                num=static_cast<int>(m_uiconfig->toolButtonChordFont->getFont().pointSizeF()*1.2);
                Text(m_document,c1,col,m_line+num,m_uiconfig->toolButtonChordFont);
                }
            else
               { 
                  int l=static_cast<int>(m_line+m_uiconfig->spuChordHorizontalSize->getPdfU());
                  
                  displayChord(ch.chord(),l,col,static_cast<int>(m_uiconfig->spuChordHorizontalSize->getPdfU()),m_uiconfig->comboBoxChordLang->currentData().toString());
                  lenghtc1=static_cast<int>(m_uiconfig->spuChordHorizontalSize->getPdfU());
                  num=static_cast<int>(m_uiconfig->spuChordHorizontalSize->getPdfU());
               }
            // For solo or chords without blank we increase the position (not if the chord is positionned on margin
            // no text between chord
            Text(m_document,t2,col,m_line,m_uiconfig->toolButtonNormalFont);
            if ( lenghtc1 > lenghtt2 )
              col+=lenghtc1;
              else
            col+=lenghtt2;
            text=reste;
           }
        if ( (text ==tr("Chorus :") || text ==tr("Solo :")) && m_uiconfig->comboBoxChordInText->currentIndex()== FormConfig::DiagramInText)
                  num = static_cast<int>(m_uiconfig->spuChordHorizontalSize->getPdfU());
        else if ( m_uiconfig->comboBoxChordInText->currentIndex()!= FormConfig::DiagramInText )
                  num= static_cast<int>(m_uiconfig->toolButtonChordFont->getFont().pointSizeF());
          Text(m_document,text,col,m_line,m_uiconfig->toolButtonNormalFont);
        }
     }
     else if ( ! m_firstlyric)
            {
              m_CurrentString=text;
              NextLine(num);
            }
        if ( num ==0 && ! text.replace(" ","").isEmpty() && text!=tr("Chorus :") 
        &&!text.contains(QRegularExpression("^\\{(Column_break|colb)"))) num=lastnum;
        m_CurrentString=text;
        
        NextLine(num);
        
        lastnum=num;
    }
    m_firstlyric=true;
    m_initiallyric=0;
    }
     catch (const PoDoFo::PdfError &e)
   {
    throw(e);
   }
}

void ProcessorLyrics::includeSos() {

    m_BufLyrics<<"STARTSOLO";
}

void ProcessorLyrics::includeEos() {

    m_BufLyrics<<"ENDSOLO";

}


void ProcessorLyrics::Drawline( double pos, int num , FontButton *ptr )
{
    try{
    m_painter.Circle(pos ,m_line+num-ptr->getFont().pointSizeF()/2.0,1.3);
    m_painter.Fill(true);
    ///m_painter.DrawLine(pos,m_line+num-num-ptr->getFont().pointSizeF()/2.0,pos,m_line+num-num-ptr->getFont().pointSizeF()/4.0);
    m_painter.DrawLine(pos,m_line+num-ptr->getFont().pointSizeF()/2.0,pos+ptr->getFont().pointSizeF()/2.0,m_line+num-ptr->getFont().pointSizeF()/2.0);
    }
     catch (const PoDoFo::PdfError &e)
   {
    throw(e);
   }
}

//void ProcessorLyrics::doColumnBreak(QString line)
//{
//    try {
//    QRegularExpression ColumnBreakREX("^ *\\{(column_break|colb)(:lyrics) *\\}",QRegularExpression::CaseInsensitiveOption);
//    if ( line.contains(ColumnBreakREX))
//    {
//        if ( m_colnumber > 1 && currentColumn() < m_colnumber  )
//              {
//                m_column = nextColumn( currentColumn() ) ;
//                 if ( m_pagecontinued  == 1 )
//                       m_line=m_initiallyric;
//                else
//                        m_line= m_initialvposition;
////                m_line=static_cast<int>(m_initialvposition)-m_verticalStartOfLyric;
//                m_columnChangeAsked=true;
//              }
//            else
//            {
//                if (m_tocpages.count() != 0)
//                    m_tocpages[m_title]++;
//                newPage();
//            }
//    }
//    }
//     catch (const PoDoFo::PdfError &e)
//   {
//    throw(e);
//   }
//}
