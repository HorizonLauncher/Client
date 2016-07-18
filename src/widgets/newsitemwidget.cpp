#include "newsitemwidget.h"

#include <QVBoxLayout>
#include <QDesktopServices>
#include <QUrl>

/**
 * NewsItemWidget constructor
 * \param p The color palette for the UI
 * \param URL The URL to the RSS article
 * \param source The RSS feed the headline came from
 * \param title The headline to be displayed
 * \param parent The parent QWidget
 */
NewsItemWidget::NewsItemWidget(QSettings* p, QString URL, QString source, QString title, QString descriptionString, QWidget* parent) : QWidget(parent)
{
    this->setStyleSheet(
                "QLabel {"
                "color: " + p->value("Primary/LightText").toString() + ";"
                "font-family: Arial;"
                "font-size: 16px;"
                "}"
                );

    this->palette = p;

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setSpacing(0);
    imageLabel = new QLabel (""); //TODO: Change this to default image
    imageLabel->setContentsMargins(5,5,5,5);
    layout->addWidget(imageLabel);


    titleLabel = new QLabel (title);
    titleLabel->setContentsMargins(0,0,5,5);
    titleLabel->setWordWrap(true);
    layout->addWidget(titleLabel);

    descriptionLabel = new QLabel(descriptionString);
    descriptionLabel->setContentsMargins(0,0,5,5);
    descriptionLabel->setWordWrap(true);
    descriptionLabel->setStyleSheet("font-size: 12px");
    layout->addWidget(descriptionLabel);

    sourceLabel = new QLabel (source);
    sourceLabel->setContentsMargins(0,0,5,5);
    sourceLabel->setWordWrap(true);
    sourceLabel->setStyleSheet("font-size: 10px;");
    layout->addWidget(sourceLabel);

    this->urlString = URL;
    this->descriptionString = descriptionString;

    //Load thumbnail
    manager = new QNetworkAccessManager(this);
    const QUrl url(this->urlString);
    QNetworkRequest* req = new QNetworkRequest(url);
    req->setRawHeader("User-Agent", "Horizon Launcher");
    QNetworkReply* reply = manager->get(*req);
    connect (reply, &QNetworkReply::finished, [=] {
        if(reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() == 301)
        {
          QString newUrl = reply->attribute(QNetworkRequest::RedirectionTargetAttribute).toString();
          QNetworkRequest* req = new QNetworkRequest(newUrl);
          req->setRawHeader("User-Agent", "Horizon Launcher");
          QNetworkReply* newReply = manager->get(*req);
          connect(newReply, &QNetworkReply::finished, [=] {
              sourceCodeFetchComplete(newReply);
          });
        }

        else
        {
            sourceCodeFetchComplete(reply);
        }
    });
}

void NewsItemWidget::mousePressEvent(QMouseEvent *)
{
    QDesktopServices::openUrl(QUrl(this->urlString));
}

/**
 * Called when the source HTML is fetched for a webpage.
 * Retrieves the thumbnail for the page using the OpenGraph image property, if
 * defined in the source
 * @param reply The network reply containing the HTML source
 */
void NewsItemWidget::sourceCodeFetchComplete(QNetworkReply* reply)
{
    QString htmlSource = reply->readAll();

    //TODO: Get rid of magic numbers here LEARN REGEX
    int startIndex = htmlSource.indexOf("og:image") + 19;
    int endIndex = htmlSource.indexOf("jpg", startIndex);
    QString imgUrl =  htmlSource.mid(startIndex, endIndex - startIndex + 3);

    const QUrl url(imgUrl);
    QNetworkRequest* req = new QNetworkRequest(url);
    req->setRawHeader("User-Agent", "Horizon Launcher");
    QNetworkReply* imageReply = manager->get(*req);
    connect (imageReply, &QNetworkReply::finished, [=]
    {
        QByteArray jpegData = imageReply->readAll();
        QPixmap pixmap;
        pixmap.loadFromData(jpegData);
        if (!pixmap) { return; }
        imageLabel->setPixmap(pixmap.scaledToWidth(imageLabel->width()));
    });
}

void NewsItemWidget::enterEvent(QEvent* event)
{
   titleLabel->setStyleSheet("background-color: " + this->palette->value("Primary/HoverSelection").toString() + ";");
   descriptionLabel->setStyleSheet("font-size: 12px;"
                                   "background-color: " + this->palette->value("Primary/HoverSelection").toString() + ";");
   sourceLabel->setStyleSheet("font-size: 10px; "
                              "background-color: " + this->palette->value("Primary/HoverSelection").toString() + ";");
   imageLabel->setStyleSheet("background-color: " + this->palette->value("Primary/HoverSelection").toString() + ";");
   this->setCursor(Qt::PointingHandCursor);

}

void NewsItemWidget::leaveEvent(QEvent* event)
{
   titleLabel->setStyleSheet("background-color: " + this->palette->value("Body/Background").toString() + ";");
   descriptionLabel->setStyleSheet("font-size: 12px;"
                                   "background-color: " + this->palette->value("Body/Background").toString() + ";");
   sourceLabel->setStyleSheet("font-size: 10px; "
                              "background-color: " + this->palette->value("Body/Background").toString() + ";");
   imageLabel->setStyleSheet("background-color: " + this->palette->value("Body/Background").toString() + ";");
   this->setCursor(Qt::ArrowCursor);

}
