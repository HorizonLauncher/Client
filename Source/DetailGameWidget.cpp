#include "DetailGameWidget.h"
#include <QSpacerItem>

DetailGameWidget::DetailGameWidget(Game game, QSettings* palette, QWidget* parent) : QWidget(parent)
{
    this->palette = palette;
    this->setObjectName("DetailGameWidget");
    this->setStyleSheet("background-color: rgba(0,0,0,0)");

    QHBoxLayout* widgetLayout = new QHBoxLayout(this);
    widgetLayout->setSpacing(0);
    QLabel* imageLabel = new QLabel("");


    QPixmap imageMap(":SystemMenu/Images/LibraryGridPlaceholder.png");
    imageLabel->setPixmap(imageMap.scaledToHeight(100));
    imageLabel->setContentsMargins(5,5,5,5);
    widgetLayout->addWidget(imageLabel);

    //Create the main title label and sublabels
    gameTitleWidget = new QWidget(this);
    gameTitleWidget->setAutoFillBackground(false);
    QVBoxLayout* gameTitleWidgetLayout = new QVBoxLayout(gameTitleWidget);
    QLabel* titleLabel = new QLabel(game.gameName);
    titleLabel->setStyleSheet("font-family: Roboto; font-style: normal; font-size: 16pt");
    QLabel* hoursLabel = new QLabel("999 Hours");
    styleLabel(hoursLabel);
    gameTitleWidgetLayout->addWidget(titleLabel);
    gameTitleWidgetLayout->addWidget(hoursLabel);
    QWidget* titleSpacerWidget = new QWidget(this);
    titleSpacerWidget->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Expanding);
    gameTitleWidgetLayout->addWidget(titleSpacerWidget);
    gameTitleWidget->setLayout(gameTitleWidgetLayout);
    widgetLayout->addWidget(gameTitleWidget);

    QWidget* spacerWidget = new QWidget(this);
    QHBoxLayout* layout = new QHBoxLayout(spacerWidget);
    layout->addWidget(new QLabel (""));
    //spacerWidget->setStyleSheet("background-color: red");
    //spacerWidget->setMinimumWidth(200 - game.gameName.length()); //Yay hacks!
    spacerWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    widgetLayout->addWidget(spacerWidget);

    //Create the detailswidget
    this->gameDetailsWidget = new QWidget(this);
    QVBoxLayout* detailsLayout = new QVBoxLayout(gameDetailsWidget);
    gameDetailsWidget->setLayout(detailsLayout);
    QLabel* reviewsLabel = new QLabel("Reviews: 7.5/10");
    QLabel* releaseDateLabel = new QLabel("Release Date: 10/20/30");
    QWidget* detailsSpacerWidget = new QWidget(this);
    styleLabel(reviewsLabel);
    styleLabel(releaseDateLabel);
    detailsSpacerWidget->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Expanding);
    detailsLayout->addWidget(reviewsLabel);
    detailsLayout->addWidget(releaseDateLabel);
    detailsLayout->addWidget(detailsSpacerWidget);
    widgetLayout->addWidget(gameDetailsWidget);

    //Create mod/achievement support widget
    this->gameSupportInfoWidget = new QWidget(this);
    QLabel* gameModeLabel = new QLabel("Mode: SinglePlayer");
    QLabel* modSupportLabel = new QLabel ("Mods: Supported");
    QLabel* achievementSupportLabel = new QLabel ("Achievements: Supported");
    styleLabel(gameModeLabel);
    styleLabel(modSupportLabel);
    styleLabel(achievementSupportLabel);
    QVBoxLayout* supportWidgetLayout = new QVBoxLayout(this->gameSupportInfoWidget);
    QWidget* gameSupportInfoSpacerWidget = new QWidget (this);
    gameSupportInfoSpacerWidget->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Expanding);
    supportWidgetLayout->addWidget(gameModeLabel);
    supportWidgetLayout->addWidget(modSupportLabel);
    supportWidgetLayout->addWidget(achievementSupportLabel);
    supportWidgetLayout->addWidget(gameSupportInfoSpacerWidget);
    widgetLayout->addWidget(gameSupportInfoWidget);

    //Create Button
    QPushButton* playButton = new QPushButton();
    playButton->setStyleSheet("background-image: url(:SystemMenu/Icons/LaunchIcon.png);"
                              "background-repeat: no-repeat;"
                              "background-color: white");
    playButton->setMinimumSize(40, 40);
    widgetLayout->addWidget(playButton);

    this->setLayout(widgetLayout);
}

void DetailGameWidget::enterEvent(QEvent* event)
{
   //this->setStyleSheet("background-color: " + this->palette->value("Primary/HoverSelection").toString());
   this->currentlyHoveredOver = true;
   this->setCursor(Qt::PointingHandCursor);

}

void DetailGameWidget::leaveEvent(QEvent* event)
{
    //this->setStyleSheet("background-color: " + this->palette->value("Primary/SecondaryBase").toString());
    this->currentlyHoveredOver = false;
    this->setCursor(Qt::ArrowCursor);

}

/**
  Applies default font and color styling to a label for display
  @param label The label to style
  */
void DetailGameWidget::styleLabel(QLabel *label){
    label->setStyleSheet("font-family: Roboto; font-style: normal; font-size: 10pt");
}

void DetailGameWidget::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);  
    QString imageString;
    QPixmap imageMap(":SystemMenu/Images/LibraryListPlaceholderBG.png");
    painter.drawPixmap(0,0,this->width(), this->height(), imageMap);
}

