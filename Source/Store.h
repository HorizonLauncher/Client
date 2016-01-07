#ifndef _STORE_H_
#define _STORE_H_

#include <QWidget>
#include <QWebView>
#include <QLabel>
#include <QSettings>

class Store : public QWidget
{
    Q_OBJECT

public:
    Store(QSettings* p, QWidget* parent = 0);

private:
    void init();
    QLabel* urlLabel;
    QWebView* webView;

private slots:
    void changeUrlLabel();
};

#endif
