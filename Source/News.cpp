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
                        "QLabel {"
                        "color: " + p->value("Primary/LightText").toString() + ";"
                        "font-family: SourceSansPro;"
                        "}"
                        "#gameNameLabel { font-size: 20px; }");
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
   // QObject::connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(errorOccured(QNetworkReply::NetworkError)));

    std::string PCGamerUrlString = "http://feeds.feedburner.com/RockPaperShotgun?format=xml";
    const QUrl PCGamerUrl (QString::fromStdString(PCGamerUrlString));
    QNetworkRequest* PCGamerReq = new QNetworkRequest(PCGamerUrl);
    PCGamerReq->setRawHeader("User-Agent", "Horizon Launcher");
    QNetworkReply* PCGamerReply = manager->get(*PCGamerReq);
    QObject::connect(PCGamerReply, SIGNAL(finished()), this, SLOT (onFetchCompletePCGamer()));


    std::string PCMRUrlString = "http://www.reddit.com/r/pcmasterrace.rss";
    const QUrl PCMRUrl (QString::fromStdString(PCMRUrlString));
    QNetworkRequest* PCMRReq = new QNetworkRequest(PCMRUrl);
    PCMRReq->setRawHeader("User-Agent", "Horizon Launcher");
    QNetworkReply* PCMRReply = manager->get(*PCMRReq);
    QObject::connect(PCMRReply, SIGNAL(finished()), this, SLOT (onFetchCompleteReddit()));


}

News::~News()
{
    qDebug("deleted");
    delete ui;
}

void News::errorOccured(QNetworkReply::NetworkError)

{
       qDebug("error");
}

void News::onFetchCompleteIGN()

{\
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

               NewsItemWidget* currentItemWidget = new NewsItemWidget(settings, this);
               QString title = reader.readElementText();
               QListWidgetItem* item = new QListWidgetItem(" ");
               currentItemWidget->titleLabel->setText(title);

               QString text = "";

               while (reader.name() != "description") {
                   reader.readNext();
               }

               text = this->parseElementText(reader.readElementText());
               currentItemWidget->contentLabel->setText(text);
               ui->firstColumn->addWidget(currentItemWidget);

            }

        }

        reader.readNext();

    }

    reply->deleteLater();
}



void News::onFetchCompletePCGamer () {

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

                NewsItemWidget* currentItemWidget = new NewsItemWidget(settings, this);
                QString title = reader.readElementText();
                QListWidgetItem* item = new QListWidgetItem(" ");
                currentItemWidget->titleLabel->setText(title);

                QString text = "";

                while (reader.name() != "description") {
                    reader.readNext();
                }

                text = this->parseElementText(reader.readElementText());
                currentItemWidget->contentLabel->setText(text);
                ui->secondColumn->addWidget(currentItemWidget);

            }

        }

        reader.readNextStartElement();
    }

}


void News::onFetchCompleteReddit() {
    \
    QNetworkReply *reply = (QNetworkReply*)sender();

    if (reply->error()) {
        qDebug() << reply->errorString();
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

               NewsItemWidget* currentItemWidget = new NewsItemWidget(settings, this);
               QString title = reader.readElementText();
               QListWidgetItem* item = new QListWidgetItem(" ");
               currentItemWidget->titleLabel->setText(title);

               QString text = "";

               while (reader.name() != "link") {
                   reader.readNext();
               }

               text = this->parseElementText(reader.readElementText());
               currentItemWidget->contentLabel->setText(text);
               ui->thirdColumn->addWidget(currentItemWidget);
               reader.readNextStartElement();
               while (reader.name() != "title") reader.readNext();
               reader.readNext();

           }
        }

        reader.readNext();
    }


}

QString News::parseElementText(QString input) {

   // qDebug() << input;
    return input;
}


