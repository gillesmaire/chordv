#ifndef PROCESSORCHORD_H
#define PROCESSORCHORD_H

#include "processor.h"
#include "mainwindow.h"
#include <podofo/podofo.h>
#include <podofo/podofo-base.h>

#include <QMap>


class ProcessorChord : public Processor
{
public:
    ProcessorChord(Ui::MainWindow *ui1, Ui::FormConfig *ui2);
private:
    ///
    /// \brief displayLyrics display lyrics
    ///
    void displayLyrics();
    ///
    /// \brief displayBpm display Beat per minute
    ///
    void displayBpm();
    ///
    /// \brief displayPageSubtitle display page subtile
    ///
    void displayPageSubtitle(QString ){}
    ///
    /// \brief includeSoc include Start of Solo
    ///
    void includeSos();
    ///
    /// \brief includeSor include Start of Chorus
    ///
    void includeSoc();

    int m_nbBeatPerBar;
    void doColumnBreak(QString){}
    int m_w;
    int m_h;
    int m_x0;
    ///
    /// \brief m_vm vertical margin between two chord lines
    ///
    int m_vm;
    int m_hm;
    int m_fontchordsize;
    int m_barsperline;
    ///
    /// \brief m_posx can be 1 2 3 4 ( if 4 = m_nbBeatPerBar )
    ///
    int m_posx;

    QMap <QString, int >m_beats;
    QStringList m_subbar;
    PoDoFo::PdfFont *m_chordfont;
    bool CompleteBeats(QStringList ListNotes,int nbbeat);
    ///
    /// \brief DisplayBar display chord  or % if % is on column 1 print alternative
    /// \param ch : chord or %
    /// \param alternartive chord if column is 1
    ///
    void DisplayBar(QString ch, QString alternartive=QString());
    
    
    ///
    /// \brief SubBar2Bar
    ///
    void SubBar2Bar();
};

#endif // PROCESSORCHORD_H
