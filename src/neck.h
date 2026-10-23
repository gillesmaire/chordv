#ifndef NECK_H
#define NECK_H

#include <QGraphicsView>

///
/// \brief The Neck class is derivated of graphicsView to allow to show a guitar neck
/// and analyse the string touched to calculate chord.
///
///  The result is given by the signal ChordsDetected(QStringList chordname,QString notes);
///
class Neck : public QGraphicsView
{
    Q_OBJECT
public:
    ///
    /// \brief Neck neck constructor complient with widget promotion in QtDesigner
    /// \param parent parent of the widget
    ///
    Neck(QWidget *parent);
    ~Neck();
    ///
    /// \brief ShowChord : show chord on neck
    /// \param value : chord in form "3 0 3 3 2 0 0" (first is the relative index on neck
    ///
    void ShowChord(QString value) ;
    ///
    void DrawNeck() ;
private:
    QGraphicsScene *m_scene;

    void DrawFret(int fret);
    void resizeEvent(QResizeEvent *event);
    QBrush m_woodbrush;
    QBrush m_fretbrush;
    QPen m_fretpen;
    QPen m_woodpen;
    QPen m_stringpen;

    void DrawStrings();
    void SizeString(int string);
    void DrawCircle(int fret);
    void mousePressEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
    void DrawNotes();
    void DrawNote( int note, int string);
    void DrawNoNote(int x,int y);
    ///
    /// \brief String2Note convert a fret pressed on a string
    /// \param string : the string
    /// \param fret : the pressed fred
    /// \return  : the note in string format
    ///
    QString String2Note(int string, int fret);
    ///
    /// \brief NotesToChord convert a list of notes ("D","E","G" ) to chord name
    /// \param Notes a list of note in form "C" "C#" "D" "D#" ... "B"
    /// \return the chord in QString format
    ///
    QStringList NotesToChord(QStringList Notes);

    ///
    /// \brief getNotes return the list of 6 notes clicked on neck as QStringList form
    /// Exemple : ("D",,"E",,,,) .
    /// \return  a QStringList with "" for no note and note in form "C" "C#" "D" "D#" ... "B"
    ///
    QStringList getNotes();

    ///
    /// \brief m_chord fret values on each chord
    /// chord is 0 x 2 3 3 2 ( 1 chord is high )
    ///
    int m_chord[7];
    ///
    /// \brief m_notes list of 17 notes  :  C C# Db D D# Eb E F F# Gb G G# Ab A A# Bb B
    ///                                     0 1  1  2 3  3  4 5 6  6  7 8  8  9 10 10 11
    QStringList m_notes;
    ///
    /// \brief m_values list of values for each 17 notes : 0 1 1 2 3 3 4 5 6 6 7 8 8 9 10 10 11
    ///
    int m_values[17]= {0,1,1,2,3,3,4,5,6,6,7,8,8,9,10,10,11};
    ///
    /// \brief m_necknotes
    ///
    QStringList m_necknotes;
    ///
    /// \brief m_notevalues m_notevalues["C#"] = 1 ...
    ///
    QHash <QString,int> m_notevalues;
    ///
    ////// \brief m_strings note of each strings on the guitar (from high to bass)
    ///
    int m_strings[6] ={4,11,7,2,9,4};
    QList <int>m_chordInterval ;
    QStringList m_chordNote;
    ///
    /// \brief EraseEmptyNote erase in QStringList with six chords, the unplayed chord
    /// \param notes QStringList with several notes
    /// \return  QStringList without any "" notes
    ///
    QStringList EraseEmptyNote(QStringList notes);
    ///
    /// \brief chordInterval return interval in string char. O is ommitted.
    /// \param i : interval
    /// \return  : the string value
    ///
    QString chordInterval(int i);
    ///
    /// \brief toString tranform m_chord to x32233 string
    /// \return
    ///
    QString toString();
    void mouseDoubleClickEvent(QMouseEvent *e);
signals:
    ///
    /// \brief ChordsDetected the chords detected are send as a signal with the list of chordnames, and
    /// the notes in x0101000 notation
    /// \param chordname : list of chordnames
    /// \param notes  : notes
    ///
    void ChordsDetected(QStringList chordname,QString notes);
};


#endif // NECK_H
