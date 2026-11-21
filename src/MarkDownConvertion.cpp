// GPL 3.0 License
// Copyright (c)2026 Gilles Maire <gilles@gillesmaire.com>
// Project : %PROJECTNAME%
// Filename : MarkDownConvertion.cpp
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
void MainWindow::ConvertInMarkDown()
{
    QSettings s;
    QStringList lines=ui->textEditCho3File->document()->toPlainText().split('\n');
    bool split =s.value("SplitInSeparateFile",false).toBool();
    QString directoryMD=s.value("DestinationDirectoryMarkDown").toString();
    QString DirectorySlideBar=s.value("DirectoryWithMusicFilesMarkDown").toString();
    QFile file(directoryMD+"/chansons.md"); 
    QFile toc(DirectorySlideBar+"/sidebar.html"); 
    QDir dir(directoryMD);
   
    for (const QString &file : dir.entryList(QDir::Files)) {
        dir.remove(file) ;
    }
    QTextStream out(&file);
    QTextStream outtoc(&toc);
    
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {qDebug()<< "problem file";return;}
    if (!toc.open(QIODevice::WriteOnly | QIODevice::Text)) {qDebug()<<"problem toc";return;}
    outtoc<<"<section>\n<h3>{{T \"Liste des chansons\"}}</h3>\n<ul class=\"links\">\n";
    QString last;
    QString lastlast;
    for (auto line : lines )
    {
        if (line.startsWith('{'))
        {   
            static const QRegularExpression titleRe("^\\{title:(.*?)\\}$");
            static const QRegularExpression subtitleRe("^\\{subtitle:(.*?)\\}$");
            static const QRegularExpression disqueRe("^\\{subtitle: *Disque");
            static const QRegularExpression multiblank(" +");
            static const QRegularExpression refrain("{start_of_chorus}");
            if ( line.contains(titleRe))  {  
             if ( split )
              { 
                line.replace(titleRe,"\\1");
                line=line.trimmed();
                out<<"\n\n[ ◀ "+lastlast+"](../"<<lastlast.toLower().replace(" ","_").replace("'","~")<<") ​ ​ ​ ​ ​ ​ ​ ​ ​ ​ ​ ​["+line+" ▶](../"<<line.toLower().replace(" ","_").replace("'","~")<<")\n\n";
                lastlast=last;
                last=line;
                out<<"\n<b><sub>Chanson sous licence Creative Common BY-NC-ND</sub></b>\n";
                file.close();
                
            
                QString line2=line;
                line2=line2.replace(' ','_').replace('\'','~').toLower();
                // out<<last<<current<<next;
                out<<"---\n";
                out<<QString("Title: \"%1\"\n").arg(line);
                //out<<QString("slug: \"%1\"\n").arg(line2);
                out<<QString("sidebar_left: sidebar\n");
                out<<"---\n\n";
                //out<<"# "+line+"\n";
                outtoc<<QString("<li><a href=\"../%1\">%2</a></li>\n").arg(line2).arg(line);
                file.setFileName(s.value("DestinationDirectoryMarkDown").toString()+"/"+line2+".md");
                if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
                    return;
              }  
              else    
                {
                 line.replace(titleRe, "# \\1");
                  out<<line<<" \n";
                } 
            }
            else if (line.contains(subtitleRe)) {
                  if (!  line.contains(disqueRe) )
                  {
                   line.replace(subtitleRe,"## \\1");
                   out<<line<<"\n";
                   }        
               }
               
           
            else if ( line.length()==0 )
            {
//               line=line.simplified();
               
            }
            else if (line.contains(refrain)) 
            {
               out<<"\n\n**Refrain:**\n\n";
            }

        }
        else    
        {
          QRegularExpression re("\\[.*?\\]");
          line.remove(re);
          line.replace("\\","\n");
          line=line.simplified();
          out << line<<"  \n";
        }
  
   
    }      
    
   if (s.value("ShowMarkDownDirectory").toBool())
    {
        QMessageBox msg(this);
        msg.setText(tr("Your MarkDon import files are located in %1 directory")
                        .arg(DialogConfiguration::getDirectoryMMA()));
        msg.setStandardButtons(QMessageBox::Ok);
        QCheckBox dontask(tr("Don't ask"));
        msg.setCheckBox(&dontask);
        if ( msg.exec() == QMessageBox::Ok) {
            if (dontask.isChecked()) s.setValue("ShowMMADirectory",false);
        }
    }
    
}
