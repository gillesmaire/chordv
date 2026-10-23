#include "lineedittest.h"

LineEditTest::LineEditTest(QWidget *parent):QLineEdit (parent)
{

}

void LineEditTest::setTestSentence(QString text)
{
    setText(text);
}

void LineEditTest::SetFont(QFont font)
{

   this->setFont(font);
}

void LineEditTest::SetTextColor(QColor )
{
}


void LineEditTest::SetBackgroundColor(QColor)
{

}
