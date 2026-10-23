#ifndef LANGUAGE_H
#define LANGUAGE_H

#include <QString>
#include <QComboBox>

///
/// \brief The Language class manage the translation files.
///
/// Translation files are stored in AppDataLocation/English/ directory
/// All the languages are in the directories English, Français etc ...
/// The file you have to provide are
/// - img : directory containing the documentation images
/// - en.html : documentation in html format ( automatic conversion by markdown) (fr.html for french)
/// - en.md : mardown language file (fr.md for french)
/// - en.chord : a chord translation file containing
///      - all the notes from C to B , separated
/// C,D,E,F,G,A,B
///      - translaton for minor :
/// -,m : to ne more EXPLAINED
///
/// en is called LanguageCode or code
/// English : is called Language
///
class Language
{
public:
    ///
    /// \brief InitLanguage init the static QMap LanguageToCode and CodeToLanguage
    ///
    static void InitLanguage();
    ///
    /// \brief getTranslationQmFileName from language file name ( English for example ) return for
    /// example /usr/share/chordV/English/en.qm (the Qt application translation file)
    /// \param language : language English, Français ...
    /// \return return the complete path and file name of qm laguage file
    ///
    static QString getTranslationQmFileName(QString language);
    ///
    /// \brief setLanguageComboBox : populate a QComboBox with all the available languages
    /// \param ptr : pointer on QComboBox object
    ///
    static void setLanguageComboBox(QComboBox *ptr);
    ///
    /// \brief getTranslationDocFileName : return from a language File ( French) the code of language (fr)
    /// the program look for the name of the html file for example fr.html ( be care take the first html file in dir)
    /// \param language : language name English, Français ...
    /// \return  the code en, fr ...
    static QString getTranslationDocFileName(QString language);
    ///
    /// \brief ListNotes return the list of notes for the language given
    /// \param language : for example English
    /// \return  : C,D,E,F,G,A,B,C or Do,Ré,Mi,Fa,Sol,La,Si
    ///
    static QStringList ListNotes(QString language);
    ///
    /// \brief ListMinor return list of suymbol in preference order for minor
    /// \param language : for example English
    /// \return -,m
    ///
    static QStringList ListMinor(QString language);
    ///
    /// \brief getCodeLang : return code lang (en) from language. The code lang is given by the name of html fist file ( en.html)
    /// \param language : for example English
    /// \return en or en
    ///
    static QString getCodeLang(QString language);
    ///
    /// \brief getLanguage return Français for codelang fr
    /// \param codelang : fr or en
    /// \return  Français or English
    ///
    static QString getLanguage(QString codelang);
    ///
    /// \brief TranslateLineWithchord translate a line containing chords from a
    /// language to an other one, ie converts all the chord between [ ]
    /// For example : Yellow [D] Submarine => Yellow [Ré] submarine
    /// \param line : the line to be translated
    /// \param fromlanguage : for example English
    /// \param frommin : minor mode for example -
    /// \param tolanguage : for example Français
    /// \param tomin : minor mode after translation for example m
    /// \return
    ///
    static QString TranslateLineWithchord(QString line, QString fromlanguage, QString frommin, QString tolanguage, QString tomin);
    ///
    /// \brief ListLanguage return the list of available languages ( English, Français ..)
    /// \return a QStringList
    ///
    static QStringList ListLanguage();
    ///
    /// \brief ListCodeLang return the list of available codelang (en,fr)
    /// \return a QStringList
    ///
    static QStringList ListCodeLang();
    ///
    /// \brief getTranslationDemoFileName return  the name of demo file
    /// \param language
    /// \return
    ///
    static QString getTranslationDemoFileName(QString language);
    static QString getFlagName(QString lang);
};
#endif // LANGUAGE_H
