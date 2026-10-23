#ifndef UTIL_H
#define UTIL_H

#include <QStringList>
/// Some utilities

///
/// \brief The ChordUtil class a set of static function given information
///
class ChordUtil
{
public:
    ///
    /// \brief LastProjects give the list of lastProjects used to add in MainWindow the list
    /// of last project opened. The list is stored in QSettings
    /// \return the QStringList with all the lastproject
    ///
    static QStringList LastProjects();
    ///
    /// \brief MemorizeProject when we add a new project save the last 10 projects ( if 10 is the number
    /// of project stored). If the project is contained in the list, all the project are stored, if the
    /// given project is new, the older will be removed.
    /// \param filename the name of the new project
    ///
    static void MemorizeProject( QString filename);
    ///
    /// \brief getLastDirectory retrieve the name of the last directory opened. It is useful to
    /// memorize the last directory open by user in QFileFialog or other.
    /// \return : the name of the last directory opened
    ///
    static QString getLastDirectory();
    ///
    /// \brief setLastDirectory set the last directory opened. See getLastDirectory()
    /// \param filename : the directory whe want to memorize
    ///
    static void setLastDirectory(QString filename);
    ///
    /// \brief toRomain convert 1 to I, 5 to V, 9 to IX
    /// \param i : the integer value
    /// \return  a string with romain notation
    ///
    static QString toRomain(int i);
    ///
    /// \brief Normalize old fonction for compatubility ( Obsolete)
    /// \param FretAnd6Strings
    /// \return
    ///
    static QStringList Normalize(QStringList FretAnd6Strings);
    ///
    /// \brief fromRomain convert from I to 1, from V to 5, from IX to 9
    /// \param i : QString of value in romain notation
    /// \return the value in integer
    ///
    static int fromRomain(QString i);
};

#endif // UTIL_H
