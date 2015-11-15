#include "News.h"
#include "ui_newspanel.h"

#include <QSettings>
#include <string>
#include <QDebug>
#include <QListWidgetItem>
#include <QtNetwork>

News::News(QSettings* p, QWidget* parent) :
    QWidget(parent)
{
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
    this->loadFeedUrlsFromSettings();

    setupUI();
    loadXML();
}

void News::loadXML()
{
    for (auto urlString : feedUrls)
    {
        QNetworkAccessManager* manager = new QNetworkAccessManager(this);
        const QUrl url(urlString);
        QNetworkRequest* req = new QNetworkRequest(url);
        req->setRawHeader("User-Agent", "Horizon Launcher");
        QNetworkReply* reply = manager->get(*req);
        QObject::connect(reply, SIGNAL(finished()), this, SLOT(onFetchComplete()));
    }
}

void News::loadFeedUrlsFromSettings()
{
    QSettings settings ("Horizon Launcher", "Launcher");
    int size = settings.beginReadArray ("URLs");

    for (int i = 0; i < size; ++i) {
        settings.setArrayIndex(i);
        QString current = settings.value("url").toString();
        feedUrls.append(current);
    }

    settings.endArray();
}
News::~News()
{
    for (int i = 0; i < headlines.size(); ++i)
    {
        delete headlines[i];
    }

    delete firstColumn;
    delete secondColumn;
    delete thirdColumn;
    delete mainLayout;
}

void News::setupUI() {

    mainLayout = new QHBoxLayout();
    firstColumn = new QVBoxLayout();
    secondColumn = new QVBoxLayout();
    thirdColumn = new QVBoxLayout();
    mainLayout->addLayout(firstColumn);
    mainLayout->addLayout(secondColumn);
    mainLayout->addLayout(thirdColumn);
    this->setLayout(mainLayout);

}

void News::onFetchComplete()
{
    QNetworkReply *reply = (QNetworkReply*)sender();

    if (reply->error())
    {
        qDebug("Error with network request");
    }

    QByteArray array = reply->readAll();
    QXmlStreamReader reader(array);

    if (reader.hasError())
    {
        qDebug("Error parsing XML");
    }

    reader.readNext();

    while(!reader.atEnd())
    {
        if (reader.isStartElement())
        {
            if (reader.name() == "item")
            {
               while (reader.name() != "title")
               {
                    reader.readNext();
               }

               NewsItemWidget* currentItemWidget = new NewsItemWidget(settings, nullptr);
               QString title = reader.readElementText();
               QListWidgetItem* item = new QListWidgetItem(" ");
               currentItemWidget->titleLabel->setText(title);
               QString text = "";

               while (reader.name() != "link")
               {
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

void News::reloadHeadlines()
{
    int size = headlines.size();
    if (size == 0) return;
    for (int i = 0; i < 9; ++i)
    {
        firstColumn->addWidget(headlines.at(qrand() % size));
        secondColumn->addWidget(headlines.at(qrand() % size));
        thirdColumn->addWidget(headlines.at(qrand() % size));
    }
}
