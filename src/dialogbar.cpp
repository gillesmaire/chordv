#include "dialogbar.h"
#include "ui_dialogbar.h"

DialogBar::DialogBar(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogBar)
{
    ui->setupUi(this);
    m_cancel=true;
    connect (ui->pushButtonCancel,SIGNAL(clicked(bool)),this,SLOT(close()));
    connect (ui->pushButtonOk,SIGNAL(clicked(bool)),this,SLOT(Save()));
}

DialogBar::~DialogBar()
{
    delete ui;
}


void DialogBar::Save()
{
    m_cancel=false;
    m_bar=QString("%1/%2").arg(ui->spinBox1->value()).arg(ui->spinBox2->value());
    close();
}

bool DialogBar::canceled()
{
    return m_cancel;
}

QString DialogBar::value()
{
    return m_bar;
}
