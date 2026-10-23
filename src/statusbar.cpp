#include "statusbar.h"
#include <QFileInfo>
#include <QDir>
#include <QDebug>
#include <QSettings>
#include <QRegularExpression>

#include <QCoreApplication>
StatusBar::StatusBar(QWidget *parent) : QStatusBar(parent)
{
     addWidget(&mLabelactivestacked);
     mLabelactivestacked.setStyleSheet("Background-color:#d5e6ef;Color:#5d5555;font: bold 12px; ");
     addWidget(&mFilenameStatusBar);
     addWidget(&mDirnameStatusBar);
     addWidget(&mSongnumberStatusBar);
     mSelectedsongStatusBar.setStyleSheet("QLineEdit:disabled{background-color:#e8e99c;Color:#5d5555};QLineEdit{font: bold 12px;}");
     mSongnumberStatusBar.setStyleSheet("Background-color:#d5e6ef;Color:#5d5555;font: bold 12px;");
     addWidget(&mSelectedsongStatusBar);
     addWidget(&mProtectStatusBar);
     mProtectStatusBar.setStyleSheet("Background-color:#d5e6ef;Color:#5d5555;font: bold 12px;");
     mCheckWin = new DialogChekAllChordDefined(mFormEditor,this);
     setCheckBoxActived();

}

void StatusBar::setFormEditor(FormEditor *formeditor)
{
    mFormEditor=formeditor;
}

DialogChekAllChordDefined* StatusBar::getCheckWin()
{
    return mCheckWin;
}


void StatusBar::setModified(bool modified)
{
    if ( !modified )
         mFilenameStatusBar.setStyleSheet("Background-color:#95ce7e;Color:#5d5555;font: bold 12px;");
    else
        mFilenameStatusBar.setStyleSheet("Background-color:#E08546;Color:#5d5555;font: bold 12px;");
}

void StatusBar::makeStackedList()
{
    mListStacked.clear();
    mListStacked<<QCoreApplication::translate("StatusBar","Editor")<<QCoreApplication::translate("StatusBar","Global definitions")
                 <<QCoreApplication::translate("StatusBar","Text and chord Mode")<<QCoreApplication::translate("StatusBar","Chord Mode")
                 <<QCoreApplication::translate("StatusBar","Text mode")<<QCoreApplication::translate("StatusBar","Memory Mode");
}

QString StatusBar::getStackedLabel(int i)
{
    return mListStacked.at(i);
}

void StatusBar::Retranslate()
{
    makeStackedList();
    setSongNumberStatusbar(mSongNumber);
    setActiveStacked(mActiveStacked);
    setCheckBoxActived();

}

void StatusBar::setFileNameStatusBar(QString filename , bool modified)
{
    QFileInfo fi(filename);
    if (filename.isEmpty() )
      {
        QSettings s;
        mFilenameStatusBar.clear();
        mDirnameStatusBar.setStyleSheet("Background-color:#95ce7e;Color:#5d5555;font: bold 12px;");
        mDirnameStatusBar.setText(s.value("LastOpenedDirectory","").toString());
      }
    else
    {
        if ( fi.isWritable() )
          setModified(modified);
        else
          mFilenameStatusBar.setStyleSheet("Background-color:#dd8585;Color:#5d5555;font: bold 12px;");
        mFilenameStatusBar.setText(fi.fileName());
        mDirnameStatusBar.setText(fi.absoluteDir().absolutePath());
    }
}

//void StatusBar::setDirNameStatusBar(QString filename)
//{
//    QFileInfo fi(filename);
//    if ( fi.isWritable() )
//         mDirnameStatusBar.setStyleSheet("Background-color:#95ce7e;Color:#5d5555;font: bold 12px;");
//    else
//         mDirnameStatusBar.setStyleSheet("Background-color:#dd8585;Color:#5d5555;font: bold 12px;");
//    mDirnameStatusBar.setText(fi.absolutePath());
//}

void StatusBar::setSongNumberStatusbar(int number)
{
    mSongnumberStatusBar.setText(QCoreApplication::translate("StatusBar","%1 song(s)").arg(number));
    mSongNumber=number;
}

void StatusBar::setCheckBoxActived()
{
     mProtectStatusBar.setText(QCoreApplication::translate("StatusBar","Keep selected Song"));
}

QLineEdit *StatusBar::getSelectedSongStatusBar()
{
    return &mSelectedsongStatusBar;
}

QLabel *StatusBar::getLabelactivestacked()
{
    return &mLabelactivestacked;
}

QLabel *StatusBar::getLabelFileName()
{
    return &mFilenameStatusBar;
}



QLabel *StatusBar::getLabelDirName()
{
    return &mDirnameStatusBar;
}



void StatusBar::setSelectedSongStatusBar(QString songname)
{
    if ( ! mProtectStatusBar.isChecked())   mSelectedsongStatusBar.setText(songname);
}

void StatusBar::setActiveStacked(int index)
{
    makeStackedList();
    mActiveStacked=index;
    mLabelactivestacked.setText(mListStacked.at(index));
}

QString StatusBar::getProjectName()
{
    return mFilenameStatusBar.text().replace(QRegularExpression("\\.chop$"),"");
}

QString StatusBar::getCompleteFileName()
{
    return getLabelDirName()->text()+"/"+getProjectName()+".chop";
}

