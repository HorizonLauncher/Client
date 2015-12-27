#pragma once

#include "TabWidget.h"
#include "Navbar.h"
#include "Library.h"
#include "Homepage.h"
#include "BuddyList.h"
#include "Community.h"
#include "News.h"
#include "Settings.h"

#include <QStackedWidget>
#include <QApplication>
#include <QSettings>
#include <QWidget>

/** MainPanel class.
* Class to construct the core content of the launcher window.
* Also moderates interaction between navbar and stacked content.
*/
class MainPanel : public QWidget
{
    Q_OBJECT

public:
    MainPanel(QWidget* parent = 0);
    ~MainPanel()
    {
        delete p;
    }

public slots:
    virtual void pushButtonMinimize() = 0;
    virtual void pushButtonMaximize() = 0;
    virtual void pushButtonClose() = 0;
    void onStackedChanged(int index);

protected:
    QStackedWidget* stack;
    QScrollArea* scrollArea;
    void init();

private:
    QSettings* p;
    Navbar* navbar;
    TabWidget* activeTab;

    Homepage* home;
    Community* community;
    Library* library;
    Settings* settings;
    News* news;

private slots:
    void setHome()
    {
        if (!(activeTab == navbar->homeTab))
        {
            activeTab->toggleInactive();
            stack->setCurrentWidget(home);
            activeTab = navbar->homeTab;
            activeTab->toggleActive();
        }
    }
    void setStore()
    {
        if (!(activeTab == navbar->storeTab))
        {
            activeTab->toggleInactive();
            activeTab = navbar->storeTab;
            activeTab->toggleActive();
        }
    }
    void setGames()
    {
        if (!(activeTab == navbar->gamesTab))
        {
            activeTab->toggleInactive();
            stack->setCurrentWidget(library);
            activeTab = navbar->gamesTab;
            activeTab->toggleActive();
        }
    }
    void setCommunity()
    {
        if (!(activeTab == navbar->communityTab))
        {
            activeTab->toggleInactive();
            stack->setCurrentWidget(community);
            activeTab = navbar->communityTab;
            activeTab->toggleActive();
        }
    }
    void setNews()
    {
        if (!(activeTab == navbar->newsTab))
        {
            activeTab->toggleInactive();
            stack->setCurrentWidget(news);
            activeTab = navbar->newsTab;
            activeTab->toggleActive();
        }
    }
    void setMods()
    {
        if (!(activeTab == navbar->modsTab))
        {
            activeTab->toggleInactive();
            activeTab = navbar->modsTab;
            activeTab->toggleActive();
        }
    }
    void openSettings()
    {
        settings->show();
    }
};
