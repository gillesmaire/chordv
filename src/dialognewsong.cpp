#include "dialognewsong.h"
#include "ui_dialognewsong.h"
#include <QSettings>
#include <QDebug>

DialogNewSong::DialogNewSong(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogNewSong)
{
    ui->setupUi(this);
    QSettings s;
    ui->comboBoxIncludeOrder->setCurrentIndex(s.value("IncludeOrder").toInt());
    ActiveInsertButton();
    ui->lineEditTitle->clear();
    ui->lineEditSubtitle->clear();
    connect (ui->lineEditTitle,SIGNAL(textChanged(QString)),this,SLOT(ActiveInsertButton()));
    connect (ui->pushButtonCancel,SIGNAL(clicked(bool)),this,SLOT(close()));
    connect (ui->pushButtonInsert,SIGNAL(clicked(bool)),this,SLOT(Insert()));
    connect(ui->comboBoxIncludeOrder,SIGNAL(currentIndexChanged(int)),this,SLOT(SaveIncludeOrder(int)));
}


void DialogNewSong::SaveIncludeOrder(int i)
{
    QSettings s;
    s.setValue("IncludeOrder", i);
}

DialogNewSong::~DialogNewSong()
{
    delete ui;
}


void DialogNewSong::ActiveInsertButton()
{
    ui->pushButtonInsert->setDisabled( ui->lineEditTitle->text().isEmpty());
}

void DialogNewSong::Insert()
{
    emit InsertSong(ui->lineEditTitle->text(),ui->lineEditSubtitle->text(),ui->checkBoxCompress->isChecked(),ui->spinBoxColumn->value(),
            ui->spinBoxTime1->value(),ui->spinBoxTime2->value(),ui->spinBoxTempo->value(), static_cast<Insertion>(ui->comboBoxIncludeOrder->currentIndex()));
    close();
}
