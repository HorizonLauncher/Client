#include "DRMSetupWizard.h"
#include "Libs/SteamVdfParse.hpp"
#include "Database.h"
#include "Library.h"
#include <QtWidgets>
#include <iostream>
#include <boost/property_tree/info_parser.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/optional.hpp>
#include <thread>
#include <future>

namespace pt = boost::property_tree;

GameList addedVector;
QList<QButtonGroup*> originBtnGroupVector;
QList<QButtonGroup*> uplayBtnGroupVector;

/** DRMSetupWizard constructor
 * Defines the pages and initializes the database with the path given. Also sets up some window-related properties,
 * such as title and initial size.
 * \param parent Parent widget to draw from
 * \param dbPath Path to the database used
 */
DRMSetupWizard::DRMSetupWizard(QWidget* parent, QString dbPath) : QWizard(parent), db(QDir(dbPath).filePath("horizon.db"))
{
    drmPage = new DRMPage();
    resultsPage = new ResultsPage(db, *drmPage);
    finalPage = new FinalPage(db);
    setPage(pages::INTRO, new IntroPage());
    setPage(pages::DRM, drmPage);
    setPage(pages::RESULTS, resultsPage);
    setPage(pages::FINAL, finalPage);
    setWindowTitle(tr("Horizon Launcher Setup"));
    setFixedSize(QSize(700, 450));
    addedVector.erase(addedVector.begin(), addedVector.end());
    if (!db.init())
    {
        QMessageBox error;
        error.critical(0, tr("Error!"), tr("An error occurred while trying to load the database."));
        exit(EXIT_FAILURE);
    }
}

/** IntroPage constructor
 * Defines some initial properties for the introduction page.
 * \param parent Parent widget to draw from
 */
IntroPage::IntroPage(QWidget* parent) : QWizardPage(parent)
{
    setTitle(tr("Finding games"));
    setSubTitle(tr("This wizard will try to find all your games."));
    auto layout = new QGridLayout();
    auto label = new QLabel(tr("Click next to attempt to find Steam, Origin and Uplay games where applicable."));
    layout->addWidget(label);
    setLayout(layout);
}

/** DRMPage constructor
 * Defines some initial properties for the DRM page.
 * \param parent Parent widget to draw from
 */
DRMPage::DRMPage(QWidget* parent) : QWizardPage(parent)
{
    setTitle(tr("Checking for Steam"));
    steamBox = new QCheckBox();
    originBox = new QCheckBox();
    uplayBox = new QCheckBox();
    registerField("steamFound", steamBox);
    registerField("uplayFound", uplayBox);
    registerField("originFound", originBox);
    layout = new QGridLayout();
    platformLabel = new QLabel(tr("<b>Steam</b>"));
    descLabel = new QLabel();
    statusLabel = new QLabel();
    platformLabel->setTextFormat(Qt::TextFormat::RichText);
    checkSteamExists();

#if defined(_WIN32) || defined(_WIN64) || defined(__APPLE__)
    setTitle(title() += tr(", Origin"));
    platformLabel = new QLabel("<b>Origin</b>");
    statusLabel = new QLabel();
    checkOriginExists();
#endif

#if defined(_WIN32) || defined (_WIN64)
    setTitle(title() += tr(" and Uplay."));
    platformLabel = new QLabel("<b>Uplay</b>");
    statusLabel = new QLabel();
    checkUplayExists();
#endif

    setLayout(layout);
}

/** ResultsPage constructor
 * Defines some initial properties for the results page.
 */
ResultsPage::ResultsPage(Database db, DRMPage& drmPage, QWidget* parent) : QWizardPage(parent), db(db)
{
    setSubTitle(tr("We found the following on your system."));
    steamRoot = drmPage.steamPath;
    originRoot = drmPage.originPath;
    uplayRoot = drmPage.uplayPath;
}

/** FinalPage constructor
 * Defines some initial properties for the final page.
 */
