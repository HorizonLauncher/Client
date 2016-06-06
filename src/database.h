#ifndef DATABASE_H
#define DATABASE_H

#include <QSqlQuery>
#include <QtWidgets>
#include <QObject>

/** Game type.
 * Defines a game type.
 * Members correspond to a field within the database.
 */
struct Game
{
    unsigned int id;  /**< DB ID of the game. */
    QString gameName; /**< Name of the game to display */
    QString gameDirectory; /**< Working directory of the game */
    QString executablePath; /**< Path to the executable */
    QString arguments; /**< Arguments to pass to the executable */
    int drm; /**< DRM the game came from, where 0 = None, 1 = Steam, 2 = Origin, 3 = uPlay */
    QString developer;
    QString publisher;
    QString releaseDate;
    QString genre;
    QString bannerPath;
};

typedef std::vector<Game> GameList;

/** Database class.
 * Wrapper class to manage the internal game database.
 * Also has a simple interface to find games by certain conditions.
 */
class Database : public QObject
{
    Q_OBJECT

public:
    Database(QObject* parent = 0);
    Database(QString name, QObject* parent = 0);

    bool init();
    bool reset();

    bool addGame(Game game);
    void addGames(GameList games);

    bool removeGameById(unsigned int id);
    bool removeGameByName(QString name);

    Game getGameById(unsigned int id);
    Game getGameByName(QString name);

    std::pair<bool, Game> isExistant(unsigned int id);
    std::pair<bool, Game> isExistant(QString name);

    QList<Game> getGames();
    unsigned int getGameCount() const;

    bool setLaunchOptionsById(unsigned int id, QString launchOpts);
    bool setLaunchOptionsByName(QString name, QString launchOpts);

    bool updateGame(Game game);

    static QString getDRMString(int drmID);

signals:
    void dbChanged();

private:
    QSqlDatabase db;

    QString path;
};

#endif
