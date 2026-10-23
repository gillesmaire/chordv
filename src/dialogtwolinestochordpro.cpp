#include "dialogtwolinestochordpro.h"
#include "ui_dialogtwolinestochordpro.h"
#include "language.h"

#include <QDebug>
#include <QComboBox>

DialogTwoLinesToChordPro::DialogTwoLinesToChordPro(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogTwoLinesToChordPro)
{
    ui->setupUi(this);
    QFont f;
    f.setFamily("Courier");
    ui->plainTextEdit->setFont(f);

    connect ( ui->pushButtonClose,SIGNAL(clicked(bool)),this,SLOT(close()));
    connect (ui->pushButtonConvert,SIGNAL(clicked(bool)),this,SLOT(Convert()));
    connect(ui->pushButtonClear,SIGNAL(clicked(bool)),ui->plainTextEdit,SLOT(clear()));
    connect(ui->pushButtonDemo,SIGNAL(clicked(bool)),SLOT(Demo()));

   }

DialogTwoLinesToChordPro::~DialogTwoLinesToChordPro()
{
    delete ui;
}




void DialogTwoLinesToChordPro::Convert()
{
    QStringList out;
    QStringList lines=ui->plainTextEdit->document()->toPlainText().split("\n");
    foreach ( QString line, lines)
    {
      if( isChords(line) )
          ConvertToMap(line);
      else
          if (MapIsEmpty()) out <<line;
          else
            {
              out<<mixLineAndMap(line);
              ClearMap();
            }
    }
    ui->plainTextEdit->clear();
    ui->plainTextEdit->insertPlainText(out.join("\n"));

}

bool DialogTwoLinesToChordPro::isChords(QString line)
{
    line.replace(QRegularExpression(" +")," ");
    line.replace(QRegularExpression("^ *"),"");
    line.replace(QRegularExpression(" *$"),"");

    QStringList words=line.split(" ");
    QString chordreg=Language::ListNotes(ui->widget->getInputLang()).join("|");
    chordreg=QString("^(%1)").arg(chordreg);

    foreach ( QString word, words)
    {
         if ( ! word.contains(QRegularExpression(chordreg)))
             return false;
    }
    return true;
}

void DialogTwoLinesToChordPro::ConvertToMap( QString line)
{
    QStringList listbeginingchord=Language::ListNotes(ui->widget->getInputLang());
    for ( int i=0; i< line.length();i++)
    {
        QChar c=line[i];
        if (listbeginingchord.contains(c))
            if (i==0 || line[i-1]==' ')
            {
                QString w;
                int j=i;
                while ( j!=line.length() && line[j] != ' ' )
                {
                    w+=line[j];
                    j++;
                }
                m_chords[i]=w;
            }
    }
}


bool DialogTwoLinesToChordPro::MapIsEmpty()
{
    return m_chords.count()==0;
}

void DialogTwoLinesToChordPro::ClearMap()
{
    m_chords.clear();
}

QString DialogTwoLinesToChordPro::mixLineAndMap(QString line)
{
    foreach ( int i, m_chords.keys())
    {
        line.insert(i,QString("[%1]").arg(translate(m_chords[i])));
    }
    return line;
}


QString DialogTwoLinesToChordPro::translate(QString chord)
{
    QString chordreg=Language::ListNotes(ui->widget->getInputLang()).join("|");
    QStringList chordoutput=Language::ListNotes(ui->widget->getOutputLang());
    chordreg=QString("^(%1)").arg(chordreg);
    QRegularExpression reg(chordreg);
    QRegularExpressionMatch match;
    if ( chord.contains(reg,&match))
    {
        QString c=match.captured(1);
        int i=Language::ListNotes(ui->widget->getInputLang()).indexOf(c);
        chord.replace(c,chordoutput.at(i));
        chord.replace(ui->widget->getInputMinor(),ui->widget->getOutputMinor());
    }
   return chord;
}

