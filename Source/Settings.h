#pragma once
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
public:
    explicit Settings(QSettings* p, QWidget* parent = 0);
};
