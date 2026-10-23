#include "dialogshowexported.h"
#include "ui_dialogshowexported.h"
#include "dialogconfiguration.h"
#include <QSettings>
#include <QDir>
#include <QDebug>
#include <QRegularExpression>

DialogShowExported::DialogShowExported(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogShowExported)
{
    ui->setupUi(this);
    connect (ui->pushButtonClose,SIGNAL(clicked()),this,SLOT(close()));
}

DialogShowExported::~DialogShowExported()
{
    delete ui;
}


void DialogShowExported::setText(QString text)
{
    ReadDefaultConfig();
    text.replace(QRegularExpression("\\[[^]]+\\]"),"");
    for ( auto k : mKeys.keys())
       {
        QRegularExpression reg=QRegularExpression(QString("\\{%1:([^}]+)\\}").arg(k));
        QRegularExpressionMatch match;
        QString regWithout2Points=(QString("{%1}").arg(k.toLower()));
        if ( text.contains(reg,&match) )
        {
          QString replace=match.captured(1);
          text.replace(reg,QString("$%1").arg(k.toLower()));
          text.replace(QString("$%1").arg(k),mKeys[k.toLower()]);
          text.replace(QString("$%1").arg(k),replace);
        }
        text.replace(regWithout2Points,mKeys[k],Qt::CaseInsensitive);
       }
    text.replace(QRegularExpression("^\n+"),"\n");
    ui->textEdit->setText(text);
}

void DialogShowExported::ReadDefaultConfig()
{
    QString defaultFile=DialogConfiguration::ConfigPath()+"/default.fec";
    QSettings s(defaultFile,QSettings::IniFormat);
    for ( auto key : s.allKeys() )
        mKeys[key.toLower()]=s.value(key).toString();
}
