#include "LibraryCarouselView.h"

#include "GridGameWidget.h"
#include "AddGameWizard.h"

/** LibraryCarouselView constructor
 * Initialize the carousel UI and generate an initial list of all the games available.
 * \param p Inherited palette configuration for setting StyleSheets.
 * \param library Pointer to the parent library (used for launching games etc)
 * \param parent Pointer to parent widget.
*/
LibraryCarouselView::LibraryCarouselView(QSettings* p, Library* library, QWidget* parent)
    : QWidget(parent)
{
    this->setObjectName("libraryCarouselUI");
    this->setStyleSheet("QPushButton {"
                        "color: " + p->value("Primary/LightText").toString() + "; "
                        "background-color: " + p->value("Primary/DarkElement").toString() + "; "
                        "border: none; margin: 0px; padding: 0px;} "
                        "QPushButton:hover {"
                        "background-color: " + p->value("Primary/InactiveSelection").toString() + ";} "
                        "QListWidget {"
                        "background-color: " + p->value("Primary/TertiaryBase").toString() + "; "
                        "color: " + p->value("Primary/LightText").toString() + ";}"
                        "QLabel {"
                        "color: " + p->value("Primary/LightText").toString() + ";"
                        "font-family: SourceSansPro; }"
                        "QComboBox::down-arrow { image: url(:/SystemMenu/Icons/DropdownArrow.png); }");

    this->library = library;

    init(p);

    QList<Game> games = Library::db.getGames();

    connect(&Library::db, &Database::dbChanged, this, &LibraryCarouselView::refreshGames);

    launchConnection = connect(launchBtn, &QPushButton::clicked, [=]{});

    refreshGames();
}

void LibraryCarouselView::init(QSettings* p)
{
    QGridLayout* mainGrid = new QGridLayout(this);

    QWidget* contentWidget = new QWidget();
    QVBoxLayout* contentLayout = new QVBoxLayout(contentWidget);
    mainGrid->addWidget(contentWidget, 0, 0);

    QPushButton* addGameBtn = new QPushButton(tr("Add game"));
    addGameBtn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    addGameBtn->setStyleSheet("margin: 11px 0 0 11px; padding: 5px;");
    contentLayout->addWidget(addGameBtn, 0, 0);
    connect(addGameBtn, &QPushButton::clicked, this, &LibraryCarouselView::addGame);

    nameLabel = new QLabel("");
    nameLabel->setStyleSheet("font-size: 20pt;");
    nameLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    contentLayout->addWidget(nameLabel);

    QWidget* hoursLaunchWidget = new QWidget();
    hoursLaunchWidget->setStyleSheet("background-color: " + p->value("Primary/DarkestBase").toString() + ";");
    hoursLaunchWidget->setMinimumWidth(350);
    hoursLaunchWidget->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    contentLayout->addWidget(hoursLaunchWidget);

    QGridLayout* hoursLaunchLayout = new QGridLayout(hoursLaunchWidget);

    QWidget* hoursLastWidget = new QWidget();
    hoursLaunchLayout->addWidget(hoursLastWidget);

    QVBoxLayout* hoursLastLayout = new QVBoxLayout(hoursLastWidget);
    hoursLastLayout->setMargin(0);

    //: 999 refers to the number of hours played. In the future, this will be dynamic.
    hoursPlayedLbl = new QLabel(tr("Hours played: 999"));
    hoursLastLayout->addWidget(hoursPlayedLbl, 0, 0);//todo: not a grid
    //: 1 refers to the number of hours ago the game was last played. In the future, this will be dynamic.
    lastPlayedLbl = new QLabel(tr("Last played: 1 hour ago"));
    hoursLastLayout->addWidget(lastPlayedLbl, 1, 0);//todo: not a grid

    launchBtn = new QPushButton("Launch Game");
    launchBtn->setStyleSheet("background-color: #E82218; padding: 5px;");
    launchBtn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    hoursLaunchLayout->addWidget(launchBtn, 0, 1);

    QWidget* friendsPlayingWidget = new QWidget();
    friendsPlayingWidget->setStyleSheet("background-color: " + p->value("Primary/DarkestBase").toString() + ";");
    friendsPlayingWidget->setMinimumWidth(350);
    friendsPlayingWidget->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    contentLayout->addWidget(friendsPlayingWidget);

    QGridLayout* friendsPlayingLayout = new QGridLayout(friendsPlayingWidget);

    friendsPlayingTitle = new QLabel("Friends playing Grey Goo");
    friendsPlayingLayout->addWidget(friendsPlayingTitle, 0, 0);
    friendsPlayingLayout->addWidget(new QLabel("View all friends"), 0, 1, Qt::AlignRight);
    friendsPlayingLayout->addWidget(new QLabel("None"), 1, 0);

    QTabWidget* gameInfoTabs = new QTabWidget();
    gameInfoTabs->setStyleSheet(
        "QTabWidget::pane {"
            "border: none;"
            "border-top: 2px solid " + p->value("Primary/DarkestBase").toString() +";"
            "left: 2px;"
        "}"
        "QTabBar::tab {"
            "padding: 5px;"
            "border: none;"
            "color: " + p->value("Primary/LightText").toString() + ";"
            "margin: 2px;"
        "}"
        "QTabBar::tab::selected, QTabBar::tab::hover {"
            "background-color: " + p->value("Primary/DarkestBase").toString() + ";"
        "}"
        "QTabBar::tab::!selected{"
            "background-color: " + p->value("Primary/TertiaryBase").toString() + ";"
        "}"
        "QTabWidget {"
            "color: " + p->value("Primary/LightText").toString() + ";"
        "}");
    gameInfoTabs->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    gameInfoTabs->setMinimumWidth(350);
    contentLayout->addWidget(gameInfoTabs);

    QWidget* gameInfoPage = new QWidget();

    QVBoxLayout* gameInfoLayout = new QVBoxLayout(gameInfoPage);

    QLabel* gameInfoText = new QLabel(" Lorem ipsum dolor sit amet, consectetur"
        "adipiscing elit. Donec aliquam nisi ac accumsan pharetra. Nunc luctus "
        "dignissim ipsum, vitae posuere ex. Nunc eget lacus consectetur, blandit"
        " tellus vitae, porta nisl. Vestibulum et lacus bibendum, scelerisque "
        "libero pretium, mattis leo. Donec id egestas libero. Cras eleifend "
        "suscipit nunc, sit amet aliquet ligula facilisis non. Duis at mi at "
        "felis congue fringilla et sit amet eros. Phasellus vestibulum orci sed "
        "nisl consequat, ac suscipit mauris bibendum. Quisque orci ligula, "
        "venenatis et arcu a, euismod mollis ligula. Etiam ac aliquam nisi. "
        "Suspendisse id diam tincidunt, ornare lacus tristique, vehicula ex. "
        "Phasellus volutpat ultrices accumsan. Suspendisse commodo vehicula "
        "leo, at eleifend purus condimentum eget. Integer aliquet metus sit amet"
        " tellus fermentum porta. Suspendisse a risus luctus, pulvinar nulla "
        "vitae, pretium erat. In semper bibendum orci a lobortis. ");
    gameInfoText->setWordWrap(true);
    gameInfoLayout->addWidget(gameInfoText, 0, Qt::AlignTop);

    QWidget* achievementsPage = new QWidget();
    QWidget* screenshotsPage = new QWidget();
    QWidget* modsPage = new QWidget();

    gameInfoTabs->addTab(gameInfoPage, "Game Info");
    gameInfoTabs->addTab(achievementsPage, "Achievements");
    gameInfoTabs->addTab(screenshotsPage, "Screenshots");
    gameInfoTabs->addTab(modsPage, "Mods");

    QWidget* filler = new QWidget();
    contentLayout->addWidget(filler);

    QWidget* gamesWidget = new QWidget();
    gamesLayout = new QVBoxLayout(gamesWidget);
    mainGrid->addWidget(gamesWidget, 0, 1, Qt::AlignRight);
}

