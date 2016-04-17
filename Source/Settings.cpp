#include "Settings.h"
#include "SetupWizard.h"
#include "NewsFeedChooserWindow.h"

#include <QTabWidget>

/** Settings constructor
* Initialize the settings UI
* \param p Inherited palette configuration for setting StyleSheets.
* \param parent Pointer to parent widget.
*/
Settings::Settings(QSettings* p, QWidget* parent) :
    QWidget(parent)
{
    this->setObjectName("settingsUI");
    this->setStyleSheet("background-color: " + p->value("Primary/SecondaryBase").toString() + ";} "
        "QLabel, QGroupBox {"
        "color: " + p->value("Primary/LightText").toString() + "; }"
        "QPushButton {"
        "color: " + p->value("Primary/LightText").toString() + "; "
        "background-color: " + p->value("Primary/DarkElement").toString() + "; "
        "border: none; margin: 0px; padding: 0px;} "
        "QPushButton:hover {"
        "background-color: " + p->value("Primary/InactiveSelection").toString() + ";} "
        "QCheckBox {"
        "border-color: white;"
        "color: " + p->value("Primary/ActiveElement").toString() + ";}"
        "color: " + p->value("Primary/LightText").toString() + ";");

    init(p);
}

void Settings::init(QSettings* p)
{
    QSettings config(QSettings::IniFormat, QSettings::UserScope, "HorizonLauncher", "config");

    QHBoxLayout* mainLayout = new QHBoxLayout(this);

    QFont buttonFont("SourceSansPro", 9);

    QWidget* sectionTabsWidget = new QWidget();
    sectionTabsWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    sectionTabsWidget->setStyleSheet("font-size: 10pt;");
    mainLayout->addWidget(sectionTabsWidget, Qt::AlignLeft | Qt::AlignTop);

    QVBoxLayout* sectionTabsLayout = new QVBoxLayout(sectionTabsWidget);

    QPushButton* clientSettingsLabel = new QPushButton(tr("General"));
    clientSettingsLabel->setStyleSheet("font-size: 12pt; background: none;");
    QPushButton* styleSettingsLabel = new QPushButton(tr("Appearance"));
    styleSettingsLabel->setStyleSheet("font-size: 12pt; background: none;");
    sectionTabsLayout->addSpacing(20);
    sectionTabsLayout->addWidget(clientSettingsLabel, Qt::AlignLeft);
    sectionTabsLayout->addWidget(styleSettingsLabel, Qt::AlignLeft);
    sectionTabsLayout->addStretch();

    stack = new QStackedWidget();
    stack->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    mainLayout->addWidget(stack, Qt::AlignHCenter);

    /*// USER SETTINGS GROUP
    QGroupBox* userGroup = new QGroupBox(tr("User Settings"));
    userGroup->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    stack->addWidget(userGroup);

    QVBoxLayout* userGroupLayout = new QVBoxLayout(userGroup);

    QPushButton* changeUsernameBtn = new QPushButton(tr("Change Username"));
    changeUsernameBtn->setStyleSheet("padding: 5px;");
    changeUsernameBtn->setFont(buttonFont);
    changeUsernameBtn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    userGroupLayout->addWidget(changeUsernameBtn, Qt::AlignTop);

    QPushButton* changePwdBtn = new QPushButton(tr("Change Password"));
    changePwdBtn->setStyleSheet("padding: 5px;");
    changePwdBtn->setFont(buttonFont);
    changePwdBtn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    userGroupLayout->addWidget(changePwdBtn, Qt::AlignTop);*/

    // CLIENT SETTINGS GROUP
    QGroupBox* clientGroup = new QGroupBox(tr("General"));
    clientGroup->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    stack->addWidget(clientGroup);

    QVBoxLayout* clientGroupLayout = new QVBoxLayout(clientGroup);

    QPushButton* setupWizardBtn = new QPushButton(tr("Open Setup Wizard"));
    setupWizardBtn->setStyleSheet("padding: 5px;");
    setupWizardBtn->setFont(buttonFont);
    setupWizardBtn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    clientGroupLayout->addWidget(setupWizardBtn);
    connect(setupWizardBtn, &QPushButton::clicked, [=]
    {
        SetupWizard* wiz = new SetupWizard();
        wiz->show();
    });

    QPushButton* clearDBBtn = new QPushButton(tr("Clear database"));
    clearDBBtn->setStyleSheet("padding: 5px;");
    clearDBBtn->setFont(buttonFont);
    clearDBBtn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    clientGroupLayout->addWidget(clearDBBtn);
    connect(clearDBBtn, &QPushButton::clicked, this, &Settings::confirmClearDb);

    QPushButton* clearLaunchBtn = new QPushButton(tr("Clear launch options"));
    clearLaunchBtn->setStyleSheet("padding: 5px;");
    clearLaunchBtn->setFont(buttonFont);
    clearLaunchBtn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    clientGroupLayout->addWidget(clearLaunchBtn);

    QPushButton* manageNewsFeedButton = new QPushButton(tr("Manage News Feeds"));
    manageNewsFeedButton->setStyleSheet("padding: 5px;");
    manageNewsFeedButton->setFont(buttonFont);
    manageNewsFeedButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    clientGroupLayout->addWidget(manageNewsFeedButton);

    connect (manageNewsFeedButton, &QPushButton::clicked, [&]
    {
        NewsFeedChooserWindow* window = new NewsFeedChooserWindow();
        connect(window, &NewsFeedChooserWindow::shouldRefreshRSSFeeds, this, &Settings::didUpdateFeedURLs);
        window->show();
    });

    QWidget* multipleGamesWidget = new QWidget();
    QHBoxLayout* multipleGamesLayout = new QHBoxLayout(multipleGamesWidget);
    QCheckBox* multipleGamesCB = new QCheckBox();
    multipleGamesCB->setCheckState((config.value("GameLauncher/MultipleExec").toBool() ? Qt::Checked : Qt::Unchecked));
    QLabel* multipleGamesLbl = new QLabel(tr("Allow launching of multiple games"));
    multipleGamesLayout->addWidget(multipleGamesCB);
    multipleGamesLayout->addWidget(multipleGamesLbl);
    multipleGamesWidget->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    clientGroupLayout->addWidget(multipleGamesWidget);
    connect(multipleGamesCB, &QCheckBox::stateChanged, [=] (int state) { setMultipleExec(state == Qt::Checked); });

    QWidget* fulltabWidget = new QWidget();
    QHBoxLayout* fulltabLayout = new QHBoxLayout(fulltabWidget);
    QCheckBox* fulltabCB = new QCheckBox();
    fulltabCB->setCheckState((p->value("TitleBar/Fulltab").toBool() ? Qt::Checked : Qt::Unchecked));
    QLabel* fulltabLbl = new QLabel(tr("Full-tab highlighting"));
    fulltabLayout->addWidget(fulltabCB);
    fulltabLayout->addWidget(fulltabLbl);
    //clientGroupLayout->addWidget(fulltabWidget);
    connect(fulltabCB, &QCheckBox::stateChanged, [=] (int state) { setFulltab(state == Qt::Checked); });

    // STYLE SETTINGS GROUP
    QGroupBox* styleGroup = new QGroupBox(tr("Appearance"));
    styleGroup->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    stack->addWidget(styleGroup);

    QGridLayout* styleGroupLayout = new QGridLayout(styleGroup);

    QWidget* bodyColorWidget = new QWidget();
    QHBoxLayout* bodyColorLayout = new QHBoxLayout(bodyColorWidget);
    bodyColor = new QPushButton();
    bodyColor->setStyleSheet("background-color: " + p->value("Body/Background").toString() + "; }");
    bodyColor->setMinimumSize(20, 20);
    bodyColor->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    bodyColorLayout->addWidget(bodyColor);
    bodyColorLayout->addWidget(new QLabel(tr("Body Background")));
    styleGroupLayout->addWidget(bodyColorWidget, 0, 0);

    QWidget* navbarBGWidget = new QWidget();
    QHBoxLayout* navbarBGLayout = new QHBoxLayout(navbarBGWidget);
    navbarBG = new QPushButton();
    navbarBG->setStyleSheet("background-color: " + p->value("Navbar/Background").toString() + "; }");
    navbarBG->setMinimumSize(20, 20);
    navbarBG->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    navbarBGLayout->addWidget(navbarBG);
    navbarBGLayout->addWidget(new QLabel(tr("Navbar Background")));
    styleGroupLayout->addWidget(navbarBGWidget, 1, 0);

    QWidget* navbarHoverWidget = new QWidget();
    QHBoxLayout* navbarHoverLayout = new QHBoxLayout(navbarHoverWidget);
    navbarHover = new QPushButton();
    navbarHover->setStyleSheet("background-color: " + p->value("Navbar/Hover").toString() + "; }");
    navbarHover->setMinimumSize(20, 20);
    navbarHover->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    navbarHoverLayout->addWidget(navbarHover);
    navbarHoverLayout->addWidget(new QLabel(tr("Navbar Hover")));
    styleGroupLayout->addWidget(navbarHoverWidget, 2, 0);

    QWidget* navbarSelectedWidget = new QWidget();
    QHBoxLayout* navbarSelectedLayout = new QHBoxLayout(navbarSelectedWidget);
    navbarSelected = new QPushButton();
    navbarSelected->setStyleSheet("background-color: " + p->value("Navbar/SelectedColor").toString() + "; }");
    navbarSelected->setMinimumSize(20, 20);
    navbarSelected->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    navbarSelectedLayout->addWidget(navbarSelected);
    navbarSelectedLayout->addWidget(new QLabel(tr("Navbar Selected")));
    styleGroupLayout->addWidget(navbarSelectedWidget, 3, 0);

    QWidget* titleBarColorWidget = new QWidget();
    QHBoxLayout* titleBarColorLayout = new QHBoxLayout(titleBarColorWidget);
    titleBarColor = new QPushButton();
    titleBarColor->setStyleSheet("background-color: " + p->value("TitleBar/Color").toString() + "; }");
    titleBarColor->setMinimumSize(20, 20);
    titleBarColor->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    titleBarColorLayout->addWidget(titleBarColor);
    titleBarColorLayout->addWidget(new QLabel(tr("Titlebar Color")));
    styleGroupLayout->addWidget(titleBarColorWidget, 0, 1);

    QWidget* activeElementWidget = new QWidget();
    QHBoxLayout* activeElementLayout = new QHBoxLayout(activeElementWidget);
    activeElement = new QPushButton();
    activeElement->setStyleSheet("background-color: " + p->value("Primary/ActiveElement").toString() + "; }");
    activeElement->setMinimumSize(20, 20);
    activeElement->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    activeElementLayout->addWidget(activeElement);
    activeElementLayout->addWidget(new QLabel(tr("Active Element")));
    styleGroupLayout->addWidget(activeElementWidget, 1, 1);

    QWidget* inactiveSelectionWidget = new QWidget();
    QHBoxLayout* inactiveSelectionLayout = new QHBoxLayout(inactiveSelectionWidget);
    inactiveSelection = new QPushButton();
    inactiveSelection->setStyleSheet("background-color: " + p->value("Primary/InactiveSelection").toString() + "; }");
    inactiveSelection->setMinimumSize(20, 20);
    inactiveSelection->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    inactiveSelectionLayout->addWidget(inactiveSelection);
    inactiveSelectionLayout->addWidget(new QLabel(tr("Inactive Selection")));
    styleGroupLayout->addWidget(inactiveSelectionWidget, 2, 1);

    QWidget* hoverSelectionWidget = new QWidget();
    QHBoxLayout* hoverSelectionLayout = new QHBoxLayout(hoverSelectionWidget);
    hoverSelection = new QPushButton();
    hoverSelection->setStyleSheet("background-color: " + p->value("Primary/HoverSelection").toString() + "; }");
    hoverSelection->setMinimumSize(20, 20);
    hoverSelection->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    hoverSelectionLayout->addWidget(hoverSelection);
    hoverSelectionLayout->addWidget(new QLabel(tr("Hover Selection")));
    styleGroupLayout->addWidget(hoverSelectionWidget, 3, 1);

    QWidget* darkElementWidget = new QWidget();
    QHBoxLayout* darkElementLayout = new QHBoxLayout(darkElementWidget);
    darkElement = new QPushButton();
    darkElement->setStyleSheet("background-color: " + p->value("Primary/DarkElement").toString() + "; }");
    darkElement->setMinimumSize(20, 20);
    darkElement->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    darkElementLayout->addWidget(darkElement);
    darkElementLayout->addWidget(new QLabel(tr("Dark Element")));
    styleGroupLayout->addWidget(darkElementWidget, 0, 2);

    QWidget* lightTextWidget = new QWidget();
    QHBoxLayout* lightTextLayout = new QHBoxLayout(lightTextWidget);
    lightText = new QPushButton();
    lightText->setStyleSheet("background-color: " + p->value("Primary/LightText").toString() + "; }");
    lightText->setMinimumSize(20, 20);
    lightText->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    lightTextLayout->addWidget(lightText);
    lightTextLayout->addWidget(new QLabel(tr("Light Text")));
    styleGroupLayout->addWidget(lightTextWidget, 1, 2);

    QWidget* darkTextWidget = new QWidget();
    QHBoxLayout* darkTextLayout = new QHBoxLayout(darkTextWidget);
    darkText = new QPushButton();
    darkText->setStyleSheet("background-color: " + p->value("Primary/DarkText").toString() + "; }");
    darkText->setMinimumSize(20, 20);
    darkText->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    darkTextLayout->addWidget(darkText);
    darkTextLayout->addWidget(new QLabel(tr("Dark Text")));
    styleGroupLayout->addWidget(darkTextWidget, 2, 2);

    QWidget* subTextWidget = new QWidget();
    QHBoxLayout* subTextLayout = new QHBoxLayout(subTextWidget);
    subText = new QPushButton();
    subText->setStyleSheet("background-color: " + p->value("Primary/SubText").toString() + "; }");
    subText->setMinimumSize(20, 20);
    subText->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    subTextLayout->addWidget(subText);
    subTextLayout->addWidget(new QLabel(tr("Sub Text")));
    styleGroupLayout->addWidget(subTextWidget, 3, 2);

    QWidget* primaryBaseWidget = new QWidget();
    QHBoxLayout* primaryBaseLayout = new QHBoxLayout(primaryBaseWidget);
    primaryBase = new QPushButton();
    primaryBase->setStyleSheet("background-color: " + p->value("Primary/PrimaryBase").toString() + "; }");
    primaryBase->setMinimumSize(20, 20);
    primaryBase->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    primaryBaseLayout->addWidget(primaryBase);
    primaryBaseLayout->addWidget(new QLabel(tr("Primary Base")));
    styleGroupLayout->addWidget(primaryBaseWidget, 0, 3);

    QWidget* secondaryBaseWidget = new QWidget();
    QHBoxLayout* secondaryBaseLayout = new QHBoxLayout(secondaryBaseWidget);
    secondaryBase = new QPushButton();
    secondaryBase->setStyleSheet("background-color: " + p->value("Primary/SecondaryBase").toString() + "; }");
    secondaryBase->setMinimumSize(20, 20);
    secondaryBase->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    secondaryBaseLayout->addWidget(secondaryBase);
    secondaryBaseLayout->addWidget(new QLabel(tr("Secondary Base")));
    styleGroupLayout->addWidget(secondaryBaseWidget, 1, 3);

    QWidget* tertiaryBaseWidget = new QWidget();
    QHBoxLayout* tertiaryBaseLayout = new QHBoxLayout(tertiaryBaseWidget);
    tertiaryBase = new QPushButton();
    tertiaryBase->setStyleSheet("background-color: " + p->value("Primary/TertiaryBase").toString() + "; }");
    tertiaryBase->setMinimumSize(20, 20);
    tertiaryBase->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    tertiaryBaseLayout->addWidget(tertiaryBase);
    tertiaryBaseLayout->addWidget(new QLabel(tr("Tertiary Base")));
    styleGroupLayout->addWidget(tertiaryBaseWidget, 2, 3);

    QWidget* darkestBaseWidget = new QWidget();
    QHBoxLayout* darkestBaseLayout = new QHBoxLayout(darkestBaseWidget);
    darkestBase = new QPushButton();
    darkestBase->setStyleSheet("background-color: " + p->value("Primary/DarkestBase").toString() + "; }");
    darkestBase->setMinimumSize(20, 20);
    darkestBase->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    darkestBaseLayout->addWidget(darkestBase);
    darkestBaseLayout->addWidget(new QLabel(tr("Darkest Base")));
    styleGroupLayout->addWidget(darkestBaseWidget, 3, 3);

    QPushButton* resetColorsBtn = new QPushButton(tr("Reset Colors to Default"));
    resetColorsBtn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    resetColorsBtn->setStyleSheet("padding: 5px;");
    connect(resetColorsBtn, &QPushButton::clicked, this, &Settings::resetColors);
    styleGroupLayout->addWidget(resetColorsBtn, 4, 0);

    QPushButton* exportThemeBtn = new QPushButton(tr("Export Theme"));
    exportThemeBtn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    exportThemeBtn->setStyleSheet("padding: 5px;");
    connect(exportThemeBtn, &QPushButton::clicked, this, &Settings::exportTheme);
    styleGroupLayout->addWidget(exportThemeBtn, 4, 1);

    QPushButton* importThemeBtn = new QPushButton(tr("Import Theme"));
    importThemeBtn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    importThemeBtn->setStyleSheet("padding: 5px;");
    connect(importThemeBtn, &QPushButton::clicked, this, &Settings::importTheme);
    styleGroupLayout->addWidget(importThemeBtn, 4, 2);

    connect(bodyColor, &QPushButton::clicked, [=]() { pickSetColor(1); });
    connect(navbarBG, &QPushButton::clicked, [=]() { pickSetColor(2); });
    connect(navbarHover, &QPushButton::clicked, [=]() { pickSetColor(3); });
    connect(navbarSelected, &QPushButton::clicked, [=]() { pickSetColor(4); });
    connect(titleBarColor, &QPushButton::clicked, [=]() { pickSetColor(5); });
    connect(activeElement, &QPushButton::clicked, [=]() { pickSetColor(6); });
    connect(inactiveSelection, &QPushButton::clicked, [=]() { pickSetColor(7); });
    connect(hoverSelection, &QPushButton::clicked, [=]() { pickSetColor(8); });
    connect(darkElement, &QPushButton::clicked, [=]() { pickSetColor(9); });
    connect(lightText, &QPushButton::clicked, [=]() { pickSetColor(10); });
    connect(darkText, &QPushButton::clicked, [=]() { pickSetColor(11); });
    connect(subText, &QPushButton::clicked, [=]() { pickSetColor(12); });
    connect(primaryBase, &QPushButton::clicked, [=]() { pickSetColor(13); });
    connect(secondaryBase, &QPushButton::clicked, [=]() { pickSetColor(14); });
    connect(tertiaryBase, &QPushButton::clicked, [=]() { pickSetColor(15); });
    connect(darkestBase, &QPushButton::clicked, [=]() { pickSetColor(16); });


    connect(clientSettingsLabel, &QPushButton::clicked,
        [=]()
        {
            stack->setCurrentWidget(clientGroup);
            //clientSettingsLabel->
        });

    connect(styleSettingsLabel, &QPushButton::clicked,
        [=]()
        {
            stack->setCurrentWidget(styleGroup);
            //clientSettingsLabel->
        });
}

