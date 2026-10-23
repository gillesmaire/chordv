#ifndef COLORBUTTON_H
#define COLORBUTTON_H

#include <QToolButton>

///
/// \brief The ColorButton class allows to draw the color of QToolButton when
/// user choose a color after click on button. The allow to show the color
/// chooser by the button.
/// This class is QtDesigner/QtCreator complient
///
class ColorButton : public QToolButton
{
    Q_OBJECT
public:
    ///
    /// \brief ColorButton construtor with the parent widget argument allowing to
    /// use in QtDesigner
    /// \param parent : the parent
    ///
    ColorButton(QWidget *parent=0);
    ///
    /// \brief getColor get the color of the button
    /// \return the color in QColor type
    ///
    QColor getColor(){ return m_color;}
    ///
    /// \brief setColor set the color of the button
    /// \param color giver with QColor attribute
    ///
     void setColor(QColor color);
private:
    QColor m_color;
private slots:
    void setColor(bool);
signals:
    void ColorChanged(QColor color);
};

#endif // COLORBUTTON_H
