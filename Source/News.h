#ifndef NEWS_H
#define NEWS_H

#include <QWidget>
#include <QSettings>
#include <QVector>
#include <QVBoxLayout>
#include <QHBoxLayout>

/*
 * Networking headers for consuming RSS
 */
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QByteArray>
#include <QUrl>

/*
 * XML Headers
 *
 */

#include <qxmlstream.h>

/*
 * Custom Widget Headers
 */

#include "NewsItemWidget.h"

class News : public QWidget
{
    Q_OBJECT

public:
    explicit News(QSettings* p, QWidget* parent = 0);
    ~News();

public slots:
    void loadFeeds();

private:
    void loadXMLfromUrls();
    void setupUI();
    void reloadHeadlines();
    void loadFeedUrlsFromSettings();
    void clearLayout(QLayout* layout);
    void onFetchComplete(QNetworkReply *reply, QString sourceLabel);
    QSettings* settings;
    QVector<NewsItemWidget*> headlines;
    QVector<QString> feedUrls;

    //Layout properties
    QVBoxLayout* firstColumn;
    QVBoxLayout* secondColumn;
    QVBoxLayout* thirdColumn;
    QHBoxLayout* mainLayout;
};

#endif // News_H