void Settings::resetColors()
{
    QSettings palette(QSettings::IniFormat, QSettings::UserScope, "HorizonLauncher", "palette");

    if (palette.isWritable())
    {
        palette.beginGroup("Primary");
        palette.setValue("ActiveElement", "#FFFFFF");
        palette.setValue("InactiveSelection", "#686868");
        palette.setValue("HoverSelection", "#ADADAD");
        palette.setValue("DarkElement", "#4D5250");
        palette.setValue("LightText", "#FFFFFF");
        palette.setValue("DarkText", "#242424");
        palette.setValue("SubText", "#B2B4B3");
        palette.setValue("PrimaryBase", "#282828");
        palette.setValue("SecondaryBase", "#1F1F1F");
        palette.setValue("TertiaryBase", "#131313");
        palette.setValue("DarkestBase", "#0F0F0F");
        palette.endGroup();

        palette.beginGroup("Body");
        palette.setValue("Background", "#212121");
        palette.endGroup();

        palette.beginGroup("Navbar");
        palette.setValue("Background", "#111111");
        palette.setValue("SelectedColor", "#9351E5");
        palette.setValue("HoverColor", "#ADADAD");
        palette.endGroup();

        palette.beginGroup("TitleBar");
        palette.setValue("Color", "#F5F6F7");
        palette.endGroup();
    }

    bodyColor->setStyleSheet("background-color: " + palette.value("Body/Background").toString() + ";}");
    navbarBG->setStyleSheet("background-color: " + palette.value("Navbar/Background").toString() + ";}");
    navbarHover->setStyleSheet("background-color: " + palette.value("Navbar/HoverColor").toString() + ";}");
    navbarSelected->setStyleSheet("background-color: " + palette.value("Navbar/SelectedColor").toString() + ";}");
    titleBarColor->setStyleSheet("background-color: " + palette.value("TitleBar/Color").toString() + ";}");
    activeElement->setStyleSheet("background-color: " + palette.value("Primary/ActiveElement").toString() + ";}");
    inactiveSelection->setStyleSheet("background-color: " + palette.value("Primary/InactiveSelection").toString() + ";}");
    hoverSelection->setStyleSheet("background-color: " + palette.value("Primary/HoverSelection").toString() + ";}");
    darkElement->setStyleSheet("background-color: " + palette.value("Primary/DarkElement").toString() + ";}");
    lightText->setStyleSheet("background-color: " + palette.value("Primary/LightText").toString() + ";}");
    darkText->setStyleSheet("background-color: " + palette.value("Primary/DarkText").toString() + ";}");
    subText->setStyleSheet("background-color: " + palette.value("Primary/SubText").toString() + ";}");
    primaryBase->setStyleSheet("background-color: " + palette.value("Primary/PrimaryBase").toString() + ";}");
    secondaryBase->setStyleSheet("background-color: " + palette.value("Primary/SecondaryBase").toString() + ";}");
    tertiaryBase->setStyleSheet("background-color: " + palette.value("Primary/TertiaryBase").toString() + ";}");
    darkestBase->setStyleSheet("background-color: " + palette.value("Primary/DarkestBase").toString() + ";}");
}