FinalPage::FinalPage(Database db, QWidget* parent) : QWizardPage(parent), db(db)
{
    setTitle(tr("Done"));
}

/** Check if Origin is installed on the current computer, if applicable, and sets some values for later pages to
 * check on.
 * \see checkSteamExists()
 * \see checkUplayExists()
 */
void DRMPage::checkOriginExists()
{
    QDir originRoot;
    QDir originFolder;
#if defined(_WIN32) || defined(_WIN64)
    originRoot = QDir(qgetenv("APPDATA").append("/Origin"));
#else
    originRoot = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation).append("/Origin/");
#endif


    if (originRoot.exists())
    {
        pt::ptree originTree;
        read_xml(originRoot.filePath("local.xml").toLocal8Bit().constData(), originTree);


        for (auto &xmlIter : originTree.get_child("Settings"))
        {
            if (xmlIter.second.get<std::string>("<xmlattr>.key") == "DownloadInPlaceDir")
            {
                originFolder = QString::fromStdString(xmlIter.second.get<std::string>("<xmlattr>.value"));
                break;
            }
        }

        if (originFolder == QDir("."))
        {
            originFolder = QDir("C:\\Program Files (x86)\\Origin Games\\");
        }
    }
    else
    {
        return;
    }

    if (originFolder.filePath("").trimmed() != "" && originFolder.exists())
    {
        statusLabel->setPixmap(QPixmap(":/SystemMenu/Icons/Tick.svg"));
        //: %1 will be replaced with the path to Origin.
        descLabel = new QLabel(QString(tr("Origin found in %1")).arg(originFolder.filePath("")));
        originBox->setChecked(true);
        originPath = originFolder.filePath("");
    }
    else
    {
        statusLabel->setPixmap(QPixmap(":/SystemMenu/Icons/Cross.svg"));
        descLabel = new QLabel(tr("Origin not found. Verify installation and try again."));
    }
    layout->addWidget(platformLabel, 3, 0, 0);
    layout->addWidget(descLabel, 4, 0, 0);
    layout->addWidget(statusLabel, 3, 1, 0);
    layout->setRowMinimumHeight(4, 40);
}

/** Check if Steam is installed on the current computer, if applicable, and sets some values for later pages to
 * check on.
 * \see checkOriginExists()
 * \see checkUplayExists()
 */
void DRMPage::checkSteamExists()
{
    QDir steamFolder;
    bool steamExists = false;

#if defined(__linux__)
    QProcess which;
    which.setProcessChannelMode(QProcess::MergedChannels);
    QStringList args;
    args << "steam";
    which.start("which", args);
    which.waitForFinished();
    if (which.exitCode() == 0)
    {
        steamFolder = QDir(QDir::homePath() + "/.local/share/Steam");
        steamExists = true;
    }
#elif defined(_WIN32) || defined(_WIN64)
    QSettings settings("HKEY_CURRENT_USER\\Software\\Valve\\Steam", QSettings::NativeFormat);
    if (settings.contains("SteamPath"))
    {
        steamFolder = QDir(settings.value("SteamPath").toString()).canonicalPath();
        steamExists = true;
    }
#elif defined(__APPLE__)
    steamFolder = QDir(QDir::home().filePath("Library/Application Support/Steam"));
    steamExists = steamFolder.exits()
#endif

    if (steamFolder.filePath("").trimmed() != "" && steamFolder.exists() && steamExists)
    {
        statusLabel->setPixmap(QPixmap(":SystemMenu/Icons/Tick.svg"));
        //: %1 will be replaced with the path to Steam.
        descLabel = new QLabel(QString(tr("Steam found in %1")).arg(steamFolder.filePath("")));
        steamBox->setChecked(true);
        steamPath = steamFolder.filePath("");
    }
    else
    {
        statusLabel->setPixmap(QPixmap(":SystemMenu/Icons/Cross.svg"));
        descLabel = new QLabel(tr("Steam not found. Verify installation and try again."));
    }
    layout->addWidget(platformLabel, 0, 0, 0);
    layout->addWidget(descLabel, 1, 0, 0);
    layout->addWidget(statusLabel, 0, 1, 0);
    layout->setRowMinimumHeight(1, 40);
}

