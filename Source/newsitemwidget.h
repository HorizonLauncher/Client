#ifndef NEWSITEMWIDGET_H
#define NEWSITEMWIDGET_H

#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QSettings>
class NewsItemWidget : public QWidget
{
    Q_OBJECT
public:
    explicit NewsItemWidget(QSettings* p, QWidget *parent);
    QLabel* titleLabel;
    QLabel* contentLabel;

signals:

public slots:
};    

#endif // NEWSITEMWIDGET_H
