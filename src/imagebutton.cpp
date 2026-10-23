#include "imagebutton.h"
#include "chordutil.h"
#include <QFileDialog>
#include <QSettings>

#include <QDebug>

ImageButton::ImageButton(QWidget *parent):QToolButton(parent)
{
    connect (this,SIGNAL(clicked(bool)),this,SLOT(SetImage(bool)));
}

void ImageButton::SetImage(bool)
{
    QSettings s;
    QString filename=QFileDialog::getOpenFileName(this,tr("Choose an Image file"),ChordUtil::getLastDirectory(),tr("Image(*.jpg *.png)"));
    QDir dir(m_projectpath);
    m_image=dir.relativeFilePath(filename);
    if ( ! m_image.isEmpty())  emit ImageSelelected(m_image);
}

QString ImageButton::getImage()
{
    QSettings s;
    if ( m_image.isEmpty()) return "";
    else return m_image;
}

void ImageButton::setImage(QString image)
{
   m_image=image;
   emit ImageSelelected(image);
}

void ImageButton::setProjectPath(QString path)
{
    m_projectpath=path;
}
