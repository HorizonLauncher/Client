#include "NewsItemWidget.h"
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
                "font-family: SourceSansPro;"
                "font-size: 16px;"
                "}"
                "*:hover { background: "  + p->value("Primary/HoverSelection").toString() + "}");

    QVBoxLayout* layout = new QVBoxLayout(this);

    titleLabel = new QLabel (title);
    titleLabel->setWordWrap(true);
    layout->addWidget(titleLabel);

    descriptionLabel = new QLabel(descriptionString);
    descriptionLabel->setWordWrap(true);
    descriptionLabel->setStyleSheet("font-size: 12px");
    layout->addWidget(descriptionLabel);

    sourceLabel = new QLabel (source);
    sourceLabel->setWordWrap(true);
    sourceLabel->setStyleSheet("font-size: 10px;");
    layout->addWidget(sourceLabel);

    this->urlString = URL;
    this->descriptionString = descriptionString;
}

void NewsItemWidget::mousePressEvent(QMouseEvent *)
{
    QDesktopServices::openUrl(QUrl(this->urlString));
}
