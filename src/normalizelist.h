#ifndef NORMALIZELIST_H
#define NORMALIZELIST_H

#include <QVariant>
#include <QMap>
#include <QList>
#include <QStringList>
#include <QDebug>

//QList <QVariant> list;
//QStringList l;
//l<<"A"<<"B"<<"A"<<"B";list<<l;l.clear();
//l<<"D"<<"B"<<"D"<<"B";list<<l;l.clear();
//l<<"A"<<"B"<<"A"<<"B";list<<l;l.clear();
//l<<"D"<<"B"<<"D"<<"B";list<<l;l.clear();
//l<<"A"<<"B"<<"A"<<"B";list<<l;l.clear();
//l<<"D"<<"B"<<"D"<<"B";list<<l;l.clear();
//NormalizeList NL(list);
//NL.getNormalizedList();

class NormalizeList
{
public:
    NormalizeList( QList<QVariant> list);
    QList <QVariant> getNormalizedList();
private:
    bool isReductible(QList<QVariant> list,int num);
    QList <QVariant> m_list;
    QList <QVariant> m_normalized;
    QList <QVariant> ConstructList(int i);
    void Formated(QList<QVariant> list);
    QList<QVariant> BestList(QMap<int, QList<QVariant> > maplist);
};

#endif // NORMALIZELIST_H
