#ifndef DIALOGNEWSONG_H
#define DIALOGNEWSONG_H

#include <QDialog>

namespace Ui {
class DialogNewSong;
}
///
/// \brief The DialogNewSong class this Dialog ask title, sybtitel,  compress  output yes or no, and column number for a song
/// it provide a SIGNAL whe you clik on Insert button
///
class DialogNewSong : public QDialog
{
    Q_OBJECT

public:
    enum Insertion { UnderCursor, SongSort, AuthorSongSort };
    ///
    /// \brief DialogNewSong : classic constructor
    /// \param parent : parent window
    ///
    explicit DialogNewSong(QWidget *parent = 0);
    ~DialogNewSong();

private:
    Ui::DialogNewSong *ui;
signals:
    ///
    /// \brief InsertSong signal sent when Insert button is clicked
    /// \param title : title of song
    /// \param subtitle : subtitle
    /// \param compress : compress true or false
    /// \param columns : number of columns for the song
    /// \param tempo : the tempo or beat per minute default 120
    /// \param time1 : default 4 :  4 / 4 the second 4 is time2
    /// \param time2 : see time1
    ///
    void InsertSong(QString title, QString subtitle, bool compress, int columns, double tempo, int time1 , int time2 ,
                    DialogNewSong::Insertion i );
private slots:
    ///
    /// \brief ActiveInsertButton set or unset insert button : enable only if title is not empty
    ///
    void ActiveInsertButton();
    ///
    /// \brief Insert call the signal when the insert button is pressed
    ///
    void Insert();
    void SaveIncludeOrder(int);
};

#endif // DIALOGNEWSONG_H
