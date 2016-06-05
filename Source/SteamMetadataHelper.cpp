#include "SteamMetadataHelper.h"

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>

#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QJsonDocument>

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
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QUrl url("http://store.steampowered.com/api/appdetails?appids=" + this->appid);
    QNetworkRequest *req = new QNetworkRequest(url);
    QNetworkReply *reply = manager->get(*req);

    QObject::connect(reply, &QNetworkReply::finished, [=]
    {
        this->headerRequestFinished(reply);
    });
}

/** Download game header art.
 * Stub.
 * When done, the headerDownloadCompleted signal will be emitted.
 */
void SteamMetadataHelper::downloadHeader(QString filePath)
{}

void SteamMetadataHelper::headerRequestFinished(QNetworkReply *reply)
{
    QByteArray replyBytes = reply->readAll();
    QString jsonString(replyBytes);

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
