#ifndef LIBRARYGRIDVIEW_H
#define LIBRARYGRIDVIEW_H

class Library;

#include "../stack/library.h"

#include <QWidget>
#include <QGridLayout>

/** LibraryGridView class
 * Class for the library "Grid view"
 */
class LibraryGridView : public QWidget
{
    Q_OBJECT

public:
    LibraryGridView(QSettings* p, Library* library, QWidget* parent = 0);

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
