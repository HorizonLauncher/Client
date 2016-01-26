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
        gameTitleWidget->setLayout(gameTitleWidgetLayout);

        widgetLayout->addWidget(gameTitleWidget);
        widgetLayout->addStretch();

         this->gameDetailsWidget = new QWidget(this);
         QVBoxLayout* detailsLayout = new QVBoxLayout(gameDetailsWidget);
         gameDetailsWidget->setLayout(detailsLayout);

         QLabel* directoryLabel = new QLabel(game.gameDirectory);
        QLabel* DRMLabel = new QLabel("DRM: " + QString::number(game.drm));
         detailsLayout->addWidget(directoryLabel);
        detailsLayout->addWidget(DRMLabel);
        widgetLayout->addWidget(gameDetailsWidget);

   this->setLayout(widgetLayout);

   //this->setMinimumSize(1000, 200);
   //this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);


}

