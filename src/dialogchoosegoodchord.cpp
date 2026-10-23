#include "dialogchoosegoodchord.h"
#include "ui_dialogchoosegoodchord.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>

DialogChooseGoodChord::DialogChooseGoodChord(QString oldname, QString oldvalue, QString newname, QString newvalue,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogChooseGoodChord)
{
    ui->setupUi(this);
    ui->lineEditNewName->setText(newname);
    ui->lineEditOldName->setText(oldname);
    ui->widgetNew->setDiagram(newvalue);
    ui->widgetOld->setDiagram(oldvalue);
    connect (ui->pushButtonKeepOld,SIGNAL(clicked(bool)),this,SLOT(close()));
    connect (ui->pushButtonReplaceOld,SIGNAL(clicked(bool)),this,SLOT(TryToSave()));
}

DialogChooseGoodChord::~DialogChooseGoodChord()
{
    delete ui;
}


void DialogChooseGoodChord::TryToSave()
{
    QString name=ui->lineEditNewName->text();
    QString value=ui->widgetNew->getDiagram();
    QSqlQuery q(QString("DELETE FROM Chords WHERE name='%1'").arg(name));
    if ( q.lastError().isValid() )
    {
        QMessageBox::warning(this,tr("Error when delete old chord"),q.lastError().text());
        close();
    }
    else
    {
    QSqlQuery qinsert(QString ("INSERT INTO Chords (name,value) VALUES ('%1','%2')").arg(name).arg(value));
    if ( qinsert.lastError().isValid() )
        QMessageBox::warning(this,tr("Error when insert new chord"),q.lastError().text());
    close();
    }
}
