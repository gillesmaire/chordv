#include "dialogdocumentation.h"
#include "ui_dialogdocumentation.h"

#include "language.h"

#include <QSettings>
#include <QFile>
#include <QFileInfo>
#include <QFileInfo>
#include <QDebug>
#include <QDir>

DialogDocumentation::DialogDocumentation(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogDocumentation)
{
    ui->setupUi(this);
    connect (ui->pushButtonClose,SIGNAL(clicked(bool)),this,SLOT(close()));
    QSettings s;
    QString language=s.value("InterfaceLanguage").toString();
    if (language.isEmpty()) language="English";

    QString filename=Language::getTranslationDocFileName(language);
    QString path= QDir(QFileInfo(filename).absoluteDir()).path();
    QFile   file(filename);
    file.open(QIODevice::ReadOnly|QIODevice::Text);
    QString content=file.readAll();
    file.close();
    QStringList pathlist;
    pathlist<<path;
    ui->textBrowser->setSearchPaths(pathlist);
    ui->textBrowser->setHtml(content);


}

DialogDocumentation::~DialogDocumentation()
{
    delete ui;
}
