#include "steamhelper.h"

#include <QProcess>
#include <QSettings>
#include <QDir>
#include <QFile>
#include <QRegExp>
#include <QTextStream>

/** Checks if Steam exists on the system
*   \return true if Steam is installed, false otherwise
*/
bool SteamHelper::doesExist()
{
    #if defined(__linux__)
        QProcess which;
        which.start("which", QStringList() << "steam");
        which.waitForFinished();
        return which.exitCode() == 0;
    #elif defined(_WIN32) || defined(_WIN64)
        QSettings settings("HKEY_CURRENT_USER\\Software\\Valve\\Steam", QSettings::NativeFormat);
        return settings.contains("SteamPath");
    #elif defined(__APPLE__)
        QDir folder = QDir(QDir::home().filePath("Library/Application Support/Steam"));
        return folder.exists();
    #endif
}

/** Gets the path to Steam
*   \return The path to the Steam installation folder
*/
QString SteamHelper::getFolder()
{
    if (!doesExist())
    {
        return "";
    }

    #if defined(__linux__)
        return QDir(QDir::homePath() + "/.local/share/Steam").canonicalPath();
    #elif defined(_WIN32) || defined(_WIN64)
        QSettings settings("HKEY_CURRENT_USER\\Software\\Valve\\Steam", QSettings::NativeFormat);
        return QDir(settings.value("SteamPath").toString()).canonicalPath();
    #elif defined(__APPLE__)
        return QDir(QDir::home().filePath("Library/Application Support/Steam")).canonicalPath();
    #endif
}

/** Returns a map of key/value pairs in a VDF file.
*   Really hacky. Ignores objects (e.g. a = "b", b { z = "s" }, c { z = "z" })
*   z will be "z" because the objects get ignored. Will break if Valve
*   complicates the one file that this is used for.
*
*   \param vdfPath a path to the VDF file to be read
*   \return A QMap of key/value pairs in a VDF file.
*/
QMap<QString, QString> SteamHelper::getVdfKeyVals(QString vdfPath)
{
    QFile vdfFile(vdfPath);

    if (!vdfFile.open(QIODevice::ReadOnly))
    {
        return QMap<QString, QString>();
    }

    QTextStream fileStream(&vdfFile);

    QRegExp keyValPairRegex("^(?:\\s+)?\"(.+)\"\\s+\"(.+)\"(?:\\s+)?$");

    QMap<QString, QString> keyVals;

    while (!fileStream.atEnd())
    {
        QString line = fileStream.readLine();
        if (line.contains(keyValPairRegex))
        {
            keyValPairRegex.indexIn(line);
            QString key = keyValPairRegex.cap(1);
            QString val = keyValPairRegex.cap(2);
            keyVals[key] = val;
        }
    }

    return keyVals;
}

/** Gets list of Steam library folders
*   \return A QList with paths to Steam library folders. Does NOT include main folder.
*/
QList<QString> SteamHelper::getLibFolders()
{
    if (!doesExist())
    {
        return QList<QString>();
    }

    QDir steamappsPath = QDir(getFolder());
    if (steamappsPath.exists("steamapps"))
    {
        steamappsPath.cd("steamapps");
    }
    else if (steamappsPath.exists("SteamApps"))
    {
        steamappsPath.cd("SteamApps");
    }
    else
    {
        return QList<QString>();
    }
    QString libraryFoldersPath = steamappsPath.filePath("libraryfolders.vdf");

    QMap<QString, QString> keyVals = getVdfKeyVals(libraryFoldersPath);

    QList<QString> libFolders;

    for (QString key : keyVals.keys())
    {
        if (key != "TimeNextStatsReport" && key != "ContentStatsID")
        {
            libFolders << keyVals[key];
        }
    }

    return libFolders;
}

/** Get a list of all games in a Steam library folder
*   \param folderPath A path to the Steam library folder
*   \return A map of games. Key is the name, value is the appID.
*/
QMap<QString, QString> SteamHelper::getGamesInFolder(QString folderPath)
{
    QDir folder(folderPath);
    if (folder.exists("steamapps"))
    {
        folder.cd("steamapps");
    }
    else if (folder.exists("SteamApps"))
    {
        folder.cd("SteamApps");
    }
    else
    {
        return QMap<QString, QString>();
    }

    folder.setNameFilters(QStringList("appmanifest_*.acf"));
    QFileInfoList appmanifests = folder.entryInfoList();

    QMap<QString, QString> games;

    for (QFileInfo appmanifest : appmanifests)
    {
        QMap<QString, QString> keyVals = getVdfKeyVals(appmanifest.canonicalFilePath());

        if (!keyVals.contains("name")) { continue; }
        if (keyVals.contains("appid"))
        {
            games[keyVals["name"]] = keyVals["appid"];
        }
        else if (keyVals.contains("appID"))
        {
            games[keyVals["name"]] = keyVals["appID"];
        }
        else
        {
            continue;
        }
    }

    return games;
}

/** Get a map of ALL games in all Steam library folders.
*   \return A map of games. Key is the name, value is the appID.
*/
QMap<QString, QString> SteamHelper::getGames()
{
    if (!doesExist())
    {
        return QMap<QString, QString>();
    }

    QString steamPath = getFolder();
    QDir steamappsPath(steamPath);
    if (steamappsPath.exists("steamapps"))
    {
        steamappsPath.cd("steamapps");
    }
    else if (steamappsPath.exists("SteamApps"))
    {
        steamappsPath.cd("SteamApps");
    }
    else
    {
        return QMap<QString, QString>();
    }

    QMap<QString, QString> gamesList;

    QList<QString> libraryFolders = getLibFolders();
    libraryFolders << steamPath;

    for (QString folder : libraryFolders)
    {
        QMap<QString, QString> games = getGamesInFolder(folder);

        for (QString game : games.keys())
        {
            if (gamesList.contains(game)) { continue; }
            gamesList[game] = games[game];
        }
    }

    return gamesList;
}
