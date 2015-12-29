#include "NewsItemWidget.h"
#include <QVBoxLayout>
#include <QDesktopServices>
#include <QUrl>

/**
 * NewsItemWidget constructor
 * \param p The color palette for the UI
 * \param URL The URL to the RSS article
 * \param source The RSS feed the headline came from (unused)
 * \param title The headline to be displayed
 * \param parent The parent QWidget
 */
NewsItemWidget::NewsItemWidget(QSettings* p, QString URL, QString source, QString title, QWidget* parent) : QWidget(parent)
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

    sourceLabel = new QLabel (source);
    sourceLabel->setWordWrap(true);
    //layout->addWidget(titleLabel); TODO: Add a source to the widget

    this->urlString = URL;
}


void NewsItemWidget::mousePressEvent(QMouseEvent *)
{
    QDesktopServices::openUrl(QUrl(this->urlString));
}
