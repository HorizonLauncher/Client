#ifndef BUDDYLIST_H
#define BUDDYLIST_H

#include <QWidget>

class BuddyList : public QWidget
{
    Q_OBJECT

public:
    explicit BuddyList(QWidget *parent = 0);
private:
    void init();
};

#endif // BUDDYLIST_H
