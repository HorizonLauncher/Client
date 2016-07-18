#ifndef LIBRARY_H
#define LIBRARY_H

class LibraryGridView;
class LibraryCarouselView;

#include "../../database.h"
#include "../../gamelauncher.h"
#include "../buddylist.h"
#include "../views/librarygridview.h"
#include "../views/librarydetailview.h"
#include "../views/librarycarouselview.h"

#include <QWidget>
#include <QProcess>
#include <QSettings>
#include <QDir>
#include <QGridLayout>
#include <QList>
#include <QStackedWidget>

/** Library class.
 * Class to handle the library section of the launcher
*/
class Library : public QWidget
{
    Q_OBJECT

public:
    Library(QSettings* p, QWidget* parent = 0);

    void launchGame(QString gameName);

    static Database db;

public slots:
    void setGridView();
    void setDetailView();
    void setCarouselView();

private:
    GameLauncher gl;
    QGridLayout* mainLayout;
    LibraryGridView* gridView;
    LibraryDetailView* detailView;
    LibraryCarouselView* carouselView;
    QLineEdit* searchBox;
    QStackedWidget* viewStack;

    int currentView;

    void init(QSettings* p);
};

#endif
