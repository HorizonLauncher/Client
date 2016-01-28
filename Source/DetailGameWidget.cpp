#include "DetailGameWidget.h"

DetailGameWidget::DetailGameWidget(Game game, QSettings* palette, QWidget* parent) : QWidget(parent)
{
    //mainLayout = new QHBoxLayout(this);

    QHBoxLayout* widgetLayout = new QHBoxLayout(this);
    QLabel* imageLabel = new QLabel("");
    imageLabel->setStyleSheet("background-image: url();");

    QPixmap imageMap(":SystemMenu/Images/LibraryGridPlaceholder.png");
    imageLabel->setPixmap(imageMap.scaledToHeight(this->height()));
    widgetLayout->addWidget(imageLabel);

    //Create the main title label and sublabels
    gameTitleWidget = new QWidget(this);
    QVBoxLayout* gameTitleWidgetLayout = new QVBoxLayout(gameTitleWidget);
    QLabel* titleLabel = new QLabel(game.gameName);
    QLabel* hoursLabel = new QLabel("999 Hours");
    gameTitleWidgetLayout->addWidget(titleLabel);
    gameTitleWidgetLayout->addWidget(hoursLabel);
    gameTitleWidget->setLayout(gameTitleWidgetLayout);
    widgetLayout->addWidget(gameTitleWidget);

    widgetLayout->addStretch();

    //Create the detailswidget
    this->gameDetailsWidget = new QWidget(this);
    QVBoxLayout* detailsLayout = new QVBoxLayout(gameDetailsWidget);
    gameDetailsWidget->setLayout(detailsLayout);
    QLabel* reviewsLabel = new QLabel("Reviews: 7.5/10");
    QLabel* releaseDateLabel = new QLabel("Release Date: 10/20/30");
    detailsLayout->addWidget(reviewsLabel);
    detailsLayout->addWidget(releaseDateLabel);
    widgetLayout->addWidget(gameDetailsWidget);

    //Create mod/achievement support widget
    this->gameSupportInfoWidget = new QWidget(this);
    QLabel* gameModeLabel = new QLabel("Mode: SinglePlayer");
    QLabel* modSupportLabel = new QLabel ("Mods: Supported");
    QLabel* achievementSupportLabel = new QLabel ("Achievements: Supported");
    QVBoxLayout* supportWidgetLayout = new QVBoxLayout(this->gameSupportInfoWidget);
    supportWidgetLayout->addWidget(gameModeLabel);
    supportWidgetLayout->addWidget(modSupportLabel);
    supportWidgetLayout->addWidget(achievementSupportLabel);
    widgetLayout->addWidget(gameSupportInfoWidget);

    this->setLayout(widgetLayout);

    //this->setMinimumSize(1000, 200);
    //this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);


}

