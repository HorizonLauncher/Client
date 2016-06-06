#ifndef GAMELAUNCHER_H
#define GAMELAUNCHER_H

#include <QProcess>
#include <QObject>

/** GameLauncher class
 * Class to handle launching games
 */
class GameLauncher : public QObject
{
    Q_OBJECT

public:
    void runProcess(QString file, QString workingDirectory);
    void runProcessWithArgs(QString file, QString workingDirectory, QString args);

private slots:
    void onLaunchError(QProcess::ProcessError error);
    void finished(int exitCode, QProcess::ExitStatus exitStatus);
};

#endif //GAMELAUNCHER_H