void LibraryCarouselView::filterGames(QString searchString)
{
    this->searchString = searchString;
    refreshGames();
}


/** Event handler for adding a game.
 * Prompts the user for various paths, and adds the final game to the database.
*/
void LibraryCarouselView::addGame()
{
    AddGameWizard* wiz = new AddGameWizard();
    wiz->show();
}

/** Recreate the list of games displayed in the main widget.
* Sort the list alphabetically by name
*/
void LibraryCarouselView::refreshGames()
{
    for (int i = 0; i < gamesWidgets.size(); i++)
    {
        QWidget* widget = gamesWidgets[i];
        gamesLayout->removeWidget(widget);
        widget->deleteLater();
    }
    gamesWidgets.clear();

    QList<Game> gameList = Library::db.getGames();
    std::sort(gameList.begin(), gameList.end(),
        [&](const Game& g1, const Game& g2)
        {
            return g1.gameName < g2.gameName;
        }
    );

    int i = 0;
    for (auto game : gameList)
    {
        QString lowerGameName = game.gameName.toLower();
        if (!searchString.isEmpty() && !lowerGameName.contains(searchString.toLower()))
        {
            continue;
        }

        QString displayedName = game.gameName;
        if (displayedName.length() > 20)
        {
            displayedName = displayedName.left(20) + "...";
        }

        if (i == 0)
        {
            nameLabel->setText(game.gameName);
            friendsPlayingTitle->setText("Friends playing " + displayedName);
            disconnect(launchConnection);
            launchConnection = connect(launchBtn, &QPushButton::clicked,
                [=]
                {
                    library->launchGame(game.gameName);
                }
            );
            i = 1;
        }

        GridGameWidget* gameWidget = new GridGameWidget(displayedName, 999);
        gamesLayout->addWidget(gameWidget);
        gamesWidgets.append(gameWidget);

        connect(gameWidget, &GridGameWidget::leftClick,
            [=]
            {
                nameLabel->setText(game.gameName);
                friendsPlayingTitle->setText("Friends playing " + displayedName);
                disconnect(launchConnection);
                launchConnection = connect(launchBtn, &QPushButton::clicked,
                    [=]
                    {
                        library->launchGame(game.gameName);
                    }
                );
            }
        );
    }
}
