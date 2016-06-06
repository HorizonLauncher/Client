#ifndef STEAMMETADATAHELPER_H
#define STEAMMETADATAHELPER_H

#include <QObject>

#include <QtNetwork/QNetworkReply>

struct SteamMetadata
{
    QString appid;
    QString developer;
    QString publisher;
    QString releaseDate;
    QString genres;
};

class SteamMetadataHelper : public QObject
{
    Q_OBJECT

public:
    SteamMetadataHelper(QString appid);

    void getMetadata();
    void downloadHeader(QString filePath);

signals:
    void metadataRecieved(SteamMetadata metadata);
    void headerDownloadCompleted();

private:
    QString appid;

    void metadataRequestFinished(QNetworkReply *reply);
};

#endif
