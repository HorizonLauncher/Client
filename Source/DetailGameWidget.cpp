#include "DetailGameWidget.h"
#include <QSpacerItem>

DetailGameWidget::DetailGameWidget(Game game, QSettings* palette, QWidget* parent) : QWidget(parent)
{
    this->palette = palette;
    this->setStyleSheet("background-color: " + palette->value("Primary/SecondaryBase").toString() + ";"
                        "QLayoutItem {"
                        "background-color : red"
                        "}");
    QHBoxLayout* widgetLayout = new QHBoxLayout(this);
    widgetLayout->setSpacing(0);
    QLabel* imageLabel = new QLabel("");
    imageLabel->setStyleSheet("background-image: url();");

    QPixmap imageMap(":SystemMenu/Images/LibraryGridPlaceholder.png");
    imageLabel->setPixmap(imageMap.scaledToHeight(100));
    imageLabel->setContentsMargins(5,5,5,5);
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

    QWidget* spacerWidget = new QWidget(this);
    QHBoxLayout* layout = new QHBoxLayout(spacerWidget);
    layout->addWidget(new QLabel (""));
    //spacerWidget->setStyleSheet("background-color: red");
    spacerWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    widgetLayout->addWidget(spacerWidget);

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

void DetailGameWidget::enterEvent(QEvent* event)
{
   this->setStyleSheet("background-color: " + this->palette->value("Primary/HoverSelection").toString());
   this->setCursor(Qt::PointingHandCursor);

}

void DetailGameWidget::leaveEvent(QEvent* event)
{
    this->setStyleSheet("background-color: " + this->palette->value("Primary/SecondaryBase").toString());
     this->setCursor(Qt::ArrowCursor);

}

