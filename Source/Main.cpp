#include "Init.h"
#include "BuddyList.h"
#include "SetupWizard.h"
#include "Defines.h"

#include <QApplication>
#include <QFile>
#include <QDebug>
#include <iostream>
#include <QIcon>



#ifdef Q_OS_WIN
#include "BorderlessWindow.h"

#else
#include "UnixWindow.h"

#endif

int main(int argc, char* argv[])
{
    QApplication::setStyle("fusion");
    QApplication* application = new QApplication(argc, argv);

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
    SetupWizard* wiz = new SetupWizard();

    #ifndef Q_OS_WIN
        // dynamic loading of the icon under Linux/UNIX
        application->setWindowIcon(QIcon(":/SystemMenu/Icons/Horizon.ico"));
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

    #ifdef Q_OS_WIN
        // Background color
        // This is only for WinApi window, Qt widgets use BorderlessWindow.css stylesheet
        HBRUSH windowBackground = CreateSolidBrush(RGB(15, 15, 15));

        // Create a Win window
        BorderlessWindow window(application, windowBackground, 1152, 648);
        window.setMinimumSize(830, 550);
    #else
        // Create a Unix window
        UnixWindow window;
        window.setMinimumSize(830, 550);
    #endif

    // Launch
    if (!dbExists)
    {
        wiz->show();
    }
    application->exec();
    return 0;
}
