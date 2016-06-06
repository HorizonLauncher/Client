#include "detailgamewidget.h"

#include <QSpacerItem>

DetailGameWidget::DetailGameWidget(Game game, QSettings* palette, QString background, QWidget* parent) : QWidget(parent)
{
    this->palette = palette;
    this->setObjectName("DetailGameWidget");
    this->setStyleSheet("background-color: rgba(0, 0, 0, 0)");

    QHBoxLayout* widgetLayout = new QHBoxLayout(this);
    widgetLayout->setSpacing(0);

    QLabel* imageLabel = new QLabel("");
    QPixmap imageMap(background);
    imageLabel->setPixmap(imageMap.scaledToHeight(100));
    imageLabel->setContentsMargins(5, 5, 5, 5);
    widgetLayout->addWidget(imageLabel);

    //Create the main title label and sublabels
    gameTitleWidget = new QWidget(this);
    gameTitleWidget->setAutoFillBackground(false);
    QVBoxLayout* gameTitleWidgetLayout = new QVBoxLayout(gameTitleWidget);
    widgetLayout->addWidget(gameTitleWidget);

    QLabel* titleLabel = new QLabel(game.gameName);
    titleLabel->setStyleSheet("font-family: Roboto; font-weight: 300; font-size: 14pt");
    gameTitleWidgetLayout->addWidget(titleLabel);

    QLabel* hoursLabel = new QLabel(tr("999 Hours"));
    styleLabel(hoursLabel);
    gameTitleWidgetLayout->addWidget(hoursLabel);

    widgetLayout->addStretch();

    this->gameDetailsWidget = new QWidget(this);
    QVBoxLayout* detailsLayout = new QVBoxLayout(gameDetailsWidget);

    QLabel* genresLabel = new QLabel("Genres: " + game.genre);
    styleLabel(genresLabel);
    detailsLayout->addWidget(genresLabel);

    QLabel* releaseDateLabel = new QLabel("Release Date: " + game.releaseDate);
    styleLabel(releaseDateLabel);
    detailsLayout->addWidget(releaseDateLabel);

    QLabel* developersLabel = new QLabel("Developers: " + game.developer);
    styleLabel(developersLabel);
    detailsLayout->addWidget(developersLabel);

    QLabel* publishersLabel = new QLabel ("Publishers: " + game.publisher);
    styleLabel(publishersLabel);
    detailsLayout->addWidget(publishersLabel);

    widgetLayout->addWidget(gameDetailsWidget);

    QPushButton* playButton = new QPushButton();
    playButton->setStyleSheet("background-image: url(:res/icon/Material/Launch.svg);"
                              "background-repeat: no-repeat;"
                              "background-color: transparent;");
    playButton->setMinimumSize(40, 40);
    widgetLayout->addWidget(playButton);

    this->setLayout(widgetLayout);
}

void DetailGameWidget::enterEvent(QEvent* event)
{
    this->currentlyHoveredOver = true;
    this->setCursor(Qt::PointingHandCursor);
}

void DetailGameWidget::leaveEvent(QEvent* event)
{
    this->currentlyHoveredOver = false;
    this->setCursor(Qt::ArrowCursor);
}

/**
  Applies default font and color styling to a label for display
  @param label The label to style
  */
void DetailGameWidget::styleLabel(QLabel *label){
    label->setStyleSheet("font-family: Roboto; font-weight: 300; font-size: 10pt");
}

void DetailGameWidget::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    painter.fillRect(0, 0, this->width(), this->height(), QBrush(QColor(0, 0, 0, 80)));
}
