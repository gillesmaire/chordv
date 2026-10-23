#include "dialogprocessmemory.h"
#include "ui_dialogprocessmemory.h"

#include <QApplication>
#include <QDebug>
#include <QRegularExpression>
#include <QTimer>
#include <QKeyEvent>
#include <QFontMetrics>
#include <QMessageBox>
#include <QDateTime>
#include <QString>
#include <QScreen>


DialogProcessMemory::DialogProcessMemory(QWidget *parent, QString songs, int position,QString title,  bool showrythm,
                                         bool click, int volume, bool accentuedfirst ,QFont font, QColor textcolor, QColor background, bool fullScreen, bool twolines, double advance,
                                         int timebefore, bool jacksynchro, int timebeforeunit):

    QDialog(parent),
ui(new Ui::DialogProcessMemory)
{
    ui->setupUi(this);
    m_player= new QMediaPlayer;
  //  m_player->setVolume(volume);
    m_lasttime=0;
    m_tocomplete=false;
    m_stop=false;
    m_pause=false;
    m_jacksynchro=jacksynchro;
    m_timebeforeunit=timebeforeunit;
    m_font=font;
    m_showrythm=showrythm;
    m_textcolor=textcolor;
    m_backgroundcolor=background;
    m_countrythm=1;
    m_advance=advance;
    m_click=click;
    m_firststart=true;
    m_accentuedfirst=accentuedfirst;
    m_status=NotStarted;
    SetCurrentWindow(fullScreen,twolines,position,m_font,m_backgroundcolor,m_textcolor);
    if ( ! m_showrythm ) ui->labelTimeBullet->setVisible(false);
    m_nblinecouplet=0;
    m_nblinerefrain=0;
    getInfo(songs,title);
    AddTimeBefore(timebefore,timebeforeunit,static_cast<int>(m_tempo),m_timeup,advance);
    m_period=m_millisecondperbeat/4; //?
    m_indice=-1;
    m_indicemax=m_lyrics.count();
    m_timerlyrics = new QTimer;
    connect ( m_timerlyrics,SIGNAL(timeout()),this,SLOT(displaySong()));
    m_timeline = new QTimer;
    m_time=0;
#ifdef __linux__
    m_jackclient=nullptr;
#endif
    if (  m_showrythm || m_click )  m_timeline->start(m_millisecondperbeat);
    if ( m_jacksynchro)
        StartJack();
    else
    {
        connect ( m_timeline,SIGNAL(timeout()),this,SLOT(showRythm()));
        displaySong();
    }
    Debug();

}

void DialogProcessMemory::StartJack()
{
#ifdef __linux__
    jack_status_t  status;
    if ((m_jackclient = jack_client_open ("chordVJack", JackNoStartServer,&status)) == nullptr)
    {
         QMessageBox::warning(nullptr,tr("Jack not found"),tr("Jack seems to not be launched ! \n(Try to launch Jack manually without QJackCtl)"));
         m_jacksynchro=false;
    }
    else
    {
        if (jack_activate (m_jackclient) )
        {
            QMessageBox::warning(nullptr,tr("Jack problem"),tr("Jack cannot be activated !"));
            m_jacksynchro=false;
        }
        else
        {
            connect(m_timeline, SIGNAL(timeout()), this, SLOT(JackMessages()));
            m_timeline->start(m_period);
        }
    }
#endif
}

void DialogProcessMemory::SetTime(qint32 msec)
{
    int i=0;
    int total=0;
    while ( total+m_mseconds[i]<msec )
    {
        if ( i == m_mseconds.count() ) break;
        total+=m_mseconds[i];
        i++;
    }
    m_indice=i;
    ui->labelText1->setText(m_lyrics[m_indice-1]);
    if ( m_indice < m_indicemax) ui->labelText2->setText(m_lyrics[m_indice]);
    else ui->labelText2->setText("");

    m_timerlyrics->start(msec-total);

}

qint32 DialogProcessMemory::getMilliSeconds(qint32 bar, qint32 beat, qint32 tick, double bst)
{
    if ( bst-1920 < 0.000001 ) return -1 ;
    qint32 ret=static_cast<qint32>((static_cast<double>(tick)/1920+bar*m_timeup+beat)*m_millisecondperbeat);
    return ret;
}

void DialogProcessMemory::Debug()
{
    for ( auto i : m_lyrics.keys())
    {
        qDebug()<<i<<"=> "<<m_lyrics[i]<<":"<<m_mseconds[i];
    }
}

