#ifndef PDFVIEWER_H
#define PDFVIEWER_H

#include <QWidget>

class PdfViewer : public QObject
{
    Q_OBJECT
public:
    PdfViewer( QString filename, QWidget *parent );
    bool getStatus();
    QString getStatusError();
    QString getStatusInfo();
private:
    bool m_status;
    QString m_error;
    QString m_filename;
};

#endif // PDFVIEWER_H
