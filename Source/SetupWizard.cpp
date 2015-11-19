#include "SetupWizard.h"
#include "AddGameWizard.h"
#include "DRMSetupWizard.h"

/** SetupWizard constructor
 */
SetupWizard::SetupWizard(QWidget* parent, Qt::WindowFlags flags)
{
    addPage(new WelcomePage());
    addPage(new AddGamePage());

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
