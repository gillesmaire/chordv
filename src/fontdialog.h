#ifndef FONTDIALOG_H
#define FONTDIALOG_H

#include <QFontDialog>

///
/// \brief The FontDialog class is a derived class from QFontDialog to force QFontDialog to be a QWidget that can be included as a promoted Widget
///
class FontDialog : public QFontDialog
{
    Q_OBJECT

public:
    ///
    /// \brief FontDialog can be a promoted widget displaying font
    /// \param parent
    ///
    explicit FontDialog(QWidget *parent = 0);
    ~FontDialog();
};

#endif // FONTDIALOG_H
