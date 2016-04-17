#ifndef _SETTINGS_H_
#define _SETTINGS_H_

#include "Library.h"

#include <QtWidgets>
#include <QSettings>

/** Settings class.
* Class to handle the settings section of the launcher
*/
class Settings : public QWidget
{
    Q_OBJECT
private:
    void pickSetColor(int id);
    void init(QSettings* p);
    void setMultipleExec(bool multipleExec);
    void setFulltab(bool fulltab);

    QStackedWidget* stack;

    QPushButton* bodyColor;
    QPushButton* navbarBG;
    QPushButton* navbarHover;
    QPushButton* navbarSelected;
    QPushButton* titleBarColor;
    QPushButton* activeElement;
    QPushButton* inactiveSelection;
    QPushButton* hoverSelection;
    QPushButton* darkElement;
    QPushButton* lightText;
    QPushButton* darkText;
    QPushButton* subText;
    QPushButton* primaryBase;
    QPushButton* secondaryBase;
    QPushButton* tertiaryBase;
    QPushButton* darkestBase;

public slots:
    void confirmClearDb();
    void resetColors();
    void updateColor(int id, QColor color);

private slots:
    void exportTheme();
    void importTheme();

public:
    explicit Settings(QSettings* p, QWidget* parent = 0);

signals:
    void didUpdateFeedURLs();
};

#endif
