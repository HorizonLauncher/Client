#include "DRMSetupWizard.h"
#include "AddGameWizard.h"
#include "SteamHelper.h"
#include "Library.h"

/** DRMSetupWizard constructor
 */
DRMSetupWizard::DRMSetupWizard(QWidget* parent, Qt::WindowFlags flags)
{
    addPage(new StartPage());
    addPage(new DRMsFoundPage());
    addPage(new GamesFoundPage());

    setWindowTitle(tr("DRM Setup Wizard"));
}

StartPage::StartPage(QWidget* parent)
{
    setTitle(tr("Welcome"));

    QVBoxLayout* layout = new QVBoxLayout(this);

    QLabel* label = new QLabel(tr("This wizard will help you add all of your games to Horizon."));
    label->setWordWrap(true);
    layout->addWidget(label);
}

DRMsFoundPage::DRMsFoundPage(QWidget* parent)
{
    setTitle(tr("DRMs Found"));

    QVBoxLayout* layout = new QVBoxLayout(this);

    QString steamFoundText;
    if (SteamHelper::doesExist())
    {
        steamFoundText = tr("Steam was found.");
    }
    else
    {
        steamFoundText = tr("Steam was not found.");
    }
    QLabel* label = new QLabel(steamFoundText);
    label->setWordWrap(true);
    layout->addWidget(label);
}

GamesFoundPage::GamesFoundPage(QWidget* parent)
{
    setTitle(tr("Games Found"));

    QVBoxLayout* layout = new QVBoxLayout(this);

    QLabel* infoLabel = new QLabel(tr("The following games will be added:"));
    infoLabel->setStyleSheet("font-size: 14px; font-weight: 600;");
    layout->addWidget(infoLabel);

    QWidget* gamesWidget = new QWidget();
    gamesLayout = new QVBoxLayout(gamesWidget);

    QScrollArea* scrollArea = new QScrollArea();
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(gamesWidget);
    scrollArea->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    layout->addWidget(scrollArea);
}

void GamesFoundPage::initializePage()
{
    QMap<QString, QString> gamesList = SteamHelper::getGames();

    for (QString game : gamesList.keys())
    {
        if (Library::db.isExistant(game).first) { continue; }
        addGames[game] = gamesList[game];
        gamesLayout->addWidget(new QLabel(game));
    }
}

bool GamesFoundPage::validatePage()
{
    for (QString game : addGames.keys())
    {
        #if defined(__linux__)
            QString openWith = "xdg-open";
        #elif defined(_WIN32) || defined(_WIN64)
            QString openWith = "explorer.exe";
        #elif defined(__APPLE__)
            QString openWith = "open";
        #endif
        Library::db.addGame(game, ".", openWith, "steam://run/" + addGames[game], 1);
    }
}
