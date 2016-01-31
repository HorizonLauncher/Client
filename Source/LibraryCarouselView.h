#ifndef _LIBRARYCAROUSELVIEW_H_
#define _LIBRARYCAROUSELVIEW_H_

class Library;

#include "Library.h"

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
    void init(QSettings* p);

    QGridLayout* gamesLayout;
    QList<QWidget*> gamesWidgets;
    Library* library;

    QString searchString;
};

#endif
