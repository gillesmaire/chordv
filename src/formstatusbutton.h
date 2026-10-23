#ifndef FORMSTATUSBUTTON_H
#define FORMSTATUSBUTTON_H

#include <QWidget>

namespace Ui {
class FormStatusButton;
}

class FormStatusButton : public QWidget
{
    Q_OBJECT

public:
    explicit FormStatusButton(QWidget *parent = nullptr);
    ~FormStatusButton();
    bool status() const;
    void setStatus(bool status);
    void setLabel( QString label);

private:
    Ui::FormStatusButton *ui;
    bool mStatus;
};

#endif // FORMSTATUSBUTTON_H
