#ifndef STORE_H
#define STORE_H

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
