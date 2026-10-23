#include "dialogsearch.h"
#include "ui_dialogsearch.h"

DialogSearch::DialogSearch(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogSearch)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose);
    connect (ui->pushButtonClose,SIGNAL(clicked(bool)),this,SLOT(close()));
    connect (ui->pushButtonSearch,SIGNAL(clicked(bool)),this,SLOT(Search()));
    connect (ui->pushButtonPrevious,SIGNAL(clicked(bool)),this,SLOT(BackwardSearch()));
    connect (ui->tooButtonErase,SIGNAL(clicked(bool)),ui->lineEditTextToSearch,SLOT(clear()));

}

DialogSearch::~DialogSearch()
{
    delete ui;
}


void DialogSearch::Search()
{
    if (! ui->lineEditTextToSearch->text().isEmpty() )
    {
        emit Search (ui->lineEditTextToSearch->text(),ui->checkBoxCaseSenstitve->isChecked(),ui->checkBoxWordONly->isChecked());
    }
}

void DialogSearch::BackwardSearch()
{
    if (! ui->lineEditTextToSearch->text().isEmpty() )
    {
        emit SearchBack(ui->lineEditTextToSearch->text(),ui->checkBoxCaseSenstitve->isChecked(),ui->checkBoxWordONly->isChecked());
    }
}
