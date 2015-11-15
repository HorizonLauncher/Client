#include "NewsFeedChooserWindow.h"

#include <QVBoxLayout>
#include <QPushButton>
#include <QApplication>
#include <QLabel>
#include <QCloseEvent>
#include <QtWidgets>

NewsFeedChooserWindow::NewsFeedChooserWindow(QWidget *parent) : QDialog(parent)
{
    layout = new QVBoxLayout();
    this->setLayout(layout);
    newsSettingsFilePath = QApplication::applicationDirPath().left(1) + "news.ini";
    this->loadURLsFromSettings();
    this->createURLLabels();

    labels = new QWidget(this);
    labelLayout = new QVBoxLayout();
    labels->setLayout(labelLayout);
    QPushButton* addURLButton = new QPushButton("Add");
    layout->addWidget(labels);
    layout->addWidget(addURLButton);
    connect(addURLButton, &QPushButton::clicked, this, &NewsFeedChooserWindow::onAddURLButtonClicked);
}

void NewsFeedChooserWindow::loadURLsFromSettings()
{
    QSettings settings (newsSettingsFilePath, QSettings::NativeFormat);
    int size = settings.beginReadArray ("URLs");

    for (int i = 0; i < size; ++i) {
        settings.setArrayIndex(i);
        QString current = settings.value("url").toString();
        urls.append(current);
    }
}

void NewsFeedChooserWindow::saveURLs()
{
    QSettings settings (newsSettingsFilePath, QSettings::NativeFormat);
    settings.beginWriteArray("URLs");

    for (int i = 0; i < newUrls.size(); ++i) {
        settings.setArrayIndex(i);
        settings.setValue("url", newUrls[i]);
    }
}

void NewsFeedChooserWindow::onAddURLButtonClicked() {

    bool error;
    QString newUrl = QInputDialog::getText(this, tr("Add a new URL"), tr("URL: "));

    if (!error && !newUrl.isEmpty()) {
        newUrls.append(newUrl);
        urls.append(newUrl);
        this->labelLayout->addWidget(new QLabel(newUrl));
    }
}

void NewsFeedChooserWindow::createURLLabels() {

    for (auto url : this->urls) {
        this->labelLayout->addWidget(new QLabel(url));
    }
}

void NewsFeedChooserWindow::closeEvent(QCloseEvent* event)
{
    this->saveURLs();
    event->accept();
}
