#ifndef DIALOGSYSTEMINFO_H
#define DIALOGSYSTEMINFO_H

#include <QDialog>

namespace Ui {
class DialogSystemInfo;
}
///
/// \brief The DialogSystemInfo class this dialog show a set of system information like
/// date of compilation, release version, Git SHA1 version, program arguments, configuration file,
/// Database location, PID of process, location of language information
///
class DialogSystemInfo : public QDialog
{
    Q_OBJECT

public:
    ///
    /// \brief DialogSystemInfo the constructor open the dialog window with parent giver in argument
    /// \param parent
    ///
    explicit DialogSystemInfo(QWidget *parent = 0);
    ~DialogSystemInfo();

private:
    Ui::DialogSystemInfo *ui;
};

#endif // DIALOGSYSTEMINFO_H
