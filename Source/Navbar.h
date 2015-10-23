#pragma once

#include "TabWidget.h"

#include <QWidget>
#include <QSettings>

/** Navbar class.
* Class to handle the layout and control of the navbar.
*/
class Navbar : public QWidget
{
    Q_OBJECT

public:
    explicit Navbar(QSettings* p, QWidget* parent = 0);
    virtual QSize sizeHint() const;

    TabWidget *homeTab, *storeTab, *gamesTab, *communityTab, *newsTab, *modsTab, *settingsTab;

protected:
    void paintEvent(QPaintEvent* event);
};
