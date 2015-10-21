#include "Sidebar.h"
#include "TabWidget.h"

#include <QPainter>
#include <QStyleOption>
#include <QLayout>
#include <QLabel>

/** MainPanel constructor
* Constructs the sidebar by depth - back to front.
* \param p Inherited palette configuration for setting StyleSheets.
* \param parent Pointer to parent widget.
*/
Sidebar::Sidebar(QSettings* p, QWidget* parent)
    : QWidget(parent)
{
    setObjectName("sidebar");

    setMinimumHeight(50);
    setMaximumHeight(50);
    setStyleSheet("background-color: #111111;");

    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    // Main layout
    QHBoxLayout* mainLayout = new QHBoxLayout;
    mainLayout->setSpacing(0);
    mainLayout->setMargin(0);
    setLayout(mainLayout);

    // Upper layout
    QHBoxLayout* upperLayout = new QHBoxLayout;
    upperLayout->setSpacing(5);
    upperLayout->setMargin(18);
    mainLayout->addLayout(upperLayout);

    // Tabs
    // Home tab
    homeTab = new TabWidget("homeTab", "HOME", p, this);
    upperLayout->addWidget(homeTab);

    // Store tab
    storeTab = new TabWidget("storeTab", "STORE", p, this);
    upperLayout->addWidget(storeTab);

    // Games tab
    gamesTab = new TabWidget("gamesTab", "GAMES", p, this);
    upperLayout->addWidget(gamesTab);

    // Community tab
    communityTab = new TabWidget("communityTab", "COMMUNITY", p, this);
    upperLayout->addWidget(communityTab);

    // News tab
    newsTab = new TabWidget("newsTab", "NEWS", p, this);
    upperLayout->addWidget(newsTab);

    // Mods tab
    modsTab = new TabWidget("modsTab", "MODS", p, this);
    upperLayout->addWidget(modsTab);
    upperLayout->addStretch();

    // Settings tab
    settingsTab = new TabWidget("settingsTab", "SETTINGS", p, this);
    upperLayout->addWidget(settingsTab);

    show();
}

/** Overridden size hint.
* Ensures visibility of the sidebar at app launch.
* \return Constant QSize representing the preferred sidebar size.
*/
QSize Sidebar::sizeHint() const
{
    return QSize(224, 224);
}

/** Overridden paint event.
* Necessary for displaying stylesheets correctly.
* \param event The QPaintEvent trigger.
*/
void Sidebar::paintEvent(QPaintEvent* event)
{
    // Default paint event
    QWidget::paintEvent(event);

    // Style-aware drawing
    QStyleOption option;
    QPainter p(this);
    option.init(this);
    style()->drawPrimitive(QStyle::PE_Widget, &option, &p, this);
}