/** Check if Uplay is installed on the current computer, if applicable, and sets some values for later pages to
 * check on.
 * \see checkSteamExists()
 * \see checkOriginExists()
 */
void DRMPage::checkUplayExists()
{
    QDir uplayFolder;
    QDir uplayRoot = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation).append("/Ubisoft Game Launcher");
    bool loggedIn = false;
    if (uplayRoot.exists())
    {
        std::ifstream uplaySettings(uplayRoot.filePath("settings.yml").toLocal8Bit().constData(), std::ifstream::in);
        std::string line;
        if (uplaySettings.is_open())
        {
            while (std::getline(uplaySettings, line))
            {
                if (line.find("user") != std::string::npos)
                {
                    loggedIn = true;
                }
                if (line.find("game_installation_path") != std::string::npos)
                {
                    std::vector<std::string> strSplit;
                    boost::split(strSplit, line, boost::is_any_of("\""));
                    uplayFolder = QDir(QString::fromStdString(strSplit.at(1)));
                    break;
                }
            }
        }

        if (loggedIn)
        {
            if (uplayFolder == QDir("."))
            {
                QSettings settings("HKEY_LOCAL_MACHINE\\SOFTWARE\\Wow6432Node\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Uplay", QSettings::NativeFormat);
                if (!settings.value("InstallLocation").isNull())
                {
                    uplayFolder = QDir(settings.value("InstallLocation").toString());
                }
            }
        }

    }

    if (uplayFolder.filePath("").trimmed() != "" && uplayFolder != QDir(".") && uplayFolder.exists())
    {
        statusLabel->setPixmap(QPixmap(":/SystemMenu/Icons/Tick.svg"));
        //: %1 will be replaced with the path to uPlay.
        descLabel = new QLabel(QString(tr("Uplay found in %1")).arg(uplayFolder.filePath("")));
        uplayBox->setChecked(true);
        uplayPath = uplayFolder.filePath("");
    }
    else
    {
        statusLabel->setPixmap(QPixmap(":/SystemMenu/Icons/Cross.svg"));
        descLabel = new QLabel(tr("Uplay not found on the system. Verify installation and try again."));
    }
    layout->addWidget(platformLabel, 5, 0, 0);
    layout->addWidget(descLabel, 6, 0, 0);
    layout->addWidget(statusLabel, 5, 1, 0);
    layout->setRowMinimumHeight(6, 40);
}

/** Initializes the results page. This function is called when the NextButton on the previous page is clicked,
 * which allows the state of the previous page to be transferred to this one.
 * The function then runs each respective check*Exists function, and populates the various sections with correct information.
 * \see checkSteamExists()
 * \see checkUplayExists()
 * \see checkOriginExists()
 */
