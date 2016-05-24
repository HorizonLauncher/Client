#include "Init.h"
#include "BuddyList.h"
#include "SetupWizard.h"
#include "Defines.h"
#include "Library.h"
#include "Database.h"

#include <QApplication>
#include <QFile>
#include <QIcon>
#include <QTextStream>

#include <QCommandLineParser>
#include <QCommandLineOption>

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>

#ifdef Q_OS_WIN
    #include "BorderlessWindow.h"
#else
    #include "UnixWindow.h"
#endif

int main(int argc, char* argv[])
{
    QCoreApplication::addLibraryPath("./");

    QApplication::setStyle("fusion");
    QApplication* application = new QApplication(argc, argv);
    application->setApplicationName("Horizon Launcher");

    QTranslator horizonTranslator;
    horizonTranslator.load("horizon_" + QLocale::system().name());
    application->installTranslator(&horizonTranslator);

    QDir configFolder(CONFIG_FOLDER);
    if (!configFolder.exists())
    {
        configFolder.cdUp();
        configFolder.mkdir("HorizonLauncher");
    }

    bool dbExists = QFile(QDir(CONFIG_FOLDER).filePath("horizon.db")).exists();

    if (!Library::db.init())
    {
        QMessageBox error;
        error.critical(0, QWidget::tr("Error!"), QWidget::tr("An error occurred while trying to load the database."));
        exit(EXIT_FAILURE);
    }

    QCommandLineParser parser;

    parser.setApplicationDescription("Horizon Launcher");
    parser.addHelpOption();

    QCommandLineOption noLaunchOption(QStringList() << "n" << "nolaunch",
                                  QWidget::tr("Don't launch Horizon's GUI -- exit after processing command-line arguments."));
    parser.addOption(noLaunchOption);

    QCommandLineOption jsonOption(QStringList() << "json",
                                  QWidget::tr("Output in JSON format instead of human-readable."));
    parser.addOption(jsonOption);

    QCommandLineOption listOption(QStringList() << "l" << "list",
                                  QWidget::tr("List all games in the Horizon database."));
    parser.addOption(listOption);

    parser.process(*application);

    if (parser.isSet(listOption))
    {
        QTextStream sout(stdout);

        if (parser.isSet(jsonOption))
        {
            QJsonArray gamesJSON;
            for (Game game : Library::db.getGames())
            {
                QJsonObject gameJSON;
                gameJSON["id"] = QString::number(game.id);
                gameJSON["name"] = game.gameName;
                gameJSON["directory"] = game.gameDirectory;
                gameJSON["executable"] = game.executablePath;
                gameJSON["arguments"] = game.arguments;
                gameJSON["drm"] = game.drm;

                gamesJSON.append(gameJSON);
            }

            QJsonDocument doc(gamesJSON);

            sout << doc.toJson(QJsonDocument::Compact) << "\r\n";
            sout.flush();
        }
        else
        {
            QList<Game> games = Library::db.getGames();
            int largestID = QString::number(games[0].id).length();
            int largestName = games[0].gameName.length();
            int largestDir = games[0].gameDirectory.length();
            int largestBin = games[0].executablePath.length();
            int largestArgs = games[0].arguments.length();
            int largestDRM = Database::getDRMString(games[0].drm).length();

            for (Game game : games)
            {
                int idLen = QString::number(game.id).length();
                int nameLen = game.gameName.length();
                int dirLen = game.gameDirectory.length();
                int binLen = game.executablePath.length();
                int argLen = game.arguments.length();
                int drmLen = Database::getDRMString(game.drm).length();

                if (idLen > largestID)
                {
                    largestID = idLen;
                }
                if (nameLen > largestName)
                {
                    largestName = nameLen;
                }
                if (dirLen > largestDir)
                {
                    largestDir = dirLen;
                }
                if (binLen > largestBin)
                {
                    largestBin = binLen;
                }
                if (argLen > largestArgs)
                {
                    largestArgs = argLen;
                }
                if (drmLen > largestDRM)
                {
                    largestDRM = drmLen;
                }
            }

            sout << QString("ID").leftJustified(largestID, ' ') + "\t" +
                    QString("NAME").leftJustified(largestName, ' ') + "\t" +
                    QString("DIR").leftJustified(largestDir, ' ') + "\t" +
                    QString("BIN").leftJustified(largestBin, ' ') + "\t" +
                    QString("ARGS").leftJustified(largestArgs, ' ') + "\t" +
                    QString("DRM").leftJustified(largestDRM, ' ') + "\r\n";
            for (Game game : games)
            {
                QJsonObject gameJSON;
                sout << QString::number(game.id).leftJustified(largestID, ' ') + "\t" +
                        QString(game.gameName).leftJustified(largestName, ' ') + "\t" +
                        QString(game.gameDirectory).leftJustified(largestDir, ' ') + "\t" +
                        QString(game.executablePath).leftJustified(largestBin, ' ') + "\t" +
                        QString(game.arguments).leftJustified(largestArgs, ' ') + "\t" +
                        Database::getDRMString(game.drm).leftJustified(largestDRM, ' ') + "\r\n";
            }
            sout.flush();
        }
    }

    SetupWizard* wiz = new SetupWizard();

    #ifndef Q_OS_WIN
        // dynamic loading of the icon under Linux/UNIX
        application->setWindowIcon(QIcon(":/Resource/Icons/Horizon.ico"));
    #endif

    // Global stylesheet
    QFile stylesheet(":/Styles/Horizon.css");
    if (stylesheet.open(QFile::ReadOnly))
    {
        QString styleSheet = stylesheet.readAll();
        application->setStyleSheet(styleSheet);
    }

    entryPoint::initSettings(*application);
    entryPoint::initFonts(*application);

    if (!parser.isSet(noLaunchOption))
    {
        #ifdef Q_OS_WIN
            // Background color
            // This is only for WinApi window, Qt widgets use BorderlessWindow.css stylesheet
            HBRUSH windowBackground = CreateSolidBrush(RGB(15, 15, 15));

            // Create a Win window
            BorderlessWindow window(application, windowBackground, 1152, 648);
            window.setMinimumSize(1152, 648);
        #else
            // Create a Unix window
            UnixWindow window;
            window.setMinimumSize(1152, 648);
        #endif

        if (!dbExists)
        {
            wiz->show();
        }
        application->exec();
    }
    else
    {
        application->quit();
    }

    return 0;
}
