#include "fontchooser.h"
#include "ui_fontchooser.h"
#include <QFontDialog>
#include <QDebug>
#include <QResizeEvent>

FontChooser::FontChooser(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FontChooser)
{
    ui->setupUi(this);
    connect(ui->buttonBox,SIGNAL(accepted()),this,SLOT(setFont()));
}

void FontChooser::setFont(QFont font, QColor textcolor, QColor backgroundcolor)
{
    ui->widget->setCurrentFont(font);
    ui->toolButtonBackgroundColor->setColor(backgroundcolor);
    ui->toolButtonFontColor->setColor(textcolor);
    emit SelectFont(font,textcolor,backgroundcolor);
}

FontChooser::~FontChooser()
{
    delete ui;
}


void FontChooser::setFont()
{
    setFont(ui->widget->currentFont(),ui->toolButtonFontColor->getColor(),ui->toolButtonBackgroundColor->getColor());
    emit SelectFont(ui->widget->currentFont(),ui->toolButtonFontColor->getColor(),ui->toolButtonBackgroundColor->getColor());
}
