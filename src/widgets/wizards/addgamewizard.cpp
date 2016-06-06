#include "addgamewizard.h"
#include "../../database.h"
#include "../stack/library.h"

#include <QString>

/** AddGameWizard constructor
 * Defines the pages and initializes the database with the path given. Also sets up some window-related properties,
 * such as title and initial size.
 * \param parent Parent widget to draw from
 */
AddGameWizard::AddGameWizard(QWidget* parent) : QWizard(parent)
{
    addPage(new InitPage());
    addPage(new InfoPage());
    addPage(new LastPage());

    setWindowTitle(tr("Add game wizard"));
}

/** pickFile function
 * Picks a file or directory and puts the path in edit
 * \param edit A QLineEdit to put the file path in
 * \param mode A FileMode to choose what files can be selectedFiles
 */
void InfoPage::pickFile(QLineEdit* edit, QFileDialog::FileMode mode)
{
    QFileDialog dlg(this);
    dlg.setFileMode(mode);

    if (dlg.exec())
    {
        edit->setText(dlg.selectedFiles().at(0));
    }
}

/** InitPage constructor
 * Defines some initial properties for the initial page.
 * \param parent Parent widget to draw from
 */
InitPage::InitPage(QWidget* parent) : QWizardPage(parent)
{
    setTitle(tr("Intro"));
    QLabel* label = new QLabel(tr("This wizard will help you easily add games to your database."));
    label->setWordWrap(true);
    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(label);
    setLayout(layout);
}

/** InfoPage constructor
 * Defines some initial properties for the page in which the user will input the game's information.
 * \param parent Parent widget to draw from
 */
InfoPage::InfoPage(QWidget* parent) : QWizardPage(parent)
{
    setTitle(tr("Input game details."));
    QLineEdit* nameEdit = new QLineEdit();
    this->dirEdit = new QLineEdit();
    this->exeEdit = new QLineEdit();
    QLineEdit* argsEdit = new QLineEdit();

    QLabel* nameLabel = new QLabel(tr("Name: "));
    QLabel* dirLabel = new QLabel(tr("Directory: "));
    QLabel* exeLabel = new QLabel(tr("Executable: "));
    QLabel* argsLabel = new QLabel(tr("Arguments (optional): "));

    QPixmap browsePixmap(":/res/icon/Material/File.svg");
    QIcon browseIcon(browsePixmap);

    QPushButton* dirFileBtn = new QPushButton();
    dirFileBtn->setIcon(browseIcon);
    QPushButton* exeFileBtn = new QPushButton();
    exeFileBtn->setIcon(browseIcon);

    connect(dirFileBtn, &QPushButton::clicked, [=] { pickFile(this->dirEdit, QFileDialog::Directory); });
    connect(exeFileBtn, &QPushButton::clicked, [=] { pickFile(this->exeEdit, QFileDialog::ExistingFile); });

    registerField("nameEdit*", nameEdit);
    registerField("dirEdit*", dirEdit);
    registerField("exeEdit*", exeEdit);
    registerField("argsEdit", argsEdit);

    QGridLayout* layout = new QGridLayout();
    layout->addWidget(nameLabel, 0, 0);
    layout->addWidget(nameEdit, 0, 1);
    layout->addWidget(dirLabel, 1, 0);
    layout->addWidget(this->dirEdit, 1, 1);
    layout->addWidget(dirFileBtn, 1, 2);
    layout->addWidget(exeLabel, 2, 0);
    layout->addWidget(this->exeEdit, 2, 1);
    layout->addWidget(exeFileBtn, 2, 2);
    layout->addWidget(argsLabel, 3, 0);
    layout->addWidget(argsEdit, 3, 1);

    setLayout(layout);
}

/** InfoPage constructor
 * Defines some initial properties for the page in which the user will input the game's information.
 * \param parent Parent widget to draw from
 */
LastPage::LastPage(QWidget* parent) : QWizardPage(parent)
{
    setTitle(tr("Done"));
}

/** Initializes the last page. This function is called when the NextButton on the previous page is clicked,
 * which allows the state of the previous page to be transferred to this one.
 * The function checks if the game already exists in the database, and if not, adds that game.
*/
void LastPage::initializePage()
{
    QLabel* label = new QLabel();

    if (!std::get<0>(Library::db.isExistant(field("nameEdit").toString())))
    {
        Game game = Game {0,
                         field("nameEdit").toString(),
                         field("dirEdit").toString(),
                         field("exeEdit").toString(),
                         field("argsEdit").toString(),
                         0};
        Library::db.addGame(game);
        label->setText(tr("Game added successfully."));
    }
    else
    {
        label->setText(tr("Game already exists."));
    }
    label->setWordWrap(true);
    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(label);
    setLayout(layout);
}
