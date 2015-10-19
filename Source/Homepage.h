#ifndef HOMEPAGE_H
#define HOMEPAGE_H

#include "Database.h"
#include "GameLauncher.h"

#include <QWidget>
#include <QSettings>

namespace Ui {
class Homepage;
}

class Homepage : public QWidget
{
    Q_OBJECT

public:
    Homepage(QSettings* p, QWidget* parent = 0);
    ~Homepage();

private slots:
    void on_newRandom_clicked();
    void on_playRandom_clicked();

private:
    Database db;
    Ui::Homepage* ui;
    GameLauncher gl;
    QString curRandom;

    void selectRandomGame();
};

#endif // HOMEPAGE_H
