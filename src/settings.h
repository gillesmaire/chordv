#ifndef SETTINGS_H
#define SETTINGS_H

#include <QSettings>
#include <QVariant>

///
/// \brief The Settings class derivate the QSettings class by forcing INI format when constructor is called
/// whith a file name.
///
/// Also it allow a default value in value.
///
class Settings : public QSettings
{
public:
    ///
    /// \brief Settings constructor
    ///
    Settings();
    ///
    /// \brief Settings constructor by transtypage
    /// \param filename
    ///
    Settings(QString filename);
    ///
    /// \brief value return the value with a default value
    /// \param key : the key as a string
    /// \param defaut : the default value as a variant
    /// \return the result value
    ///
    QVariant value(QString key, QVariant defaut);
};

#endif // SETTINGS_H
