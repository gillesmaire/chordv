#include "dialogcheckdatabase.h"
#include "ui_dialogcheckdatabase.h"
#include "chorddiagram.h"

#include <QSettings>
#include <QFileInfo>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>


DialogCheckDataBase::DialogCheckDataBase(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogCheckDataBase)
{
    ui->setupUi(this);

    QSettings s;
    QString filename= s.fileName();
    QFileInfo fi(filename);

    mDbOld=QSqlDatabase::database();

    QString newbasefile=fi.absolutePath()+"/ChordNew.db";
    mDbNew=QSqlDatabase::cloneDatabase(mDbOld,newbasefile);
    mDbNew.setDatabaseName(newbasefile);
    mDbNew.open();
    DisplaysUnCommonChords();
    connect (ui->pushButtonClose,SIGNAL(clicked(bool)),this,SLOT(close()));
    connect (ui->pushButtonDelete,SIGNAL(clicked(bool)),this,SLOT(DeleteSelected()));
    connect (ui->pushButtonMoveInNew,SIGNAL(clicked(bool)),this,SLOT(MoveInNew()));
    connect (ui->pushButtonMoverInActual,SIGNAL(clicked(bool)),this,SLOT(MoveInActual()));
}


void DialogCheckDataBase::DisplaysUnCommonChords()
{
    ui->tableWidgetNew->disconnect();
    ui->tableWidgetOld->disconnect();
    ui->tableWidgetNew->clear();
    ui->tableWidgetOld->clear();
    ui->tableWidgetNew->setRowCount(0);
    ui->tableWidgetOld->setRowCount(0);
    QSqlQuery qold("SELECT Name,Value FROM Chords",mDbOld);
    ui->tableWidgetNew->setColumnCount(3);
    ui->tableWidgetOld->setColumnCount(3);
    QStringList labels;
    labels<<tr("Chord")<<tr("Value")<<tr("Display");
    ui->tableWidgetNew->setHorizontalHeaderLabels(labels);
    ui->tableWidgetOld->setHorizontalHeaderLabels(labels);
    while (qold.next())
    {
        QString NameOld=qold.value(0).toString();
        QString ValueOld=qold.value(1).toString();
        QSqlQuery qnew(QString("SELECT Name,Value FROM Chords WHERE Name ='%1'").arg(qold.value(0).toString())
                       ,mDbNew);
        if ( ! qnew.next())
            addInfo( NameOld, ValueOld, QString("") , QString("")) ;
        else if ( ValueOld != qnew.value(1).toString() ){
            addInfo(NameOld,ValueOld,qnew.value(0).toString(),qnew.value(1).toString());
        }

    }
    QSqlQuery qnewc("SELECT COUNT(Name) FROM Chords",mDbNew);
    qnewc.next();
    QSqlQuery qnew("SELECT Name,Value FROM Chords ",mDbNew);
    while (qnew.next())
    {
        QSqlQuery qold(QString("SELECT Name FROM Chords WHERE Name ='%1'").arg(qnew.value(0).toString())
                       ,mDbOld);
        if ( ! qold.next())
            addInfo( QString(""),QString(""),qnew.value(0).toString(),qnew.value(1).toString() ) ;

    }
    connect(ui->tableWidgetNew,SIGNAL(cellChanged(int,int)),this,SLOT(ModifyChordNew(int,int)));
    connect(ui->tableWidgetOld,SIGNAL(cellChanged(int,int)),this,SLOT(ModifyChordActual(int,int)));
    resize(this->size());

}
DialogCheckDataBase::~DialogCheckDataBase()
{
    mDbNew.close();
    delete ui;
}

void DialogCheckDataBase::DeleteSelected()
{
    for (auto item : ui->tableWidgetNew->selectedItems() )
    {
        if ( item->column() == 0 )
            QSqlQuery query (QString("DELETE FROM chords WHERE name='%1'")
                       .arg(ui->tableWidgetNew->item(item->row(),0)->data(Qt::DisplayRole).toString()),mDbNew);
    }
    for (auto item : ui->tableWidgetOld->selectedItems() )
    {
        if ( item->column() == 0 )
            QSqlQuery query (QString("DELETE FROM chords WHERE name='%1'")
                       .arg(ui->tableWidgetOld->item(item->row(),0)->data(Qt::DisplayRole).toString()),mDbOld);
    }
    DisplaysUnCommonChords();
}


