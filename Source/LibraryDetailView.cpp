#include "LibraryDetailView.h"
#include "Database.h"
#include "Library.h"
#include <QPixmap>
#include <QScrollArea>
#include "DetailGameWidget.h"

LibraryDetailView::LibraryDetailView(QSettings* p, QWidget *parent) : QWidget(parent)
{

    this->mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(10);
    this->palette = p;

    connect(&Library::db, &Database::dbChanged, this, &LibraryDetailView::refreshGames);

    refreshGames();
}

void LibraryDetailView::filterGames(QString searchString)
{
    this->searchString = searchString;
    refreshGames();
}

void LibraryDetailView::refreshGames()
{
    for (int i = 0; i < gamesWidgets.size(); i++)
    {
        QWidget* widget = gamesWidgets[i];
        mainLayout->removeWidget(widget);
        widget->deleteLater();
    }
    gamesWidgets.clear();

    QList<Game> games = Library::db.getGames();
    for (auto game : games)
    {
        QString lowerGameName = game.gameName.toLower();
        if (!searchString.isEmpty() && !lowerGameName.contains(searchString.toLower()))
        {
            continue;
        }

        DetailGameWidget* gameWidget = new DetailGameWidget(game, this->palette, this);
        gameWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        mainLayout->addWidget(gameWidget);
        gamesWidgets.append(gameWidget);
    }

    this->setLayout(mainLayout);
}
