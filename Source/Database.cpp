#include "Database.h"
#include "Defines.h"

/** Database constructor
 * Constructs the local database.
 * Currently no interface to handle remote databases, just creates one in the
 * current working directory.
 */
Database::Database(QObject* parent)
{
    this->path = QDir(CONFIG_FOLDER).filePath("horizon.db");
}

Database::Database(QString path, QObject* parent)
{
    this->path = path;
}

/** Initialize the actual database, if it hasn't been done already.
 * \return Success/failure of the operation.
*/
bool Database::init()
{
    db = QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE"));
    db.setHostName("localhost");
    db.setDatabaseName(this->path);

    bool status = db.open();
    if (!status)
    {
        QMessageBox(QMessageBox::Critical, tr("Database Error"),
                                           tr("Could not connect to the database!")).exec();
        return false;
    }

    QSqlQuery existsQuery(db);
    existsQuery.exec("SELECT name FROM sqlite_master WHERE type=\"table\" AND name=\"games\";");

    bool rtn = true;

    if(!existsQuery.next())
    {
        QSqlQuery createQuery(db);
        rtn = createQuery.exec("CREATE TABLE IF NOT EXISTS games ("
                               "ID INTEGER PRIMARY KEY ASC, "
                               "GAMENAME TEXT NOT NULL, "
                               "GAMEDIRECTORY TEXT NOT NULL, "
                               "GAMEEXECUTABLE TEXT NOT NULL, "
                               "ARGUMENTS TEXT NOT NULL, "
                               "DRM INT DEFAULT 0,"
                               "DEVELOPER TEXT,"
                               "PUBLISHER TEXT,"
                               "RELEASEDATE TEXT,"
                               "GENRE TEXT,"
                               "BANNERPATH TEXT);");

        createQuery.exec("PRAGMA user_version = 1;");

        if (rtn)
        {
            emit dbChanged();
        }
    }

    QSqlQuery query(db);
    query.exec("PRAGMA user_version;");
    query.next();
    int userVersion = query.value(0).toInt();

    if (userVersion == 0)
    {
        query.exec("ALTER TABLE games ADD COLUMN DEVELOPER TEXT;");
        query.exec("ALTER TABLE games ADD COLUMN PUBLISHER TEXT;");
        query.exec("ALTER TABLE games ADD COLUMN RELEASEDATE TEXT;");
        query.exec("ALTER TABLE games ADD COLUMN GENRE TEXT;");
        query.exec("ALTER TABLE games ADD COLUMN BANNERPATH TEXT;");
        query.exec("PRAGMA user_version = 1;");
    }

    return rtn;
}

/** Remove every table in the database.
 * \return Success/failure of the operation.
*/
bool Database::reset()
{
    QSqlQuery query(db);
    bool rtn = query.exec("DROP TABLE IF EXISTS games") &&
               query.exec("CREATE TABLE IF NOT EXISTS games("
                          "ID INTEGER PRIMARY KEY ASC,"
                          "GAMENAME TEXT NOT NULL,"
                          "GAMEDIRECTORY TEXT NOT NULL,"
                          "GAMEEXECUTABLE TEXT NOT NULL,"
                          "ARGUMENTS TEXT NOT NULL,"
                          "DRM INT DEFAULT 0,"
                          "DEVELOPER TEXT,"
                          "PUBLISHER TEXT,"
                          "RELEASEDATE TEXT,"
                          "GENRE TEXT,"
                          "BANNERPATH TEXT);");

    if (rtn)
    {
        emit dbChanged();
    }

    return rtn;
}

/** Add a game to the database and repopulate the games list.
 * \param game The game struct to add.
 * \return Success/failure of the operation.
*/
bool Database::addGame(Game game)
{
    QSqlQuery query(db);
    query.prepare("INSERT OR IGNORE INTO GAMES (GAMENAME,"
                                               "GAMEDIRECTORY,"
                                               "GAMEEXECUTABLE,"
                                               "ARGUMENTS,"
                                               "DRM,"
                                               "DEVELOPER,"
                                               "PUBLISHER,"
                                               "RELEASEDATE,"
                                               "GENRE,"
                                               "BANNERPATH)"
                                        "VALUES (:gameName,"
                                                ":gameDirectory,"
                                                ":executablePath,"
                                                ":arguments,"
                                                ":drm,"
                                                ":developer,"
                                                ":publisher,"
                                                ":releaseDate,"
                                                ":genre,"
                                                ":bannerPath);");

    query.bindValue(":gameName", game.gameName);
    query.bindValue(":gameDirectory", game.gameDirectory);
    query.bindValue(":executablePath", game.executablePath);
    query.bindValue(":arguments", game.arguments);
    query.bindValue(":drm", game.drm);
    query.bindValue(":developer", game.developer);
    query.bindValue(":publisher", game.publisher);
    query.bindValue(":releaseDate", game.releaseDate);
    query.bindValue(":genre", game.genre);
    query.bindValue(":bannerPath", game.bannerPath);

    bool rtn = query.exec();

    if (rtn)
    {
        emit dbChanged();
    }
}