void DialogProcessMemory::JackMessages()
{
#ifdef __linux__
    jack_position_t current;
    //double jackTempo=current.beats_per_minute;
    qint32 msecond=getMilliSeconds(current.bar,current.beat,current.tick,current.bar_start_tick);
    if (  msecond != -1  )
    {
        if ( msecond != 0 && m_lasttime!=0)
            if (std::abs(msecond-m_lasttime) > 700 ) SetTime(msecond) ;
        m_lasttime=msecond;
    }

    jack_transport_state_t transport_state;
    jack_nframes_t frame_time;
    transport_state = jack_transport_query (m_jackclient, &current);
    frame_time = jack_frame_time (m_jackclient);
    Q_UNUSED(frame_time);

    switch (transport_state) {
        case JackTransportStopped:
            if (m_status==Running)
            {
                m_status=Paused;
                if ( m_timerlyrics) m_timerlyrics->stop();
            }
            break;
        case JackTransportRolling:
            if (m_firststart) displaySong();
            m_firststart=false;
            if (m_status!=Running)
            {
                m_status=Running;
                if ( m_timerlyrics) m_timerlyrics->start();
            }
            break;
        case JackTransportStarting:
            qWarning()<<"state: Starting";
            break;
        default:
            qWarning()<<"state: [unknown]";
        }
#endif
}

DialogProcessMemory::~DialogProcessMemory()
{
    delete m_player;
    delete m_timerlyrics;
    //if (m_jackclient) delete (m_jackclient);
    delete ui;
}

void DialogProcessMemory::displaySong()
{
    m_indice++;
    if ( m_indice - 1< m_indicemax)
    {
        ui->labelText1->setText(m_lyrics[m_indice-1]);
        if ( m_indice < m_indicemax) ui->labelText2->setText(m_lyrics[m_indice]);
        else ui->labelText2->setText("");
        m_timerlyrics->start(m_mseconds[m_indice-1]);
    }
   else
        QTimer::singleShot(3000,this,SLOT(Stop()));
}

void DialogProcessMemory::showRythm()
{
    if ( m_click  )
    {
#if QT_VERSION >= 0x060000
       if (m_accentuedfirst && m_countrythm % m_timeup == 1) m_player->setSource(QUrl("qrc:/sound/1.wav"));
       else  m_player->setSource(QUrl("qrc:/sound/2.wav"));
#else
        if (m_accentuedfirst && m_countrythm % m_timeup == 1) m_player->setMedia(QUrl("qrc:/sound/1.wav"));
        else  m_player->setMedia(QUrl("qrc:/sound/2.wav"));
#endif
       m_player->play();
    }
    if ( m_showrythm )
    {
        if ( m_countrythm % m_timeup == 0 ) ui->labelTimeBullet->setPixmap(QPixmap(":/Image/Images/redbull.png"));
        else ui->labelTimeBullet->setPixmap(QPixmap(":/Image/Images/greenbull.png"));
        QTimer::singleShot(300,this,SLOT(eraseBull()));
    }
    m_countrythm++;
}

void DialogProcessMemory::Stop()
{
    m_timeline->stop();
}


void DialogProcessMemory::PauseUnPause()
{
    if (! m_pause)
    {
      m_timerlyrics->stop();
      m_timeline->stop();
    }
   else
   {
     m_timerlyrics->start();
     m_timeline->start();
   }
   m_pause=!m_pause;
}

int DialogProcessMemory::getNumberOfBeat(QString &line,int timeup)
{
    int number=0;
    QRegularExpression chordEx("\\[([^]]*)\\]");
    QRegularExpressionMatch match;
    while ( line.indexOf(chordEx,0,&match)!=-1)
    {
        QString chord=match.captured(1);
        line.replace(QString("[%1]").arg(match.captured(1)),"");
        QRegularExpression chord1("x([0-9]+)");
        QRegularExpression chord2(":([1-4]+)");
        QRegularExpressionMatch matchi;
        if ( chord.contains(chord1,&matchi) )
        {   bool ok;
            int nb=matchi.captured(1).toInt(&ok);
            if ( ok )
              number+=timeup*nb;
            else
              number+=timeup;
        }
        else if ( chord.contains(chord2,&matchi) )
        {
            bool ok;
            int nb=matchi.captured(1).toInt(&ok);
            if ( ok )
               number+=timeup/nb;
            else number+=timeup;
        }
        else number+=timeup;
    }
    return number;
}

void DialogProcessMemory::eraseBull()
{
    ui->labelTimeBullet->setPixmap(QPixmap());
}

