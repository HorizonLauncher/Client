#ifndef DRMSETUPWIZARD_H
#define DRMSETUPWIZARD_H

#include <QWizard>
#include <QtWidgets>

/** DRMSetupWizard Class
 * Class help the user add all of their games to Horizon
*/

class DRMSetupWizard : public QWizard
{
    Q_OBJECT

public:
    DRMSetupWizard(QWidget* parent = 0, Qt::WindowFlags flags = 0);
};

class StartPage : public QWizardPage
{
    Q_OBJECT

public:
    StartPage(QWidget* parent = 0);
};

class DRMsFoundPage : public QWizardPage
{
    Q_OBJECT

public:
    DRMsFoundPage(QWidget* parent = 0);
};

class GamesFoundPage : public QWizardPage
{
    Q_OBJECT

public:
    GamesFoundPage(QWidget* parent = 0);

    bool validatePage() Q_DECL_OVERRIDE;

private:
    QVBoxLayout* gamesLayout;
    QMap<QString, QString> addGames;

protected:
    void initializePage() Q_DECL_OVERRIDE;
};

#endif
