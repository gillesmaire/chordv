#include "fontdialog.h"

FontDialog::FontDialog(QWidget *parent) :
    QFontDialog(parent)
{
    setWindowFlags(Qt::Widget);
    setOptions(QFontDialog::NoButtons |QFontDialog::DontUseNativeDialog);
}

FontDialog::~FontDialog()
{
}
