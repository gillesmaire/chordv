#include "normalizelist.h"



NormalizeList::NormalizeList(QList <QVariant> list)
{
  m_list=list;
  QMap <int, QList<QVariant>> allList;
  int al=0;
  for ( int i=2; i<= m_list.count()/2; i++)
  {
      allList[al++]=ConstructList(i);
  }
  m_normalized=BestList(allList);
}

void NormalizeList::Formated(QList <QVariant> list)
{
    foreach ( QVariant v, list)
    {
        if (v.toInt()!=0)
        {
            qDebug()<<QString("%1x").arg(v.toInt());
        }
        else
            qDebug()<< v.toStringList();
    }
}

QList<QVariant> NormalizeList::getNormalizedList()
{
    Formated(m_normalized);
    return m_normalized;
}

bool NormalizeList::isReductible(QList<QVariant> list,int num)
{
    for ( int i=0 ; i<num ;i++)
        if (list.at(i)!=list.at(i+list.count()/2)) return false;
    return true;
}


QList<QVariant> NormalizeList::ConstructList( int i)
{
    int rest=m_list.count()%i;
    QList<QVariant> retlist;
    retlist=m_list;
    QMap <int, QList<QVariant> > maplist;
    int k=0;
    for ( int comb=0;comb<=rest;comb++)
        {
            if ( comb == 0 )
            {
                if ( isReductible(retlist,i) )
                {
                    QList <QVariant> list;
                    for ( int j=0; j<i; j++)
                        list<<retlist.at(j);
                    maplist[k++]<<i<<list;
                }
            }
            else
            {
                retlist.removeFirst();
                if ( isReductible(retlist,i) )
                {
                    QList <QVariant> list;
                    for ( int j=0; j<i; j++)
                        list<<m_list.at(j);
                    maplist[k++]<<i<<list;
                }
            }
        }
    retlist=BestList(maplist);
    return retlist;
}


QList <QVariant> NormalizeList::BestList(QMap <int, QList<QVariant> > maplist )
{
    int count=1000000000;
    int kmmin=1000000000;
    foreach ( int km, maplist.keys())
    {
        if (maplist[km].count() < count )
        {
            kmmin=km;
            count =maplist[km].count();
        }
    }
    return maplist[kmmin];
}
