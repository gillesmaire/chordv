// GPL 3.0 License
// Copyright (c)2026 Gilles Maire <gilles@gillesmaire.com>
// Project : %PROJECTNAME%
// Filename : JazzLabConvertion.cpp
// Creation date : 21/11/2026
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
// THIS FILE IS AUTO GENERATED DO NOT CHANGE MANUALLY.
//
// Source: %GITLOCATION%
//


QString MainWindow::MakeChordsList(const QStringList &chords)
{
    QString result;
    QString currentBar;
    int countInBar = 0;     // accords déjà dans la mesure
    int barDivision = 0;    // nombre d'accords attendus pour cette mesure
    bool firstBar = true;

    QSettings s;

    auto flushBar = [&]() {
        QString trimmed = currentBar.trimmed();
        if (!trimmed.isEmpty()) {
            if (firstBar) {
                result += "| 4/4 " + trimmed + " | ";
                firstBar = false;
            } else {
                result += "| " + trimmed + " | ";
            }
        }
        currentBar.clear();
        countInBar = 0;
        barDivision = 0;
    };

    for (const QString &c : chords)
    {
        QString chord = c.trimmed();
        if (chord.isEmpty())
            continue;

        // supprimer les degrés (III), (IV), etc.
        chord.remove(QRegularExpression("\\([^)]*\\)"));

        // conversion en anglais
        Chord CH(chord, s.value("ChordLang").toString());
        chord = CH.ToEnglish(chord);
        if (chord.isEmpty())
            continue;

        // gestion des répétitions xN
        if (chord.contains('x')) {
            QString base = chord.section('x', 0, 0);
            int times = chord.section('x', 1, 1).toInt();
            
            for (int i = 0; i < times; ++i) {
                 currentBar = base;
                flushBar();   // chaque répétition = une mesure
                }
              continue;
            }
            // for (int i = 0; i < times; ++i) {
            //     currentBar += base + " ";
            //     countInBar++;
            //     // si aucune division n'est définie, 2 accords par mesure par défaut
            //     if (barDivision == 0) barDivision = 2;
            //     if (countInBar >= barDivision)
            //         flushBar();
            //}
            //continue;

        // gestion des :n
        QRegularExpression re(":([0-9]+)$");
        QRegularExpressionMatch m = re.match(chord);
        QString base = chord;
        int division = 1;
        if (m.hasMatch()) {
            division = m.captured(1).toInt();
            base.remove(re);
        }

        if (barDivision == 0)
            barDivision = division;

        currentBar += base + " ";
        countInBar++;

        if (countInBar >= barDivision)
            flushBar();
    }

    flushBar();

    // post-traitements
    result.replace("| |","|");      // supprime les barres vides
    result.replace("-","m");
    result.replace("C#","Db");
    result.replace("D#","Eb");
    result.replace("F#","Gb");
    result.replace("G#","Ab");
    result.replace("A#","Bb");

    return result.trimmed();
}

void MainWindow::ConvertInJazzFiles()
{
    QSettings s;
    QStringList lines=ui->textEditCho3File->document()->toPlainText().split('\n');
    QString directoryJF=s.value("DestinationDirectoryJazzFiles").toString();
    QString lastchord;

    QFile file;
    bool filetobeclose=false;
    QString titleSong;
    static const QRegularExpression titleRe("^\\{title:(.*?) *\\}$");
    static const QRegularExpression timeSignature("^\\{time:(.*?) *\\}$");
    static const QRegularExpression tempoSignature("^\\{tempo:(.*?) *\\}$");
    QTextStream out;
    QString Title,Time("4/4"),Tempo("120");
    QStringList chordlist;
    QString list;
    QString songname;
    for (auto line : lines )
    {   
        if (line.startsWith('{'))
        {   
            Time="4/4";
            Tempo="120";
        // we get title, timesignature and tempoBPM
           
            if ( line.contains(titleRe))  {                     
            
               list=MakeChordsList(chordlist);
              //  QString chl=chordlist.join('|');
              // //chl.replace("^| ","|"+Tempo);
               out<<list;
               
               chordlist.clear();
               line.replace(titleRe,"\\1");
               QString OriginalName=line.trimmed();
               songname=line.trimmed();
               songname.replace(" ","");
               songname.replace("'","");
               if (filetobeclose) file.close();
               QDir dir;
               dir.mkpath(directoryJF+"/"+songname);
               file.setFileName(directoryJF+"/"+songname+"/"+songname+".txt");
               if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
                    qWarning() << "Pb opening file";
                    return;
                }
               out.setDevice(&file);
               out<<"#title="<<OriginalName<<"\n";
               Title=OriginalName;
            }
            else if (line.contains(timeSignature)){
               line.replace(timeSignature,"#time=\\1");
               Time=line;
               out<<line<<"\n";
               
            }
            else if (line.contains(tempoSignature) ){
               line.replace(tempoSignature,"#tempo=\\1");
               Tempo=line;
               out<<line<<"\n";
               QFile tempo;
               tempo.setFileName(directoryJF+"/"+songname+"/"+Tempo);
               tempo.open(QIODevice::WriteOnly| QIODevice::Text );
            }   
        }
        else 
        {
        // here we get all the chords
        QRegularExpression re("\\[(.*?)\\]");
        QRegularExpressionMatchIterator it = re.globalMatch(line); 
        while (it.hasNext()) {
              QRegularExpressionMatch match = it.next();
              chordlist << match.captured(1).remove(QRegularExpression("\\(.*?\\)"));

        }

        }
     
    }
    lastchord=chordlist.last();
    out<<"#keysig="<<lastchord<<"ok"<<"\n";
    out<<list;
    if (s.value("ShowJazzLabDirectory").toBool())
    {
        QMessageBox msg(this);
        msg.setText(tr("Your JjazzLab import files are located in %1 directory")
                        .arg(DialogConfiguration::getDirectoryJjazzLib()));
        msg.setStandardButtons(QMessageBox::Ok);
        QCheckBox dontask(tr("Don't ask"));
        msg.setCheckBox(&dontask);
        if ( msg.exec() == QMessageBox::Ok) {
            if (dontask.isChecked()) s.setValue("ShowJazzLabDirectory",false);
        }
    }
}
