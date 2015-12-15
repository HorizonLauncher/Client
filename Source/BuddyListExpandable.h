#ifndef _BUDDYLISTEXPANDABLE_H_
#define _BUDDYLISTEXPANDABLE_H_

#include <QWidget>
#include <QSettings>
#include <QBoxLayout>

/** BuddyList Expandable class.
 * Expandable "sections" that can open and close
*/
class BuddyListExpandable : public QWidget
{
    Q_OBJECT

public:
    BuddyListExpandable(QString sectionName, bool expanded, QSettings* p, QWidget* parent = 0);

    void mousePressEvent(QMouseEvent* event);

    void addWidget(QWidget* widget);
    void expand();
    void collapse();

    bool isExpanded;

private:
    void init();

    QVBoxLayout* coreLayout;
    QWidget* children;
    QVBoxLayout* childrenLayout;

signals:
    void expanded();
    void collapsed();
};

#endif
