#ifndef _STORE_H_
#define _STORE_H_

#include <QWidget>

class Store : public QWidget
{
    Q_OBJECT

public:
    Store(QWidget* parent = 0);

private:
    void init();
    QWebView* webView;
};

#endif
