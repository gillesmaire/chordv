#ifndef FONTCHOOSER_H
#define FONTCHOOSER_H

#include "fontdialog.h"
#include <QFontDialog>

namespace Ui {
class FontChooser;
}

class FontChooser : public QDialog
{
    Q_OBJECT

public:
    explicit FontChooser(QWidget *parent = 0);
    void setFont(QFont font, QColor text, QColor background);
    ~FontChooser();
private slots:
    void setFont();
private:
    Ui::FontChooser *ui;
signals:
   void SelectFont(QFont font,QColor text, QColor background);
};

#endif // FONTCHOOSER_H
