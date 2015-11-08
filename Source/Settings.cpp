#include "Settings.h"
#include "ui_Settings.h"
#include "DRMSetupWizard.h"

/** Settings constructor
* Initialize the settings UI
* \param p Inherited palette configuration for setting StyleSheets.
* \param parent Pointer to parent widget.
*/
Settings::Settings(QSettings* p, QWidget* parent) : QWidget(parent), ui(new Ui::Settings)
{
    ui->setupUi(this);
    this->setObjectName("settingsUI");
    this->setStyleSheet("background-color: " + p->value("Primary/SecondaryBase").toString() + ";} "
        "QLabel { color:  #ffffff;}"
        "QPushButton {"
        "color: " + p->value("Primary/LightText").toString() + "; "
        "background-color: " + p->value("Primary/DarkElement").toString() + "; "
        "border: none; margin: 0px; padding: 0px;} "
        "QPushButton:hover {"
        "background-color: " + p->value("Primary/InactiveSelection").toString() + ";} "
        "color: " + p->value("Primary/LightText").toString() + ";");

    QFont buttonFont("SourceSansPro", 9);

    ui->WizardButton->setFont(buttonFont);
    ui->WizardButton->setText(tr("Add Games to Horizon"));

    ui->ClearDatabaseButton->setFont(buttonFont);
    ui->ClearDatabaseButton->setText(tr("Clear Database"));

    ui->BodyColor->setFont(buttonFont);
    ui->BodyColor->setStyleSheet("background-color: " + p->value("Body/Background").toString() + ";}");

    ui->NavbarBG->setFont(buttonFont);
    ui->NavbarBG->setStyleSheet("background-color: " + p->value("Navbar/Background").toString() + ";}");

    ui->NavbarHover->setFont(buttonFont);
    ui->NavbarHover->setStyleSheet("background-color: " + p->value("Navbar/HoverColor").toString() + ";}");

    ui->NavbarSelected->setFont(buttonFont);
    ui->NavbarSelected->setStyleSheet("background-color: " + p->value("Navbar/SelectedColor").toString() + ";}");

    ui->TitleBarColor->setFont(buttonFont);
    ui->TitleBarColor->setStyleSheet("background-color: " + p->value("TitleBar/Color").toString() + ";}");

    ui->ActiveElement->setFont(buttonFont);
    ui->ActiveElement->setStyleSheet("background-color: " + p->value("Primary/ActiveElement").toString() + ";}");

    ui->InactiveSelection->setFont(buttonFont);
    ui->InactiveSelection->setStyleSheet("background-color: " + p->value("Primary/InactiveSelection").toString() + ";}");

    ui->HoverSelection->setFont(buttonFont);
    ui->HoverSelection->setStyleSheet("background-color: " + p->value("Primary/HoverSelection").toString() + ";}");

    ui->DarkElement->setFont(buttonFont);
    ui->DarkElement->setStyleSheet("background-color: " + p->value("Primary/DarkElement").toString() + ";}");

    ui->LightText->setFont(buttonFont);
    ui->LightText->setStyleSheet("background-color: " + p->value("Primary/LightText").toString() + ";}");

    ui->DarkText->setFont(buttonFont);
    ui->DarkText->setStyleSheet("background-color: " + p->value("Primary/DarkText").toString() + ";}");

    ui->SubText->setFont(buttonFont);
    ui->SubText->setStyleSheet("background-color: " + p->value("Primary/SubText").toString() + ";}");

    ui->PrimaryBase->setFont(buttonFont);
    ui->PrimaryBase->setStyleSheet("background-color: " + p->value("Primary/PrimaryBase").toString() + ";}");

    ui->SecondaryBase->setFont(buttonFont);
    ui->SecondaryBase->setStyleSheet("background-color: " + p->value("Primary/SecondaryBase").toString() + ";}");

    ui->TertiaryBase->setFont(buttonFont);
    ui->TertiaryBase->setStyleSheet("background-color: " + p->value("Primary/TertiaryBase").toString() + ";}");

    ui->DarkestBase->setFont(buttonFont);
    ui->DarkestBase->setStyleSheet("background-color: " + p->value("Primary/DarkestBase").toString() + ";}");

    ui->ResetColors->setFont(buttonFont);

    ui->label_2->setStyleSheet("{color: #FFFFFF}");
    ui->UserSettingsBox->setStyleSheet("color: #FFFFFF;} ");
    ui->ClientSettingsBox->setStyleSheet("color: #FFFFFF;} ");
    ui->StyleSettingsBox->setStyleSheet("color: #FFFFFF;} ");

    connect(ui->WizardButton, SIGNAL(clicked()), QApplication::instance(), SLOT(Settings::on_WizardButton_clicked()));
    connect(ui->BodyColor, &QPushButton::clicked, [=]() { pickSetColor(1); });
    connect(ui->NavbarBG, &QPushButton::clicked, [=]() { pickSetColor(2); });
    connect(ui->NavbarHover, &QPushButton::clicked, [=]() { pickSetColor(3); });
    connect(ui->NavbarSelected, &QPushButton::clicked, [=]() { pickSetColor(4); });
    connect(ui->TitleBarColor, &QPushButton::clicked, [=]() { pickSetColor(5); });
    connect(ui->ActiveElement, &QPushButton::clicked, [=]() { pickSetColor(6); });
    connect(ui->InactiveSelection, &QPushButton::clicked, [=]() { pickSetColor(7); });
    connect(ui->HoverSelection, &QPushButton::clicked, [=]() { pickSetColor(8); });
    connect(ui->DarkElement, &QPushButton::clicked, [=]() { pickSetColor(9); });
    connect(ui->LightText, &QPushButton::clicked, [=]() { pickSetColor(10); });
    connect(ui->DarkText, &QPushButton::clicked, [=]() { pickSetColor(11); });
    connect(ui->SubText, &QPushButton::clicked, [=]() { pickSetColor(12); });
    connect(ui->PrimaryBase, &QPushButton::clicked, [=]() { pickSetColor(13); });
    connect(ui->SecondaryBase, &QPushButton::clicked, [=]() { pickSetColor(14); });
    connect(ui->TertiaryBase, &QPushButton::clicked, [=]() { pickSetColor(15); });
    connect(ui->DarkestBase, &QPushButton::clicked, [=]() { pickSetColor(16); });
    connect(ui->ResetColors, SIGNAL(clicked()), QApplication::instance(), SLOT(Settings::on_ResetColors_clicked()));
    connect(ui->ClearDatabaseButton, SIGNAL(clicked()), QApplication::instance(), SLOT(Settings::on_ClearDatabaseButton_clicked()));

    if (!db.init())
    {
        QMessageBox error;
        error.critical(0, tr("Error!"), tr("An error occured while trying to load the database."));
        exit(EXIT_FAILURE);
    }
}

