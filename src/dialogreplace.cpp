#include "dialogreplace.h"
#include "ui_dialogreplace.h"
#include <QDebug>

DialogReplace::DialogReplace(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogReplace)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose);
    connect (ui->pushButtonClose,SIGNAL(clicked(bool)),this,SLOT(close()));
    connect (ui->pushButtonSearch,SIGNAL(clicked(bool)),this,SLOT(Search()));
    connect (ui->pushButtonSkip,SIGNAL(clicked(bool)),this,SLOT(Search()));
    connect (ui->pushButtonReplace,SIGNAL(clicked(bool)),this,SLOT(Replace()));
    connect(ui->toolButtonErase1,SIGNAL(clicked(bool)),ui->lineEditTextToReplace,SLOT(clear()));
    connect(ui->toolButtonErase2,SIGNAL(clicked(bool)),ui->lineEditReplaceWith,SLOT(clear()));


}

DialogReplace::~DialogReplace()
{
    delete ui;
}

void DialogReplace::Search()
{
    if ( ui->checkBoxBackward->isChecked()) emit SearchBack(ui->lineEditTextToReplace->text(),ui->checkBoxCaseSensitive->isChecked(),ui->checkBoxWordOnly->isChecked());
    else emit Search(ui->lineEditTextToReplace->text(),ui->checkBoxCaseSensitive->isChecked(),ui->checkBoxWordOnly->isChecked());
}


void DialogReplace::Replace()
{
   qWarning()<<ui->lineEditReplaceWith->text();
   emit Replace(ui->lineEditTextToReplace->text(),ui->lineEditReplaceWith->text(),ui->checkBoxAllDocument->isChecked(),ui->checkBoxBackward->isChecked(),ui->checkBoxWordOnly->isChecked(),ui->checkBoxBackward->isChecked());
}
