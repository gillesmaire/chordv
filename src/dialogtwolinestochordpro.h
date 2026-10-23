#ifndef DIALOGTWOLINESTOCHORDPRO_H
#define DIALOGTWOLINESTOCHORDPRO_H

#include <QDialog>
#include <QMap>
#include <QComboBox>
#include <QLabel>

namespace Ui {
class DialogTwoLinesToChordPro;
}

///
/// \brief The DialogTwoLinesToChordPro class is an utility to convert song in two lines :
///
/// C             D
/// Hello how are you
///
/// to chord in one line :
/// [C] Hello how are [D]you
///

class DialogTwoLinesToChordPro : public QDialog
{
    Q_OBJECT

public:
    ///
    /// \brief DialogTwoLinesToChordPro the dialog windows
    /// \param parent
    ///
    explicit DialogTwoLinesToChordPro(QWidget *parent = 0);
    ~DialogTwoLinesToChordPro();

private slots:
    void Convert();
    void Demo();
private:
    Ui::DialogTwoLinesToChordPro *ui;
    bool isChords(QString line);
    QMap <int,QString> m_chords;
    bool MapIsEmpty();
    QString mixLineAndMap(QString line);
    void ConvertToMap(QString line);
    void ClearMap();
    QString m_minorfrom;
    QString m_minorto;
    QString translate(QString chord);
};

#endif // DIALOGTWOLINESTOCHORDPRO_H
