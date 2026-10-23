#include "settings.h"

Settings::Settings()
{

}

Settings::Settings(QString filename):QSettings(filename,QSettings::IniFormat)
{

}

QVariant Settings::value(QString key, QVariant defaut)
{
    if (this->QSettings::value(key).isValid() )
    {
        return QSettings::value(key);
    }
    else
    {
        setValue(key,defaut);
        return defaut;
    }
}


