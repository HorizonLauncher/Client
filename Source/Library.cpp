#include "Library.h"
#include "AddGameWizard.h"
#include "Defines.h"
#include "GridGameWidget.h"

#include <QFileDialog>
#include <QInputDialog>
#include <QMessageBox>
#include <QDebug>
#include <QFileSystemWatcher>

Database Library::db(QDir(CONFIG_FOLDER).filePath("horizon.db"));

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
                        "font-family: SourceSansPro; }"
                        "QComboBox::down-arrow { image: url(:/SystemMenu/Icons/DropdownArrow.png); }");

    init(p);

    QList<Game> games = Library::db.getGames();
    for (auto game : games)
    {
        qDebug() << game.id << game.gameName << game.gameDirectory << game.executablePath;
    }

    connect(&db, &Database::dbChanged, this, &Library::refreshGames);

    refreshGames();
}

void Library::init(QSettings* p)
{
    QGridLayout* mainLayout = new QGridLayout(this);
    mainLayout->setMargin(0);

    QWidget* searchBar = new QWidget();
    searchBar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    searchBar->setMinimumHeight(40);
    searchBar->setStyleSheet("background-color:" + p->value("Navbar/SelectedColor").toString() + "; position: absolute;");
    mainLayout->addWidget(searchBar, 0, 0);

    QHBoxLayout* searchLayout = new QHBoxLayout(searchBar);

    searchBox = new QLineEdit();
    searchBox->setPlaceholderText(tr("Search games"));
    searchBox->setStyleSheet("border: none;"
                             "color: " + p->value("Primary/LightText").toString() + ";");
    searchBox->setMinimumWidth(225);
    searchBox->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    searchLayout->addWidget(searchBox);
    connect(searchBox, &QLineEdit::returnPressed, this, &Library::refreshGames);

    QPixmap search(":/SystemMenu/Icons/SearchInverted.png");
    QIcon searchIcon(search);

    QPushButton* searchBtn = new QPushButton("");
    searchBtn->setIcon(searchIcon);
    searchBtn->setIconSize(QSize(16, 16));
    searchBtn->setStyleSheet("background-color: transparent;");
    searchBtn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    searchLayout->addWidget(searchBtn);
    connect(searchBtn, &QPushButton::clicked, this, &Library::refreshGames);

    QWidget* line = new QWidget();
    line->setMinimumSize(1, 2);
    line->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    line->setStyleSheet("border: 1px solid #9e5eee;");
    searchLayout->addWidget(line);

    QFont textFont("Roboto", 11);

    QLabel* filterLabel = new QLabel(tr("Filter by: "));
    filterLabel->setFont(textFont);
    searchLayout->addWidget(filterLabel);

    QComboBox* filterBox = new QComboBox();
    filterBox->addItem(tr("All"));
    searchLayout->addWidget(filterBox);

    QComboBox* actionBox = new QComboBox();
    actionBox->addItem(tr("Action"));
    searchLayout->addWidget(actionBox);

    QComboBox* ratingBox = new QComboBox();
    ratingBox->addItem(tr("Rating"));
    searchLayout->addWidget(ratingBox);

    searchLayout->addStretch();

    QPixmap grid(":/SystemMenu/Icons/GridViewInverted.png");
    QIcon gridIcon(grid);
    QPixmap listImg(":/SystemMenu/Icons/DetailedListViewInverted.png");
    QIcon listIcon(listImg);
    QPixmap carousel(":/SystemMenu/Icons/CarouselViewInverted.png");
    QIcon carouselIcon(carousel);

    QPushButton* gridBtn = new QPushButton("");
    gridBtn->setIcon(gridIcon);
    gridBtn->setIconSize(QSize(16, 16));
    gridBtn->setStyleSheet("background-color: transparent;");
    gridBtn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    searchLayout->addWidget(gridBtn);

    QPushButton* listBtn = new QPushButton("");
    listBtn->setIcon(listIcon);
    listBtn->setIconSize(QSize(16, 16));
    listBtn->setStyleSheet("background-color: transparent;");
    listBtn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    searchLayout->addWidget(listBtn);

    QPushButton* carouselBtn = new QPushButton("");
    carouselBtn->setIcon(carouselIcon);
    carouselBtn->setIconSize(QSize(16, 16));
    carouselBtn->setStyleSheet("background-color: transparent;");
    carouselBtn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    searchLayout->addWidget(carouselBtn);

    QPushButton* addGameBtn = new QPushButton(tr("Add game"));
    addGameBtn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    addGameBtn->setStyleSheet("margin: 11px 0 0 11px; padding: 5px;");
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
        Game game = Library::db.getGameByName(gameName);
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
        QString searchString = searchBox->text().trimmed();
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
        connect(gameWidget, &GridGameWidget::clicked, [=] { launchGame(game.gameName); });
        gamesWidgets.append(gameWidget);

        if (col == 3)
        {
            row++;
        }
        col = (col < 3 ? col + 1 : 0);
    }
}
