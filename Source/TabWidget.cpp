#include "TabWidget.h"
#include "TabLabel.h"
#include <QLayout>
#include <QPainter>
#include <QStyleOption>
#include <QDebug>

/** TabWidget constructor
* Initialize the layout of the tab, as well as colorization settings.
* \param icon The pixmap (icon) for this tab.
* \param name The desired object name for this tab.
* \param text The text written on this tab.
* \param palette Inherited palette configuration for setting StyleSheets.
* \param parent Pointer to parent widget.
*/
TabWidget::TabWidget(const QString &name, const QString &text, QSettings* palette, QWidget* parent)
    : QWidget(parent)
{
    this->setObjectName(name);
    this->setMinimumHeight(34);
    this->setMaximumHeight(34);
    this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    //this->setStyleSheet("border-top-left-radius: 2px; border-top-right-radius: 2px;");

    p = palette;

    // Toggle mouse tracking
    this->setAttribute(Qt::WA_Hover, true);
    this->setMouseTracking(true);

    // Background layout
    QGridLayout* bgLayout = new QGridLayout;
    bgLayout->setMargin(0);
    bgLayout->setSpacing(0);
    this->setLayout(bgLayout);

    // Content widget
    QWidget* contentWidget = new QWidget(this);
    contentWidget->setObjectName("contentWidget");
    bgLayout->addWidget(contentWidget);

    // Horizontal layout
    QHBoxLayout* tabLayout = new QHBoxLayout;
    tabLayout->setMargin(5);
    tabLayout->setSpacing(10);
    contentWidget->setLayout(tabLayout);

    QFont font = QFont("Raleway", 10);
    font.setBold(true);

    // Tab text
    tabText = new TabLabel(this, palette);
    tabText->setObjectName("tabText");
    tabText->setStyleSheet("color: #7d8f94;");
    tabText->setFont(font);
    tabText->setText(text);
    tabText->setAlignment(Qt::AlignCenter);
    tabText->setContentsMargins(5, 0, 5, 0);
    tabLayout->addWidget(tabText);

    // Hover colorize effect
    opacity = 0;
    effect = new QGraphicsColorizeEffect();
    effect->setColor(QColor(p->value("Navbar/HoverColor").toString()));
    effect->setStrength(opacity);
    contentWidget->setGraphicsEffect(effect);

    // Colorize animation
    animation = new QPropertyAnimation(this, "opacity");
    animation->setDuration(100);
    animation->setEasingCurve(QEasingCurve::Linear);

    // Effect signals
    connect(this, &TabWidget::hovered, this, &TabWidget::toggleHovered);
    connect(this, &TabWidget::unhovered, this, &TabWidget::toggleUnhovered);
}

/** Overridden enter event.
* Emits the hover signal for this tab.
*/
void TabWidget::enterEvent(QEvent* event)
{
    Q_EMIT hovered();
    QWidget::enterEvent(event);
}

/** Overridden leave event.
* Emits the unhover signal for this tab.
*/
void TabWidget::leaveEvent(QEvent* event)
{
    Q_EMIT unhovered();
    QWidget::leaveEvent(event);
}

/** Overridden mouse press event.
* Emits the click signal for this tab.
*/
void TabWidget::mousePressEvent(QMouseEvent* event)
{
    Q_EMIT clicked();
    QWidget::mousePressEvent(event);
}

/** Overridden paint event.
* Necessary for displaying stylesheets correctly.
* \param event The QPaintEvent trigger.
*/
void TabWidget::paintEvent(QPaintEvent* event)
{
    // Default paint event
    QWidget::paintEvent(event);
    QPainter p(this);
    QStyleOption option;
    option.init(this);
    style()->drawPrimitive(QStyle::PE_Widget, &option, &p, this);

}

/** Starts the hover colorization animation.
*/
void TabWidget::toggleHovered()
{
    if (!isActive)
    {
        animation->stop();
        animation->setStartValue(opacity);
        animation->setEndValue(1.0);
        animation->start();
    }
}

/** Starts the unhover colorization animation.
*/
void TabWidget::toggleUnhovered()
{
    if (!isActive)
    {
        animation->stop();
        animation->setStartValue(opacity);
        animation->setEndValue(0.0);
        animation->start();
    }
}

/** Sets this tab active on the navbar.
*/
void TabWidget::toggleActive()
{
    isActive = true;

    this->update();
    if (p->value("TitleBar/Fulltab").toBool())
    {
        effect->setColor(QColor(p->value("Navbar/SelectedColor").toString()));
    }

    else {
        effect->setColor(QColor(p->value("Navbar/BackgroundColor").toString()));
    }

    this->setStyleSheet("border-top-left-radius: 2px;"
                        "border-top-right-radius: 2px;"
                        "background-color: #000000;");
    tabText->setStyleSheet("color: #ffffff;");
    setOpacity(1.0);
    tabText->isActive = true;
    tabText->update();
}

/** Sets this tab inactive on the navbar.
*/
void TabWidget::toggleInactive()
{
    isActive = false;
    effect->setColor(QColor(p->value("Navbar/HoverColor").toString()));
    this->setStyleSheet("border-top-left-radius: 2px;"
                        "border-top-right-radius: 2px;"
                        "background-color: " + p->value("Navbar/Background").toString() + ";");
    tabText->setStyleSheet("color: #7d8f94");
    tabText->isActive = false;
    setOpacity(0.0);
}
