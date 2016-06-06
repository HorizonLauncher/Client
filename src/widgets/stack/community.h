#ifndef COMMUNITY_H
#define COMMUNITY_H

#include <QWidget>
#include <QSettings>

class Community : public QWidget
{
    Q_OBJECT

public:
    explicit Community(QSettings* p, QWidget* parent = 0);

private slots:
    void openBuddyList(QSettings* p);

private:
    void init(QSettings* p);
};

#endif // COMMUNITY_H
