#include "gamepropertyeditor.h"
#include "stack/library.h"
#include "../defines.h"

#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPixmap>
#include <QFileDialog>
#include <QDir>
#include <QDateTime>

GamePropertyEditor::GamePropertyEditor(Game game, QSettings* palette, QWidget* parent)
{
    this->gameID = game.id;

    this->setStyleSheet("background-color: " + palette->value("Primary/SecondaryBase").toString() + ";} "
                        "QPushButton {"
                            "color: " + palette->value("Primary/LightText").toString() + "; "
                            "background-color: " + palette->value("Primary/DarkElement").toString() + "; "
                            "border: none; margin: 0px; padding: 0px;"
                            "font-family: Roboto; "
                            "font-weight: 300;"
                            "font-size: 14px;"
                        "} "
                        "QPushButton:hover {"
                            "background-color: " + palette->value("Primary/InactiveSelection").toString() + ";"
                        "} "
                        "QLabel {"
                            "color: " + palette->value("Primary/DarkText").toString() + ";"
                            "font-family: Roboto; "
                            "font-weight: 300;"
                            "font-size: 14px;"
                        "} "
                        "QLineEdit {"
                            "color: " + palette->value("Primary/LightText").toString() + ";"
                            "font-family: Roboto; "
                            "font-weight: 300;"
                            "font-size: 14px;"
                            "border: none;"
                            "padding: 4px;"
                            "background-color: " + palette->value("Primary/TextHighlight").toString() + ";"
                        "} "
                        "QComboBox {"
                            "color: " + palette->value("Primary/LightText").toString() + ";"
                            "font-family: Roboto; "
                            "font-weight: 300;"
                            "font-size: 14px;"
                        "}");

    this->setWindowTitle(game.gameName + " Details");

    QGridLayout* mainLayout = new QGridLayout(this);

    QVBoxLayout* leftLayout = new QVBoxLayout();
    mainLayout->addLayout(leftLayout, 0, 0);

    QString dispBannerPath;
    if (game.bannerPath.isEmpty())
    {
        QStringList backgrounds;
        backgrounds << ":res/image/LibraryGridPlaceholder.png"
                    << ":res/image/LibraryGridPlaceholder1.png"
                    << ":res/image/LibraryGridPlaceholder2.png"
                    << ":res/image/LibraryGridPlaceholder3.png"
                    << ":res/image/LibraryGridPlaceholder4.png";

        dispBannerPath = backgrounds[qrand() % backgrounds.length()];
        this->bannerPath = "";
    }
    else
    {
        dispBannerPath = game.bannerPath;
        this->bannerPath = game.bannerPath;
    }

    imageLabel = new QLabel("");
    QPixmap imagePixmap(dispBannerPath);
    imageLabel->setPixmap(imagePixmap);
    leftLayout->addWidget(imageLabel, 0, Qt::AlignHCenter);

    QPushButton* editBannerBtn = new QPushButton(tr("Change banner image"));
    editBannerBtn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    editBannerBtn->setStyleSheet("margin: 11px 0 0 11px; padding: 5px;");
    connect(editBannerBtn, &QPushButton::clicked, this, &GamePropertyEditor::editBannerClicked);
    leftLayout->addWidget(editBannerBtn, 0, Qt::AlignTop | Qt::AlignHCenter);

    leftLayout->addStretch();

    QVBoxLayout* rightLayout = new QVBoxLayout();
    rightLayout->setSpacing(10);
    mainLayout->addLayout(rightLayout, 0, 1);

    QVBoxLayout* titleLayout = new QVBoxLayout();
    titleLayout->setSpacing(0);
    titleLayout->addWidget(new QLabel(tr("Title:")));
    titleEdit = new QLineEdit();
    titleEdit->setText(game.gameName);
    titleLayout->addWidget(titleEdit);
    rightLayout->addLayout(titleLayout);

    QVBoxLayout* exeLayout = new QVBoxLayout();
    exeLayout->setSpacing(0);
    exeLayout->addWidget(new QLabel(tr("Executable Path:")));
    executableEdit = new QLineEdit();
    executableEdit->setText(game.executablePath);
    exeLayout->addWidget(executableEdit);
    rightLayout->addLayout(exeLayout);

    QVBoxLayout* wdLayout = new QVBoxLayout();
    wdLayout->setSpacing(0);
    wdLayout->addWidget(new QLabel(tr("Executable Working Directory:")));
    wdEdit = new QLineEdit();
    wdEdit->setText(game.gameDirectory);
    wdLayout->addWidget(wdEdit);
    rightLayout->addLayout(wdLayout);

    QVBoxLayout* argsLayout = new QVBoxLayout();
    argsLayout->setSpacing(0);
    argsLayout->addWidget(new QLabel(tr("Launch Options:")));
    argsEdit = new QLineEdit();
    argsEdit->setText(game.arguments);
    argsLayout->addWidget(argsEdit);
    rightLayout->addLayout(argsLayout);

    QVBoxLayout* drmLayout = new QVBoxLayout();
    drmLayout->setSpacing(0);
    drmLayout->addWidget(new QLabel(tr("DRM:")));
    drmComboBox = new QComboBox();
    drmComboBox->insertItem(0, "None", QVariant(0));
    drmComboBox->insertItem(1, "Steam", QVariant(1));
    drmComboBox->insertItem(2, "Origin", QVariant(2));
    drmComboBox->insertItem(3, "uPlay", QVariant(3));
    drmComboBox->setCurrentIndex(game.drm);
    drmLayout->addWidget(drmComboBox);
    rightLayout->addLayout(drmLayout);

    QVBoxLayout* devLayout = new QVBoxLayout();
    devLayout->setSpacing(0);
    devLayout->addWidget(new QLabel(tr("Developers:")));
    developerEdit = new QLineEdit();
    developerEdit->setText(game.developer);
    devLayout->addWidget(developerEdit);
    rightLayout->addLayout(devLayout);

    QVBoxLayout* pubLayout = new QVBoxLayout();
    pubLayout->setSpacing(0);
    pubLayout->addWidget(new QLabel(tr("Publishers:")));
    publisherEdit = new QLineEdit();
    publisherEdit->setText(game.publisher);
    pubLayout->addWidget(publisherEdit);
    rightLayout->addLayout(pubLayout);

    QVBoxLayout* releaseDateLayout = new QVBoxLayout();
    releaseDateLayout->setSpacing(0);
    releaseDateLayout->addWidget(new QLabel(tr("Release Date:")));
    releaseDateEdit = new QLineEdit();
    releaseDateEdit->setText(game.releaseDate);
    releaseDateLayout->addWidget(releaseDateEdit);
    rightLayout->addLayout(releaseDateLayout);

    QVBoxLayout* genreLayout = new QVBoxLayout();
    genreLayout->setSpacing(0);
    genreLayout->addWidget(new QLabel(tr("Genres:")));
    genreEdit = new QLineEdit();
    genreEdit->setText(game.genre);
    genreLayout->addWidget(genreEdit);
    rightLayout->addLayout(genreLayout);

    QHBoxLayout* bottomLayout = new QHBoxLayout();
    mainLayout->addLayout(bottomLayout, 1, 0, 1, 2);

    bottomLayout->addStretch();

    QPushButton* applyButton = new QPushButton("Apply");
    applyButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    applyButton->setStyleSheet("padding: 5px;");
    connect(applyButton, &QPushButton::clicked, this, &GamePropertyEditor::saveGameProperties);
    bottomLayout->addWidget(applyButton);

    QPushButton* closeButton = new QPushButton("Close");
    closeButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    closeButton->setStyleSheet("padding: 5px;");
    connect(closeButton, &QPushButton::clicked, [=]
    {
        this->hide();
        this->deleteLater();
    });
    bottomLayout->addWidget(closeButton);
}

