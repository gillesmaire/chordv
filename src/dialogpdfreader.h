#ifndef DIALOGPDFREADER_H
#define DIALOGPDFREADER_H

#include <QDialog>

namespace Ui {
class DialogPdfReader;
}

class DialogPdfReader : public QDialog
{
    Q_OBJECT

public:
    explicit DialogPdfReader(QString filename, QWidget *parent = nullptr);
    ~DialogPdfReader();

private:
    Ui::DialogPdfReader *ui;
};

#endif // DIALOGPDFREADER_H
