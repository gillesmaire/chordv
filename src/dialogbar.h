#ifndef DIALOGBAR_H
#define DIALOGBAR_H

#include <QDialog>

namespace Ui {
class DialogBar;
}

///
/// \brief The DialogBar class allow to choose the bar dialog for example 4/4 or 3/4 information.
///
class DialogBar : public QDialog
{
    Q_OBJECT

public:
    ///
    /// \brief DialogBar is complient with QtDesigner allowing to promote a widget to DialogBar
    /// \param parent
    ///
    explicit DialogBar(QWidget *parent = 0);
    ~DialogBar();
    ///
    /// \brief canceled the dialog was canceled
    /// \return
    ///
    bool canceled();
    ///
    /// \brief return value like 4/4 or 3/4
    /// \return the QString value returned
    ///
    QString value();
private slots:
    void Save();
private:
    Ui::DialogBar *ui;
    bool m_cancel;
    QString m_bar;
};

#endif // DIALOGBAR_H
