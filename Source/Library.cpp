#include "Library.h"
#include "ui_Library.h"
#include "AddGameWizard.h"

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
                        "#content {"
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
                        "#gameHoursLabel, #lastPlayedLabel {"
                        "font-size: 14px;}");

    QFont buttonFont("SourceSansPro", 12);
    ui->addGame->setFont(buttonFont);
    ui->addGame->setText("Add Game");
    ui->removeGame->setFont(buttonFont);
    ui->removeGame->setText("Remove Game");
    ui->testLaunch->setFont(buttonFont);
    ui->testLaunch->setText("Play");

    if (!db.init())
    {
        QMessageBox error;
        error.critical(0, "Error!", "An error occured while trying to load the database.");
        exit(EXIT_FAILURE);
    }

    QList<Game> games = db.getGames();
    for (auto game : games)
    {
        qDebug() << game.id << game.gameName << game.gameDirectory << game.executablePath;
    }

    QFileSystemWatcher* watcher = new QFileSystemWatcher;
    watcher->addPath(QDir(".").filePath("horizon.db"));
    connect(watcher, SIGNAL(fileChanged(QString)), this, SLOT(refreshGames()));

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
        messageBox.setText("Error: an application is already running.");
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
    ui->gameNameLabel->setText(currentText);
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
