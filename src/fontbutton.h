#ifndef FONTBUTTON_H
#define FONTBUTTON_H

#include <QToolButton>


class FontButton : public QToolButton
{
    Q_OBJECT
public:
    FontButton(QWidget *parent);
    void setFamily( QString family) ;
    void setColor ( QColor color) ;
    void setBackgroundColor ( QColor color ) ;
    void setWeight(int i) ;
    void setItalic(bool b);
    void setPointsize ( int size);
    void setFont(QFont font);
    QFont getFont() { return m_font;}
    QColor getBackgroundColor (){return m_backgroundcolor;  }
    QColor getTextColor (){return m_textcolor;}
public slots:
    void SelectedFont(QFont font, QColor text, QColor background);

private:
    QColor m_textcolor;
    QColor m_backgroundcolor;
    QFont m_font;
private slots:
    void setFont(bool );

signals:
    void sendSelectedFont(QFont font, QColor text, QColor background);
};

#endif // FONTBUTTON_H
