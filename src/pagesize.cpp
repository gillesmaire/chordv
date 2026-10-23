#include "pagesize.h"

#include <QRegularExpression>
#include <QDebug>

PageSize::PageSize(QWidget *parent) : QComboBox(parent)
{
    m_sizes.insert("A4","210x297 mm");
    m_sizes.insert("US Letter","216x279 mm");
    m_sizes.insert("US Legal","216x356 mm");
    m_sizes.insert("US Tabloid","279x432 mm");
    m_sizes.insert("A0","841x1189 mm");
    m_sizes.insert("A1","594x841 mm");
    m_sizes.insert("A2","420x594 mm");
    m_sizes.insert("A3","297x420 mm");
    m_sizes.insert("A5","148x210 mm");
    m_sizes.insert("A6","105x148 mm");
    m_sizes.insert("A7","74x105 mm");
    m_sizes.insert("A8","52x74 mm");
    m_sizes.insert("A9","37x52 mm");
    m_sizes.insert("A10","26x37 mm");
    m_sizes.insert("B0","1414x1000 mm");
    m_sizes.insert("B1","1000x707 mm");
    m_sizes.insert("B2","707x500 mm");
    m_sizes.insert("B3","500x353 mm");
    m_sizes.insert("B4","353x250 mm");
    m_sizes.insert("B5","250x176 mm");
    m_sizes.insert("B6","176x125 mm");
    m_sizes.insert("B7","125x88 mm");
    m_sizes.insert("B8","88x62 mm");
    m_sizes.insert("B9","62x44 mm");
    m_sizes.insert("B10","44x31 mm");
    m_sizes.insert(tr("Custom"),"x");
    this->insertItems(0,m_sizes.keys());
}


QString PageSize::getUnit()
{
    QString u;
    double w,h;
    getVals(w, h, u);
    return u;
}

double PageSize::getWidth()
{
    QString u;
    double w,h;
    getVals( w,  h,  u);
    return w;
}

double PageSize::getHeight()
{
    QString u;
    double w,h;
    getVals( w,  h,  u);
    return h;
}

void PageSize::setCurrentText(const QString &text)
{
    int val=this->findText(text);
    if ( val  != -1)
            QComboBox::setCurrentText(text);
    else
       {
        QComboBox::setCurrentText(tr("Other"));
        double w,h;
        QString u;
        getVals(text,w,h,u);
       }

}

void PageSize::setCurrentTextByValue( QString value)
{
   foreach (QString k,  m_sizes.keys())
   {
       if ( m_sizes[k] == value )
       {
           setCurrentText(k);
           return ;
       }

   }
}

void PageSize::getVals(double &w, double &h, QString &u)
{
    getVals(m_sizes[currentText()],w,h,u);
}

void PageSize::getVals(QString text,double &w, double &h, QString &u)
{
    QRegularExpression exp("([0-9.,]+)x([0-9.,]+) *([a-z]{2})");
    QRegularExpressionMatch match;
    if ( text.contains(exp,&match))
    {
        QString weight=match.captured(1);
        QString height=match.captured(2);
        u=match.captured(3);
        w=weight.toDouble();
        h=height.toDouble();
    }
    else
    {
        u=QString("");
        h=w=-1;
    }
}

QString PageSize::findSize(double w,double h , bool &landscape )
{
    QString value1=QString("%1x%2 mm").arg(w).arg(h);
    QString value2=QString("%1x%2 mm").arg(h).arg(w);
    if (m_sizes.values().indexOf(value1) != -1 )
    {
        landscape=false;
        return m_sizes.values().at(m_sizes.values().indexOf(value1));
    }
    else if (m_sizes.values().indexOf(value2) != -1 )
    {
      landscape=true;
      return m_sizes.values().at(m_sizes.values().indexOf(value2));
    }
    else
    {
        landscape=false;
        return "x";
    }
}
