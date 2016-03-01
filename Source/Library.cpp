#include "Library.h"
#include "Defines.h"

#include <QFileDialog>
#include <QInputDialog>
#include <QMessageBox>
#include <QDebug>
#include <QFileSystemWatcher>
#include <QScrollArea>

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
}

void Library::init(QSettings* p)
{
    mainLayout = new QGridLayout(this);
    mainLayout->setMargin(0);

    QWidget* searchBar = new QWidget();
    searchBar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    searchBar->setMinimumHeight(40);
    searchBar->setStyleSheet("background-color:" + p->value("Navbar/SelectedColor").toString() + ";");
    mainLayout->addWidget(searchBar, 0, 0);

    QHBoxLayout* searchLayout = new QHBoxLayout(searchBar);

    searchBox = new QLineEdit();
    searchBox->setPlaceholderText(tr("Search games"));
    searchBox->setStyleSheet("border: none;"
                             "color: " + p->value("Primary/LightText").toString() + ";");
    searchBox->setMinimumWidth(225);
    searchBox->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    searchLayout->addWidget(searchBox);
    connect(searchBox, &QLineEdit::returnPressed, [=]
    {
        gridView->filterGames(searchBox->text().trimmed());
    });

    QPixmap search(":/SystemMenu/Icons/SearchInverted.png");
    QIcon searchIcon(search);

    QPushButton* searchBtn = new QPushButton("");
    searchBtn->setIcon(searchIcon);
    searchBtn->setIconSize(QSize(16, 16));
    searchBtn->setStyleSheet("background-color: transparent;");
    searchBtn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    searchLayout->addWidget(searchBtn);
    connect(searchBtn, &QPushButton::clicked, [=]
    {
        gridView->filterGames(searchBox->text().trimmed());
    });

    QColor* selectedColor = new QColor(p->value("Navbar/SelectedColor").toString());
    QColor lineColor = selectedColor->lighter(120); //#9e5eee

    QWidget* line = new QWidget();
    line->setMinimumSize(1, 2);
    line->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    line->setStyleSheet("border: 1px solid " + lineColor.name() + ";");
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
    connect(gridBtn, &QPushButton::clicked, this, &Library::setGridView);

    QPushButton* listBtn = new QPushButton("");
    listBtn->setIcon(listIcon);
    listBtn->setIconSize(QSize(16, 16));
    listBtn->setStyleSheet("background-color: transparent;");
    listBtn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    searchLayout->addWidget(listBtn);
    connect(listBtn, &QPushButton::clicked, this, &Library::setDetailView);

    QPushButton* carouselBtn = new QPushButton("");
    carouselBtn->setIcon(carouselIcon);
    carouselBtn->setIconSize(QSize(16, 16));
    carouselBtn->setStyleSheet("background-color: transparent;");
    carouselBtn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    searchLayout->addWidget(carouselBtn);
    connect(carouselBtn, &QPushButton::clicked, this, &Library::setCarouselView);

    gridView = new LibraryGridView(p, this);
    detailView = new LibraryDetailView(p, this);
    detailView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    carouselView = new LibraryCarouselView(p, this);

    viewStack = new QStackedWidget(this);

    QScrollArea* gridScrollArea = new QScrollArea(this);
    gridScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    gridScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    gridScrollArea->setFrameShape(QFrame::NoFrame);
    gridScrollArea->setWidgetResizable(true);
    gridScrollArea->setWidget(gridView);
    gridScrollArea->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    viewStack->addWidget(gridScrollArea);

    QScrollArea* detailScrollArea = new QScrollArea(this);
    detailScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    detailScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    detailScrollArea->setFrameShape(QFrame::NoFrame);
    detailScrollArea->setWidgetResizable(true);
    detailScrollArea->setWidget(detailView);
    detailScrollArea->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    viewStack->addWidget(detailScrollArea);

    QScrollArea* carouselScrollArea = new QScrollArea(this);
    carouselScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    carouselScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    carouselScrollArea->setFrameShape(QFrame::NoFrame);
    carouselScrollArea->setWidgetResizable(true);
    carouselScrollArea->setWidget(carouselView);
    carouselScrollArea->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    viewStack->addWidget(carouselScrollArea);

    mainLayout->addWidget(viewStack, 2, 0);
}

/** Function to launch a game.
 * Populates a message box on failure, or runs the games upon success.
 * \param gameName The name of the game to launch
*/
void Library::launchGame(QString gameName)
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

void Library::changeLaunchOpts(QString gameName)
{
    Game game = Library::db.getGameByName(gameName);
    bool ok;

    QString newLaunchOpts = QInputDialog::getText(this,
                                tr("Change Launch Options"),
                                tr("Launch Options: "),
                                QLineEdit::Normal,
                                game.arguments,
                                &ok);

    if (ok)
    {
        Library::db.setLaunchOptionsByName(gameName, newLaunchOpts);
    }
}


/**
  Sets the current games layout to the grid view
 */
void Library::setGridView()
{
    viewStack->setCurrentIndex(0);
}

/**
  Sets the current games layout to the list (detail) view
 */
void Library::setDetailView()
{
    viewStack->setCurrentIndex(1);
}

/**
  Sets the current games layout to the carousel view
 */
void Library::setCarouselView()
{
    viewStack->setCurrentIndex(2);
}
