#include "News.h"
#include <QSettings>
#include <string>
#include <QDebug>
#include <QListWidgetItem>
#include <QtNetwork>
#include <QShortcut>
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
    QShortcut* shortcut = new QShortcut(QKeySequence("Ctrl+R"), parent);
    connect(shortcut, &QShortcut::activated, this, &News::refreshRequested);

    setupUI();
    loadFeeds();
}

void News::refreshRequested() {
    qDebug() << "Refresh" << endl;
    loadFeeds();
}

void News::loadFeeds() {
    headlines.clear();
    clearLayout(firstColumn);
    clearLayout(secondColumn);
    clearLayout(thirdColumn);
    loadFeedUrlsFromSettings();
    loadXMLfromUrls();
}
void News::loadXMLfromUrls()
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
    feedUrls.clear();
    QSettings settings ("Horizon Launcher", "Launcher");
    int size = settings.beginReadArray ("URLs");

    for (int i = 0; i < size; ++i) {
        settings.setArrayIndex(i);
        QString current = settings.value("url").toString();
        qDebug() << "Read URL " << current << endl;
        feedUrls.append(current);
    }
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
    qDebug() << "Fetch compete" << endl;
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
               qDebug() << "Added headline" << endl;

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
    qDebug() << "Total Size " << size << endl;
    //Initialize an array to keep track of which items were already inserted into the view
    bool* inserted = new bool[size];
    for (int i = 0; i < size; ++i) {
        inserted[i] = false;
    }

    bool done = false;
    int columnToInsert = 1;
    int insertedCount = 0;
    while(!done) {
        int indexOfNewItem = qrand() % size;
        if (!inserted[indexOfNewItem]){
            if(columnToInsert == 1) firstColumn->addWidget(headlines.at(indexOfNewItem));
            else if (columnToInsert == 2) secondColumn->addWidget(headlines.at(indexOfNewItem));
            else if (columnToInsert == 3) thirdColumn->addWidget(headlines.at(indexOfNewItem));
            inserted[indexOfNewItem] = true;
            columnToInsert++;
            if (columnToInsert > 3) columnToInsert = 1;
            insertedCount++;
        }

        if (insertedCount == size || insertedCount > 30) done = true;
    }
}

void News::clearLayout(QLayout *layout)
{
     QLayoutItem *item;
     while ((item = layout->takeAt(0))) {
        delete item->widget();
        delete item;
    }
}
