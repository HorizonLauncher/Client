#pragma once
#include "Database.h"

#include <QtWidgets>
#include <QSettings>

/** Settings UI namespace. */
namespace Ui {
    class Settings;
}
/** Settings class.
* Class to handle the settings section of the launcher
*/
class Settings : public QWidget
{
    Q_OBJECT
private:
    Ui::Settings *ui;
    Database db;
    void pickSetColor(int id);

public slots:
    void on_WizardButton_clicked();
    void on_ClearDatabaseButton_clicked();
    void on_ResetColors_clicked();
    void on_ManageNewsButton_clicked(); 
    void updateColor(int id, QColor color);
public:
    explicit Settings(QSettings* p, QWidget* parent = 0);
    ~Settings();
};
