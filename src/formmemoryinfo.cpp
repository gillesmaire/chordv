#include "formmemoryinfo.h"
#include "ui_formmemoryinfo.h"

#include <QSettings>

FormMemoryInfo::FormMemoryInfo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormMemoryInfo)
{
    ui->setupUi(this);
    Init();
    SetUnsetPosition();
    SetUnsetAdvance(ui->spinBoxTimeBeforeStart->value());
    SetUnsetClick();
    SetSynchronisation(ui->checkBoxJackSynchronisation->isChecked());
    connect (ui->toolButtonFontMemory,SIGNAL(sendSelectedFont(QFont,QColor,QColor)),
             this,SLOT(ShowSelectedFont(QFont,QColor,QColor)));
    connect (ui->checkBoxFullScreenMemory,SIGNAL(clicked(bool)),this,SLOT(SetUnsetPosition()));
    connect (ui->spinBoxTimeBeforeStart,SIGNAL(valueChanged(int)),this,SLOT(SetUnsetAdvance(int)));
    connect (ui->checkBoxClick,SIGNAL(clicked(bool)),this,SLOT(SetUnsetClick()));
    connect (ui->checkBoxJackSynchronisation,SIGNAL(clicked(bool)),this,SLOT(SetSynchronisation(bool)));
    connect (ui->pushButtonSetAsDefault,SIGNAL(clicked(bool)),this,SLOT(SaveAsDefault()));
}


void FormMemoryInfo::SaveAsDefault()
{
    Save();
}
void FormMemoryInfo::setOffSetAsDefault()
{
    ui->pushButtonSetAsDefault->setVisible(false);
}

void FormMemoryInfo::SetUnsetAdvance(int time)
{
    ui->doubleSpinBoxAdvance->setDisabled(time<=0);
}

void FormMemoryInfo::SetUnsetClick()
{
    ui->checkBoxMarkFirst->setEnabled(ui->checkBoxClick->isChecked());
    ui->horizontalSliderVolume->setEnabled((ui->checkBoxClick->isChecked()));
}

void FormMemoryInfo::SetSynchronisation(bool val)
{
    ui->checkBoxClick->setDisabled(val);
    ui->checkBoxMarkFirst->setDisabled(val);
    ui->checkBoxShowRythmTextMemory->setDisabled(val);
    ui->horizontalSliderVolume->setDisabled(val);
    ui->labelVolume->setDisabled(val);
    emit SynchroMode(val);
}

void FormMemoryInfo::Init(QString filename)
{
    QSettings *s;
    if (filename.isEmpty())
        s=new QSettings;
    else
        s=new QSettings(filename);
    ui->checkBoxFullScreenMemory->setChecked(s->value("Memory/FullScreenBox",false).toBool());
    ui->checkBoxShowRythmTextMemory->setChecked(s->value("Memory/ShowRythm",false).toBool());
    ui->checkBoxTwoLines->setChecked(s->value("Memory/ShowTwoLines",false).toBool());
    ui->comboBoxPositionMemory->setCurrentIndex(s->value("Memory/Position",2).toInt());
    ui->toolButtonFontMemory->setColor(QColor(s->value("Memory/FontColor","#F8F3C9").toString()));
    ui->toolButtonFontMemory->setBackgroundColor(QColor(s->value("Memory/BackgroundColor","#757575").toString()));
    ui->doubleSpinBoxAdvance->setValue(s->value("Memory/Advance",3).toDouble());
    ui->spinBoxTimeBeforeStart->setValue(s->value("Memory/TimeBeforeStart",2).toInt());
    ui->comboBoxTimeBeforeUnit->setCurrentIndex(s->value("Memory/TimeBeforeUnit",0).toInt());
    ui->horizontalSliderVolume->setValue(s->value("Memory/Volume",50).toInt());
    ui->checkBoxClick->setChecked(s->value("Memory/Click",true).toBool());
    ui->checkBoxMarkFirst->setChecked(s->value("Memory/MarkFirstClick",true).toBool());
    ui->checkBoxJackSynchronisation->setChecked(s->value("Memory/JackSynchro",false).toBool());

    QFont f;
    f.fromString(s->value("Memory/Font").toString());
    ui->toolButtonFontMemory->setFont(f);
    ShowSelectedFont(ui->toolButtonFontMemory->getFont(),ui->toolButtonFontMemory->getTextColor(),ui->toolButtonFontMemory->getBackgroundColor());
}

