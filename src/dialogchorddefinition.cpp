#include "dialogchorddefinition.h"
#include "ui_dialogchorddefinition.h"

#include "chordutil.h"
#include "logmessages.h"
#include "dialogchoosegoodchord.h"
#include "language.h"
#include "chord.h"

#include <QVector>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QTextCursor>
#include <QTextBlock>
#include <QGraphicsScene>
#include <QRegularExpression>
#include <QSqlError>
#include <QLabel>
#include <QSettings>
#include <QMessageBox>


DialogChordDefinition::DialogChordDefinition(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogChordDefinition)
{
    ui->setupUi(this);
    ui->ButtonApproved->setLabel(tr("Approved"));
    ui->ButtonDatabase->setLabel(tr("In database"));
    m_initialname=windowTitle();
    SetChordLanguage("en","-");
    m_model=new QSqlTableModel(this);
    m_model->setTable("Chords");
    m_model->setHeaderData(0,Qt::Horizontal,tr("Name"));
    m_model->setHeaderData(1,Qt::Horizontal,tr("Value"));
    m_model->setHeaderData(2,Qt::Horizontal,tr("Index"));
    m_model->setHeaderData(2,Qt::Horizontal,tr("Approved"));

    m_model->select();
    m_modelapprove= new QSqlQueryModel(this);
    m_modelapprove->setHeaderData(0,Qt::Horizontal,tr("Name"));
    m_modelapprove->setHeaderData(1,Qt::Horizontal,tr("Value"));
    m_modelapprove->setHeaderData(2,Qt::Horizontal,tr("Index"));
    ui->tableViewNonApproved->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_modelapprove->setQuery("select Name,Value,Id FROM chords where approved = 0");
    ui->tableViewNonApproved->setModel(m_modelapprove);
     m_model->setHeaderData(2,Qt::Horizontal,tr("Index"));
    ui->tableViewChordDefinition->setModel(m_model);
    ui->tableViewChordDefinition->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableViewChordDefinition->setSelectionMode(QAbstractItemView::ContiguousSelection);
    ui->pushButtonInsertChord->setVisible(false);


    m_selectionmodel = ui->tableViewChordDefinition->selectionModel();
    connect(m_selectionmodel, SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
            this, SLOT(currentRowChanged(QModelIndex,QModelIndex)));
    connect ( ui->pushButtonCancel,SIGNAL(clicked(bool)),this,SLOT(close()));
    connect (ui->pushButtonImport,SIGNAL(clicked(bool)),this,SLOT(Import()));
    connect ( ui->neck,SIGNAL(ChordsDetected(QStringList,QString)),this,SLOT(ShowChords(QStringList,QString)));
    connect (ui->listWidgetChords,SIGNAL(clicked(QModelIndex)),this,SLOT(ShowChord(QModelIndex)));
    connect (ui->listWidgetChords,SIGNAL(activated(QModelIndex)),this,SLOT(ShowChord(QModelIndex)));

    connect (ui->toolButtonPlus,SIGNAL(clicked(bool)),this,SLOT(AddChord()));
   // connect(ui->tableViewChordDefinition,SIGNAL(clicked(QModelIndex)),this,SLOT(ChordClicked(QModelIndex)));

    connect (ui->tableViewNonApproved,SIGNAL(clicked(QModelIndex)),this,SLOT(ChordClickedNonApproved(QModelIndex)));
    connect (ui->pushButtonModify,SIGNAL(clicked(bool)),this,SLOT(ModifyChord()));
    connect (ui->pushButtonDelete,SIGNAL(clicked(bool)),this,SLOT(DeleteChord()));
    connect (ui->pushButtonApprove,SIGNAL(clicked(bool)),this,SLOT(Approve()));
    connect (ui->pushButtonDeleteApprove,SIGNAL(clicked(bool)),this,SLOT(DeleteApproved()));
    connect (ui->radioButtonChordName,SIGNAL(clicked(bool)),this,SLOT(SortOnChord(bool)));
    connect (ui->radioButtonFret,SIGNAL(clicked(bool)),this,SLOT(SortOnFret(bool)));
    connect (ui->radioButtonSortIndex,SIGNAL(clicked(bool)),this,SLOT(SortOnIndex(bool)));
    connect (ui->toolButtonClearFilter,SIGNAL(clicked(bool)),ui->lineEditFilter,SLOT(clear()));
    connect (ui->lineEditFilter,SIGNAL(textChanged(QString)),this,SLOT(SetFilter(QString)));
    connect (ui->pushButtonInsertChord,SIGNAL(clicked(bool)),this,SLOT(InsertChord(bool)));
    connect (ui->tabWidget,SIGNAL(currentChanged(int)),this,SLOT(NeckUpdate(int)));
}

