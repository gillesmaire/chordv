#include "verticalspacing.h"

VerticalSpacing::VerticalSpacing(QWidget *parent):QComboBox(parent)
{
    m_values<<tr("Very compressed")<<tr("Compressed")<<tr("Regular")<<tr("Medium")<<tr("Large")
           <<tr("Extra large")<<tr("Big")<<tr("Extra Big")<<tr("Huge")<<tr("Extra Huge");
    m_datas<<0.9<<1<<1.2<<1.5<<2<<2.5<<3<<3.5<<4<<4.5;
    for ( int i=0; i<=9; i++)
        this->addItem(m_values.at(i),m_datas.at(i));
    this->setCurrentIndex(2);
}


