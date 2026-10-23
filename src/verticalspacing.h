#ifndef VERTICALSPACING_H
#define VERTICALSPACING_H

#include <QComboBox>

class VerticalSpacing : public QComboBox
{
public:
    VerticalSpacing(QWidget *parent);
private:
    QStringList m_values;
    QList<double> m_datas;

};

#endif // VERTICALSPACING_H
