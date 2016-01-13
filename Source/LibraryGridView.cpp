#include "LibraryGridView.h"

#include "GridGameWidget.h"
#include "AddGameWizard.h"

/** LibraryGridView constructor
 * Initialize the grid UI and generate an initial list of all the games available.
 * \param p Inherited palette configuration for setting StyleSheets.
 * \param parent Pointer to parent widget.
*/
LibraryGridView::LibraryGridView(QSettings* p, Library* library, QWidget* parent)
    : QWidget(parent)
{
    this->setObjectName("libraryUI");
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
    for (auto game : games)
    {
        qDebug() << game.id << game.gameName << game.gameDirectory << game.executablePath;
    }

    connect(&Library::db, &Database::dbChanged, this, &LibraryGridView::refreshGames);

    refreshGames();
}

void LibraryGridView::init(QSettings* p)
{
    QGridLayout* mainLayout = new QGridLayout(this);
    mainLayout->setMargin(0);

    QPushButton* addGameBtn = new QPushButton(tr("Add game"));
    addGameBtn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    addGameBtn->setStyleSheet("margin: 11px 0 0 11px; padding: 5px;");
    mainLayout->addWidget(addGameBtn, 0, 0);
    connect(addGameBtn, &QPushButton::clicked, this, &LibraryGridView::addGame);

    QWidget* gamesWidget = new QWidget();
    mainLayout->addWidget(gamesWidget, 1, 0);

    gamesLayout = new QGridLayout(gamesWidget);
    gamesLayout->setSpacing(12);
}

/** Recreate the list of games displayed in the main widget.
* Sort the list alphabetically by name
*/
void LibraryGridView::refreshGames()
{
    for (int i = 0; i < gamesWidgets.size(); i++)
    {
        QWidget* widget = gamesWidgets[i];
        gamesLayout->removeWidget(widget);
        widget->deleteLater();
    }
    gamesWidgets.clear();

    QList<Game> gameList = Library::db.getGames();
    std::sort(gameList.begin(), gameList.end(), [&](const Game& g1, const Game& g2){return g1.gameName < g2.gameName; });
    int row = 0, col = 0;
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

        GridGameWidget* gameWidget = new GridGameWidget(displayedName, 999);
        gamesLayout->addWidget(gameWidget, row, col);
        connect(gameWidget, &GridGameWidget::leftClick, [=] { library->launchGame(game.gameName); });
        connect(gameWidget, &GridGameWidget::changeLaunchOpts, [=]{ library->changeLaunchOpts(game.gameName); });
        connect(gameWidget, &GridGameWidget::removeGame, [=]{ Library::db.removeGameByName(game.gameName); });
        gamesWidgets.append(gameWidget);

        if (col == 3)
        {
            row++;
        }
        col = (col < 3 ? col + 1 : 0);
    }
}

void LibraryGridView::filterGames(QString searchString)
{
    this->searchString = searchString;
}


/** Event handler for adding a game.
 * Prompts the user for various paths, and adds the final game to the database.
*/
void LibraryGridView::addGame()
{
    AddGameWizard* wiz = new AddGameWizard();
    wiz->show();
}
