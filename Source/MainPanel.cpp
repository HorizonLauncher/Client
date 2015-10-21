#include "MainPanel.h"
#include "Settings.h"

#include <QMessageBox>
#include <QGridLayout>
#include <QPushButton>
#include <QScrollArea>
#include <QLabel>


/** MainPanel constructor
* Sets base size policy and object name.
* \param parent Pointer to parent widget.
*/
MainPanel::MainPanel(QWidget* parent)
    : QWidget(parent)
{
    setObjectName("mainPanel");

    init();
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    show();
}

/** Main initializer for the UI.
* QObjects are initialized by depth - back to front.
* Note that the sidebar is initialized as a derived class.
*/
void MainPanel::init()
{
    p = new QSettings(QSettings::IniFormat, QSettings::UserScope, "Horizon Launcher", "palette");

    // Main panel layout
    QGridLayout* mainGridLayout = new QGridLayout;
    mainGridLayout->setSpacing(0);
    mainGridLayout->setMargin(0);
    setLayout(mainGridLayout);

    // Main panel scroll area
    QScrollArea* scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setObjectName("mainPanelScrollArea");
    scrollArea->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    mainGridLayout->addWidget(scrollArea);

    // Core widget
    QWidget* coreWidget = new QWidget(scrollArea);
    coreWidget->setObjectName("coreWidget");
    coreWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    scrollArea->setWidget(coreWidget);
    coreWidget->setStyleSheet("background-color: #212121;");

    // Vertical layout #1
    QVBoxLayout* verticalLayout1 = new QVBoxLayout;
    verticalLayout1->setSpacing(0);
    verticalLayout1->setMargin(0);
    verticalLayout1->setAlignment(Qt::AlignHCenter);
    coreWidget->setLayout(verticalLayout1);

    QWidget* borderWidget = new QWidget;
    borderWidget->setStyleSheet("background-color: #F5F6F7;");
    verticalLayout1->addWidget(borderWidget);

    // Window Control Horizontal Layout
    QHBoxLayout* windowControlLayout = new QHBoxLayout;
    windowControlLayout->setSpacing(0);
    windowControlLayout->setMargin(8);
    borderWidget->setLayout(windowControlLayout);
    windowControlLayout->addStretch();

    // Window controls
    // Minimize
    QPushButton* pushButtonMinimize = new QPushButton("", coreWidget);
    pushButtonMinimize->setObjectName("pushButtonMinimize");
    windowControlLayout->addWidget(pushButtonMinimize);
    QObject::connect(pushButtonMinimize, SIGNAL(clicked()), this, SLOT(pushButtonMinimize()));
    // Maximize
    QPushButton* pushButtonMaximize = new QPushButton("", coreWidget);
    pushButtonMaximize->setObjectName("pushButtonMaximize");
    windowControlLayout->addWidget(pushButtonMaximize);
    QObject::connect(pushButtonMaximize, SIGNAL(clicked()), this, SLOT(pushButtonMaximize()));
    // Close
    QPushButton* pushButtonClose = new QPushButton("", coreWidget);
    pushButtonClose->setObjectName("pushButtonClose");
    windowControlLayout->addWidget(pushButtonClose);
    QObject::connect(pushButtonClose, SIGNAL(clicked()), this, SLOT(pushButtonClose()));

    // Sidebar widget - locked width
    sidebar = new Sidebar(p, coreWidget);
    verticalLayout1->addWidget(sidebar);

    // Horizontal layout #2
    QHBoxLayout* horizontalLayout2 = new QHBoxLayout;
    horizontalLayout2->setSpacing(0);
    horizontalLayout2->setMargin(0);
    horizontalLayout2->setAlignment(Qt::AlignVCenter);
    verticalLayout1->addLayout(horizontalLayout2);

    // Backdrop widget - vertical layout #3
    QWidget* mainPanelBackdrop = new QWidget(coreWidget);
    mainPanelBackdrop->setObjectName("mainPanelBackdrop");
    horizontalLayout2->addWidget(mainPanelBackdrop);

    // Vertical layout #3
    QVBoxLayout* verticalLayout3 = new QVBoxLayout;
    verticalLayout3->setSpacing(0);
    verticalLayout3->setMargin(0);
    verticalLayout3->setAlignment(Qt::AlignHCenter);
    mainPanelBackdrop->setLayout(verticalLayout3);

    // Stacked content panel
    stack = new QStackedWidget(coreWidget);
    stack->setObjectName("stack");
    stack->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    verticalLayout3->addWidget(stack);

    // Stack widgets
    home = new Homepage(p, stack);
    library = new Library(p, stack);
    community = new Community(p, stack);
	settings = new Settings(p, stack);
    stack->addWidget(home);
    stack->addWidget(library);
    stack->addWidget(community);
	stack->addWidget(settings);
    stack->setCurrentWidget(library);

    // Set active tab
    activeTab = sidebar->gamesTab;
    activeTab->toggleActive();

    // Connect signals
    connect(sidebar->homeTab, SIGNAL(clicked()), this, SLOT(setHome()));
    // connect(sidebar->storeTab, SIGNAL(clicked()), this, SLOT(setStore()));
    connect(sidebar->gamesTab, SIGNAL(clicked()), this, SLOT(setGames()));
    connect(sidebar->communityTab, SIGNAL(clicked()), this, SLOT(setCommunity()));
    // connect(sidebar->newsTab, SIGNAL(clicked()), this, SLOT(setNews()));
    // connect(sidebar->modsTab, SIGNAL(clicked()), this, SLOT(setMods()));
    connect(sidebar->settingsTab, SIGNAL(clicked()), this, SLOT(setSettings()));

    // Show
    show();
}
