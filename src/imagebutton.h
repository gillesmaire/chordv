#ifndef IMAGEBUTTON_H
#define IMAGEBUTTON_H

#include <QToolButton>
#include <QLabel>
class ImageButton : public QToolButton
{

   Q_OBJECT
public:
    ImageButton(QWidget *parent);
    QString getImage() ;
    void setImage(QString image) ;
    void setProjectPath(QString path);
private:
    QString m_image;
    QString m_projectpath;
private slots:
    void SetImage(bool);
signals:
    void ImageSelelected( QString Image);
};

#endif // IMAGEBUTTON_H
