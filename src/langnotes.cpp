#include "langnotes.h"

#include <QApplication>

LangNotes::LangNotes()
{
    m_note["A"]=qApp->translate("LangNote","A");
    m_note["B"]=qApp->translate("LangNote","B");
    m_note["C"]=qApp->translate("LangNote","C");
    m_note["D"]=qApp->translate("LangNote","D");
    m_note["E"]=qApp->translate("LangNote","E");
    m_note["F"]=qApp->translate("LangNote","F");
    m_note["G"]=qApp->translate("LangNote","G");
}

QString LangNotes::noteLocale(QString englishnote)
{
    return m_note[englishnote];
}

QString LangNotes::noteEnglish(QString localenote)
{
    foreach ( QString note, m_note.keys())
        if (m_note[note]==localenote) return note;
    return QString("ERR");
}

QString LangNotes::note(QString lang, QString note)
{
    if (lang!= "en") return noteLocale(note);
    else return noteEnglish(note);
}


