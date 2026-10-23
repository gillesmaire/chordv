#include "formconfig.h"
#include "ui_formconfig.h"


#include "colorbutton.h"
#include "examplelabel.h"
#include "settings.h"
#include "language.h"
#include "constants.h"

#include <QDebug>
#include <QRegularExpression>
#include <QLineEdit>
#include <QSettings>

void FormConfig::setOffButtonSetAsDefault()
{
    ui->pushButtonSetAsDefault1->setVisible(false);
    ui->pushButtonSetAsDefault2->setVisible(false);
}

FormConfig::FormConfig(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormConfig)
{
    ui->setupUi(this);
    SetConfigFromInit();
    Language::setLanguageComboBox(ui->comboBoxChordLang);
    m_parent=parent;
    connect (ui->checkBoxCover,SIGNAL(stateChanged(int)),this,SLOT(setCover(int)));
    foreach (FontButton *w ,m_parent->findChildren<FontButton*>())
    connect (w,SIGNAL(sendSelectedFont(QFont,QColor,QColor)),this,SLOT(displayFont(QFont,QColor,QColor)));
    connect(ui->comboBoxMediaBox,SIGNAL(currentTextChanged(QString)),this,SLOT(SizeChanged(QString)));
    connect(ui->checkBoxLandscape,SIGNAL(clicked(bool)),this,SLOT(SizeChanged(bool)));
    connect(ui->toolButtonCoverImage,SIGNAL(ImageSelelected(QString)),this,SLOT(displayThumb(QString)));
    connect(ui->spuPageHeight,SIGNAL(valueChanged(double)),this,SLOT(FindSize(double)));
    connect(ui->spuPageWidth,SIGNAL(valueChanged(double)),this,SLOT(FindSize(double)));
    connect(ui->toolButtonSuppressCoverImage,SIGNAL(clicked(bool)),this,SLOT(deleteCoverImage(bool)));
    connect (ui->comboBoxPageNumberStyle,SIGNAL(currentIndexChanged(int)),this,SLOT(CheckArrow(int)));
    connect (ui->pushButtonSetAsDefault1,SIGNAL(clicked(bool)),this,SLOT(SaveAsConfig()));
    connect (ui->pushButtonSetAsDefault2,SIGNAL(clicked(bool)),this,SLOT(SaveAsConfig()));
}

void FormConfig::SizeChanged(bool)
{
    Calculate();
}

void FormConfig::SizeChanged(QString)
{
    Calculate();
}


void FormConfig::CheckArrow(int i )
{
    if ( i == Constants::NumberAndArrows )
    {
        ui->comboBoxPageNumber->setCurrentIndex(Constants::Center);
        ui->comboBoxPageNumber->setDisabled(true);
    }
    else
    {
        ui->comboBoxPageNumber->setDisabled(false);
    }
}

void FormConfig::Calculate()
{
    double height=ui->comboBoxMediaBox->getHeight();
    double width=ui->comboBoxMediaBox->getWidth();
    QString unit=ui->comboBoxMediaBox->getUnit();
     if ( ui->checkBoxLandscape->isChecked() )
     {
         double l=width;
         width=height;
         height=l;
     }
    ui->spuPageHeight->setValue(height,ui->spuPageHeight->string2unit(unit));
    ui->spuPageWidth->setValue(width,ui->spuPageWidth->string2unit(unit));
}

void FormConfig::FindSize(double)
{
    bool landscape;
    QString val=ui->comboBoxMediaBox->findSize(ui->spuPageWidth->getValue(),ui->spuPageHeight->getValue(),landscape);
    ui->comboBoxMediaBox->disconnect();
    ui->checkBoxLandscape->disconnect();
    ui->comboBoxMediaBox->setCurrentTextByValue(val);
    ui->checkBoxLandscape->setChecked(landscape);
    connect(ui->comboBoxMediaBox,SIGNAL(currentTextChanged(QString)),this,SLOT(SizeChanged(QString)));
    connect(ui->checkBoxLandscape,SIGNAL(clicked(bool)),this,SLOT(SizeChanged(bool)));
}

FormConfig::~FormConfig()
{
    delete ui;
}


