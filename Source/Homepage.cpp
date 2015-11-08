#include "Homepage.h"
#include "ui_homepage.h"

Homepage::Homepage(QSettings* p, QWidget* parent) :
    QWidget(parent),
    ui(new Ui::Homepage)
{
    ui->setupUi(this);
    this->setObjectName("homepageUI");
    this->setStyleSheet("QPushButton {"
                        "font-family: SourceSansPro;"
                        "background-color: " + p->value("Primary/DarkElement").toString() + ";"
                        "color: " + p->value("Primary/LightText").toString() + ";"
                        "border: none; margin: 0px; padding: 0px;}"
                        "QPushButton:hover {"
                        "background-color: " + p->value("Primary/InactiveSelection").toString() + ";} "
                        "QLabel {"
                        "font-family: SourceSansPro;"
                        "color: " + p->value("Primary/LightText").toString() + " }"
                        "#playRandom { font-size: 12px; }"
                        "#randomGameLbl { font-size: 16px; }"
                        "#newRandom { background-color: transparent; }");

    QPixmap refresh(":/SystemMenu/Icons/RefreshInverted.png");
    QIcon icon(refresh);
    ui->newRandom->setIcon(icon);
    ui->newRandom->setIconSize(QSize(18, 18));

    if (!db.init())
    {
        QMessageBox error;
        error.critical(0, tr("Error!"), tr("An error occured while trying to load the database."));
        exit(EXIT_FAILURE);
    }

    selectRandomGame();
}

void Homepage::on_newRandom_clicked()
{
    selectRandomGame();
}

void Homepage::on_playRandom_clicked()
{
    if (!gl.isProcessRunning())
    {
        if(noGames)
        {
            return;
        }

        Game game = db.getGameByName(curRandom);
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
    QList<Game> games = db.getGames();
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

    ui->randomGameLbl->setText(tr("Random Game: ") + curRandom);
    ui->randomGameLbl->adjustSize();
}

Homepage::~Homepage()
{
    delete ui;
}
