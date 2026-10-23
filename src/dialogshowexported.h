#ifndef DIALOGSHOWEXPORTED_H
#define DIALOGSHOWEXPORTED_H

#include <QDialog>
#include <QMap>

namespace Ui {
class DialogShowExported;
}

class DialogShowExported : public QDialog
{
    Q_OBJECT

public:
    explicit DialogShowExported(QWidget *parent = nullptr);
    ~DialogShowExported();

    void setText(QString text);
private:
    Ui::DialogShowExported *ui;
    void ReadDefaultConfig();
    QHash<QString,QString> mKeys;
};

#endif // DIALOGSHOWEXPORTED_H
