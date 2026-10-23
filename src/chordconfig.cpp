#include "chordconfig.h"
#include <QRegularExpression>

ChordConfig::ChordConfig(QWidget *parent):FormConfig(parent)
{
   disableWidgets(QRegularExpression("ChordDiagramHorizontalSize$"));
   disableWidgets(QRegularExpression("SubtitleNumber"));
   disableWidgets(QRegularExpression("VerticalPausePerPage"));
   //disableWidgets(QRegularExpression("ColumnNumber"));
   InitDefault("Chord");
   QStringList list;
   list<<tr("Name of chord");
   list<<tr("Diagram of chord");
   InitComboBoxChordInclusion(list);
}
