#include "GameLauncher.h"

#include <QDebug>
#include <QMessageBox>

GameLauncher::GameLauncher()
    : runningProcess(new QProcess(this))
{
    connect(runningProcess, (void (QProcess::*) (int, QProcess::ExitStatus)) &QProcess::finished, this, &GameLauncher::finished);
    connect(runningProcess, (void (QProcess::*) (QProcess::ProcessError)) &QProcess::error, this, &GameLauncher::onLaunchError);
}

GameLauncher::~GameLauncher()
{
    delete runningProcess;
}

/** Launch a new QProcess using the passed exe and working directory.
 * \param file Location of the exe to run.
 * \param workingDirectory The directory that QProcess should spawn in.
*/
void GameLauncher::runProcess(QString file, QString workingDirectory)
{
    // TODO: Implement some threading
    if (!isProcessRunning())
    {
        qDebug() << "Launching:" << file << ", at" << workingDirectory;
        runningProcess->setWorkingDirectory(workingDirectory);
        runningProcess->setStandardErrorFile("error.txt");
        runningProcess->setStandardOutputFile("log.txt");
        runningProcess->start(file, QStringList());
        runningProcess->waitForStarted();
    }
}

/** Launch a new QProcess using the passed exe and working directory.
 * \param file Location of the exe to run.
 * \param workingDirectory The directory that QProcess should spawn in.
 * \param args String of arguments to launch the executable with.
*/
void GameLauncher::runProcessWithArgs(QString file, QString workingDirectory, QString args)
{
    // TODO: Implement some threading
    if (!isProcessRunning())
    {
        qDebug() << "Launching:" << file << ", at" << workingDirectory << "with " << args;
        runningProcess->setWorkingDirectory(workingDirectory);
        runningProcess->setStandardErrorFile("error.txt");
        runningProcess->setStandardOutputFile("log.txt");
        runningProcess->start(file, QStringList(args.split(" ")));
        runningProcess->waitForStarted();
    }
}

/** Check if a process is running already.
 * \return Success/failure upon completion.
*/
bool GameLauncher::isProcessRunning() const
{
    // We shall consider "Starting" to be running here too
    return runningProcess->state() != QProcess::NotRunning;
}

/** Attempt to handle process ending unexpectedly or fork.
 * \param exitCode Exit code to check.
 * \param exitStatus Status to check.
*/
void GameLauncher::finished(int exitCode, QProcess::ExitStatus exitStatus)
{
    if (exitCode != 0)
    {
        QMessageBox(QMessageBox::Warning, tr("Warning"), tr("The game finished, but it claims to have encountered an error")).exec();
    }
}

/** Handle errors before the process has launched.
 * \param error The error to translate.
*/
void GameLauncher::onLaunchError(QProcess::ProcessError error)
{
    switch (error)
    {
        case QProcess::FailedToStart:
            QMessageBox(QMessageBox::Critical, tr("Error"), tr("Could not start the game. Please double check that you are using the correct file to launch it.")).exec();
            break;
        case QProcess::Crashed:
            QMessageBox(QMessageBox::Warning, tr("Crash!"), tr("The launched game has crashed")).exec();
            break;
        default:
            // Other cases are errors unrelated to startup, so let's not handle them
            break;
    }
}
