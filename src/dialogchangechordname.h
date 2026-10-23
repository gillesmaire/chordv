#ifndef DIALOGCHANGECHORDNAME_H
#define DIALOGCHANGECHORDNAME_H

#include <QDialog>

namespace Ui {
class DialogChangeChordName;
}

///
/// \brief The DialogChangeChordName class used to change language of name of a chord by transposition
///
class DialogChangeChordName : public QDialog
{
    Q_OBJECT

public:
    ///
    /// \brief DialogChangeChordName Dialog to Change chord Name : language or transposition
    /// \param parent
    ///
    explicit DialogChangeChordName(QWidget *parent = 0);
    ~DialogChangeChordName();
    ///
    /// \brief getFromLang after change return from language
    /// \return QString for language for example  Français or English
    ///
    QString getFromLang();
    ///
    /// \brief getToLang after change return to language
    /// \return QString for language  for example Français or English
    ///
    QString getToLang();
    ///
    /// \brief getFromMin after change return Minor notation for from conversion. Exampele m or -
    /// \return  QString with letter for minor
    ///
    QString getFromMin();
    ///
    /// \brief getToMin after change return Minor notation for to conversion. Exampele m or -
    /// \return QString with letter for minor
    ///
    QString getToMin();
    ///
    /// \brief getFromLangIndex return index of FromLang combobox
    /// \return  int with the value of index of FromLang
    ///
    int getFromLangIndex();
    ///
    /// \brief getToLangIndex return index of ToLang Combobox
    /// \return  integer with the value of index of FromLang
    ///
    int getToLangIndex();
    ///
    /// \brief getToMinIndex return index of ToLang Combobox
    /// \return integer with the value of index of ToLang
    ///
    int getToMinIndex();
    ///
    /// \brief getFromMinIndex return index of FromLang Combobox
    /// \return integer with the value of index of FromLang
    ///
    int getFromMinIndex();
    ///
    /// \brief setFromLang set the index of  FromLang combobox
    /// \param lang the index of FromLang
    ///
    void setFromLang(int lang);
    ///
    /// \brief setToLang set tje index of ToFrom combobox
    /// \param min the to langa
    ///
    void setToLang(int min);
    ///
    /// \brief setFromMin idem with Minor letter
    /// \param lang
    ///
    void setFromMin(int lang);
    ///
    /// \brief setToMin same as above with To record
    /// \param min
    ///
    void setToMin(int min);
    ///
    /// \brief OkDialog the dialog return ok (true) or cancel(false)
    /// \return the bool is true only if ok button is clicked
    ///
    bool OkDialog();
private slots:
    void Convert();
private:
    bool m_convert;
    Ui::DialogChangeChordName *ui;
};

#endif // DIALOGCHANGECHORDNAME_H
