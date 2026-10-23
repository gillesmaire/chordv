#include "logmessages.h"

#include <QDebug>
#include <QFontMetrics>
#include <QRegularExpression>

LogMessages::LogMessages(QWidget *parent):QTextEdit(parent)
{
 QFont f;
 QFontMetrics fm(f);
 this->setMinimumHeight(fm.height()*1.2);
}


QString LogMessages::ColorizeUntilColon( QString color, QString message)
{
    QRegularExpression exp("([^:]+:)(.*)");
    QRegularExpressionMatch match;
   if ( message.contains(exp,&match))
        message.replace(exp,QString("<font color=\"%1\">%2</font>%3").arg(color).arg(match.captured(1).arg(match.captured(2))));
    return message;
}

void LogMessages::Error(QString message)
{
    append(ColorizeUntilColon("red",message));
}


void LogMessages::Info(QString message)
{
    this->append(ColorizeUntilColon("green",message));
}
