#pragma once

#include <QSqlQuery>
#include <QtWidgets>

/** Game type.
 * Defines a game type.
 * Members correspond to a field within the database.
 */
typedef struct
{
    unsigned int id;  /**< DB ID of the game. */
    QString gameName; /**< Name of the game to display */
    QString gameDirectory; /**< Working directory of the game */
    QString executablePath; /**< Path to the executable */
    QString arguments; /**< Arguments to pass to the executable */
    int drm; /**< DRM the game came from, where 0 = None, 1 = Steam, 2 = Origin, 3 = uPlay */
} Game;

typedef std::vector<Game> GameList;

/** Database class.
 * Wrapper class to manage the internal game database.
 * Also has a simple interface to find games by certain conditions.
 */
class Database
{
public:
    Database();
    Database(QString name);
    bool init();
    bool reset();

    bool addGame(QString gameName, QString gameDirectory, QString executablePath, QString arguments, int drm = 0);
    void addGames(GameList games);
    bool removeGameById(unsigned int id);
    bool removeGameByName(QString name);
    Game getGameById(unsigned int id);
    Game getGameByName(QString name);
    std::pair<bool, Game> isExistant(unsigned int id);
    std::pair<bool, Game> isExistant(QString name);
    QList<Game> getGames();
    unsigned int getGameCount() const;

private:
    QSqlDatabase db;
};
