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

    this->feedUrls.append("http://feeds.ign.com/ign/news?format=xml");
    this->feedUrls.append("http://feeds.feedburner.com/RockPaperShotgun?format=xml");
    this->feedUrls.append("http://www.reddit.com/r/pcmasterrace.rss");


    loadXML();
}

void News::loadXML()

{

    for (auto urlString : feedUrls) {

        QNetworkAccessManager* manager = new QNetworkAccessManager(this);
        const QUrl url(urlString);
        QNetworkRequest* req = new QNetworkRequest(url);
        req->setRawHeader("User-Agent", "Horizon Launcher");
        QNetworkReply* reply = manager->get(*req);
        QObject::connect(reply, SIGNAL(finished()), this, SLOT(onFetchComplete()));

    }
}

News::~News()
{
    delete ui;
}

void News::onFetchComplete()

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

            if (reader.name() == "item") {

               while (reader.name() != "title") {
                    reader.readNext();
               }

               NewsItemWidget* currentItemWidget = new NewsItemWidget(settings, nullptr);
               QString title = reader.readElementText();
               QListWidgetItem* item = new QListWidgetItem(" ");
               currentItemWidget->titleLabel->setText(title);

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

void News::reloadHeadlines() {

    int size = headlines.size();

    for (int i = 0; i < 9; ++i) {

        ui->firstColumn->addWidget(headlines.at(qrand() % size));
        ui->secondColumn->addWidget(headlines.at(qrand() % size));
        ui->thirdColumn->addWidget(headlines.at(qrand() % size));

    }
}