void FormMemoryInfo::Save(QString filename)
{
    QSettings *s;
    if (filename.isEmpty())
        s=new QSettings;
    else
        s=new QSettings(filename);
    s->setValue("Memory/FullScreenBox", ui->checkBoxFullScreenMemory->isChecked());
    s->setValue("Memory/ShowRythm", ui->checkBoxShowRythmTextMemory->isChecked());
    s->setValue("Memory/Position",ui->comboBoxPositionMemory->currentIndex());
    s->setValue("Memory/ShowTwoLines",ui->checkBoxTwoLines->isChecked());
    s->setValue("Memory/Font",ui->toolButtonFontMemory->getFont().toString());
    s->setValue("Memory/FontColor",ui->toolButtonFontMemory->getTextColor().name());
    s->setValue("Memory/BackgroundColor",ui->toolButtonFontMemory->getBackgroundColor().name());
    s->setValue("Memory/Advance",ui->doubleSpinBoxAdvance->value());
    s->setValue("Memory/TimeBeforeStart",ui->spinBoxTimeBeforeStart->value());
    s->setValue("Memory/TimeBeforeUnit",ui->comboBoxTimeBeforeUnit->currentIndex());
    s->setValue("Memory/Volume",ui->horizontalSliderVolume->value());
    s->setValue("Memory/Click",ui->checkBoxClick->isChecked());
    s->setValue("Memory/MarkFirstClick",ui->checkBoxMarkFirst->isChecked());
    s->setValue("Memory/JackSynchro",ui->checkBoxJackSynchronisation->isChecked());
    s->sync();
}

QFont FormMemoryInfo::getFont()
{
    return ui->toolButtonFontMemory->getFont();
}

QColor FormMemoryInfo::getBackgroundColor()
{
    return ui->toolButtonFontMemory->getBackgroundColor();
}

QColor FormMemoryInfo::getTextColor()
{
    return ui->toolButtonFontMemory->getTextColor();
}

bool FormMemoryInfo::getShowRythm()
{
    return ui->checkBoxShowRythmTextMemory->isChecked();
}

bool FormMemoryInfo::getClick()
{
    return ui->checkBoxClick->isChecked();
}

bool FormMemoryInfo::getAccentuedFirst()
{
    return ui->checkBoxMarkFirst->isChecked();
}

int FormMemoryInfo::getVolume()
{
    return ui->horizontalSliderVolume->value();
}

int FormMemoryInfo::getDelay()
{
    return ui->spinBoxTimeBeforeStart->value();
}

bool FormMemoryInfo::getFullscreenMode()
{
    return ui->checkBoxFullScreenMemory->isChecked();
}

int FormMemoryInfo::getPosition()
{
    return ui->comboBoxPositionMemory->currentIndex();
}

bool FormMemoryInfo::getShowTwoLines()
{
     return ui->checkBoxTwoLines->isChecked();
}

bool FormMemoryInfo::getJackSynchro()
{
     return ui->checkBoxJackSynchronisation->isChecked();
}


void FormMemoryInfo::setJackSynchro(bool val)
{
    ui->checkBoxJackSynchronisation->setChecked(val);
}

void FormMemoryInfo::Retranslate()
{
    ui->retranslateUi(this);
}

double FormMemoryInfo::getAdvance()
{
     return ui->doubleSpinBoxAdvance->value();
}

int  FormMemoryInfo::getTimeBeforeUnit()
{
    return ui->comboBoxTimeBeforeUnit->currentIndex();
}

FormMemoryInfo::~FormMemoryInfo()
{
    delete ui;
}

void FormMemoryInfo::ShowSelectedFont(QFont font,QColor textcolor,QColor backgroundcolor)
{
    ui->labelExample->setFont(font);
    ui->labelExample->setStyleSheet(QString("QLabel { background-color: %1; color: %2;}").arg(backgroundcolor.name()).arg(textcolor.name()));
}


void FormMemoryInfo::SetUnsetPosition()
{
  ui ->comboBoxPositionMemory->setDisabled(ui->checkBoxFullScreenMemory->isChecked());
}
