#include "SteamHelper.h"

#include <QProcess>
#include <QSettings>
#include <QDir>
#include <QFile>
#include <QRegExp>
#include <QDebug>

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

QMap<QString, QString> SteamHelper::getVdfKeyVals(QString vdfPath)
{
    QFile vdfFile(vdfPath);

    if (!vdfFile.open(QIODevice::ReadOnly))
    {
        qDebug() << "Couldn't open " << vdfPath << " for reading.";
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
