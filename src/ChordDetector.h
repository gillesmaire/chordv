#ifndef CHORDDETECT_H
#define CHORDDETECT_H


#include <QStringList>
#include <QMap>
///
/// \brief The ChordDetector class allow to detect a chord from listNotes.
/// It is used on guitar neck to transform a list of notes to QStringList of
/// chords given by the method detectChord();
/// The list of notes given to constructor are for example A, E, F
/// The algorythm is given buy the tblscitype of the ChordV database.
/// The ChordV Database containes two tables : tblscitype and Chords.
/// - **tblscitype** allows ot find txtcode of chord from interval
/// - **chords**  keep chords with A7(IV) or A6 notation and the correspondance 4212222
///   or 0x02020
///
class ChordDetector
{
public:
    ///
    /// \brief ChordDetector the constuctor is called with list of notes given as a QStringList,
    /// for example A,E,F.
    /// \param listNotes : QStringList with all notes found on the neck. Th duplicated notes are removed.
    ///
    ChordDetector( QStringList listNotes);
    ///
    /// \brief detectChord return the list of chord found.
    /// \return
    ///
    QStringList detectChord ();
private:
    int note2num(QString note);
    QString num2spellingElement(int inNum);
    int spellingElement2num(QString inSc);
    QStringList orderSpelling(QStringList spelling);
    QString convertToCodeIfPossible(QStringList spelling);
    QMap<int, QMap<int, QString> > getModeArray(QString spelling);
    QList<QStringList> m_list;
    void round(QStringList &list, int i);
    void CalculateAllChords(QStringList list, int first);
    QList<QStringList> getAllChords();
    QList<QStringList> m_listAllChords;
    QStringList m_listNotes;

};

#endif
