#include "newsitemwidget.h"
#include <QVBoxLayout>
#include <QDesktopServices>
#include <QUrl>

NewsItemWidget::NewsItemWidget(QSettings* p, QWidget *parent) : QWidget(parent)
{
    QVBoxLayout* layout = new QVBoxLayout(this);
    this->setStyleSheet(
                "QLabel {"
                "color: " + p->value("Primary/LightText").toString() + ";"
                "font-family: SourceSansPro;" +
                "}" +

                "QLabel: hover { background-color: yellow } "
                );
    titleLabel = new QLabel ("1 ");
    titleLabel->setWordWrap(true);
    contentLabel = new QLabel ("1 ");
    contentLabel->setWordWrap(true);
    layout->addWidget(titleLabel);
   // layout->addWidget(contentLabel);
    this->setLayout(layout);
    source = "";
}


void NewsItemWidget::mousePressEvent(QMouseEvent *) {

    QDesktopServices::openUrl(QUrl(this->urlString));

}

