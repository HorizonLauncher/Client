#ifndef NEWSITEM_WIDGET_H
#define NEWSITEM_WIDGET_H

#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QSettings>
class NewsItemWidget : public QWidget
{
    Q_OBJECT
public:
    explicit NewsItemWidget(QSettings* p, QString URL, QString source, QString title, QWidget *parent = 0);
    void mousePressEvent(QMouseEvent *) override;

signals:

public slots:

private:
    QLabel* titleLabel;
    QLabel* sourceLabel;
    QString urlString;
};

#endif // NEWSITEM_WIDGET_H
