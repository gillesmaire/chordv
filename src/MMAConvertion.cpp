// GPL 3.0 License
// Copyright (c)2026 Gilles Maire <gilles@gillesmaire.com>
// Project : %PROJECTNAME%
// Filename : MMAConvertion.cpp
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

#include <QString>
#include <mainwindow.h>
#include <QTemporaryFile>
#include <QSettings>
#include <QRegularExpression>
#include <algorithm>
#include "ui_mainwindow.h"
#include <QDir>
#include <QDirIterator>
#include <QMessageBox>
#include "dialogconfiguration.h"

QMap<QString, QStringList> MainWindow::getMmaMixLevels(const QString &groove)
{
    QMap<QString, QStringList> result;

    QTemporaryFile trackFile;

    if (!trackFile.open())
        return result;

    QTextStream out(&trackFile);

    out << "Time 4/4\n";
    out << "Groove " << groove << "\n";
    out << "Print PISTES : $_Tracklist\n";

    out.flush();
    trackFile.flush();

    QProcess process;
    process.start("mma", QStringList() << trackFile.fileName());

    if (!process.waitForFinished(5000))
    {
        process.kill();
        return result;
    }

    QString output =
        QString::fromLocal8Bit(process.readAllStandardOutput());

    QStringList tracks;

    for (const QString &line : output.split('\n'))
    {
        if (line.startsWith("PISTES :"))
        {
            tracks = line.mid(8).trimmed()
                         .split(' ', Qt::SkipEmptyParts);
            break;
        }
    }

    if (tracks.isEmpty())
        return result;

    QTemporaryFile volumeFile;

    if (!volumeFile.open())
        return result;

    QTextStream volumeOut(&volumeFile);

    volumeOut << "Time 4/4\n";
    volumeOut << "Groove " << groove << "\n";

    for (const QString &track : tracks)
    {
        volumeOut << "Print VOLUME:" << track
                  << " : $_" << track << "_Volume\n";
    }

    volumeFile.flush();
    volumeOut.flush();
    
    process.start("mma", QStringList() << volumeFile.fileName());

    if (!process.waitForFinished(5000))
    {
        process.kill();
        return result;
    }

    output =
        QString::fromLocal8Bit(process.readAllStandardOutput());

    for (const QString &line : output.split('\n'))
    {
        if (!line.startsWith("VOLUME:"))
            continue;

        int separator = line.indexOf(" :");

        if (separator < 0)
            continue;

        QString track = line.mid(
            QString("VOLUME:").length(),
            separator - QString("VOLUME:").length()).trimmed();

        QString values =
            line.mid(separator + 2).trimmed();

        if (!track.isEmpty() && !values.isEmpty())
        {
            result.insert(
                track,
                values.split(' ', Qt::SkipEmptyParts));
        }
    }

    return result;
}


 void MainWindow::ConvertInMMAFiles()
    {
        QSettings s;

        // Reconstruit [MMA] à partir des grooves de fin de la bibliothèque.
        const QString libPath = "/usr/share/mma/lib";
        QStringList grooveFiles;

        QDirIterator it(libPath,
                        QStringList() << "*.mma",
                        QDir::Files,
                        QDirIterator::Subdirectories);

        while (it.hasNext())
            grooveFiles << it.next();

        // stdlib est prioritaire, comme dans la recherche des grooves de MMA.
        std::sort(grooveFiles.begin(), grooveFiles.end(),
                  [&libPath](const QString &a, const QString &b)
        {
            const QString ra = QDir(libPath).relativeFilePath(a);
            const QString rb = QDir(libPath).relativeFilePath(b);
            const bool aStdlib = ra.startsWith("stdlib/");
            const bool bStdlib = rb.startsWith("stdlib/");

            if (aStdlib != bStdlib)
                return aStdlib;

            return ra < rb;
        });

        static const QRegularExpression defGrooveRx(
            R"(^\s*DefGroove\s+([^\s/]+))",
            QRegularExpression::CaseInsensitiveOption);

        // Ces suffixes identifient les grooves de fin : End, Ending,
        // End1/End2, EndingA... et FillA... (sans faux positif au milieu).
        static const QRegularExpression endNameRx(
            R"((?:END(?:ING)?[A-Z0-9]*|FILL[A-Z0-9]*)$)",
            QRegularExpression::CaseInsensitiveOption);

        QMap<QString, QString> endGrooves;

        for (const QString &grooveFileName : grooveFiles)
        {
            QFile grooveFile(grooveFileName);

            if (!grooveFile.open(QIODevice::ReadOnly | QIODevice::Text))
                continue;

            QTextStream in(&grooveFile);
            QString relativePath =
                QDir(libPath).relativeFilePath(grooveFileName);
            relativePath.chop(4);
            relativePath.replace(QDir::separator(), '/');

            while (!in.atEnd())
            {
                const QString line = in.readLine().trimmed();
                const auto match = defGrooveRx.match(line);

                if (!match.hasMatch())
                    continue;

                const QString grooveName = match.captured(1);

                if (endNameRx.match(grooveName).hasMatch()
                    && !endGrooves.contains(grooveName))
                {
                    endGrooves.insert(relativePath,grooveName);
                }
            }
        }

        // QSettings écrit alors une seule rubrique [MMA], avec une clé
        // par groove de fin : NomDuGrooveDeFin=chemin/du/fichier/source.
        if (!endGrooves.isEmpty())
        {
            s.remove("MMA");
            s.beginGroup("MMA");

            for (auto it = endGrooves.cbegin(); it != endGrooves.cend(); ++it)
                s.setValue(it.key(), it.value());

            s.endGroup();
        }
        else
        {
            qWarning() << "No MMA ending grooves found in" << libPath;
        }

        s.sync();

        if (QMenu *menu = ui->actionBuild_MMA_files->parent()
                     ? qobject_cast<QMenu *>(ui->actionBuild_MMA_files->parent())
                     : nullptr)
                  menu->hide();
        int menuindex=ui->stackedWidget->currentIndex();
        ui->stackedWidget->setCurrentIndex(6);
        
        ui->stackedWidget->show();

        QApplication::processEvents();        
       


        QStringList lines =
            ui->textEditCho3File->document()->toPlainText().split('\n');

        QString directoryName =
            s.value("DestinationDirectoryMMAFiles").toString();

        QFile file;
        QTextStream out;

        QString title;
        QString songname;
        QString time = "120";
        QString tempo = "4/4";
        QString Defaultgroove = s.value("DefaultGroove","stdlib:swing").toString();
        QString groove;

        bool clickbegin=false;

        bool GrooveMade=false;
        bool TempoMade=false;
        bool TimeMade=false;
        
        // pre click
        
        
        QStringList currentVerse;

        QString fullSong;
        int blockCount = 0;

        // Dernier accord réellement traité dans toute la chanson
        QString lastChordOfSong;

        static const QRegularExpression titleRe("^\\{title:(.*?) *\\}$");
        static const QRegularExpression timeRe("^\\{time:(.*?) *\\}$");
        static const QRegularExpression tempoRe("^\\{tempo:(.*?) *\\}$");
        static const QRegularExpression grooveRe("^\\{style:(.*?) *\\}$");
        

        auto normalizeChord = [](QString chord) -> QString
        {
            chord.remove(QRegularExpression("\\(.*?\\)"));

            chord.replace("Rém", "Dm");
            chord.replace("Ré", "D");
            chord.replace("Mim", "Em");
            chord.replace("Mi", "E");
            chord.replace("Lam", "Am");
            chord.replace("Sim", "Bm");

            chord.replace("Do", "C");
            chord.replace("Fa", "F");
            chord.replace("Sol", "G");
            chord.replace("La", "A");
            chord.replace("Si", "B");

            chord.replace("Sib", "Bb");
            chord.replace("Lab", "Ab");
            chord.replace("Réb", "Db");
            chord.replace("Mib", "Eb");
            chord.replace("Solb", "Gb");

            return chord.trimmed();
        };

// Ajoute les instruments du groove dans un bloc AllGrooves


auto writeGrooveVoices = [&](const QString &grooveName)
{
    QString name = grooveName.trimmed();

    if (name.endsWith(".mma", Qt::CaseInsensitive))
        name.chop(4);

    const QString libPath = "/usr/share/mma/lib";
    QString groovePath;

    // Respecte explicitement le sous-répertoire MMA :
    // stdlib:swing -> /usr/share/mma/lib/stdlib/swing.mma
    const int colon = name.indexOf(':');

    if (colon >= 0) {
        const QString library = name.left(colon);
        const QString groove = name.mid(colon + 1);

        groovePath = QDir(libPath).filePath(
            library + "/" + groove + ".mma");
    }
    else {
        QDirIterator it(libPath,
                        QStringList() << "*.mma",
                        QDir::Files,
                        QDirIterator::Subdirectories);

        while (it.hasNext()) {
            const QString path = it.next();

            if (QFileInfo(path).completeBaseName().compare(
                    name, Qt::CaseInsensitive) == 0) {
                groovePath = path;
                break;
            }
        }
    }

    if (groovePath.isEmpty() || !QFile::exists(groovePath)) {
        qWarning() << "Groove not found:" << grooveName
                   << groovePath;
        return;
    }


    QFile grooveFile(groovePath);

    if (!grooveFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Cannot open groove:" << groovePath;
        return;
    }

    QTextStream in(&grooveFile);

    QString currentTrack;
    QMap<QString, QString> voices;

    static const QRegularExpression beginRx(
        R"(^\s*Begin\s+(\S+)\s*$)",
        QRegularExpression::CaseInsensitiveOption);

    static const QRegularExpression voiceRx(
        R"(^\s*Voice\s+(\S+))",
        QRegularExpression::CaseInsensitiveOption);

    static const QRegularExpression endRx(
        R"(^\s*End\s*$)",
        QRegularExpression::CaseInsensitiveOption);

    while (!in.atEnd()) {
        const QString line = in.readLine().trimmed();

        const auto beginMatch = beginRx.match(line);

        if (beginMatch.hasMatch()) {
            currentTrack = beginMatch.captured(1);
            continue;
        }

        if (endRx.match(line).hasMatch()) {
            currentTrack.clear();
            continue;
        }

        const auto voiceMatch = voiceRx.match(line);

        if (voiceMatch.hasMatch() && !currentTrack.isEmpty())
            voices.insert(currentTrack, voiceMatch.captured(1));
    }

    

    if (voices.isEmpty()) {
        qWarning() << "No Voice found in:" << groovePath;
        return;
    }
    out << "// If you change this groove, the following lines will be recalculated.\n";
    out << "// Feel free to change the instrument names here without changing "
    << grooveName << ".\n\n";
    
   out << "Begin AllGrooves\n";

    for (auto it = voices.cbegin(); it != voices.cend(); ++it)
    out << "    " << it.key()
        << " Voice " << it.value() << "\n";

    // Récupération des volumes définis dans le groove
    QMap<QString, QStringList> levels =
    getMmaMixLevels(grooveName);

    // Écriture des volumes dans AllGrooves 
    for (auto it = levels.cbegin(); it != levels.cend(); ++it)
    {
    if (it.value().isEmpty())
        continue;

    out << "    " << it.key()
        << " Volume " << it.value().first() << "\n";
    }

    out << "End\n\n";
    
   
    
};


      
auto convertVerseToMMA = [&](const QStringList &verse) -> QString
        {
            QString FileName;
            QString result;
            QString grov;
            int counter = 1;
            
            QRegularExpression re("\\[(.*?)\\]");
            for (const QString &line : verse)
            {
                QRegularExpressionMatchIterator it = re.globalMatch(line);

                while (it.hasNext())
                {
                    auto match = it.next();
                    QString token = match.captured(1);

                    token.remove(QRegularExpression("\\(.*?\\)"));

                    // Accord, durée éventuelle (:2) et répétition éventuelle (x2, x3...)
                    QRegularExpression tokenRe("^(.*?)(?::(\\d+))?(?:x(\\d+))?$");
                    auto m = tokenRe.match(token);

                    if (!m.hasMatch())
                        continue;

                    QString chord = normalizeChord(m.captured(1));

                    int duration = 4;
                    if (!m.captured(2).isEmpty())
                        duration = m.captured(2).toInt();

                    int repeat = 1;
                    if (!m.captured(3).isEmpty())
                        repeat = m.captured(3).toInt();

                    if (duration == 2 && it.hasNext())
                    {
                        // Accord suivant : deuxième moitié de la mesure
                        auto nextMatch = it.next();
                        QString nextToken = nextMatch.captured(1);

                        nextToken.remove(QRegularExpression("\\(.*?\\)"));

                        auto nextParsed = tokenRe.match(nextToken);

                        if (nextParsed.hasMatch())
                        {
                            QString nextChord =
                                normalizeChord(nextParsed.captured(1));

                            result += QString("%1 %2 %3\n")
                                          .arg(counter)
                                          .arg(chord)
                                          .arg(nextChord);

                            // Le dernier accord réellement écrit est nextChord
                            lastChordOfSong = nextChord;

                            counter++;
                        }
                        else
                        {
                            // Sécurité : si le deuxième accord est invalide,
                            // on conserve le premier
                            result += QString("%1 %2\n")
                                          .arg(counter)
                                          .arg(chord);

                            lastChordOfSong = chord;
                            counter++;
                        }
                    }
                    else
                    {
                        for (int i = 0; i < repeat; ++i)
                        {
                            result += QString("%1 %2\n")
                                          .arg(counter)
                                          .arg(chord);

                            // Cet accord est effectivement ajouté au MMA.
                            // Donc il devient le dernier accord connu.
                            lastChordOfSong = chord;

                            counter++;
                        }
                    }
                }
            }

            return result;
        };

        auto flushCurrentVerse = [&]()
        {
            if (currentVerse.isEmpty())
                return;

            QString verseText = convertVerseToMMA(currentVerse);

            fullSong += verseText;
            currentVerse.clear();
            blockCount++;
        };

        auto flushSong = [&]()
        {
    
            QString grov = groove.isEmpty() ? Defaultgroove : groove;
            flushCurrentVerse();

            if (fullSong.isEmpty())
                return;

            // Maintenant que tous les couplets ont été convertis,
            // lastChordOfSong contient bien le dernier accord.
            //out << "KeySig " << lastChordOfSong << "\n";
            
            if ( ! GrooveMade )
            {
             if (  clickbegin && s.value("ClickAtBegin").toBool()) 
                    {
                    out <<"\nGroove  Metronome2-4\n";
                    out << "z * 2\n\n";
                    clickbegin=false;
                    }
                    
             grov=groove.isEmpty()?Defaultgroove:groove;
             out <<"Groove " << grov <<"\n\n";
             writeGrooveVoices(grov);
 
             GrooveMade=true;
             
            }
            if ( ! TempoMade ) out <<"Tempo " << tempo<<"\n";
            if ( ! TimeMade ) out <<"Time " << time<<"\n";
            
            out << "Repeat\n";
            out << fullSong;
            out << "RepeatEnd " << blockCount << "\n\n";

            // Groove de fin
            if (!lastChordOfSong.isEmpty())
            {
                QString endGrooveKey = grov;
                endGrooveKey.replace(':', '/');

                QString endGroove =
                    s.value("MMA/" + endGrooveKey).toString();

                if (!endGroove.isEmpty())
                {
                    out << "Groove " << endGroove << "\n";
                    out << "1 " << lastChordOfSong << "\n\n";
                    out.flush();
                    file.flush();
                }
            }

            fullSong.clear();
            blockCount = 0;
           
        };

        for (QString line : lines)
        {
            QString l = line.trimmed();

            // ligne vide = nouveau bloc
            if (l.isEmpty())
            {
                flushCurrentVerse();
                continue;
            }

            // directives
            if (l.startsWith('{'))
            {
                if (l.contains(titleRe))
                {
                    flushSong();

                    if (file.isOpen())
                        file.close();


                    auto m = titleRe.match(l);
                    title = m.captured(1).trimmed();

                    songname = title;
                    songname.remove(" ");
                    songname.remove("'");
                    
                    time = "4/4";
                    tempo = "120";
                    TimeMade=false;
                    TempoMade=false;
                    GrooveMade=false;

                    // Nouveau morceau :
                    // on efface le dernier accord du morceau précédent.
                    lastChordOfSong.clear();

                    QDir().mkpath(directoryName + "/" + songname);
                    QString FileName=directoryName + "/" +
                                     songname + "/" +
                                     songname + ".mma";
                    file.setFileName(FileName);

                    if (!file.open(QIODevice::WriteOnly |
                                   QIODevice::Text))
                    {
                        qWarning() << "Cannot open file";    
                        return;
                    }

                    out.setDevice(&file);

                    out << tr("// Generated by ChordV\n");
                    out << tr("// SongName ") << songname << "\n";
                    out << tr("// Title ") << title << "\"\n\n";
                    out << tr("// *** IMPORTANT **** ") << "\n" ;
                    out << tr("// This file is generated automatically)")<<"\n";
                    out << tr("// If you want to modify this file rename it")<<"\n";
                    out << tr("// On the next generation all your modifications will be lost !")<<"\n\n";
                    

                    clickbegin=true;                  
                   
                    
                    continue;
                }

                if (l.contains(timeRe))
                {
                    auto m = timeRe.match(l);
                    time = m.captured(1).trimmed();
                    out << "Time " << time << "\n";
                    TimeMade=true;
                    continue;
                }

                if (l.contains(tempoRe))
                {
                    auto m = tempoRe.match(l);
                    tempo = m.captured(1).trimmed();
                    out << "Tempo " << tempo << "\n";
                    TempoMade=true;
                    continue;
                }
                
                if (l.contains(grooveRe))
                {
                if (  clickbegin && s.value("ClickAtBegin").toBool()) 
                    {
                    out <<"\nGroove  Metronome2-4\n";
                    out << "z * 2\n\n";
                    clickbegin=false;
                    }
                auto m = grooveRe.match(l);
                groove = m.captured(1).trimmed();
                groove.replace("/",":");
                out << "Groove " << groove << "\n\n";
                out << tr("// if you change this groove the following lines will be recalculated\n");
                out << tr("// feel free to change the name of instruments here whithout change ")<<groove<<"\n";
                out.flush();
                writeGrooveVoices(groove); 
                GrooveMade=true;
                continue;
                }

              
            }

            currentVerse << l;
        }


         
        // Fin du fichier
        flushSong();

        if (file.isOpen())
            file.close();
            

    if (s.value("ShowMMADirectory").toBool())
    {
        QMessageBox msg(this);
        msg.setText(tr("Your MMA import files are located in %1 directory")
                        .arg(DialogConfiguration::getDirectoryMMA()));
        msg.setStandardButtons(QMessageBox::Ok);
        QCheckBox dontask(tr("Don't ask"));
        msg.setCheckBox(&dontask);
        if ( msg.exec() == QMessageBox::Ok) {
            if (dontask.isChecked()) s.setValue("ShowMMADirectory",false);
        }
    }
    if (s.value("ShowDirectory").toBool())
    {
        QMessageBox msg(this);
        msg.setText(tr("Your MMA import files are located in %1 directory")
                        .arg(DialogConfiguration::getDirectoryMMA()));
        msg.setStandardButtons(QMessageBox::Ok);
        QCheckBox dontask(tr("Don't ask"));
        msg.setCheckBox(&dontask);
        if ( msg.exec() == QMessageBox::Ok) {
            if (dontask.isChecked()) s.setValue("ShowMMADirectory",false);
        }
    }
     ui->stackedWidget->setCurrentIndex(menuindex);
    }
