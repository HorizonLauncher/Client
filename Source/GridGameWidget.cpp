#include "GridGameWidget.h"

#include <QLabel>
#include <QBoxLayout>

GridGameWidget::GridGameWidget(QString gameName, int hours, QWidget* parent)
    : QWidget(parent)
{
    this->setMinimumSize(270, 170);
    this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    QGridLayout* coreLayout = new QGridLayout(this);

    QFrame* backgroundWidget = new QFrame();
    backgroundWidget->setStyleSheet("background-image: url(:SystemMenu/Images/LibraryGridPlaceholder.png);");
    backgroundWidget->setMinimumSize(270, 170);
    coreLayout->addWidget(backgroundWidget, 0, 0);

    QVBoxLayout* backgroundLayout = new QVBoxLayout(backgroundWidget);
    backgroundLayout->setSpacing(0);
    backgroundLayout->setMargin(0);
    backgroundLayout->setAlignment(Qt::AlignBottom);

    QWidget *infoBar = new QWidget();
    infoBar->setStyleSheet("background: rgba(16, 16, 16, 0.80);");
    infoBar->setMinimumSize(270, 70);
    infoBar->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    backgroundLayout->addWidget(infoBar, Qt::AlignBottom);

    QVBoxLayout* otherLayout = new QVBoxLayout(infoBar);

    otherLayout->setSpacing(0);
    otherLayout->setMargin(20);
    otherLayout->addStretch(120);

    QLabel* gameNameLabel = new QLabel(gameName);
    gameNameLabel->setStyleSheet("font-size: 18px; background-color: transparent;");
    gameNameLabel->setWordWrap(true);
    gameNameLabel->raise();
    otherLayout->addWidget(gameNameLabel, Qt::AlignBottom | Qt::AlignLeft);

    QLabel* hoursLabel = new QLabel(QString::number(hours) + " hours");
    hoursLabel->setStyleSheet("font-size: 12px; background-color: transparent;");
    hoursLabel->setContentsMargins(0, 0, 0, 0);
    hoursLabel->raise();
    otherLayout->addWidget(hoursLabel, Qt::AlignBottom | Qt::AlignLeft);
}

/** Overridden mouse press event.
* Emits the click signal for this tab.
*/
void GridGameWidget::mousePressEvent(QMouseEvent* event)
{
    Q_EMIT clicked();
    QWidget::mousePressEvent(event);
}