void ResultsPage::initializePage()
{
    int originCount = 0;
    int steamCount = 0;
    int uPlayCount = 0;

    if (!field("uplayFound").toBool() && !field("steamFound").toBool() && !field("originFound").toBool())
    {
        setTitle(tr("We found no games."));
        setSubTitle(tr("Install Steam, Origin and/or Uplay to find games with this wizard, or check current installation(s)."));
    }
    else
    {
        setSubTitle(tr("Change the title for each game by clicking the text box and editing."));
        btnGroup = new QButtonGroup();
        btnGroup->setExclusive(false);
        tabWidget = new QTabWidget();
        top_layout = new QGridLayout();
        if (field("steamFound").toBool())
        {
            steamLayout = new QGridLayout();
            steamScrollArea = new QScrollArea();
            steamViewport = new QWidget();
            auto t = std::async(&ResultsPage::findSteamGames, this);
            t.get();
            int row = 0;
            steamCount = steamVector.size();
            for (auto i : steamVector)
            {
                QCheckBox* checkBox = new QCheckBox(tr("Executable not found"));
                QLineEdit* name = new QLineEdit(i.gameName);
                name->setFixedWidth(350);
                checkBox->setStyleSheet("QLabel { color: red; }");
                for (auto dir : steamDirectoryList)
                {
                    if (i.executablePath.contains(dir))
                    {
                        checkBox->setStyleSheet("QLabel { color: black; }");
                        checkBox->setText(tr("Executable:  ") + i.executablePath.remove(QDir(dir).filePath("SteamApps/common")).remove(0, 1));
                        break;
                    }
                }
                steamLayout->addWidget(name, row, 0, Qt::AlignVCenter | Qt::AlignLeft);
                row++;
                steamLayout->addWidget(checkBox, row, 0, Qt::AlignVCenter | Qt::AlignLeft);
                row++;
                btnGroup->addButton(checkBox);
            }
            steamViewport->setLayout(steamLayout);
            steamScrollArea->setWidget(steamViewport);
            tabWidget->addTab(steamScrollArea, tr("Steam"));
        }

        if (field("originFound").toBool())
        {
            originScrollArea = new QScrollArea();
            originViewport = new QWidget();
            originLayout = new QGridLayout();
            auto t = std::async(&ResultsPage::findOriginGames, this);
            t.get();
            int row = 0;
            int count = originTree.get<int>("games.count");
            originCount = count;


            for (pt::ptree::value_type& games : originTree.get_child("games"))
            {
                boost::optional<std::string> exeTest = games.second.get_optional<std::string>("exes");
                if (exeTest)
                {
                    QButtonGroup* group = new QButtonGroup();
                    QLineEdit* name = new QLineEdit(QString::fromStdString(games.second.get<std::string>("name")));
                    name->setFixedWidth(350);
                    QCheckBox* checkBox = new QCheckBox();
                    originLayout->addWidget(name, row, 0, Qt::AlignVCenter | Qt::AlignLeft);
                    row++;
                    for (auto& exe : games.second.get_child("exes"))
                    {
                        checkBox = new QCheckBox("Executable: " + QString::fromStdString(exe.second.data()));
                        group->addButton(checkBox);
                        originLayout->addWidget(checkBox, row, 0, Qt::AlignVCenter | Qt::AlignLeft);
                        row++;
                    }
                    originBtnGroupVector.push_back(group);
                }
            }
            originViewport->setLayout(originLayout);
            originScrollArea->setWidget(originViewport);
            tabWidget->addTab(originScrollArea, "Origin");
        }

        if (field("uplayFound").toBool())
        {
            uplayLayout = new QGridLayout();
            uplayScrollArea = new QScrollArea();
            uplayViewport = new QWidget();
            auto t = std::async(&ResultsPage::findUplayGames, this);
            t.get();
            int row = 0;
            int count = uplayTree.get<int>("games.count");
            uPlayCount = count;

            for (pt::ptree::value_type& games : uplayTree.get_child("games"))
            {
                boost::optional<std::string> exeTest = games.second.get_optional<std::string>("exes");
                if (exeTest)
                {
                    QButtonGroup* group = new QButtonGroup();
                    QLineEdit* name = new QLineEdit(QString::fromStdString(games.second.get<std::string>("name")));
                    name->setFixedWidth(350);
                    QCheckBox* checkBox = new QCheckBox();
                    uplayLayout->addWidget(name, row, 0, Qt::AlignVCenter | Qt::AlignLeft);
                    row++;
                    for (auto& exe : games.second.get_child("exes"))
                    {
                        checkBox = new QCheckBox("Executable: " + QString::fromStdString(exe.second.data()));
                        group->addButton(checkBox);
                        uplayLayout->addWidget(checkBox, row, 0, Qt::AlignVCenter | Qt::AlignLeft);
                        row++;
                    }
                    uplayBtnGroupVector.push_back(group);
                }
            }
            uplayViewport->setLayout(uplayLayout);
            uplayScrollArea->setWidget(uplayViewport);
            tabWidget->addTab(uplayScrollArea, "Uplay");
        }
        //: %1, %2, and %3 will be replaced with the number of Steam, Origin, and uPlay games, respectively
        setTitle(QString(tr("We found %1 Steam games, %2 Origin games, and %3 uPlay games.")).arg(QString::number(steamCount),
                                QString::number(originCount), QString::number(uPlayCount)));

        QPushButton* selectAllBtn = new QPushButton(tr("Select all"));
        QPushButton* deselectAllBtn = new QPushButton(tr("Deselect all"));
        QPushButton* invertBtn = new QPushButton(tr("Invert selection"));
        connect(selectAllBtn, &QPushButton::clicked, this, &ResultsPage::selectAll);
        connect(deselectAllBtn, &QPushButton::clicked, this, &ResultsPage::deselectAll);
        connect(invertBtn, &QPushButton::clicked, this, &ResultsPage::invert);

        top_layout->addWidget(tabWidget);
        QHBoxLayout* boxLayout = new QHBoxLayout();
        boxLayout->addWidget(selectAllBtn);
        boxLayout->addWidget(deselectAllBtn);
        boxLayout->addWidget(invertBtn);
        top_layout->addLayout(boxLayout, 1, 0, 0);
        connect(tabWidget, &QTabWidget::currentChanged, this, &ResultsPage::tabSelected);
        setLayout(top_layout);
    }
}

