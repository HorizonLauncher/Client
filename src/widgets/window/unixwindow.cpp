#include "unixwindow.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QVBoxLayout>
#include <QMouseEvent>

UnixWindow::UnixWindow()
{
    setObjectName("unixWindow");

    QSettings palette(QSettings::IniFormat, QSettings::UserScope, "HorizonLauncher", "palette");
    customTB = palette.value("TitleBar/CustomTitleBar").toBool();

    mainPanel = new UnixPanel(this);

    QVBoxLayout* layout = new QVBoxLayout();
    layout->setMargin(0);
    setLayout(layout);
    layout->addWidget(mainPanel);

    if (customTB)
    {
        this->setWindowFlags(Qt::FramelessWindowHint);
    }

    mainPanel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // Center window at runtime
    QRect rec = QApplication::desktop()->screenGeometry();
    int width = 1152, height = 648, screenWidth = rec.width(), screenHeight = rec.height();
    int offsetX = (screenWidth - width) / 2;
    int offsetY = (screenHeight - height) / 2;

    this->resize(width, height);
    this->move(offsetX, offsetY);
    this->show();
}

void UnixWindow::minimize()
{
    showMinimized();
}

void UnixWindow::maximize()
{
    if (isMaximized())
    {
        showNormal();
    }
    else
    {
        showMaximized();
    }
}

void UnixWindow::closeWindow()
{
    this->close();
}

void UnixWindow::mousePressEvent(QMouseEvent* evt)
{
    if (!customTB) { return; }
    oldWindowPos = evt->globalPos();
    if (evt->pos().y() < 32)
    {
        if (evt->button() == Qt::LeftButton)
        {
            dragging = true;
        }
    }
}

void UnixWindow::mouseDoubleClickEvent(QMouseEvent* evt)
{
    if (!customTB) { return; }
    if (evt->pos().y() < 32)
    {
        if (evt->button() == Qt::LeftButton)
        {
            maximize();
        }
    }
}

void UnixWindow::mouseReleaseEvent(QMouseEvent* evt)
{
    dragging = false;
}

void UnixWindow::mouseMoveEvent(QMouseEvent* evt)
{
    if (!customTB) { return; }
    const QPoint c_delta = evt->globalPos() - oldWindowPos;
    if (dragging)
    {
        dragging = true;
        move(x() + c_delta.x(), y() + c_delta.y());
        oldWindowPos = evt->globalPos();
    }
}
