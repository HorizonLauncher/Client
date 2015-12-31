#include "News.h"
#include <QSettings>
#include <string>
#include <QDebug>
#include <QListWidgetItem>
#include <QtNetwork>
#include <QShortcut>

/**
 * News constructor
 * @param p The color palette for the UI
 * @param parent The parent widget
 */
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

    QShortcut* shortcut = new QShortcut(QKeySequence("R"), parent);
    connect(shortcut, &QShortcut::activated, [this]
    {
        loadFeeds();
    });

    setupUI();
    loadFeeds();
}

/**
 *  Clears the columns and calls functions to fill them with new headlines.
 *  Can also be called to
 *
 */
void News::loadFeeds()
{
    
    qDebug() << "load called" << endl; 
    headlines.clear();
    clearLayout(firstColumn);
    clearLayout(secondColumn);
    clearLayout(thirdColumn);
    loadFeedUrlsFromSettings();
    loadXMLfromUrls();
}

/**
 * Sends fetch requests to the urls defined by the user
 *
 */
void News::loadXMLfromUrls()
{
    for (auto urlString : feedUrls)
    {
        QNetworkAccessManager* manager = new QNetworkAccessManager(this);
        const QUrl url(urlString);
        QNetworkRequest* req = new QNetworkRequest(url);
        req->setRawHeader("User-Agent", "Horizon Launcher");
        QNetworkReply* reply = manager->get(*req);

        QObject::connect(reply, &QNetworkReply::finished, [=]
        {
            QString feedLabel;

            QRegExp getSubreddit("^[\\w\\d]+:\\/\\/[\\w\\d]+\\.reddit\\.com\\/r\\/(.+)\\/.rss$");
            QRegExp getDomain("^[\\w\\d]+:\\/\\/([\\w\\d]+\\.)+([\\w\\d]+\\.)[\\w\\d]+");
            if (urlString.contains(getSubreddit))
            {
                getSubreddit.indexIn(urlString);
                feedLabel = "/r/" + getSubreddit.cap(1);
            }
            else if (urlString.contains(getDomain))
            {
                getDomain.indexIn(urlString);
                QString sortaDomain = getDomain.cap(2);
                feedLabel = sortaDomain.left(sortaDomain.length() - 1);
            }

            qDebug() << "Fetch compete";
            if (reply->error())
            {
                qDebug("Error with network request");
            }

            onFetchComplete(reply, feedLabel);
        });
    }
}

/**
 * Loads the user-defined XML urls from the settings file
 */
void News::loadFeedUrlsFromSettings()
{
    feedUrls.clear();
    QSettings settings ("HorizonLauncher", "news");
    int size = settings.beginReadArray ("URLs");
    for (int i = 0; i < size; ++i)
    {
        settings.setArrayIndex(i);
        QString current = settings.value("url").toString();
        qDebug() << "Read URL " << current << endl;
        feedUrls.append(current);
    }
}

/**
 * Destructor
 */
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

/**
 * Creates the main layout and the columns
 */
void News::setupUI()
{

    mainLayout = new QHBoxLayout();
    firstColumn = new QVBoxLayout();
    secondColumn = new QVBoxLayout();
    thirdColumn = new QVBoxLayout();
    mainLayout->addLayout(firstColumn);
    mainLayout->addLayout(secondColumn);
    mainLayout->addLayout(thirdColumn);
    this->setLayout(mainLayout);

}

/**
 * Called when the the fetches from the XML urls are finished.
 * Parses the XML and stores the headlines.
 */
void News::onFetchComplete(QNetworkReply *reply, QString sourceLabel)
{
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

               QString title = reader.readElementText();
               while (reader.name() != "link")
               {
                   reader.readNext();
               }
               QString urlString = reader.readElementText();

               NewsItemWidget* currentItemWidget = new NewsItemWidget(settings, urlString, sourceLabel, title);
               headlines.append(currentItemWidget);
            }

        }

        reader.readNext();
    }

    reply->deleteLater();
    reloadHeadlines();
}

/**
 * Displays the headlines onto the columns
 */
void News::reloadHeadlines()
{
    int size = headlines.size();
    if (size == 0) return;
    qDebug() << "Total Size " << size << endl;
    //Initialize an array to keep track of which items were already inserted into the view
    bool* inserted = new bool[size];
    for (int i = 0; i < size; ++i)
    {
        inserted[i] = false;
    }

    bool done = false;
    int columnToInsert = 1;
    int c1 = 0;
    int c2 = 0;
    int c3 = 0;
    int insertedCount = 0;
    while(!done)
    {
        int indexOfNewItem = qrand() % size;
        if (!inserted[indexOfNewItem])
        {
            if(columnToInsert == 1)
            {
                firstColumn->addWidget(headlines.at(indexOfNewItem));
                c1++;
            }
            else if (columnToInsert == 2)
            {
                secondColumn->addWidget(headlines.at(indexOfNewItem));
                c2++;
            }
            else if (columnToInsert == 3)
            {
                thirdColumn->addWidget(headlines.at(indexOfNewItem));
                c3++;
            }
            inserted[indexOfNewItem] = true;
            columnToInsert++;
            if (columnToInsert > 3) columnToInsert = 1;
            insertedCount++;
        }

        if (insertedCount == size || insertedCount > 26)
        {
            done = true;
        }
    }
}

void News::clearLayout(QLayout *layout)
{
     QLayoutItem *item;
     while ((item = layout->takeAt(0)))
     {
        delete item->widget();
        delete item;
    }
}
