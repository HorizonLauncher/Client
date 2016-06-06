#include "buddylist.h"

#include <QLabel>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QLineEdit>

BuddyList::BuddyList(QSettings* p, QWidget *parent) :
    QWidget(parent)
{
    this->setStyleSheet("background-color: " + p->value("Primary/SecondaryBase").toString() + ";} "
                        "color: " + p->value("Primary/LightText").toString() + ";");

    init(p);
}

void BuddyList::init(QSettings* p)
{
    QVBoxLayout* vLayout = new QVBoxLayout(this);

    QWidget* userDetails = new QWidget();
    userDetails->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    vLayout->addWidget(userDetails, 0, Qt::AlignTop);

    QGridLayout* userDetailsLayout = new QGridLayout(userDetails);

    QWidget* profilePicWidget = new QWidget();
    profilePicWidget->setStyleSheet("border-image: url(:/res/Images/ProfilePlaceholder.png) 0 0 0 0 stretch stretch;");
    profilePicWidget->setMinimumSize(64, 64);
    profilePicWidget->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    userDetailsLayout->addWidget(profilePicWidget, 0, 0, Qt::AlignLeft);

    QWidget* userDetailsWidget = new QWidget();
    userDetailsLayout->addWidget(userDetailsWidget, 0, 1, Qt::AlignLeft);

    QVBoxLayout* userDetailsVLayout = new QVBoxLayout(userDetailsWidget);

    QLabel* usernameLabel = new QLabel(tr("TheDarocker"));
    usernameLabel->setStyleSheet("color: " + p->value("Primary/LightText").toString() + ";");
    userDetailsVLayout->addWidget(usernameLabel);

    QLabel* userStatus = new QLabel(tr("Online"));
    userStatus->setStyleSheet("color: " + p->value("Primary/LightText").toString() + ";");
    userDetailsVLayout->addWidget(userStatus);

    QLineEdit* friendSearchBox = new QLineEdit();
    friendSearchBox->setPlaceholderText(tr("Search friends"));
    friendSearchBox->setStyleSheet("color: " + p->value("Primary/LightText").toString() + ";");
    friendSearchBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    vLayout->addWidget(friendSearchBox, 0, Qt::AlignTop);

    ingame = new BuddyListExpandable(tr("In-game"), false, p);
    ingame->addWidget(new QLabel("topCyder"));
    vLayout->addWidget(ingame);

    online = new BuddyListExpandable(tr("Online"), true, p);
    online->addWidget(new QLabel("thomasross"));
    vLayout->addWidget(online);

    offline = new BuddyListExpandable(tr("Offline"), false, p);
    offline->addWidget(new QLabel("mySelfLuls"));
    vLayout->addWidget(offline);


    vLayout->addStretch();

    QLabel* onlineFriends = new QLabel(QString(tr("%1 friends online.")).arg(0));
    onlineFriends->setStyleSheet("color: " + p->value("Primary/LightText").toString() + ";");
    vLayout->addWidget(onlineFriends, 0, Qt::AlignBottom);

    setMinimumSize(260, 524);
}