/** Event handler for Wizard Button
* Runs setup wizard on click
*/
void Settings::on_WizardButton_clicked()
{
    DRMSetupWizard* wiz = new DRMSetupWizard();
    wiz->show();
}

void Settings::on_ResetColors_clicked()
{
    QSettings palette(QSettings::IniFormat, QSettings::UserScope, "Horizon Launcher", "palette");

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

    ui->BodyColor->setStyleSheet("background-color: " + palette.value("Body/Background").toString() + ";}");
    ui->NavbarBG->setStyleSheet("background-color: " + palette.value("Navbar/Background").toString() + ";}");
    ui->NavbarHover->setStyleSheet("background-color: " + palette.value("Navbar/HoverColor").toString() + ";}");
    ui->NavbarSelected->setStyleSheet("background-color: " + palette.value("Navbar/SelectedColor").toString() + ";}");
    ui->ActiveElement->setStyleSheet("background-color: " + palette.value("Primary/ActiveElement").toString() + ";}");
    ui->InactiveSelection->setStyleSheet("background-color: " + palette.value("Primary/InactiveSelection").toString() + ";}");
    ui->HoverSelection->setStyleSheet("background-color: " + palette.value("Primary/HoverSelection").toString() + ";}");
    ui->DarkElement->setStyleSheet("background-color: " + palette.value("Primary/DarkElement").toString() + ";}");
    ui->LightText->setStyleSheet("background-color: " + palette.value("Primary/LightText").toString() + ";}");
    ui->DarkText->setStyleSheet("background-color: " + palette.value("Primary/DarkText").toString() + ";}");
    ui->SubText->setStyleSheet("background-color: " + palette.value("Primary/SubText").toString() + ";}");
    ui->PrimaryBase->setStyleSheet("background-color: " + palette.value("Primary/PrimaryBase").toString() + ";}");
    ui->SecondaryBase->setStyleSheet("background-color: " + palette.value("Primary/SecondaryBase").toString() + ";}");
    ui->TertiaryBase->setStyleSheet("background-color: " + palette.value("Primary/TertiaryBase").toString() + ";}");
    ui->DarkestBase->setStyleSheet("background-color: " + palette.value("Primary/DarkestBase").toString() + ";}");
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

    QSettings palette(QSettings::IniFormat, QSettings::UserScope, "Horizon Launcher", "palette");

    if (palette.isWritable())
    {
        if (id == 1) // Body/Background
        {
            palette.beginGroup("Body");
            palette.setValue("Background", color.name());
            palette.endGroup();

            ui->BodyColor->setStyleSheet("background-color: " + color.name() + ";}");
        }
        else if (id <= 4) // Navbar
        {
            palette.beginGroup("Navbar");
            if (id == 2) // Background
            {
                palette.setValue("Background", color.name());
                ui->NavbarBG->setStyleSheet("background-color: " + color.name() + ";}");
            }
            else if (id == 3) // Hover
            {
                palette.setValue("HoverColor", color.name());
                ui->NavbarHover->setStyleSheet("background-color: " + color.name() + ";}");
            }
            else if (id == 4) // Selected
            {
                palette.setValue("SelectedColor", color.name());
                ui->NavbarSelected->setStyleSheet("background-color: " + color.name() + ";}");
            }
            palette.endGroup();
        }
        else if (id == 5) //TitleBar/Color
        {
            palette.beginGroup("TitleBar");
            palette.setValue("Color", color.name());
            palette.endGroup();
            ui->TitleBarColor->setStyleSheet("background-color: " + color.name() + ";}");
        }
        else if (id <= 16)
        {
            palette.beginGroup("Primary");

            if (id == 6)
            {
                palette.setValue("ActiveElement", color.name());
                ui->ActiveElement->setStyleSheet("background-color: " + color.name() + ";}");
            }
            else if (id == 7)
            {
                palette.setValue("InactiveSelection", color.name());
                ui->InactiveSelection->setStyleSheet("background-color: " + color.name() + ";}");
            }
            else if (id == 8)
            {
                palette.setValue("HoverSelection", color.name());
                ui->HoverSelection->setStyleSheet("background-color: " + color.name() + ";}");
            }
            else if (id == 9)
            {
                palette.setValue("DarkElement", color.name());
                ui->DarkElement->setStyleSheet("background-color: " + color.name() + ";}");
            }
            else if (id == 10)
            {
                palette.setValue("LightText", color.name());
                ui->LightText->setStyleSheet("background-color: " + color.name() + ";}");
            }
            else if (id == 11)
            {
                palette.setValue("DarkText", color.name());
                ui->DarkText->setStyleSheet("background-color: " + color.name() + ";}");
            }
            else if (id == 12)
            {
                palette.setValue("SubText", color.name());
                ui->SubText->setStyleSheet("background-color: " + color.name() + ";}");
            }
            else if (id == 13)
            {
                palette.setValue("PrimaryBase", color.name());
                ui->PrimaryBase->setStyleSheet("background-color: " + color.name() + ";}");
            }
            else if (id == 14)
            {
                palette.setValue("SecondaryBase", color.name());
                ui->SecondaryBase->setStyleSheet("background-color: " + color.name() + ";}");
            }
            else if (id == 15)
            {
                palette.setValue("TertiaryBase", color.name());
                ui->TertiaryBase->setStyleSheet("background-color: " + color.name() + ";}");
            }
            else if (id == 16)
            {
                palette.setValue("DarkestBase", color.name());
                ui->DarkestBase->setStyleSheet("background-color: " + color.name() + ";}");
            }

            palette.endGroup();
        }
    }
}

void Settings::on_ClearDatabaseButton_clicked()
{
    int ret = QMessageBox(QMessageBox::Question, "Deleting Database", "Proceeding will delete the database, the database will be non-recoverable. Proceed?", QMessageBox::Yes | QMessageBox::No).exec();
    switch (ret)
    {
        case QMessageBox::Yes:
            db.reset();
            break;
        case QMessageBox::No:
            break;
        default:
            break;
    }
}

Settings::~Settings()
{
    delete ui;
}
