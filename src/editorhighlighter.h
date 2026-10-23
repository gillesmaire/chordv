#ifndef EDITORHIGHLIGHTER_H
#define EDITORHIGHLIGHTER_H

#include <QSyntaxHighlighter>

class EditorHighlighter : public QSyntaxHighlighter
{
public:
    EditorHighlighter(QTextDocument *parent);
    void highlightBlock(const QString &text);
};

#endif // EDITORHIGHLIGHTER_H