/** Add games to the database and repopulate the games list.
 * \param games GameList of games to add.
 * \return Success/failure of the operation.
*/
void Database::addGames(GameList games)
{
    for (auto& game : games)
    {
        addGame(game);
    }
}

/** Remove a game from the database by their ID.
 * \param id ID of the game to remove.
 * \return Success/failure of the operation.
*/
bool Database::removeGameById(unsigned int id)
{
    if (std::get<0>(isExistant(id)))
    {
        QSqlQuery query(db);
        query.prepare("DELETE FROM games WHERE ID = :id;");
        query.bindValue(":id", id);
        bool rtn = query.exec();

        if (rtn)
        {
            emit dbChanged();
        }

        return rtn;
    }
    else
    {
        return false;
    }
}

/** Remove a game from the database by their name.
 * \param name Name of the game to remove.
*/
bool Database::removeGameByName(QString name)
{
    if (std::get<0>(isExistant(name)))
    {
        QSqlQuery query(db);
        query.prepare("DELETE FROM GAMES WHERE GAMENAME = :name;");
        query.bindValue(":name", name);
        bool rtn = query.exec();

        if (rtn)
        {
            emit dbChanged();
        }

        return rtn;
    }
    else
    {
        return false;
    }
}

/** Wrapper to access the Game object from the ID.
 * \param id ID to find.
 * \return A Game object, empty upon failure.
*/
Game Database::getGameById(unsigned int id)
{
    return std::get<1>(isExistant(id));
}

/** Wrapper to access the Game object from the name.
 * \param id ID to find.
 * \return A Game object, empty upon failure.
*/
Game Database::getGameByName(QString name)
{
    return std::get<1>(isExistant(name));
}

/** Perform a query to find a specific game in the database by their ID. Unsafe at the
 * moment.
 * \param id ID of the game to find.
 * \return A Game object upon success, 0 upon failure.
*/
std::pair<bool, Game> Database::isExistant(unsigned int id)
{
    QSqlQuery query(db);
    query.prepare("SELECT ID,"
                      "GAMENAME,"
                      "GAMEDIRECTORY,"
                      "GAMEEXECUTABLE,"
                      "ARGUMENTS,"
                      "DRM,"
                      "DEVELOPER,"
                      "PUBLISHER,"
                      "RELEASEDATE,"
                      "GENRE,"
                      "BANNERPATH"
                      " FROM GAMES WHERE ID = :id;");
    query.bindValue(":id", id);
    query.exec();

    if (query.next())
    {
        QString name = query.value(1).toString();
        QString path = query.value(2).toString();
        QString exe = query.value(3).toString();
        QString args = query.value(4).toString();
        int drm = query.value(5).toInt();
        QString developer = query.value(6).toString();
        QString publisher = query.value(7).toString();
        QString releaseDate = query.value(8).toString();
        QString genre = query.value(9).toString();
        QString bannerPath = query.value(10).toString();

        return std::make_pair(true, Game {id,
                                          name,
                                          path,
                                          exe,
                                          args,
                                          drm,
                                          developer,
                                          publisher,
                                          releaseDate,
                                          genre,
                                          bannerPath});
    }
    else
    {
        return std::make_pair(false, Game{});
    }
}

