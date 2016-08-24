#ifndef BUDDYLIST_H
#define BUDDYLIST_H

#include "buddylistexpandable.h"

#include <QWidget>
#include <QSettings>

class BuddyList : public QWidget
{
    Q_OBJECT

public:
    explicit BuddyList(QSettings* p, QWidget* parent = 0);
private:
    void init(QSettings* p);
    BuddyListExpandable* online;
    BuddyListExpandable* ingame;
    BuddyListExpandable* offline;
};

#endif // BUDDYLIST_H
