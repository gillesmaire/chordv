#include     "pdfviewer.h"
#include "dialogpdfreader.h"
#include <QSettings>
#include <QProcess>
#include <QMessageBox>
#include <QDebug>
#include <QApplication>
#include <QFileInfo>

PdfViewer::PdfViewer(QString filename, QWidget *parent): QObject(parent)
{
    QSettings s;
    m_filename=filename;
    QString command =s.value("PDFReader").toString();
    QFileInfo fi(filename);
    if ( ! fi.exists() )
    {
        QMessageBox::warning(parent,tr("File not found")
                             ,tr("PDF file %1 not yet generated !").arg(filename)
                             );
        m_status=false;
        m_error= tr("PdfReader: file %1 not found").arg(filename);

    }
    else if ( s.value("InternalPDFReader").toBool())
    {
        DialogPdfReader *dial= new DialogPdfReader(m_filename,parent);
        dial->setAttribute(Qt::WA_DeleteOnClose);
        dial->show();
    }
    else if ( command.isEmpty() )
    {
        QMessageBox::warning(parent,tr("Variable PDFReader not set in Configuration")
                             ,tr("Go to Tools/Preference and set the PDF Reader name and path ")
                             ,QMessageBox::Ok);
        m_status=false;
        m_error= tr("PdfReader: PDFReader not configured");

    }
    else
    {
        QStringList arg;
        arg<<m_filename;
        m_status =QProcess::startDetached(command,arg);
        m_error=tr("PdfReader: Unknown error");

     }

}

bool PdfViewer::getStatus()
{
    return m_status ;
}

QString PdfViewer::getStatusError()
{
    return m_error;
}

QString PdfViewer::getStatusInfo()
{
    return (tr("PdfReader: %1 launched").arg(m_filename));
}
