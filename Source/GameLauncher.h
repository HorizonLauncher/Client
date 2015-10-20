#ifndef GAMELAUNCHER_H
#define GAMELAUNCHER_H

#include <QProcess>
#include <QWidget>
#include <QSettings>

/** GameLauncher class
 * Class to handle launching games
 */
class GameLauncher : public QWidget
{

public:
    GameLauncher();
    ~GameLauncher();
    bool isProcessRunning() const;
    void runProcess(QString file, QString workingDirectory);
    void runProcessWithArgs(QString file, QString workingDirectory, QString args);

private slots:
    void onLaunchError(QProcess::ProcessError error);
    void finished(int exitCode, QProcess::ExitStatus exitStatus);

private:
    QProcess* runningProcess;
};

#endif //GAMELAUNCHER_H
