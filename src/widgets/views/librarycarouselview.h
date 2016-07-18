#ifndef LIBRARYCAROUSELVIEW_H
#define LIBRARYCAROUSELVIEW_H

class Library;

#include "../stack/library.h"

#include <QWidget>
#include <QGridLayout>

/** LibraryCarouselView class
 * Class for the library "carousel view"
 */
class LibraryCarouselView : public QWidget
{
    Q_OBJECT

public:
    LibraryCarouselView(QSettings* p, Library* library, QWidget* parent = 0);

    void filterGames(QString searchString);

private slots:
    void refreshGames();
    void addGame();

private:
    void init();

    QSettings* palette;

    QVBoxLayout* gamesLayout;
    QList<QWidget*> gamesWidgets;
    Library* library;

    QLabel* nameLabel;
    QLabel* hoursPlayedLbl;
    QLabel* lastPlayedLbl;
    QPushButton* launchBtn;
    QMetaObject::Connection launchConnection;
    QLabel* friendsPlayingTitle;
    QString searchString;
};

#endif