void Settings::exportTheme()
{
    QSettings palette(QSettings::IniFormat, QSettings::UserScope, "HorizonLauncher", "palette");

    QFileDialog* dlg = new QFileDialog();
    dlg->setFileMode(QFileDialog::AnyFile);

    if (dlg->exec())
    {
        QString filename = dlg->selectedFiles().at(0);
        QFile file(filename);

        if (!file.open(QIODevice::WriteOnly))
        {
            qDebug() << "Couldn't open " << filename << " for writing.";
            return;
        }

        QTextStream fileStream(&file);

        QStringList keys = palette.allKeys();
        for (int i; i < keys.size(); i++)
        {
            QString key = keys[i];
            QString value = palette.value(key).toString();

            fileStream << key << "=" << value << "\n";
        }
    }
}

void Settings::importTheme()
{
    QSettings palette(QSettings::IniFormat, QSettings::UserScope, "HorizonLauncher", "palette");

    QRegExp properLineFormat("^[\\w\\/]+=#\\w+$");

    QFileDialog* dlg = new QFileDialog();
    dlg->setFileMode(QFileDialog::ExistingFile);

    if (dlg->exec())
    {
        QString filename = dlg->selectedFiles().at(0);
        QFile file(filename);

        if (!file.open(QIODevice::ReadOnly))
        {
            qDebug() << "Couldn't open " << filename << " for reading.";
            return;
        }

        QTextStream fileStream(&file);

        while (!fileStream.atEnd())
        {
            QString line = fileStream.readLine();
            if (!line.contains(properLineFormat))
            {
                qDebug() << "Error: Line doesn't contain proper format.";
                continue;
            };

            int eqPos = line.indexOf("=");
            QString leftSide = line.left(eqPos);
            QString rightSide = line.mid(eqPos + 1);
            palette.setValue(leftSide, rightSide);
        }
    }
}

