#ifndef _UNIXWINDOW_H_
#define _UNIXWINDOW_H_

#include "UnixPanel.h"

class UnixWindow : public QWidget
{
    Q_OBJECT

public:
    UnixWindow();

    void minimize();
    void maximize();
    void closeWindow();

private:
    UnixPanel* mainPanel;
    QPoint oldWindowPos;
    bool dragging;

    void mousePressEvent(QMouseEvent* evt);
    void mouseDoubleClickEvent(QMouseEvent* evt);
    void mouseReleaseEvent(QMouseEvent* evt);
    void mouseMoveEvent(QMouseEvent* evt);
};

#endif
