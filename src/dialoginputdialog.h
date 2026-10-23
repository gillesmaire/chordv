#ifndef DIALOGINPUTDIALOG_H
#define DIALOGINPUTDIALOG_H

#include <QDialog>
#include <QCompleter>

namespace Ui {
class DialogInputDialog;
}

class DialogInputDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DialogInputDialog(QWidget *parent = nullptr);
    QString value() ;
    void setValue(QString label1, QString label2, QString Name);
    ~DialogInputDialog();
private:
    Ui::DialogInputDialog *ui;
    QCompleter *mCompleter;
};

#endif // DIALOGINPUTDIALOG_H
