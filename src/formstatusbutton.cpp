#include "formstatusbutton.h"
#include "ui_formstatusbutton.h"

FormStatusButton::FormStatusButton(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormStatusButton)
{
    ui->setupUi(this);
    ui->labelIcon->clear();
    setStatus(false);
}

FormStatusButton::~FormStatusButton()
{
    delete ui;
}

bool FormStatusButton::status() const
{
    return mStatus;
}

void FormStatusButton::setStatus(bool status)
{
    if ( status ) ui->labelIcon->setPixmap(QPixmap(":/Image/Images/task-complete.png"));
    else  ui->labelIcon->setPixmap(QPixmap(":/Image/Images/task-reject.png"));
    mStatus = status;
}

void FormStatusButton::setLabel(QString label)
{
 ui->label->setText(label);
}
