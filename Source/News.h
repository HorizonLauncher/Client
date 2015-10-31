#ifndef NEWS_H
#define NEWS_H

#include <QWidget>
#include <QSettings>
#include <QVector>
#include <QVBoxLayout>

/*
 * Networking headers for consuming RSS
 *
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

#include "newsitemwidget.h"

class News : public QWidget
{
    Q_OBJECT

public:
    explicit News(QSettings* p, QWidget* parent = 0);
    ~News();

public slots:
    void onFetchComplete();

private:
    void loadXML();
    void setupUI();
    void reloadHeadlines();
    QSettings* settings;
    QVector<NewsItemWidget*> headlines;
    QVector<QString> feedUrls;

    //Layout properties
    QVBoxLayout* firstColumn;
    QVBoxLayout* secondColumn;
    QVBoxLayout* thirdColumn;
};

#endif // News_H