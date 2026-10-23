#include "processortext.h"
#include "ui_formconfig.h"

#include <QDebug>

ProcessorText::ProcessorText(Ui::MainWindow *ui1, Ui::FormConfig *ui2):Processor(ui1,ui2)
{
    m_mode="text";
}

