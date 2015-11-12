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

    init();

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

void Library::init()
{
    QGridLayout* mainLayout = new QGridLayout(this);

    QPushButton* addGameBtn = new QPushButton("Add game");
    addGameBtn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    mainLayout->addWidget(addGameBtn, 0, 0);
    connect(addGameBtn, &QPushButton::clicked, this, &Library::addGame);

    QWidget* gamesWidget = new QWidget();
    mainLayout->addWidget(gamesWidget, 1, 0, 1, 2);

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
