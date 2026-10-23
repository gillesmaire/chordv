#ifndef PAGESIZE_H
#define PAGESIZE_H

#include <QComboBox>
#include <QMultiMap>

///
/// \brief The PageSize class is a QCombobox derived type containing list of format like A4 A4 B2 ...
/// the method get the size
///
class PageSize : public QComboBox
{
    Q_OBJECT
    
public:
    ///
    /// \brief PageSize is a QtDesigner complient Widget
    /// \param parent
    ///
    PageSize( QWidget *parent);
    ///
    /// \brief getUnit return unit of QComboBox
    /// \return
    ///
    QString getUnit();
    ///
    /// \brief getWidth return the Width in mm unit
    /// \return double width
    ///
    double getWidth();
    ///
    /// \brief getHeight return the Height in mm unit
    /// \return  double height
    ///
    double getHeight();
    ///
    /// \brief setCurrentText : set custom text
    /// \param text in QString
    ///
    void setCurrentText( const QString &text);
    ///
    /// \brief findSize from width and height return format and landscape or portrait orientation
    /// \param w :double for width paper size
    /// \param h : double for height paper size
    /// \param landscape : return true if landscape
    /// \return the QString format (for example A4)
    ///
    QString findSize(double w, double h, bool &landscape);
    ///
    /// \brief setCurrentTextByValue set the current text positionned by value
    /// \param value
    ///
    void setCurrentTextByValue(QString value);
private:
    QMap <QString, QString> m_sizes;
    void getVals(double &w,double &h,QString &u);
    void getVals(QString text,double &w,double &h,QString &u);
};

#endif // PAGESIZE_H
