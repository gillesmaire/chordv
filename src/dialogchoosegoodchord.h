#ifndef DIALOGCHOOSEGOODCHORD_H
#define DIALOGCHOOSEGOODCHORD_H

#include <QDialog>

namespace Ui {
class DialogChooseGoodChord;
}

class DialogChooseGoodChord : public QDialog
{
    Q_OBJECT

public:
    explicit DialogChooseGoodChord(QString oldname, QString oldvalue, QString newname, QString newvalue,QWidget *parent = 0);
    ~DialogChooseGoodChord();

private slots:
    void TryToSave();
private:
    Ui::DialogChooseGoodChord *ui;
signals:
    void Error(QString texterror);
};

#endif // DIALOGCHOOSEGOODCHORD_H
