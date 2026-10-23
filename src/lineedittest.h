#ifndef LINEEDITTEST_H
#define LINEEDITTEST_H

#include <QLineEdit>

class LineEditTest : public QLineEdit
{
public:
    LineEditTest(QWidget *parent);
    void setTestSentence ( QString text);
private:
    QFont m_font;
    QColor m_textcolor;
    QColor m_backgroundcolor;
private slots:
    void SetFont(QFont font);
    void SetBackgroundColor ( QColor color);
    void SetTextColor ( QColor color);
};

#endif // LINEEDITTEST_H
