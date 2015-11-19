#include "AddGameWizard.h"
#include "Database.h"

#include <QDebug>
#include <QString>

/** AddGameWizard constructor
 * Defines the pages and initializes the database with the path given. Also sets up some window-related properties,
 * such as title and initial size.
 * \param parent Parent widget to draw from
 * \param dbPath Path to the database used
 */
AddGameWizard::AddGameWizard(QWidget* parent, QString dbPath) : QWizard(parent), db(QDir(dbPath).filePath("horizon.db"))
{
    addPage(new InitPage());
    addPage(new InfoPage());
    addPage(new LastPage(db));

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
    setTitle("Intro");
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

    QPushButton* dirFileBtn = new QPushButton(tr("Browse"));
    QPushButton* exeFileBtn = new QPushButton(tr("Browse"));

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
LastPage::LastPage(Database db, QWidget* parent) : QWizardPage(parent), db(db)
{
    setTitle(tr("Done"));
}

/** Initializes the last page. This function is called when the NextButton on the previous page is clicked,
 * which allows the state of the previous page to be transferred to this one.
 * The function checks if the game already exists in the database, and if not, adds that game.
*/
void LastPage::initializePage()
{
    db.init();
    QLabel* label = new QLabel();
    qDebug() << field("nameEdit").toString();
    qDebug() << field("dirEdit").toString();
    qDebug() << field("exeEdit").toString();
    qDebug() << field("argsEdit").toString();

    if (!std::get<0>(db.isExistant(field("nameEdit").toString())))
    {
        label->setText(tr("Game added successfully."));
        db.addGame(field("nameEdit").toString(),field("dirEdit").toString(), field("exeEdit").toString(),field("argsEdit").toString(), 0);
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
