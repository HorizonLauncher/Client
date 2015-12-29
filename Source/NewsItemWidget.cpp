#include "NewsItemWidget.h"
#include <QVBoxLayout>
#include <QDesktopServices>
#include <QUrl>

/**
* NewsItemWidget constructor
* \param p The color palette for the UI
* \param parent The parent QWidget
*/
NewsItemWidget::NewsItemWidget(QSettings* p, QWidget* parent = 0) : QWidget(parent)
{
    QVBoxLayout* layout = new QVBoxLayout(this);
    this->setStyleSheet(
                "QLabel {"
                "color: " + p->value("Primary/LightText").toString() + ";"
                "font-family: SourceSansPro;"
                "font-size: 16px;"
                "}"
                "*:hover { background: "  + p->value("Primary/HoverSelection").toString() + "}"
                );
    titleLabel = new QLabel ("1 ");
    titleLabel->setWordWrap(true);
    contentLabel = new QLabel ("1 ");
    contentLabel->setWordWrap(true);
    layout->addWidget(titleLabel);
    this->setLayout(layout);
    source = "";
}


void NewsItemWidget::mousePressEvent(QMouseEvent *)
{
    QDesktopServices::openUrl(QUrl(this->urlString));
}
