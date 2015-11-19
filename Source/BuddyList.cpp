#include "BuddyList.h"

#include <QLabel>
#include <QVBoxLayout>

BuddyList::BuddyList(QWidget *parent) :
    QWidget(parent)
{
    init();
}

void BuddyList::init()
{
    QVBoxLayout* vLayout = new QVBoxLayout(this);

    QLabel* wipLabel = new QLabel(tr("WIP"));
    vLayout->addWidget(wipLabel, 0, Qt::AlignHCenter);

    setMinimumSize(260, 524);
}