/** Overloads the nextId function to add the selected games from the main results widget to a vector called "addedVector",
 * which will be added to the main database at a later point.
 * \see QWizardPage::nextId()
 */
int ResultsPage::nextId() const
{
    if (field("uplayFound").toBool() || field("steamFound").toBool() || field("originFound").toBool())
    {
        int steamOffset = 0;
        for (auto i : btnGroup->buttons())
        {
            if (i->isChecked())
            {
                std::cout << "Adding " << i->text().toLocal8Bit().constData() << std::endl;
                addedVector.push_back(steamVector.at(steamOffset));
            }
            steamOffset++;
        }
        for (auto i : originBtnGroupVector)
        {
            for (auto group : i->buttons())
            {
                if (group->isChecked())
                {
                    QDir path = group->text().remove(tr("Executable: "));
                    std::string name = QString(QDir::cleanPath(path.filePath("").remove(originRoot.filePath((""))))).toLocal8Bit().constData();
                    std::vector<std::string> strSplit;
                    boost::split(strSplit, name, boost::is_any_of("/"));
                    name = strSplit.at(1);

                    std::cout << "Adding " << name << std::endl;
                    unsigned int count = db.getGameCount();
                    addedVector.push_back({count, QString::fromStdString(name), originRoot.filePath(QString::fromStdString(name)), path.filePath(""), "", 2});
                }
            }
        }
        for (auto i : uplayBtnGroupVector)
        {

            for (auto group : i->buttons())
            {
                if (group->isChecked())
                {
                    QDir path = group->text().remove(tr("Executable: "));
                    std::string name = QString(QDir::cleanPath(path.filePath("").remove(uplayRoot.filePath((""))))).toLocal8Bit().constData();
                    std::vector<std::string> strSplit;
                    boost::split(strSplit, name, boost::is_any_of("/"));
                    name = strSplit.at(1);

                    std::cout << "Adding " << name << std::endl;
                    unsigned int count = db.getGameCount();
                    addedVector.push_back({count, QString::fromStdString(name), uplayRoot.filePath(QString::fromStdString(name)), path.filePath(""), "", 3});
                }
            }
        }
    }
    return pages::FINAL;
}

