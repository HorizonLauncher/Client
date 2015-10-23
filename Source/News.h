#ifndef NEWS_H
#define NEWS_H

#include <QWidget>
#include <QSettings>

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


namespace Ui {
class News;
}

class News : public QWidget
{
    Q_OBJECT

public:
    explicit News(QSettings* p, QWidget* parent = 0);
    ~News();

public slots:
    void onFetchComplete();
    void errorOccured(QNetworkReply::NetworkError);

private:
    Ui::News *ui;
    void loadXML();
};

#endif // News_H
