#ifndef FORMMEMORYINFO_H
#define FORMMEMORYINFO_H

#include <QWidget>

namespace Ui {
class FormMemoryInfo;
}

///
/// \brief The FormMemoryInfo class manage the ùmemory information configuration and execution parameter like FormConfig
///
///
class FormMemoryInfo : public QWidget
{
    Q_OBJECT

public:
    ///
    /// \brief FormMemoryInfo constructor complient with Designer call of widget children
    /// \param parent
    ///
    explicit FormMemoryInfo(QWidget *parent = 0);
    void setOffSetAsDefault();
    ~FormMemoryInfo();

private:
    Ui::FormMemoryInfo *ui;
    ///
    /// \brief Init init all the records from setting file
    ///


public:
    void Init(QString filenamae = QString());
    ///
    /// \brief Save save all the records in setting file
    ///
    void Save(QString filename=QString());
    ///
    /// \brief getFont return selected Font
    /// \return
    ///

    QFont getFont();
    ///
    /// \brief getBackgroundColor return selected background color
    /// \return
    ///
    QColor getBackgroundColor();
    ///
    /// \brief getTextColor return selected font color
    /// \return
    ///
    QColor getTextColor();
    ///
    /// \brief getScrollingText return selected text color
    /// \return
    ///
    bool getShowRythm();
    ///
    /// \brief getFullscreenMode return true if scrolling text is asked
    /// \return
    ///
    bool getFullscreenMode();
    ///
    /// \brief getPosition return 0 if top position asked, 1 for middle, 2 for bottom
    /// \return
    ///
    int getPosition();
    bool getShowTwoLines();
    double getAdvance();
    bool getClick();
    bool getAccentuedFirst();
    int getVolume();
    int getDelay();
    bool getJackSynchro();
    void setJackSynchro(bool val);
    void Retranslate();
    int getTimeBeforeUnit();
private slots:
    void ShowSelectedFont(QFont, QColor, QColor);
    void SetUnsetPosition();
    void SetUnsetAdvance(int time);
    void SetUnsetClick();
    void SetSynchronisation(bool);
    void SaveAsDefault();
signals:
    void SynchroMode(bool);
};

#endif // FORMMEMORYINFO_H
