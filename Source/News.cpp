#include "News.h"
#include "ui_newspanel.h"

#include <QSettings>
#include <string>
#include <QDebug>

News::News(QSettings* p, QWidget* parent) :
    QWidget(parent),
    ui(new Ui::News)
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
                        "QListWidget {"
                        "background-color: " + p->value("Primary/TertiaryBase").toString() + "; "
                        "color: " + p->value("Primary/LightText").toString() + ";}"
                        "QLabel {"
                        "color: " + p->value("Primary/LightText").toString() + ";"
                        "font-family: SourceSansPro;"
                        "}"
                        "#gameNameLabel { font-size: 20px; }");

    loadXML();
}

void News::loadXML()

{
    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    std::string urlString = "http://feeds.ign.com/ign/news?format=xml";

    const QUrl url(QString::fromStdString(urlString));


    QNetworkRequest* req = new QNetworkRequest(url);


   req->setRawHeader("User-Agent", "Horizon Launcher");

    QNetworkReply* reply = manager->get(*req);

    QObject::connect(reply, SIGNAL(finished()), this, SLOT(onFetchComplete()));
    QObject::connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(errorOccured(QNetworkReply::NetworkError)));

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

void News::onFetchComplete()

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

                ui->newsList->addItem(reader.readElementText());

            }

        }

        reader.readNext();
    }

    reply->deleteLater();
}


