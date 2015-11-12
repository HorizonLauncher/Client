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

private slots:
    void addGame();
    void refreshGames();

private:
    Database db;
    GameLauncher gl;

    QGridLayout* gamesLayout;
    QList<QWidget*> gamesWidgets;

    void init();
    void launchGame(QString gameName);
};
