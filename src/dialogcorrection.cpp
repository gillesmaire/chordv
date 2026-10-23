#include "dialogcorrection.h"
#include "ui_dialogcorrection.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

DialogCorrection::DialogCorrection(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogCorrection)
{
    ui->setupUi(this);
    connect (ui->pushButtonCorrec,SIGNAL(clicked()),this,SLOT(Close()));
    connect (ui->pushButtonAdd,SIGNAL(clicked()),this,SLOT(Add()));
    connect (ui->pushButtonClose,SIGNAL(clicked()),this,SLOT(close()));
    connect (ui->pushButtonUndo,SIGNAL(clicked()),this,SLOT(undo()));
}

void DialogCorrection::Close()
{
    emit Search(ui->lineEdit->text());
    close();
}
void DialogCorrection::undo()
{
    QString req(QString("DELETE FROM Dico WHERE rowid = ( SELECT MAX(rowid) FROM Dico) "));
    QSqlQuery q(req);
    q.exec();
}

DialogCorrection::~DialogCorrection()
{
    delete ui;
}
void DialogCorrection::Add()
{
    QString req(QString("INSERT INTO Dico (mot) VALUES ('%1')").arg(ui->lineEdit->text()))  ;
    QSqlQuery q(req);
    q.exec();
    if ( bads.isEmpty() )ui->stackedWidget->setCurrentIndex(1);
    else
    {
    QString bad=bads.takeFirst();
    QString req(QString("SELECT  mot FROM Dico WHERE ( mot='%1')").arg(bad))  ;
    QSqlQuery q(req);
    if ( ! q.next())
       ui->lineEdit->setText(bad);
    }
}
void DialogCorrection::setBadWord(QStringList s)
{
    bads=s;
    if (bads.isEmpty()) ui->stackedWidget->setCurrentIndex(1);
    else {
    QString bad=bads.takeFirst();
    QString req(QString("SELECT  mot FROM Dico WHERE ( mot='%1')").arg(bad))  ;
    QSqlQuery q(req);
    if ( ! q.next())
       ui->lineEdit->setText(bad);
    }
}

bool DialogCorrection::AuthorizedWord(QString word)
{
    QString req(QString("SELECT  mot FROM Dico WHERE ( mot='%1')").arg( word))  ;
    QSqlQuery q(req);
    return ( q.next() );
}

void DialogCorrection::setSentence(QString sentence)
{
    ui->textEdit->setPlainText(sentence);
}
