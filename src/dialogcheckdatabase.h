#ifndef DIALOGCHECKDATABASE_H
#define DIALOGCHECKDATABASE_H

#include <QDialog>
#include <QSqlDatabase>

namespace Ui {
class DialogCheckDataBase;
}

class DialogCheckDataBase : public QDialog
{
    Q_OBJECT

public:
    explicit DialogCheckDataBase(QWidget *parent = nullptr);
    ~DialogCheckDataBase() override;

private slots:
    void ModifyChordNew(int row, int col);
    void ModifyChordActual(int row, int col);
    void DeleteSelected();
    void MoveInNew();
    void MoveInActual();
private:
    Ui::DialogCheckDataBase *ui;
    void addInfo(QString chorold, QString valueold, QString chordnew, QString valuenew);
    void resizeEvent( QResizeEvent *e) override ;
    void DisplaysUnCommonChords();
    int mChordHeight=60;
    QSqlDatabase mDbOld, mDbNew;
};

#endif // DIALOGCHECKDATABASE_H