void DialogChordDefinition::NeckUpdate( int i)
{
    if ( i==0 )
         ui->neck->ShowChord(ui->lineEditValueIndex->text());
    else if ( i==1)
    {
        if ( ui->ButtonDatabase->status() )
        {
            bool found =false;
            int row;

            QString chord=ui->lineEditName->text().split(":").at(1);

            for ( row=0  ; row<ui->tableViewChordDefinition->model()->rowCount(); row++)
            {
                if ( chord== ui->tableViewChordDefinition->model()->index(row,1).data().toString() )
                {
                    found = true;
                    break;
                }
            }

            if (found )
            {
                ui->tableViewChordDefinition->selectRow(row);
                ui->tableViewChordDefinition->scrollTo(ui->tableViewChordDefinition->model()->index(row,0),QTableView::PositionAtTop);
            }
        }
    }
}

void DialogChordDefinition::currentRowChanged(QModelIndex index ,QModelIndex )
{

    ChordClicked( index);

}

void DialogChordDefinition::ActiveInsertButton()
{
    ui->pushButtonInsertChord->setVisible(true);
    ui->tabWidget->setCurrentIndex(1);
    ui->tabWidget->removeTab(2);
    ui->tabWidget->removeTab(3);
    this->setWindowTitle(tr("Chord insertion"));
}

void DialogChordDefinition::ShowChord(QString name)
{
    QSettings s;
    Chord chord("A");
    QString Englishname=chord.ToEnglish(name);
    setWindowTitle(m_initialname+ QString(" : %1").arg(Englishname));
    int row;
    bool found =false;
    for ( row=0  ; row<ui->tableViewChordDefinition->model()->rowCount(); row++)
    {
        if ( Englishname == ui->tableViewChordDefinition->model()->index(row,0).data().toString() )
        {
            found = true;
            break;
        }
    }
    if (found )
    {
        ui->tableViewChordDefinition->selectRow(row);
        ui->tableViewChordDefinition->scrollTo(ui->tableViewChordDefinition->model()->index(row,0),QTableView::PositionAtTop);
    }
}

void DialogChordDefinition::InsertChord(bool )
{
  if ( ! ui->lineEditNameIndex->text().isEmpty()) emit ChordToInsert(QString("[%1]").arg(ui->lineEditNameIndex->text()));
  close();
}

void DialogChordDefinition::SetFilter(QString filter)
{
    m_model->setFilter(QString("name LIKE '%1%'").arg(filter));
    m_model->select();
}

void DialogChordDefinition::SortOnFret(bool checked)
{
    if ( checked ) m_model->setSort(1,Qt::AscendingOrder);
    m_model->select();
}


void DialogChordDefinition::SortOnIndex( bool checked)
{
    if (checked ) m_model->setSort(2,Qt::AscendingOrder);
    m_model->select();
}


void DialogChordDefinition::SortOnChord( bool checked)
{
    if (checked) m_model->setSort(0,Qt::AscendingOrder);
    m_model->select();
}

void DialogChordDefinition::ModifyChord()
{
    QSqlQuery q(QString("UPDATE Chords SET name='%1', value='%1' WHERE Id='%3'")
                .arg(ui->lineEditNameIndex->text()).arg(ui->lineEditValueIndex->text())
                .arg(m_index));
    m_model->select();
}

void DialogChordDefinition::Approve()
{
    QSqlQuery q(QString("UPDATE Chords SET approved='1' WHERE Id='%3'")
                               .arg(m_indexnonapproved));
    m_modelapprove->setQuery("select Name,Value,Id FROM chords where approved=0");
}

void DialogChordDefinition::DeleteChord()
{
    foreach ( QModelIndex index, ui->tableViewChordDefinition->selectionModel()->selectedIndexes())
    {
        QSqlQuery q(QString("DELETE FROM Chords WHERE  Id=%1")
                    .arg(index.sibling(index.row(),2).data().toString()));
    }
    m_model->select();
}


