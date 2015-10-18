#include "News.h"
#include "ui_news.h"

#include <QSettings>
#include <string>
#include <QDebug>
#include <Source/Libs/pugixml.hpp>


News::News(QSettings* p, QWidget* parent) :
    QWidget(parent),
    ui(new Ui::News)
{
    ui->setupUi(this);
    this->setObjectName("NewsUI");
    this->setStyleSheet("background-color: " + p->value("Primary/SecondaryBase").toString() + ";} "
                        "QPushButton {"
                        "color: " + p->value("Primary/LightText").toString() + "; "
                        "background-color: " + p->value("Primary/DarkElement").toString() + "; "
                        "border: none; margin: 0px; padding: 0px;} "
                        "QPushButton:hover {"
                        "background-color: " + p->value("Primary/InactiveSelection").toString() + ";} "
                        "color: " + p->value("Primary/LightText").toString() + ";");


    ui->newsList->addItem("test");

    loadXML();

}

void News::loadXML() {

    QNetworkAccessManager manager;
    std::string urlString = "dynamic.feedsportal.com/pf/510578/http://www.pcgamer.com/feed/rss2/";

    const QUrl url(QString::fromStdString(urlString));


    QNetworkRequest req(url);


    req.setRawHeader("User-Agent", "Meeting C++ RSS Reader");

    QNetworkReply* reply = manager.get(req);

    QByteArray array = reply->readAll();

    QXmlStreamReader reader(array);

    reader.readNext();

    while(!reader.atEnd()) {

        if (reader.isStartElement()) {

            qDebug(reader.name().string()->toStdString().c_str());
        }

        reader.readNext();
    }

}

News::~News()
{
    delete ui;
}