/** Slot to set all buttons to checked */
void ResultsPage::selectAll()
{
    for (auto i : btnGroup->buttons())
    {
        i->setChecked(true);
    }
}

/** Slot to set all buttons to unchecked */
void ResultsPage::deselectAll()
{
    for (auto i : btnGroup->buttons())
    {
        i->setChecked(false);
    }
}
/** Slot to set all buttons to the opposite state */
void ResultsPage::invert()
{
    for (auto i : btnGroup->buttons())
    {
        i->setChecked(!i->isChecked());
    }
}

/** Slot to update the title based upon the tab. Steam (yet) doesn't generate multiple executables, so the 2nd part
 * isn't needed.
 */
void ResultsPage::tabSelected()
{
    if (tabWidget->currentIndex() >= 1)
    {
        setSubTitle(tr("Change the title for each game by clicking the text box and editing. Choose which executable to use from the tree view."));
    }
    else
    {
        setSubTitle(tr("Change the title for each game by clicking the text box and editing."));
    }
}

/** Using steamRoot, which is initialized earlier in the wizard, find the location of every Steam library folder, and
 * run the parseAcf function on every *.acf file found within the directory. This file gives basic information about the game;
 * such as directory within steamapps, name, etc.
 * \see findOriginGames()
 * \see findUplayGames()
 */
void ResultsPage::findSteamGames()
{
    QDir steamAppsDir = steamRoot.filePath("steamapps");
    if (!steamAppsDir.exists())
    {
        steamAppsDir = steamRoot.filePath("SteamApps");
    }
    pt::ptree libraryFolders;
    pt::read_info(steamAppsDir.filePath("libraryfolders.vdf").toLocal8Bit().constData(), libraryFolders);
    steamDirectoryList.append(steamRoot.filePath(""));
    QString pathString = "" + steamDirectoryList.at(0) + "\n";

    for (auto kv : libraryFolders.get_child("LibraryFolders"))
    {
        if (std::isdigit(static_cast<int>(*kv.first.data())))
        {
            std::string path = kv.second.data();
            QDir dir(QString::fromStdString(path));
            if (dir.exists())
            {
                steamDirectoryList.append(dir.filePath(""));
                pathString += dir.filePath("");
                pathString += "\n";
            }
        }
    }

    parseAcf(steamRoot);
}

/** Using originRoot, which is initialized earlier in the wizard, utilize `recursiveFindFiles()` to find every executable within
 * each respective directory. Some directories will contain more than one executable, so it's up to the user to select the correct one.
 * \see findSteamGames()
 * \see findUplayGames()
 */
void ResultsPage::findOriginGames()
{
    originRoot.setFilter(QDir::Dirs | QDir::NoDotAndDotDot | QDir::NoSymLinks);
    QStringList folderList = originRoot.entryList();
    int count = 0;

    for (auto i : folderList)
    {
        if (i != "DownloadCache")
        {
            pt::ptree& node = originTree.add("games.game", "");
            QDir dir(originRoot.absoluteFilePath(i));
            dir.setNameFilters(QStringList("*.exe"));
            dir.setFilter(QDir::Files | QDir::NoDotAndDotDot | QDir::NoSymLinks);
            QStringList test = recursiveFindFiles(dir);
            node.put("name", dir.dirName().toLocal8Bit().constData());
            for (auto exe : test)
            {
                node.add("exes.exe", exe.toLocal8Bit().constData());
            }
            count++;
        }
    }
    originTree.add("games.count", count);
}

/** Using uplayRoot, which is initialized earlier in the wizard, utilize `recursiveFindFiles()` to find every executable within
 * each respective directory. Some directories will contain more than one executable, so it's up to the user to select the correct one.
 * \see findSteamGames()
 * \see findOriginGames()
 */
