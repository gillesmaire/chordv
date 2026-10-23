#ifndef DIALOGABOUT_H
#define DIALOGABOUT_H

#include <QDialog>
namespace Ui {
class DialogAbout;
}

///
/// \brief The DialogAbout class show the Dialog about
///
class DialogAbout : public QDialog
{
    Q_OBJECT

public:
    ///
    /// \brief DialogAbout the constructor to show the About Dialog
    /// \param parent
    ///
    explicit DialogAbout(QWidget *parent = 0);
    ~DialogAbout();

private:
    Ui::DialogAbout *ui;
};

#endif // DIALOGABOUT_H
