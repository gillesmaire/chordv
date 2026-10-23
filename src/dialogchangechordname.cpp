#include "dialogchangechordname.h"
#include "ui_dialogchangechordname.h"

DialogChangeChordName::DialogChangeChordName(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogChangeChordName)
{
    ui->setupUi(this);
    m_convert=false;
    connect(ui->pushButtonCancel,SIGNAL(clicked(bool)),this,SLOT(close()));
    connect(ui->pushButtonConvert,SIGNAL(clicked(bool)),this,SLOT(Convert()));
}

DialogChangeChordName::~DialogChangeChordName()
{
    delete ui;
}

QString DialogChangeChordName::getToLang()
{
    return ui->widget->getOutputLang();
}


QString DialogChangeChordName::getFromMin()
{
    return ui->widget->getInputMinor();
}

QString DialogChangeChordName::getToMin()
{
    return ui->widget->getOutputMinor();
}

int DialogChangeChordName::getFromLangIndex()
{
    return ui->widget->getInputLangIndex();
}

int DialogChangeChordName::getToLangIndex()
{
    return ui->widget->getOutputLangIndex();
}

int DialogChangeChordName::getToMinIndex()
{
    return ui->widget->getOutputMinorIndex();
}

int DialogChangeChordName::getFromMinIndex()
{
    return ui->widget->getInputMinorIndex();
}





void DialogChangeChordName::setToMin(int min)
{
    ui->widget->setOutputMinor(min);
}

void DialogChangeChordName::setFromMin(int min)
{
    ui->widget->setInputMinor(min);
}

void DialogChangeChordName::setFromLang(int language)
{
    ui->widget->setInputLang(language);
}

void DialogChangeChordName::setToLang(int language)
{
    ui->widget->setOutputLang(language);
}


QString DialogChangeChordName::getFromLang()
{
    return ui->widget->getInputLang();
}

void DialogChangeChordName::Convert()
{
    m_convert=true;
    close();
}

bool DialogChangeChordName::OkDialog()
{
    return true;
}