void DialogChordDefinition::DeleteApproved()
{
    foreach ( QModelIndex index, ui->tableViewNonApproved->selectionModel()->selectedIndexes())
    {
        QSqlQuery q(QString("DELETE FROM Chords WHERE  Id=%1")
                    .arg(index.sibling(index.row(),2).data().toString()));
    }
    m_modelapprove->setQuery("select Name,Value,Id FROM chords where approved=0");

}

void DialogChordDefinition::ChordClicked(QModelIndex index)
{
    QString name=index.sibling(index.row(),0).data().toString();
    QString value=index.sibling(index.row(),1).data().toString();
    ui->lineEditNameIndex->setText(name);
    ui->lineEditValueIndex->setText(value);
    ui->lineEditNameTranaslatedIndex->setText ( Chord::translate(name,"en","-",m_language,m_minor));
    m_index=index.sibling(index.row(),2).data().toInt();
    ui->widget->setDiagram(ui->lineEditValueIndex->text());
    ui->neck->ShowChord(value);


}


void DialogChordDefinition::ChordClickedNonApproved(QModelIndex index)
{
    QString value=index.sibling(index.row(),1).data().toString();
    QString name=index.sibling(index.row(),0).data().toString();
    ui->lineEditNameNoApproved->setText(name);
    ui->lineEditTranslatedNameNoApproved->setText(Chord::translate(name,"en","-",Language::getCodeLang(m_language),m_minor));
    ui->lineEditValueNoApproved->setText(value);
    m_indexnonapproved=index.sibling(index.row(),2).data().toInt();
    ui->widget_2->setDiagram(ui->lineEditValueNoApproved->text());
    ui->neck->ShowChord(value);

}


DialogChordDefinition::~DialogChordDefinition()
{
    delete ui;
}

void DialogChordDefinition::SetChordLanguage(QString lang, QString minor)
{
    m_codelang=Language::getCodeLang(lang);
    if ( ! lang.isEmpty())m_language=lang;

    if ( ! minor.isEmpty())m_minor=minor;
    bool visible= ! ( m_codelang == "en" && m_minor== "-");
    ui->lineEditNameTranaslatedIndex->setVisible(visible);
    ui->lineEditTranslatedNameNoApproved->setVisible(visible);
    ui->labelTranslatedChordnName->setVisible(visible);
    ui->lineEditTranslatedChord->setVisible(visible);
}

void DialogChordDefinition::ShowChords(QStringList chordnames,QString chordstring)
{
   QString chordmin=tr("Undetermined");
   for ( QString chord : chordnames)
       if (chord.length()< chordmin.length() ) chordmin=chord;
   ui->lineEditChord->setText(chordmin);
   ui->lineEditTranslatedChord->setText(Chord::translate(chordmin,"en","-",m_language,m_minor));
   ui->listWidgetChords->clear();
   ui->listWidgetChords->addItems(chordnames);
   ui->lineEditNotation->setText(chordstring);
   ui->lineEditName->setText(findName(chordstring,chordmin));
   QStringList list=ui->lineEditName->text().split(":");
   QString name=list.first();
   QString value=list.last();
   QSqlQuery query(QString("SELECT * FROM chords WHERE Value='%2'").arg(value));
   bool present=query.next();
   ui->ButtonDatabase->setStatus(present);
   if (present) ShowChord(name) ;
   bool approved= present && query.value("Approved").toString()=="1";
   ui->ButtonApproved->setStatus(approved);
}

void DialogChordDefinition::setIcon(QLabel *label,bool status)
{
    if ( status ) label->setPixmap(QPixmap(":/Image/Images/task-complete.png"));
    else label->setPixmap(QPixmap(":/Image/Images/task-reject.png"));
}


