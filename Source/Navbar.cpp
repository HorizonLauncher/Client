#include "Navbar.h"
#include "TabWidget.h"

#include <QPainter>
#include <QStyleOption>
#include <QLayout>
#include <QLabel>

/** MainPanel constructor
* Constructs the navbar by depth - back to front.
* \param p Inherited palette configuration for setting StyleSheets.
* \param parent Pointer to parent widget.
*/
Navbar::Navbar(QSettings* p, QWidget* parent)
    : QWidget(parent)
{
    setObjectName("navbar");

    setMinimumHeight(50);
    setMaximumHeight(50);
    setStyleSheet("background-color: " + p->value("Navbar/Background").toString() + ";");

    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    // Main layout
    QHBoxLayout* mainLayout = new QHBoxLayout;
    mainLayout->setSpacing(0);
    mainLayout->setMargin(0);
    setLayout(mainLayout);

    // Upper layout
    QHBoxLayout* upperLayout = new QHBoxLayout;
    upperLayout->setSpacing(40);
    upperLayout->setMargin(18);
    mainLayout->addLayout(upperLayout);

    // Tabs
    // Home tab
    homeTab = new TabWidget("homeTab", tr("HOME"), p, this);
    upperLayout->addWidget(homeTab);

    // Store tab
    storeTab = new TabWidget("storeTab", tr("STORE"), p, this);
    upperLayout->addWidget(storeTab);

    // Games tab
    gamesTab = new TabWidget("gamesTab", tr("GAMES"), p, this);
    upperLayout->addWidget(gamesTab);

    // Community tab
    communityTab = new TabWidget("communityTab", tr("COMMUNITY"), p, this);
    upperLayout->addWidget(communityTab);

    // News tab
    newsTab = new TabWidget("newsTab", tr("NEWS"), p, this);
    upperLayout->addWidget(newsTab);

    // Mods tab
    modsTab = new TabWidget("modsTab", tr("MODS"), p, this);
    upperLayout->addWidget(modsTab);
    upperLayout->addStretch();

    // Settings tab
    settingsTab = new TabWidget("settingsTab", tr("SETTINGS"), p, this);
    upperLayout->addWidget(settingsTab);

    show();
}

/** Overridden size hint.
* Ensures visibility of the navbar at app launch.
* \return Constant QSize representing the preferred navbar size.
*/
QSize Navbar::sizeHint() const
{
    return QSize(224, 224);
}

/** Overridden paint event.
* Necessary for displaying stylesheets correctly.
* \param event The QPaintEvent trigger.
*/
void Navbar::paintEvent(QPaintEvent* event)
{
    // Default paint event
    QWidget::paintEvent(event);

    // Style-aware drawing
    QStyleOption option;
    QPainter p(this);
    option.init(this);
    style()->drawPrimitive(QStyle::PE_Widget, &option, &p, this);
}
