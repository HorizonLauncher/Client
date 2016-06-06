#ifndef HOMEPAGE_H
#define HOMEPAGE_H

#include "database.h"
#include "../../gamelauncher.h"

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
