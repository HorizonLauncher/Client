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
<<<<<<< HEAD
    void on_WizardButton_clicked();
    void on_ClearDatabaseButton_clicked();
    void on_ResetColors_clicked();
    void on_ManageNewsButton_clicked(); 
=======
    void confirmClearDb();
    void resetColors();

>>>>>>> 01d64a9d910d28dd718073562ee4d9f62b20335f
    void updateColor(int id, QColor color);
public:
    explicit Settings(QSettings* p, QWidget* parent = 0);
};