QString DialogChordDefinition::findName( QString chordstring, QString chordname)
{
    int noteMin=100;
    int noteAfterMin=100;
    QStringList chord=chordstring.split("-");
    QVector <QString> notes(6);
    int i=0;
    for ( QString string : chord)
    {
        if ( string != "x")
        {
            int note=string.toInt();
            if ( note < noteMin)
                noteMin=note;
            notes[i]=QString("%1").arg(note);
        }
        else notes[i]="x";
        i++;
    }
    for ( QString string : chord)
    {
        if ( string != "x")
        {
            int note=string.toInt();
            if ( note > noteMin && note < noteAfterMin)
                noteAfterMin=note;
        }
    }

    if (noteMin == 0 && noteAfterMin > 2 )
        noteMin=noteAfterMin;
    if ( noteMin > 2 )
    {
        for (int j=0; j<=5;j++)
        {
           if ( notes[j] != "x" )
           {   int n=notes[j].toInt();
               n-=noteMin-1;
               if (n<0) n=0;
               notes[j]=QString("%1").arg(n);
           }
        }
    }
    else noteMin=0;

   chordname.replace(QRegularExpression("M$"),"").replace(" ","");
   if ( noteMin > 2 ) chordname=QString("%1(%2)").arg(chordname).arg(ChordUtil::toRomain(noteMin));
   return QString("%1:%2 %3 %4 %5 %6 %7 %8")
           .arg(chordname).arg(noteMin).arg(notes[0])
           .arg(notes[1]).arg(notes[2]).arg(notes[3])
           .arg(notes[4]).arg(notes[5]);
}

void DialogChordDefinition::ShowChord(QModelIndex index)
{
    QString chordname=index.data().toString();
    ui->lineEditChord->setText(chordname);
    QString newname=chordname;
    newname.replace(" ","");
    QString oldname= ui->lineEditName->text();
    QRegularExpression exp("^([^:(]+)((?:\\([^:]+){0,1}):");
    QRegularExpressionMatch match;
    if ( oldname.indexOf(exp,0,&match) != -1)
     {
      QString f=match.captured(2);
      oldname.replace(exp,QString("%1%2:").arg(newname).arg(f));
     }
    ui->lineEditName->setText(oldname);
}


void DialogChordDefinition::Import()
{
    QStringList lines=ui->plainTextEditImport->toPlainText().split("\n");
    int linenumber=0;
    foreach ( QString line, lines)
    {
        linenumber=1;
        line.replace(QRegularExpression("^\\s+"),"").replace(QRegularExpression("\\+$"),"");
        QStringList records=line.split("=");
        if ( records.count()== 2 )
        {
         QSqlQuery q(QString("INSERT INTO Chords (Name,Value) VALUES ('%1', '%2')").arg(records.at(0)).arg(records.at(1)));
         if ( q.lastError().isValid())
             setError(tr("ERROR line %1 : %2").arg(linenumber).arg(q.lastError().text()),linenumber);
        }
        else
            setError(tr("ERROR line %1 :syntax error").arg(linenumber),linenumber);
    }
    m_model->select();


}

void DialogChordDefinition::setError(QString message,int linenumber)
{
    QTextCursor cursor(ui->plainTextEditImport->document()->findBlockByLineNumber(linenumber));
    cursor.select(QTextCursor::BlockUnderCursor);
    ui->plainTextEditImport->setTextCursor(cursor);
    emit Error(message);
}

void DialogChordDefinition::paintEvent(QPaintEvent *)
{
    int neckwidht=ui->neck->width();
    int neckheight=static_cast<int>(neckwidht/10.0);
    ui->neck->setMaximumHeight(neckheight);

}


void DialogChordDefinition::AddChord()
{
    QStringList chord=ui->lineEditName->text().replace("'","").split(":");
    QString name=chord.at(0);
    QString value=chord.at(1);
    QSqlQuery q(QString("SELECT name,value FROM Chords WHERE name='%1'").arg(name));
    if (q.next())
    {
        QString oldname=q.value("name").toString();
        QString oldvalue=q.value("value").toString();
        if ( oldname==name && oldvalue==value )
            emit Error(tr("Warning : Chord name exist in the database with the same name and same value"));
        else
        {
            DialogChooseGoodChord *dial = new DialogChooseGoodChord(oldname,oldvalue,name,value,this);
            connect (dial,SIGNAL(Error(QString)),this,SIGNAL(Error(QString)));
            dial->exec();
            delete dial;
        }
    }
    else
    {
        QSqlQuery qinsert(QString ("INSERT INTO Chords (name,value,approved) VALUES ('%1','%2',1)").arg(name).arg(value));
        if ( qinsert.lastError().isValid())
            QMessageBox::warning(this,tr("Database error"),qinsert.lastError().text());
        else
        {
          ui->ButtonApproved->setStatus(true);
          ui->ButtonDatabase->setStatus(true);
        }
    }
    m_model->select();
}
