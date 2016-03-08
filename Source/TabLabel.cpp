#include "TabLabel.h"
#include <QPainter>
#include <QDebug>
#include <QColor>

TabLabel::TabLabel(QWidget* parent = 0, QSettings* palette = 0) : QLabel(parent)
{
    this->palette = palette;
    this->isActive = false;
    this->lineColor = palette->value("Navbar/SelectedColor").toString();
}

void TabLabel::paintEvent(QPaintEvent* event)
{
    QLabel::paintEvent(event);
    QColor color(Qt::white);
    if (isActive)
    {
        if (!palette->value("TitleBar/Fulltab").toBool())
        {
            color.setNamedColor(lineColor);
        }

        QLabel::paintEvent(event);
        QPainter painter(this);

        QPen pen(color, 2, Qt::SolidLine);
        painter.setPen(pen);
        painter.drawLine(0,this->height() - 2, this->width(), this->height() - 2);
    }
}