void FormConfig::disableWidgets(QRegularExpression value)
{
    foreach (QLineEdit *w , m_parent->findChildren<QLineEdit*>(value))
        w->setVisible(false);
    foreach (QLabel *w ,m_parent->findChildren<QLabel*>(value))
        w->setVisible(false);
    foreach (QToolButton *w ,m_parent->findChildren<QToolButton*>(value))
        w->setVisible(false);
    foreach (SpinBoxUnit *w ,m_parent->findChildren<SpinBoxUnit*>(value))
        w->setVisible(false);
    foreach (QCheckBox *w ,m_parent->findChildren<QCheckBox*>(value))
        w->setVisible(false);
    foreach (QComboBox *w ,m_parent->findChildren<QComboBox*>(value))
        w->setVisible(false);

}

void FormConfig::disableGroup(QString group)
{
    foreach (QGroupBox *w ,m_parent->findChildren<QGroupBox*>(group))
        w->setVisible(false);
}

void FormConfig::setProjectPath(QString path)
{
    m_projectpath=path;
    ui->toolButtonCoverImage->setProjectPath(path);
}

void FormConfig::setCover(int val)
{

    ui->labelCoverImage->setEnabled(val);
    ui->toolButtonCoverImage->setEnabled(val);
    ui->comboBoxTitlePosition->setEnabled(val);
    ui->toolButtonSuppressCoverImage->setEnabled(val);
}



void FormConfig::setValue(QString var, QVariant value)
{
    QRegularExpression spu ("^spu(.*)") ;
    if ( var.contains(spu))
    {
        for (auto w : m_parent->findChildren<SpinBoxUnit*>(var)) w->setValue(value.toString());

    }
    if ( var.endsWith("Font"))
    {
        QFont f;
        f.fromString(value.toString());
        foreach (FontButton *w ,m_parent->findChildren<FontButton*>(QString("toolButton")+var))  w->setFont(f);
    }
    else if ( var.endsWith("FontColor"))
    {
        foreach (FontButton *w ,m_parent->findChildren<FontButton*>(QString("toolButton")+var))  w->setColor(QColor(value.toString()));
    }
    else if ( var.endsWith("FontBackgroundColor"))
    {
        foreach (FontButton *w ,m_parent->findChildren<FontButton*>(QString("toolButton")+var))  w->setBackgroundColor(QColor(value.toString()));
    }
    else if ( var==QString("Cover") )
    {
         ui->checkBoxCover->setChecked(value.toInt()==1);
    }
  else if ( var.startsWith("comboBox") )
    {
            foreach (QComboBox *w , m_parent->findChildren<QComboBox*>(var)) w->setCurrentText(value.toString());
    }
    else if ( var.startsWith("spinBox"))
    {
           foreach (QSpinBox *w ,m_parent->findChildren<QSpinBox*>(var)) w->setValue(value.toInt());

    }
    else if ( var.startsWith("doubleSpinBox") )
    {
           foreach (QDoubleSpinBox *w ,m_parent->findChildren<QDoubleSpinBox*>(var)) w->setValue(value.toDouble());

    }
    else if ( var.endsWith("Image"))
    {

        foreach (ImageButton *w ,m_parent->findChildren<ImageButton*>(QString("toolButton")+var))
            w->setImage(value.toString());

    }
    else if ( var.endsWith("MediaBox"))
    {
        foreach (PageSize *w ,m_parent->findChildren<PageSize*>(QString("comboBoxMediaBox")))
        {
            w->setCurrentText(value.toString());
        }

    }
   else if ( var.startsWith("colorButton"))
    {
       foreach (ColorButton *w ,m_parent->findChildren<ColorButton*>(QString("colorButton")+var))
            {
                w->setColor(QColor(value.toString()));
            }

    }
   // else emit sendLog(QString ("Notice: (à finir) %1 => %2").arg(var).arg(value.toString()));

}


