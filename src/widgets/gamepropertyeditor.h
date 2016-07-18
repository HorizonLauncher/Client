#ifndef GAMEPROPERTYEDITOR_H
#define GAMEPROPERTYEDITOR_H

#include "../database.h"

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>

class GamePropertyEditor : public QWidget
{
    Q_OBJECT

public:
    GamePropertyEditor(Game game, QSettings* palette, QWidget* parent = 0);

private slots:
    void editBannerClicked();
    void saveGameProperties();

private:
    int gameID;

    QString bannerPath;
    QLabel* imageLabel;
    QPushButton* editBannerBtn;

    QLineEdit* titleEdit;
    QLineEdit* executableEdit;
    QLineEdit* wdEdit;
    QLineEdit* argsEdit;
    QComboBox* drmComboBox;
    QLineEdit* developerEdit;
    QLineEdit* publisherEdit;
    QLineEdit* releaseDateEdit;
    QLineEdit* genreEdit;
};

#endif
