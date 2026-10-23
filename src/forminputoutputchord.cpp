#include "forminputoutputchord.h"
#include "ui_forminputoutputchord.h"
#include "language.h"

FormInputOutputChord::FormInputOutputChord(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormInputOutputChord)
{
    ui->setupUi(this);
    Language::setLanguageComboBox(ui->comboBoxInputChordLanguage);
    Language::setLanguageComboBox(ui->comboBoxOutputChordLanguage);
    ui->comboBoxMinorInput->addItems(Language::ListMinor(getInputLang()));
    ui->comboBoxMinorOutput->addItems(Language::ListMinor(getOutputLang()));
    SetInvisibleIfOnlyOnceChoice(ui->comboBoxMinorInput,ui->labelMinorInput);
    SetInvisibleIfOnlyOnceChoice(ui->comboBoxMinorOutput,ui->labelMinorOutput);
    connect (ui->comboBoxInputChordLanguage,SIGNAL(currentTextChanged(QString)),this,SLOT(LanguageInputChanged(QString)));
    connect (ui->comboBoxOutputChordLanguage,SIGNAL(currentTextChanged(QString)),this,SLOT(LanguageOutputChanged(QString)));

}

QString FormInputOutputChord::getInputLang()
{
    return ui->comboBoxInputChordLanguage->currentText();
}


void FormInputOutputChord::setOutputMinor(int index)
{
    ui->comboBoxMinorOutput->setCurrentIndex(index);
}

void FormInputOutputChord::setInputMinor(int index)
{
    ui->comboBoxMinorInput->setCurrentIndex(index);
}

void FormInputOutputChord::setInputLang(int index)
{
    ui->comboBoxInputChordLanguage->setCurrentIndex(index);
}

void FormInputOutputChord::setOutputLang(int index)
{
    ui->comboBoxOutputChordLanguage->setCurrentIndex(index);
}

QString FormInputOutputChord::getOutputLang()
{
    return ui->comboBoxOutputChordLanguage->currentText();
}

QString FormInputOutputChord::getInputMinor()
{
    return ui->comboBoxMinorInput->currentText();
}

QString FormInputOutputChord::getOutputMinor()
{
    return ui->comboBoxMinorOutput->currentText();
}

int FormInputOutputChord::getInputLangIndex()
{
    return ui->comboBoxInputChordLanguage->currentIndex();
}

int FormInputOutputChord::getOutputLangIndex()
{
    return ui->comboBoxOutputChordLanguage->currentIndex();
}

int FormInputOutputChord::getInputMinorIndex()
{
    return ui->comboBoxMinorInput->currentIndex();
}

int FormInputOutputChord::getOutputMinorIndex()
{
    return ui->comboBoxMinorOutput->currentIndex();
}

FormInputOutputChord::~FormInputOutputChord()
{
    delete ui;
}


void FormInputOutputChord::SetInvisibleIfOnlyOnceChoice( QComboBox *ptr , QLabel *ptrl)
{
    ptr->setVisible(ptr->count()>1);
    ptrl->setVisible(ptr->count()>1);

}

void FormInputOutputChord::LanguageOutputChanged(QString )
{
    ui->comboBoxMinorOutput->clear();
    ui->comboBoxMinorOutput->addItems(Language::ListMinor(getOutputLang()));
    SetInvisibleIfOnlyOnceChoice(ui->comboBoxMinorOutput,ui->labelMinorOutput);
}

void FormInputOutputChord::LanguageInputChanged(QString     )
{
    ui->comboBoxMinorInput->clear();
    ui->comboBoxMinorInput->addItems(Language::ListMinor(getInputLang()));
    SetInvisibleIfOnlyOnceChoice(ui->comboBoxMinorInput,ui->labelMinorInput);
}

