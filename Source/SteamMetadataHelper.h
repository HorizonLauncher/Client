#ifndef STEAMMETADATAHELPER_H
#define STEAMMETADATAHELPER_H

#include <QObject>

#include <QtNetwork/QNetworkReply>

#include <QThread>

struct SteamMetadata
{
    QString appid;
    QString developer;
    QString publisher;
    QString releaseDate;
    QString genres;
};

class SteamMetadataWorker : public QObject
{
    Q_OBJECT

public:
    void downloadData(QString url);

signals:
    void resultReady(QByteArray result);
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

    void startMetadataDownload(QString url);
    void startHeaderDownload(QString url);

private:
    QString appid;
    QThread workerThread;

private slots:
    void metadataRequestFinished(QByteArray response);
};

#endif
