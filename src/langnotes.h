#ifndef LANGNOTES_H
#define LANGNOTES_H

#include <QMap>

class LangNotes
{
public:
    LangNotes();
    QString noteLocale(QString englishnote);
    QString noteEnglish(QString localenote);
    QString note(QString lang, QString note);
    QStringList EnglishNotes();
    QStringList LocalNotes();
private:
    QMap  < QString ,QString > m_note;
};

#endif // LANGNOTES_H
