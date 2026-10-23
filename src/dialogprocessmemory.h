#ifndef DIALOGPROCESSMEMORY_H
#define DIALOGPROCESSMEMORY_H

#include <QDialog>
#include <QMap>
#include <QTimer>
#include <QMediaPlayer>
#ifdef __linux__
#include <jack/jack.h>
#include <jack/transport.h>
#endif


namespace Ui {
class DialogProcessMemory;
}

class DialogProcessMemory : public QDialog
{
    Q_OBJECT

public:
    explicit DialogProcessMemory(QWidget *parent ,QString allsongs,int position, QString title,  bool showrythm, bool click, int volume, bool firstaccentued,
                                 QFont font, QColor text, QColor background,bool fullscreen, bool twolines,double advance, int timebefore, bool jacksynchro,int timebeforeunit);
    ~DialogProcessMemory();
    bool getJackConnected() { return m_jacksynchro ;}

private slots:
    ///
    /// \brief displaySong display all line of song
    ///
    void displaySong();
    ///
    /// \brief showRythm display and play the rythm
    ///
    void showRythm();
    ///
    /// \brief eraseBull this slot erase the rythm bullet after a short timer
    ///
    void eraseBull();
    ///
    /// \brief JackMessages is the call back
    ///
    void JackMessages();
    ///
    /// \brief Stop just set m_stop to true
    ///
    void Stop();

private:
    enum State{NotStarted,Running,Paused,Finished};
    Ui::DialogProcessMemory *ui;
    bool m_stop;
    bool   m_firststart;
    bool m_timebeforeunit;
    bool m_pause;
    double m_tempo;
    bool m_tocomplete; // first line don't start with a chord and we need to add a line with lyrircs before chord
    int m_timeup;
    bool m_click;
    bool m_accentuedfirst;
    qint32 m_lasttime;
    State m_status;
    int m_timebefore;
    double m_advance;
    QTimer *m_timerlyrics;
    QTimer *m_timeline;
    QMap <int,int> m_mseconds;
    QMap <int,QString> m_lyrics;
    QStringList m_refrain;
    QMap <int,int>m_refrainnbbeat;
    QMap <int,int>m_coupletnbbeat;
    int m_nblinecouplet;
    int m_nblinerefrain;
    int m_nblyrics;
    bool m_showrythm;
    bool m_jacksynchro;
    QFont m_font;
    QColor m_textcolor;
    QColor m_backgroundcolor;
    int m_indice;
    int m_indicemax;
    int m_countrythm;
    int m_period;
    int m_millisecondperbeat;
    int m_time;
    qint64 m_counttheorical;
    qint64 m_countreal;
    QMediaPlayer *m_player;
#ifdef __linux__
    jack_client_t *m_jackclient;
#endif
    ///
    /// \brief getInfo get all the information in song file, about the song named title
    /// the two QMap m_lyrics and m_mseconds are set
    /// \param songs : the whole songs buffer
    /// \param title : the title of song searched
    ///
    void getInfo(QString songs,QString title);
    ///
    /// \brief getNumberOfBeat return the number on beat from a line reading the chord of the line and knowing the timeup
    /// \param line : line with chords with x : notation [Dx2] for 2 bars [D:2] for two beat if timeup is 2
    /// \param timeup : is the 3 if signature is 3/4
    /// \return number of beat of the song line
    ///
    int getNumberOfBeat(QString &line, int timeup);
    ///
    /// \brief getColorBetween return the color between color1 and color2.
    /// This color is calculated for the secon line
    /// \param color1
    /// \param color2
    /// \return
    ///
    QColor getColorBetween(QColor color1, QColor color2);
    ///
    /// \brief keyPressEvent read if ESCAPE is typed and call pause pause on un pause
    /// \brief keyPressEvent
    /// \param event keyboard event
    void keyPressEvent(QKeyEvent *event);
    ///
    /// \brief PauseUnPause pause if the lyrics are running unpause if they are in pause
    ///
    void PauseUnPause();
    ///
    /// \brief getTimeBefore convert timebefore value and unit to
    /// \param timebefore :the time in unit
    /// \param unit 0 for second, 1 for beat, 1 for bar
    /// \param tempo in double
    /// \param timeup is number of beat per bar
    /// \return  the time in millisecond
    ///
    int getTimeBefore(int timebefore, int unit, double tempo, int timeup);
    ///
    /// \brief SetCurrentWindow construct the window to display the lyrics, the windows is on top without decoration
    /// \param fullscreen : true isf fullscreen false else
    /// \param twoline : true if two lines are used false if only one line is displayed.
    /// \param position : 0 (top) 1 (middle) 2 (bottom)
    /// \param font : the font for the lyrics
    /// \param backgroundcolor : the background color of the window
    /// \param textcolor : the text color of the window
    ///
    void SetCurrentWindow(bool fullscreen, bool twoline, int position, QFont font, QColor backgroundcolor, QColor textcolor);
    ///
    /// \brief AddTimeBefore add in m_lyrics and m_mseconds qmap the before time
    /// \param timebefore
    /// \param timebeforeunit
    /// \param tempo
    /// \param timeup
    /// \param advance
    ///
    void AddTimeBefore(int timebefore, int timebeforeunit, int tempo, int timeup, double advance);
    ///
    /// \brief StartJack start jack client if need
    ///
    void StartJack();
    ///
    /// \brief SetTime set Time answer to the situation where the position is changed on master jack sofware.
    /// In this case we need to :
    /// - stop the current timer showing lyrics
    /// - calculate the lyrics and time to set on the timer
    /// \param mesecond is the new position
    ///
    void SetTime(qint32 msecond);
    ///
    /// \brief getMilliSeconds get milliseconds from start on song from bar, beat and tick from
    /// given bar, beat and tick
    /// \param bar : the bar
    /// \param beat
    /// \param tick
    /// \return millisecond number
    qint32 getMilliSeconds (qint32 bar, qint32 beat, qint32 tick, double sst);
    ///
    /// \brief Debug show m_lyrics and m_msecond qmap
    ///
    void Debug();

};

#endif // DIALOGPROCESSMEMORY_H