void DialogCheckDataBase::ModifyChordNew( int row, int )
{
   QString value=ui->tableWidgetNew->item(row,1)->data(Qt::DisplayRole).toString();
   QString chord=ui->tableWidgetNew->item(row,0)->data(Qt::DisplayRole).toString();
   QSqlQuery query (QString("UPDATE chords SET value='%1' WHERE name='%2'").arg(value).arg(chord),mDbNew);

}

void DialogCheckDataBase::ModifyChordActual( int row, int )
{
    QString value=ui->tableWidgetOld->item(row,1)->data(Qt::DisplayRole).toString();
    QString chord=ui->tableWidgetOld->item(row,0)->data(Qt::DisplayRole).toString();
    QSqlQuery query(QString("UPDATE chords SET value='%1' WHERE name='%2'").arg(value).arg(chord),mDbOld);
}

void DialogCheckDataBase::MoveInNew()
{
    for (auto item : ui->tableWidgetOld->selectedItems() )
    {
        if ( item->column() == 0 )
            QSqlQuery query (QString("INSERT INTO chords ( 'name', 'value', 'approved' ) VALUES ('%1','%2',0)")
                       .arg(ui->tableWidgetOld->item(item->row(),0)->data(Qt::DisplayRole).toString())
                       .arg(ui->tableWidgetOld->item(item->row(),1)->data(Qt::DisplayRole).toString()),mDbNew);
    }
    DisplaysUnCommonChords();
}

void DialogCheckDataBase::MoveInActual()
{
    for (auto item : ui->tableWidgetNew->selectedItems() )
    {
        if ( item->column() == 0 )
            QSqlQuery query (QString("INSERT INTO chords ( 'name', 'value', 'approved' ) VALUES ('%1','%2',0)")
                       .arg(ui->tableWidgetNew->item(item->row(),0)->data(Qt::DisplayRole).toString())
                       .arg(ui->tableWidgetNew->item(item->row(),1)->data(Qt::DisplayRole).toString()),mDbOld);
    }
    DisplaysUnCommonChords();
}

void DialogCheckDataBase::addInfo(QString chordold, QString valueold , QString chordnew, QString valuenew)
{
    ui->tableWidgetOld->setRowCount(ui->tableWidgetOld->rowCount()+1);
    QTableWidgetItem *item1old=new QTableWidgetItem(chordold);
    item1old->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
    ui->tableWidgetOld->setItem(ui->tableWidgetOld->rowCount()-1,0,item1old );
    ui->tableWidgetOld->setItem(ui->tableWidgetOld->rowCount()-1,1,new QTableWidgetItem(valueold));
    if ( !chordold.isEmpty() )
        ui->tableWidgetOld->setCellWidget(ui->tableWidgetOld->rowCount()-1,2,
                       new  ChordDiagram(this,mChordHeight,mChordHeight,valueold));
    ui->tableWidgetNew->setRowCount(ui->tableWidgetNew->rowCount()+1);
    QTableWidgetItem *item1new=new QTableWidgetItem(chordnew);
    item1new->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
    ui->tableWidgetNew->setItem(ui->tableWidgetNew->rowCount()-1,0,item1new);
    ui->tableWidgetNew->setItem(ui->tableWidgetNew->rowCount()-1,1,new QTableWidgetItem(valuenew));
    if ( !chordnew.isEmpty() )
        ui->tableWidgetNew->setCellWidget(ui->tableWidgetNew->rowCount()-1,2,
                       new  ChordDiagram(this,mChordHeight,mChordHeight,valuenew));
}

void DialogCheckDataBase::resizeEvent(QResizeEvent *)
{

        QFont f;
        QFontMetrics fm(f);
        int col1=fm.horizontalAdvance("Solm13(VIII)  ");
        int col2=fm.horizontalAdvance("x 12 12 12 12 12 12 ");
        ui->tableWidgetNew->setColumnWidth(0,col1);
        ui->tableWidgetNew->setColumnWidth(1,col2);
        ui->tableWidgetNew->setColumnWidth(2,ui->tableWidgetNew->width()-col1-col2);
        for ( int i =0 ; i< ui->tableWidgetNew->rowCount();i++)
            ui->tableWidgetNew->setRowHeight(i,mChordHeight);
        ui->tableWidgetOld->setColumnWidth(0,col1);
        ui->tableWidgetOld->setColumnWidth(1,col2);
        ui->tableWidgetOld->setColumnWidth(2,ui->tableWidgetNew->width()-col1-col2);
        for ( int i =0 ; i< ui->tableWidgetNew->rowCount();i++)
            ui->tableWidgetOld->setRowHeight(i,mChordHeight);

}


