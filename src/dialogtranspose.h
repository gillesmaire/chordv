#ifndef DIALOGTRANSPOSE_H
#define DIALOGTRANSPOSE_H

#include <QDialog>

namespace Ui {
class DialogTranspose;
}
///
/// \brief The DialogTranspose class is a Dialog class to ask how many chroma negative or positive the user wants to transpose.
/// It ask if the chord has to be with parenthesis or no and ask if just the current chord has to print translated, all the chord in the current line,
/// all the chord on the current song or all the chord of the current booklet.
///
class DialogTranspose : public QDialog
{
    Q_OBJECT

public:
    ///
    /// \brief DialogTranspose the QDialog constructor
    /// \param parent
    ///
    explicit DialogTranspose(QWidget *parent = 0);
    ~DialogTranspose();

private slots:
    void ChangeValue(int i);
    void Apply();
private:
    Ui::DialogTranspose *ui;
signals:
    ///
    /// \brief AskTranspose ask change with halftonenumber, parenthesis yes or no, and range defined as just the current chord has to print translated, all the chord in the current line,
    /// all the chord on the current song or all the chord of the current booklet.
    /// \param halftonenumber
    /// \param Range
    void AskTranspose(int halftonenumber, int Range);
};

#endif // DIALOGTRANSPOSE_H