void GamePropertyEditor::editBannerClicked()
{
    QFileDialog dlg;
    dlg.setFileMode(QFileDialog::ExistingFile);
    //: Please only translate "Images". Leave "(*.png *.xpm *.jpg)" alone.
    dlg.setNameFilter(tr("Images (*.png *.xpm *.jpg)"));

    if (dlg.exec())
    {
        QString filename = dlg.selectedFiles().at(0);
        QPixmap p(filename);

        if (!p.isNull())
        {
            QPixmap scaledP = p.scaled(364, 170);

            QDateTime dateTime;
            QString currentTime = QString::number(dateTime.toTime_t());

            QString filePath = CONFIG_FOLDER + QDir::separator() + "image" + QDir::separator() + "user_" + QString::number(this->gameID) + "_" + currentTime + ".png";
            this->bannerPath = filePath;
            scaledP.save(filePath, "PNG");

            imageLabel->setPixmap(scaledP);
        }
    }
}

void GamePropertyEditor::saveGameProperties()
{
    Game newGame;
    newGame.id = this->gameID;

    newGame.gameName = titleEdit->text();
    newGame.gameDirectory = executableEdit->text();
    newGame.executablePath = wdEdit->text();
    newGame.arguments = argsEdit->text();
    newGame.drm = drmComboBox->currentIndex();
    newGame.developer = developerEdit->text();
    newGame.publisher = publisherEdit->text();
    newGame.releaseDate = releaseDateEdit->text();
    newGame.genre = genreEdit->text();
    newGame.bannerPath = this->bannerPath;

    Library::db.updateGame(newGame);
}
