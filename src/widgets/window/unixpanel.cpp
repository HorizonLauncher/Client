#include "unixpanel.h"
#include "unixwindow.h"

UnixPanel::UnixPanel(UnixWindow* panel)
    : MainPanel((QWidget*) panel),
      unixPanel(panel)
{
    this->adjustSize();
}

void UnixPanel::pushButtonMinimize()
{
    unixPanel->minimize();
}

void UnixPanel::pushButtonMaximize()
{
    unixPanel->maximize();
}

void UnixPanel::pushButtonClose()
{
    unixPanel->closeWindow();
}
