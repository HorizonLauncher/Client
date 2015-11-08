#include "Library.h"
#include "ui_Library.h"
#include "AddGameWizard.h"
#include "Defines.h"

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
    : QWidget(parent),
      ui(new Ui::Library)
{
    ui->setupUi(this);
    this->setObjectName("libraryUI");
    this->setStyleSheet("#leftSidebar {"
                        "background-color: " + p->value("Primary/SecondaryBase").toString() + ";} "
                        "QPushButton {"
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
                        "}"
                        "#gameNameLabel { font-size: 20px; }"
                        "#gameHoursLabel, #lastPlayedLabel, #gameDRMLabel {"
                        "font-size: 14px; }");

    QFont buttonFont("SourceSansPro", 12);
    ui->addGame->setFont(buttonFont);
    ui->addGame->setText(tr("Add Game"));
    ui->removeGame->setFont(buttonFont);
    ui->removeGame->setText(tr("Remove Game"));
    ui->testLaunch->setFont(buttonFont);
    ui->testLaunch->setText(tr("Play"));

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


Library::~Library()
{
    delete ui;
}

/** Event handler for launching a game.
 * Populates a message box on failure, or runs the games upon success.
*/
void Library::on_testLaunch_clicked()
{
    if (!gl.isProcessRunning())
    {
        auto selection = ui->gameListWidget->currentItem();
        if (selection != nullptr)
        {
            Game game = db.getGameByName(selection->text());
            if (game.arguments.trimmed() == "")
            {
                gl.runProcess(game.executablePath, game.gameDirectory);
            }
            else
            {
                gl.runProcessWithArgs(game.executablePath, game.gameDirectory, game.arguments);
            }
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
void Library::on_addGame_clicked()
{
    AddGameWizard* wiz = new AddGameWizard();
    wiz->show();
}

/** Event handler for removing a game.
*/
void Library::on_removeGame_clicked()
{
    auto selection = ui->gameListWidget->currentItem();
    if (selection != nullptr)
    {
        db.removeGameByName(selection->text());
        refreshGames();
    }
}
/** Event handler for when a game is selected.
 */
void Library::on_gameListWidget_currentTextChanged(const QString & currentText)
{
    Game game = db.getGameByName(currentText);
    QString drmString = "";
    if (game.drm == 0)
    {
        drmString = tr("None");
    }
    else if (game.drm == 1)
    {
        drmString = tr("Steam");
    }
    else if (game.drm == 2)
    {
        drmString = tr("Origin");
    }
    else if (game.drm == 3)
    {
        drmString = tr("uPlay");
    }

    ui->gameNameLabel->setText(currentText);
    ui->gameDRMLabel->setText(drmString);
}

/** Recreate the list of games displayed in the main widget.
* Sort the list alphabetically by name
*/
void Library::refreshGames()
{
    ui->gameListWidget->clear();
    QList<Game> gameList = db.getGames();
	std::sort(gameList.begin(), gameList.end(), [&](const Game& g1, const Game& g2){return g1.gameName < g2.gameName; });
    for (auto game : gameList)
    {
        ui->gameListWidget->addItem(game.gameName);
    }
}
