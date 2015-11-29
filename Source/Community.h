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
    void openBuddyList();

private:
    void init();
};

#endif // COMMUNITY_H
