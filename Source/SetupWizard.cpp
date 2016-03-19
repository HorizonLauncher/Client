#include "SetupWizard.h"
#include "AddGameWizard.h"
#include "DRMSetupWizard.h"

#include <QScrollArea>

/** SetupWizard constructor
 */
SetupWizard::SetupWizard(QWidget* parent, Qt::WindowFlags flags)
{
    addPage(new WelcomePage());
    addPage(new AddGamePage());
    addPage(new AddNewsFeedsPage());

    setWindowTitle(tr("Horizon Setup Wizard"));
}

WelcomePage::WelcomePage(QWidget* parent)
{
    setTitle("Welcome");

    QLabel* label = new QLabel(tr("This wizard will help you set up Horizon."));
    label->setWordWrap(true);

    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(label);

    setLayout(layout);
}

void AddGamePage::openAddGameWizard()
{
    AddGameWizard* wiz = new AddGameWizard();
    wiz->show();
}

void AddGamePage::openDRMWizard()
{
    DRMSetupWizard* wiz = new DRMSetupWizard();
    wiz->show();
}

AddGamePage::AddGamePage(QWidget* parent)
{
    setTitle(tr("Add your games"));

    QPushButton* addGameBtn = new QPushButton(tr("Add game"));
    QPushButton* addDRMBtn = new QPushButton(tr("Add DRM"));

    QLabel* addGameLbl = new QLabel(tr("Use this for adding games that aren't "
    "attached to a DRM."));
    QLabel* addDRMLbl = new QLabel(tr("Use this for adding games that are managed "
    "by a DRM, such as Steam.\n"
    "The wizard will attempt to automatically add all games found.\n"
    "Supported: Steam, uPlay, Origin"));

    addGameBtn->setFixedSize(80, 20);
    addDRMBtn->setFixedSize(80, 20);

    connect(addGameBtn, &QPushButton::clicked, this, &AddGamePage::openAddGameWizard);
    connect(addDRMBtn, &QPushButton::clicked, this, &AddGamePage::openDRMWizard);

    QGridLayout* layout = new QGridLayout;
    layout->addWidget(addGameBtn, 0, 0, Qt::AlignLeft);
    layout->addWidget(addGameLbl, 0, 1, Qt::AlignLeft);
    layout->addWidget(addDRMBtn, 1, 0, Qt::AlignLeft);
    layout->addWidget(addDRMLbl, 1, 1, Qt::AlignLeft);

    setLayout(layout);
}

