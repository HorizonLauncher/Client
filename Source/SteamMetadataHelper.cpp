#include "SteamMetadataHelper.h"

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>

#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QJsonDocument>

#include <QPixmap>
#include <QFile>

void SteamMetadataWorker::downloadData(QString url)
{
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QNetworkRequest *req = new QNetworkRequest(QUrl(url));
    QNetworkReply *reply = manager->get(*req);

    QObject::connect(reply, &QNetworkReply::finished, [=]
    {
        emit resultReady(reply->readAll());
    });
}

/** SteamMetadataHelper constructor
 * Class to handle downloading metadata & game art from SteamHelper
 * \param appid The AppID of the game to download metadata of.
 */
SteamMetadataHelper::SteamMetadataHelper(QString appid)
    : QObject(), appid(appid)
{
}

/** Get metadata.
 * Metadata will be returned in the metadataRecieved signal.
 */
void SteamMetadataHelper::getMetadata()
{
    SteamMetadataWorker *worker = new SteamMetadataWorker;
    worker->moveToThread(&this->workerThread);

    connect(&this->workerThread, &QThread::finished, worker, &QObject::deleteLater);
    connect(this, &SteamMetadataHelper::startMetadataDownload, worker, &SteamMetadataWorker::downloadData);
    connect(worker, &SteamMetadataWorker::resultReady, this, &SteamMetadataHelper::metadataRequestFinished);

    workerThread.start();
    emit startMetadataDownload("http://store.steampowered.com/api/appdetails?appids=" + this->appid);
}

/** Download game header art.
 * When done, the headerDownloadCompleted signal will be emitted.
 * \param filePath The path for the image to be saved to.
 */
void SteamMetadataHelper::downloadHeader(QString filePath)
{
    SteamMetadataWorker *worker = new SteamMetadataWorker;
    worker->moveToThread(&this->workerThread);

    connect(&this->workerThread, &QThread::finished, worker, &QObject::deleteLater);
    connect(this, &SteamMetadataHelper::startHeaderDownload, worker, &SteamMetadataWorker::downloadData);
    connect(worker, &SteamMetadataWorker::resultReady, [=](QByteArray result)
    {
        QPixmap p;
        p.loadFromData(result);

        if (!p.isNull())
        {
            QPixmap scaledP = p.scaled(364, 170);
            scaledP.save(filePath, "PNG");

            emit headerDownloadCompleted();
        }
    });

    QString url("http://localhost:8000/steam/apps/" + this->appid + "/header.jpg");
    //QUrl url("http://cdn.akamai.steamstatic.com/steam/apps/" + this->appid + "/header.jpg");

    workerThread.start();
    emit startHeaderDownload(url);
}

void SteamMetadataHelper::metadataRequestFinished(QByteArray response)
{
    QString jsonString(response);

    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonString.toUtf8());
    QJsonObject jsonObj = jsonDoc.object();

    QString developers, publishers, releaseDate, genres;

    if (jsonObj.contains(this->appid))
    {
        QJsonObject appTree = jsonObj[this->appid].toObject();
        if (appTree.contains("success") && appTree["success"].toBool())
        {
            QJsonObject data = appTree["data"].toObject();

            QJsonArray developersArray = data["developers"].toArray();
            QJsonArray publishersArray = data["publishers"].toArray();
            for (int i = 0; i < developersArray.size(); i++)
            {
                if (i != 0)
                    developers += ", ";

                developers += developersArray[i].toString();
            }

            for (int i = 0; i < publishersArray.size(); i++)
            {
                if (i != 0)
                    publishers += ", ";

                publishers += publishersArray[i].toString();
            }

            releaseDate = data["release_date"].toObject()["date"].toString();

            QJsonArray genresArray = data["genres"].toArray();
            for (int i = 0; i < genresArray.size(); i++)
            {
                QJsonObject genreObj = genresArray[i].toObject();
                if (i != 0)
                    genres += ", ";

                genres += genreObj["description"].toString();
            }
        }
    }

    SteamMetadata metadata =  SteamMetadata {this->appid,
                                             developers,
                                             publishers,
                                             releaseDate,
                                             genres};

     emit metadataRecieved(metadata);
}
