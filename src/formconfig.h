#ifndef FORMCONFIG_H
#define FORMCONFIG_H

#include <QWidget>

namespace Ui {
class FormConfig;
}
///
/// \brief The FormConfig class manage the display of configuration of attributes : (Color, Fonts, ...)
/// for the definition of each booklet.
/// It is used in Preference and also on each project
///
class FormConfig : public QWidget
{
    Q_OBJECT

public:
    /// Classes is different for each style of booklet
//    enum Classes { /// FormConfig is Chord oriented
//                   Chord,
//                   ///  FormConfig is Lyrics oriented
//                   Lyrics,
//                   /// FormConfig is Memory oriented
//                   Memory,
//                   /// FormConfig is Text only oriented
//                   Text
//                 };
    /// ChordInclusion has different capabilities for Text and Lyrics
    enum ChordInclusion {
        /// Diagram are included in Text
        DiagramInText,
        /// Diagram are include at begining and in Text
        ChordAtBeginingAndChordNameInText,
        /// No diagram at beginin but in text
        NoChordAtBeginingAndChordNameInText};
    ///
    /// \brief FormConfig constructor complient with QtDesigner allowing promotion of QWidget
    /// \param parent
    ///
    void setOffButtonSetAsDefault();
    explicit FormConfig(QWidget *parent = 0);
    ~FormConfig();
    ///
    /// \brief setValue allow to set a Value for an object it is based on the name of the object for example
    /// comboBoxName => Name is extracted and QSettings is used to Name value
    /// The object types analyzed are SpinBoxUnit,FontButton,Font, FontColor,FontBackgroundColor,Cover,comboBox,
    /// spinBox,doubleSpinBox,Image,MediaBox,colorButton
    /// \param var the name of object for example comboBoxName
    /// \param value : value given to Name extracted for comboBox
    ///
    void setValue(QString var, QVariant value);
    ///
    /// \brief Save all the SpinBoxUnit,FontButton,Font, FontColor,FontBackgroundColor,Cover,comboBox,
    /// spinBox,doubleSpinBox,Image,MediaBox,colorButton extracted name presented in setValue
    /// in a filename given in argument. The value is the classname : Chord, Memory, Lyrics or Text
    /// \param filename : the QString filename to save the value
    /// \param section : Classes enum representing the section in Settings save file
    ///
    void Save(QString filename,QString section);
    ///
    /// \brief InitDefault init all the SpinBoxUnit,FontButton,Font, FontColor,FontBackgroundColor,Cover,comboBox,
    /// spinBox,doubleSpinBox,Image,MediaBox,colorButton extracted name with the values given but preference QDialog
    /// \param classe : Classes enum representing the section in Settings save fi
    ///
    void InitDefault(QString classe);
    ///
    /// \brief getUi this virtual function return the ui used on each derived class
    /// \return  the Ui::FormConfig pointer on the child class
    ///
    virtual Ui::FormConfig *getUi() { return ui;}
    ///
    /// \brief SetConfigFromFile
    /// \param FileName
    ///
    void SetConfigFromFile(QString FileName);
    void SetConfigFromInit();
    void Retranslate();
    void disableGroup(QString group);
    void setProjectPath(QString path);
public slots:
    void SaveAsConfig();
protected:
    void disableWidgets(QRegularExpression value);
    void InitComboBoxChordInclusion(QStringList list);
private:
    Ui::FormConfig *ui;
    QWidget *m_parent;
    QString m_configFileName;
    QString m_projectpath;
    void Calculate();
signals:
    void sendLog (QString message);

private slots:
    void setCover(int);
    void displayFont(QFont font, QColor text, QColor background);
    void displayThumb(QString image);
    void SizeChanged(QString size);
    void SizeChanged(bool);
    void FindSize(double);
    void deleteCoverImage(bool);
    ///
    /// \brief CheckArrow number with arrow force number centered
    /// \param i index of combobox number style
    ///

    void CheckArrow(int i);
};

#endif // FORMCONFIG_H
