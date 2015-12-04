#pragma once

#include "Database.h"
#include "GameLauncher.h"
#include "BuddyList.h"

#include <QWidget>
#include <QProcess>
#include <QSettings>
#include <QDir>
#include <QGridLayout>
#include <QList>

/** Library class.
 * Class to handle the library section of the launcher
*/
class Library : public QWidget
{
    Q_OBJECT

public:
    Library(QSettings* p, QWidget* parent = 0);

    static Database db;

private slots:
    void addGame();
    void refreshGames();

private:
    GameLauncher gl;

    QGridLayout* gamesLayout;
    QList<QWidget*> gamesWidgets;
    QLineEdit* searchBox;

    void init(QSettings* p);
    void launchGame(QString gameName);
    void changeLaunchOpts(QString gameName);
};