void Settings::setMultipleExec(bool multipleExec)
{
    QSettings config(QSettings::IniFormat, QSettings::UserScope, "HorizonLauncher", "config");

    config.beginGroup("GameLauncher");
    config.setValue("MultipleExec", multipleExec);
    config.endGroup();
}

void Settings::setFulltab(bool fulltab)
{
    QSettings palette(QSettings::IniFormat, QSettings::UserScope, "HorizonLauncher", "palette");
    // palette so that you can export it in a theme!

    palette.beginGroup("TitleBar");
    palette.setValue("Fulltab", fulltab);
    palette.endGroup();
}

void Settings::pickSetColor(int id)
{
    QColor color = QColorDialog::getColor(Qt::white);
    updateColor(id, color);
}

void Settings::updateColor(int id, QColor color)
{
    if (!color.isValid())
    {
        return;
    }

    QSettings palette(QSettings::IniFormat, QSettings::UserScope, "HorizonLauncher", "palette");

    if (palette.isWritable())
    {
        if (id == 1) // Body/Background
        {
            palette.beginGroup("Body");
            palette.setValue("Background", color.name());
            palette.endGroup();

            bodyColor->setStyleSheet("background-color: " + color.name() + ";}");
        }
        else if (id <= 4) // Navbar
        {
            palette.beginGroup("Navbar");
            if (id == 2) // Background
            {
                palette.setValue("Background", color.name());
                navbarBG->setStyleSheet("background-color: " + color.name() + ";}");
            }
            else if (id == 3) // Hover
            {
                palette.setValue("HoverColor", color.name());
                navbarHover->setStyleSheet("background-color: " + color.name() + ";}");
            }
            else if (id == 4) // Selected
            {
                palette.setValue("SelectedColor", color.name());
                navbarSelected->setStyleSheet("background-color: " + color.name() + ";}");
            }
            palette.endGroup();
        }
        else if (id == 5) //TitleBar/Color
        {
            palette.beginGroup("TitleBar");
            palette.setValue("Color", color.name());
            palette.endGroup();
            titleBarColor->setStyleSheet("background-color: " + color.name() + ";}");
        }
        else if (id <= 16)
        {
            palette.beginGroup("Primary");

            if (id == 6)
            {
                palette.setValue("ActiveElement", color.name());
                activeElement->setStyleSheet("background-color: " + color.name() + ";}");
            }
            else if (id == 7)
            {
                palette.setValue("InactiveSelection", color.name());
                inactiveSelection->setStyleSheet("background-color: " + color.name() + ";}");
            }
            else if (id == 8)
            {
                palette.setValue("HoverSelection", color.name());
                hoverSelection->setStyleSheet("background-color: " + color.name() + ";}");
            }
            else if (id == 9)
            {
                palette.setValue("DarkElement", color.name());
                darkElement->setStyleSheet("background-color: " + color.name() + ";}");
            }
            else if (id == 10)
            {
                palette.setValue("LightText", color.name());
                lightText->setStyleSheet("background-color: " + color.name() + ";}");
            }
            else if (id == 11)
            {
                palette.setValue("DarkText", color.name());
                darkText->setStyleSheet("background-color: " + color.name() + ";}");
            }
            else if (id == 12)
            {
                palette.setValue("SubText", color.name());
                subText->setStyleSheet("background-color: " + color.name() + ";}");
            }
            else if (id == 13)
            {
                palette.setValue("PrimaryBase", color.name());
                primaryBase->setStyleSheet("background-color: " + color.name() + ";}");
            }
            else if (id == 14)
            {
                palette.setValue("SecondaryBase", color.name());
                secondaryBase->setStyleSheet("background-color: " + color.name() + ";}");
            }
            else if (id == 15)
            {
                palette.setValue("TertiaryBase", color.name());
                tertiaryBase->setStyleSheet("background-color: " + color.name() + ";}");
            }
            else if (id == 16)
            {
                palette.setValue("DarkestBase", color.name());
                darkestBase->setStyleSheet("background-color: " + color.name() + ";}");
            }

            palette.endGroup();
        }
    }
}

void Settings::confirmClearDb()
{
    int ret = QMessageBox(QMessageBox::Question, "Deleting Database", "Proceeding will delete the database, the database will be non-recoverable. Proceed?", QMessageBox::Yes | QMessageBox::No).exec();
    switch (ret)
    {
        case QMessageBox::Yes:
            Library::db.reset();
            break;
        case QMessageBox::No:
            break;
        default:
            break;
    }
}
