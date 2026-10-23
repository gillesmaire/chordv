#ifndef CONSTANTS
#define CONSTANTS
#include <QString>
#include <QApplication>
#include <QDebug>
///
/// \brief The Const class set of constant classes
///
class Constants{
public:
    ///
    /// \brief The PageNumber enum can be No for no pagnumber, Center ou Outside
    ///
    enum PageNumber { No, Center, Outside };
    ///
    /// \brief The Plex enum can be Simplex it one side is printed, or Duplex if each page is printed
    ///
    enum Plex { Simplex, Duplex};
    ///
    /// \brief The PageStyle enum type of page number on button of each page
    ///  Number is 1
    ///  Number and Tiret - 1 -
    ///  NumberDivideByPageNUmber 1/50
    ///  NumberAndArrows <1>
    ///
    enum PageStyle { Number, NumberAndTiret, NumberDivideByPageNUmber, NumberAndArrows};
    ///
    /// \brief getPageNumber return No Center or Outsde from 0 1 or 2 parameter
    /// \param i
    /// \return
    ///
    static PageNumber getPageNumber( int i ) {if ( i == 1 ) return Center; else if ( i == 2 ) return Outside ; else return No; }

    enum OSIdent { Linux , Windows, Mac, Android };
    static  OSIdent getOSName()
    {
        return static_cast<OSIdent>(OSNAME);
    }
    static QString AppDataLocationRoot()
    {
        if ( getOSName() == Linux ) return ( QString("/usr/share/%1/").arg(qAppName()));
        if ( getOSName() == Mac ) return ( QString("/usr/share/%1/").arg(qAppName()));
        if ( getOSName() == Windows ) return ( QString("C:/ProgramData/%1/").arg(qAppName()));
        if ( getOSName() == Android ) return ( QString("/usr/share/%1/").arg(qAppName()));
        return(qAppName());
    }

    static QString AppDataLocation(QString lang)
    {
        return QString("%1/%2/").arg(AppDataLocationRoot()).arg(lang);
    }
}
;

#endif // CONSTANTS


