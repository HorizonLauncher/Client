#include "NewsFeedChooserWindow.h"

#include <QPushButton>
#include <QLabel>
#include <QCloseEvent>
#include <QtWidgets>
#include <QMessageBox>

NewsFeedChooserWindow::NewsFeedChooserWindow(QWidget *parent) : QDialog(parent)
{
    layout = new QVBoxLayout();
    this->setLayout(layout);

    labels = new QWidget(this);
    labelLayout = new QVBoxLayout();
    labels->setLayout(labelLayout);
    this->loadURLsFromSettings();
    this->createURLLabels();

    layout->addWidget(labels);

    QWidget* buttonsWidget = new QWidget();
    layout->addWidget(buttonsWidget);

    QHBoxLayout* buttonsLayout = new QHBoxLayout(buttonsWidget);

    QPushButton* addURLButton = new QPushButton("Add");
    buttonsLayout->addWidget(addURLButton);
    connect(addURLButton, &QPushButton::clicked, this, &NewsFeedChooserWindow::onAddURLButtonClicked);

    QPushButton* addSubredditButton = new QPushButton("Add subreddit");
    buttonsLayout->addWidget(addSubredditButton);
    connect(addSubredditButton, &QPushButton::clicked, this, &NewsFeedChooserWindow::onAddSubredditButtonClicked);
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

void NewsFeedChooserWindow::onAddSubredditButtonClicked()
{
    QString newSubreddit = QInputDialog::getText(this, tr("Add a new subreddit"), tr("/r/"));

    if (!newSubreddit.isEmpty())
    {
        // https://github.com/reddit/reddit/blob/fc2841ad2d45bb48433da4481046291a7f9394f4/r2/r2/models/subreddit.py#L385
        QRegExp subredditRegex("^[A-Za-z0-9][A-Za-z0-9_]{2,20}$");

        if (newSubreddit.contains(subredditRegex))
        {
            QString urlString = "https://www.reddit.com/r/" + newSubreddit + "/.rss";
            qDebug() << urlString;

            newUrls.append(urlString);
            urls.append(urlString);
            this->labelLayout->addWidget(new QLabel("/r/" + newSubreddit));
        }
        else
        {
            QMessageBox errorBox;
            errorBox.setText("Invalid subreddit name!");
            errorBox.setIcon(QMessageBox::Warning);
            errorBox.exec();
        }
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
        QPixmap cross(":/Resource/Icons/Material/delete.svg");
        QIcon crossIcon(cross);

        QWidget* labelWidget = new QWidget();
        QHBoxLayout* hLabelLayout = new QHBoxLayout(labelWidget);

        QRegExp getSubreddit("^[\\w\\d]+:\\/\\/[\\w\\d]+\\.reddit\\.com\\/r\\/(.+)\\/.rss$");
        if (this->urls[i].contains(getSubreddit))
        {
            getSubreddit.indexIn(this->urls[i]);
            hLabelLayout->addWidget(new QLabel("/r/" + getSubreddit.cap(1)));
        }
        else
        {
            hLabelLayout->addWidget(new QLabel(this->urls[i]));
        }

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
