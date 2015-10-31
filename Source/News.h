#ifndef NEWS_H
#define NEWS_H

#include <QWidget>
#include <QSettings>
#include <QVector>

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


namespace Ui {
class NewsPanel;
}

class News : public QWidget
{
    Q_OBJECT

public:
    explicit News(QSettings* p, QWidget* parent = 0);
    ~News();

public slots:
    void onFetchComplete();

private:
    Ui::NewsPanel *ui;
    void loadXML();
    void reloadHeadlines();
    QSettings* settings;
    QVector<NewsItemWidget*> headlines;
    QVector<QString> feedUrls;
};

#endif // News_H
