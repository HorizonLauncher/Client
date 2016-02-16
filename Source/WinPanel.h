#ifndef _WINPANEL_H_
#define _WINPANEL_H_

#include "MainPanel.h"

class WinWindow;

class WinPanel : public MainPanel
{
    Q_OBJECT

public:
    WinPanel(WinWindow* panel);

public slots:
    void pushButtonMinimize();
    void pushButtonMaximize();
    void pushButtonClose();

private:
    WinWindow* winPanel;
};

#endif
