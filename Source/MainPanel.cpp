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
* Note that the navbar is initialized as a derived class.
*/
void MainPanel::init()
{
    p = new QSettings(QSettings::IniFormat, QSettings::UserScope, "HorizonLauncher", "palette");

    // Main panel layout
    QGridLayout* mainGridLayout = new QGridLayout;
    mainGridLayout->setSpacing(0);
    mainGridLayout->setMargin(0);
    setLayout(mainGridLayout);

    // Core widget
    QWidget* coreWidget = new QWidget();
    coreWidget->setObjectName("coreWidget");
    coreWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    coreWidget->setStyleSheet("background-color: " + p->value("Body/Background").toString() + ";");
    mainGridLayout->addWidget(coreWidget);

    // Vertical layout #1
    QVBoxLayout* verticalLayout1 = new QVBoxLayout;
    verticalLayout1->setSpacing(0);
    verticalLayout1->setMargin(0);
    verticalLayout1->setAlignment(Qt::AlignHCenter);
    coreWidget->setLayout(verticalLayout1);

    // Title bar widget
    QWidget* borderWidget = new QWidget;
    borderWidget->setStyleSheet("background-color: " + p->value("TitleBar/Color").toString() + ";"
                                "background-image: url(:SystemMenu/Images/TitleBarPattern.png);"
                                "background-repeat: repeat-x;");
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
    QObject::connect(pushButtonMinimize, &QPushButton::clicked, this, &MainPanel::pushButtonMinimize);
    // Maximize
    QPushButton* pushButtonMaximize = new QPushButton("", coreWidget);
    pushButtonMaximize->setObjectName("pushButtonMaximize");
    windowControlLayout->addWidget(pushButtonMaximize);
    QObject::connect(pushButtonMaximize, &QPushButton::clicked, this, &MainPanel::pushButtonMaximize);
    // Close
    QPushButton* pushButtonClose = new QPushButton("", coreWidget);
    pushButtonClose->setObjectName("pushButtonClose");
    windowControlLayout->addWidget(pushButtonClose);
    QObject::connect(pushButtonClose, &QPushButton::clicked, this, &MainPanel::pushButtonClose);

    // Navbar
    navbar = new Navbar(p, coreWidget);
    verticalLayout1->addWidget(navbar);

    // Main Horizontal Layout
    QHBoxLayout* horizontalLayout = new QHBoxLayout;
    horizontalLayout->setSpacing(0);
    horizontalLayout->setMargin(0);
    horizontalLayout->setAlignment(Qt::AlignVCenter);
    verticalLayout1->addLayout(horizontalLayout);

    // Backdrop widget
    QWidget* mainPanelBackdrop = new QWidget(coreWidget);
    mainPanelBackdrop->setObjectName("mainPanelBackdrop");
    horizontalLayout->addWidget(mainPanelBackdrop);

    // Vertical layout #2
    QVBoxLayout* verticalLayout2 = new QVBoxLayout;
    verticalLayout2->setSpacing(0);
    verticalLayout2->setMargin(0);
    verticalLayout2->setAlignment(Qt::AlignHCenter);
    mainPanelBackdrop->setLayout(verticalLayout2);

    // Main panel scroll area
    scrollArea = new QScrollArea(coreWidget);
    scrollArea->setWidgetResizable(true);
    scrollArea->setObjectName("mainPanelScrollArea");
    scrollArea->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    verticalLayout2->addWidget(scrollArea);

    // Stacked content panel
    stack = new QStackedWidget(scrollArea);
    stack->setObjectName("stack");
    stack->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    scrollArea->setWidget(stack);

    // Stack widgets
    home = new Homepage(p, stack);
    home->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    library = new Library(p, stack);
    library->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    community = new Community(p, stack);
    community->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    news = new News(p, stack);
    news->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    stack->addWidget(home);
    stack->addWidget(library);
    stack->addWidget(community);
    stack->addWidget(news);
    stack->setCurrentWidget(library);

    connect(stack, &QStackedWidget::currentChanged, this, &MainPanel::onStackedChanged);

    // Set active tab
    activeTab = navbar->gamesTab;
    activeTab->toggleActive();

    // Connect signals
    connect(navbar->homeTab, &TabWidget::clicked, this, &MainPanel::setHome);
    // connect(navbar->storeTab, &TabWidget::clicked, this, &MainPanel::setStore);
    connect(navbar->gamesTab, &TabWidget::clicked, this, &MainPanel::setGames);
    connect(navbar->communityTab, &TabWidget::clicked, this, &MainPanel::setCommunity);
    connect(navbar->newsTab, &TabWidget::clicked, this, &MainPanel::setNews);
    // connect(navbar->modsTab, &TabWidget::clicked, this, &MainPanel::setMods);

    settings = new Settings(p);
    connect(navbar->settingsTab, &TabWidget::clicked, this, &MainPanel::openSettings);
    connect(settings, &Settings::didUpdateFeedURLs, news, &News::loadFeeds);
    // Show
    show();
}

void MainPanel::onStackedChanged(int index)
{
    QWidget* curWidget = stack->widget(index);
    QSize windowSize = this->size();
    windowSize.setHeight(windowSize.height() - (windowSize.height() - scrollArea->viewport()->height()));
    curWidget->setMinimumSize(windowSize);
    curWidget->adjustSize();
}
