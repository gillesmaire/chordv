#include "chorddiagram.h"
#include <QPainter>


ChordDiagram::ChordDiagram(QWidget *parent) : QWidget(parent)
{
    m_diagram=QString();
    setMinimumHeight(50);
}

ChordDiagram::ChordDiagram(QWidget *parent, int , int height, QString diagram) : QWidget (parent)
{
    m_diagram=diagram;
    setMaximumHeight(height);
}

void ChordDiagram::setDiagram(QString diagram)
{
    m_diagram=diagram;
    repaint();
}

QString ChordDiagram::getDiagram()
{
    return m_diagram;
}

void ChordDiagram::paint(QPainter &painter , int w, int h , QString diagram )
{
    int csx,csy;
    if ( h > w)
    {
        csx=w/6; // case size x
        csy=w/5; // case size x
    }
    else
    {
        csx=h/6; // case size x
        csy=h/5; // case size x
    }

    int hm=h/8; // horizontal margin
    int xmfret=0;   // x to draw text with fret
    int ymfret=csy*3/5;  // y to draw text with fret
    int x=w/2-3*csx;
    int y=hm;
    int rcercle=csx/2;
    // draw string
    for ( int i=1;i<=6;i++)
    {
        painter.drawLine(x+csx*i,y,x+csx*i,h);
    }
    // draw fret
    for (int i= 1 ; i<= 5 ; i++)
    {
        painter.drawLine(x+csx,y+csy*i,x+csx*6,y+csy*i);
    }

    if ( !diagram.isEmpty())
    {
        QStringList list=diagram.split(" ");
        QFont font;
        font.setPixelSize(rcercle<8?8:rcercle);
        painter.setFont(font);
        if ( list.at(0) != "0") painter.drawText(QPoint(x+xmfret,y+ymfret),list.at(0));
        for ( int i=1;i<=6;i++)
        {
            QString note=list.at(i);
            if (note == "0" )
            {
                QBrush brush(Qt::NoBrush);
                painter.setBrush(brush);
                painter.drawEllipse(x+csx*i-csx/4,y+csy*note.toInt()-csy/2-rcercle/3,rcercle,rcercle);
            }
            else if ( note!="x")
            {
                QBrush brush(Qt::SolidPattern);
                painter.setBrush(brush);
                painter.drawEllipse(x+csx*i-csx/4,y+csy*note.toInt()-csy/2-rcercle/2,rcercle,rcercle);
            }
            else
            {
                painter.drawText(QPoint(x+csx*i-csx/4,y*note.toInt()+csx/2),"x");
            }
        }

    }
    QPen pen;
    pen.setWidth(3);
    painter.setPen(pen);
    painter.drawLine(x+csx,y-2,x+csx*6,y-2);

}

QPixmap ChordDiagram::DiagramToPixmap(QString diagram, int width, int height)
{
    QPixmap image(width, height);
    QPainter p;
    p.begin(&image);
    paint(p,width, height, diagram);
    p.end();
    return image;
}

void ChordDiagram::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    paint ( p, width(), height(), m_diagram );
}

