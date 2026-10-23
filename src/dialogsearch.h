#ifndef DIALOGSEARCH_H
#define DIALOGSEARCH_H

#include <QDialog>

namespace Ui {
class DialogSearch;
}
///
/// \brief The DialogSearch class allow to search word in Editor
///
class DialogSearch : public QDialog
{
    Q_OBJECT

public:
    ///
    /// \brief DialogSearch when the DialogSearch dialog is shown, user can choose
    /// word to search and relative features.
    /// \param parent : the parent Widget
    ///
    explicit DialogSearch(QWidget *parent = 0);
    ~DialogSearch();

private slots:
    void Search();
    void BackwardSearch();
private:
    Ui::DialogSearch *ui;
signals:
    void Search(QString text, bool casesensitive, bool wordonly);
    void SearchBack(QString text, bool casesensitive, bool wordonly);
};

#endif // DIALOGSEARCH_H
