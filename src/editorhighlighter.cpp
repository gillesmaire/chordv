#include "editorhighlighter.h"
#include <QRegularExpression>
#include <QDebug>
#include <QSettings>
EditorHighlighter::EditorHighlighter(QTextDocument *parent):QSyntaxHighlighter(parent)
{
}

void EditorHighlighter::highlightBlock(const QString &text)
{
    QSettings s;
    QColor ChordTextColor=QColor(s.value("ChordTextColor").toString());
    QColor TagsTextColor=QColor(s.value("TagsTextColor").toString());
    
    QTextCharFormat format1;
    format1.setFontWeight(QFont::Bold);
    format1.setForeground(TagsTextColor);
    QTextCharFormat format2;
    format2.setFontWeight(QFont::Bold);
    format2.setForeground(ChordTextColor);
    QTextCharFormat format3;
    format3.setFontWeight(QFont::Bold);
    format3.setForeground(Qt::darkGreen);
    QTextCharFormat format4;
    format3.setFontWeight(QFont::Medium);
    format3.setForeground(Qt::darkGreen);


    QString pattern1 = "\\{[^}]+\\}";
    QRegularExpression expression1(pattern1) ;
    QRegularExpressionMatch match;
    int index1 = text.indexOf(expression1,0,&match);
    while ( index1>=0 ) {
        int length = match.capturedLength();
        setFormat(index1, length, format1);
        index1=text.indexOf(expression1,index1+length,&match);
    }

    QString pattern2 = "\\[[^]]+\\]";
    QRegularExpression expression2(pattern2) ;
    int index2 = text.indexOf(expression2,0,&match);
    while ( index2>=0 ) {
        int length = match.capturedLength();
        setFormat(index2, length, format2);
        index2=text.indexOf(expression2,index2+length);
    }
    QRegularExpression expression3("\\{(comment|c):[^}]+\\}");
    int index3 = text.indexOf(expression3,0,&match);
    while ( index3>=0 ) {
        int length = match.capturedLength();
        setFormat(index3, length, format3);
        index3=text.indexOf(expression3,index3+length);
    }
    QRegularExpression expression4("\\{(comment_italic|ci):[^}]+\\}");
    int index4 = text.indexOf(expression4,0,&match);
    while ( index4>=0 ) {
        int length = match.capturedLength();
        setFormat(index4, length, format4);
        index4=text.indexOf(expression4,index4+length);
    }

}