QColor DialogProcessMemory::getColorBetween(QColor color1, QColor color2)
{
    int r1,g1,b1,r2,g2,b2;
    color1.getRgb(&r1,&g1,&b1);
    color2.getRgb(&r2,&g2,&b2);
    return QColor((r1+r2)/2,(g1+g2)/2,(b1+b2)/2);
}

void DialogProcessMemory::keyPressEvent(QKeyEvent *event)
{
    if ( ! m_jacksynchro)
    {
        if (event->key()==Qt::Key_Space )
        {
        PauseUnPause();
        }
    }
    QDialog::keyPressEvent(event);
}

int DialogProcessMemory::getTimeBefore( int timebefore,int unit, double tempo, int timeup)
{
    if ( unit == 1 ) return static_cast<int>(timebefore*60000/tempo); //beat
    else if ( unit == 0 )  return static_cast<int>( timebefore*1000);//second
     return static_cast<int>(timebefore*timeup*60000/tempo);
}

void DialogProcessMemory::SetCurrentWindow(bool fullscreen, bool twoline, int position, QFont font, QColor backgroundcolor, QColor textcolor)
{
    QRect rect=this->screen()->geometry();
    QFontMetrics fm(font);
    if ( fullscreen )
    {
        setGeometry(rect);
        setWindowState(Qt::WindowFullScreen);
    }
    else
    {
        int H2=fm.height();
        if (position ==0 ) setGeometry(rect.x(),rect.y(),rect.width(),2*H2);
        else if  ( position==1 ) setGeometry(rect.x(),rect.height()/2-H2,rect.width(),H2*2);
        else setGeometry(rect.x(),rect.height()-H2*2,rect.width(),H2*2);
    }


    setWindowFlags(Qt::Window | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    setStyleSheet(QString("background-color:%1").arg(backgroundcolor.name()));
    ui->labelText1->setFont(font);
    ui->labelText1->setStyleSheet(QString("color:%1;").arg(textcolor.name()));
    ui->labelText2->setFont(font);
    ui->labelText2->setStyleSheet(QString("color:%1;").arg(getColorBetween(backgroundcolor, textcolor).name()));
    if ( ! twoline) ui->labelText2->setVisible(false);
}

void DialogProcessMemory::getInfo( QString songs,QString title)
{
    QRegularExpression TitleREX("^ *\\{(?:title|t): *([^}]*)\\}",QRegularExpression::CaseInsensitiveOption);
    QRegularExpressionMatch titlematch;
    QRegularExpressionMatch solmatch;
    QRegularExpression SosREX("^ *\\{(?:sol|start_of_solo)\\}",QRegularExpression::CaseInsensitiveOption);
    QRegularExpression EosREX("^ *\\{(?:eos|end_of_solo)\\}",QRegularExpression::CaseInsensitiveOption);
    QRegularExpression SocREX("^ *\\{(?:soc|start_of_chorus)\\}",QRegularExpression::CaseInsensitiveOption);
    QRegularExpression EocREX("^ *\\{(?:eoc|end_of_chorus)\\}",QRegularExpression::CaseInsensitiveOption);
    QRegularExpression TempoREX("^ *\\{(?:tempo): *([0-9.]*)\\}",QRegularExpression::CaseInsensitiveOption);
    QRegularExpressionMatch temporexmatch;
    QRegularExpression TimeREX("^ *\\{(?:time): *([1-5])/(1|2|4|8|16|32|64)\\}",QRegularExpression::CaseInsensitiveOption);
    QRegularExpressionMatch timerexmatch;
    QRegularExpression OtherREX("^ *\\{");
    QStringList buf=songs.split("\n");
    bool found=false;
    m_tempo=120;
    m_millisecondperbeat=static_cast<int>(60000/m_tempo);
    m_timeup=4;
    bool solo=false;
    bool refrain=false;
    bool firstcouplet=false;
    bool refrainexist=false;
    bool refrainmade=false;
    int coupletindex=0;
    m_nblyrics=0;
    ui->labelText1->setFont(m_font);
    bool firstline=true;

    foreach ( QString line, buf)
    {
        if ( line.contains(TitleREX))
        {
            if ( found ) break;
            if (titlematch.captured(1) == title)
                found =true;
        }
        else if ( found )
        {
            if ( line.contains(TempoREX))
            {
                m_tempo=temporexmatch.captured(1).toDouble();
                if ( m_tempo<1) m_tempo=120;
                m_millisecondperbeat=static_cast<int>(60000/m_tempo);
            }
            else if ( line.contains(TimeREX,&timerexmatch))
            {
                m_timeup=timerexmatch.captured(1).toInt();
            }
            else if (line.contains(SosREX))
                solo=true;
            else if (line.contains(EosREX))
                solo=false;
            else if (line.contains(SocREX))
            {
                m_nblinerefrain=0;
                refrain=true;
            }
            else if (line.contains(EocREX))
            {
                refrain=false;
                refrainmade=true;
            }
            else if (line.contains(OtherREX)) ;
            else if ( line.isEmpty())
            {
                firstcouplet=false;
                coupletindex=0;
                if ( refrainexist && ! refrainmade)
                {
                  int index=1;
                  foreach (QString ref, m_refrain)
                  {
                      m_nblyrics++;
                      m_lyrics[m_nblyrics]=ref;
                      m_mseconds[m_nblyrics]=m_refrainnbbeat[index]*m_millisecondperbeat;
                      index++;
                  }
                }
            }
            else if (solo)
            {
             m_nblyrics++;
             int nbbeat=getNumberOfBeat(line,m_timeup);
             m_lyrics[m_nblyrics]="...";
             m_mseconds[m_nblyrics]=m_millisecondperbeat*nbbeat;
            }
            else if ( refrain )
            {
                m_nblinerefrain++;
                if ( firstline )
                {
                    QRegularExpression splitREX("^([^[]+)(.*)");
                    QRegularExpressionMatch splitmatch;
                    if ( line.contains(splitREX,&splitmatch))
                    {
                       m_nblyrics++;
                       m_lyrics[m_nblyrics]=splitmatch.captured(1);
                       line=splitmatch.captured(2);
                       m_tocomplete=true;
                    }
                }
                firstline=false;
                int nbbeat=getNumberOfBeat(line,m_timeup);
                m_refrainnbbeat[m_nblinerefrain]=nbbeat;
                m_refrain<<line;
                m_nblyrics++;
                m_lyrics[m_nblyrics]=line;
                m_mseconds[m_nblyrics]=m_millisecondperbeat*nbbeat;
                refrainexist=true;
                refrainmade=true;
            }
            else
            {
                if ( line.contains("[") || firstcouplet )
                {
                    firstcouplet=true;
                    m_nblinecouplet++;
                    if ( firstline )
                    {
                        QRegularExpression splitREX("^([^[]+)(.*)");
                        QRegularExpressionMatch splitmatch;
                        if ( line.contains(splitREX,&splitmatch))
                        {
                           m_nblyrics++;
                           m_lyrics[m_nblyrics]=splitmatch.captured(1);
                           line=splitmatch.captured(2);
                           m_tocomplete=true;
                        }
                    }
                    firstline=false;
                    int nbbeat=getNumberOfBeat(line,m_timeup);
                    m_coupletnbbeat[m_nblinecouplet]=nbbeat;
                    m_nblyrics++;
                    m_lyrics[m_nblyrics]=line;
                    m_mseconds[m_nblyrics]=m_millisecondperbeat*nbbeat;
                    coupletindex=0;
                }
                else
                 {
                    coupletindex++;
                    m_nblyrics++;
                    m_lyrics[m_nblyrics]=line;
                    m_mseconds[m_nblyrics]=m_coupletnbbeat[coupletindex]*m_millisecondperbeat;
                    refrainmade=false;
                 }
           }
        }
    }
     int count=0;
     if ( ! refrain )
     {
        for  ( int line=0; line<m_mseconds.count(); line++ )
        {

          if ( m_lyrics[line].isEmpty() || m_lyrics[line].contains(QRegularExpression("^ *$")))
              count=0;
          else
              count ++;
          if ( m_mseconds[line]==0 &&  ! m_lyrics[line].isEmpty() && m_lyrics[line].contains(QRegularExpression("^ *$")))
          {
              m_mseconds[line]=m_mseconds[count];
          }

        }


    }
    if ( m_tocomplete )
    {
        QFontMetrics fm(m_font);

        while ( fm.horizontalAdvance(m_lyrics[1])< fm.horizontalAdvance(m_lyrics[2]))
            m_lyrics[1]=" "+m_lyrics[1];
        m_mseconds[1]=m_mseconds[2];
    }

}

void DialogProcessMemory::AddTimeBefore(int timebefore, int timebeforeunit , int tempo, int timeup, double advance)
{
    timebefore=getTimeBefore(timebefore,timebeforeunit,tempo,timeup);
    if (m_tocomplete ) timebefore-=m_mseconds[1];
    m_lyrics[0]="";
    m_mseconds[0]=static_cast<int>(timebefore-1000*advance);
}
