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
    explicit NewsItemWidget(QSettings* p, QWidget *parent);
    void mousePressEvent(QMouseEvent *) override;
    QLabel* titleLabel;
    QLabel* contentLabel;
    QString urlString;
    QString source;

signals:

public slots:

private:
};

#endif // NEWSITEM_WIDGET_H
