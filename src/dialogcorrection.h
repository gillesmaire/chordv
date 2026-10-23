#ifndef DIALOGCORRECTION_H
#define DIALOGCORRECTION_H

#include <QDialog>

namespace Ui {
class DialogCorrection;
}

class DialogCorrection : public QDialog
{
    Q_OBJECT

public:
    explicit DialogCorrection(QWidget *parent = nullptr);
    ~DialogCorrection();
    void setBadWord( QStringList s);
    static  bool AuthorizedWord( QString word) ;
    void setSentence( QString sentence);

private slots:
    void Add();
    void undo();
    void Close();
private:
    Ui::DialogCorrection *ui;
    QStringList bads;
signals:
    void Search(QString);
};

#endif // DIALOGCORRECTION_H
