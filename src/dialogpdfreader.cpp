#include "dialogpdfreader.h"
#include "ui_dialogpdfreader.h"

//#if (QT_VERSION >= QT_VERSION_CHECK(5, 15, 0))
//#include <QQmlApplicationEngine>
//#endif

DialogPdfReader::DialogPdfReader(QString filename,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogPdfReader)
{
    ui->setupUi(this);
    setWindowTitle(filename);


}

DialogPdfReader::~DialogPdfReader()
{
    delete ui;
}
