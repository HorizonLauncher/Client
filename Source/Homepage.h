#ifndef HOMEPAGE_H
#define HOMEPAGE_H

#include "Database.h"
#include "GameLauncher.h"

#include <QWidget>
#include <QSettings>

class Homepage : public QWidget
{
    Q_OBJECT

public:
    Homepage(QSettings* p, QWidget* parent = 0);

private slots:
    void playRandomGame();

private:
    Database db;
    GameLauncher gl;
    QString curRandom;
    bool noGames;

    QLabel *randomGameLbl;
    QPushButton* playRandom;
    QPushButton* newRandom;

    void init();
    void selectRandomGame();
};

#endif // HOMEPAGE_H
