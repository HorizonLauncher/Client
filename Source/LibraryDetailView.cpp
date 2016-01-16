#include "LibraryDetailView.h"
#include "Database.h"
#include "Library.h"
#include "DetailGameWidget.h"

LibraryDetailView::LibraryDetailView(QWidget *parent) : QWidget(parent)
{
    this->mainLayout = new QVBoxLayout();
    refreshGames();
}

void LibraryDetailView::refreshGames() {
    QList<Game> games = Library::db.getGames();

    for (auto game : games) {
        DetailGameWidget* gameWidget = new DetailGameWidget(game, this);
        mainLayout->addWidget(gameWidget);
    }

     this->setLayout(mainLayout);

}
