#include "gridgamewidget.h"

#include <QLabel>
#include <QBoxLayout>
#include <QMouseEvent>
#include <QMenu>

GridGameWidget::GridGameWidget(QString gameName, int hours, QString background, QWidget* parent)
    : QWidget(parent)
{
    this->setMinimumSize(364, 170);
    this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    QGridLayout* coreLayout = new QGridLayout(this);

    QFrame* backgroundWidget = new QFrame();
    backgroundWidget->setStyleSheet("background-image: url(" + background + ");"
                                    "background-repeat: none;"
                                    "background-color: #232323;");
    backgroundWidget->setMinimumSize(364, 170);
    coreLayout->addWidget(backgroundWidget, 0, 0);

    QVBoxLayout* backgroundLayout = new QVBoxLayout(backgroundWidget);
    backgroundLayout->setSpacing(0);
    backgroundLayout->setMargin(0);
    backgroundLayout->setAlignment(Qt::AlignBottom);

    QWidget *infoBar = new QWidget();
    infoBar->setStyleSheet("background: rgba(16, 16, 16, 0.80);");
    infoBar->setMinimumSize(364, 70);
    infoBar->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    backgroundLayout->addWidget(infoBar, Qt::AlignBottom);

    QVBoxLayout* otherLayout = new QVBoxLayout(infoBar);

    otherLayout->setSpacing(0);
    otherLayout->setMargin(20);
    otherLayout->addStretch(120);

    QLabel* gameNameLabel = new QLabel(gameName);
    gameNameLabel->setStyleSheet("font-size: 18px; background-color: transparent;");
    otherLayout->addWidget(gameNameLabel, Qt::AlignBottom | Qt::AlignLeft);

    QLabel* hoursLabel = new QLabel(QString::number(hours) + " hours");
    hoursLabel->setStyleSheet("font-size: 12px; background-color: transparent;");
    hoursLabel->setContentsMargins(0, 0, 0, 0);
    hoursLabel->setWordWrap(true);
    otherLayout->addWidget(hoursLabel, Qt::AlignBottom | Qt::AlignLeft);

    this->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this,
        &GridGameWidget::customContextMenuRequested,
        this, &GridGameWidget::showContextMenu);
}

/** Overridden mouse press event.
* Emits the click signal for this tab.
*/
void GridGameWidget::mousePressEvent(QMouseEvent* event)
{
    emit clicked();
    if (event->button() == Qt::LeftButton)
    {
        emit leftClick();
    }
    else if (event->button() == Qt::RightButton)
    {
        emit rightClick();
    }
    QWidget::mousePressEvent(event);
}

/** Slot to show the right click (context) menu */
void GridGameWidget::showContextMenu(const QPoint& pos)
{
    QPoint globalPos = mapToGlobal(pos);

    QMenu contextMenu;
    contextMenu.addAction(tr("Change game details"));
    contextMenu.addAction(tr("Remove game from Library"));

    QAction* selected = contextMenu.exec(globalPos);
    if (selected)
    {
        if (selected->text() == tr("Change game details"))
        {
            emit openPropertyEditor();
        }
        else if (selected->text() == tr("Remove game from Library"))
        {
            emit removeGame();
        }
    }
}
