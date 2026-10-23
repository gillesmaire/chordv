#ifndef DIALOGCHEKALLCHORDDEFINED_H
#define DIALOGCHEKALLCHORDDEFINED_H

#include <QDialog>
#include <QMap>
#include "formeditor.h"

namespace Ui {
class DialogChekAllChordDefined;
}

class DialogChekAllChordDefined : public QDialog
{
    Q_OBJECT

public:
    explicit DialogChekAllChordDefined(FormEditor *fe, QWidget *parent = nullptr);
    ~DialogChekAllChordDefined() override;
public slots:
    void CheckChords();

private slots:
    void cellClicked(int row, int col);
private:
    Ui::DialogChekAllChordDefined *ui;
    FormEditor *mFormEditor;
    QStringList  mNotFound;
    QMap <QString, QStringList> mInSongs;
    QMap <QString, QStringList> mChordInSongs;
    void resizeEvent(QResizeEvent *e) override;
signals:
    void AskChord(QString chord, QString song);
};

#endif // DIALOGCHEKALLCHORDDEFINED_H
