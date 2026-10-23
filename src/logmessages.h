#ifndef LOGMESSAGES_H
#define LOGMESSAGES_H

#include <QTextEdit>

class LogMessages : public QTextEdit
{
public:
    LogMessages(QWidget *parent);
public slots:
    void Error ( QString message );
    void Info (QString message);
private:
    QString ColorizeUntilColon(QString color, QString message);
};

#endif // LOGMESSAGES_H