/** Perform a query to find a specific game by their name (soon to be
 * deprecated).
 *
 * \param name Name of the game to find.
 * \return A Game object upon success, 0 upon failure.
*/
std::pair<bool, Game> Database::isExistant(QString name)
{
    QSqlQuery query(db);
    query.prepare("SELECT ID,"
                      "GAMEDIRECTORY,"
                      "GAMEEXECUTABLE,"
                      "ARGUMENTS,"
                      "DRM,"
                      "DEVELOPER,"
                      "PUBLISHER,"
                      "RELEASEDATE,"
                      "GENRE,"
                      "BANNERPATH"
                      " FROM GAMES WHERE GAMENAME = :name;");
    query.bindValue(":name", name);
    query.exec();
    if (query.next())
    {
        unsigned int id = query.value(0).toUInt();
        QString path = query.value(1).toString();
        QString exe = query.value(2).toString();
        QString args = query.value(3).toString();
        int drm = query.value(4).toInt();
        QString developer = query.value(5).toString();
        QString publisher = query.value(6).toString();
        QString releaseDate = query.value(7).toString();
        QString genre = query.value(8).toString();
        QString bannerPath = query.value(9).toString();

        return std::make_pair(true, Game {id,
                                          name,
                                          path,
                                          exe,
                                          args,
                                          drm,
                                          developer,
                                          publisher,
                                          releaseDate,
                                          genre,
                                          bannerPath});
    }
    else
    {
        return std::make_pair(false, Game{});
    }
}

/** Perform a query to find every game in the database.
 * \return A QList of Game objects containing everything in the database.
*/
QList<Game> Database::getGames()
{
    QList<Game> games;
    QSqlQuery query;
    query.exec("SELECT ID,"
                      "GAMENAME,"
                      "GAMEDIRECTORY,"
                      "GAMEEXECUTABLE,"
                      "ARGUMENTS,"
                      "DRM,"
                      "DEVELOPER,"
                      "PUBLISHER,"
                      "RELEASEDATE,"
                      "GENRE,"
                      "BANNERPATH"
                      " FROM GAMES;");
    while (query.next())
    {
        unsigned int id = query.value(0).toUInt();
        QString name = query.value(1).toString();
        QString path = query.value(2).toString();
        QString exe = query.value(3).toString();
        QString args = query.value(4).toString();
        int drm = query.value(5).toInt();
        QString developer = query.value(6).toString();
        QString publisher = query.value(7).toString();
        QString releaseDate = query.value(8).toString();
        QString genre = query.value(9).toString();
        QString bannerPath = query.value(10).toString();

        games.append({id,
                      name,
                      path,
                      exe,
                      args,
                      drm,
                      developer,
                      publisher,
                      releaseDate,
                      genre,
                      bannerPath});
    }
    return games;
}

/** Queries the database to find the number of games.
 * \return Total number of games stored so far.
*/
unsigned int Database::getGameCount() const
{
    QSqlQuery query(db);
    query.exec("SELECT count() FROM GAMES;");
    if (!query.next())
    {
        return 0;
    }

    return query.value(0).toUInt();
}

/** Sets the launch options of a game by ID.
 * \param id The ID of the game
 * \param launchOpts The new launch options
 * \return Success (true)/Failure (false) of the operation.
*/
bool Database::setLaunchOptionsById(unsigned int id, QString launchOpts)
{
    if (std::get<0>(isExistant(id)))
    {
        QSqlQuery query(db);
        query.prepare("UPDATE games SET `ARGUMENTS` = :newOpts WHERE ID = :id;");
        query.bindValue(":newOpts", launchOpts);
        query.bindValue(":id", id);
        bool rtn = query.exec();

        if (rtn)
        {
            emit dbChanged();
        }

        return rtn;
    }
    else
    {
        return false;
    }
}

/** Sets the launch options of a game by name.
 * \param name The name of the game (as stored in the database)
 * \param launchOpts The new launch options
 * \return Success (true)/Failure (false) of the operation.
*/
bool Database::setLaunchOptionsByName(QString name, QString launchOpts)
{
    if (std::get<0>(isExistant(name)))
    {
        QSqlQuery query(db);
        query.prepare("UPDATE games SET `ARGUMENTS` = :newOpts WHERE GAMENAME = :name;");
        query.bindValue(":newOpts", launchOpts);
        query.bindValue(":name", name);
        bool rtn = query.exec();

        if (rtn)
        {
            emit dbChanged();
        }

        return rtn;
    }
    else
    {
        return false;
    }
}

/** Returns the name of a DRM ID
 * \param drmID The drm ID
 * \return The localized DRM name
*/
QString Database::getDRMString(int drmID)
{
    if (drmID == 0)
    {
        return tr("None");
    }
    else if (drmID == 1)
    {
        return tr("Steam");
    }
    else if (drmID == 2)
    {
        return tr("Origin");
    }
    else if (drmID == 3)
    {
        return tr("uPlay");
    }
}