void ResultsPage::findUplayGames()
{
    uplayRoot.setFilter(QDir::Dirs | QDir::NoDotAndDotDot | QDir::NoSymLinks);
    QStringList folderList = uplayRoot.entryList();
    QStringList ignoreList;
    ignoreList << "cache"
               << "data"
               << "locales"
               << "logs";
    int count = 0;

    for (auto i : folderList)
    {
        if (!ignoreList.contains(i))
        {
            pt::ptree& node = uplayTree.add("games.game", "");
            QDir dir(uplayRoot.absoluteFilePath(i));
            dir.setNameFilters(QStringList("*.exe"));
            dir.setFilter(QDir::Files | QDir::NoDotAndDotDot | QDir::NoSymLinks);
            QStringList test = recursiveFindFiles(dir);
            node.put("name", dir.dirName().toLocal8Bit().constData());
            for (auto exe : test)
            {
                node.add("exes.exe", exe.toLocal8Bit().constData());
            }
            count++;
        }
    }

    uplayTree.add("games.count", count);
}

/** Debug function to print out a ptree */
void ResultsPage::printTree(pt::ptree &pt, int level)
{
    if (pt.empty())
    {
        std::cerr << "\""<< pt.data()<< "\"";
    }
    else
    {
        auto indent = [&](int level){ std::string s; for (int i=0; i<level; i++) s += "  "; return s; };
        if (level) std::cerr << std::endl;
        std::cerr << indent(level) << "{" << std::endl;
        for (pt::ptree::iterator pos = pt.begin(); pos != pt.end();)
        {
            std::cerr << indent(level+1) << "\"" << pos->first << "\": ";
            printTree(pos->second, level + 1);
            ++pos;
            if (pos != pt.end())
            {
                std::cerr << ",";
            }
            std::cerr << std::endl;
        }
        std::cerr << indent(level) << " }";
    }
}

/** For a given steamRoot directory, find every appmanifest file, parse it and then use the retrived id to parse the appinfo.vdf
 * file to ascertain extended information; such as executables included within the game. This currently only retrieves one exe, more
 * work will need to go into creating a fast and efficient algorithm to parse multiple games.
 * \param steamRoot The root directory to user's Steam installation.
 */