void FormConfig::InitDefault(QString classe)
{

    Settings *s;
    if( m_configFileName.isEmpty()) s = new Settings() ;
    else s= new Settings(m_configFileName);
    ui->checkBoxCover->setChecked(s->value(QString("%1/Cover").arg(classe),true).toBool());
    ui->checkBoxFullScreenMode->setChecked(s->value(QString("%1/FullScreenMode").arg(classe),false).toBool());
    ui->checkBoxTitleInUppercase->setChecked(s->value(QString("%1/TitleInUppercase").arg(classe),false).toBool());

    ui->comboBoxChordInText->setCurrentIndex(s->value(QString("%1/ChordInText").arg(classe),0).toInt());
    ui->comboBoxChordLang->setCurrentIndex(ui->comboBoxChordLang->findData(s->value(QString("%1/ChordLang").arg(classe),"en")));
    ui->comboBoxTocColumnNumber->setCurrentIndex(s->value(QString("%1/TocColumnNumber").arg(classe),0).toInt());
    ui->comboBoxTocSpaceCharacter->setCurrentIndex(s->value(QString("%1/TocSpaceCharacter").arg(classe),0).toInt());
    ui->comboBoxTocPosition->setCurrentIndex(s->value(QString("%1/TocPosition").arg(classe),1).toInt());
    ui->comboBoxTitlePosition->setCurrentIndex(s->value(QString("%1/TitlePosition").arg(classe),0).toInt());
    ui->comboBoxDuplex->setCurrentIndex(s->value(QString("%1/Duplex").arg(classe),0).toInt());
    ui->comboBoxPageNumber->setCurrentIndex(s->value(QString("%1/PageNumber").arg(classe),1).toInt());
    ui->comboBoxTocVerticalSpacing->setCurrentIndex(s->value(QString("%1/TocVerticalSpacing").arg(classe),1).toInt());
    ui->comboBoxPageNumberStyle->setCurrentIndex(s->value(QString("%1/PageNumberStyle").arg(classe),1).toInt());
    ui->comboBoxColumnNumber->setCurrentIndex(s->value(QString("%1/ColumnNumber").arg(classe),0).toInt());
    ui->comboBoxSubtitleNumber->setCurrentIndex(s->value(QString("%1/SubtitleNumber").arg(classe),0).toInt());
    ui->comboBoxVerticalPausePerPage->setCurrentIndex(s->value(QString("%1/VerticalPausePerPage").arg(classe),0).toInt());
    ui->checkBoxYear->setChecked(s->value(QString("%1/Year").arg(classe),false).toBool());
    ui->checkBoxComposer->setChecked(s->value(QString("%1/Composer").arg(classe),false).toBool());
    ui->checkBoxLyricist->setChecked(s->value(QString("%1/Lyricist").arg(classe),false).toBool());
    ui->checkBoxArtist->setChecked(s->value(QString("%1/Artist").arg(classe),false).toBool());
    ui->lineEditOutFile->setText(s->value(QString("%1/OutFile").arg(classe),classe).toString());
    ui->spuChordHorizontalSize->setValue(s->value(QString("%1/ChordHorizontalSize").arg(classe),"2mm").toString());
    ui->spuHorizontalMargin->setValue(s->value(QString("%1/HorizontalMargin").arg(classe),"2mm").toString());
    ui->spuVerticalMargin->setValue(s->value(QString("%1/VerticalMargin").arg(classe),"5mm").toString());
    ui->spuPageHeight->setValue(s->value(QString("%1/PageHeight").arg(classe),"297mm").toString());
    ui->spuPageWidth->setValue(s->value(QString("%1/PageWidth").arg(classe),"210mm").toString());
    ui->spuTocHorizontalMargin->setValue(s->value(QString("%1/TocHorizontalMargin").arg(classe),"10mm").toString());
    ui->spuPageNumberPosition->setValue(s->value(QString("%1/PageNumberPosition").arg(classe),"10mm").toString());

    QFont f;
    f.fromString(s->value(QString("%1/ChordFont").arg(classe),QFont().toString()).toString());
    ui->toolButtonChordFont->setFont(f);
    f.fromString(s->value(QString("%1/CoverFont").arg(classe),QFont().toString()).toString());
    ui->toolButtonCoverFont->setFont(f);
    f.fromString(s->value(QString("%1/NormalFont").arg(classe),QFont().toString()).toString());
    ui->toolButtonNormalFont->setFont(f);
    f.fromString(s->value(QString("%1/SubtitleFont").arg(classe),QFont().toString()).toString());
    ui->toolButtonSubtitleFont->setFont(f);
    f.fromString(s->value(QString("%1/TitleFont").arg(classe),QFont().toString()).toString());
    ui->toolButtonTitleFont->setFont(f);
    f.fromString(s->value(QString("%1/TocFont").arg(classe),QFont().toString()).toString());
    ui->toolButtonTocFont->setFont(f);
    f.fromString(s->value(QString("%1/PageNumberFont").arg(classe),QFont().toString()).toString());
    ui->toolButtonPageNumberFont->setFont(f);
    f.fromString(s->value(QString("%1/CommentFont").arg(classe),QFont().toString()).toString());
    ui->toolButtonCommentFont->setFont(f);
    f.fromString(s->value(QString("%1/CommentItalicFont").arg(classe),QFont().toString()).toString());
    ui->toolButtonCommentItalicFont->setFont(f);
    f.fromString(s->value(QString("%1/CommentBoxFont").arg(classe),QFont().toString()).toString());
    ui->toolButtonCommentBoxFont->setFont(f);

    ui->toolButtonChordFont->setColor(QColor(s->value(QString("%1/ChordFontColor").arg(classe),"black").toString()));
    ui->toolButtonCoverFont->setColor(QColor(s->value(QString("%1/CoverFontColor").arg(classe),"black").toString()));
    ui->toolButtonNormalFont->setColor(QColor(s->value(QString("%1/NormalFontColor").arg(classe),"black").toString()));
    ui->toolButtonSubtitleFont->setColor(QColor(s->value(QString("%1/SubtitleFontColor").arg(classe),"black").toString()));
    ui->toolButtonTitleFont->setColor(QColor(s->value(QString("%1/TitleFontColor").arg(classe),"black").toString()));
    ui->toolButtonPageNumberFont->setColor(QColor(s->value(QString("%1/PageNumberFontColor").arg(classe),"black").toString()));
    ui->toolButtonTocFont->setColor(QColor(s->value(QString("%1/TocFontColor").arg(classe),"black").toString()));
    ui->toolButtonCommentFont->setColor(QColor(s->value(QString("%1/CommentFontColor").arg(classe),"black").toString()));
    ui->toolButtonCommentItalicFont->setColor(QColor(s->value(QString("%1/CommentItalicFontColor").arg(classe),"black").toString()));
    ui->toolButtonCommentBoxFont->setColor(QColor(s->value(QString("%1/CommentBoxFontColor").arg(classe),"black").toString()));

    ui->toolButtonChordFont->setBackgroundColor(QColor(s->value(QString("%1/ChordFontBackgroundColor").arg(classe),"white").toString()));
    ui->toolButtonCoverFont->setBackgroundColor(QColor(s->value(QString("%1/CoverFontBackgroundColor").arg(classe),"white").toString()));
    ui->toolButtonNormalFont->setBackgroundColor(QColor(s->value(QString("%1/NormalFontBackgroundColor").arg(classe),"white").toString()));
    ui->toolButtonSubtitleFont->setBackgroundColor(QColor(s->value(QString("%1/SubtitleFontBackgroundColor").arg(classe),"white").toString()));
    ui->toolButtonTitleFont->setBackgroundColor(QColor(s->value(QString("%1/TitleFontBackgroundColor").arg(classe),"white").toString()));
    ui->toolButtonTocFont->setBackgroundColor(QColor(s->value(QString("%1/TocFontBackgroundColor").arg(classe),"white").toString()));
    ui->toolButtonCommentFont->setBackgroundColor(QColor(s->value(QString("%1/CommentFontBackgroundColor").arg(classe),"white").toString()));
    ui->toolButtonCommentItalicFont->setBackgroundColor(QColor(s->value(QString("%1/CommentItalicFontBackgroundColor").arg(classe),"white").toString()));
    ui->toolButtonCommentBoxFont->setBackgroundColor(QColor(s->value(QString("%1/CommentBoxFontBackgroundColor").arg(classe),"white").toString()));
    ui->toolButtonPageNumberFont->setBackgroundColor(QColor(s->value(QString("%1/PageNumberFontBackgroundColor").arg(classe),"white").toString()));

    ui->toolButtonCoverImage->setImage(s->value(QString("%1/CoverImage").arg(classe),"").toString());

    ui->colorButtonPaperColor->setColor(s->value(QString("%1/PaperColor").arg(classe),"#FFFFFF").toString());

    delete s;
}

