#include "community.h"
#include "../buddylist.h"

#include <QSettings>
#include <QHBoxLayout>
#include <QPushButton>

Community::Community(QSettings* p, QWidget* parent) :
    QWidget(parent)
{
    this->setObjectName("communityUI");
    this->setStyleSheet("background-color: " + p->value("Primary/SecondaryBase").toString() + ";} "
                        "QPushButton {"
                        "color: " + p->value("Primary/LightText").toString() + "; "
                        "background-color: " + p->value("Primary/DarkElement").toString() + "; "
                        "border: none; margin: 0px; padding: 0px;} "
                        "QPushButton:hover {"
                        "background-color: " + p->value("Primary/InactiveSelection").toString() + ";} "
                        "color: " + p->value("Primary/LightText").toString() + ";");

    init(p);
}

void Community::init(QSettings* p)
{
    QHBoxLayout* mainLayout = new QHBoxLayout(this);

    QFont buttonFont("SourceSansPro", 12);
    QPushButton* buddyButton = new QPushButton(tr("Friends"));
    buddyButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    buddyButton->setStyleSheet("padding: 5px;");
    buddyButton->setFont(buttonFont);
    mainLayout->addWidget(buddyButton, Qt::AlignCenter);
    connect(buddyButton, &QPushButton::clicked, [=] { openBuddyList(p); });
}

void Community::openBuddyList(QSettings* p)
{
    QWidget *buddyL = new BuddyList(p);
    buddyL->show();
}
