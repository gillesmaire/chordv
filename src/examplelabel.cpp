#include "examplelabel.h"
#include <QTextCharFormat>
#include <QDebug>


ExampleLabel::ExampleLabel(QWidget *parent,Qt::WindowFlags ):QLabel(parent)
{


}

void ExampleLabel::setColor(QFont font, QColor color, QColor background)
{
    this->setFont(font);
    this->setStyleSheet(QString("QLabel { background-color: %1; color: %2;}").arg(background.name()).arg(color.name()));
}

