#include "BuddyListExpandable.h"

#include <QLabel>
#include <QMouseEvent>
#include <QMenu>

BuddyListExpandable::BuddyListExpandable(QString sectionName, bool expanded, QSettings* p, QWidget* parent)
    : QWidget(parent)
{
    this->setMinimumHeight(20);
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    coreLayout = new QVBoxLayout(this);

    QWidget* titleBar = new QWidget();
    titleBar->setStyleSheet("background-color: " + p->value("Primary/TertiaryBase").toString() + ";");
    titleBar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    titleBar->setMinimumHeight(20);
    coreLayout->addWidget(titleBar);

    QVBoxLayout* titleLayout = new QVBoxLayout(titleBar);

    QLabel* title = new QLabel(sectionName);
    title->setStyleSheet("color: " + p->value("Primary/LightText").toString() + ";");
    titleLayout->addWidget(title);

    children = new QWidget();
    coreLayout->addWidget(children);
    children->hide();

    childrenLayout = new QVBoxLayout(children);

    if (expanded)
    {
        this->expand();
    }
}

/** Overridden mouse press event.
* Emits the click signal for this tab.
*/
void BuddyListExpandable::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
    {
        if (this->isExpanded)
        {
            this->collapse();
        }
        else
        {
            this->expand();
        }
    }
}

void BuddyListExpandable::addWidget(QWidget* widget)
{
    childrenLayout->addWidget(widget);
}

void BuddyListExpandable::expand()
{
    this->isExpanded = true;
    children->show();
}

void BuddyListExpandable::collapse()
{
    this->isExpanded = false;
    children->hide();
}
