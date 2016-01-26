#include "LibraryDetailView.h"
#include "Database.h"
#include "Library.h"
#include <QPixmap>
#include "DetailGameWidget.h"
LibraryDetailView::LibraryDetailView(QSettings* p, QWidget *parent) : QWidget(parent)
{
    this->mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(1);
    this->palette = p;
    refreshGames();
}

void LibraryDetailView::refreshGames() {
    QList<Game> games = Library::db.getGames();

    for (int i = 0; i < 5; ++i) {
        Game game = games.at(i);
        DetailGameWidget* gameWidget = new DetailGameWidget(game, this->palette, this);

        mainLayout->addWidget(gameWidget);


     }

     this->setLayout(mainLayout);

}
