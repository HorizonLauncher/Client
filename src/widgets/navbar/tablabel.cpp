#include "tablabel.h"
#include <QPainter>
#include <QColor>

TabLabel::TabLabel(QWidget* parent = 0, QSettings* palette = 0) : QLabel(parent)
{
    this->palette = palette;
    this->isActive = false;
}

void TabLabel::paintEvent(QPaintEvent* event)
{
    QLabel::paintEvent(event);
    QColor color(Qt::white);
    if (isActive)
    {
        if (!palette->value("TitleBar/Fulltab").toBool())
        {
            color.setNamedColor(palette->value("Navbar/SelectedColor").toString());
        }

        QLabel::paintEvent(event);
        QPainter painter(this);
        painter.setBrush(QBrush(color));
        painter.drawLine(0,this->height() - 2, this->width(), this->height() - 2);
    }
}
