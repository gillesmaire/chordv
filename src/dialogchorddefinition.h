#ifndef DIALOGCHORDDEFINITION_H
#define DIALOGCHORDDEFINITION_H

#include <QDialog>
#include <QHash>
#include <QGraphicsScene>
#include <QModelIndex>
#include <QSqlTableModel>
#include <QTableView>
#include <QSqlQueryModel>
#include <QLabel>
#include <QModelIndex>
#include <QItemSelectionModel>

#include <QPaintEvent>

namespace Ui {
class DialogChordDefinition;
}



class DialogChordDefinition : public QDialog
{
    Q_OBJECT

public:
    ///
    /// \brief DialogChordDefinition show the chord in 4 QTableWidgets:
    /// - 1 : the neck
    /// - 2  : in the list of chord
    /// - 3 : in the list of non approved chords
    /// - 4 : in list of imported chord
    /// \param parent
    ///
    explicit DialogChordDefinition(QWidget *parent = 0);
    ~DialogChordDefinition();
    ///
    /// \brief SetChordLanguage : if the chords are in other language give the language
    /// \param lang : fr, en ..
    /// \param minor : -, m for minor
    ///
    void SetChordLanguage(QString lang, QString minor);
    ///
    /// \brief ActiveInsertButton : if Actived the insert button allows to insert chord in texteditor
    ///
    void ActiveInsertButton();
    ///
    /// \brief ShowChord Show chord display the chord in the ist of approved chord ( index 2 in QTabWidgets)
    /// and in the neck (index 1 in QTabWidgets)
    /// \param name :  name of chord
    ///
    void ShowChord(QString name);
protected slots:
    void ShowChords(QStringList chordnames,QString chortdstring);
private slots:
    void ShowChord(QModelIndex index);
    void Import();
    void ChordClicked(QModelIndex);
    void ModifyChord();
    void DeleteChord();
    void AddChord();
    void SortOnFret(bool checked);
    void SortOnIndex(bool checked);
    void SortOnChord(bool checked);
    void SetFilter(QString filter);
    void InsertChord(bool);
    void ChordClickedNonApproved(QModelIndex index);
    void Approve();
    void DeleteApproved();
    void currentRowChanged(QModelIndex index1, QModelIndex index2);
    void NeckUpdate(int i);
private:
    Ui::DialogChordDefinition *ui;
    QHash <QString, QString> m_chords;
    QString findName(QString chordstring, QString chordmin);
    QSqlTableModel *m_model;
    QSqlQueryModel *m_modelapprove;
    QItemSelectionModel *m_selectionmodel;
    int m_index;
    int m_indexnonapproved;
    QString m_codelang;
    QString m_language;
    QString m_minor;

    void setError(QString message, int linenumber);
    void paintEvent(QPaintEvent *event);
    void setIcon(QLabel *label, bool status);
    QString m_initialname;
signals:
    void Error(QString message);
    void ChordToInsert(QString chord);
};


#endif // DIALOGCHORDDEFINITION_H
