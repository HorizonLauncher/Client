#ifndef SETUPWIZARD_H
#define SETUPWIZARD_H

#include <QWizard>
#include <QtWidgets>

/** SetupWizard Class
 * Class to be shown when the user first starts Horizon.
*/

class SetupWizard : public QWizard
{
    Q_OBJECT

public:
    SetupWizard(QWidget* parent = 0, Qt::WindowFlags flags = 0);
};

class WelcomePage : public QWizardPage
{
    Q_OBJECT

public:
    WelcomePage(QWidget* parent = 0);
};

class AddGamePage : public QWizardPage
{
    Q_OBJECT

public:
    AddGamePage(QWidget* parent = 0);

private slots:
    void openAddGameWizard();
    void openDRMWizard();
};

class AddNewsFeedsPage : public QWizardPage
{
    Q_OBJECT

public:
    AddNewsFeedsPage(QWidget* parent = 0);
    bool validatePage() Q_DECL_OVERRIDE;

private:
    QMap<QCheckBox*, QString> checkboxes;
};
#endif
