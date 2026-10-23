#include "dialogchekallchorddefined.h"
#include "ui_dialogchekallchorddefined.h"
#include "chord.h"

#include <QDebug>
#include <QSqlQuery>
#include "QFontMetrics"

DialogChekAllChordDefined::DialogChekAllChordDefined( FormEditor *fe, QWidget *parent) :
     QDialog(parent),ui(new Ui::DialogChekAllChordDefined), mFormEditor(fe)
{
    ui->setupUi(this);
    ui->tableWidget->setColumnCount(2);
    QStringList l;
    l<<tr("Chords")<<tr("in Songs");
    ui->tableWidget->setHorizontalHeaderLabels(l);
    connect ( ui->tableWidget,SIGNAL(cellClicked(int,int)),this,SLOT(cellClicked(int,int)));
    connect (ui->pushButtonReload,SIGNAL(clicked()), this, SLOT( CheckChords()));
    connect (ui->pushButtonClose,SIGNAL(clicked()), this, SLOT( close()));
}

DialogChekAllChordDefined::~DialogChekAllChordDefined()
{
    delete ui;
}

void DialogChekAllChordDefined::cellClicked(int row, int )
{
        emit AskChord(ui->tableWidget->item(row,0)->data(Qt::DisplayRole).toString(),
                      ui->tableWidget->item(row,1)->data(Qt::DisplayRole).toString().split(",").at(0));
}

void DialogChekAllChordDefined::CheckChords()
{
    QString key;
    QRegularExpression TitleREX("^ *\\{(?:title|t): *([^}]*)\\}");
    QRegularExpression ChordREX("\\[([^]]+)\\]");
    QRegularExpressionMatch match;
    // Get all chord for each songs
    mInSongs.clear();
    mChordInSongs.clear();
    for ( auto line : mFormEditor->document()->toPlainText().split("\n"))
    {
        if ( line .contains(TitleREX,&match))
           key=match.captured(1).replace(QRegularExpression("^ +"),"").replace(QRegularExpression(" +$"),"");
         while ( line.indexOf(ChordREX,0,&match)!=-1)
        {
             QString chord=match.captured(1);
             line.replace(QString("[%1]").arg(chord),"");
             chord.replace(QRegularExpression(":.\\d*$"),"").replace(QRegularExpression("x\\d+$"),"");
             mInSongs[key]<<chord;
             Chord ch(chord);
             QString enchord=ch.ToEnglish(ch);
             mChordInSongs[enchord]<<key;
        }
    }
    // remove Duplicates
    for ( auto key : mInSongs.keys())
        mInSongs[key].removeDuplicates();
    // Chek All Chord are in the database
    mNotFound.clear();
    for ( auto key : mInSongs.keys())
    {
        for ( auto chord : mInSongs[key] )
        {
         Chord ch(chord);
         QString enchord=ch.ToEnglish(ch);
         QSqlQuery q(QString("SELECT Count(Name) FROM Chords WxHERE Name='%1'").arg(enchord));
         if ( q.next() )
         {
            if (q.value(0).toInt() == 0 )
              mNotFound<<enchord ;
         }
        }
    }
    /// list is in mNotFound
    ui->tableWidget->clearContents();
    ui->tableWidget->setSortingEnabled(false);
    ui->tableWidget->setColumnCount(2);
    ui->tableWidget->setRowCount(0);
    for ( QString chord : mNotFound )
    {
        QStringList list=mChordInSongs[chord];
        list.removeDuplicates();
        if ( list.isEmpty()) list<<"??";
        int row=ui->tableWidget->rowCount()+1;
        ui->tableWidget->setRowCount(row);
        ui->tableWidget->setItem(row-1,0,new QTableWidgetItem(chord));
        ui->tableWidget->setItem(row-1,1,new QTableWidgetItem(list.join(",")));
    }
     ui->tableWidget->setSortingEnabled(true);
}

void DialogChekAllChordDefined::resizeEvent(QResizeEvent *)
{
    QFont f;
    QFontMetrics fm(f);
    int col1=fm.horizontalAdvance("Solm13(VIII)  ");
    ui->tableWidget->setColumnWidth(0,col1);
    ui->tableWidget->setColumnWidth(1,ui->tableWidget->width()-col1);

}


