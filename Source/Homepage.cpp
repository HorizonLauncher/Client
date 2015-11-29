#include "Homepage.h"
#include "Library.h"

Homepage::Homepage(QSettings* p, QWidget* parent) :
    QWidget(parent)
{
    this->setObjectName("homepage");
    this->setStyleSheet("QPushButton {"
                        "font-family: SourceSansPro;"
                        "background-color: " + p->value("Primary/DarkElement").toString() + ";"
                        "color: " + p->value("Primary/LightText").toString() + ";"
                        "border: none; margin: 0px; padding: 0px;}"
                        "QPushButton:hover {"
                        "background-color: " + p->value("Primary/InactiveSelection").toString() + ";} "
                        "QLabel {"
                        "font-family: SourceSansPro;"
                        "color: " + p->value("Primary/LightText").toString() + " }");

    init();

    selectRandomGame();
}

void Homepage::init()
{
    QGridLayout* mainLayout = new QGridLayout(this);

    randomGameLbl = new QLabel(tr("Random game: "));
    randomGameLbl->setStyleSheet("font-size: 16px;");
    mainLayout->addWidget(randomGameLbl, 0, 0, 1, 1, Qt::AlignBottom);

    playRandom = new QPushButton(tr("PLAY"));
    playRandom->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    playRandom->setStyleSheet("font-size: 12px; padding: 5px;");
    mainLayout->addWidget(playRandom, 0, 1, 1, 1, Qt::AlignBottom);

    QPixmap refresh(":/SystemMenu/Icons/RefreshInverted.png");
    QIcon icon(refresh);

    newRandom = new QPushButton("");
    newRandom->setIcon(icon);
    newRandom->setIconSize(QSize(18, 18));
    newRandom->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    newRandom->setStyleSheet("background-color: transparent;");
    mainLayout->addWidget(newRandom, 0, 2, 1, 1, Qt::AlignBottom | Qt::AlignLeft);

    connect(newRandom, &QPushButton::clicked, [=] { selectRandomGame(); });
    connect(playRandom, &QPushButton::clicked, this, &Homepage::playRandomGame);

    selectRandomGame();
}

void Homepage::playRandomGame()
{
    if (!gl.isProcessRunning())
    {
        if(noGames)
        {
            return;
        }

        Game game = Library::db.getGameByName(curRandom);
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

void Homepage::selectRandomGame()
{
    QList<Game> games = Library::db.getGames();
    if(games.length() != 0)
    {
        Game game = games[rand() % games.length()];
        curRandom = game.gameName;
        noGames = false;
    }
    else
    {
        noGames = true;
        curRandom = tr("No games!");
    }

    randomGameLbl->setText(tr("Random Game: ") + curRandom);
    randomGameLbl->adjustSize();
}
