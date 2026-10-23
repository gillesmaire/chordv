#include "textconfig.h"

TextConfig::TextConfig(QWidget *parent):FormConfig(parent)
{
    disableGroup("groupBoxChord");
    InitDefault("Text");
}
