#include "DetailGameWidget.h"

DetailGameWidget::DetailGameWidget(Game game, QWidget* parent) : QWidget(parent)
{
  mainLayout = new QHBoxLayout();

  imageLabel = new QLabel("test");
  mainLayout->addWidget(imageLabel);
  this->setLayout(mainLayout);
//  //Create the main title label and sublabels
//  gameTitleWidget = new QWidget(this);
//  QVBoxLayout* gameTitleLayout = new QVBoxLayout();

//  QLabel* titleLabel = new QLabel(game.gameName);
//  titleLabel->setStyleSheet("background-color: red");
//  gameTitleLayout->addWidget(titleLabel);
//    gameTitleWidget->setLayout(gameTitleLayout);
//  mainLayout->addWidget(gameTitleWidget);

//  this->gameDetailsWidget = new QWidget(this);
//  QGridLayout* detailsLayout = new QGridLayout();
//  gameDetailsWidget->setLayout(detailsLayout);
//  QLabel* someLabel = new QLabel(game.gameDirectory);
//  detailsLayout->addWidget(someLabel, 0, 0, 1, 1);
//  mainLayout->addWidget(gameDetailsWidget);
}

