#include "Store.h"

#include <QVBoxLayout>
#include <QUrl>

Store::Store(QWidget* parent)
    : QWidget(parent)
{
    init();
}

void Store::init()
{
    QVBoxLayout* layout = new QVBoxLayout(this);

    webView = new QWebView();
    webView->load(QUrl("http://store.launchhorizon.com/"));
    layout->addWidget(webView);
}
