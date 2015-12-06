#ifndef BUDDYLIST_H
#define BUDDYLIST_H

#include <QWidget>
#include <QSettings>

class BuddyList : public QWidget
{
    Q_OBJECT

public:
    explicit BuddyList(QSettings* p, QWidget *parent = 0);
private:
    void init(QSettings* p);
};

#endif // BUDDYLIST_H
