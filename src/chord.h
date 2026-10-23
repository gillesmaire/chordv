#ifndef CHORD_H
#define CHORD_H

#include "langnotes.h"
#include <QString>
#include <QStringList>

/// \brief The Chord class
/// Manipulate the chords in D,D6,D7m,D7-,D7(V) on the Vth fret, D7x2 : on two bear, D7:2 on 1/2 bar
///
class Chord : public QString
{
public:
    ///
    /// \brief Chord chord is defined by a chordname and the language name for the chord
    /// \param chord : chordname for example D7(III)x2
    /// \param codelang : en, fr
    ///
    Chord(QString chord,QString codelang="en");
    ///
    /// \brief chord
    /// \return the original name given in constructor
    ///
    QString chord();
    ///
    /// \brief removeRytm
    /// \return the name of the chord without :2 or x2 and translated in locale
    ///
    ///
    QString removeRytm();
    ///
    ////// \brief getRhythm get rhythm from chord.
    ////// \return  :2 x2 empty string etc
    ///
    QString getRhythm();
    ///
    /// \brief nbBar
    /// \return the number after the x
    ///
    int nbBar();
    
    ///
    /// \brief nbBeat
    /// \return  the number after the :
    ///
    int nbBeat();
    ///
    /// \brief toStrings
    /// \return the chords on string for example x x 0 2 1 2
    ///
    ///
    QStringList toStrings();
    ///
    /// \brief fret
    /// \return  fret number : 0 1 etc...
    ///
    QString fret();
    ///
    /// \brief ToEnglish
    /// \param chord in locale
    /// \return the chord name translated
    ///
    QString ToEnglish(QString chord);
    ///
    /// \brief ToLocale
    /// \return the locale chord name
    ///
    QString ToLocale(QString chord);
    ///
    /// \brief nameEnglish
    /// \return the english chord name
    ///
    QString nameEnglish();
    ///
    /// \brief nameLocale
    /// \return the locale chord name
    ///
    QString nameLocale();
    ///
    /// \brief pureNameLocale
    /// \return
    ///
    QString pureNameLocale();
    ///
    /// \brief pureNameEnglish
    /// \return  same as nameLocale but without beat, bar and degre
    /// D7(V)x2  become Re7
    ///
    QString pureNameEnglish();
    ///
    /// \brief removeDupplicateWhithoutRytm
    /// \param chords list of names of chord
    /// \param codelang : en,fr ...
    /// \return : suppress the dupplicate chords after removing the :2 x2
    ///
    static QStringList removeDupplicateWhithoutRytm(QStringList chords,QString codelang);
    ///
    /// \brief translate a chord name from codelangfrom to codelangto
    /// \param chord : a chord name
    /// \param codelangfrom :  code from language : fr or en
    /// \param minfrom : symbol for minor : m or -
    /// \param codelangto : code for language : fr or en
    /// \param minto : symbol for minor : for example m
    /// \return the chord translated
    ///
    static QString translate ( QString chord, QString codelangfrom, QString minfrom, QString codelangto, QString minto );
    ///
    /// \brief translate translate chord name from chodelangfrom to chodelangto
    /// This translate function keep unchanged the minor symbol
    /// \param chord : a chord name
    /// \param codelangfrom : code lang from
    /// \param codelangto : code lang to
    /// \return  the chord translated
    /// 
   
    ///
    /// \brief SplitChord decompose a name like D7m(V)x2  to purechord D7m 
    /// \param chord  the complete name 
    /// \param purechord : D7m
    /// \param fret : (V)
    /// \param ryhtm ! x2 
    /// \return 
    ///
    static bool SplitChord(QString chord, QString &purechord,QString &fret, QString & ryhtm );
   
   
    QString translate(QString chord, QString codelangfrom, QString codelangto);

