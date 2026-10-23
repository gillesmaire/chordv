
#include "ChordDetector.h"
#include <math.h>
#include <QSqlQuery>
#include <QVariant>

QStringList ChordDetector::detectChord()
{
    QStringList ret;
    if (m_listNotes.isEmpty()) return ret;


    CalculateAllChords(m_listNotes,0);
    foreach (QStringList list,getAllChords())
    {
      QString startNote=list.at(0);
      QStringList spellingArray;
     for(int i=0;i<=11; i++)
    {
     bool found=false;
     foreach (QString note,list )if(note2num(note)==(note2num(startNote)+i)%12 ) found=true;
     if(found)
         spellingArray<<num2spellingElement(i);
    }
    QString code=convertToCodeIfPossible(orderSpelling(spellingArray));

    if ( ! code.contains("ERROR"))
    ret<<QString("%1 %2").arg(startNote).arg(code);
    }
    ret.removeDuplicates();
    return ret;
}


ChordDetector::ChordDetector(QStringList listNotes)
{
    listNotes.removeDuplicates();
    m_listNotes=listNotes;

}


QString ChordDetector::convertToCodeIfPossible(QStringList spelling)
{
 QString spellingStr=spelling.join(",");
 QSqlQuery query(QString("select txtCode from tblSciType where txtSpelling='%1'").arg(spellingStr));
 if (query.next() && ! query.value(0).toString().isEmpty())
   return query.value(0).toString();
 else
 {
  return QString("ERROR");
 }
}


int ChordDetector::note2num(QString note)
{
 if(note=="C") return 0;
 if(note=="C#") return 1;
 if(note=="Db") return 1;
 if(note=="D") return 2;
 if(note=="D#") return 3;
 if(note=="Eb") return 3;
 if(note=="E") return 4;
 if(note=="F") return 5;
 if(note=="F#") return 6;
 if(note=="Gb") return 6;
 if(note=="G") return 7;
 if(note=="G#") return 8;
 if(note=="Ab") return 8;
 if(note=="A") return 9;
 if(note=="A#") return 10;
 if(note=="Bb") return 10;
 if(note=="B") return 11;
 return -1;
 }


QString   ChordDetector::num2spellingElement(int inNum)
{
    if(inNum==1 || inNum%12==1) return "b2";
    if(inNum==2 || inNum%12==2) return "2";
    if(inNum==3 || inNum%12==3) return "b3";
    if(inNum==4 || inNum%12==4) return "3";
    if(inNum==5 || inNum%12==5) return "4";
    if(inNum==6 || inNum%12==6) return "b5";
    if(inNum==7 || inNum%12==7) return "5";
    if(inNum==8 || inNum%12==8) return "b6";
    if(inNum==9 || inNum%12==9) return "6";
    if(inNum==10 || inNum%12==10) return "b7";
    if(inNum==11 || inNum%12==11) return "7";
    if(inNum==0 || inNum%12==0) return "1";
    return "ERROR NUM2SPELLINGELEMENT";
}


QStringList ChordDetector::orderSpelling( QStringList spelling)
 {
  QStringList outSpellingArray ;
  for(int i=1;i<=12;i++)
  {
   bool found=false;
   foreach( QString sciElement, spelling )
       if(spellingElement2num(sciElement)==i)  found=true;
   if(found) outSpellingArray<<num2spellingElement(i);
  }
  return outSpellingArray;
 }


int ChordDetector::spellingElement2num(QString inSc)
{
    if(inSc=="1") return 0;
    if(inSc=="b2" || inSc=="#1") return 1;
    if(inSc=="2") return 2;
    if(inSc=="b3" || inSc=="#2") return 3;
    if(inSc=="3" || inSc=="b4")  return 4;
    if(inSc=="4" || inSc=="#3")  return 5;
    if(inSc=="b5" || inSc=="#4") return 6;
    if(inSc=="5") return 7;
    if(inSc=="b6" || inSc=="#5") return 8;
    if(inSc=="6") return 9;
    if(inSc=="b7" || inSc=="#6") return 10;
    if(inSc=="7"  || inSc=="b1") return 11;
    return 0;
}


QMap <int, QMap <int , QString > > ChordDetector::getModeArray(QString spelling)
 {
  QMap <int,QMap <int , QString >> realModeArray;
  realModeArray[0][0]="1";
  realModeArray[0][1]=spelling;

  QString spellingWithOne = QString("1,%1").arg(spelling);
  int numNotes=spellingWithOne.split(",").count();
  QMap <int,QMap <int , QString >> modeArray;
  int key=0;
  int   realModeNum=1;
  for( int thisMode=1; thisMode<numNotes; thisMode++)
  {
    foreach ( QString value, spellingWithOne.split(","))
    {
     modeArray[thisMode][(thisMode+key)%numNotes]=value;
     key++;
    }
  }
  QString outMSc;
  for( int thisMode=numNotes-1; thisMode>0; thisMode--)
  {
   outMSc.clear();
   int startNum = spellingElement2num(modeArray[thisMode][0]);
   for( int thisSc=0; thisSc < numNotes; thisSc++ )
   {
    if( thisSc >= 2 )
      outMSc+=",";
    if( thisSc >= 1 )
    {
      outMSc += num2spellingElement((spellingElement2num(modeArray[thisMode][thisSc]) + 12 - startNum )%12 );
     }
  }
   realModeArray[realModeNum][0]=modeArray[thisMode][0];
   realModeArray[realModeNum][1]=outMSc;
   realModeNum++;

 }
  return realModeArray;
}


void ChordDetector::round(QStringList &list,int i)
{
      QString temp = list[i];
      for(int j=i;j < list.count()-1;j++){
          list[j] = list[j+1];
      }
      list[list.count()-1]= temp;
}

// Calculate All chord fo all the first note possible.
// The chord given is then given by tblSciType record in chordV Database
void ChordDetector::CalculateAllChords(QStringList list,int first)
{
          if ((list.count() - first) <= 1){
            m_listAllChords<<list ;
          }else {
            for (int i = 0; i < list.count()-first ; i++) {
            round(list, first);
            CalculateAllChords(list, first+1);
            }
          }
}

QList<QStringList> ChordDetector::getAllChords()
{
    return m_listAllChords;
    m_listAllChords.clear();
}
