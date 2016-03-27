#ifndef STEAMHELPER_H
#define STEAMHELPER_H

#include <QMap>

class SteamHelper
{
public:
    static bool doesExist();
    static QString getFolder();
    static QMap<QString, QString> getVdfKeyVals(QString vdfPath);
    static QList<QString> getLibFolders();
    static QMap<QString, QString> getGamesInFolder(QString folderPath);
    static QMap<QString, QString> getGames();
};

#endif
