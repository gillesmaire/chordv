#include "dialogtranspose.h"
#include "ui_dialogtranspose.h"
#include <QDebug>

DialogTranspose::DialogTranspose(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogTranspose)
{
    ui->setupUi(this);
    connect(ui->pushButtonClose,SIGNAL(clicked(bool)),this,SLOT(close()));
    connect(ui->pushButtonApply,SIGNAL(clicked(bool)),this,SLOT(Apply()));
    connect(ui->spinBoxChorma,SIGNAL(valueChanged(int)),this,SLOT(ChangeValue(int)));
    connect(ui->dialChroma,SIGNAL(valueChanged(int)),this,SLOT(ChangeValue(int)));

}


void DialogTranspose::ChangeValue(int i)
{
    QString s(sender()->objectName());
    ui->spinBoxChorma->disconnect();
    ui->dialChroma->disconnect();
    if ( s==QString("spinBoxChorma"))
        ui->dialChroma->setValue(i);
    else
        ui->spinBoxChorma->setValue(i);
    connect(ui->spinBoxChorma,SIGNAL(valueChanged(int)),this,SLOT(ChangeValue(int)));
    connect(ui->dialChroma,SIGNAL(valueChanged(int)),this,SLOT(ChangeValue(int)));
}

void DialogTranspose::Apply()
{
    emit AskTranspose( ui->spinBoxChorma->value(),ui->comboBoxRange->currentIndex());
}


DialogTranspose::~DialogTranspose()
{
    delete ui;
}
