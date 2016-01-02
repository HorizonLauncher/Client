#include "NewsFeedChooserWindow.h"
#include <QPushButton>
#include <QLabel>
#include <QCloseEvent>
#include <QtWidgets>

NewsFeedChooserWindow::NewsFeedChooserWindow(QWidget *parent) : QDialog(parent)
{
    layout = new QVBoxLayout();
    this->setLayout(layout);

    labels = new QWidget(this);
    labelLayout = new QVBoxLayout();
    labels->setLayout(labelLayout);
    this->loadURLsFromSettings();
    this->createURLLabels();

    QPushButton* addURLButton = new QPushButton("Add");
    layout->addWidget(labels);
    layout->addWidget(addURLButton);
    connect(addURLButton, &QPushButton::clicked, this, &NewsFeedChooserWindow::onAddURLButtonClicked);
}

void NewsFeedChooserWindow::loadURLsFromSettings()
{
    QSettings settings ("HorizonLauncher", "news");
    int size = settings.beginReadArray ("URLs");
    for (int i = 0; i < size; ++i)
    {
        settings.setArrayIndex(i);
        QString current = settings.value("url").toString();
        urls.append(current);
    }
}

void NewsFeedChooserWindow::saveURLs()
{
    QSettings settings ("HorizonLauncher", "news");
    settings.beginWriteArray("URLs");

    for (int i = 0; i < urls.size(); ++i)
    {
        settings.setArrayIndex(i);
        settings.setValue("url", urls[i]);
    }

    settings.endArray();
}

void NewsFeedChooserWindow::onAddURLButtonClicked()
{
    bool error;
    QString newUrl = QInputDialog::getText(this, tr("Add a new URL"), tr("URL: "));

    if (!error && !newUrl.isEmpty())
    {
        newUrls.append(newUrl);
        urls.append(newUrl);
        this->labelLayout->addWidget(new QLabel(newUrl));
    }
}

void NewsFeedChooserWindow::createURLLabels()
{
    for (int i = 0; i < this->labelWidgets.size(); i++)
    {
        QWidget* widget = this->labelWidgets[i];
        this->labelLayout->removeWidget(widget);
        widget->deleteLater();
    }
    this->labelWidgets.clear();

    for (int i = 0; i < this->urls.size(); ++i)
    {
        QPixmap cross(":/SystemMenu/Icons/cross.png");
        QIcon crossIcon(cross);

        QWidget* labelWidget = new QWidget();
        QHBoxLayout* hLabelLayout = new QHBoxLayout(labelWidget);
        hLabelLayout->addWidget(new QLabel(this->urls[i]));

        QPushButton* deleteBtn = new QPushButton();
        deleteBtn->setIcon(crossIcon);
        deleteBtn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        hLabelLayout->addWidget(deleteBtn);
        connect(deleteBtn, &QPushButton::clicked, [=]
        {
            this->urls.removeAll(this->urls[i]);
            this->saveURLs();
            this->createURLLabels();
        });

        this->labelLayout->addWidget(labelWidget);
        this->labelWidgets.append(labelWidget);
    }
}

void NewsFeedChooserWindow::closeEvent(QCloseEvent* event)
{
    this->saveURLs();
    emit shouldRefreshRSSFeeds();
    event->accept();
}