void FormConfig::SetConfigFromFile(QString FileName)
{
    m_configFileName=FileName;
}

void FormConfig::SetConfigFromInit()
{
    m_configFileName="";
}
void FormConfig::SaveAsConfig()
{
    QString mode=this->parent()->objectName().replace(QRegularExpression("^page"),"").replace(QRegularExpression("Mode$"),"");
    QSettings s;
    Save( s.fileName(),mode);
}

void FormConfig::Save(QString filename, QString section)
{
    QSettings sf(filename,QSettings::IniFormat);

    foreach (FontButton *w ,m_parent->findChildren<FontButton*>())
    {
        QRegularExpression tb("^toolButton");
        QRegularExpression f("Font$");
        QString name=w->objectName().replace(tb,"").replace(f,"");
        sf.setValue(QString("%1/%2Font").arg(section).arg(name),w->getFont().toString());
        sf.setValue(QString("%1/%2FontColor").arg(section).arg(name),w->getTextColor().name());
        sf.setValue(QString("%1/%2FontBackgroundColor").arg(section).arg(name),w->getBackgroundColor().name());
    }
    foreach (ColorButton *w ,m_parent->findChildren<ColorButton*>() )
    {
        QRegularExpression tb("^toolButton");
        QRegularExpression c("Color$");
        QString name=w->objectName().replace(tb,"").replace(c,"");
        sf.setValue(QString("%1/%2Color").arg(section).arg(name),w->getColor().name());
    }
    foreach (ImageButton *w ,m_parent->findChildren<ImageButton*>())
    {
        QRegularExpression tb("^toolButton");
        QRegularExpression i("Image$");
        QString name=w->objectName().replace(tb,"").replace(i,"");
        sf.setValue(QString("%1/%2Image").arg(section).arg(name),w->getImage());
    }
    foreach (QCheckBox *w ,m_parent->findChildren<QCheckBox*>())
    {
         QRegularExpression tb("^checkBox");
         QString name=w->objectName().replace(tb,"");
         sf.setValue(QString("%1/%2").arg(section).arg(name),w->isChecked()?"1":"0");
    }
    foreach (QComboBox *w ,m_parent->findChildren<QComboBox*>())
    {
         QRegularExpression tb("^comboBox");
         QString name=w->objectName().replace(tb,"");
         sf.setValue(QString("%1/%2").arg(section).arg(name),w->currentIndex());
    }
    foreach (QLineEdit *w ,m_parent->findChildren<QLineEdit*>())
    {
         QRegularExpression tb("^lineEdit");
         QString name=w->objectName().replace(tb,"");
         sf.setValue(QString("%1/%2").arg(section).arg(name),w->text());
    }
    foreach (SpinBoxUnit *w ,m_parent->findChildren<SpinBoxUnit*>())
    {
         QRegularExpression tb("^spu");
         QString name=w->objectName().replace(tb,"");
         sf.setValue(QString("%1/%2").arg(section).arg(name),w->valueunit());
    }
    foreach (QDoubleSpinBox *w ,m_parent->findChildren<QDoubleSpinBox*>())
    {
         QRegularExpression tb("^doubleSpinBox");
         QString name=w->objectName().replace(tb,"");
         sf.setValue(QString("%1/%2").arg(section).arg(name),w->value());
    }
    foreach (ColorButton *w ,m_parent->findChildren<ColorButton*>())
    {
         QRegularExpression tb("^colorButton");
         QString name=w->objectName().replace(tb,"");
         sf.setValue(QString("%1/%2").arg(section).arg(name),w->getColor().name());
    }
    sf.sync();
}



void FormConfig::displayFont ( QFont font, QColor text, QColor background)
{    QString name=sender()->objectName().replace("toolButton","");
    name=QString("label%1Example").arg(name);
    foreach (ExampleLabel *w ,m_parent->findChildren<ExampleLabel*>(name))
    w->setColor(font,text,background);

}


void FormConfig::displayThumb(QString image)
{

    QSettings *s;
    if ( m_configFileName.isEmpty() ) s= new QSettings;
    else s= new QSettings(m_configFileName);
    if (image.isEmpty()){ ui->labelCoverViewImage->setPixmap(QPixmap());return;}
    QPixmap pix(QString("%2").arg(image));
    QPixmap p=pix.scaledToWidth(150);
    ui->labelCoverViewImage->setPixmap(p);
    delete s;
}


void FormConfig::deleteCoverImage(bool)
{
    ui->labelCoverViewImage->setPixmap(QPixmap());
    ui->toolButtonCoverImage->setImage("");
}


void FormConfig::Retranslate()
{
    ui->retranslateUi(this);
}

void FormConfig::InitComboBoxChordInclusion(QStringList list)
{
    ui->comboBoxChordInText->clear();
    ui->comboBoxChordInText->addItems(list);
}
