#include "Store.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QUrl>
#include <QPushButton>

Store::Store(QSettings* p, QWidget* parent)
    : QWidget(parent)
{
    this->setStyleSheet("color: "+ p->value("Primary/LightText").toString() + ";");

    init();
}

void Store::init()
{
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setMargin(0);
    QWidget* controlsWidget = new QWidget();
    layout->addWidget(controlsWidget);

    QHBoxLayout* controlsLayout = new QHBoxLayout(controlsWidget);
    //controlsLayout->setMargin(5);

    QPixmap back(":/SystemMenu/Icons/Material/NavArrowLeft.svg");
    QIcon backIcon(back);
    QPixmap forward(":/SystemMenu/Icons/Material/NavArrowRight.svg");
    QIcon forwardIcon(forward);

    QPushButton* backBtn = new QPushButton("");
    backBtn->setIcon(backIcon);
    backBtn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    controlsLayout->addWidget(backBtn);

    QPushButton* forwardBtn = new QPushButton("");
    forwardBtn->setIcon(forwardIcon);
    forwardBtn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    controlsLayout->addWidget(forwardBtn);

    urlLabel = new QLabel();
    urlLabel->setAlignment(Qt::AlignHCenter);
    urlLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    controlsLayout->addWidget(urlLabel);

    webView = new QWebView();
    webView->setContentsMargins(0,0,0,0);
    webView->load(QUrl("http://store.launchhorizon.com/"));
    webView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    layout->addWidget(webView);

    connect(webView, &QWebView::loadStarted, this, &Store::changeUrlLabel);
    connect(webView, &QWebView::loadProgress, this, &Store::changeUrlLabel);
    connect(webView, &QWebView::loadFinished, this, &Store::changeUrlLabel);

    connect(backBtn, &QPushButton::clicked, webView, &QWebView::back);
    connect(forwardBtn, &QPushButton::clicked, webView, &QWebView::forward);
}

void Store::changeUrlLabel()
{
    urlLabel->setText(webView->url().toString());
}
