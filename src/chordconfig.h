#ifndef CHORDCONFIG_H
#define CHORDCONFIG_H

#include <QMainWindow>
#include <QObject>
#include "formconfig.h"

class ChordConfig : public FormConfig
{
  Q_OBJECT
public:
    ChordConfig(QWidget *parent = 0);
};

#endif // CHORDCONFIG_H
