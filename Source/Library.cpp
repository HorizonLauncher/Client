#include "Library.h"
#include "AddGameWizard.h"
#include "Defines.h"
#include "GridGameWidget.h"

#include <QFileDialog>
#include <QInputDialog>
#include <QMessageBox>
#include <QDebug>
#include <QFileSystemWatcher>

/** Library constructor
 * Initialize the library UI and generate an initial list of all the games available.
 * \param p Inherited palette configuration for setting StyleSheets.
 * \param parent Pointer to parent widget.
*/
Library::Library(QSettings* p, QWidget* parent)
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
                        "font-family: SourceSansPro;"
                        "}");

    init(p);

    if (!db.init())
    {
        QMessageBox error;
        error.critical(0, tr("Error!"), tr("An error occurred while trying to load the database."));
        exit(EXIT_FAILURE);
    }

    QList<Game> games = db.getGames();
    for (auto game : games)
    {
        qDebug() << game.id << game.gameName << game.gameDirectory << game.executablePath;
    }

    QFileSystemWatcher* watcher = new QFileSystemWatcher;
    watcher->addPath(QDir(CONFIG_FOLDER).filePath("horizon.db"));
    connect(watcher, &QFileSystemWatcher::fileChanged, this, &Library::refreshGames);

    refreshGames();
}

void Library::init(QSettings* p)
{
    QGridLayout* mainLayout = new QGridLayout(this);
    mainLayout->setMargin(0);

    QWidget* searchBar = new QWidget();
    searchBar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    searchBar->setMinimumHeight(40);
    searchBar->setStyleSheet("background-color:" + p->value("Navbar/SelectedColor").toString() + ";");
    mainLayout->addWidget(searchBar, 0, 0);

    QHBoxLayout* searchLayout = new QHBoxLayout(searchBar);

    QLineEdit* searchBox = new QLineEdit();
    searchBox->setPlaceholderText("Search games");
    searchBox->setStyleSheet("border: none;"
                             "color: " + p->value("Primary/LightText").toString() + ";");
    searchBox->setMinimumWidth(225);
    searchBox->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    searchLayout->addWidget(searchBox);

    QPixmap search(":/SystemMenu/Icons/SearchInverted.png");
    QIcon searchIcon(search);

    QPushButton* searchBtn = new QPushButton("");
    searchBtn->setIcon(searchIcon);
    searchBtn->setIconSize(QSize(16, 16));
    searchBtn->setStyleSheet("background-color: transparent;");
    searchBtn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    searchLayout->addWidget(searchBtn);
    searchLayout->addStretch();

    QPushButton* addGameBtn = new QPushButton("Add game");
    addGameBtn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    addGameBtn->setStyleSheet("margin: 11px 0 0 11px;");
    mainLayout->addWidget(addGameBtn, 1, 0);
    connect(addGameBtn, &QPushButton::clicked, this, &Library::addGame);

    QWidget* gamesWidget = new QWidget();
    mainLayout->addWidget(gamesWidget, 2, 0);

    gamesLayout = new QGridLayout();
    gamesLayout->setSpacing(12);
    gamesWidget->setLayout(gamesLayout);
}

/** Function to launch a game.
 * Populates a message box on failure, or runs the games upon success.
 * \param gameName The name of the game to launch
*/
void Library::launchGame(QString gameName)
{
    if (!gl.isProcessRunning())
    {
        Game game = db.getGameByName(gameName);
        if (game.arguments.trimmed() == "")
        {
            gl.runProcess(game.executablePath, game.gameDirectory);
        }
        else
        {
            gl.runProcessWithArgs(game.executablePath, game.gameDirectory, game.arguments);
        }
    }
    else
    {
        QMessageBox messageBox;
        messageBox.setText(tr("Error: an application is already running."));
        messageBox.exec();
    }
}

/** Event handler for adding a game.
 * Prompts the user for various paths, and adds the final game to the database.
*/
void Library::addGame()
{
    AddGameWizard* wiz = new AddGameWizard();
    wiz->show();
}

/** Recreate the list of games displayed in the main widget.
* Sort the list alphabetically by name
*/
void Library::refreshGames()
{
    for (auto gameWidget : gamesWidgets)
    {
        gamesLayout->removeWidget(gameWidget);
        gameWidget->deleteLater();
    }

    QList<Game> gameList = db.getGames();
    std::sort(gameList.begin(), gameList.end(), [&](const Game& g1, const Game& g2){return g1.gameName < g2.gameName; });
    int row = 0, col = 0;
    for (auto game : gameList)
    {
        GridGameWidget* gameWidget = new GridGameWidget(game.gameName, 999);
        gamesLayout->addWidget(gameWidget, row, col);
        connect(gameWidget, &GridGameWidget::clicked, [=] { launchGame(game.gameName); });
        gamesWidgets.append(gameWidget);

        if (col == 3)
        {
            row++;
        }
        col = (col < 3 ? col + 1 : 0);
    }
}