AddNewsFeedsPage::AddNewsFeedsPage(QWidget* parent)
{
    setTitle(tr("Add News Feeds"));

    QWidget* contentWidget = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(contentWidget);

    QLabel* label = new QLabel(tr("Choose what news feeds you would like on "
                                  "your news page. You can change these at any "
                                  "time"));
    label->setWordWrap(true);
    layout->addWidget(label);

    QPushButton* selectAllBtn = new QPushButton(tr("Select all"));
    selectAllBtn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    QPushButton* selectNoneBtn = new QPushButton(tr("Select none"));
    selectNoneBtn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    connect(selectAllBtn, &QPushButton::clicked, [=]
    {
        QMap<QCheckBox*, QString>::const_iterator i = checkboxes.constBegin();
        while (i != checkboxes.constEnd())
        {
            QCheckBox* checkbox = i.key();
            checkbox->setChecked(true);
            ++i;
        }
    });

    connect(selectNoneBtn, &QPushButton::clicked, [=]
    {
        QMap<QCheckBox*, QString>::const_iterator i = checkboxes.constBegin();
        while (i != checkboxes.constEnd())
        {
            QCheckBox* checkbox = i.key();
            checkbox->setChecked(false);
            ++i;
        }
    });

    QHBoxLayout* selectBtns = new QHBoxLayout();
    selectBtns->addWidget(selectAllBtn);
    selectBtns->addWidget(selectNoneBtn);
    layout->addLayout(selectBtns);

    QList<QList<QString>> newsFeeds;
    newsFeeds.append(*new QList<QString>({"/r/launchhorizon", "https://www.reddit.com/r/launchhorizon/.rss"}));
    newsFeeds.append(*new QList<QString>({"/r/gaming", "https://www.reddit.com/r/gaming/.rss"}));
    newsFeeds.append(*new QList<QString>({"/r/pcgaming", "https://www.reddit.com/r/pcgaming/.rss"}));
    newsFeeds.append(*new QList<QString>({"/r/gaming4gamers", "https://www.reddit.com/r/gaming4gamers/.rss"}));
    newsFeeds.append(*new QList<QString>({"/r/games", "https://www.reddit.com/r/games/.rss"}));
    #ifdef __linux__
        newsFeeds.append(*new QList<QString>({"/r/linux_gaming", "https://www.reddit.com/r/pcgaming/.rss"}));
        newsFeeds.append(*new QList<QString>({"gamingonlinux.com", "https://www.gamingonlinux.com/article_rss.php"}));
        newsFeeds.append(*new QList<QString>({"Phoronix", "https://www.phoronix.com/rss.php"}));
    #endif
    newsFeeds.append(*new QList<QString>({"Gamasutra", "http://feeds.feedburner.com/GamasutraNews"}));
    newsFeeds.append(*new QList<QString>({"GameInformer", "http://www.gameinformer.com/feeds/thefeedrss.aspx"}));
    newsFeeds.append(*new QList<QString>({"Gamespot", "http://www.gamespot.com/feeds/mashup/"}));
    newsFeeds.append(*new QList<QString>({"Destructoid", "http://feeds.feedburner.com/Destructoid"}));
    newsFeeds.append(*new QList<QString>({"EuroGamer PC", "http://www.eurogamer.net/?format=rss&platform=PC"}));
    newsFeeds.append(*new QList<QString>({"Games Revolution", "http://www.gamerevolution.com/rss/rss_everything.xml"}));
    newsFeeds.append(*new QList<QString>({"GamesIndistry.biz", "http://www.gamesindustry.biz/rss/gamesindustry_news_feed.rss"}));
    newsFeeds.append(*new QList<QString>({"Indiegames.com", "http://feeds.feedburner.com/IndependentGaming"}));
    newsFeeds.append(*new QList<QString>({"Kotaku", "http://feeds.gawker.com/kotaku/full"}));
    newsFeeds.append(*new QList<QString>({"MP 1st", "http://mp1st.com/feed"}));
    newsFeeds.append(*new QList<QString>({"Nintendojo", "http://www.nintendojo.com/category/news/feed"}));
    newsFeeds.append(*new QList<QString>({"PC Gamer", "http://www.pcgamer.com/feed/rss2/"}));
    newsFeeds.append(*new QList<QString>({"PCGamesN", "http://www.pcgamesn.com/mainrss.xml"}));
    newsFeeds.append(*new QList<QString>({"Player Theory", "http://playertheory.com/rss"}));
    newsFeeds.append(*new QList<QString>({"Pocket Gamer", "http://www.pocketgamer.co.uk/rss.asp?t=news"}));
    newsFeeds.append(*new QList<QString>({"Polygon News", "http://www.polygon.com/rss/group/news/index.xml"}));
    newsFeeds.append(*new QList<QString>({"Rock, Paper, Shotgun", "http://feeds.feedburner.com/RockPaperShotgun"}));
    newsFeeds.append(*new QList<QString>({"Siliconera", "http://www.siliconera.com/feed/"}));
    newsFeeds.append(*new QList<QString>({"VG274", "https://www.vg247.com/feed/"}));


    for (QList<QString> feed : newsFeeds)
    {
        QCheckBox* checkbox = new QCheckBox(feed[0]);
        layout->addWidget(checkbox);

        checkboxes.insert(checkbox, feed[1]);
    }

    QScrollArea* scrollArea = new QScrollArea();
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(contentWidget);
    scrollArea->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    QVBoxLayout* baseLayout = new QVBoxLayout(this);
    baseLayout->addWidget(scrollArea);
}

bool AddNewsFeedsPage::validatePage()
{
    QList<QString> feeds;

    QSettings newsSettings("HorizonLauncher", "news");
    int size = newsSettings.beginReadArray("URLs");
    for (int i = 0; i < size; ++i)
    {
        newsSettings.setArrayIndex(i);
        QString current = newsSettings.value("url").toString();
        feeds.append(current);
    }
    newsSettings.endArray();

    QMap<QCheckBox*, QString>::const_iterator i = checkboxes.constBegin();
    while (i != checkboxes.constEnd())
    {
        QCheckBox* checkbox = i.key();
        QString feedURL = i.value();

        if (checkbox->isChecked())
        {
            feeds.append(feedURL);
        }
        ++i;
    }

    newsSettings.beginWriteArray("URLs");
    for (int i = 0; i < feeds.size(); ++i)
    {
        newsSettings.setArrayIndex(i);
        newsSettings.setValue("url", feeds[i]);
        qDebug() << feeds[i];
    }
    newsSettings.endArray();

    return true;
}
