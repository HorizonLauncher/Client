#include "newsitemwidget.h"
#include <QVBoxLayout>

NewsItemWidget::NewsItemWidget(QSettings* p, QWidget *parent) : QWidget(parent)
{
    QVBoxLayout* layout = new QVBoxLayout(this);
    this->setStyleSheet(
                "QLabel {"
                "color: " + p->value("Primary/DarkText").toString() + ";"
                "font-family: SourceSansPro;"
                "}"
                );
    titleLabel = new QLabel ("1 ");
    contentLabel = new QLabel ("1 ");
    contentLabel->setWordWrap(true);
    layout->addWidget(titleLabel);
    layout->addWidget(contentLabel);
    this->setLayout(layout);
}