void DialogTwoLinesToChordPro::Demo()
{
    ui->widget->setInputLang(0);
    ui->widget->setInputMinor(0);
    ui->widget->setOutputLang(1);
    ui->widget->setOutputMinor(1);
    ui->plainTextEdit->setPlainText("Titre   : Ma plus belle histoire d'amour\n"
"\n"
                          "------------------------------------------------------------------------------\n"
"\n"
                          "G                    G7\n"
                          "Du plus loin que me revienne\n"
                          "Em                   Am\n"
                          "L'ombre de mes amours anciennes\n"
                          "D7\n"
                          "Du plus loin du premier rendez-vous\n"
                          "G7\n"
                          "Du temps des premières peines\n"
                          "Em                       Am\n"
                          "Lors j'avais quinze ans à peine\n"
                          "Am                D7           G\n"
                          "Cœur tout blanc et griffes aux genoux\n"
                          "C                     Am\n"
                          "Que ce fut, j'étais précoce\n"
                          "D7                  G    B7\n"
                          "De tendres amours de gosse\n"
                          "Em       A7              D7\n"
                          "Ou les morsures d'un amour fou\n"
                          "Em                        D7\n"
                          "Du plus loin qu'il m'en souvienne\n"
                          "D                    G     Em\n"
                          "Si depuis j'ai dit je t'aime\n"
                          "Am                       D7        G   D#7\n"
                          "Ma plus belle histoire d'amour c'est vous\n"
"\n"
                          "G#                      G#7\n"
                          "C'est vrai je ne fus pas sage\n"
                          "Fm                     A#m\n"
                          "Et j'ai tourné bien des pages\n"
                          "D#7            G#\n"
                          "Sans les lir' blanches et puis rien dessus\n"
                          "G#7\n"
                          "C'est vrai je ne fus pas sage\n"
                          "Fm                    A#m\n"
                          "Et mes guerriers de passage\n"
                          "D#7      G#   G#7   F#   G#7\n"
                          "A peine vu déjà disparus\n"
                          "C#                   A#m\n"
                          "Mais à travers leur visages\n"
                          "D#7              G#   C7\n"
                          "C'était déjà votre image\n"
                          "Fm          A#7            D#7   Fm   D#7\n"
                          "C'était vous déjà et le cœur nu\n"
                          "Fm\n"
                          "Je refaisais mes bagages\n"
                          "D#7                G#   Fm\n"
                          "Et poursuivais mon mirage\n"
                          "A#m                      D#7       G#\n"
                          "Ma plus belle histoire d'amour c'est vous\n"
                          "A            E    B7             E\n"
                          "Sur la longue route qui menait vers vous\n"
                          "A            E             B7     E\n"
                          "Sur la longue route j'allais le cœur fou\n"
                          "A         E       B7          E\n"
                          "Le vent de décembre me gelait au cou\n"
                          "A         E       D#7       G#\n"
                          "Qu'importe décembre si c'était vous.\n"

                          "Elle fut longue la route\n"
                          "Mais je l'ai faite la route\n"
                          "Celle-là qui menait jusqu'à vous\n"
                          "Et je ne suis pas parjure\n"
                          "Si ce soir je vous jure\n"
                          "Que pour vous je l'eus faite à genoux\n"
                          "Il en eut fallu bien d'autres\n"
                          "Que quelques mauvais apôtres\n"
                          "Que l'hiver ou la neige à mon cou\n"
                          "Pour que je perde patience\n"
                          "Et j'ai calmé ma violence\n"
                          "Ma plus belle histoire d'amour c'est vous\n"
"\n"
                          "Mais tant d'hivers et d'automnes\n"
                          "De nuits, de jours et personne\n"
                          "Vous n'étiez jamais au rendez-vous\n"
                          "Et de vous perdant courage\n"
                          "Soudain me prenait la rage\n"
                          "Mon Dieu, que j'avais besoin de vous\n"
                          "Que le diable vous emporte\n"
                          "D'autres m'ont ouvert leur porte\n"
                          "Heureus' je m'en allais loin de vous\n"
                          "Oui, je vous fus infidèle\n"
                          "Mais vous revenais quand même\n"
                          "Ma plus belle histoire d'amour, c'est vous\n"
                          "J'ai pleuré mes larmes, mais qu'il me fut doux\n"
                          "Oh qu'il me fut doux, ce premier sourire de vous\n"
                          "Et pour une larme qui venait de vous\n"
                          "J'ai pleuré d'amour, vous souvenez-vous ?\n"

                          "Ce fut un soir en septembre\n"
                          "Vous étiez venus m'attendre\n"
                          "Ici mêm' vous en souvenez-vous ?\n"
                          "A nous regarder sourire\n"
                          "A nous aimer sans rien dire\n"
                          "C'est ici que j'ai compris tout à coup\n"
                          "J'avais fini mon voyage\n"
                          "Et j'ai posé mes bagages\n"
                          "Vous étiez venus au rendez-vous\n"
                          "Qu'importe ce qu'on peut en dire\n"
                          "Je tenais à vous le dire\n"
                          "A#m            D#7        G#   G#7\n"
                          "Ce soir je vous remercie de vous\n"
                          "C#                         A#m\n"
                          "Qu'importe ce qu'on peut on dire\n"
                          "D#7                    G#\n"
                          "Je suis venue pour vous dire\n"
                          "A#m7                                G#   G#7\n"
                          "Ma plus belle histoire d'amour c'est vous.\n"
);
}