    ///
    /// \brief up the name of the chord on the next fret
    /// \return
    ///
    QString up();
    ///
    /// \brief down the name of the chord on tne previous fret
    /// \return
    ///
    QString down();
    ///
    /// \brief transposeWithLanguage find the language and transpose
    /// \param chord the chord to transpose
    /// \param lang : chord lkanguage
    /// \param minor : minor symbol
    /// \param degre
    /// \return 
    ///
    static QString transposeWithLanguage( QString chord, QString lang, QString minor, int degre);
    ///
    /// \brief transpose : transpose a chord
    /// \param chordinenglish : the chord in english language
    /// \param degre : number of fret
    /// \return  : the transposed chord
    ///
    static QString transpose(QString chordinenglish,int degre);
    ///
    /// \brief GetCodeLang get the all codelang possible for a chorname to be in
    /// For example : getCodeLang("[D7x2]") return ("en")
    /// getCodeLang("[Lam]") return ("fr")
    /// QStringList can be empty if no codelang is found or can contains many codelang if the chord
    /// correspond to many languages.
    /// \param chord : a QString like [A(III)] [A] or [Dx2] or [Lam:2] always given between brakets
    /// \return the list of codelangs found
    ///
    static QStringList getCodeLang( QString chord);
    ///
    /// \brief getMinor return the minor codelang if found
    /// For example : getCodeLang("[D7x2]") return ("")
    /// getCodeLang("[D7mx2]") return "m"
    /// getCodeLang("[D7-x2]") return "-"
    /// \param chord a QString like [A(III)] [A] or [Dx2] or [Lam:2] always given between brakets
    /// \return the QString for minor code or nothing
    ///
    static QString getMinor( QString chord);
    ///
    /// \brief getNote return the note from a chord for example
    /// getNote("[D6]") return D
    /// \param chord the chord given with or without [ ]
    /// \return QString for the note
    ///
    static QString getNote(QString chord);

    ///
    /// \brief isChord answer true if string is a chord
    /// \param string : a QString don't starting by [ no ending with ] and with no space inside
    /// \return true if the string is a chord
    ///
    static bool isChord( QString string);
    
private:
    ///
    /// \brief m_originalchord : is the original chord name given to constuctor
    /// It can be :
    ///
    QString m_originalchord;
    ///
    /// \brief m_nbbar
    /// 2 : from D7x2
    ///
    int m_nbbar;
    ///
    /// \brief m_nbbeat
    /// 2 : from D7:2
    ///
    int m_nbbeat;
    ///
    /// \brief m_purechordLocale
    /// Translated chord
    /// Ré7(III)
    ///
    QString m_purechordLocale;
    ///
    /// \brief m_purechordEnglish
    /// chord in engliosh name
    /// D7(III)
    ///
    QString m_purechordEnglish;
    ///
    /// \brief m_chordonstrings
    /// x x 0 2 1 2 notation
    ///
    QStringList m_chordonstrings;
    ///
    /// \brief m_fret
    /// fret number : 0 1 ...
    ///
    QString m_fret;
    ///
    /// \brief m_nameLocale
    /// Name of chord in locale Ré7
    ///
    QString m_nameLocale;
    ///
    /// \brief m_nameEnglish
    ///  Name in english : D7
    ///
    QString m_nameEnglish;
    ///
    /// \brief m_notes
    ///
    LangNotes m_notes;
    ///
    /// \brief m_lang
    /// is the language : English or Français
    ///
    QString m_lang;
    ///
    /// \brief m_ring is a string list with all note :
    /// m_ring<<"A"<<"A#"<<"B"<<"C"<<"C#"<<"D"<<"D#"<<"E"<<"F"<<"F#"<<"G"<<"G#";
    QStringList m_ring;
    ///
    /// \brief m_rang
    ///rang is a map with note in key and value as value
    /// m_rang[A#]=1
    /// m_rang[Bb=1
    /// m_rang[B]=2
    ///
    QMap <QString,int> m_rang;
    ///
    /// \brief m_duration is "" "x2" ":2"
    ///
    QString m_duration;



};


#endif // CHORD_H