void ResultsPage::parseAcf(QDir steamRoot)
{
    // TODO: This stuff needs its own thread
    QString vdfPath = steamRoot.filePath("appcache/appinfo.vdf");
    qDebug() << "Parsing Steam vdf, located at:" << vdfPath;
    auto games = SteamVdfParse::parseVdf(vdfPath.toLocal8Bit().constData());

    for (QString iter : steamDirectoryList)
    {
        QDir steamAppsDir(iter);
        if (steamAppsDir.exists("SteamApps"))
        {
            steamAppsDir = steamAppsDir.filePath("SteamApps");
        }
        else
        {
            steamAppsDir = steamAppsDir.filePath("steamapps");
        }

        QStringList fileList = steamAppsDir.entryList(QStringList("*.acf"), QDir::Files | QDir::NoSymLinks);

        for (auto fileIter : fileList)
        {
            boost::property_tree::ptree fileTree;
            std::string acfDir = steamAppsDir.filePath(fileIter).toLocal8Bit().constData();
            boost::property_tree::info_parser::read_info(acfDir, fileTree);

            QString name;
            QString path = steamAppsDir.filePath("common/" + QString::fromStdString(fileTree.get<std::string>("AppState.installdir")));

            boost::optional<std::string> nameTest = fileTree.get_optional<std::string>("AppState.name");
            if (!nameTest)
            {
                nameTest = fileTree.get_optional<std::string>("AppState.UserConfig.name");
            }

            if (nameTest)
            {
                name = QString::fromStdString(nameTest.get());
            }
            else
            {
                name = QDir(path).dirName();
            }

            // TODO: Either add SteamID to db, or add getGameByPath

            if (!std::get<0>(db.isExistant(name)))
            {
                QString exe;
                QString args;

                int id;
                boost::optional<int> idTest = fileTree.get_optional<int>("AppState.appID");
                if (!idTest)
                {
                    idTest = fileTree.get_optional<int>("AppState.appid");
                }

                id = idTest.get();
                try
                {
                    auto game = games.at(id);
                    auto launch = game.pt.get_child("config.launch");
                    boost::optional<std::string> oslistTest = game.pt.get_optional<std::string>("common.oslist");
                    if (oslistTest)
                    {

                        // Loop through the 0, 1, and 2 configurations
                        for (auto pair : launch)
                        {
                            boost::property_tree::ptree section = pair.second;

                            QString oslist = QString::fromStdString(section.get("config.oslist", "windows"));

#if defined(__linux__)
                            if (oslist == "linux")

#elif defined(_WIN32) || defined(_WIN64)
                            if (oslist == "windows")
#elif defined(__APPLE__)
                                if (oslist == "macos")
#endif
                            {
                                exe = QDir(path).filePath(QString::fromStdString(section.get<std::string>("executable")));
                                exe = QString(QDir::cleanPath(exe));
                                path = QDir(path).filePath(QString::fromStdString(section.get("workingdir", "")));
                                path = QString(QDir::cleanPath(path));
                                args = QString::fromStdString(section.get("arguments", ""));
                                break;
                            }
                        }
                    }
                    else
                    {
                        exe = QDir(path).filePath(QString::fromStdString(launch.get<std::string>("0.executable")));
                        exe = QString(QDir::cleanPath(exe));
                        path = QDir(path).filePath(QString::fromStdString(launch.get("0.workingdir", "")));
                        path = QString(QDir::cleanPath(path));
                        args = QString::fromStdString(launch.get("0.arguments", ""));
                    }
                }
                catch (const std::out_of_range&)
                {
                    qDebug() << "The game id:" << id << "was not found in the Steam appinfo.vdf.";
                    QStringList exeList = QDir(path).entryList(QDir::Files | QDir::NoSymLinks | QDir::Executable);

                    QFileDialog exeDialog;
                    exeDialog.setWindowTitle(tr("Select Executable"));
                    exeDialog.setFileMode(QFileDialog::ExistingFile);
                    exeDialog.setDirectory(path);
                    if (exeDialog.exec())
                    {
                        exe = exeDialog.selectedFiles().at(0);
                    }
                }

//                db.addGame(name, path, exe, args);
//                refreshGames();
                steamVector.push_back({0, name, path, exe, args, 1});
            }
        }
    }
}

/** For a given dir, find every file contained within. It's up to the implementer of this function to insure that the
 * QDir has the relevant filters.
 * \param dir Directory to search in.
 */
QStringList ResultsPage::recursiveFindFiles(QDir dir)
{
    QStringList dirList;
    QDirIterator it(dir, QDirIterator::Subdirectories);

    while (it.hasNext())
    {
        QDir cur = it.next();
        bool found = false;
        for (auto foundIter : dirList)
        {
            if (QDir(foundIter).dirName() == cur.dirName())
            {
                found = true;
                break;
            }
        }
        if (!found && cur.dirName() != QDir::currentPath())
        {
            dirList.append(cur.filePath(""));
        }
    }

    return dirList;
}

/** Initializes the final page. This function is called when the NextButton on the previous page is clicked,
 * which allows the state of the previous page to be transferred to this one.
 * The function then loops through the addedVector and adds the games to the database.
*/
void FinalPage::initializePage()
{
    std::sort(addedVector.begin(), addedVector.end(), [&](const Game& g1, const Game& g2){return g1.gameName < g2.gameName;});
    db.addGames(addedVector);
    setSubTitle(QString(tr("Added %1 games to the database. Click finish to complete the wizard.")).arg(addedVector.size()));
}

/** Overloads the nextId function */
int IntroPage::nextId() const
{
    return pages::DRM;
}

/** Overloads the nextId function */
int DRMPage::nextId() const
{
    return pages::RESULTS;
}
