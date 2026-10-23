#ifndef STATUSBAR_H
#define STATUSBAR_H

#include <QStatusBar>
#include <QLabel>
#include <QLineEdit>
#include <QCheckBox>
#include "dialogchekallchorddefined.h"
#include "formeditor.h"

class Ui_MainWindow;

class StatusBar : public QStatusBar
{
public:
    StatusBar( QWidget *parent );
    void setFormEditor( FormEditor *formeditor);
    DialogChekAllChordDefined *getCheckWin();
    void setFileNameStatusBar( QString filename , bool modified);
  //  void setDirNameStatusBar(QString filename);
    void setSongNumberStatusbar(int number);
    QLineEdit *getSelectedSongStatusBar();
    QLabel *getLabelactivestacked();
    QLabel *getLabelFileName();
    QLabel *getLabelDirName();
    void setSelectedSongStatusBar(QString songname);
    void setActiveStacked(int index);
    QString getProjectName();
    QString getCompleteFileName();
    void setModified(bool modified);
    void Retranslate();
    void makeStackedList();
    QString getStackedLabel(int i);
    void setCheckBoxActived();
private:
     QLabel mLabelactivestacked;
     QLabel mFilenameStatusBar;
     QLabel mDirnameStatusBar;
     QLabel mSongnumberStatusBar;
     QLineEdit mSelectedsongStatusBar;
     QCheckBox mProtectStatusBar;
     FormEditor *mFormEditor ;
     DialogChekAllChordDefined *mCheckWin;
     int mSongNumber=0;
     int mActiveStacked;
     QStringList mListStacked;
};

#endif // STATUSBAR_H
