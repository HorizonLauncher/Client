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
    refreshGames();
}

void LibraryDetailView::refreshGames() {
    QList<Game> games = Library::db.getGames();
    for (auto game : games) {
        DetailGameWidget* gameWidget = new DetailGameWidget(game, this->palette, this);
        mainLayout->addWidget(gameWidget);
    }

    this->setLayout(mainLayout);
}
