#include "dialoginputdialog.h"
#include "ui_dialoginputdialog.h"
#include <QSqlQuery>
#include <QSettings>
#include <QDebug>

DialogInputDialog::DialogInputDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogInputDialog)
{
    ui->setupUi(this);
}


QString DialogInputDialog::value()
{
    return ui->lineEdit->text();
}

void DialogInputDialog::setValue(QString label1, QString label2, QString Name)
{
    setWindowTitle(label1);
    ui->label->setText(label2);
    QSettings s;
    ui->lineEdit->setText(s.value(QString("%1Name").arg(Name)).toString());
}

DialogInputDialog::~DialogInputDialog()
{
    delete ui;
}
