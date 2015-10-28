#include "News.h"
#include "ui_newspanel.h"

#include <QSettings>
#include <string>
#include <QDebug>
#include <QListWidgetItem>
#include <QtNetwork>

News::News(QSettings* p, QWidget* parent) :
    QWidget(parent),

    ui(new Ui::NewsPanel)
{
    ui->setupUi(this);
    this->setObjectName("NewsUI");
    this->setStyleSheet("#leftSidebar {"
                        "background-color: " + p->value("Primary/SecondaryBase").toString() + ";} "
                        "#content {"
                        "background-color: " + p->value("Primary/SecondaryBase").toString() + ";} "
                        "QPushButton {"
                        "color: " + p->value("Primary/LightText").toString() + "; "
                        "background-color: " + p->value("Primary/DarkElement").toString() + "; "
                        "border: none; margin: 0px; padding: 0px;} "
                        "QPushButton:hover {"
                        "background-color: " + p->value("Primary/InactiveSelection").toString() + ";} "
                        "QVBoxLayout {"
                        "background-color: " + p->value("Primary/TertiaryBase").toString() + "; "
                        "color: " + p->value("Primary/LightText").toString() + ";}"
                           );
    this->settings = p;

    loadXML();
}

void News::loadXML()

{
    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    std::string IGNurlString = "http://feeds.ign.com/ign/news?format=xml";
    const QUrl IGNurl(QString::fromStdString(IGNurlString));
    QNetworkRequest* IGNreq = new QNetworkRequest(IGNurl);
    IGNreq->setRawHeader("User-Agent", "Horizon Launcher");
    QNetworkReply* IGNreply = manager->get(*IGNreq);
    QObject::connect(IGNreply, SIGNAL(finished()), this, SLOT(onFetchCompleteIGN()));

    std::string RPSUrlString = "http://feeds.feedburner.com/RockPaperShotgun?format=xml";
    const QUrl RPSUrl (QString::fromStdString(RPSUrlString));
    QNetworkRequest* RPSReq = new QNetworkRequest(RPSUrl);
    RPSReq->setRawHeader("User-Agent", "Horizon Launcher");
    QNetworkReply* RPSReply = manager->get(*RPSReq);
    QObject::connect(RPSReply, SIGNAL(finished()), this, SLOT (onFetchCompleteRPS()));

    std::string PCMRUrlString = "http://www.reddit.com/r/pcmasterrace.rss";
    const QUrl PCMRUrl (QString::fromStdString(PCMRUrlString));
    QNetworkRequest* PCMRReq = new QNetworkRequest(PCMRUrl);
    PCMRReq->setRawHeader("User-Agent", "Horizon Launcher");
    QNetworkReply* PCMRReply = manager->get(*PCMRReq);
    QObject::connect(PCMRReply, SIGNAL(finished()), this, SLOT (onFetchCompleteReddit()));


}

News::~News()
{
    delete ui;
}

void News::onFetchCompleteIGN()

{
    QNetworkReply *reply = (QNetworkReply*)sender();

    if (reply->error()) {
        qDebug("Error with network request");
    }

    QByteArray array = reply->readAll();

    QXmlStreamReader reader(array);

    if (reader.hasError()) {

        qDebug("Error parsing XML");
    }

    reader.readNext();

    while(!reader.atEnd()) {

        if (reader.isStartElement()) {

            if (reader.name() == "title") {

               NewsItemWidget* currentItemWidget = new NewsItemWidget(settings, nullptr);
               QString title = reader.readElementText();
               QListWidgetItem* item = new QListWidgetItem(" ");
               currentItemWidget->titleLabel->setText(title + " [IGN]");

               QString text = "";

               while (reader.name() != "link") {
                   reader.readNext();
               }

               currentItemWidget->urlString = reader.readElementText();
               headlines.append(currentItemWidget);

            }

        }

        reader.readNext();

    }

    reply->deleteLater();
    reloadHeadlines();
}

void News::onFetchCompleteRPS () {

    QNetworkReply *reply = (QNetworkReply*)sender();

    if (reply->error()) {
        qDebug("Error with network request");
    }

    QByteArray array = reply->readAll();

    qDebug() << "fetch complete";

    QXmlStreamReader reader(array);

    if (reader.hasError()) {
        qDebug("Error parsing XML");
    }

    reader.readNextStartElement();

    while(!reader.atEnd()) {

        if (reader.isStartElement()) {

            if (reader.name() == "title") {

                NewsItemWidget* currentItemWidget = new NewsItemWidget(settings, nullptr);
                QString title = reader.readElementText();
                QListWidgetItem* item = new QListWidgetItem(" ");
                currentItemWidget->titleLabel->setText(title + " [Rock, Paper, Shotgun]");

                QString text = "";
                while (reader.name() != "link") reader.readNext();

                currentItemWidget->urlString = reader.readElementText();
                headlines.append(currentItemWidget);
            }

        }

        reader.readNext();
    }

    reply->deleteLater();
    reloadHeadlines();

}


void News::onFetchCompleteReddit() {
    \
    QNetworkReply *reply = (QNetworkReply*)sender();

    if (reply->error()) {
        qDebug() << reply->errorString();
    }

    QByteArray array = reply->readAll();

    QXmlStreamReader reader(array);

    if (reader.hasError()) {

        qDebug("Error parsing XML");
    }

    reader.readNext();

    while (reader.name() != "item") reader.readNextStartElement();

    while(!reader.atEnd()) {

        if (reader.name() == "item" && reader.isStartElement()) {

            NewsItemWidget* currentItem = new NewsItemWidget(settings, nullptr);

            while (reader.name() != "title") reader.readNext();
            currentItem->titleLabel->setText(reader.readElementText() + " [reddit]");

            while (reader.name() != "link") reader.readNext();
            currentItem->urlString = reader.readElementText();

            headlines.append(currentItem);
        }

        reader.readNextStartElement();
    }

    reply->deleteLater();
    reloadHeadlines();

}

void News::reloadHeadlines() {

    int size = headlines.size();

    for (int i = 0; i < 9; ++i) {

        ui->firstColumn->addWidget(headlines.at(qrand() % size));
        ui->secondColumn->addWidget(headlines.at(qrand() % size));
        ui->thirdColumn->addWidget(headlines.at(qrand() % size));

    }
}



