#include "spinboxunit.h"
#include <QDebug>

SpinBoxUnit::SpinBoxUnit(QWidget *parent) : QWidget(parent)
{
   m_layout=new QHBoxLayout(this);
   m_doublespinbox = new QDoubleSpinBox(this);
   m_doublespinbox->setMaximum(9999);
   m_doublespinbox->setMinimum(0);
   m_cbunit=new QComboBox(this);
   m_cbunit->addItem(tr("mm"));
   m_cbunit->addItem(tr("cm"));
   m_cbunit->addItem(tr("in"));
   m_layout->addWidget(m_doublespinbox);
   m_layout->addWidget(m_cbunit);
   m_spacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
   m_layout->addSpacerItem(m_spacer);
   m_cbunit->setMaximumWidth(100);
   m_doublespinbox->setMaximumWidth(220);
   m_value=0;
   m_unit=mm;
   this->setLayout(m_layout);
   Connect();
}

QString SpinBoxUnit::valueunit()
{
    return QString("%1%2").arg(m_value).arg(unit2String(m_unit));
}

QString SpinBoxUnit::value()
{
    return (QString("%1").arg(m_value));
}

double SpinBoxUnit::getValue()
{
    return m_value;
}

double SpinBoxUnit::getValueInUnit()
{
    return m_doublespinbox->value();
}

QString SpinBoxUnit::getUnit()
{
    if (m_cbunit->currentIndex()==0) return tr("mm");
    else if ( m_cbunit->currentIndex() == 1 ) return tr("cm");
    else return tr("in");
}

void SpinBoxUnit::Disconnect()
{
    m_doublespinbox->disconnect();
    m_cbunit->disconnect();
}

void SpinBoxUnit::Connect()
{
    connect (m_cbunit,SIGNAL(currentIndexChanged(int)),this,SLOT(changeUnit(int)));
    connect (m_doublespinbox,SIGNAL(valueChanged(double)),this,SLOT(setValue(double)));
}

void SpinBoxUnit::setValue( double val, SpinBoxUnit::unit u)
{

    Disconnect();
    m_doublespinbox->QDoubleSpinBox::setValue(val);
    m_cbunit->setCurrentIndex(unit2int(u));
    Connect();
    m_unit=u;
    m_value=toMM(val,u);

    emit valueChanged(m_value);
}

void SpinBoxUnit::setValue( double val)
{
    Disconnect();
    m_doublespinbox->QDoubleSpinBox::setValue(val);
    Connect();
    m_value=toMM(val,m_unit);
    emit valueChanged(m_value);
}

void SpinBoxUnit::changeUnit(int unit)
{
    Disconnect();
    m_unit=int2unit(unit);
    m_doublespinbox->setValue(mUnit2unit());
    Connect();
}

double SpinBoxUnit::toMM(double value, unit u)
{
    if ( u == mm ) return value;
    else if  ( u == cm ) return value*10.0;
    else  return value*10.0/2.54;
}

double SpinBoxUnit::mUnit2unit()
{
    if ( m_unit == mm ) return m_value;
    else if  ( m_unit == cm ) return m_value/10.0;
    else  return m_value*2.54/10.0;
}


double SpinBoxUnit::fromMM(double value)
{
    if ( m_unit == mm ) return value;
    else if  ( m_unit == cm ) return value/10.0;
    else  return value*2.54/10.0;
}

int SpinBoxUnit::unit2int( unit u)
{
    if ( u == mm ) return 0;
    else if ( u == cm  ) return 1;
    else return 2;
}

SpinBoxUnit::unit SpinBoxUnit::int2unit(int i)
{
    if (i==0) return mm;
    else if ( i == 1 ) return cm;
    else return in;
}

SpinBoxUnit::unit SpinBoxUnit::string2unit(QString i)
{
    if (i=="in") return in;
    else if ( i == "cm" ) return cm;
    else return mm;

}

QString SpinBoxUnit::unit2String( unit u)
{
    if ( u == mm ) return tr("mm");
    else if ( u == cm  ) return tr("cm");
    else return tr("in");
}

void SpinBoxUnit::setValue(QString valunit)
{
    SpinBoxUnit::unit u;
    QString unit=QString("(%1|%2|%3)").arg(QObject::tr("cm")).arg(QObject::tr("mm")).arg(QObject::tr("in"));
    QRegularExpression reg(QString("([0-9.,]+)(%1)").arg(unit));
    QRegularExpressionMatch match;
    if ( valunit.contains(reg,&match) )
    {
        double val=match.captured(1).toDouble();

        u=string2unit(match.captured(2));
        setValue(val,u);
    }
}

double SpinBoxUnit::getPdfU()
{
    return(m_value*72.0/25.4);
}




