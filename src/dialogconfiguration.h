#ifndef DIALOGCONFIGURATION_H
#define DIALOGCONFIGURATION_H

#include <QDialog>

#include <QComboBox>
#include <QTranslator>

namespace Ui {
class DialogConfiguration;
}

class DialogConfiguration : public QDialog
{
    Q_OBJECT

public:
    explicit DialogConfiguration(QWidget *parent = 0);
    void setTranslator( QTranslator *translator);
    ~DialogConfiguration();
    void InitSettings();
    static void setLanguageComboBox(QComboBox *ptr);
    static QString getTranslationQmFileName(QString lang);
    static QString ConfigPath();
    static QString getDirectoryJjazzLib();
    static QString getDirectoryMMA();
    static QString getDirectoryMarkDown();
private slots:
    void Save();
    void SetPDFReader();
    void SelectLanguage(QString lang);
    void InternalPDFStatus(int status);
    void LoadDefaultFormat();
    void LoadFormat();
    void SaveFormat();
    void SaveDefaultFormat();

    void setDirectoryWithMusicMarkDown();
    void setMarkDownShowDir();
    bool getMarkDownShowDir();
    
    void setDirectoryJjazzLib();
    bool getJjazzLabShowDir();
    void setJjazzLabShowDir();

    void setDirectoryMMA();
    bool getMMAShowDir();
    void setMMAShowDir();


private:
    Ui::DialogConfiguration *ui;
    QWidget *m_parent;
    QTranslator *m_translator;
    void Connect();
    void Retranslate(QString lang);
signals:
    void LanguageChanged(QString lang);
    void PdfReaderChanged();

};

#endif // DIALOGCONFIGURATION_H
